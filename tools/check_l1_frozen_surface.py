#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""检查 DATA-L1 已冻结公开面、领域例外和生产依赖边界。"""

from __future__ import annotations

import argparse
import collections
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path


根目录 = Path(__file__).resolve().parents[1]
清单路径 = 根目录 / "tools" / "l1_frozen_surface_manifest.json"
服务路径 = 根目录 / "海中鱼巣" / "核心" / "服务.L1事实基座.ixx"
冻结路径 = (
    "海中鱼巣/核心/服务.L1事实基座.ixx",
    "海中鱼巣/核心/L1公共事实.数据.h",
    "海中鱼巣/核心/L1中性CRUD.数据.h",
    "海中鱼巣/核心/L1所有者范围CRUD.数据.h",
    "海中鱼巣/核心/L1事实基座持久恢复.数据.h",
)
操作组 = (
    "L1事实基座服务",
    "L1所有者范围写端口",
    "L1所有者范围签发器",
    "L1事实基座运行包",
    "free_functions",
)
预期操作 = {
    "L1事实基座服务": (
        "尝试读取中性一致当前投影", "提交中性写集", "读取中性写入首次结果",
        "执行L1物理清理", "读取中性当前节点", "读取中性当前关系",
        "读取中性当前源关系组", "读取中性当前目标关系组", "读取中性历史关系组",
        "读取中性历史属性值组", "读取中性当前值", "读取中性当前属性",
        "读取中性历史事实", "读取中性当前事实代次", "读取所有者范围当前节点",
        "读取所有者范围当前关系", "读取所有者范围当前值",
        "按来源读取所有者范围全部当前值", "按属性类型读取所有者范围全部当前值",
        "读取所有者范围当前源关系组", "读取所有者范围当前目标关系组",
        "读取所有者范围历史关系组", "读取所有者范围历史属性值组",
        "读取所有者范围历史事实", "尝试读取所有者范围一致当前投影",
        "尝试读取所有者范围一致关系类型闭包投影", "读取当前结构所有者",
        "读取历史结构所有者",
    ),
    "L1所有者范围写端口": (
        "所有者身份", "有效", "绑定于", "提交所有者范围中性写集",
        "提交跨所有者原子事务", "提交三分区原子事务_v2",
        "提交有限N分区原子事务_v3", "读取首次写入材料",
    ),
    "L1所有者范围签发器": (
        "建立所有者范围", "重新签发所有者范围写端口", "退出所有者范围",
    ),
    "L1事实基座运行包": ("读取服务", "所有者范围签发器"),
    "free_functions": ("建立L1事实基座运行包", "建立L1事实基座持久运行包_v1"),
}
允许顶层键 = {
    "schema_version", "specification", "surface_contract_status",
    "data_l1_status", "accepted_commit", "acceptance_record", "source_module",
    "public_operations", "frozen_source_files", "legacy_domain_exceptions",
    "technical_token_exceptions", "forbidden_domain_tokens",
}


def 读取文本(路径: Path) -> str:
    return 路径.read_text(encoding="utf-8-sig")


def 规范字节(路径: Path) -> bytes:
    return 路径.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n")


def 去注释与字符串(文本: str) -> str:
    return re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"', " ", 文本,
                  flags=re.DOTALL)


def 读取清单(错误: list[str]) -> dict:
    try:
        清单 = json.loads(读取文本(清单路径), object_pairs_hook=dict)
    except (OSError, UnicodeError, json.JSONDecodeError) as 异常:
        错误.append(f"冻结清单无法读取: {异常}")
        return {}
    if set(清单) != 允许顶层键:
        错误.append("冻结清单顶层字段集合不等于 schema 1")
    if 清单.get("schema_version") != 1 or 清单.get("specification") != "4080-v1.0":
        错误.append("冻结清单 schema/specification 不匹配")
    if 清单.get("surface_contract_status") != "FROZEN":
        错误.append("surface_contract_status 必须为 FROZEN")
    if 清单.get("source_module") != 冻结路径[0]:
        错误.append("source_module 不是唯一 L1 服务模块")
    return 清单


def 检查状态(清单: dict, 错误: list[str]) -> None:
    状态 = 清单.get("data_l1_status")
    提交 = 清单.get("accepted_commit")
    记录 = 清单.get("acceptance_record")
    if 状态 == "PENDING_ACCEPTANCE":
        if 提交 is not None or 记录 is not None:
            错误.append("PENDING_ACCEPTANCE 不得登记 accepted_commit/acceptance_record")
        return
    if 状态 != "FROZEN":
        错误.append("data_l1_status 只允许 PENDING_ACCEPTANCE/FROZEN")
        return
    if not isinstance(提交, str) or not re.fullmatch(r"[0-9a-f]{40}", 提交):
        错误.append("FROZEN 必须登记 40 位 accepted_commit")
        return
    if not isinstance(记录, str) or not (根目录 / 记录).is_file():
        错误.append("FROZEN 必须登记存在的 acceptance_record")
        return
    命令 = [sys.executable, str(根目录 / "tools" / "check_integration_acceptance.py"),
          "--manifest", str(根目录 / 记录), "--verify-git", "--strict"]
    if subprocess.run(命令, cwd=根目录, capture_output=True).returncode != 0:
        错误.append("FROZEN 的独立集成验收记录/commit 未通过检查")


