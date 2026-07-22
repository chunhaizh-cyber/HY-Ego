#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""海中鱼巣正式规范目录一致性检查工具。"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class 检查项:
    等级: str
    路径: str
    说明: str


@dataclass(frozen=True)
class 目录项:
    编号: str
    标题: str
    链接: str


根目录 = Path(__file__).resolve().parents[1]
规范目录 = 根目录 / "规范"
正式目录路径 = 规范目录 / "规范目录.md"

必需静态入口 = [
    "AGENTS.md",
    "规范/规范目录.md",
]
必需正式规范编号 = ("0050", "0100", "0300")
预期对象根编号 = (
    "1100", "1110", "1120", "1130", "1140", "1150", "1160", "1170", "1180", "1190",
    "2100", "2200", "2210", "2300", "2310", "2320", "2330", "3100", "3200", "3300",
)

已退出文件 = [
    "规范/000_项目规则总纲.md",
    "规范/001_规则迁移清单.md",
    "规范/0000_规范梳理总索引_20260721.md",
    "规范/0200_根规范覆盖矩阵_20260720.md",
    "规范/0400_根规范缺失补齐清单_20260720.md",
    "规范/工具与规则迁移治理规范.md",
    "规范/迁移路线权力分层规范.md",
    "规范/鱼巢任务筹办与方法执行逻辑迁移规范.md",
    "规范/鱼巢安全服务与自我因果闭环逻辑迁移规范.md",
    "规范/鱼巢本能方法与学习边界逻辑迁移规范.md",
    "规范/鱼巢正式规范业务逻辑迁移规则.md",
    "规范/鱼巢正式规范迁移目录.md",
    "规范/鱼巢特征状态动态与因果逻辑迁移规范.md",
    "规范/鱼巢观察外设与视觉分层逻辑迁移规范.md",
    "规范/鱼巢语言语素信息入口与存在逻辑迁移规范.md",
    "规范/鱼巢运行治理与阶段验收逻辑迁移规范.md",
    "规范/鱼巢通用结构与主信息根规则逻辑迁移规范.md",
    "规范/鱼巢需求生成入树与拒绝治理逻辑迁移规范.md",
    "规范/0600_顶层任务树与子智能体协作治理规范_20260721.md",
    "规范/1175_根规范_特征表达与判断收束规则_20260720.md",
    "规范/1195_根规范_因果模板_20260720.md",
    "规范/多线程防锁机制规范.md",
    "规范/公用函数规则规范.md",
    "规范/UTF8与中文标识符工程规范.md",
    "规范/现状流程图与施工流程图区分规范.md",
    "规范/仓库逻辑空间与领域树结构规范.md",
    "规范/仓库与服务分层事务边界规范.md",
    "规范/错误结果与入口拒绝规范.md",
    "规范/非权威缓存与统计规范.md",
    "规范/基础信息服务分层规范.md",
    "规范/节点类型与关系类型枚举规范.md",
    "规范/领域服务授权写入规范.md",
    "规范/主信息系统规范.md",
    "规范/动作入口规范.md",
    "规范/方法系统规范.md",
    "规范/任务系统规范.md",
    "规范/需求结算记录规范.md",
    "规范/因果用途观察与方法学习无环接线规范.md",
    "规范/语素系统规范.md",
    "规范/重构总纲.md",
    "规范/最小闭环设计.md",
    "规范/最小闭环验收用例.md",
    "规范/D455相机外设材料规范.md",
]

目录行模式 = re.compile(
    r"^\|\s*(\d{3,4})\s*\|\s*\[([^\]]+)\]\(([^)]+)\)\s*\|",
    re.MULTILINE,
)
正文编号模式 = re.compile(r"^编号[：:]\s*(\d{3,4})\s*$", re.MULTILINE)
正文状态模式 = re.compile(r"^状态[：:]\s*(.+?)\s*$", re.MULTILINE)
一级标题模式 = re.compile(r"^#\s+(.+?)\s*$", re.MULTILINE)
目录声明数量模式 = re.compile(r"本目录共登记\s*(\d+)\s*份编号规范")
关系字段模式 = re.compile(
    r"^(直接上级规范|直接依赖规范|相关边界规范|上级规范)[：:](.*?)"
    r"(?=^(?:直接上级规范|直接依赖规范|相关边界规范|上级规范)[：:]|^#{1,6}\s|\Z)",
    re.MULTILINE | re.DOTALL,
)
关系编号范围模式 = re.compile(r"(?<!\d)(\d{3,4})\s*[—–-]\s*(\d{3,4})(?!\d)")
关系编号模式 = re.compile(r"(?<!\d)(\d{3,4})(?!\d)")

