Set-StrictMode -Version Latest

function 解析-项目根目录 {
    param([string]$路径)
    if ($路径) {
        return [System.IO.Path]::GetFullPath($路径)
    }
    return [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..'))
}

function 导入-VisualStudio开发环境 {
    param(
        [ValidateSet('Win32', 'x64')]
        [string]$平台 = 'x64'
    )

    $现有编译器 = Get-Command cl.exe -ErrorAction SilentlyContinue
    if ($现有编译器 -and $env:INCLUDE) {
        return $现有编译器.Source
    }

    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) {
        throw "找不到 vswhere: $vswhere"
    }
    $安装目录 = (& $vswhere -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath).Trim()
    if (-not $安装目录) {
        throw '没有找到带 MSVC 工具链的 Visual Studio。'
    }

    $vcvars = Join-Path $安装目录 'VC\Auxiliary\Build\vcvarsall.bat'
    if (-not (Test-Path -LiteralPath $vcvars -PathType Leaf)) {
        throw "找不到 vcvarsall.bat: $vcvars"
    }
    $架构 = if ($平台 -eq 'x64') { 'amd64' } else { 'x86' }
    $环境输出 = & cmd.exe /d /s /c "call `"$vcvars`" $架构 >nul && set"
    if ($LASTEXITCODE -ne 0) {
        throw "Visual Studio 开发环境初始化失败，退出码: $LASTEXITCODE"
    }
    foreach ($行 in $环境输出) {
        if ($行 -match '^([^=]+)=(.*)$') {
            [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
        }
    }

    $编译器 = Get-Command cl.exe -ErrorAction Stop
    return $编译器.Source
}

function 读取-工程文档 {
    param([string]$项目根目录)
    $工程文件 = Join-Path $项目根目录 '海中鱼巣.vcxproj'
    if (-not (Test-Path -LiteralPath $工程文件 -PathType Leaf)) {
        throw "找不到工程文件: $工程文件"
    }
    $文档 = New-Object System.Xml.XmlDocument
    $文档.Load($工程文件)
    return $文档
}

function 读取-条件属性 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$配置,
        [string]$平台,
        [string]$节点名
    )
    $命名空间管理器 = New-Object System.Xml.XmlNamespaceManager($文档.NameTable)
    $命名空间管理器.AddNamespace('msb', 'http://schemas.microsoft.com/developer/msbuild/2003')
    $条件 = "'`$(Configuration)|`$(Platform)'=='$配置|$平台'"
    $节点 = $文档.SelectSingleNode(
        "/msb:Project/msb:ItemDefinitionGroup[@Condition=`"$条件`"]/msb:ClCompile/msb:$节点名",
        $命名空间管理器)
    if ($节点) {
        return [string]$节点.InnerText
    }
    return ''
}

function 读取-配置属性 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$配置,
        [string]$平台,
        [string]$节点名
    )
    $命名空间管理器 = New-Object System.Xml.XmlNamespaceManager($文档.NameTable)
    $命名空间管理器.AddNamespace('msb', 'http://schemas.microsoft.com/developer/msbuild/2003')
    $条件 = "'`$(Configuration)|`$(Platform)'=='$配置|$平台'"
    $节点 = $文档.SelectSingleNode(
        "/msb:Project/msb:PropertyGroup[@Condition=`"$条件`"]/msb:$节点名",
        $命名空间管理器)
    if ($节点) {
        return [string]$节点.InnerText
    }
    return ''
}

function 展开-工程属性 {
    param(
        [string]$文本,
        [string]$项目根目录,
        [string]$配置,
        [string]$平台,
        [bool]$启用D455RealSense
    )
    $结果 = $文本
    $项目目录 = $项目根目录.TrimEnd('\') + '\'
    $结果 = $结果.Replace('$(ProjectDir)', $项目目录)
    $结果 = $结果.Replace('$(Configuration)', $配置)
    $结果 = $结果.Replace('$(Platform)', $平台)
    $结果 = $结果.Replace(
        '$(EnableD455RealSense)',
        $(if ($启用D455RealSense) { 'true' } else { 'false' }))
    $结果 = $结果.Replace(
        '$(D455RealSensePreprocessorDefinitions)',
        $(if ($启用D455RealSense -and $平台 -eq 'x64') {
            'HY_EGO_ENABLE_D455_REALSENSE;'
        } else {
            ''
        }))
    foreach ($名称 in @(
        'EntryInitializationSelfTestDebugDefinition',
        'DatabaseSelfTestDebugDefinition',
        'WarehousePerformanceSelfTestDebugDefinition',
        'D455SelfTestDebugDefinition')) {
        $结果 = $结果.Replace("`$($名称)", '')
    }
    $结果 = $结果.Replace('%(PreprocessorDefinitions)', '')
    $结果 = $结果.Replace('%(AdditionalIncludeDirectories)', '')
    $vcpkg根目录 = if ($env:VcpkgRoot) {
        $env:VcpkgRoot
    } elseif ($env:VCPKG_ROOT) {
        $env:VCPKG_ROOT
    } else {
        ''
    }
    $结果 = $结果.Replace('$(D455VcpkgRoot)', $vcpkg根目录)
    return $结果
}

