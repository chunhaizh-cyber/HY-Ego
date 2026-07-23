# 文件规则：本工具只从施工流程图和当前代码生成文档基线；不得修改代码或计划索引，不得把未匹配施工节点声明为已实现。
from __future__ import annotations

import argparse
import html
import re
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path


根目录 = Path(__file__).resolve().parents[1]
流程图目录 = 根目录 / "流程图"
输出目录 = 流程图目录 / "现状流程图"
核查目录 = 流程图目录 / "现状流程图核查"

排除名称片段 = ("迁移模板", "现状流程图")
证据图名称 = {"20260710_旧鱼巢自我线程逻辑提取流程图_v0.1.md"}

未实现主题 = {
    "需求父子原子挂载与重挂": "当前没有需求父子原子挂载与重挂正式实现，#192 仍为依赖门控计划。",
    "概念命名用途与生命周期治理": "当前 S6-S8 尚未实施；现有代码只到活动概念图和抽象树只读投影。",
    "结构化事件段持久化与只读校验": "当前没有结构化事件段持久化和只读校验正式入口，#200 仍是待执行计划。",
}

部分实现主题 = {
    "控制面板数据库重建候选",
    "真实闭环串联优先目标",
    "海中鱼巣自我治理循环承载边界",
    "鱼巢信息逻辑迁移",
    "概念图自动生长与抽象关系树形视图",
    "特征值系统第二批代码实施",
}

主题代码提示 = {
    "仓库快照": ["海中鱼巣/核心/仓库快照服务.h"],
    "动态记录": ["海中鱼巣/领域/动态服务.h", "海中鱼巣/领域/状态服务.h"],
    "方法候选": ["海中鱼巣/领域/方法服务.h", "海中鱼巣/领域/任务服务.h"],
    "方法结构": ["海中鱼巣/领域/方法服务.h"],
    "方法执行": ["海中鱼巣/领域/方法服务.h", "海中鱼巣/领域/特征服务.h"],
    "非权威缓存": ["海中鱼巣/领域/统计服务.h"],
    "基础信息入账": ["海中鱼巣/领域/世界服务.h"],
    "控制面板数据库": ["海中鱼巣/领域/控制面板服务.h", "海中鱼巣/适配/SQL数据库适配.h"],
    "轻量因果": ["海中鱼巣/领域/因果服务.h"],
    "任务承接": ["海中鱼巣/领域/任务服务.h", "海中鱼巣/领域/需求服务.h"],
    "任务回执": ["海中鱼巣/领域/任务服务.h", "海中鱼巣/线程/任务管理上行桥.ixx"],
    "任务状态机": ["海中鱼巣/领域/任务服务.h", "海中鱼巣/线程/任务管理线程.ixx", "海中鱼巣/线程/任务工作线程.ixx"],
    "事件日志": ["海中鱼巣/核心/日志系统.h", "海中鱼巣/线程/事件日志线程.ixx"],
    "特征与状态": ["海中鱼巣/领域/特征服务.h", "海中鱼巣/领域/状态服务.h"],
    "外部材料语素": ["海中鱼巣/领域/语素服务.h"],
    "显示层": ["海中鱼巣/领域/显示服务.h", "海中鱼巣/领域/控制面板服务.h"],
    "需求创建": ["海中鱼巣/领域/需求服务.h"],
    "需求结算": ["海中鱼巣/领域/需求服务.h"],
    "需求树": ["海中鱼巣/领域/需求服务.h", "海中鱼巣/核心/关系仓库.h"],
    "需求父子": ["海中鱼巣/领域/需求服务.h", "海中鱼巣/核心/关系仓库.h"],
    "语素": ["海中鱼巣/领域/语素服务.h"],
    "运行宿主": ["海中鱼巣/线程/运行宿主线程.ixx", "海中鱼巣/线程/自我线程.ixx"],
    "总入口": ["海中鱼巣/入口.cpp"],
    "控制面板功能": ["海中鱼巣/领域/控制面板服务.h", "海中鱼巣/界面/控制面板窗口.cpp"],
    "运行宿主与多线程": ["海中鱼巣/线程/运行宿主线程.ixx", "海中鱼巣/线程/有界运行消息队列.ixx"],
    "真实闭环": ["海中鱼巣/线程/自我治理领域路由.ixx", "海中鱼巣/领域/需求服务.h"],
    "自我治理循环": ["海中鱼巣/线程/自我线程.ixx", "海中鱼巣/线程/自我治理领域路由.ixx"],
    "控制面板原生": ["海中鱼巣/界面/控制面板窗口.cpp", "海中鱼巣/领域/控制面板服务.h"],
    "控制台启动": ["海中鱼巣/入口.cpp"],
    "每线程独立": ["海中鱼巣/线程/运行宿主线程.ixx", "海中鱼巣/线程/任务管理线程.ixx", "海中鱼巣/线程/任务工作线程.ixx", "海中鱼巣/线程/事件日志线程.ixx", "海中鱼巣/线程/缓存统计线程.ixx", "海中鱼巣/线程/外设采样材料线程.ixx"],
    "鱼巢信息逻辑迁移": ["海中鱼巣/入口.cpp"],
    "自我线程系统初始化": ["海中鱼巣/线程/自我线程.ixx", "海中鱼巣/领域/初始化.需求.ixx"],
    "特征值系统第二轮第一批": ["海中鱼巣/领域/特征值服务.h", "海中鱼巣/领域/特征服务.h"],
    "特征值系统第二批": ["海中鱼巣/领域/特征值服务.h", "海中鱼巣/领域/特征服务.h"],
    "结构审计投影与控制面板查看": ["海中鱼巣/适配/SQL数据库适配.h", "海中鱼巣/领域/控制面板服务.h", "海中鱼巣/界面/控制面板窗口.cpp"],
    "SQLServer": ["海中鱼巣/适配/SQL数据库适配.h", "海中鱼巣/领域/控制面板服务.h", "海中鱼巣/界面/控制面板窗口.cpp"],
    "概念命名": ["海中鱼巣/领域/概念图服务.h"],
    "概念图自动": ["海中鱼巣/领域/概念图服务.h", "海中鱼巣/领域/概念图算法.h"],
    "控制面板六类": ["海中鱼巣/领域/控制面板服务.h", "海中鱼巣/界面/控制面板窗口.cpp"],
}