正式规范禁止过程模式 = [
    re.compile(r"规范迁移|逻辑迁移|迁移完成|迁移过程|迁移来源|旧能力迁移|字段迁移|函数迁移"),
    re.compile(r"详细设计"),
    re.compile(r"\bGit\b", re.IGNORECASE),
    re.compile(r"\bCodex\b", re.IGNORECASE),
    re.compile(r"\bworktree\b", re.IGNORECASE),
    re.compile(r"旧项目|旧规范|历史来源"),
    re.compile(r"设计窗口|执行窗口|集成窗口|只读复核窗口"),
    re.compile(r"提取鱼巢|保留鱼巢|换装|迁回"),
    re.compile(r"当前实现边界|当前代码|现有代码|本规范只形成.{0,40}不宣称"),
]

def 读取文本(文件路径: Path) -> str:
    return 文件路径.read_text(encoding="utf-8-sig")


def 转为相对路径(文件路径: Path) -> str:
    return 文件路径.relative_to(根目录).as_posix()


def 检查必需入口(项目: list[目录项]) -> list[检查项]:
    结果: list[检查项] = []
    for 相对路径 in 必需静态入口:
        if not (根目录 / 相对路径).is_file():
            结果.append(检查项("ERROR", 相对路径, "缺少正式规范治理入口"))

    按编号 = {项.编号: 项 for 项 in 项目}
    for 编号 in 必需正式规范编号:
        if 编号 not in 按编号:
            结果.append(检查项("ERROR", "规范/规范目录.md", f"缺少必需正式规范编号: {编号}"))

    代理规则路径 = 根目录 / "AGENTS.md"
    if 代理规则路径.is_file():
        文本 = 读取文本(代理规则路径)
        必需引用 = ["规范/规范目录.md"]
        必需引用.extend(f"规范/{按编号[编号].链接}" for 编号 in 必需正式规范编号 if 编号 in 按编号)
        for 相对路径 in 必需引用:
            if 相对路径 not in 文本:
                结果.append(检查项("ERROR", "AGENTS.md", f"开工必读未引用: {相对路径}"))
        for 相对路径 in 已退出文件:
            if 相对路径 in 文本:
                结果.append(检查项("ERROR", "AGENTS.md", f"仍引用已退出入口: {相对路径}"))
    return 结果


def 读取正式目录项() -> tuple[list[目录项], list[检查项], str]:
    if not 正式目录路径.is_file():
        return [], [], ""

    文本 = 读取文本(正式目录路径)
    项目 = [目录项(*匹配.groups()) for 匹配 in 目录行模式.finditer(文本)]
    结果: list[检查项] = []
    if not 项目:
        结果.append(检查项("ERROR", "规范/规范目录.md", "未解析到任何编号规范目录项"))

    数量匹配 = 目录声明数量模式.search(文本)
    if not 数量匹配:
        结果.append(检查项("ERROR", "规范/规范目录.md", "缺少正式规范登记总数声明"))
    else:
        正式规范数量 = sum(项.编号 != "000" for 项 in 项目)
        if int(数量匹配.group(1)) == 正式规范数量:
            return 项目, 结果, 文本
        结果.append(
            检查项(
                "ERROR",
                "规范/规范目录.md",
                f"目录声明 {数量匹配.group(1)} 份，实际编号规范 {正式规范数量} 份",
            )
        )
    return 项目, 结果, 文本


def 检查目录唯一性(项目: list[目录项]) -> list[检查项]:
    结果: list[检查项] = []
    已见编号: dict[str, str] = {}
    已见链接: dict[str, str] = {}
    for 项 in 项目:
        if 项.编号 in 已见编号:
            结果.append(
                检查项("ERROR", "规范/规范目录.md", f"编号 {项.编号} 重复: {已见编号[项.编号]} / {项.链接}")
            )
        else:
            已见编号[项.编号] = 项.链接

        if 项.链接 in 已见链接:
            结果.append(
                检查项("ERROR", "规范/规范目录.md", f"链接 {项.链接} 重复用于 {已见链接[项.链接]} / {项.编号}")
            )
        else:
            已见链接[项.链接] = 项.编号
    return 结果


