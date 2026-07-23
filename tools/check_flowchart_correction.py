#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 文件规则：流程图纠偏检查工具只检查文档产物完整性和登记痕迹；不得修改项目文件、不得裁决机器事实。
"""
海中鱼巣流程图驱动代码纠偏产物检查工具。

默认只报告问题；传入 --strict 时，发现 ERROR 返回非零退出码。
"""

from __future__ import annotations

import argparse
import re
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class 检查项:
    等级: str
    路径: str
    说明: str


根目录 = Path(__file__).resolve().parents[1]


def 读取文本(路径文本: str) -> str:
    路径 = Path(路径文本)
    if not 路径.is_absolute():
        路径 = 根目录 / 路径
    return 路径.read_text(encoding="utf-8")


def 路径存在(路径文本: str | None) -> bool:
    if not 路径文本:
        return False
    路径 = Path(路径文本)
    if not 路径.is_absolute():
        路径 = 根目录 / 路径
    return 路径.exists()


def 相对或原样路径(路径文本: str | None) -> str:
    if not 路径文本:
        return ""
    路径 = Path(路径文本)
    if 路径.is_absolute():
        try:
            return str(路径.relative_to(根目录)).replace("\\", "/")
        except ValueError:
            return str(路径).replace("\\", "/")
    return 路径文本.replace("\\", "/")


def 提取_markdown_mermaid(图文本: str) -> str:
    匹配 = re.search(r"```mermaid\s*(.*?)\s*```", 图文本, re.S)
    return 匹配.group(1).strip() if 匹配 else ""


def 提取_html_mermaid(图文本: str) -> str:
    匹配 = re.search(r'<div\s+class=["\']mermaid["\']>\s*(.*?)\s*</div>', 图文本, re.S)
    return 匹配.group(1).strip() if 匹配 else ""


def 检查必需文件(参数: argparse.Namespace) -> list[检查项]:
    结果: list[检查项] = []
    必需路径 = {
        "现状流程图 Markdown": 参数.flowchart_md,
        "现状流程图 HTML": 参数.flowchart_html,
        "逐行代码映射表": 参数.mapping,
        "输入契约与调用语境表": 参数.contract,
        "非成功返回二分审查表": 参数.non_success,
        "偏差清单": 参数.deviation,
    }
    if 参数.require_plan or 参数.require_plan_registration:
        必需路径["修订计划"] = 参数.plan

    for 名称, 路径文本 in 必需路径.items():
        if not 路径存在(路径文本):
            结果.append(检查项("ERROR", 路径文本 or 名称, f"缺少{名称}"))
    return 结果


def 检查短语(路径文本: str, 短语列表: list[str], 名称: str) -> list[检查项]:
    结果: list[检查项] = []
    if not 路径存在(路径文本):
        return 结果
    文本 = 读取文本(路径文本)
    for 短语 in 短语列表:
        if 短语 not in 文本:
            结果.append(检查项("ERROR", 路径文本, f"{名称}缺少短语: {短语}"))
    return 结果


def 检查流程图(参数: argparse.Namespace) -> list[检查项]:
    结果: list[检查项] = []
    if not 路径存在(参数.flowchart_md) or not 路径存在(参数.flowchart_html):
        return 结果

    md文本 = 读取文本(参数.flowchart_md)
    html文本 = 读取文本(参数.flowchart_html)

    for 短语 in ["图类型：现状流程图", "逐行映射表", "```mermaid"]:
        if 短语 not in md文本:
            结果.append(检查项("ERROR", 参数.flowchart_md, f"现状流程图 Markdown 缺少: {短语}"))

    if "cdn.jsdelivr.net/npm/mermaid" not in html文本:
        结果.append(检查项("ERROR", 参数.flowchart_html, "现状流程图 HTML 缺少 Mermaid CDN import"))

    md图 = 提取_markdown_mermaid(md文本)
    html图 = 提取_html_mermaid(html文本)
    if not md图:
        结果.append(检查项("ERROR", 参数.flowchart_md, "未找到 Mermaid fenced block"))
    if not html图:
        结果.append(检查项("ERROR", 参数.flowchart_html, "未找到 Mermaid div"))
    if md图 and html图 and md图 != html图:
        结果.append(检查项("ERROR", 参数.flowchart_html, "HTML Mermaid 图文本与 Markdown 不一致"))

    return 结果


