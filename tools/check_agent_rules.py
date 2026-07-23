#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""检查 Codex 代理规则与项目规范、计划列表之间的职责分离。"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path


根目录 = Path(__file__).resolve().parents[1]
代理入口 = 根目录 / "AGENTS.md"
代理规则目录 = 根目录 / ".codex" / "rules"
技能目录 = 根目录 / ".codex" / "skills"
计划索引 = 根目录 / "计划" / "计划索引.md"

必需代理规则 = (
    ".codex/rules/设计执行双窗口交互规则.md",
    ".codex/rules/多工作树并发与集成规则.md",
)

已退出混放路径 = (
    "规范/设计执行双窗口交互规范.md",
    "规范/多工作树并发与集成规范.md",
)

禁止当前治理引用 = (
    "项目记忆/",
    "项目记忆\\",
    "任务状态台账.md",
    "Codex任务队列.md",
    "并行工作树登记表.md",
    "窗口交互记录.md",
    "当前状态.md",
    "计划树",
    "静态激活",
)


def 读取文本(路径: Path) -> str:
    return 路径.read_text(encoding="utf-8-sig")


def 相对路径(路径: Path) -> str:
    return 路径.relative_to(根目录).as_posix()


def 收集代理文件() -> list[Path]:
    文件 = [代理入口]
    文件.extend(sorted(代理规则目录.glob("*.md")))
    文件.extend(sorted(技能目录.glob("*/SKILL.md")))
    文件.extend(sorted(技能目录.glob("*/agents/*.yaml")))
    return [路径 for 路径 in 文件 if 路径.is_file()]


def 检查() -> list[str]:
    错误: list[str] = []

    if not 代理入口.is_file():
        错误.append("缺少 AGENTS.md")
    if not 计划索引.is_file():
        错误.append("缺少 计划/计划索引.md")

    for 路径文本 in 必需代理规则:
        if not (根目录 / 路径文本).is_file():
            错误.append(f"缺少代理规则: {路径文本}")

    for 路径文本 in 已退出混放路径:
        if (根目录 / 路径文本).exists():
            错误.append(f"代理规则仍混放在项目规范目录: {路径文本}")

    for 文件路径 in 收集代理文件():
        文本 = 读取文本(文件路径)
        for 禁止词 in 禁止当前治理引用:
            if 禁止词 in 文本:
                错误.append(f"{相对路径(文件路径)} 仍含当前治理禁用词: {禁止词}")
        for 旧路径 in 已退出混放路径:
            if 旧路径 in 文本:
                错误.append(f"{相对路径(文件路径)} 仍引用已退出路径: {旧路径}")

    if 代理入口.is_file():
        入口文本 = 读取文本(代理入口)
        for 路径文本 in 必需代理规则:
            if 路径文本 not in 入口文本:
                错误.append(f"AGENTS.md 未引用代理规则: {路径文本}")

    if 计划索引.is_file():
        索引文本 = 读取文本(计划索引)
        必需表头 = "| 路由 | 计划 | 直接依赖 / 静态门禁 | 计划状态 |"
        if 必需表头 not in 索引文本:
            错误.append("计划索引缺少计划列表状态表头")
        if "| 执行通道 |" in 索引文本:
            错误.append("计划索引仍保存 Codex 执行通道")
        for 状态 in ("待激活", "可执行", "已退出"):
            if 状态 not in 索引文本:
                错误.append(f"计划索引缺少计划状态: {状态}")

    return 错误


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="检查 Codex 代理规则与项目规范、计划列表的职责分离")
    解析器.add_argument("--strict", action="store_true", help="发现错误时返回非零退出码")
    参数 = 解析器.parse_args()

    错误 = 检查()
    if not 错误:
        print("Codex 代理规则职责分离检查: 通过")
        return 0

    print(f"Codex 代理规则职责分离检查: {len(错误)} 个错误")
    for 项 in 错误:
        print(f"[ERROR] {项}")
    return 1 if 参数.strict else 0


if __name__ == "__main__":
    sys.exit(主函数())