def 检查目录文件(项目: list[目录项]) -> list[检查项]:
    结果: list[检查项] = []
    已登记链接 = {项.链接 for 项 in 项目}

    for 项 in 项目:
        if "://" in 项.链接 or Path(项.链接).is_absolute():
            结果.append(检查项("ERROR", "规范/规范目录.md", f"编号 {项.编号} 使用非本地相对链接: {项.链接}"))
            continue

        文件路径 = 规范目录 / 项.链接
        if 文件路径.parent != 规范目录:
            结果.append(检查项("ERROR", "规范/规范目录.md", f"编号 {项.编号} 不在规范根目录: {项.链接}"))
            continue
        if not 文件路径.is_file():
            结果.append(检查项("ERROR", "规范/规范目录.md", f"编号 {项.编号} 链接不存在: {项.链接}"))
            continue

        文本 = 读取文本(文件路径)
        编号匹配 = 正文编号模式.search(文本)
        if not 编号匹配 or 编号匹配.group(1) != 项.编号:
            实际编号 = 编号匹配.group(1) if 编号匹配 else "缺失"
            结果.append(检查项("ERROR", 转为相对路径(文件路径), f"正文编号 {实际编号} 与目录编号 {项.编号} 不一致"))

        标题匹配 = 一级标题模式.search(文本)
        if not 标题匹配 or 项.标题 not in 标题匹配.group(1):
            实际标题 = 标题匹配.group(1) if 标题匹配 else "缺失"
            结果.append(检查项("ERROR", 转为相对路径(文件路径), f"一级标题 {实际标题} 未包含目录标题 {项.标题}"))

        状态匹配 = 正文状态模式.search(文本)
        if not 状态匹配 or "正式" not in 状态匹配.group(1):
            结果.append(检查项("ERROR", 转为相对路径(文件路径), "正文状态不是正式规范或正式规范目录"))

        if 项.编号 != "000":
            数值编号 = int(项.编号)
            是根规范 = 1100 <= 数值编号 <= 3999
            是子规范 = 数值编号 == 500 or 4000 <= 数值编号 <= 8999
            if 是根规范 and (
                "_根规范_" not in 文件路径.name
                or not 状态匹配
                or "正式根规范" not in 状态匹配.group(1)
                or not 标题匹配
                or "根规范" not in 标题匹配.group(1)
            ):
                结果.append(检查项("ERROR", 转为相对路径(文件路径), "1100—3999 对象规范的根规范层级不一致"))
            if 是子规范 and (
                "_子规范_" not in 文件路径.name
                or not 状态匹配
                or "正式子规范" not in 状态匹配.group(1)
                or not 标题匹配
                or "子规范" not in 标题匹配.group(1)
            ):
                结果.append(检查项("ERROR", 转为相对路径(文件路径), "0500 或 4000—8999 规范的子规范层级不一致"))

        if 项.编号 == "000":
            if 项.链接 != "规范目录.md":
                结果.append(检查项("ERROR", "规范/规范目录.md", "编号 000 必须指向规范目录.md"))
        else:
            if len(项.编号) != 4 or not 文件路径.name.startswith(f"{项.编号}_"):
                结果.append(检查项("ERROR", 转为相对路径(文件路径), f"文件名未以前缀 {项.编号}_ 开始"))
        for 模式 in 正式规范禁止过程模式:
            if 模式.search(文本):
                结果.append(检查项("ERROR", 转为相对路径(文件路径), f"正式机器逻辑规范含过程性内容: {模式.pattern}"))

    for 文件路径 in sorted(规范目录.glob("[0-9][0-9][0-9][0-9]_*.md")):
        if 文件路径.name not in 已登记链接:
            结果.append(检查项("ERROR", 转为相对路径(文件路径), "编号规范文件未登记到规范目录"))
    return 结果


def 检查对象根集合(项目: list[目录项]) -> list[检查项]:
    结果: list[检查项] = []
    实际对象根编号 = {
        项.编号
        for 项 in 项目
        if 1100 <= int(项.编号) <= 3999
    }
    预期集合 = set(预期对象根编号)
    缺少编号 = sorted(预期集合 - 实际对象根编号)
    多余编号 = sorted(实际对象根编号 - 预期集合)
    if 缺少编号:
        结果.append(检查项("ERROR", "规范/规范目录.md", f"缺少对象根规范: {', '.join(缺少编号)}"))
    if 多余编号:
        结果.append(检查项("ERROR", "规范/规范目录.md", f"存在非独立对象伪根规范: {', '.join(多余编号)}"))
    return 结果