def 检查逐行映射表(路径文本: str) -> list[检查项]:
    return 检查短语(
        路径文本,
        ["文件", "行号", "代码行为", "流程图节点", "行为类型", "结构变化", "权威性", "非成功口径", "纠偏建议"],
        "逐行代码映射表",
    )


def 检查输入契约表(路径文本: str) -> list[检查项]:
    return 检查短语(
        路径文本,
        ["入口", "调用方", "输入字段", "输入来源", "上游是否保证有效", "允许逻辑内返回", "追根因触发条件", "结构变化", "验证方式"],
        "输入契约与调用语境表",
    )


def 检查非成功审查表(路径文本: str) -> list[检查项]:
    return 检查短语(
        路径文本,
        ["节点", "代码位置", "返回条件", "输入语境", "是否上游保证有效", "结构是否变化", "设计是否允许", "二分口径", "追根因解决", "逻辑内返回"],
        "非成功返回二分审查表",
    )


def 检查偏差清单(路径文本: str) -> list[检查项]:
    return 检查短语(
        路径文本,
        ["编号", "现状代码事实", "流程图", "详细设计", "偏差类型", "纠偏方向", "是否需要代码计划"],
        "偏差清单",
    )


def 检查计划(参数: argparse.Namespace) -> list[检查项]:
    结果: list[检查项] = []
    if not 参数.plan or not 路径存在(参数.plan):
        return 结果

    结果.extend(检查短语(
        参数.plan,
        ["依据", "允许文件", "禁止文件", "验证", "完成声明边界"],
        "修订计划",
    ))

    计划文本 = 读取文本(参数.plan)
    for 产物路径 in [参数.flowchart_md, 参数.mapping, 参数.contract, 参数.non_success, 参数.deviation]:
        相对路径 = 相对或原样路径(产物路径)
        if 相对路径 and 相对路径 not in 计划文本:
            结果.append(检查项("WARN", 参数.plan, f"修订计划未显式引用产物: {相对路径}"))

    return 结果


def 检查计划登记(参数: argparse.Namespace) -> list[检查项]:
    结果: list[检查项] = []
    if not 参数.require_plan_registration:
        return 结果
    if not 参数.plan:
        结果.append(检查项("ERROR", "计划", "要求登记计划但未提供 --plan"))
        return 结果

    计划路径 = 相对或原样路径(参数.plan)
    if not 路径存在(参数.plan_index):
        结果.append(检查项("ERROR", 参数.plan_index, "缺少计划索引"))
        return 结果
    索引文本 = 读取文本(参数.plan_index)
    if 计划路径 not in 索引文本 and Path(计划路径).name not in 索引文本:
        结果.append(检查项("ERROR", 参数.plan_index, f"计划索引未登记修订计划: {计划路径}"))

    return 结果


def 执行检查(参数: argparse.Namespace) -> list[检查项]:
    结果: list[检查项] = []
    结果.extend(检查必需文件(参数))
    结果.extend(检查流程图(参数))
    结果.extend(检查逐行映射表(参数.mapping))
    结果.extend(检查输入契约表(参数.contract))
    结果.extend(检查非成功审查表(参数.non_success))
    结果.extend(检查偏差清单(参数.deviation))
    if 参数.require_plan or 参数.require_plan_registration or 参数.plan:
        结果.extend(检查计划(参数))
    结果.extend(检查计划登记(参数))
    return 结果


def 打印结果(结果: list[检查项]) -> None:
    if not 结果:
        print("流程图驱动代码纠偏检查: 未发现问题")
        return

    print("流程图驱动代码纠偏检查:")
    for 项 in 结果:
        print(f"[{项.等级}] {项.路径}: {项.说明}")


def 写入临时文件(目录: Path, 名称: str, 内容: str) -> str:
    路径 = 目录 / 名称
    路径.write_text(内容, encoding="utf-8")
    return str(路径)


def 标准路径(路径文本: str) -> str:
    return 路径文本.replace("\\", "/")