def 检查操作与指纹(清单: dict, 错误: list[str]) -> None:
    操作 = 清单.get("public_operations")
    if not isinstance(操作, dict) or tuple(操作) != 操作组:
        错误.append("公开操作组或组序不匹配")
        return
    for 组 in 操作组:
        值 = 操作.get(组)
        if not isinstance(值, list) or any(not isinstance(x, str) or not x for x in 值):
            错误.append(f"公开操作组 {组} 形状非法")
            continue
        if len(值) != len(set(值)):
            错误.append(f"公开操作组 {组} 含重复名称")
        if tuple(值) != 预期操作[组]:
            错误.append(f"公开操作组 {组} 与 4080 v1.0 不相等")
    if sum(len(操作.get(组, [])) for 组 in 操作组) != 43:
        错误.append("公开操作总数不是 43")
    服务 = 读取文本(服务路径) if 服务路径.is_file() else ""
    for 组, 名称组 in 预期操作.items():
        for 名称 in 名称组:
            if not re.search(rf"(?<![\w\u4e00-\u9fff]){re.escape(名称)}\s*\(", 服务):
                错误.append(f"服务源码缺少冻结操作: {组}::{名称}")

    文件项 = 清单.get("frozen_source_files")
    if not isinstance(文件项, list) or [x.get("path") for x in 文件项 if isinstance(x, dict)] != list(冻结路径):
        错误.append("冻结文件集合或顺序不匹配")
        return
    for 项 in 文件项:
        路径 = 根目录 / 项["path"]
        if not 路径.is_file():
            错误.append(f"冻结文件不存在: {项['path']}")
            continue
        字节 = 规范字节(路径)
        if 项.get("normalized_size") != len(字节):
            错误.append(f"冻结文件规范化长度变化: {项['path']}")
        if 项.get("sha256") != hashlib.sha256(字节).hexdigest():
            错误.append(f"冻结文件 SHA-256 变化: {项['path']}")


def 检查领域例外(清单: dict, 错误: list[str]) -> None:
    例外 = 清单.get("legacy_domain_exceptions")
    if not isinstance(例外, list) or len(例外) != 9:
        错误.append("legacy_domain_exceptions 必须恰有 9 个限定项")
        return
    唯一键 = {(x.get("path"), x.get("owner"), x.get("member"), x.get("kind"))
           for x in 例外 if isinstance(x, dict)}
    if len(唯一键) != 9 or any(x.get("count") != 1 for x in 例外):
        错误.append("legacy 例外必须限定到唯一物理成员且次数为 1")

    头 = 去注释与字符串(读取文本(根目录 / 冻结路径[3]))
    服务 = 去注释与字符串(读取文本(服务路径))
    规则 = {
        (冻结路径[3], "L1跨所有者原子事务参与者序号", "状态", "enum_item"):
            r"\b状态\s*=\s*1\b",
        (冻结路径[3], "L1跨所有者原子事务参与者序号", "动态", "enum_item"):
            r"\b动态\s*=\s*2\b",
        (冻结路径[3], "L1跨所有者原子事务请求", "状态写集", "field"):
            r"L1跨所有者原子参与者写集\s+状态写集\s*;",
        (冻结路径[3], "L1跨所有者原子事务请求", "动态写集", "field"):
            r"L1跨所有者原子参与者写集\s+动态写集\s*;",
        (冻结路径[3], "L1跨所有者原子事务结果", "状态编码映射", "field"):
            r">>\s+状态编码映射\s*;",
        (冻结路径[3], "L1跨所有者原子事务结果", "动态编码映射", "field"):
            r">>\s+动态编码映射\s*;",
        (冻结路径[0], "L1所有者范围写端口", "提交跨所有者原子事务", "method"):
            r"L1跨所有者原子事务结果\s+提交跨所有者原子事务\s*\(",
        (冻结路径[0], "L1所有者范围写端口::提交跨所有者原子事务", "状态写集", "field_access"):
            r"请求\.状态写集\.所有者",
        (冻结路径[0], "L1所有者范围写端口::提交跨所有者原子事务", "动态写集", "field_access"):
            r"请求\.动态写集\.所有者",
    }
    for 项 in 例外:
        键 = (项.get("path"), 项.get("owner"), 项.get("member"), 项.get("kind"))
        模式 = 规则.get(键)
        if not 模式:
            错误.append(f"未知 legacy 例外: {键}")
            continue
        文本 = 服务 if 项["path"] == 冻结路径[0] else 头
        if len(re.findall(模式, 文本)) != 项["count"]:
            错误.append(f"legacy 例外缺失、移动、重复或变种: {键}")

    禁词 = 清单.get("forbidden_domain_tokens")
    if 禁词 != ["存在", "场景", "特征", "动态", "因果", "需求", "任务", "方法", "动作", "安全", "服务"]:
        错误.append("forbidden_domain_tokens 与 4080 不一致")
        return
    技术例外 = {(x.get("path"), x.get("identifier")): x.get("count")
            for x in 清单.get("technical_token_exceptions", []) if isinstance(x, dict)}
    允许领域 = {
        (冻结路径[3], "动态"): 1,
        (冻结路径[3], "动态写集"): 1,
        (冻结路径[3], "动态编码映射"): 1,
        (冻结路径[0], "动态写集"): 1,
    }
    允许 = collections.Counter({**技术例外, **允许领域})
    实际 = collections.Counter()
    标识模式 = re.compile(r"[A-Za-z_\u4e00-\u9fff][A-Za-z0-9_\u4e00-\u9fff]*")
    for 相对路径 in 冻结路径:
        文本 = 去注释与字符串(读取文本(根目录 / 相对路径))
        for 标识 in 标识模式.findall(文本):
            if any(词 in 标识 for 词 in 禁词):
                实际[(相对路径, 标识)] += 1
    if 实际 != 允许:
        多余 = 实际 - 允许
        缺失 = 允许 - 实际
        if 多余:
            错误.append(f"出现未限定领域标识符: {dict(多余)}")
        if 缺失:
            错误.append(f"限定领域/技术例外缺失或次数变化: {dict(缺失)}")


