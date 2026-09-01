#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""检查服务导向集成验收清单的身份绑定、完整性和结论升级门禁。"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
import uuid
from dataclasses import dataclass
from pathlib import Path
from typing import Any


根目录 = Path(__file__).resolve().parents[1]
提交格式 = re.compile(r"^[0-9a-fA-F]{40}$")
标准场景 = ("成功", "精确重复", "合法拒绝", "外部失败", "并发", "崩溃恢复", "正式读回", "正式构建")
场景状态 = {"PASS", "FAIL", "NOT_APPLICABLE", "BLOCKED", "NOT_RUN"}
结论类型 = {
    "服务验收通过", "自检通过", "编译接入通过", "入口巡检通过", "局部可继续",
    "未观察到错误", "未正式接线", "事实失败", "环境阻断",
}
强结论短语 = ("服务验收通过", "能力完成", "项目完成", "正式接线完成")


@dataclass(frozen=True)
class 检查项:
    路径: str
    说明: str


def 取字典(对象: Any, 键: str) -> dict[str, Any]:
    值 = 对象.get(键) if isinstance(对象, dict) else None
    return 值 if isinstance(值, dict) else {}


def 取列表(对象: Any, 键: str) -> list[Any]:
    值 = 对象.get(键) if isinstance(对象, dict) else None
    return 值 if isinstance(值, list) else []


def 非空文本(值: Any) -> bool:
    return isinstance(值, str) and bool(值.strip())


def 文本列表有效(值: Any, 允许空: bool = False) -> bool:
    return isinstance(值, list) and (允许空 or bool(值)) and all(非空文本(项) for 项 in 值)


def 加错误(错误: list[检查项], 路径: str, 说明: str) -> None:
    错误.append(检查项(路径, 说明))


def 检查文本字段(错误: list[检查项], 对象: dict[str, Any], 键: str, 前缀: str) -> None:
    if not 非空文本(对象.get(键)):
        加错误(错误, f"{前缀}.{键}", "必须是非空文本")


def 检查文本列表(错误: list[检查项], 对象: dict[str, Any], 键: str, 前缀: str, 允许空: bool = False) -> None:
    if not 文本列表有效(对象.get(键), 允许空=允许空):
        要求 = "文本列表（允许为空）" if 允许空 else "非空文本列表"
        加错误(错误, f"{前缀}.{键}", f"必须是{要求}")


def 检查基线(清单: dict[str, Any], 错误: list[检查项]) -> None:
    基线 = 取字典(清单, "基线")
    if not 基线:
        加错误(错误, "基线", "缺少基线对象")
        return
    检查文本字段(错误, 基线, "仓库", "基线")
    for 键 in ("候选提交", "远端提交"):
        if not 提交格式.fullmatch(str(基线.get(键, ""))):
            加错误(错误, f"基线.{键}", "必须是 40 位 Git 提交")
    检查文本字段(错误, 基线, "远端分支", "基线")
    if 基线.get("已推送") is not True:
        加错误(错误, "基线.已推送", "必须明确为 true")
    if 基线.get("使用未提交内容") is not False:
        加错误(错误, "基线.使用未提交内容", "验收输入必须明确为 false")
    输入文件 = 取列表(基线, "输入文件")
    if not 输入文件:
        加错误(错误, "基线.输入文件", "至少绑定一项正式输入及 blob")
    for 序号, 项 in enumerate(输入文件):
        前缀 = f"基线.输入文件[{序号}]"
        if not isinstance(项, dict):
            加错误(错误, 前缀, "必须是对象")
            continue
        检查文本字段(错误, 项, "路径", 前缀)
        if not 提交格式.fullmatch(str(项.get("blob", ""))):
            加错误(错误, f"{前缀}.blob", "必须是 40 位 Git blob")


