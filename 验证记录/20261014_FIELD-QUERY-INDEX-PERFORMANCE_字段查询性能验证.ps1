param(
    [Parameter(Mandatory=$true)][string]$Executable,
    [Parameter(Mandatory=$true)][string]$OutputDirectory,
    [int]$TimeoutSeconds = 60,
    [int]$SampleIntervalSeconds = 15
)
$ErrorActionPreference = 'Stop'
# 只驱动实际项目已有自检入口，用系统调试API采集该进程；没有业务替代实现。
Add-Type -TypeDefinition @'
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
public static class FieldQueryStackProbe {
    [DllImport("kernel32.dll", SetLastError=true)] static extern IntPtr OpenProcess(uint access, bool inherit, int id);
    [DllImport("kernel32.dll", SetLastError=true)] static extern IntPtr OpenThread(uint access, bool inherit, int id);
    [DllImport("kernel32.dll", SetLastError=true)] static extern uint SuspendThread(IntPtr thread);
    [DllImport("kernel32.dll", SetLastError=true)] static extern uint ResumeThread(IntPtr thread);
    [DllImport("kernel32.dll", SetLastError=true)] static extern bool GetThreadContext(IntPtr thread, IntPtr context);
    [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr handle);
    [DllImport("kernel32.dll", CharSet=CharSet.Unicode)] static extern IntPtr LoadLibraryW(string name);
    [DllImport("kernel32.dll", CharSet=CharSet.Ansi)] static extern IntPtr GetProcAddress(IntPtr module, string name);
    [DllImport("dbghelp.dll", CharSet=CharSet.Unicode, SetLastError=true)] static extern bool SymInitializeW(IntPtr process, string path, bool invade);
    [DllImport("dbghelp.dll")] static extern bool SymCleanup(IntPtr process);
    [DllImport("dbghelp.dll")] static extern uint SymSetOptions(uint options);
    [DllImport("dbghelp.dll", CharSet=CharSet.Unicode, SetLastError=true)] static extern bool SymFromAddrW(IntPtr process, ulong address, out ulong displacement, IntPtr symbol);
    [DllImport("dbghelp.dll", SetLastError=true)] static extern bool StackWalk64(uint machine, IntPtr process, IntPtr thread, IntPtr frame, IntPtr context, IntPtr read, IntPtr table, IntPtr module, IntPtr translate);
    static string Symbol(IntPtr process, ulong address) {
        var info = Marshal.AllocHGlobal(88 + 2048);
        try {
            Marshal.Copy(new byte[88 + 2048], 0, info, 88 + 2048);
            Marshal.WriteInt32(info, 0, 88); Marshal.WriteInt32(info, 80, 1024);
            ulong offset;
            return SymFromAddrW(process, address, out offset, info)
                ? Marshal.PtrToStringUni(IntPtr.Add(info, 84)) + "+0x" + offset.ToString("x")
                : "0x" + address.ToString("x");
        } finally { Marshal.FreeHGlobal(info); }
    }
    public static string[] Capture(int processId, string symbols) {
        var result = new List<string>();
        var process = OpenProcess(0x410, false, processId);
        if (process == IntPtr.Zero) throw new Win32Exception();
        bool initialized = false;
        try {
            SymSetOptions(0x2 | 0x4 | 0x200 | 0x80000);
            initialized = SymInitializeW(process, symbols, true);
            if (!initialized) throw new Win32Exception();
            var dbg = LoadLibraryW("dbghelp.dll");
            foreach (ProcessThread item in Process.GetProcessById(processId).Threads) {
                var thread = OpenThread(0x5a, false, item.Id);
                if (thread == IntPtr.Zero) continue;
                var block = Marshal.AllocHGlobal(1248);
                var context = new IntPtr((block.ToInt64()+15) & ~15L);
                var frame = Marshal.AllocHGlobal(2048);
                bool suspended = false;
                try {
                    suspended = SuspendThread(thread) != uint.MaxValue;
                    if (!suspended) continue;
                    Marshal.Copy(new byte[1232], 0, context, 1232);
                    Marshal.WriteInt32(context, 48, 0x10001f);
                    if (!GetThreadContext(thread, context)) throw new Win32Exception();
                    Marshal.Copy(new byte[2048], 0, frame, 2048);
                    Marshal.WriteInt64(frame, 0, Marshal.ReadInt64(context, 248)); Marshal.WriteInt32(frame, 12, 3);
                    Marshal.WriteInt64(frame, 32, Marshal.ReadInt64(context, 160)); Marshal.WriteInt32(frame, 44, 3);
                    Marshal.WriteInt64(frame, 48, Marshal.ReadInt64(context, 152)); Marshal.WriteInt32(frame, 60, 3);
                    result.Add("thread="+item.Id);
                    for (int i=0; i<20; i++) {
                        ulong pc = unchecked((ulong)Marshal.ReadInt64(frame));
                        if (pc==0) break;
                        result.Add(Symbol(process, pc));
                        if (!StackWalk64(0x8664, process, thread, frame, context, IntPtr.Zero,
                            GetProcAddress(dbg, "SymFunctionTableAccess64"), GetProcAddress(dbg, "SymGetModuleBase64"), IntPtr.Zero)) break;
                    }
                } finally {
                    if (suspended) ResumeThread(thread);
                    Marshal.FreeHGlobal(block); Marshal.FreeHGlobal(frame); CloseHandle(thread);
                }
            }
        } finally { if (initialized) SymCleanup(process); CloseHandle(process); }
        return result.ToArray();
    }
}
'@
$taskExecutable = (Resolve-Path -LiteralPath $Executable).Path
$taskDirectory = (New-Item -ItemType Directory -Path $OutputDirectory -Force).FullName
$taskWatch = [Diagnostics.Stopwatch]::StartNew()
$taskProcess = Start-Process -FilePath $taskExecutable -ArgumentList '--self-test-method-domain' -WorkingDirectory (Split-Path -Parent $taskExecutable) -WindowStyle Hidden -PassThru -RedirectStandardOutput (Join-Path $taskDirectory 'stdout.txt') -RedirectStandardError (Join-Path $taskDirectory 'stderr.txt')
$taskSamples = @()
$taskTimedOut = $false
try {
    while (-not $taskProcess.WaitForExit([Math]::Min($SampleIntervalSeconds, $TimeoutSeconds)*1000)) {
        $taskProcess.Refresh()
        $taskFrames = [FieldQueryStackProbe]::Capture($taskProcess.Id, (Split-Path -Parent $taskExecutable))
        $taskSample = [pscustomobject]@{ elapsed=$taskWatch.Elapsed.TotalSeconds; cpu=$taskProcess.TotalProcessorTime.TotalSeconds; frames=$taskFrames }
        $taskSamples += $taskSample
        $taskSample | ConvertTo-Json -Depth 5 -Compress
        if ($taskWatch.Elapsed.TotalSeconds -ge $TimeoutSeconds) { $taskTimedOut=$true; break }
    }
    $taskProcess.Refresh()
    $taskCpu = $taskProcess.TotalProcessorTime.TotalSeconds
    if ($taskTimedOut -and -not $taskProcess.HasExited) { Stop-Process -Id $taskProcess.Id -Force; $taskProcess.WaitForExit() }
    $taskWatch.Stop()
    [pscustomobject]@{ executable=$taskExecutable; pid=$taskProcess.Id; elapsed=$taskWatch.Elapsed.TotalSeconds; cpu=$taskCpu; timedOut=$taskTimedOut; exitCode=$taskProcess.ExitCode; samples=$taskSamples.Count; tail=@(Get-Content -LiteralPath (Join-Path $taskDirectory 'stdout.txt') -Tail 8); errors=@(Get-Content -LiteralPath (Join-Path $taskDirectory 'stderr.txt') -Tail 5) } | ConvertTo-Json -Depth 5 -Compress
} finally {
    if (-not $taskProcess.HasExited) { Stop-Process -Id $taskProcess.Id -Force; $taskProcess.WaitForExit() }
    $taskProcess.Dispose()
}