def 检查规范与生产边界(清单: dict, 错误: list[str]) -> None:
    规范4080 = 读取文本(根目录 / "规范" / "4080_子规范_DATA-L1通用操作全集与冻结门禁.md")
    目录 = 读取文本(根目录 / "规范" / "规范目录.md")
    if "DATA-L1 通用操作全集 / 目标公开合同状态 = FROZEN" not in 规范4080:
        错误.append("4080 未冻结通用操作/目标公开合同")
    预期整体 = f"DATA-L1 整体能力状态 = {清单.get('data_l1_status')}"
    if 预期整体 not in 规范4080:
        错误.append("4080 整体状态与冻结清单不一致")
    for 片段 in (
        "4015_子规范_L1简化事实基座.md",
        "4070_子规范_权威结构快照恢复候选与运行期原子发布.md",
        "4080_子规范_DATA-L1通用操作全集与冻结门禁.md",
        "有限 N 分区原子事务 v3",
        "目标合同 FROZEN，整体 PENDING_ACCEPTANCE",
    ):
        if 片段 not in 目录:
            错误.append(f"规范目录缺少冻结摘要: {片段}")

    import模式 = re.compile(r"\bimport\s+海中鱼巣\.核心\.仓库\.L1事实基座\s*;")
    命中: list[str] = []
    for 路径 in (根目录 / "海中鱼巣").rglob("*"):
        if 路径.suffix.lower() not in {".ixx", ".cpp", ".h"} or not 路径.is_file():
            continue
        try:
            if import模式.search(读取文本(路径)):
                命中.append(路径.relative_to(根目录).as_posix())
        except UnicodeError:
            continue
    if 命中 != [冻结路径[0]]:
        错误.append(f"仓库模块直接 import 不是唯一服务入口: {命中}")

    允许许可 = {
        "海中鱼巣/核心/服务.L1事实基座.ixx": 2,
        "海中鱼巣/核心/L1事实基座.数据.h": 1,
        "海中鱼巣/核心/L1所有者范围CRUD.数据.h": 5,
        "海中鱼巣/核心/L1中性CRUD.数据.h": 2,
    }
    实际许可: dict[str, int] = {}
    for 路径 in (根目录 / "海中鱼巣" / "核心").rglob("*"):
        if 路径.suffix.lower() not in {".ixx", ".cpp", ".h"} or not 路径.is_file():
            continue
        数 = 去注释与字符串(读取文本(路径)).count("许可拒绝")
        if 数:
            实际许可[路径.relative_to(根目录).as_posix()] = 数
    if 实际许可 != 允许许可:
        错误.append(f"许可拒绝出现位置/次数变化: {实际许可}")


def 检查冻结面() -> list[str]:
    错误: list[str] = []
    清单 = 读取清单(错误)
    if not 清单:
        return 错误
    检查状态(清单, 错误)
    检查操作与指纹(清单, 错误)
    检查领域例外(清单, 错误)
    检查规范与生产边界(清单, 错误)
    return 错误


def 主函数() -> int:
    解析器 = argparse.ArgumentParser(description="检查 DATA-L1 冻结公开面")
    解析器.add_argument("--strict", action="store_true", help="发现错误时返回非零")
    参数 = 解析器.parse_args()
    错误 = 检查冻结面()
    if 错误:
        print(f"DATA-L1 冻结面检查未通过：{len(错误)} 个错误")
        for 项 in 错误:
            print(f"[ERROR] {项}")
    else:
        print("DATA-L1 冻结面检查通过：43 项操作、5 个冻结文件与生产边界一致")
    return 1 if 参数.strict and 错误 else 0


if __name__ == "__main__":
    sys.exit(主函数())