def 检查服务(清单: dict[str, Any], 错误: list[检查项]) -> None:
    服务 = 取字典(清单, "服务切片")
    if not 服务:
        加错误(错误, "服务切片", "缺少服务切片对象")
        return
    检查文本字段(错误, 服务, "名称", "服务切片")
    检查文本字段(错误, 服务, "上级目标", "服务切片")
    检查文本列表(错误, 服务, "合法消费者", "服务切片")
    检查文本列表(错误, 服务, "正式入口", "服务切片")
    检查文本列表(错误, 服务, "输入前提", "服务切片")
    检查文本列表(错误, 服务, "安全继续条件", "服务切片")
    结果 = 取字典(服务, "结构化结果")
    for 键 in ("成功", "非成功", "禁止假成功"):
        检查文本字段(错误, 结果, 键, "服务切片.结构化结果")
    if not isinstance(服务.get("写入权威事实"), bool):
        加错误(错误, "服务切片.写入权威事实", "必须是布尔值")
    终态 = 取字典(服务, "权威终态")
    if 服务.get("写入权威事实") is True:
        for 键 in ("事实来源", "预期终态", "独立读回"):
            检查文本字段(错误, 终态, 键, "服务切片.权威终态")
    elif not 非空文本(终态.get("不适用原因")):
        加错误(错误, "服务切片.权威终态.不适用原因", "不写权威事实时必须说明不适用原因")


def 检查环境(清单: dict[str, Any], 错误: list[检查项]) -> None:
    环境 = 取字典(清单, "环境")
    if not 环境:
        加错误(错误, "环境", "缺少环境对象")
        return
    检查文本字段(错误, 环境, "运行配置", "环境")
    检查文本字段(错误, 环境, "专用命名域", "环境")
    检查文本列表(错误, 环境, "外部资源", "环境", 允许空=True)
    if not isinstance(环境.get("就绪"), bool):
        加错误(错误, "环境.就绪", "必须是布尔值")
    if 环境.get("就绪") is False and not 非空文本(环境.get("阻断原因")):
        加错误(错误, "环境.阻断原因", "环境未就绪时必须具名说明")


def 检查函数巡检(清单: dict[str, Any], 错误: list[检查项]) -> dict[str, Any]:
    巡检 = 取字典(清单, "函数巡检")
    if not 巡检:
        加错误(错误, "函数巡检", "缺少全函数编译接入与入口巡检对象")
        return {}
    检查文本字段(错误, 巡检, "范围", "函数巡检")
    检查文本字段(错误, 巡检, "清单来源", "函数巡检")
    检查文本列表(错误, 巡检, "证据", "函数巡检")
    检查文本列表(错误, 巡检, "未覆盖函数", "函数巡检", 允许空=True)
    数量字段 = ("目标函数数", "排除函数数", "编译通过数", "入口巡检通过数", "逻辑异常数")
    for 键 in 数量字段:
        值 = 巡检.get(键)
        if not isinstance(值, int) or isinstance(值, bool) or 值 < 0:
            加错误(错误, f"函数巡检.{键}", "必须是非负整数")
    if any(not isinstance(巡检.get(键), int) or isinstance(巡检.get(键), bool) for 键 in 数量字段):
        return 巡检
    目标 = 巡检["目标函数数"]
    排除 = 巡检["排除函数数"]
    编译 = 巡检["编译通过数"]
    入口 = 巡检["入口巡检通过数"]
    if 排除 > 目标:
        加错误(错误, "函数巡检.排除函数数", "不能大于目标函数数")
    可验收 = max(0, 目标 - 排除)
    if 编译 > 可验收:
        加错误(错误, "函数巡检.编译通过数", "不能大于未排除函数数")
    if 入口 > 编译:
        加错误(错误, "函数巡检.入口巡检通过数", "不能大于编译通过数")
    return 巡检