@dataclass(frozen=True)
class 函数证据:
    所有者: str
    函数名: str
    文件: Path
    起始行: int
    结束行: int


def 当前提交() -> str:
    return subprocess.check_output(["git", "rev-parse", "--short", "HEAD"], cwd=根目录, text=True).strip()


def 当前脏文件() -> list[str]:
    输出 = subprocess.check_output(
        ["git", "-c", "core.quotePath=false", "status", "--short"],
        cwd=根目录,
        text=True,
        encoding="utf-8",
    )
    路径组 = [行[3:].strip().strip('"') for 行 in 输出.splitlines() if 行.strip()]
    return [路径 for 路径 in 路径组 if 路径.startswith("海中鱼巣/")]


def 提取Mermaid(文本: str) -> str | None:
    匹配 = re.search(r"(?:```|~~~)mermaid\s*\n(.*?)\n(?:```|~~~)", 文本, re.S)
    return 匹配.group(1).rstrip() if 匹配 else None


def 提取函数标记(图: str) -> list[tuple[str, str]]:
    结果: list[tuple[str, str]] = []
    for 所有者, 函数名 in re.findall(r"([\u4e00-\u9fffA-Za-z0-9_]+)::([\u4e00-\u9fffA-Za-z0-9_]+)", 图):
        if 所有者 in {"std", "流程图"}:
            continue
        项 = (所有者, 函数名)
        if 项 not in 结果:
            结果.append(项)
    return 结果


def 代码文件组() -> list[Path]:
    结果: list[Path] = []
    for 后缀 in ("*.h", "*.cpp", "*.ixx"):
        结果.extend((根目录 / "海中鱼巣").rglob(后缀))
    return sorted(结果)