def 解析关系编号(
    原文: str,
    已登记编号: set[str],
    文件相对路径: str,
    字段名称: str,
) -> tuple[set[str], list[检查项]]:
    """只解析显式关系字段中的编号；普通正文链接不参与规范层级推断。"""

    结果: list[检查项] = []
    引用编号: set[str] = set()
    掩码字符 = list(原文)

    for 匹配 in 关系编号范围模式.finditer(原文):
        起点, 终点 = 匹配.groups()
        if int(起点) > int(终点):
            结果.append(检查项("ERROR", 文件相对路径, f"{字段名称}使用倒序编号范围: {起点}—{终点}"))
        else:
            for 编号 in (起点, 终点):
                if 编号 not in 已登记编号:
                    结果.append(检查项("ERROR", 文件相对路径, f"{字段名称}引用未登记编号: {编号}"))
            引用编号.update(
                编号
                for 编号 in 已登记编号
                if int(起点) <= int(编号) <= int(终点)
            )
        for 位置 in range(匹配.start(), 匹配.end()):
            掩码字符[位置] = " "

    for 匹配 in 关系编号模式.finditer("".join(掩码字符)):
        编号 = 匹配.group(1)
        if 编号 not in 已登记编号:
            结果.append(检查项("ERROR", 文件相对路径, f"{字段名称}引用未登记编号: {编号}"))
        else:
            引用编号.add(编号)
    return 引用编号, 结果


def 提取规范关系(
    文本: str,
    已登记编号: set[str],
    文件相对路径: str,
) -> tuple[dict[str, set[str]], list[检查项]]:
    字段内容: dict[str, list[str]] = {}
    for 匹配 in 关系字段模式.finditer(文本):
        字段内容.setdefault(匹配.group(1), []).append(匹配.group(2))

    结果: list[检查项] = []
    for 字段名称, 内容列表 in 字段内容.items():
        if len(内容列表) > 1:
            结果.append(检查项("ERROR", 文件相对路径, f"关系字段重复出现: {字段名称}"))

    关系: dict[str, set[str]] = {
        "直接上级规范": set(),
        "直接依赖规范": set(),
        "相关边界规范": set(),
    }
    for 字段名称 in 关系:
        for 内容 in 字段内容.get(字段名称, []):
            编号, 解析结果 = 解析关系编号(内容, 已登记编号, 文件相对路径, 字段名称)
            关系[字段名称].update(编号)
            结果.extend(解析结果)

    旧字段内容 = 字段内容.get("上级规范", [])
    if 旧字段内容:
        结果.append(
            检查项(
                "ERROR",
                文件相对路径,
                "仍使用已退出字段“上级规范”；必须拆分为直接上级/直接依赖/相关边界",
            )
        )
        if 字段内容.get("直接上级规范"):
            结果.append(检查项("ERROR", 文件相对路径, "同时声明“直接上级规范”和兼容字段“上级规范”"))
    return 关系, 结果


def 查找强连通分量(图: dict[str, set[str]]) -> list[list[str]]:
    下一个索引 = 0
    节点索引: dict[str, int] = {}
    节点低位: dict[str, int] = {}
    栈: list[str] = []
    在栈中: set[str] = set()
    分量: list[list[str]] = []

    def 访问(节点: str) -> None:
        nonlocal 下一个索引
        节点索引[节点] = 下一个索引
        节点低位[节点] = 下一个索引
        下一个索引 += 1
        栈.append(节点)
        在栈中.add(节点)

        for 相邻节点 in sorted(图.get(节点, set())):
            if 相邻节点 not in 节点索引:
                访问(相邻节点)
                节点低位[节点] = min(节点低位[节点], 节点低位[相邻节点])
            elif 相邻节点 in 在栈中:
                节点低位[节点] = min(节点低位[节点], 节点索引[相邻节点])

        if 节点低位[节点] != 节点索引[节点]:
            return
        当前分量: list[str] = []
        while 栈:
            出栈节点 = 栈.pop()
            在栈中.remove(出栈节点)
            当前分量.append(出栈节点)
            if 出栈节点 == 节点:
                break
        分量.append(sorted(当前分量))

    for 节点 in sorted(图):
        if 节点 not in 节点索引:
            访问(节点)
    return 分量