def 检查证据状态(对象: dict[str, Any], 前缀: str, 错误: list[检查项]) -> str:
    状态 = 对象.get("状态")
    if 状态 not in 场景状态:
        加错误(错误, f"{前缀}.状态", f"必须是 {sorted(场景状态)} 之一")
        return ""
    证据 = 对象.get("证据")
    if 状态 in {"PASS", "FAIL", "BLOCKED"} and not 文本列表有效(证据):
        加错误(错误, f"{前缀}.证据", f"{状态} 必须带非空证据列表")
    if 状态 == "NOT_APPLICABLE" and not 非空文本(对象.get("不适用原因")):
        加错误(错误, f"{前缀}.不适用原因", "NOT_APPLICABLE 必须说明合同级理由")
    if 状态 == "NOT_RUN" and not 非空文本(对象.get("未运行原因")):
        加错误(错误, f"{前缀}.未运行原因", "NOT_RUN 必须说明原因")
    return str(状态)


def 检查场景(清单: dict[str, Any], 错误: list[检查项]) -> dict[str, dict[str, Any]]:
    场景组 = 取列表(清单, "场景")
    if not 场景组:
        加错误(错误, "场景", "缺少最小风险矩阵")
        return {}
    按类型: dict[str, dict[str, Any]] = {}
    已见标识: set[str] = set()
    for 序号, 场景 in enumerate(场景组):
        前缀 = f"场景[{序号}]"
        if not isinstance(场景, dict):
            加错误(错误, 前缀, "必须是对象")
            continue
        标识 = 场景.get("标识")
        类型 = 场景.get("类型")
        if not 非空文本(标识):
            加错误(错误, f"{前缀}.标识", "必须是非空文本")
        elif 标识 in 已见标识:
            加错误(错误, f"{前缀}.标识", "场景标识重复")
        else:
            已见标识.add(标识)
        if 类型 not in 标准场景 and 类型 != "自定义":
            加错误(错误, f"{前缀}.类型", "不是标准场景或自定义场景")
        elif 类型 in 按类型 and 类型 != "自定义":
            加错误(错误, f"{前缀}.类型", "标准场景类型重复")
        elif 类型 != "自定义":
            按类型[str(类型)] = 场景
        if not isinstance(场景.get("必需"), bool):
            加错误(错误, f"{前缀}.必需", "必须是布尔值")
        状态 = 检查证据状态(场景, 前缀, 错误)
        if 场景.get("必需") is True and 状态 in {"NOT_APPLICABLE", "NOT_RUN"}:
            加错误(错误, 前缀, "必需场景不能标为不适用或未运行")
        if 状态 == "PASS":
            检查文本字段(错误, 场景, "结构化结果", 前缀)
            检查文本字段(错误, 场景, "终态观察", 前缀)
    for 类型 in 标准场景:
        if 类型 not in 按类型:
            加错误(错误, "场景", f"缺少标准场景: {类型}")
    return 按类型


def 检查边界(清单: dict[str, Any], 错误: list[检查项]) -> dict[str, dict[str, Any]]:
    边界 = 取字典(清单, "边界检查")
    if not 边界:
        加错误(错误, "边界检查", "缺少边界检查对象")
        return {}
    结果: dict[str, dict[str, Any]] = {}
    for 键 in ("正式接线", "自检隔离", "正式构建隔离"):
        项 = 取字典(边界, 键)
        if not 项:
            加错误(错误, f"边界检查.{键}", "缺少检查项")
            continue
        检查证据状态(项, f"边界检查.{键}", 错误)
        结果[键] = 项
    return 结果