def 查找函数范围(所有者: str, 函数名: str, 文件组: list[Path]) -> 函数证据 | None:
    名称模式 = re.compile(rf"\b{re.escape(函数名)}\s*\(")
    候选: list[tuple[bool, Path, int, list[str]]] = []
    for 文件 in 文件组:
        行组 = 文件.read_text(encoding="utf-8-sig", errors="replace").splitlines()
        for 索引, 行 in enumerate(行组):
            if 名称模式.search(行) and not re.search(rf"[.>]\s*{re.escape(函数名)}\s*\(", 行):
                是定义 = False
                for 展开索引 in range(索引, min(len(行组), 索引 + 16)):
                    if "{" in 行组[展开索引]:
                        是定义 = True
                        break
                    if ";" in 行组[展开索引]:
                        break
                候选.append((是定义, 文件, 索引, 行组))
    if not 候选:
        return None
    候选.sort(key=lambda 项: (not 项[0], 所有者 not in 项[1].stem, len(str(项[1])), 项[2]))
    _, 文件, 起始索引, 行组 = 候选[0]
    花括号开始 = None
    for 索引 in range(起始索引, min(len(行组), 起始索引 + 16)):
        if "{" in 行组[索引]:
            花括号开始 = 索引
            break
        if ";" in 行组[索引]:
            return 函数证据(所有者, 函数名, 文件, 起始索引 + 1, 索引 + 1)
    if 花括号开始 is None:
        return 函数证据(所有者, 函数名, 文件, 起始索引 + 1, 起始索引 + 1)
    深度 = 0
    已进入 = False
    结束索引 = 花括号开始
    for 索引 in range(花括号开始, len(行组)):
        行 = re.sub(r'"(?:\\.|[^"\\])*"', '""', 行组[索引])
        深度 += 行.count("{")
        if 行.count("{"):
            已进入 = True
        深度 -= 行.count("}")
        结束索引 = 索引
        if 已进入 and 深度 <= 0:
            break
    return 函数证据(所有者, 函数名, 文件, 起始索引 + 1, 结束索引 + 1)


def 主题提示文件(名称: str) -> list[Path]:
    结果: list[Path] = []
    for 关键词, 路径组 in 主题代码提示.items():
        if 关键词 in 名称:
            for 相对路径 in 路径组:
                路径 = 根目录 / 相对路径
                if 路径.exists() and 路径 not in 结果:
                    结果.append(路径)
    return 结果


def 分类(名称: str) -> tuple[str, str]:
    for 关键词, 原因 in 未实现主题.items():
        if 关键词 in 名称:
            return "未实现", 原因
    if any(关键词 in 名称 for 关键词 in 部分实现主题):
        return "部分实现", "当前代码只覆盖施工图中的一部分，图内未来节点继续作为缺口。"
    return "已实现或已形成第一轮入口", "当前代码存在对应入口；完成范围仍以 Git 事实和现状流程图核查为准。"


def 现状名称(源文件: Path) -> str:
    主题 = re.sub(r"^\d{8}_", "", 源文件.stem)
    主题 = re.sub(r"_v\d+\.\d+$", "", 主题)
    主题 = 主题.removesuffix("流程图")
    return f"20260711_{主题}现状流程图_v0.1"


def 增加现状入口(图: str, 状态: str) -> str:
    行组 = 图.splitlines()
    if not 行组:
        return 图
    首节点 = None
    for 行 in 行组[1:]:
        匹配 = re.match(r"\s*([A-Za-z][A-Za-z0-9_]*)\s*[\[{(]", 行)
        if 匹配:
            首节点 = 匹配.group(1)
            break
    if 首节点 is None:
        return 图
    行组.insert(1, f'    CURRENT_SCOPE["现状基线：{状态}；未来/候选节点不视为当前已实现"] --> {首节点}')
    return "\n".join(行组)


def 未实现图(标题: str, 原因: str) -> str:
    安全标题 = 标题.replace('"', "'")
    安全原因 = 原因.replace('"', "'")
    return "\n".join([
        "flowchart TD",
        f'    A["对应施工图：{安全标题}"] --> B["扫描当前代码和现状流程图核查"]',
        '    B --> C{"是否存在完整正式实现入口？"}',
        f'    C -- "否" --> D["当前现状：{安全原因}"]',
        '    D --> E["施工路径不进入当前运行流程；保持现有结构不变"]',
        '    C -- "仅有相邻材料" --> F["只标记相邻已实现入口，不宣称目标能力完成"]',
        '    F --> E',
    ])


