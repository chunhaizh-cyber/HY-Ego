#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
海中鱼巣规范治理轻量检查工具。

本工具是从旧鱼巢 `tools/check_specs.py` 的治理用途改造而来，不复制旧脚本路径
和旧项目目录假设。默认只报告问题；只有显式传入 `--strict` 时才用非零退出码
阻断后续流程。
"""

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


根目录 = Path(__file__).resolve().parents[1]

必需文件 = [
    "AGENTS.md",
    "规范/000_项目规则总纲.md",
    "规范/001_规则迁移清单.md",
    "规范/工具与规则迁移治理规范.md",
    "计划/计划索引.md",
    "项目记忆/当前状态.md",
    "项目记忆/Codex任务队列.md",
]

必需规则短语 = {
    "AGENTS.md": [
        "函数名、变量名、类内业务成员名必须使用中文",
        "每个完成声明必须给出输入入口、核心处理、结构承载、输出变化、调用路径和验证方式",
        "工具与旧规则迁移治理",
    ],
    "规范/000_项目规则总纲.md": [
        "完成声明必须同时说明",
        "禁止用以下材料承载机器逻辑",
        "工具与规则迁移治理规范",
    ],
    "规范/001_规则迁移清单.md": [
        "限载读取",
        "六类证据",
        "tools/check_specs.py",
    ],
    "规范/工具与规则迁移治理规范.md": [
        "已迁移工具",
        "待确认工具",
        "暂缓或禁止迁移工具",
        "不得直接复制旧项目工具源码",
    ],
}

禁止直接迁移模式 = [
    re.compile(r"docs/工程图谱/05_规则原子索引\.md"),
    re.compile(r"规范/规范目录\.md"),
    re.compile(r"tools/sql_projection"),
    re.compile(r"fishnest_sql_projection\.py"),
]


def 读取文本(相对路径: str) -> str:
    return (根目录 / 相对路径).read_text(encoding="utf-8")


def 检查必需文件() -> list[检查项]:
    结果: list[检查项] = []
    for 相对路径 in 必需文件:
        if not (根目录 / 相对路径).exists():
            结果.append(检查项("ERROR", 相对路径, "缺少规范治理必需文件"))
    return 结果


def 检查必需短语() -> list[检查项]:
    结果: list[检查项] = []
    for 相对路径, 短语列表 in 必需规则短语.items():
        文件路径 = 根目录 / 相对路径
        if not 文件路径.exists():
            continue
        文本 = 读取文本(相对路径)
        for 短语 in 短语列表:
            if 短语 not in 文本:
                结果.append(检查项("ERROR", 相对路径, f"缺少规则短语: {短语}"))
    return 结果


def 检查旧路径残留() -> list[检查项]:
    结果: list[检查项] = []
    检查范围 = [
        "AGENTS.md",
        "规范/000_项目规则总纲.md",
        "规范/001_规则迁移清单.md",
        "计划/计划索引.md",
        "项目记忆/当前状态.md",
        "项目记忆/Codex任务队列.md",
    ]
    for 相对路径 in 检查范围:
        文件路径 = 根目录 / 相对路径
        if not 文件路径.exists():
            continue
        文本 = 读取文本(相对路径)
        for 模式 in 禁止直接迁移模式:
            if 模式.search(文本):
                结果.append(检查项("WARN", 相对路径, f"发现旧鱼巢路径或暂缓工具引用: {模式.pattern}"))
    return 结果


def 打印结果(结果: list[检查项]) -> None:
    if not 结果:
        print("海中鱼巣规范治理检查: 未发现问题")
        return

    print("海中鱼巣规范治理检查:")
    for 项 in 结果:
        print(f"[{项.等级}] {项.路径}: {项.说明}")


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="海中鱼巣规范治理轻量检查工具")
    解析器.add_argument("--strict", action="store_true", help="遇到 ERROR 时返回非零退出码")
    参数 = 解析器.parse_args()

    结果: list[检查项] = []
    结果.extend(检查必需文件())
    结果.extend(检查必需短语())
    结果.extend(检查旧路径残留())
    打印结果(结果)

    if 参数.strict and any(项.等级 == "ERROR" for 项 in 结果):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