def 检查结论(
    清单: dict[str, Any],
    函数巡检: dict[str, Any],
    场景: dict[str, dict[str, Any]],
    边界: dict[str, dict[str, Any]],
    错误: list[检查项],
) -> None:
    结论 = 取字典(清单, "结论")
    if not 结论:
        加错误(错误, "结论", "缺少结论对象")
        return
    类型 = 结论.get("类型")
    if 类型 not in 结论类型:
        加错误(错误, "结论.类型", f"必须是 {sorted(结论类型)} 之一")
        return
    检查文本列表(错误, 结论, "允许声明", "结论")
    检查文本列表(错误, 结论, "禁止声明", "结论")
    检查文本列表(错误, 结论, "未覆盖范围", "结论", 允许空=True)

    场景状态表 = {键: 值.get("状态") for 键, 值 in 场景.items()}
    全部场景 = [项 for 项 in 取列表(清单, "场景") if isinstance(项, dict)]
    环境 = 取字典(清单, "环境")
    服务 = 取字典(清单, "服务切片")
    目标函数数 = 函数巡检.get("目标函数数")
    排除函数数 = 函数巡检.get("排除函数数")
    可验收函数数 = (
        目标函数数 - 排除函数数
        if isinstance(目标函数数, int) and isinstance(排除函数数, int)
        else -1
    )
    编译全覆盖 = 可验收函数数 >= 0 and 函数巡检.get("编译通过数") == 可验收函数数
    入口全覆盖 = 编译全覆盖 and 函数巡检.get("入口巡检通过数") == 可验收函数数
    无逻辑异常 = 函数巡检.get("逻辑异常数") == 0

    if 类型 in {"编译接入通过", "入口巡检通过", "局部可继续", "服务验收通过"}:
        if not 编译全覆盖:
            加错误(错误, "函数巡检.编译通过数", f"{类型}要求未排除目标函数全部编译通过")
        if not 无逻辑异常:
            加错误(错误, "函数巡检.逻辑异常数", f"{类型}要求当前范围不存在未收口逻辑异常")
    if 类型 in {"入口巡检通过", "局部可继续", "服务验收通过"} and not 入口全覆盖:
        加错误(错误, "函数巡检.入口巡检通过数", f"{类型}要求未排除目标函数全部完成入口巡检")

    if 类型 == "编译接入通过" and 取字典(边界, "正式构建隔离").get("状态") != "PASS":
        加错误(错误, "边界检查.正式构建隔离", "编译接入通过要求正式构建隔离 PASS")

    if 类型 == "服务验收通过":
        if 环境.get("就绪") is not True:
            加错误(错误, "结论.类型", "环境未就绪时不能形成服务验收通过")
        for 键 in ("正式接线", "自检隔离", "正式构建隔离"):
            if 取字典(边界, 键).get("状态") != "PASS":
                加错误(错误, f"边界检查.{键}", "服务验收通过要求该边界 PASS")
        if 场景状态表.get("成功") != "PASS":
            加错误(错误, "场景.成功", "服务验收通过要求成功场景 PASS")
        for 项 in 全部场景:
            if 项.get("必需") is True and 项.get("状态") != "PASS":
                加错误(错误, f"场景.{项.get('标识', '?')}", "服务验收通过要求全部必需场景 PASS")
            if 项.get("状态") in {"FAIL", "BLOCKED"}:
                加错误(错误, f"场景.{项.get('标识', '?')}", "存在失败或阻断时不能形成服务验收通过")
        if 服务.get("写入权威事实") is True and 场景状态表.get("正式读回") != "PASS":
            加错误(错误, "场景.正式读回", "写权威事实的服务必须通过正式独立读回")

    if 类型 == "环境阻断":
        if 环境.get("就绪") is not False:
            加错误(错误, "环境.就绪", "环境阻断要求明确为 false")
        if not any(项.get("状态") == "BLOCKED" for 项 in 全部场景):
            加错误(错误, "场景", "环境阻断至少需要一个 BLOCKED 场景")

    if 类型 == "事实失败":
        边界失败 = any(项.get("状态") == "FAIL" for 项 in 边界.values())
        场景失败 = any(项.get("状态") == "FAIL" for 项 in 全部场景)
        if not 边界失败 and not 场景失败:
            加错误(错误, "结论.类型", "事实失败至少需要一项 FAIL 证据")

    if 类型 == "未正式接线" and 取字典(边界, "正式接线").get("状态") == "PASS":
        加错误(错误, "结论.类型", "正式接线已经 PASS 时不能写未正式接线")

    if 类型 in {"自检通过", "局部可继续"} and not any(项.get("状态") == "PASS" for 项 in 全部场景):
        加错误(错误, "结论.类型", f"{类型} 至少需要一个带证据的 PASS 场景")

    if 类型 != "服务验收通过":
        for 序号, 声明 in enumerate(取列表(结论, "允许声明")):
            if isinstance(声明, str) and any(短语 in 声明 for 短语 in 强结论短语):
                加错误(错误, f"结论.允许声明[{序号}]", "较弱结论不得升级为服务或项目完成")