def 非成功节点(图: str) -> list[tuple[str, str]]:
    结果: list[tuple[str, str]] = []
    for 节点, 标签 in re.findall(r"^\s*([A-Za-z][A-Za-z0-9_]*)[^\n]*?\[\"([^\"]+)\"\]", 图, re.M):
        if any(词 in 标签 for 词 in ("逻辑内返回", "入口拒绝", "返回空", "拒绝", "追根因")):
            结果.append((节点, 标签))
    return 结果


def 相对(路径: Path) -> str:
    return 路径.relative_to(根目录).as_posix()


def 写核查报告(报告路径: Path, 源路径: Path, 现状路径: Path, 状态: str, 原因: str,
             证据组: list[函数证据], 提示文件组: list[Path], 图: str, 提交: str, 脏文件组: list[str]) -> None:
    行组 = [
        f"# {现状路径.stem} 配套核查表",
        "",
        "日期：2026-07-11",
        "",
        "## 1. 元数据",
        "",
        "```text",
        f"图类型：现状流程图配套核查表",
        f"对应施工流程图：{相对(源路径)}",
        f"对应现状流程图：{相对(现状路径)}",
        f"代码版本：{提交}",
        f"工作区：{'存在未提交改动：' + '、'.join(脏文件组) if 脏文件组 else '干净'}",
        f"实现状态：{状态}",
        "覆盖方式：连续同语义代码块按函数范围合并，范围覆盖其中全部有效代码行；纯括号随相邻范围；未匹配入口不得声明已实现。",
        "```",
        "",
        "## 2. 逐行代码映射表",
        "",
        "| 文件 | 行号 | 代码行为 | 流程图节点 | 行为类型 | 结构变化 | 权威性 | 非成功口径 | 纠偏建议 |",
        "| --- | --- | --- | --- | --- | --- | --- | --- | --- |",
    ]
    if 证据组:
        for 编号, 证据 in enumerate(证据组, 1):
            行组.append(
                f"| {相对(证据.文件)} | {证据.起始行}-{证据.结束行} | `{证据.所有者}::{证据.函数名}` 当前完整定义范围；区间覆盖全部有效行 | E{编号:02d} | 调用/入口拒绝/读取/写入/读回/返回 | 依函数内部路径 | 当前代码事实 | 见非成功二分表 | 与施工图逐节点复核 |"
            )
    elif 提示文件组:
        for 编号, 文件 in enumerate(提示文件组, 1):
            总行数 = len(文件.read_text(encoding="utf-8-sig", errors="replace").splitlines())
            行组.append(
                f"| {相对(文件)} | 1-{总行数} | 当前相邻实现文件；施工图未给出可稳定提取的函数标记，本轮按整文件范围登记 | E{编号:02d} | 预编译/调用/读取/写入/返回 | 依文件内部路径 | 当前代码事实 | 见非成功二分表 | 后续人工细化函数节点 |"
            )
    else:
        行组.append("| 不适用 | 不适用 | 当前没有对应正式实现入口 | GAP | 不适用 | 否 | 当前缺口 | 不适用 | 保留施工图，后续实施后重画 |")
    行组.extend([
        "",
        "## 3. 输入契约 / 调用语境表",
        "",
        "| 入口 | 调用方 | 输入字段 | 输入来源 | 上游是否保证有效 | 允许逻辑内返回 | 追根因触发条件 | 结构变化 | 验证方式 |",
        "| --- | --- | --- | --- | --- | --- | --- | --- | --- |",
        "| 施工图对应公开入口 | 外部/测试调用 | 请求字段 | 外部材料/测试材料 | 否 | 设计允许且写前拒绝时允许 | 无 | 否 | 入口负例与结构数量 |",
        "| 施工图对应正式流程 | 上游领域服务/线程/内部编排 | 已确认结构材料 | 正式流程材料 | 是 | 不允许静默掩盖契约破坏 | 上游保证后仍无效，或创建/写入/读回后不及预期 | 可能 | 追根因诊断与读回验证 |",
        "",
        "## 4. 非成功返回二分审查表",
        "",
        "| 节点 | 代码位置 | 返回条件 | 输入语境 | 是否上游保证有效 | 结构是否变化 | 设计是否允许 | 二分口径 | 追根因对象或逻辑返回理由 | 计划动作 |",
        "| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |",
    ])
    非成功组 = 非成功节点(图)
    if 非成功组:
        for 节点, 标签 in 非成功组:
            if "追根因" in 标签:
                行组.append(f"| {节点} | 见逐行映射 | {标签} | 正式流程契约 | 是 | 可能 | 否 | 追根因解决 | 内部结构或读回不符合预期 | 修图/修详细设计/修代码 |")
            else:
                行组.append(f"| {节点}A | 见逐行映射 | {标签} | 外部/测试/请求材料 | 否 | 否 | 是 | 逻辑内返回 | 设计内写前拒绝 | 保留并验证结构不变 |")
                行组.append(f"| {节点}B | 见逐行映射 | 同一条件发生在上游已保证有效后 | 正式流程契约 | 是 | 否或可能 | 否 | 追根因解决 | 追查上游生成、传递或绕过正式入口 | 补契约/修代码 |")
    else:
        行组.append("| N/A-A | 不适用 | 外部请求材料不满足准入 | 外部/测试材料 | 否 | 否 | 是 | 逻辑内返回 | 写前拒绝 | 保留 |")
        行组.append("| N/A-B | 不适用 | 正式流程承诺后仍不满足准入 | 正式流程契约 | 是 | 否 | 否 | 追根因解决 | 上游契约破坏 | 补契约 |")
    行组.extend([
        "",
        "## 5. 偏差清单",
        "",
        "| 编号 | 现状代码事实 | 流程图 / 详细设计 / 计划描述 | 偏差类型 | 纠偏方向 | 是否需要代码计划 |",
        "| --- | --- | --- | --- | --- | --- |",
        f"| DEV-001 | {原因} | 对应施工图描述目标路径 | {'代码尚未实现目标' if 状态 == '未实现' else '当前实现范围与施工目标需逐节点保持区分'} | {'等待正式实施后重画' if 状态 == '未实现' else '保留当前路径，未来节点明确标成缺口'} | {'是' if 状态 == '未实现' else '视具体偏差'} |",
        "",
        "## 6. 完成边界",
        "",
        "本报告只证明已建立施工图对应的当前代码证据基线。自动匹配不到的函数、未来节点和候选路径不得解释为当前已实现。",
        "",
    ])
    报告路径.write_text("\n".join(行组), encoding="utf-8")