def 检查规范关系图(项目: list[目录项]) -> list[检查项]:
    结果: list[检查项] = []
    已登记编号 = {项.编号 for 项 in 项目}
    直接上级图 = {项.编号: set() for 项 in 项目}
    直接依赖图 = {项.编号: set() for 项 in 项目}

    for 项 in 项目:
        文件路径 = 规范目录 / 项.链接
        if not 文件路径.is_file():
            continue
        文件相对路径 = 转为相对路径(文件路径)
        关系, 解析结果 = 提取规范关系(读取文本(文件路径), 已登记编号, 文件相对路径)
        结果.extend(解析结果)
        直接上级图[项.编号].update(关系["直接上级规范"])
        直接依赖图[项.编号].update(关系["直接依赖规范"])
        if 项.编号 in 直接上级图[项.编号]:
            结果.append(检查项("ERROR", 文件相对路径, f"直接上级规范形成自环: {项.编号} -> {项.编号}"))
        if 项.编号 in 直接依赖图[项.编号]:
            结果.append(检查项("ERROR", 文件相对路径, f"直接依赖规范形成自环: {项.编号} -> {项.编号}"))

    for 分量 in 查找强连通分量(直接上级图):
        if len(分量) <= 1:
            continue
        分量集合 = set(分量)
        内部边 = []
        for 编号 in 分量:
            目标 = sorted(直接上级图[编号] & 分量集合)
            if 目标:
                内部边.append(f"{编号} -> {','.join(目标)}")
        结果.append(
            检查项(
                "ERROR",
                "规范/规范目录.md",
                f"直接上级规范权威图存在循环 SCC [{', '.join(分量)}]；内部边: {'; '.join(内部边)}",
            )
        )
    for 分量 in 查找强连通分量(直接依赖图):
        if len(分量) <= 1:
            continue
        分量集合 = set(分量)
        内部边 = []
        for 编号 in 分量:
            目标 = sorted(直接依赖图[编号] & 分量集合)
            if 目标:
                内部边.append(f"{编号} -> {','.join(目标)}")
        结果.append(
            检查项(
                "ERROR",
                "规范/规范目录.md",
                f"直接依赖规范图存在循环 SCC [{', '.join(分量)}]；内部边: {'; '.join(内部边)}",
            )
        )
    return 结果


def 检查退出文件() -> list[检查项]:
    结果: list[检查项] = []
    for 相对路径 in 已退出文件:
        if (根目录 / 相对路径).exists():
            结果.append(检查项("ERROR", 相对路径, "已退出入口或过渡规范仍存在"))

    for 文件路径 in sorted(规范目录.glob("*.md")):
        if "迁移" in 文件路径.name:
            结果.append(检查项("ERROR", 转为相对路径(文件路径), "规范根目录仍有过程性过渡文件"))
    return 结果


def 打印结果(结果: list[检查项], 目录项数量: int) -> None:
    if not 结果:
        print(f"海中鱼巣正式规范检查: {目录项数量} 份目录项全部通过")
        return

    错误数 = sum(项.等级 == "ERROR" for 项 in 结果)
    警告数 = sum(项.等级 == "WARN" for 项 in 结果)
    状态 = "通过" if 错误数 == 0 else "未通过"
    print(f"海中鱼巣正式规范检查: {目录项数量} 份目录项，{状态}；{错误数} 个错误，{警告数} 个警告")
    for 项 in 结果:
        print(f"[{项.等级}] {项.路径}: {项.说明}")


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="检查海中鱼巣正式规范目录、编号和文件一致性")
    解析器.add_argument("--strict", action="store_true", help="遇到 ERROR 时返回非零退出码")
    参数 = 解析器.parse_args()

    结果: list[检查项] = []
    项目, 目录结果, _ = 读取正式目录项()
    结果.extend(目录结果)
    结果.extend(检查必需入口(项目))
    结果.extend(检查目录唯一性(项目))
    结果.extend(检查对象根集合(项目))
    结果.extend(检查目录文件(项目))
    结果.extend(检查规范关系图(项目))
    结果.extend(检查退出文件())
    打印结果(结果, sum(项.编号 != "000" for 项 in 项目))

    if 参数.strict and any(项.等级 == "ERROR" for 项 in 结果):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