def 自检() -> int:
    with tempfile.TemporaryDirectory() as 临时目录文本:
        临时目录 = Path(临时目录文本)
        图 = "flowchart TD\n    A[\"开始\"] --> B[\"结束\"]"
        md = 写入临时文件(临时目录, "现状流程图.md", f"# 图\n\n图类型：现状流程图\n逐行映射表：映射.md\n\n```mermaid\n{图}\n```\n")
        html = 写入临时文件(临时目录, "现状流程图.html", f"<div class=\"mermaid\">\n{图}\n</div><script type=\"module\">import mermaid from \"https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.esm.min.mjs\";</script>")
        mapping = 写入临时文件(临时目录, "映射.md", "| 文件 | 行号 | 代码行为 | 流程图节点 | 行为类型 | 结构变化 | 权威性 | 非成功口径 | 纠偏建议 |\n| --- | --- | --- | --- | --- | --- | --- | --- | --- |\n")
        contract = 写入临时文件(临时目录, "契约.md", "| 入口 | 调用方 | 输入字段 | 输入来源 | 上游是否保证有效 | 允许逻辑内返回 | 追根因触发条件 | 结构变化 | 验证方式 |\n| --- | --- | --- | --- | --- | --- | --- | --- | --- |\n")
        non_success = 写入临时文件(临时目录, "二分.md", "| 节点 | 代码位置 | 返回条件 | 输入语境 | 是否上游保证有效 | 结构是否变化 | 设计是否允许 | 二分口径 | 追根因解决 | 逻辑内返回 |\n| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |\n")
        deviation = 写入临时文件(临时目录, "偏差.md", "| 编号 | 现状代码事实 | 流程图 | 详细设计 | 偏差类型 | 纠偏方向 | 是否需要代码计划 |\n| --- | --- | --- | --- | --- | --- | --- |\n")
        plan = 写入临时文件(临时目录, "计划.md", f"# 计划\n\n## 依据\n{标准路径(md)}\n{标准路径(mapping)}\n{标准路径(contract)}\n{标准路径(non_success)}\n{标准路径(deviation)}\n## 允许文件\n待填\n## 禁止文件\n待填\n## 验证\n待填\n## 完成声明边界\n待填\n")
        plan_index = 写入临时文件(临时目录, "计划索引.md", f"{Path(plan).name}\n")

        参数 = argparse.Namespace(
            flowchart_md=md,
            flowchart_html=html,
            mapping=mapping,
            contract=contract,
            non_success=non_success,
            deviation=deviation,
            plan=plan,
            plan_index=plan_index,
            require_plan=True,
            require_plan_registration=True,
        )
        结果 = 执行检查(参数)
        打印结果(结果)
        return 1 if any(项.等级 == "ERROR" for 项 in 结果) else 0


def 构建参数() -> argparse.Namespace:
    解析器 = argparse.ArgumentParser(description="检查流程图驱动代码纠偏产物是否完整")
    解析器.add_argument("--flowchart-md", help="现状流程图 Markdown 路径")
    解析器.add_argument("--flowchart-html", help="现状流程图 HTML 路径")
    解析器.add_argument("--mapping", help="逐行代码映射表路径")
    解析器.add_argument("--contract", help="输入契约与调用语境表路径")
    解析器.add_argument("--non-success", help="非成功返回二分审查表路径")
    解析器.add_argument("--deviation", help="现状施工偏差清单路径")
    解析器.add_argument("--plan", help="修订计划路径")
    解析器.add_argument("--plan-index", default="计划/计划索引.md", help="计划索引路径")
    解析器.add_argument("--require-plan", action="store_true", help="要求提供修订计划")
    解析器.add_argument("--require-plan-registration", action="store_true", help="要求计划已登记到计划索引")
    解析器.add_argument("--strict", action="store_true", help="遇到 ERROR 时返回非零退出码")
    解析器.add_argument("--self-test", action="store_true", help="运行工具自检")
    return 解析器.parse_args()


def 主函数() -> int:
    参数 = 构建参数()
    if 参数.self_test:
        return 自检()

    结果 = 执行检查(参数)
    打印结果(结果)
    if 参数.strict and any(项.等级 == "ERROR" for 项 in 结果):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