def 写现状图(输出路径: Path, 源路径: Path, 标题: str, 状态: str, 原因: str, 图: str,
          报告路径: Path, 证据组: list[函数证据], 提示文件组: list[Path], 提交: str, 脏文件组: list[str]) -> None:
    覆盖文件 = sorted({相对(证据.文件) for 证据 in 证据组} | {相对(文件) for 文件 in 提示文件组})
    覆盖函数 = [f"{证据.所有者}::{证据.函数名}" for 证据 in 证据组]
    文本 = "\n".join([
        f"# {标题}现状流程图",
        "",
        "更新时间：2026-07-11",
        "",
        "## 元数据",
        "",
        "```text",
        "图类型：现状流程图",
        f"对应施工流程图：{相对(源路径)}",
        f"代码版本：{提交}；{'基于当前脏工作区' if 脏文件组 else '工作区干净'}",
        f"实现状态：{状态}",
        "覆盖文件：" + ("、".join(覆盖文件) if 覆盖文件 else "当前无正式实现文件"),
        "覆盖函数：" + ("、".join(覆盖函数) if 覆盖函数 else "当前无可匹配正式入口"),
        f"逐行映射表：{相对(报告路径)} 第 2 节",
        f"输入契约 / 调用语境表：{相对(报告路径)} 第 3 节",
        f"非成功返回二分审查表：{相对(报告路径)} 第 4 节",
        f"偏差清单：{相对(报告路径)} 第 5 节",
        "不得作为施工许可：是",
        "不得宣称：未匹配节点、未来候选或施工目标已经实现",
        "```",
        "",
        "## 现状说明",
        "",
        f"{原因}",
        "本图与根目录施工图并存；施工图回答准备怎样实现，本图只回答当前代码实际存在什么入口和边界。",
        "",
        "## 流程图",
        "",
        "```mermaid",
        图,
        "```",
        "",
        "## 完成边界",
        "",
        "本图是当前代码证据基线，不是代码实施许可。当前存在未提交代码时，只能解释为当前工作区事实；未实现和部分实现节点不得扩大为系统完成。",
        "",
    ])
    输出路径.write_text(文本, encoding="utf-8")