def 检查清单(清单: Any) -> list[检查项]:
    错误: list[检查项] = []
    if not isinstance(清单, dict):
        return [检查项("$", "清单根必须是 JSON 对象")]
    if 清单.get("schema_version") != 1:
        加错误(错误, "schema_version", "当前只接受版本 1")
    检查文本字段(错误, 清单, "验收标识", "$")
    检查基线(清单, 错误)
    检查服务(清单, 错误)
    检查环境(清单, 错误)
    函数巡检 = 检查函数巡检(清单, 错误)
    场景 = 检查场景(清单, 错误)
    边界 = 检查边界(清单, 错误)
    检查结论(清单, 函数巡检, 场景, 边界, 错误)
    return 错误


def 运行_git(仓库: Path, *参数: str) -> tuple[int, str]:
    结果 = subprocess.run(
        ["git", "-C", str(仓库), *参数],
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
        check=False,
    )
    return 结果.returncode, (结果.stdout or 结果.stderr).strip()


def 验证_git(清单: dict[str, Any]) -> list[检查项]:
    错误: list[检查项] = []
    基线 = 取字典(清单, "基线")
    仓库文本 = str(基线.get("仓库", ""))
    仓库 = Path(仓库文本) if 仓库文本 else 根目录
    if not 仓库.is_absolute():
        仓库 = 根目录 / 仓库
    候选 = str(基线.get("候选提交", ""))
    远端分支 = str(基线.get("远端分支", ""))
    记录远端 = str(基线.get("远端提交", ""))
    if not 仓库.is_dir():
        return [检查项("基线.仓库", f"仓库目录不存在: {仓库}")]
    退出码, _ = 运行_git(仓库, "cat-file", "-e", f"{候选}^{{commit}}")
    候选存在 = 退出码 == 0
    if not 候选存在:
        加错误(错误, "基线.候选提交", "本地 Git 中不存在该提交")
    退出码, _ = 运行_git(仓库, "cat-file", "-e", f"{记录远端}^{{commit}}")
    记录远端存在 = 退出码 == 0
    if not 记录远端存在:
        加错误(错误, "基线.远端提交", "本地 Git 中不存在该提交")
    退出码, 当前远端 = 运行_git(仓库, "rev-parse", f"{远端分支}^{{commit}}")
    远端分支存在 = 退出码 == 0
    if not 远端分支存在:
        加错误(错误, "基线.远端分支", "本地 Git 中不存在记录的远端分支")
    if 候选存在 and 记录远端存在:
        退出码, _ = 运行_git(仓库, "merge-base", "--is-ancestor", 候选, 记录远端)
        if 退出码 != 0:
            加错误(错误, "基线.远端提交", "记录远端提交不包含候选提交")
    if 记录远端存在 and 远端分支存在:
        退出码, _ = 运行_git(仓库, "merge-base", "--is-ancestor", 记录远端, 当前远端)
        if 退出码 != 0:
            加错误(错误, "基线.远端分支", "当前远端分支不包含记录远端提交")
    for 序号, 项 in enumerate(取列表(基线, "输入文件")):
        if not isinstance(项, dict):
            continue
        路径 = str(项.get("路径", ""))
        预期 = str(项.get("blob", ""))
        if not 候选存在:
            continue
        退出码, 实际 = 运行_git(仓库, "rev-parse", f"{候选}:{路径}")
        if 退出码 != 0:
            加错误(错误, f"基线.输入文件[{序号}]", "候选提交中不存在该路径")
        elif 实际.lower() != 预期.lower():
            加错误(错误, f"基线.输入文件[{序号}].blob", f"与候选提交不一致，实际 {实际}")
    return 错误


