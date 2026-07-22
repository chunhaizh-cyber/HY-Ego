#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""海中鱼巣正式规范目录一致性检查工具。"""

from __future__ import annotations

import argparse
import re
import subprocess
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


@dataclass(frozen=True)
class 任务状态项:
    字段: dict[str, str]
    行号: int


@dataclass(frozen=True)
class 任务状态事件:
    字段: dict[str, str]
    行号: int


根目录 = Path(__file__).resolve().parents[1]
规范目录 = 根目录 / "规范"
正式目录路径 = 规范目录 / "规范目录.md"
任务状态台账路径 = 根目录 / "项目记忆" / "任务状态台账.md"

必需静态入口 = [
    "AGENTS.md",
    "规范/规范目录.md",
    "项目记忆/任务状态台账.md",
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

受管任务编号 = {f"#{编号}" for 编号 in range(339, 353)}
合法任务状态 = {
    "待执行", "依赖门控待执行", "已派发待执行回执", "执行中", "分支完成待集成",
    "已预留待集成派发", "已派发待集成回执", "集成中", "集成身份冲突待设计重建",
    "集成失败退回任务", "集成分支验证通过待主线发布", "已集成待设计同步",
    "接口漂移待设计修订", "验证失败待设计裁决", "只读复核中",
    "只读复核完成待设计裁决", "已完成 / 已归档",
}
合法初始状态 = {"待执行", "依赖门控待执行", "只读复核中"}
合法任务迁移 = {
    "待执行": {"已派发待执行回执", "依赖门控待执行", "接口漂移待设计修订"},
    "依赖门控待执行": {"待执行"},
    "已派发待执行回执": {"执行中", "接口漂移待设计修订"},
    "执行中": {"分支完成待集成", "接口漂移待设计修订", "验证失败待设计裁决"},
    "分支完成待集成": {"已预留待集成派发"},
    "已预留待集成派发": {"已派发待集成回执"},
    "已派发待集成回执": {"集成中", "集成身份冲突待设计重建"},
    "集成中": {"已集成待设计同步", "集成分支验证通过待主线发布", "集成失败退回任务"},
    "集成身份冲突待设计重建": {"已预留待集成派发"},
    "集成失败退回任务": {"待执行", "依赖门控待执行"},
    "集成分支验证通过待主线发布": {"已预留待集成派发"},
    "已集成待设计同步": {"已完成 / 已归档"},
    "接口漂移待设计修订": {"待执行", "依赖门控待执行"},
    "验证失败待设计裁决": {"待执行", "依赖门控待执行", "已完成 / 已归档"},
    "只读复核中": {"只读复核完成待设计裁决"},
    "只读复核完成待设计裁决": {"已完成 / 已归档", "待执行", "依赖门控待执行"},
    "已完成 / 已归档": set(),
}
合法事件类型 = {"初始化", "状态迁移", "元数据更新"}
合法S0结果 = {"不适用", "未开始", "PASS待设计接受", "PASS已接受", "DRIFT"}
合法技术绑定阶段 = {"未绑定", "预登记", "已冻结"}
状态责任角色 = {
    "待执行": "设计", "依赖门控待执行": "设计", "已派发待执行回执": "设计",
    "执行中": "执行", "分支完成待集成": "执行", "已预留待集成派发": "设计",
    "已派发待集成回执": "设计", "集成中": "集成", "集成身份冲突待设计重建": "集成",
    "集成失败退回任务": "集成", "集成分支验证通过待主线发布": "设计",
    "已集成待设计同步": "集成", "接口漂移待设计修订": "执行",
    "验证失败待设计裁决": "执行", "只读复核中": "只读复核",
    "只读复核完成待设计裁决": "设计", "已完成 / 已归档": "设计",
}
任务状态版本模式 = re.compile(r"^JY-(\d+)/E(\d+)$")
任务工作树身份模式 = re.compile(r"WT-\d+-R\d+")
集成工作树身份模式 = re.compile(r"INT-[A-Z0-9]+(?:-[A-Z0-9]+)*-R\d+")
叶子身份模式 = re.compile(
    r"^身份[：:]\s*`?#(?P<任务>\d+)\s*/\s*(?P<DQ>DQ-\d+)\s*/\s*"
    r"(?P<计划>[A-Z0-9-]+)\s+(?P<版本>v\d+\.\d+)`?\s*$",
    re.MULTILINE,
)
叶子状态引用模式 = re.compile(
    r"^任务状态引用[：:]\s*#(?P<任务>\d+)\s*/\s*计划\s+(?P<版本>v\d+\.\d+)\s*/\s*"
    r"合同建立状态版本\s+(?P<状态>JY-\d+/E\d+)",
    re.MULTILINE,
)
叶子禁止动态模式 = [
    re.compile(r"当前不可派发"), re.compile(r"尚未派发"), re.compile(r"已派发待执行"),
    re.compile(r"等待\s*JY-\d+"), re.compile(r"现已全部满足"),
    re.compile(r"当前唯一可继续"), re.compile(r"当前可继续"),
]
叶子全文动态模式 = [
    re.compile(r"当前不可派发"), re.compile(r"尚未派发"), re.compile(r"等待\s*JY-\d+"),
    re.compile(r"现已全部满足"), re.compile(r"当前唯一可继续"), re.compile(r"当前可继续"),
    re.compile(
        r"(?:当前|现已|截至|本轮).{0,40}"
        r"(?:已派发待执行回执|执行中|分支完成待集成|已派发待集成回执|集成中|已完成\s*/\s*已归档)"
    ),
]

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


def 运行Git(*参数: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["git", *参数],
        cwd=根目录,
        text=True,
        encoding="utf-8",
        errors="replace",
        capture_output=True,
        check=False,
    )


def 清理表格单元格(内容: str) -> str:
    内容 = 内容.strip()
    if len(内容) >= 2 and 内容.startswith("`") and 内容.endswith("`"):
        return 内容[1:-1]
    return 内容


def 解析Markdown表格(
    文本: str,
    首字段: str,
    预期字段: list[str],
    文件相对路径: str,
) -> tuple[list[tuple[int, dict[str, str]]], list[检查项]]:
    行 = 文本.splitlines()
    结果: list[检查项] = []
    候选表头: list[tuple[int, list[str]]] = []
    for 索引, 当前行 in enumerate(行):
        if not 当前行.lstrip().startswith("|"):
            continue
        单元格 = [清理表格单元格(项) for 项 in 当前行.strip().strip("|").split("|")]
        if 单元格 and 单元格[0] == 首字段:
            候选表头.append((索引, 单元格))
    if not 候选表头:
        return [], [检查项("ERROR", 文件相对路径, f"缺少以“{首字段}”开头的任务治理表")]
    if len(候选表头) != 1:
        return [], [
            检查项(
                "ERROR", 文件相对路径,
                f"以“{首字段}”开头的任务治理表必须且只能出现一次，实际 {len(候选表头)} 次",
            )
        ]
    表头位置, 实际字段 = 候选表头[0]
    if 实际字段 != 预期字段:
        结果.append(
            检查项(
                "ERROR",
                文件相对路径,
                f"“{首字段}”表字段不一致；实际: {', '.join(实际字段)}",
            )
        )
        return [], 结果

    项目: list[tuple[int, dict[str, str]]] = []
    for 索引 in range(表头位置 + 2, len(行)):
        当前行 = 行[索引]
        if not 当前行.lstrip().startswith("|"):
            break
        单元格 = [清理表格单元格(项) for 项 in 当前行.strip().strip("|").split("|")]
        if len(单元格) != len(预期字段):
            结果.append(
                检查项(
                    "ERROR",
                    文件相对路径,
                    f"第 {索引 + 1} 行表格列数 {len(单元格)} 与预期 {len(预期字段)} 不一致",
                )
            )
            continue
        项目.append((索引 + 1, dict(zip(预期字段, 单元格))))
    return 项目, 结果


def 解析仓库内路径(相对路径: str) -> tuple[Path | None, str | None]:
    路径 = Path(相对路径)
    if 路径.is_absolute():
        return None, "必须使用仓库相对路径"
    解析路径 = (根目录 / 路径).resolve()
    try:
        解析路径.relative_to(根目录.resolve())
    except ValueError:
        return None, "路径越出仓库"
    return 解析路径, None


def 提取精确允许文件(计划文本: str, 当前计划路径: str | None = None) -> tuple[set[str], str | None]:
    标题匹配 = re.search(r"^##\s+\d+(?:\.\d+)*\.\s*精确允许文件\s*$", 计划文本, re.MULTILINE)
    if not 标题匹配:
        return set(), "缺少“精确允许文件”章节"
    后续标题 = re.search(r"^##\s+", 计划文本[标题匹配.end():], re.MULTILINE)
    结束位置 = 标题匹配.end() + 后续标题.start() if 后续标题 else len(计划文本)
    章节 = 计划文本[标题匹配.end():结束位置]
    允许文件: set[str] = set()
    在代码块 = False
    for 行内容 in 章节.splitlines():
        去空 = 行内容.strip()
        if 去空.startswith("```"):
            在代码块 = not 在代码块
            continue
        候选: str | None = None
        if 在代码块 and 去空:
            候选 = 去空
        else:
            表格匹配 = re.match(r"^\|\s*`([^`]+)`\s*\|", 去空)
            if 表格匹配:
                候选 = 表格匹配.group(1)
        if 候选 is None or not re.search(r"\.(?:h|cpp|ixx|vcxproj|filters|md)$", 候选, re.IGNORECASE):
            continue
        路径, 错误 = 解析仓库内路径(候选)
        if 错误 or 路径 is None:
            return set(), f"允许文件路径非法: {候选}（{错误}）"
        规范路径 = 路径.relative_to(根目录.resolve()).as_posix()
        if 当前计划路径 and 规范路径 == 当前计划路径:
            return set(), "不可变叶子计划不得列入执行允许文件"
        if 规范路径 == "AGENTS.md" or 规范路径.startswith((".codex/", "规范/", "流程图/", "计划/", "项目记忆/")):
            return set(), f"中央设计治理文件不得列入执行允许文件: {规范路径}"
        允许文件.add(规范路径)
    if not 允许文件:
        return set(), "精确允许文件章节没有可解析的仓库相对文件"
    return 允许文件, None


def 读取工作树变更路径(工作树路径: str) -> tuple[set[str], str | None]:
    状态 = subprocess.run(
        ["git", "status", "--porcelain=v1", "-z"], cwd=工作树路径, text=True,
        encoding="utf-8", errors="replace", capture_output=True, check=False,
    )
    if 状态.returncode != 0:
        return set(), 状态.stderr.strip() or 状态.stdout.strip()
    记录 = 状态.stdout.split("\0")
    变更路径: set[str] = set()
    索引 = 0
    while 索引 < len(记录):
        当前 = 记录[索引]
        if not 当前:
            索引 += 1
            continue
        if len(当前) < 4:
            return set(), f"无法解析 porcelain 记录: {当前!r}"
        状态码 = 当前[:2]
        变更路径.add(当前[3:].replace("\\", "/"))
        if "R" in 状态码 or "C" in 状态码:
            索引 += 1
            if 索引 >= len(记录) or not 记录[索引]:
                return set(), "重命名 / 复制记录缺少原路径"
            变更路径.add(记录[索引].replace("\\", "/"))
        索引 += 1
    return 变更路径, None


def 解析主线发布占用(技术状态: str) -> tuple[str | None, bool]:
    命中 = re.findall(
        r"(?:^|[；;\s])主线发布占用=(已取得|已释放|不适用)(?=\s|[；;，,。/]|$)",
        技术状态,
    )
    return (命中[0] if len(命中) == 1 else None), len(命中) > 1


def 解析分号证据片段(证据: str) -> list[str]:
    return [片段.strip() for 片段 in re.split(r"[；;]", 证据) if 片段.strip()]


def 解析全部同结构表格(
    文本: str,
    预期字段: list[str],
    文件相对路径: str,
) -> tuple[list[tuple[int, dict[str, str]]], list[检查项]]:
    行 = 文本.splitlines()
    结果: list[检查项] = []
    项目: list[tuple[int, dict[str, str]]] = []
    命中表数 = 0
    for 表头位置, 当前行 in enumerate(行):
        if not 当前行.lstrip().startswith("|"):
            continue
        单元格 = [清理表格单元格(项) for 项 in 当前行.strip().strip("|").split("|")]
        if 单元格 != 预期字段:
            continue
        命中表数 += 1
        for 索引 in range(表头位置 + 2, len(行)):
            数据行 = 行[索引]
            if not 数据行.lstrip().startswith("|"):
                break
            数据单元格 = [清理表格单元格(项) for 项 in 数据行.strip().strip("|").split("|")]
            if len(数据单元格) != len(预期字段):
                结果.append(
                    检查项(
                        "ERROR", 文件相对路径,
                        f"第 {索引 + 1} 行表格列数 {len(数据单元格)} 与预期 {len(预期字段)} 不一致",
                    )
                )
                continue
            项目.append((索引 + 1, dict(zip(预期字段, 数据单元格))))
    if 命中表数 == 0:
        结果.append(检查项("ERROR", 文件相对路径, f"缺少结构化表: {', '.join(预期字段)}"))
    return 项目, 结果


def 计算工作树Blob(相对路径: str) -> tuple[str | None, str | None]:
    结果 = 运行Git("hash-object", "--filters", f"--path={相对路径}", 相对路径)
    if 结果.returncode != 0:
        return None, 结果.stderr.strip() or 结果.stdout.strip()
    return 结果.stdout.strip(), None


def 查找状态版本提交(状态版本: str) -> list[str]:
    匹配 = 任务状态版本模式.match(状态版本)
    if not 匹配:
        return []
    JY编号, E编号 = 匹配.groups()
    日志 = 运行Git("log", "main", "--format=%H%x09%s")
    if 日志.returncode != 0:
        return []
    标题模式 = re.compile(rf"^JY-{re.escape(JY编号)}\s+E{re.escape(E编号)}\b")
    提交: list[str] = []
    for 行 in 日志.stdout.splitlines():
        if "\t" not in 行:
            continue
        哈希, 标题 = 行.split("\t", 1)
        if 标题模式.search(标题):
            提交.append(哈希)
    return 提交


def 解析状态版本(状态版本: str) -> tuple[int, int] | None:
    匹配 = 任务状态版本模式.match(状态版本)
    if not 匹配:
        return None
    return int(匹配.group(1)), int(匹配.group(2))


def 读取主线最新状态版本() -> tuple[int, int] | None:
    日志 = 运行Git("log", "main", "--format=%s")
    if 日志.returncode != 0:
        return None
    模式 = re.compile(r"^JY-(\d+)\s+E(\d+)\b")
    for 标题 in 日志.stdout.splitlines():
        匹配 = 模式.match(标题)
        if 匹配:
            return int(匹配.group(1)), int(匹配.group(2))
    return None


def 读取提交Blob(提交: str, 相对路径: str) -> str | None:
    结果 = 运行Git("rev-parse", f"{提交}:{相对路径}")
    if 结果.returncode != 0:
        return None
    return 结果.stdout.strip()


def 读取提交文本(提交: str, 相对路径: str) -> str | None:
    结果 = 运行Git("show", f"{提交}:{相对路径}")
    if 结果.returncode != 0:
        return None
    return 结果.stdout


def 读取Git对象文本(对象: str, 对象类型: str = "blob") -> str | None:
    类型结果 = 运行Git("cat-file", "-t", 对象)
    if 类型结果.returncode != 0 or 类型结果.stdout.strip() != 对象类型:
        return None
    内容结果 = 运行Git("cat-file", "-p", 对象)
    if 内容结果.returncode != 0:
        return None
    return 内容结果.stdout


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


def 检查任务状态治理() -> list[检查项]:
    文件相对路径 = "项目记忆/任务状态台账.md"
    if not 任务状态台账路径.is_file():
        return [检查项("ERROR", 文件相对路径, "缺少唯一任务状态台账")]

    结果: list[检查项] = []
    文本 = 读取文本(任务状态台账路径)
    当前字段 = [
        "任务", "DQ", "计划标识", "计划版本", "计划路径", "计划 blob", "当前生命周期", "状态版本",
        "状态责任角色", "执行身份", "worktree 引用", "技术绑定阶段", "冻结基线版本", "冻结基线提交", "上一状态",
        "最新事件", "派发状态版本", "派发计划 blob", "S0 合同版本", "S0 计划 blob", "S0 结果",
        "S0 结果版本", "专用门禁",
    ]
    事件字段 = ["事件", "事件类型", "任务", "计划版本", "状态版本", "迁移链", "操作者 / 身份", "worktree 引用", "证据"]
    当前原始, 当前结果 = 解析Markdown表格(文本, "任务", 当前字段, 文件相对路径)
    事件原始, 事件结果 = 解析Markdown表格(文本, "事件", 事件字段, 文件相对路径)
    结果.extend(当前结果)
    结果.extend(事件结果)

    台账版本匹配 = re.findall(r"^状态版本[：:]\s*(JY-\d+/E\d+)\s*$", 文本, re.MULTILINE)
    if len(台账版本匹配) != 1:
        结果.append(检查项("ERROR", 文件相对路径, "必须且只能声明一个台账状态版本"))
        台账版本 = ""
    else:
        台账版本 = 台账版本匹配[0]
    台账版本元组 = 解析状态版本(台账版本)
    主线最新版本 = 读取主线最新状态版本()
    台账未提交 = bool(运行Git("status", "--porcelain", "--", 文件相对路径).stdout.strip())
    状态版本提交缓存: dict[str, list[str]] = {}
    待形成版本: set[str] = set()

    def 核对状态版本(状态版本: str, 路径: str, 说明: str) -> str | None:
        版本元组 = 解析状态版本(状态版本)
        if 版本元组 is None:
            结果.append(检查项("ERROR", 路径, f"{说明}格式非法: {状态版本}"))
            return None
        if 状态版本 not in 状态版本提交缓存:
            状态版本提交缓存[状态版本] = 查找状态版本提交(状态版本)
        提交 = 状态版本提交缓存[状态版本]
        if len(提交) > 1:
            结果.append(检查项("ERROR", 路径, f"{说明}{状态版本} 在 main 历史中对应多个提交"))
            return None
        if len(提交) == 1:
            return 提交[0]
        if 台账未提交 and 状态版本 == 台账版本:
            待形成版本.add(状态版本)
            return None
        结果.append(检查项("ERROR", 路径, f"{说明}{状态版本} 无 main 祖先提交且不是本轮唯一待形成版本"))
        return None

    if 台账版本:
        核对状态版本(台账版本, 文件相对路径, "台账状态版本 ")

    派发闭合状态 = {
        "已派发待执行回执", "执行中", "分支完成待集成",
        "已派发待集成回执", "集成中", "集成失败退回任务",
        "集成分支验证通过待主线发布", "已集成待设计同步",
    }
    派发等待S0状态 = {"已派发待执行回执", "已派发待集成回执"}
    S0已接受状态 = {
        "执行中", "分支完成待集成", "集成中", "集成失败退回任务",
        "集成分支验证通过待主线发布", "已集成待设计同步",
    }

    当前项: dict[str, 任务状态项] = {}
    for 行号, 字段 in 当前原始:
        任务 = 字段["任务"]
        if 任务 in 当前项:
            结果.append(检查项("ERROR", 文件相对路径, f"第 {行号} 行任务 {任务} 当前状态重复"))
            continue
        当前项[任务] = 任务状态项(字段, 行号)
        if 任务 not in 受管任务编号:
            结果.append(检查项("WARN", 文件相对路径, f"第 {行号} 行任务 {任务} 不在当前 #339—#352 受管集合"))
        if 字段["当前生命周期"] not in 合法任务状态:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 使用非法生命周期: {字段['当前生命周期']}"))
        elif 字段["状态责任角色"] != 状态责任角色[字段["当前生命周期"]]:
            结果.append(
                检查项(
                    "ERROR", 文件相对路径,
                    f"{任务} 在 {字段['当前生命周期']} 的责任角色应为 {状态责任角色[字段['当前生命周期']]}，实际为 {字段['状态责任角色']}",
                )
            )
        if not 任务状态版本模式.match(字段["状态版本"]):
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 状态版本格式非法: {字段['状态版本']}"))
        else:
            核对状态版本(字段["状态版本"], 文件相对路径, f"{任务} 状态版本 ")
        if 字段["S0 结果"] not in 合法S0结果:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 结果非法: {字段['S0 结果']}"))
        if 字段["技术绑定阶段"] not in 合法技术绑定阶段:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 技术绑定阶段非法: {字段['技术绑定阶段']}"))

        绑定阶段 = 字段["技术绑定阶段"]
        绑定字段 = ("执行身份", "worktree 引用")
        基线字段 = ("冻结基线版本", "冻结基线提交")
        if 绑定阶段 == "未绑定":
            for 字段名 in (*绑定字段, *基线字段):
                if 字段[字段名] != "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 未绑定阶段不得填写 {字段名}"))
        elif 绑定阶段 == "预登记":
            for 字段名 in 绑定字段:
                if 字段[字段名] == "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 预登记阶段缺少 {字段名}"))
            for 字段名 in 基线字段:
                if 字段[字段名] != "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 预登记尚未冻结，不得填写 {字段名}"))
        elif 绑定阶段 == "已冻结":
            for 字段名 in (*绑定字段, *基线字段):
                if 字段[字段名] == "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 已冻结阶段缺少 {字段名}"))
            if not re.fullmatch(r"[0-9a-f]{40}", 字段["冻结基线提交"]):
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 冻结基线提交必须是完整 40 位哈希"))

        if 字段["当前生命周期"] in {"待执行", "依赖门控待执行", "已预留待集成派发"}:
            for 字段名 in ("派发状态版本", "派发计划 blob", "S0 合同版本", "S0 计划 blob", "S0 结果版本"):
                if 字段[字段名] != "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 在 {字段['当前生命周期']} 阶段提前填写 {字段名}"))
            if 字段["S0 结果"] != "不适用":
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 在 {字段['当前生命周期']} 阶段 S0 结果必须为不适用"))
        if 字段["当前生命周期"] == "依赖门控待执行":
            if 绑定阶段 != "未绑定":
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 依赖门控阶段不得提前建立技术绑定"))
        if 字段["当前生命周期"] == "已预留待集成派发" and 绑定阶段 != "预登记":
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 集成派发预留阶段必须使用预登记技术绑定"))
        if 字段["当前生命周期"] in 派发闭合状态:
            if 绑定阶段 != "已冻结":
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 在 {字段['当前生命周期']} 前技术身份必须已冻结"))
            for 字段名 in (
                "执行身份", "worktree 引用", "冻结基线版本", "冻结基线提交", "派发状态版本",
                "派发计划 blob", "S0 合同版本", "S0 计划 blob",
            ):
                if 字段[字段名] == "—":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 在 {字段['当前生命周期']} 阶段缺少 {字段名}"))
            if 字段["派发计划 blob"] != 字段["计划 blob"] or 字段["S0 计划 blob"] != 字段["计划 blob"]:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 计划 / 派发 / S0 blob 不一致"))
            if 字段["S0 合同版本"] != 字段["派发状态版本"]:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 合同版本必须等于派发状态版本"))
            派发提交 = 核对状态版本(字段["派发状态版本"], 文件相对路径, f"{任务} 派发状态版本 ")
            if 派发提交 and 读取提交Blob(派发提交, 字段["计划路径"]) != 字段["计划 blob"]:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 派发状态提交中的计划 blob 与当前合同不一致"))
        if 字段["当前生命周期"] in 派发等待S0状态 and 字段["S0 结果"] not in {"未开始", "PASS待设计接受"}:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 派发回执阶段 S0 结果只能为未开始或 PASS待设计接受"))
        if 字段["当前生命周期"] in S0已接受状态 and 字段["S0 结果"] != "PASS已接受":
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 进入 {字段['当前生命周期']} 前 S0 必须为 PASS已接受"))
        if 字段["S0 结果"] in {"PASS待设计接受", "PASS已接受", "DRIFT"}:
            if 字段["S0 结果版本"] == "—":
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 结果 {字段['S0 结果']} 缺少结果版本"))
            else:
                结果提交 = 核对状态版本(字段["S0 结果版本"], 文件相对路径, f"{任务} S0 结果版本 ")
                if 结果提交 and 读取提交Blob(结果提交, 字段["计划路径"]) != 字段["计划 blob"]:
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 结果版本中的计划 blob 与当前合同不一致"))
        elif 字段["S0 结果版本"] != "—":
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 结果为 {字段['S0 结果']} 时不得填写结果版本"))
        if 任务 == "#352" and 字段["计划版本"] == "v0.1" and 字段["当前生命周期"] != "依赖门控待执行":
            结果.append(检查项("ERROR", 文件相对路径, "#352 v0.1 在升 v0.2 前只能保持依赖门控待执行"))

    缺少任务 = sorted(受管任务编号 - set(当前项), key=lambda 值: int(值[1:]))
    if 缺少任务:
        结果.append(检查项("ERROR", 文件相对路径, f"缺少逐任务当前状态行: {', '.join(缺少任务)}"))

    事件项: dict[str, 任务状态事件] = {}
    每任务事件: dict[str, list[任务状态事件]] = {}
    for 行号, 字段 in 事件原始:
        事件 = 字段["事件"]
        if 事件 in 事件项:
            结果.append(检查项("ERROR", 文件相对路径, f"第 {行号} 行事件 {事件} 重复"))
            continue
        项 = 任务状态事件(字段, 行号)
        事件项[事件] = 项
        每任务事件.setdefault(字段["任务"], []).append(项)
        if 字段["事件类型"] not in 合法事件类型:
            结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 类型非法: {字段['事件类型']}"))
        核对状态版本(字段["状态版本"], 文件相对路径, f"事件 {事件} 状态版本 ")
        迁移链 = [状态.strip() for 状态 in 字段["迁移链"].split(">")]
        if len(迁移链) < 2:
            结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 迁移链不足两端"))
            continue
        if 字段["事件类型"] == "初始化" and (len(迁移链) != 2 or 迁移链[0] != "未登记" or 迁移链[1] not in 合法初始状态):
            结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 初始化链非法: {字段['迁移链']}"))
        if 字段["事件类型"] == "元数据更新":
            if len(迁移链) != 2 or 迁移链[0] != 迁移链[1] or 迁移链[0] not in 合法任务状态:
                结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 元数据更新必须是两个相同合法状态"))
        if 字段["事件类型"] == "状态迁移" and 迁移链[0] == 迁移链[-1]:
            结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 状态迁移首尾相同"))
        if 字段["事件类型"] != "元数据更新":
            for 起点, 终点 in zip(迁移链, 迁移链[1:]):
                if 起点 == "未登记":
                    if 终点 not in 合法初始状态:
                        结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 非法迁移 {起点} -> {终点}"))
                elif 起点 not in 合法任务迁移 or 终点 not in 合法任务迁移[起点]:
                    结果.append(检查项("ERROR", 文件相对路径, f"事件 {事件} 非法迁移 {起点} -> {终点}"))

    for 任务, 任务事件 in 每任务事件.items():
        前一末端: str | None = None
        前一版本: tuple[int, int] | None = None
        for 索引, 事件 in enumerate(任务事件):
            迁移链 = [状态.strip() for 状态 in 事件.字段["迁移链"].split(">")]
            当前版本 = 解析状态版本(事件.字段["状态版本"])
            if 索引 == 0 and (事件.字段["事件类型"] != "初始化" or 迁移链[0] != "未登记"):
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 首个事件必须从未登记初始化"))
            if 前一末端 is not None and 迁移链[0] != 前一末端:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 事件 {事件.字段['事件']} 与前一事件不连续"))
            if 当前版本 and 前一版本 and 当前版本 < 前一版本:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 事件版本倒退: {事件.字段['状态版本']}"))
            前一末端 = 迁移链[-1]
            前一版本 = 当前版本

    for 任务, 当前 in 当前项.items():
        最新事件 = 事件项.get(当前.字段["最新事件"])
        if not 最新事件:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 最新事件不存在: {当前.字段['最新事件']}"))
            continue
        if 最新事件.字段["任务"] != 任务:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 最新事件指向其它任务"))
        任务事件 = 每任务事件.get(任务, [])
        if not 任务事件:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 没有可用于闭合当前行的状态事件"))
            continue
        if 任务事件[-1].字段["事件"] != 当前.字段["最新事件"]:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 当前行未引用同任务最后一条事件"))
        迁移链 = [状态.strip() for 状态 in 最新事件.字段["迁移链"].split(">")]
        对照字段 = {
            "计划版本": 最新事件.字段["计划版本"],
            "状态版本": 最新事件.字段["状态版本"],
            "上一状态": 迁移链[0],
            "当前生命周期": 迁移链[-1],
        }
        for 字段名, 预期 in 对照字段.items():
            if 当前.字段[字段名] != 预期:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} 当前行 {字段名}={当前.字段[字段名]}，最新事件为 {预期}"))

    历史台账文本 = 读取提交文本("HEAD", 文件相对路径)
    if 历史台账文本:
        历史事件原始, 历史解析结果 = 解析Markdown表格(历史台账文本, "事件", 事件字段, 文件相对路径)
        if not 历史解析结果:
            if len(事件原始) < len(历史事件原始):
                结果.append(检查项("ERROR", 文件相对路径, "状态迁移事件不得删除已提交历史"))
            else:
                for 索引, 历史事件 in enumerate(历史事件原始):
                    if 事件原始[索引][1] != 历史事件[1]:
                        结果.append(检查项("ERROR", 文件相对路径, f"已提交事件 {历史事件[1]['事件']} 被改写或重排"))
                        break

    登记表路径 = 根目录 / "项目记忆" / "并行工作树登记表.md"
    登记表文本 = 读取文本(登记表路径) if 登记表路径.is_file() else ""
    登记字段 = ["身份", "计划 / 状态定位键", "目标路径 / 分支", "冻结基线", "技术所有者", "技术状态"]
    登记原始, 登记解析结果 = 解析全部同结构表格(登记表文本, 登记字段, "项目记忆/并行工作树登记表.md")
    结果.extend(登记解析结果)
    登记项 = {字段["身份"]: 字段 for _, 字段 in 登记原始}
    if len(登记项) != len(登记原始):
        结果.append(检查项("ERROR", "项目记忆/并行工作树登记表.md", "同一技术身份在结构化登记行中重复"))
    for 任务, 当前 in 当前项.items():
        字段 = 当前.字段
        计划路径 = 字段["计划路径"]
        计划文件, 计划路径错误 = 解析仓库内路径(计划路径)
        if 计划路径错误 or 计划文件 is None:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 计划路径非法: {计划路径}（{计划路径错误}）"))
            continue
        if not 计划文件.is_file():
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 计划路径不存在: {计划路径}"))
            continue
        计划文本 = 读取文本(计划文件)
        身份 = 叶子身份模式.search(计划文本)
        if not 身份:
            结果.append(检查项("ERROR", 计划路径, "缺少可解析的叶子身份"))
        else:
            实际身份 = {"任务": f"#{身份['任务']}", "DQ": 身份["DQ"], "计划标识": 身份["计划"], "计划版本": 身份["版本"]}
            for 字段名, 实际 in 实际身份.items():
                if 字段[字段名] != 实际:
                    结果.append(检查项("ERROR", 计划路径, f"叶子 {字段名}={实际} 与台账 {字段[字段名]} 不一致"))
        状态引用 = 叶子状态引用模式.search(计划文本)
        if not 状态引用:
            结果.append(检查项("ERROR", 计划路径, "缺少任务状态引用或格式非法"))
        else:
            if f"#{状态引用['任务']}" != 任务 or 状态引用["版本"] != 字段["计划版本"]:
                结果.append(检查项("ERROR", 计划路径, "任务状态引用与台账任务 / 计划版本不一致"))
        if not re.search(r"^设计状态[：:]\s*终审 PASS\s*$", 计划文本, re.MULTILINE):
            结果.append(检查项("ERROR", 计划路径, "叶子计划缺少稳定的“设计状态：终审 PASS”"))
        if re.search(r"^状态[：:]", 计划文本, re.MULTILINE):
            结果.append(检查项("ERROR", 计划路径, "叶子计划仍维护动态“状态”字段"))
        元数据文本 = 计划文本.split("##", 1)[0]
        for 模式 in 叶子禁止动态模式:
            命中 = 模式.search(元数据文本)
            if 命中:
                结果.append(检查项("ERROR", 计划路径, f"叶子计划含动态生命周期句式: {命中.group(0)}"))
        动态扫描文本 = re.sub(r"\s+", "", 计划文本)
        for 模式 in 叶子全文动态模式:
            命中 = 模式.search(动态扫描文本)
            if 命中:
                结果.append(检查项("ERROR", 计划路径, f"叶子计划正文含时态化生命周期句式: {命中.group(0)}"))
        任务编号 = int(任务.removeprefix("#")) if 任务.removeprefix("#").isdigit() else -1
        if 340 <= 任务编号 <= 351:
            合同字段模式 = {
                "共同合同基线": r"^共同合同基线[：:]\s*(?P<值>.+)\s*$",
                "合同来源": r"^合同来源[：:]\s*(?P<值>.+)\s*$",
                "消费合同": r"^消费合同[：:]\s*(?P<值>.+)\s*$",
                "提供合同": r"^提供合同[：:]\s*(?P<值>.+)\s*$",
                "合同性质": r"^合同性质[：:]\s*(?P<值>.+)\s*$",
            }
            合同字段值: dict[str, str] = {}
            for 合同字段, 合同模式 in 合同字段模式.items():
                合同匹配 = re.findall(合同模式, 计划文本, re.MULTILINE)
                if len(合同匹配) != 1:
                    结果.append(检查项("ERROR", 计划路径, f"合同并行叶子必须唯一声明 {合同字段}"))
                else:
                    合同字段值[合同字段] = 合同匹配[0]
            if "合同性质" in 合同字段值 and "预冻结待实现" not in 合同字段值["合同性质"]:
                结果.append(检查项("ERROR", 计划路径, "合同并行叶子的合同性质必须为预冻结待实现"))
            for 合同字段 in ("共同合同基线", "消费合同", "提供合同"):
                if 合同字段 in 合同字段值 and not re.search(r"(?:NT-|NODE-TYPED-MIGRATION|CUTOVER-).*/v?\d", 合同字段值[合同字段]):
                    结果.append(检查项("ERROR", 计划路径, f"{合同字段} 缺少具名版本合同"))

            合同来源文本 = ""
            if "合同来源" in 合同字段值:
                来源路径匹配 = re.search(r"`([^`]+\.md)`", 合同字段值["合同来源"])
                if not 来源路径匹配:
                    结果.append(检查项("ERROR", 计划路径, "合同来源缺少仓库内 Markdown 路径"))
                else:
                    来源路径 = 来源路径匹配.group(1)
                    来源文件, 来源错误 = 解析仓库内路径(来源路径)
                    if 来源错误 or 来源文件 is None or not 来源文件.is_file():
                        结果.append(检查项("ERROR", 计划路径, f"合同来源不可读取: {来源路径}（{来源错误 or '文件不存在'}）"))
                    else:
                        合同来源文本 = 读取文本(来源文件)
            合同闭合文本 = f"{计划文本}\n{合同来源文本}"
            合同必要要素 = {
                "模块 / 文件": r"模块.*文件|文件.*模块",
                "DTO": r"\bDTO\b",
                "完整签名": r"签名",
                "ABI / 规则版本": r"ABI|规则版本",
                "输入输出所有权": r"所有权",
                "生命周期": r"生命周期",
                "逻辑内结果": r"逻辑内",
                "内部错误": r"内部(?:逻辑)?错误",
                "漂移退回点": r"漂移.*退回|退回.*漂移",
            }
            for 要素名, 要素模式 in 合同必要要素.items():
                if not re.search(要素模式, 合同闭合文本, re.IGNORECASE | re.DOTALL):
                    结果.append(检查项("ERROR", 计划路径, f"预冻结合同链缺少 {要素名}"))
        工作树Blob, 错误 = 计算工作树Blob(计划路径)
        if 错误:
            结果.append(检查项("ERROR", 计划路径, f"无法计算计划 blob: {错误}"))
        elif 工作树Blob != 字段["计划 blob"]:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 台账计划 blob {字段['计划 blob']} 与工作树 {工作树Blob} 不一致"))
        if not re.fullmatch(r"[0-9a-f]{40}", 字段["计划 blob"]):
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 计划 blob 格式非法"))

        任务工作树匹配 = 任务工作树身份模式.fullmatch(字段["worktree 引用"])
        集成工作树匹配 = 集成工作树身份模式.fullmatch(字段["worktree 引用"])
        集成生命周期 = {
            "已预留待集成派发", "已派发待集成回执", "集成中", "集成身份冲突待设计重建",
            "集成失败退回任务", "集成分支验证通过待主线发布", "已集成待设计同步",
        }
        if 字段["当前生命周期"] in 集成生命周期 and 任务工作树匹配:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 集成生命周期不得继续引用任务 worktree 身份"))
        if 字段["当前生命周期"] not in 集成生命周期 | {"已完成 / 已归档"} and 集成工作树匹配:
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 执行生命周期不得引用集成身份"))
        worktree匹配 = 任务工作树匹配 or 集成工作树匹配
        if worktree匹配:
            工作树编号 = worktree匹配.group(0)
            技术登记 = 登记项.get(工作树编号)
            if not 技术登记:
                结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 引用未登记: {工作树编号}"))
            else:
                定位键 = 技术登记["计划 / 状态定位键"]
                for 必需片段 in (任务, 字段["DQ"], f"{字段['计划标识']} {字段['计划版本']}"):
                    if 必需片段 not in 定位键:
                        结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 登记行缺少同任务绑定: {必需片段}"))
                if 字段["计划 blob"] not in 技术登记["冻结基线"]:
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 同行未绑定当前计划 blob"))
                占用状态, 占用重复 = 解析主线发布占用(技术登记["技术状态"])
                if 占用重复:
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 技术登记重复声明主线发布占用"))
                if 字段["当前生命周期"] == "集成中" and 占用状态 != "已取得":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 集成中技术登记未证明主线发布占用"))
                if 字段["当前生命周期"] in {
                    "集成失败退回任务", "集成分支验证通过待主线发布", "已集成待设计同步",
                } and 占用状态 != "已释放":
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 集成交接 / 退回状态未登记主线占用已释放"))

                if 字段["技术绑定阶段"] == "已冻结":
                    基线提交 = 字段["冻结基线提交"]
                    if 基线提交 not in 技术登记["冻结基线"]:
                        结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 同行未登记完整冻结提交"))
                    基线版本提交 = 核对状态版本(字段["冻结基线版本"], 文件相对路径, f"{任务} 冻结基线版本 ")
                    if 基线版本提交 and 基线版本提交 != 基线提交:
                        结果.append(检查项("ERROR", 文件相对路径, f"{任务} 冻结基线版本与完整提交不一致"))
                    if re.fullmatch(r"[0-9a-f]{40}", 基线提交):
                        if 读取提交Blob(基线提交, 计划路径) != 字段["计划 blob"]:
                            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 冻结基线提交不包含台账计划 blob"))
                    路径分支 = re.findall(r"`([^`]+)`", 技术登记["目标路径 / 分支"])
                    if len(路径分支) < 2:
                        结果.append(检查项("ERROR", "项目记忆/并行工作树登记表.md", f"{工作树编号} 缺少可解析路径 / 分支"))
                    else:
                        目标路径, 目标分支 = 路径分支[0], 路径分支[1]
                        if not Path(目标路径).is_dir():
                            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 已冻结 worktree 路径不存在"))
                        else:
                            def 目标Git(*参数: str) -> subprocess.CompletedProcess[str]:
                                return subprocess.run(
                                    ["git", *参数], cwd=目标路径, text=True, encoding="utf-8",
                                    errors="replace", capture_output=True, check=False,
                                )
                            分支结果 = 目标Git("branch", "--show-current")
                            HEAD结果 = 目标Git("rev-parse", "HEAD")
                            状态结果 = 目标Git("status", "--porcelain")
                            实际HEAD = HEAD结果.stdout.strip() if HEAD结果.returncode == 0 else ""
                            if 分支结果.returncode != 0 or 分支结果.stdout.strip() != 目标分支:
                                结果.append(检查项("ERROR", 文件相对路径, f"{任务} worktree 实际分支与登记不一致"))
                            if 字段["当前生命周期"] in {"待执行", "已派发待执行回执", "已派发待集成回执"}:
                                if 实际HEAD != 基线提交:
                                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 前 worktree HEAD 必须等于冻结基线"))
                                if 状态结果.returncode != 0 or 状态结果.stdout.strip():
                                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} S0 前 worktree 必须 clean"))
                            else:
                                祖先结果 = 目标Git("merge-base", "--is-ancestor", 基线提交, 实际HEAD)
                                if not 实际HEAD or 祖先结果.returncode != 0:
                                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 冻结基线不是 worktree 当前 HEAD 的祖先"))
                                if 字段["当前生命周期"] in {
                                    "分支完成待集成", "集成失败退回任务",
                                    "集成分支验证通过待主线发布", "已集成待设计同步",
                                } and (状态结果.returncode != 0 or 状态结果.stdout.strip()):
                                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 交接 / 退回阶段 worktree 必须 clean"))
                                if 字段["当前生命周期"] == "执行中":
                                    允许文件, 允许错误 = 提取精确允许文件(计划文本, 计划路径)
                                    if 允许错误:
                                        结果.append(检查项("ERROR", 计划路径, f"无法建立执行期 dirty 白名单: {允许错误}"))
                                    else:
                                        变更路径, 状态错误 = 读取工作树变更路径(目标路径)
                                        if 状态错误:
                                            结果.append(检查项("ERROR", 文件相对路径, f"{任务} 无法读取执行期 dirty 文件: {状态错误}"))
                                        else:
                                            越界路径 = sorted(变更路径 - 允许文件)
                                            if 越界路径:
                                                结果.append(
                                                    检查项(
                                                        "ERROR", 文件相对路径,
                                                        f"{任务} 执行期 dirty 文件越出计划白名单: {', '.join(越界路径)}",
                                                    )
                                                )
        elif 字段["技术绑定阶段"] != "未绑定":
            结果.append(检查项("ERROR", 文件相对路径, f"{任务} {字段['技术绑定阶段']} 阶段缺少结构化 worktree / 集成身份引用"))

        需要核对版本 = {字段["状态版本"]}
        if 状态引用:
            需要核对版本.add(状态引用["状态"])
        for 状态版本 in 需要核对版本:
            提交 = 核对状态版本(状态版本, 文件相对路径, "状态版本 ")
            if 提交:
                提交Blob = 读取提交Blob(提交, 计划路径)
                if 提交Blob != 字段["计划 blob"]:
                    结果.append(检查项("ERROR", 文件相对路径, f"{任务} 状态版本 {状态版本} 中计划 blob 为 {提交Blob}，台账为 {字段['计划 blob']}"))

    if 台账未提交:
        if 待形成版本 != ({台账版本} if 台账版本 else set()):
            结果.append(检查项("ERROR", 文件相对路径, "本轮只能存在一个等于台账头部版本的待形成状态版本"))
        if 台账版本元组 and 主线最新版本 and 台账版本元组 != (主线最新版本[0] + 1, 主线最新版本[1] + 1):
            结果.append(检查项("ERROR", 文件相对路径, f"待形成版本 {台账版本} 不是 main 最新 JY/E 的连续下一号"))
        if 待形成版本:
            结果.append(检查项("WARN", 文件相对路径, f"状态版本 {台账版本} 尚未形成 Git 提交；提交后必须唯一解析并重验"))

    窗口路径 = 根目录 / "项目记忆" / "窗口交互记录.md"
    窗口字段 = ["事件", "状态版本", "任务", "计划版本", "前状态", "迁移链", "后状态", "迁移者", "证据与原因"]
    窗口文本 = 读取文本(窗口路径) if 窗口路径.is_file() else ""
    窗口原始, 窗口解析结果 = 解析Markdown表格(窗口文本, "事件", 窗口字段, "项目记忆/窗口交互记录.md")
    结果.extend(窗口解析结果)
    窗口项 = {字段["事件"]: 字段 for _, 字段 in 窗口原始}
    if len(窗口项) != len(窗口原始):
        结果.append(检查项("ERROR", "项目记忆/窗口交互记录.md", "窗口迁移事件 ID 重复"))
    应有窗口事件 = {事件: 项 for 事件, 项 in 事件项.items() if 项.字段["事件类型"] != "元数据更新"}
    if set(窗口项) != set(应有窗口事件):
        结果.append(检查项("ERROR", "项目记忆/窗口交互记录.md", "窗口迁移事件与台账初始化 / 状态迁移事件集合不一致"))
    for 事件, 台账事件 in 应有窗口事件.items():
        窗口事件 = 窗口项.get(事件)
        if not 窗口事件:
            continue
        迁移链 = [状态.strip() for 状态 in 台账事件.字段["迁移链"].split(">")]
        对照 = {
            "状态版本": 台账事件.字段["状态版本"], "任务": 台账事件.字段["任务"],
            "计划版本": 台账事件.字段["计划版本"], "前状态": 迁移链[0],
            "迁移链": 台账事件.字段["迁移链"], "后状态": 迁移链[-1],
            "迁移者": 台账事件.字段["操作者 / 身份"],
        }
        for 字段名, 预期 in 对照.items():
            if 窗口事件[字段名] != 预期:
                结果.append(检查项("ERROR", "项目记忆/窗口交互记录.md", f"事件 {事件} 的 {字段名} 与台账不一致"))

    历史窗口文本 = 读取提交文本("HEAD", "项目记忆/窗口交互记录.md")
    if 历史窗口文本 and "| 事件 | 状态版本 | 任务 |" in 历史窗口文本:
        历史窗口原始, 历史窗口结果 = 解析Markdown表格(历史窗口文本, "事件", 窗口字段, "项目记忆/窗口交互记录.md")
        if not 历史窗口结果:
            if len(窗口原始) < len(历史窗口原始):
                结果.append(检查项("ERROR", "项目记忆/窗口交互记录.md", "窗口迁移事件不得删除已提交历史"))
            else:
                for 索引, 历史事件 in enumerate(历史窗口原始):
                    if 窗口原始[索引][1] != 历史事件[1]:
                        结果.append(检查项("ERROR", "项目记忆/窗口交互记录.md", f"已提交窗口事件 {历史事件[1]['事件']} 被改写或重排"))
                        break

    当前状态路径 = 根目录 / "项目记忆" / "当前状态.md"
    当前状态文本 = 读取文本(当前状态路径) if 当前状态路径.is_file() else ""
    投影版本匹配 = re.findall(r"^任务状态投影版本[：:]\s*(JY-\d+/E\d+)\s*$", 当前状态文本, re.MULTILINE)
    if len(投影版本匹配) != 1:
        结果.append(检查项("ERROR", "项目记忆/当前状态.md", "必须且只能声明一个任务状态投影版本"))
    else:
        投影版本 = 投影版本匹配[0]
        投影版本元组 = 解析状态版本(投影版本)
        核对状态版本(投影版本, "项目记忆/当前状态.md", "投影版本 ")
        if 投影版本元组 and 台账版本元组 and 投影版本元组 > 台账版本元组:
            结果.append(检查项("ERROR", "项目记忆/当前状态.md", "人读投影版本不得领先于任务状态台账"))
        投影字段 = ["任务", "计划版本", "状态版本", "生命周期投影", "说明"]
        投影原始, 投影解析结果 = 解析Markdown表格(当前状态文本, "任务", 投影字段, "项目记忆/当前状态.md")
        结果.extend(投影解析结果)
        投影项 = {字段["任务"]: 字段 for _, 字段 in 投影原始}
        if len(投影项) != len(投影原始):
            结果.append(检查项("ERROR", "项目记忆/当前状态.md", "人读投影任务重复"))
        重放: dict[str, tuple[str, str, str]] = {}
        if 投影版本元组:
            for 任务, 任务事件 in 每任务事件.items():
                for 事件 in 任务事件:
                    事件版本 = 解析状态版本(事件.字段["状态版本"])
                    if 事件版本 and 事件版本 <= 投影版本元组:
                        迁移链 = [状态.strip() for 状态 in 事件.字段["迁移链"].split(">")]
                        重放[任务] = (事件.字段["计划版本"], 事件.字段["状态版本"], 迁移链[-1])
        if set(投影项) != set(重放):
            结果.append(检查项("ERROR", "项目记忆/当前状态.md", "人读投影任务集合与投影版本事件重放不一致"))
        for 任务, (计划版本, 状态版本, 生命周期) in 重放.items():
            投影 = 投影项.get(任务)
            if not 投影:
                continue
            对照 = {"计划版本": 计划版本, "状态版本": 状态版本, "生命周期投影": 生命周期}
            for 字段名, 预期 in 对照.items():
                if 投影[字段名] != 预期:
                    结果.append(检查项("ERROR", "项目记忆/当前状态.md", f"{任务} 投影 {字段名} 与事件重放不一致"))

    if "#352" in 当前项 and 当前项["#352"].字段["计划版本"] != "v0.1":
        三五二 = 当前项["#352"].字段
        版本匹配 = re.fullmatch(r"v(\d+)\.(\d+)", 三五二["计划版本"])
        if not 版本匹配 or (int(版本匹配.group(1)), int(版本匹配.group(2))) < (0, 2):
            结果.append(检查项("ERROR", 文件相对路径, "#352 非 v0.1 版本必须至少为 v0.2"))
        三五二计划路径, 三五二路径错误 = 解析仓库内路径(三五二["计划路径"])
        三五二文本 = 读取文本(三五二计划路径) if not 三五二路径错误 and 三五二计划路径 and 三五二计划路径.is_file() else ""
        最新事件 = 事件项.get(三五二["最新事件"])
        最新证据 = 最新事件.字段["证据"] if 最新事件 else ""
        证据片段 = 解析分号证据片段(最新证据)

        提前冻结合同 = {
            "计划提前冻结": (r"^计划提前冻结[：:]\s*PASS\s*$", ()),
            "执行门禁": (r"^执行门禁[：:]\s*(?P<值>.+)$", ("#339—#351", "固定候选", "#347", "调用点映射", "最终删除集")),
            "调用点映射合同": (r"^调用点映射合同[：:]\s*(?P<值>.+)$", ("#347", "实际路径", "blob", "解除门控状态事件")),
            "最终删除集合同": (r"^最终删除集合同[：:]\s*(?P<值>.+)$", ("第 5.2 节", "零消费者", "S0", "实际路径", "blob")),
            "执行白名单合同": (r"^执行白名单合同[：:]\s*(?P<值>.+)$", ("第 5 节", "固定提交", "实际路径", "blob")),
        }
        for 合同名, (合同模式, 必含词) in 提前冻结合同.items():
            合同匹配 = re.search(合同模式, 三五二文本, re.MULTILINE)
            if not 合同匹配:
                结果.append(检查项("ERROR", 三五二["计划路径"], f"#352 提前冻结计划缺少结构化 {合同名}"))
                continue
            合同值 = 合同匹配.groupdict().get("值", "")
            for 必含 in 必含词:
                if 必含 not in 合同值:
                    结果.append(检查项("ERROR", 三五二["计划路径"], f"#352 {合同名} 缺少合同要素: {必含}"))

        三五二已解除门控 = 三五二["当前生命周期"] != "依赖门控待执行"
        门禁片段 = [片段 for 片段 in 证据片段 if re.fullmatch(r"门禁结果=(?:PASS|FAIL)", 片段)]
        if not 三五二已解除门控:
            if re.search(r"^门禁结果[：:]\s*PASS\s*$", 三五二文本, re.MULTILINE) or 门禁片段 == ["门禁结果=PASS"]:
                结果.append(检查项("ERROR", 文件相对路径, "#352 依赖门控阶段不得提前声明最终门禁 PASS"))
        else:
            for 依赖编号 in range(339, 352):
                依赖 = f"#{依赖编号}"
                if 依赖 not in 当前项 or 当前项[依赖].字段["当前生命周期"] != "已完成 / 已归档":
                    结果.append(检查项("ERROR", 文件相对路径, f"#352 解除门控前 {依赖} 尚未完成 / 归档"))
            if 门禁片段 != ["门禁结果=PASS"]:
                结果.append(检查项("ERROR", 文件相对路径, "#352 解除门控事件缺少结构化门禁结果 PASS"))

            门禁合同片段 = [片段 for 片段 in 证据片段 if re.fullmatch(r"门禁合同=CUTOVER-GATE/v1", 片段)]
            if 门禁合同片段 != ["门禁合同=CUTOVER-GATE/v1"]:
                结果.append(检查项("ERROR", 文件相对路径, "#352 解除门控事件必须唯一绑定 CUTOVER-GATE/v1"))

            共同基线片段 = [片段 for 片段 in 证据片段 if re.fullmatch(r"共同集成基线=[0-9a-f]{40}", 片段)]
            if len(共同基线片段) != 1:
                结果.append(检查项("ERROR", 文件相对路径, "#352 解除门控事件必须唯一绑定共同集成基线"))
            else:
                共同集成基线 = 共同基线片段[0].split("=", 1)[1]
                if 运行Git("cat-file", "-e", f"{共同集成基线}^{{commit}}").returncode != 0:
                    结果.append(检查项("ERROR", 文件相对路径, "#352 共同集成基线不是可解析提交"))

            证据内容合同 = {
                "提供者固定集": ("CUTOVER-PROVIDERS/v1", ("#339", "#351", "计划版本", "固定提交", "文件清单", "提供合同")),
                "合同接受矩阵": ("CUTOVER-ACCEPT/v1", ("NT-C1/v0.2", "NT-C4/v0.2", "签名", "ABI", "所有权", "生命周期", "结果")),
                "调用点映射": ("CUTOVER-MAP/v1", ("调用点", "路径", "模块", "符号", "请求", "结果", "拒绝", "零命中")),
                "最终删除集": ("CUTOVER-DELETE/v1", ("删除", "保留只读", "非旧结构", "消费者", "零命中")),
                "执行白名单": ("CUTOVER-WHITELIST/v1", ("文件", "修改", "删除", "允许")),
            }
            证据Blob集合: set[str] = set()
            for 证据字段, (证据合同, 必含词) in 证据内容合同.items():
                Blob片段 = [片段 for 片段 in 证据片段 if re.fullmatch(rf"{证据字段}=[0-9a-f]{{40}}", 片段)]
                if len(Blob片段) != 1:
                    结果.append(检查项("ERROR", 文件相对路径, f"#352 解除门控事件缺少唯一 {证据字段} blob"))
                    continue
                证据Blob = Blob片段[0].split("=", 1)[1]
                if 证据Blob in 证据Blob集合:
                    结果.append(检查项("ERROR", 文件相对路径, "#352 五类门禁证据必须使用互不相同的内容 blob"))
                证据Blob集合.add(证据Blob)
                产物文本 = 读取Git对象文本(证据Blob)
                if 产物文本 is None:
                    结果.append(检查项("ERROR", 文件相对路径, f"#352 {证据字段} 不是可读取 blob"))
                    continue
                if 证据合同 not in 产物文本:
                    结果.append(检查项("ERROR", 文件相对路径, f"#352 {证据字段} blob 未声明 {证据合同}"))
                for 必含 in 必含词:
                    if 必含 not in 产物文本:
                        结果.append(检查项("ERROR", 文件相对路径, f"#352 {证据字段} blob 缺少内容合同要素: {必含}"))
            if "预冻结候选文件（非执行白名单）" in 三五二文本:
                结果.append(检查项("ERROR", 三五二["计划路径"], "#352 解除门控后仍保留 v0.1 预冻结候选口径"))

    非权威要求 = {
        "计划/计划索引.md": ["项目记忆/任务状态台账.md", "不维护待执行、派发、S0、执行、集成或完成状态"],
        "项目记忆/Codex任务队列.md": ["项目记忆/任务状态台账.md", "只登记执行顺序、直接依赖和解除条件"],
        "项目记忆/并行工作树登记表.md": ["项目记忆/任务状态台账.md", "Git 技术状态"],
        "项目记忆/当前状态.md": ["任务状态投影版本", "可能滞后", "不参与派发、S0"],
        "项目记忆/窗口交互记录.md": ["项目记忆/任务状态台账.md", "只追加任务生命周期迁移事件"],
    }
    for 相对路径, 必需文本 in 非权威要求.items():
        路径 = 根目录 / 相对路径
        if not 路径.is_file():
            结果.append(检查项("ERROR", 相对路径, "任务状态治理所需职责文件不存在"))
            continue
        当前文本 = 读取文本(路径)
        for 片段 in 必需文本:
            if 片段 not in 当前文本:
                结果.append(检查项("ERROR", 相对路径, f"缺少单一任务状态职责声明: {片段}"))
        if 相对路径 in {"计划/计划索引.md", "项目记忆/Codex任务队列.md"} and "| 当前状态 |" in 当前文本:
            结果.append(检查项("ERROR", 相对路径, "仍使用当前状态列复制任务生命周期"))
        if 相对路径 == "项目记忆/窗口交互记录.md" and "| 顶层任务 | 当前角色 | 当前职责 | 当前状态 |" in 当前文本:
            结果.append(检查项("ERROR", 相对路径, "窗口交互记录仍维护当前状态表"))
    队列文本 = 读取文本(根目录 / "项目记忆" / "Codex任务队列.md")
    if re.search(r"^##\s+[2-9]\.", 队列文本, re.MULTILINE):
        结果.append(检查项("ERROR", "项目记忆/Codex任务队列.md", "任务队列除顺序 / 依赖表外仍维护其它治理章节"))
    索引文本 = 读取文本(根目录 / "计划" / "计划索引.md")
    if "完成声明边界" in 索引文本 or "当前可声明" in 索引文本:
        结果.append(检查项("ERROR", "计划/计划索引.md", "计划索引仍维护阶段完成声明"))

    return 结果


def 打印结果(结果: list[检查项], 目录项数量: int) -> None:
    if not 结果:
        print(f"海中鱼巣正式规范与任务治理检查: {目录项数量} 份目录项全部通过")
        return

    错误数 = sum(项.等级 == "ERROR" for 项 in 结果)
    警告数 = sum(项.等级 == "WARN" for 项 in 结果)
    状态 = "通过" if 错误数 == 0 else "未通过"
    print(f"海中鱼巣正式规范与任务治理检查: {目录项数量} 份目录项，{状态}；{错误数} 个错误，{警告数} 个警告")
    for 项 in 结果:
        print(f"[{项.等级}] {项.路径}: {项.说明}")


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="检查海中鱼巣正式规范与任务状态单一权威一致性")
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
    结果.extend(检查任务状态治理())
    打印结果(结果, sum(项.编号 != "000" for 项 in 项目))

    if 参数.strict and any(项.等级 == "ERROR" for 项 in 结果):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