def 写HTML(路径: Path, 标题: str, 图: str, 状态: str, 源路径: Path) -> None:
    页面 = f'''<!doctype html>
<html lang="zh-CN"><head><meta charset="utf-8"><title>{html.escape(标题)}现状流程图</title>
<style>body{{font-family:"Microsoft YaHei","Segoe UI",sans-serif;margin:28px;color:#1f2933;background:#f7f9fb}}main{{max-width:1500px;margin:auto;background:#fff;border:1px solid #d9e2ec;border-radius:6px;padding:24px}}.mermaid{{overflow:auto;border:1px solid #d9e2ec;padding:14px}}code{{background:#eef2f6;padding:2px 4px}}</style></head>
<body><main><h1>{html.escape(标题)}现状流程图</h1><p>图类型：现状流程图；实现状态：{html.escape(状态)}</p><p>对应施工图：<code>{html.escape(相对(源路径))}</code></p>
<div class="mermaid">
{图}
</div></main><script type="module">import mermaid from "https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.esm.min.mjs";mermaid.initialize({{startOnLoad:true,securityLevel:"loose",flowchart:{{useMaxWidth:false,htmlLabels:true}}}});</script></body></html>
'''
    路径.write_text(页面, encoding="utf-8")


def 生成() -> tuple[int, int]:
    输出目录.mkdir(parents=True, exist_ok=True)
    核查目录.mkdir(parents=True, exist_ok=True)
    for 路径 in [*输出目录.glob("*.md"), *输出目录.glob("*.html"), *核查目录.glob("*.md")]:
        路径.unlink()
    提交 = 当前提交()
    脏文件组 = 当前脏文件()
    文件组 = 代码文件组()
    索引行组 = [
        "# 施工流程图对应现状流程图索引",
        "",
        "更新时间：2026-07-11",
        "",
        "本目录只放现状流程图。根目录原图继续作为施工图、代码逻辑目标图、迁移治理图或旧项目证据图保存。",
        "",
        "| 施工流程图 | 现状流程图 | 状态 | 当前代码证据项数 | 配套核查表 |",
        "| --- | --- | --- | ---: | --- |",
    ]
    生成数量 = 0
    跳过数量 = 0
    for 源路径 in sorted(流程图目录.glob("*.md")):
        if any(片段 in 源路径.name for 片段 in 排除名称片段) or 源路径.name in 证据图名称:
            跳过数量 += 1
            continue
        源文本 = 源路径.read_text(encoding="utf-8-sig", errors="replace")
        原图 = 提取Mermaid(源文本)
        if 原图 is None:
            跳过数量 += 1
            continue
        原标题 = 源文本.splitlines()[0].lstrip("# ").strip()
        标题 = re.sub(r"\s+v\d+\.\d+$", "", 原标题).removesuffix("流程图").strip()
        状态, 原因 = 分类(标题)
        函数标记组 = 提取函数标记(原图)
        证据组 = [证据 for 所有者, 函数名 in 函数标记组 if (证据 := 查找函数范围(所有者, 函数名, 文件组))]
        提示文件组 = 主题提示文件(标题)
        if 状态 != "未实现" and not 证据组 and not 提示文件组:
            状态 = "未实现"
            原因 = "当前代码未匹配到对应正式实现入口。"
        if 状态 == "未实现":
            现状图 = 未实现图(标题, 原因)
        else:
            现状图 = 增加现状入口(原图, 状态)
        基名 = 现状名称(源路径)
        现状路径 = 输出目录 / f"{基名}.md"
        HTML路径 = 输出目录 / f"{基名}.html"
        报告路径 = 核查目录 / f"{基名}配套核查表.md"
        写核查报告(报告路径, 源路径, 现状路径, 状态, 原因, 证据组, 提示文件组, 现状图, 提交, 脏文件组)
        写现状图(现状路径, 源路径, 标题, 状态, 原因, 现状图, 报告路径, 证据组, 提示文件组, 提交, 脏文件组)
        写HTML(HTML路径, 标题, 现状图, 状态, 源路径)
        索引行组.append(f"| `{相对(源路径)}` | `{相对(现状路径)}` | {状态} | {len(证据组) + len(提示文件组)} | `{相对(报告路径)}` |")
        生成数量 += 1
    for 现有图 in sorted(流程图目录.glob("*现状流程图_v0.1.md")):
        对应HTML = 现有图.with_suffix(".html")
        shutil.copy2(现有图, 输出目录 / 现有图.name)
        if 对应HTML.exists():
            shutil.copy2(对应HTML, 输出目录 / 对应HTML.name)
        索引行组.append(
            f"| `已有严格现状图（无重复施工图）` | `{相对(输出目录 / 现有图.name)}` | 已有严格现状图 | - | 见原图元数据 |"
        )
    索引行组.extend([
        "",
        "## 排除项",
        "",
        "迁移模板、已经命名为现状流程图的文件和旧鱼巢只读证据图不作为施工图重复生成。已有现状图另复制到本目录作为兼容入口。",
        "",
        "## 边界",
        "",
        "批量生成只建立当前代码证据基线。每张图的代码纠偏仍需依据配套核查表另行形成代码计划；本目录不直接授权修改 C++。",
        "",
    ])
    (输出目录 / "README.md").write_text("\n".join(索引行组), encoding="utf-8")
    return 生成数量, 跳过数量


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="按施工流程图和当前代码生成现状流程图基线")
    解析器.add_argument("--check", action="store_true", help="只检查已生成目录的一一对应和 MD/HTML 数量")
    参数 = 解析器.parse_args()
    if 参数.check:
        MD组 = [路径 for 路径 in 输出目录.glob("*.md") if 路径.name != "README.md"]
        HTML组 = list(输出目录.glob("*.html"))
        if len(MD组) != len(HTML组):
            print(f"现状流程图检查失败: Markdown={len(MD组)} HTML={len(HTML组)}")
            return 1
        缺失 = [路径.name for 路径 in MD组 if not (输出目录 / f"{路径.stem}.html").exists()]
        if 缺失:
            print("现状流程图检查失败: 缺少 HTML: " + "、".join(缺失))
            return 1
        不一致: list[str] = []
        for MD路径 in MD组:
            MD图 = 提取Mermaid(MD路径.read_text(encoding="utf-8-sig", errors="replace"))
            HTML路径 = 输出目录 / f"{MD路径.stem}.html"
            HTML文本 = HTML路径.read_text(encoding="utf-8-sig", errors="replace")
            HTML匹配 = re.search(r'<div class="mermaid">\s*\n(.*?)\n\s*</div>', HTML文本, re.S)
            if MD图 is None or HTML匹配 is None or MD图.strip() != HTML匹配.group(1).strip():
                不一致.append(MD路径.name)
        if 不一致:
            print("现状流程图检查失败: MD/HTML Mermaid 不一致: " + "、".join(不一致))
            return 1
        print(f"现状流程图检查: {len(MD组)} 对 MD/HTML，未发现缺失")
        return 0
    生成数量, 跳过数量 = 生成()
    print(f"已生成 {生成数量} 份现状流程图，跳过 {跳过数量} 份模板、现状图、证据图或无 Mermaid 文件")
    return 0


if __name__ == "__main__":
    raise SystemExit(主函数())