def 模板() -> dict[str, Any]:
    空提交 = "0" * 40
    场景 = []
    for 序号, 类型 in enumerate(标准场景, start=1):
        场景.append({
            "标识": f"S{序号:02d}",
            "类型": 类型,
            "必需": 类型 in {"成功", "正式读回", "正式构建"},
            "状态": "NOT_RUN",
            "证据": [],
            "结构化结果": "待验收",
            "终态观察": "待验收",
            "不适用原因": "",
            "未运行原因": "尚未执行",
        })
    return {
        "schema_version": 1,
        "验收标识": "待命名",
        "基线": {
            "仓库": str(根目录),
            "候选提交": 空提交,
            "远端分支": "origin/main",
            "远端提交": 空提交,
            "已推送": False,
            "使用未提交内容": False,
            "输入文件": [{"路径": "待填写", "blob": 空提交}],
        },
        "服务切片": {
            "名称": "待填写",
            "上级目标": "待填写",
            "合法消费者": ["待填写"],
            "正式入口": ["待填写"],
            "输入前提": ["待填写"],
            "结构化结果": {"成功": "待填写", "非成功": "待填写", "禁止假成功": "待填写"},
            "写入权威事实": True,
            "权威终态": {"事实来源": "待填写", "预期终态": "待填写", "独立读回": "待填写", "不适用原因": ""},
            "安全继续条件": ["待填写"],
        },
        "环境": {"运行配置": "待填写", "外部资源": [], "专用命名域": "待填写", "就绪": False, "阻断原因": "尚未准备"},
        "函数巡检": {
            "范围": "待填写",
            "清单来源": "待填写",
            "目标函数数": 0,
            "排除函数数": 0,
            "编译通过数": 0,
            "入口巡检通过数": 0,
            "逻辑异常数": 0,
            "证据": ["待填写"],
            "未覆盖函数": [],
        },
        "场景": 场景,
        "边界检查": {
            "正式接线": {"状态": "NOT_RUN", "证据": [], "不适用原因": "", "未运行原因": "尚未执行"},
            "自检隔离": {"状态": "NOT_RUN", "证据": [], "不适用原因": "", "未运行原因": "尚未执行"},
            "正式构建隔离": {"状态": "NOT_RUN", "证据": [], "不适用原因": "", "未运行原因": "尚未执行"},
        },
        "结论": {"类型": "环境阻断", "允许声明": ["尚未开始验收"], "禁止声明": ["不得声明服务通过"], "未覆盖范围": ["全部场景"]},
    }