function 读取-工程源文件 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$项目根目录,
        [string[]]$扩展名,
        [string]$配置 = 'Debug',
        [string]$平台 = 'x64',
        [bool]$启用D455RealSense = $false
    )
    $命名空间管理器 = New-Object System.Xml.XmlNamespaceManager($文档.NameTable)
    $命名空间管理器.AddNamespace('msb', 'http://schemas.microsoft.com/developer/msbuild/2003')
    $扩展集合 = [System.Collections.Generic.HashSet[string]]::new(
        [System.StringComparer]::OrdinalIgnoreCase)
    foreach ($值 in $扩展名) {
        [void]$扩展集合.Add($值)
    }
    $结果 = [System.Collections.Generic.List[string]]::new()
    foreach ($节点 in $文档.SelectNodes(
        '/msb:Project/msb:ItemGroup/msb:ClCompile',
        $命名空间管理器)) {
        $相对路径 = [string]$节点.Include
        if (-not $相对路径) {
            continue
        }
        if (-not $扩展集合.Contains([System.IO.Path]::GetExtension($相对路径))) {
            continue
        }
        $排除节点组 = @($节点.SelectNodes('msb:ExcludedFromBuild', $命名空间管理器))
        $已排除 = $false
        foreach ($排除节点 in $排除节点组) {
            if ([string]$排除节点.InnerText -notmatch '^(?i:true)$') {
                continue
            }
            $条件 = [string]$排除节点.GetAttribute('Condition')
            if (-not $条件 -or (测试-工程条件 `
                $条件 $配置 $平台 $启用D455RealSense)) {
                $已排除 = $true
                break
            }
        }
        if ($已排除) {
            continue
        }
        $绝对路径 = [System.IO.Path]::GetFullPath((Join-Path $项目根目录 $相对路径))
        if (Test-Path -LiteralPath $绝对路径 -PathType Leaf) {
            $结果.Add($绝对路径)
        } else {
            Write-Warning "工程登记但本地不存在: $绝对路径"
        }
    }
    return $结果.ToArray()
}

function 测试-工程条件 {
    param(
        [string]$条件,
        [string]$配置,
        [string]$平台,
        [bool]$启用D455RealSense
    )
    $展开条件 = $条件.
        Replace('$(Configuration)', $配置).
        Replace('$(Platform)', $平台).
        Replace(
            '$(EnableD455RealSense)',
            $(if ($启用D455RealSense) { 'true' } else { 'false' }))
    $或项组 = [regex]::Split(
        $展开条件.Trim(),
        '\s+or\s+',
        [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
    foreach ($或项 in $或项组) {
        $且成立 = $true
        $且项组 = [regex]::Split(
            $或项.Trim(),
            '\s+and\s+',
            [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
        foreach ($且项 in $且项组) {
            if ($且项.Trim() -notmatch "^'([^']*)'\s*(==|!=)\s*'([^']*)'$") {
                throw "不支持的工程条件，禁止猜测是否排除源文件: $条件"
            }
            $相等 = [string]::Equals(
                $Matches[1],
                $Matches[3],
                [System.StringComparison]::OrdinalIgnoreCase)
            $当前成立 = if ($Matches[2] -eq '==') { $相等 } else { -not $相等 }
            $且成立 = $且成立 -and $当前成立
        }
        if ($且成立) {
            return $true
        }
    }
    return $false
}

function 形成-宏和目录 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$项目根目录,
        [string]$配置,
        [string]$平台,
        [bool]$启用D455RealSense
    )
    $宏文本 = 展开-工程属性 `
        (读取-条件属性 $文档 $配置 $平台 'PreprocessorDefinitions') `
        $项目根目录 $配置 $平台 $启用D455RealSense
    $宏组 = @($宏文本 -split ';' | Where-Object {
        $_ -and $_ -notmatch '^%\(' -and $_ -notmatch '^\$\('
    })
    $字符集 = 读取-配置属性 $文档 $配置 $平台 'CharacterSet'
    if ($字符集 -eq 'Unicode') {
        $宏组 += 'UNICODE', '_UNICODE'
    } elseif ($字符集 -eq 'MultiByte') {
        $宏组 += '_MBCS'
    }
    $宏组 = @($宏组 | Select-Object -Unique)

    $目录文本 = 展开-工程属性 `
        (读取-条件属性 $文档 $配置 $平台 'AdditionalIncludeDirectories') `
        $项目根目录 $配置 $平台 $启用D455RealSense
    $目录组 = @()
    foreach ($目录 in ($目录文本 -split ';' | Where-Object {
        $_ -and $_ -notmatch '^\$\('
    })) {
        $候选 = if ([System.IO.Path]::IsPathRooted($目录)) {
            $目录
        } else {
            Join-Path $项目根目录 $目录
        }
        if (Test-Path -LiteralPath $候选 -PathType Container) {
            $目录组 += [System.IO.Path]::GetFullPath($候选)
        }
    }
    return [pscustomobject]@{
        宏 = $宏组
        目录 = $目录组
    }
}

function 形成-Clang公共参数 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$项目根目录,
        [string]$配置,
        [string]$平台,
        [bool]$启用D455RealSense,
        [string[]]$额外参数 = @()
    )
    $材料 = 形成-宏和目录 $文档 $项目根目录 $配置 $平台 $启用D455RealSense
    $参数组 = [System.Collections.Generic.List[string]]::new()
    foreach ($参数 in @(
        '-std=c++20',
        '-fms-compatibility',
        '-fms-extensions',
        '-fexceptions',
        "-I$项目根目录")) {
        $参数组.Add($参数)
    }
    foreach ($宏 in $材料.宏) {
        $参数组.Add("-D$宏")
    }
    foreach ($目录 in $材料.目录) {
        $参数组.Add('-I')
        $参数组.Add($目录)
    }
    foreach ($目录 in ($env:INCLUDE -split ';' | Where-Object {
        $_ -and (Test-Path -LiteralPath $_ -PathType Container)
    })) {
        $参数组.Add('-isystem')
        $参数组.Add([System.IO.Path]::GetFullPath($目录))
    }
    foreach ($参数 in $额外参数) {
        $参数组.Add($参数)
    }
    return $参数组.ToArray()
}

function 形成-MSVC公共参数 {
    param(
        [System.Xml.XmlDocument]$文档,
        [string]$项目根目录,
        [string]$配置,
        [string]$平台,
        [bool]$启用D455RealSense
    )
    $材料 = 形成-宏和目录 $文档 $项目根目录 $配置 $平台 $启用D455RealSense
    $参数组 = [System.Collections.Generic.List[string]]::new()
    foreach ($参数 in @('/nologo', '/std:c++20', '/utf-8', '/EHsc')) {
        $参数组.Add($参数)
    }
    $参数组.Add("/I$项目根目录")
    foreach ($宏 in $材料.宏) {
        $参数组.Add("/D$宏")
    }
    foreach ($目录 in $材料.目录) {
        $参数组.Add("/I$目录")
    }
    return $参数组.ToArray()
}

function 解析-候选输出路径 {
    param(
        [string]$输出路径,
        [string]$项目根目录,
        [string]$默认文件名
    )
    if (-not $输出路径) {
        return Join-Path ([System.IO.Path]::GetTempPath()) $默认文件名
    }
    if ([System.IO.Path]::IsPathRooted($输出路径)) {
        return [System.IO.Path]::GetFullPath($输出路径)
    }
    return [System.IO.Path]::GetFullPath(
        (Join-Path ([System.IO.Path]::GetTempPath()) $输出路径))
}

function 断言-不是正式图谱输出 {
    param(
        [string]$输出路径,
        [string]$项目根目录
    )
    $正式图谱目录 = [System.IO.Path]::GetFullPath(
        (Join-Path $项目根目录 '流程图\代码流程图'))
    $目标 = [System.IO.Path]::GetFullPath($输出路径)
    $临时根 = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).
        TrimEnd('\') + '\'
    if (-not $目标.StartsWith(
        $临时根,
        [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "候选输出只允许写入系统临时目录: $目标"
    }
    if ($目标.StartsWith(
        $正式图谱目录.TrimEnd('\') + '\',
        [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "候选输出禁止写入正式图谱目录: $目标"
    }
}