def 自检() -> int:
    有效 = 模板()
    有效["基线"]["候选提交"] = "1" * 40
    有效["基线"]["远端提交"] = "1" * 40
    有效["基线"]["已推送"] = True
    有效["基线"]["输入文件"][0]["blob"] = "2" * 40
    有效["环境"]["就绪"] = True
    有效["环境"]["阻断原因"] = ""
    有效["函数巡检"].update({"范围": "具名服务影响闭包", "清单来源": "函数身份表@测试", "目标函数数": 1, "编译通过数": 1, "入口巡检通过数": 1, "证据": ["证据/函数巡检.json"]})
    for 场景 in 有效["场景"]:
        if 场景["必需"]:
            场景.update({"状态": "PASS", "证据": ["证据/结果.txt"], "结构化结果": "符合合同", "终态观察": "符合合同", "未运行原因": ""})
        else:
            场景.update({"状态": "NOT_APPLICABLE", "不适用原因": "当前合同不包含该风险", "未运行原因": ""})
    for 项 in 有效["边界检查"].values():
        项.update({"状态": "PASS", "证据": ["证据/边界.txt"], "未运行原因": ""})
    有效["结论"] = {"类型": "服务验收通过", "允许声明": ["该具名服务切片验收通过"], "禁止声明": ["不证明其它服务完成"], "未覆盖范围": ["其它服务"]}
    if 检查清单(有效):
        print("自检失败：有效清单被拒绝")
        return 1

    无接线 = json.loads(json.dumps(有效, ensure_ascii=False))
    无接线["边界检查"]["正式接线"]["状态"] = "FAIL"
    if not any("正式接线" in 项.路径 for 项 in 检查清单(无接线)):
        print("自检失败：服务通过未阻止正式接线失败")
        return 1

    弱结论越权 = json.loads(json.dumps(有效, ensure_ascii=False))
    弱结论越权["结论"]["类型"] = "自检通过"
    弱结论越权["结论"]["允许声明"] = ["服务验收通过"]
    if not any("较弱结论" in 项.说明 for 项 in 检查清单(弱结论越权)):
        print("自检失败：较弱结论允许越权声明")
        return 1

    仅编译 = json.loads(json.dumps(有效, ensure_ascii=False))
    仅编译["函数巡检"]["入口巡检通过数"] = 0
    仅编译["结论"] = {
        "类型": "编译接入通过",
        "允许声明": ["目标函数编译接入通过"],
        "禁止声明": ["不得声明入口巡检或服务完成"],
        "未覆盖范围": ["运行期入口和业务结果"],
    }
    if 检查清单(仅编译):
        print("自检失败：合法编译接入弱结论被拒绝")
        return 1

    伪入口 = json.loads(json.dumps(仅编译, ensure_ascii=False))
    伪入口["结论"]["类型"] = "入口巡检通过"
    if not any("入口巡检通过数" in 项.路径 for 项 in 检查清单(伪入口)):
        print("自检失败：入口巡检结论未阻止零入口覆盖")
        return 1

    临时根 = Path("D:/TEMP/海中鱼巣/DATA-L1-FINAL-CLOSURE") / f"integration-self-test-{uuid.uuid4().hex}"
    try:
        临时根.mkdir(parents=True)

        def 必须_git(*参数: str) -> str:
            退出码, 输出 = 运行_git(临时根, *参数)
            if 退出码 != 0:
                raise RuntimeError(f"git {' '.join(参数)}: {输出}")
            return 输出

        必须_git("init", "-q")
        必须_git("config", "user.name", "DATA-L1 checker")
        必须_git("config", "user.email", "data-l1-checker@example.invalid")
        (临时根 / "input.txt").write_text("candidate\n", encoding="utf-8")
        必须_git("add", "--", "input.txt")
        必须_git("commit", "-q", "-m", "candidate A")
        候选甲 = 必须_git("rev-parse", "HEAD")
        输入blob = 必须_git("rev-parse", f"{候选甲}:input.txt")
        (临时根 / "record-anchor.txt").write_text("recorded remote\n", encoding="utf-8")
        必须_git("add", "--", "record-anchor.txt")
        必须_git("commit", "-q", "-m", "recorded remote R")
        记录远端甲 = 必须_git("rev-parse", "HEAD")
        候选树 = 必须_git("rev-parse", f"{候选甲}^{{tree}}")
        非祖先提交 = 必须_git("commit-tree", 候选树, "-m", "unrelated B")
        (临时根 / "governance.txt").write_text("later governance\n", encoding="utf-8")
        必须_git("add", "--", "governance.txt")
        必须_git("commit", "-q", "-m", "later governance G")
        后继治理 = 必须_git("rev-parse", "HEAD")

        Git清单 = json.loads(json.dumps(有效, ensure_ascii=False))
        Git清单["基线"].update({
            "仓库": str(临时根),
            "候选提交": 候选甲,
            "远端分支": "origin/main",
            "远端提交": 记录远端甲,
            "输入文件": [{"路径": "input.txt", "blob": 输入blob}],
        })

        def 预期_git(名称: str, 清单: dict[str, Any], 错误路径: str | None) -> bool:
            实际错误 = 验证_git(清单)
            if 错误路径 is None and 实际错误:
                print(f"自检失败：{名称} 被拒绝: {实际错误}")
                return False
            if 错误路径 is not None and not any(项.路径 == 错误路径 for 项 in 实际错误):
                print(f"自检失败：{名称} 未命中 {错误路径}: {实际错误}")
                return False
            return True

        必须_git("update-ref", "refs/remotes/origin/main", 记录远端甲)
        if not 预期_git("A <= R <= origin/main", Git清单, None):
            return 1
        必须_git("update-ref", "refs/remotes/origin/main", 后继治理)
        if not 预期_git("远端从 R 推进到 G", Git清单, None):
            return 1

        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["候选提交"] = "f" * 40
        if not 预期_git("候选不存在", 变体, "基线.候选提交"):
            return 1
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["远端提交"] = "e" * 40
        if not 预期_git("记录远端不存在", 变体, "基线.远端提交"):
            return 1
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["远端分支"] = "origin/missing"
        if not 预期_git("远端分支不存在", 变体, "基线.远端分支"):
            return 1
        必须_git("update-ref", "refs/remotes/origin/main", 非祖先提交)
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["远端提交"] = 非祖先提交
        if not 预期_git("候选不在记录远端历史", 变体, "基线.远端提交"):
            return 1
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        if not 预期_git("记录远端不在当前远端历史", 变体, "基线.远端分支"):
            return 1
        必须_git("update-ref", "refs/remotes/origin/main", 后继治理)
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["输入文件"][0]["路径"] = "missing.txt"
        if not 预期_git("输入路径不存在", 变体, "基线.输入文件[0]"):
            return 1
        变体 = json.loads(json.dumps(Git清单, ensure_ascii=False))
        变体["基线"]["输入文件"][0]["blob"] = "d" * 40
        if not 预期_git("输入 blob 不同", 变体, "基线.输入文件[0].blob"):
            return 1
    except (OSError, RuntimeError) as 异常:
        print(f"自检失败：Git 耐久性矩阵无法执行: {异常}")
        return 1
    finally:
        shutil.rmtree(临时根, ignore_errors=True)

    print("集成验收清单检查器自检: 通过（含 9 项 Git 耐久性正负例）")
    return 0


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="检查海中鱼巣服务导向集成验收清单")
    解析器.add_argument("--manifest", help="JSON 验收清单路径")
    解析器.add_argument("--verify-git", action="store_true", help="核对候选提交、远端包含关系和输入 blob")
    解析器.add_argument("--strict", action="store_true", help="发现错误时返回非零退出码")
    解析器.add_argument("--self-test", action="store_true", help="运行检查器自检")
    解析器.add_argument("--print-template", action="store_true", help="向标准输出打印清单模板")
    参数 = 解析器.parse_args()

    if 参数.self_test:
        return 自检()
    if 参数.print_template:
        print(json.dumps(模板(), ensure_ascii=False, indent=2))
        return 0
    if not 参数.manifest:
        解析器.error("必须提供 --manifest，或使用 --self-test / --print-template")

    路径 = Path(参数.manifest)
    if not 路径.is_absolute():
        路径 = 根目录 / 路径
    try:
        清单 = json.loads(路径.read_text(encoding="utf-8-sig"))
    except (OSError, json.JSONDecodeError) as 异常:
        print(f"集成验收清单读取失败: {异常}")
        return 1

    错误 = 检查清单(清单)
    if 参数.verify_git and isinstance(清单, dict):
        错误.extend(验证_git(清单))
    if not 错误:
        print("集成验收清单检查: 通过（仅证明清单与结论门禁完整，不证明运行事实真实）")
        return 0
    print(f"集成验收清单检查: {len(错误)} 个错误")
    for 项 in 错误:
        print(f"[ERROR] {项.路径}: {项.说明}")
    return 1 if 参数.strict else 0


if __name__ == "__main__":
    sys.exit(主函数())
