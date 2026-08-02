#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 文件规则：本工具只生成代码流程图维护候选，不修改正式图谱、规范、设计、计划或代码。
"""按当前源码与正式图谱基线生成不可覆盖的代码流程图增量维护候选。"""

from __future__ import annotations

import argparse
import datetime as 日期时间
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import uuid
from pathlib import Path
from typing import Callable


仓库根目录 = Path(__file__).resolve().parents[1]
默认配置路径 = Path(__file__).with_name("代码流程图自动维护.json")


class 维护错误(RuntimeError):
    pass


def 标准路径(路径: Path) -> str:
    return 路径.as_posix()


def 读取JSON(路径: Path) -> dict:
    return json.loads(路径.read_text(encoding="utf-8-sig"))


def 规范JSON(内容: object) -> bytes:
    return json.dumps(内容, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")


def 字节哈希(内容: bytes) -> str:
    return hashlib.sha256(内容).hexdigest()


def 文件哈希(路径: Path) -> str:
    return 字节哈希(路径.read_bytes())


def 运行Git(参数组: list[str], 根目录: Path, 允许失败: bool = False) -> bytes:
    结果 = subprocess.run(
        ["git", "-c", "core.quotepath=false", "-C", str(根目录), *参数组],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if 结果.returncode != 0 and not 允许失败:
        说明 = 结果.stderr.decode("utf-8", errors="replace").strip()
        raise 维护错误(f"Git 命令失败: {' '.join(参数组)}: {说明}")
    return 结果.stdout


def 当前提交(根目录: Path) -> str:
    return 运行Git(["rev-parse", "HEAD"], 根目录).decode().strip()


def 路径属于源码(路径文本: str, 配置: dict) -> bool:
    路径 = Path(路径文本)
    if 路径.suffix.lower() not in {值.lower() for 值 in 配置["源码扩展名"]}:
        return False
    规范文本 = 路径文本.replace("\\", "/")
    return any(
        规范文本 == 根.rstrip("/") or 规范文本.startswith(根.rstrip("/") + "/")
        for 根 in 配置["源码根目录"]
    )


def 收集相对差异(根目录: Path, 基线提交: str, 配置: dict) -> list[dict]:
    路径参数 = [*配置["源码根目录"], *配置["工程配置文件"]]
    输出 = 运行Git(
        ["diff", "--name-status", "-z", "--no-renames", 基线提交, "--", *路径参数],
        根目录,
    )
    片段 = [项.decode("utf-8", errors="surrogateescape") for 项 in 输出.split(b"\0") if 项]
    差异表: dict[str, str] = {}
    for 索引 in range(0, len(片段), 2):
        if 索引 + 1 >= len(片段):
            raise 维护错误("无法解析 git --name-status -z 输出")
        状态 = 片段[索引][0]
        差异表[片段[索引 + 1].replace("\\", "/")] = 状态

    未跟踪输出 = 运行Git(
        ["ls-files", "--others", "--exclude-standard", "-z", "--", *路径参数],
        根目录,
    )
    for 原始路径 in 未跟踪输出.split(b"\0"):
        if 原始路径:
            差异表.setdefault(
                原始路径.decode("utf-8", errors="surrogateescape").replace("\\", "/"), "A"
            )

    结果 = []
    for 路径文本, 状态 in sorted(差异表.items()):
        路径 = 根目录 / Path(路径文本)
        结果.append(
            {
                "路径": 路径文本,
                "状态": 状态,
                "类别": "源码" if 路径属于源码(路径文本, 配置) else "工程配置",
                "当前内容哈希": 文件哈希(路径) if 路径.is_file() else None,
            }
        )
    return 结果


def 读取图谱(根目录: Path, 配置: dict) -> tuple[dict, dict, dict, dict]:
    输入 = 配置["图谱输入"]
    return tuple(读取JSON(根目录 / Path(输入[键])) for 键 in ("函数身份表", "项目调用边表", "外部边界表", "制图队列"))


def 断言图谱输入已提交(根目录: Path, 配置: dict) -> None:
    路径组 = list(配置["图谱输入"].values())
    输出 = 运行Git(["status", "--porcelain=v1", "-z", "--", *路径组], 根目录)
    if 输出:
        变更 = [项.decode("utf-8", errors="replace") for 项 in 输出.split(b"\0") if 项]
        raise 维护错误(f"正式图谱输入存在未提交 WIP，禁止作为自动候选基线: {'; '.join(变更)}")


def 形成输入身份(根目录: Path, 配置路径: Path, 配置: dict, 基线提交: str) -> dict:
    输入 = 配置["图谱输入"]
    图谱哈希 = {
        键: {
            "路径": 路径文本,
            "内容哈希": 文件哈希(根目录 / Path(路径文本)),
        }
        for 键, 路径文本 in 输入.items()
    }
    return {
        "当前提交": 当前提交(根目录),
        "正式代码基线": 基线提交,
        "维护工具哈希": 文件哈希(Path(__file__).resolve()),
        "配置哈希": 文件哈希(配置路径),
        "图谱输入": 图谱哈希,
        "相对正式代码基线的观察差异": 收集相对差异(根目录, 基线提交, 配置),
    }


def 可比较输入身份(输入身份: dict) -> dict:
    """排除仅用于追溯的全仓提交；无关文档提交不应制造新候选或输入漂移。"""
    return {键: 值 for 键, 值 in 输入身份.items() if 键 != "当前提交"}


def 校验图谱端点(身份表: dict, 边表: dict, 队列: dict) -> list[str]:
    问题组: list[str] = []
    身份组 = 身份表.get("functions", [])
    身份集合 = {项.get("function_id") for 项 in 身份组}
    if len(身份集合) != len(身份组) or None in 身份集合:
        问题组.append("函数身份表存在空身份或重复身份")
    for 边 in 边表.get("edges", []):
        for 端点名 in ("caller", "callee"):
            if 边.get(端点名) not in 身份集合:
                问题组.append(f"调用边 {边.get('edge_id')} 的 {端点名} 悬空: {边.get(端点名)}")
    队列身份 = [项.get("function_id") for 项 in 队列.get("entries", [])]
    if set(队列身份) != 身份集合:
        问题组.append("制图队列与当前函数身份集合不一致")
    return 问题组


def 读取旧行变化(根目录: Path, 基线提交: str, 路径文本: str) -> list[tuple[int, int]]:
    输出 = 运行Git(
        ["diff", "--unified=0", "--no-color", "--no-ext-diff", 基线提交, "--", 路径文本],
        根目录,
        允许失败=True,
    ).decode("utf-8", errors="replace")
    结果: list[tuple[int, int]] = []
    for 匹配 in re.finditer(r"^@@\s+-(\d+)(?:,(\d+))?\s+\+(\d+)(?:,(\d+))?\s+@@", 输出, re.M):
        起始 = int(匹配.group(1))
        数量 = int(匹配.group(2) or "1")
        结果.append((起始, max(数量, 1)))
    return 结果


def 变化命中函数(函数: dict, 变化范围: list[tuple[int, int]]) -> bool:
    函数起始 = int(函数.get("definition_line", 0))
    函数结束 = int(函数.get("definition_end_line", 函数起始))
    for 变化起始, 变化数量 in 变化范围:
        变化结束 = 变化起始 + 变化数量 - 1
        if 变化起始 <= 函数结束 + 1 and 变化结束 >= 函数起始:
            return True
    return False


def 计算直接影响(
    根目录: Path,
    基线提交: str,
    配置: dict,
    身份表: dict,
    差异组: list[dict],
) -> tuple[set[str], list[dict], list[dict]]:
    按文件: dict[str, list[dict]] = {}
    for 函数 in 身份表.get("functions", []):
        按文件.setdefault(函数["file"].replace("\\", "/"), []).append(函数)

    直接影响: set[str] = set()
    未登记源码: list[dict] = []
    删除函数: list[dict] = []
    if any(项["类别"] == "工程配置" for 项 in 差异组):
        直接影响.update(函数["function_id"] for 函数 in 身份表.get("functions", []))

    for 差异 in 差异组:
        if 差异["类别"] != "源码":
            continue
        路径文本 = 差异["路径"]
        文件函数 = 按文件.get(路径文本, [])
        if 差异["状态"] == "A" and not 文件函数:
            未登记源码.append({**差异, "原因": "新增源码尚无正式函数身份"})
            continue
        if 差异["状态"] == "D":
            直接影响.update(项["function_id"] for 项 in 文件函数)
            删除函数.extend(
                {"函数身份": 项["function_id"], "完整签名": 项["complete_signature"], "原文件": 路径文本}
                for 项 in 文件函数
            )
            if not 文件函数:
                未登记源码.append({**差异, "原因": "删除源码没有可回查的正式函数身份"})
            continue
        if not 文件函数:
            未登记源码.append({**差异, "原因": "变更源码未出现在正式函数身份表"})
            continue
        变化范围 = 读取旧行变化(根目录, 基线提交, 路径文本)
        命中组 = [项 for 项 in 文件函数 if 变化命中函数(项, 变化范围)]
        if not 命中组:
            命中组 = 文件函数
        直接影响.update(项["function_id"] for 项 in 命中组)
    return 直接影响, 未登记源码, 删除函数


def 计算调用者闭包(直接影响: set[str], 边表: dict, 深度上限: int) -> dict[str, int]:
    入向表: dict[str, set[str]] = {}
    for 边 in 边表.get("edges", []):
        入向表.setdefault(边["callee"], set()).add(边["caller"])
    深度表 = {身份: 0 for 身份 in 直接影响}
    当前层 = set(直接影响)
    当前深度 = 0
    while 当前层 and (深度上限 < 0 or 当前深度 < 深度上限):
        当前深度 += 1
        下一层: set[str] = set()
        for 被调方 in 当前层:
            for 调用方 in 入向表.get(被调方, set()):
                if 调用方 not in 深度表:
                    深度表[调用方] = 当前深度
                    下一层.add(调用方)
        当前层 = 下一层
    return 深度表


def 建议扫描命令(差异组: list[dict]) -> list[dict]:
    当前源码 = [项["路径"] for 项 in 差异组 if 项["类别"] == "源码" and 项["状态"] != "D"]
    Cpp组 = [路径 for 路径 in 当前源码 if Path(路径).suffix.lower() in {".cpp", ".cxx", ".cc"}]
    模块组 = [路径 for 路径 in 当前源码 if Path(路径).suffix.lower() == ".ixx"]
    建议组: list[dict] = []
    if Cpp组:
        建议组.append({"类型": "普通Cpp候选", "文件": Cpp组, "入口": "tools/clang_ast/批量扫描.Cpp候选流程图.ps1"})
    if 模块组:
        建议组.append(
            {
                "类型": "模块候选",
                "文件": 模块组,
                "入口": [
                    "tools/clang_ast/扫描.MSVC模块依赖.ps1",
                    "tools/clang_ast/扫描.Clang模块候选流程图.ps1",
                ],
                "说明": "先重建完整模块依赖，再按变更文件提供的模块身份抽取 Clang AST 候选",
            }
        )
    return 建议组


def 生成分析(根目录: Path, 配置: dict, 模式: str, 身份表: dict, 边表: dict, 队列: dict, 输入身份: dict) -> dict:
    模式配置 = 配置["模式"][模式]
    图谱问题 = 校验图谱端点(身份表, 边表, 队列) if 模式配置["检查全部图谱端点"] else []
    差异组 = 输入身份["相对正式代码基线的观察差异"]
    直接影响, 未登记源码, 删除函数 = 计算直接影响(
        根目录, 输入身份["正式代码基线"], 配置, 身份表, 差异组
    )
    闭包深度 = 计算调用者闭包(直接影响, 边表, int(模式配置["调用者闭包深度"]))
    队列表 = {项["function_id"]: 项 for 项 in 队列.get("entries", [])}
    身份索引 = {项["function_id"]: 项 for 项 in 身份表.get("functions", [])}

    直接影响明细 = [身份索引[身份] for 身份 in sorted(直接影响) if 身份 in 身份索引]
    闭包明细 = [
        {
            "函数身份": 身份,
            "传播深度": 深度,
            "完整签名": 身份索引.get(身份, {}).get("complete_signature"),
            "文件": 身份索引.get(身份, {}).get("file"),
        }
        for 身份, 深度 in sorted(闭包深度.items(), key=lambda 项: (项[1], 项[0]))
    ]
    待复核图 = []
    待绘制 = []
    for 身份, 深度 in sorted(闭包深度.items(), key=lambda 项: (项[1], 项[0])):
        登记 = 队列表.get(身份, {})
        目标 = {
            "函数身份": 身份,
            "传播深度": 深度,
            "完整签名": 身份索引.get(身份, {}).get("complete_signature"),
            "图路径": 登记.get("chart_path", ""),
            "逐行映射路径": 登记.get("mapping_path", ""),
            "原制图状态": 登记.get("chart_status", "未登记"),
        }
        if 登记.get("chart_status") == "已制图":
            待复核图.append(目标)
        else:
            待绘制.append(目标)
    待绘制.extend({"函数身份": None, "原制图状态": "未登记源码候选", **项} for 项 in 未登记源码)

    return {
        "图谱问题": 图谱问题,
        "观察差异": 差异组,
        "直接影响函数": 直接影响明细,
        "调用者影响闭包": 闭包明细,
        "待复核流程图": 待复核图,
        "新增待绘制候选": 待绘制,
        "删除函数候选": 删除函数,
        "AST候选扫描建议": 建议扫描命令(差异组),
    }


def 查找相同候选(候选根目录: Path, 指纹: str) -> Path | None:
    if not 候选根目录.exists():
        return None
    for 清单路径 in 候选根目录.glob("*/manifest.json"):
        try:
            if 读取JSON(清单路径).get("候选指纹") == 指纹:
                return 清单路径.parent
        except (OSError, json.JSONDecodeError):
            continue
    return None


def 写入JSON一次(路径: Path, 内容: object) -> None:
    路径.parent.mkdir(parents=True, exist_ok=True)
    with 路径.open("x", encoding="utf-8", newline="\n") as 文件:
        json.dump(内容, 文件, ensure_ascii=False, indent=2)
        文件.write("\n")


def 形成摘要(清单: dict, 分析: dict) -> str:
    return "\n".join(
        [
            "# 代码流程图自动维护候选",
            "",
            f"- 状态：{清单['状态']}",
            f"- 模式：{清单['模式']}",
            f"- 正式代码基线：`{清单['输入身份']['正式代码基线']}`",
            f"- 观察提交：`{清单['输入身份']['当前提交']}`",
            f"- 观察差异：{len(分析['观察差异'])}",
            f"- 直接影响函数：{len(分析['直接影响函数'])}",
            f"- 调用者影响闭包：{len(分析['调用者影响闭包'])}",
            f"- 已制图待复核：{len(分析['待复核流程图'])}",
            f"- 新增待绘制候选：{len(分析['新增待绘制候选'])}",
            f"- 删除函数候选：{len(分析['删除函数候选'])}",
            "",
            "本批次只保存机器候选。正式 Mermaid、逐行行为解释、非成功分类、稳定函数身份和调用边仍须维护智能体读取源码后复核；本候选不修改正式图谱，也不证明能力完成。",
            "",
        ]
    )


def 发布批次(状态根目录: Path, 类别: str, 清单: dict, 分析: dict) -> Path:
    批次根 = 状态根目录 / 类别
    批次根.mkdir(parents=True, exist_ok=True)
    时间 = 日期时间.datetime.now().strftime("%Y%m%dT%H%M%S%f")
    目录名 = f"{时间}-{清单['候选指纹'][:12]}"
    临时目录 = 状态根目录 / f".tmp-{uuid.uuid4().hex}"
    临时目录.mkdir(parents=True, exist_ok=False)
    try:
        写入JSON一次(临时目录 / "manifest.json", 清单)
        写入JSON一次(临时目录 / "01_观察差异.json", 分析["观察差异"])
        写入JSON一次(临时目录 / "02_直接影响函数.json", 分析["直接影响函数"])
        写入JSON一次(临时目录 / "03_调用者影响闭包.json", 分析["调用者影响闭包"])
        写入JSON一次(临时目录 / "04_待复核流程图.json", 分析["待复核流程图"])
        写入JSON一次(临时目录 / "05_新增待绘制候选.json", 分析["新增待绘制候选"])
        写入JSON一次(临时目录 / "06_删除函数候选.json", 分析["删除函数候选"])
        写入JSON一次(临时目录 / "07_AST候选扫描建议.json", 分析["AST候选扫描建议"])
        写入JSON一次(临时目录 / "08_图谱问题.json", 分析["图谱问题"])
        with (临时目录 / "摘要.md").open("x", encoding="utf-8", newline="\n") as 文件:
            文件.write(形成摘要(清单, 分析))
        目标目录 = 批次根 / 目录名
        临时目录.replace(目标目录)
        return 目标目录
    except Exception:
        if 临时目录.exists():
            shutil.rmtree(临时目录)
        raise


def 取得租约(状态根目录: Path, 输入说明: str) -> Path:
    状态根目录.mkdir(parents=True, exist_ok=True)
    租约目录 = 状态根目录 / "维护租约"
    try:
        租约目录.mkdir(exist_ok=False)
    except FileExistsError as 异常:
        raise 维护错误(f"维护租约已被占用: {租约目录}") from 异常
    写入JSON一次(
        租约目录 / "owner.json",
        {
            "进程": os.getpid(),
            "取得时间": 日期时间.datetime.now().astimezone().isoformat(),
            "输入": 输入说明,
        },
    )
    return 租约目录


def 执行维护(
    根目录: Path,
    配置路径: Path,
    模式: str,
    发布前回调: Callable[[], None] | None = None,
) -> tuple[str, Path | None]:
    配置 = 读取JSON(配置路径)
    if 模式 not in 配置.get("模式", {}):
        raise 维护错误(f"未知维护模式: {模式}")
    状态根目录 = 根目录 / Path(配置["本地状态目录"])
    租约目录 = 取得租约(状态根目录, f"{模式}:{配置路径}")
    try:
        断言图谱输入已提交(根目录, 配置)
        身份表, 边表, _外部边界表, 队列 = 读取图谱(根目录, 配置)
        基线提交 = 身份表.get("code_commit")
        if not 基线提交:
            raise 维护错误("函数身份表没有 code_commit")
        运行Git(["cat-file", "-e", f"{基线提交}^{{commit}}"], 根目录)
        开始身份 = 形成输入身份(根目录, 配置路径, 配置, 基线提交)
        分析 = 生成分析(根目录, 配置, 模式, 身份表, 边表, 队列, 开始身份)
        指纹材料 = {"模式": 模式, "输入身份": 可比较输入身份(开始身份), "分析": 分析}
        候选指纹 = 字节哈希(规范JSON(指纹材料))
        已有 = 查找相同候选(状态根目录 / "候选批次", 候选指纹)
        if 已有:
            return "重复候选", 已有
        if not 分析["观察差异"] and not 分析["图谱问题"]:
            return "无变化", None

        if 发布前回调:
            发布前回调()
        结束身份 = 形成输入身份(根目录, 配置路径, 配置, 基线提交)
        if 可比较输入身份(结束身份) != 可比较输入身份(开始身份):
            失败材料 = {"模式": 模式, "开始身份": 开始身份, "结束身份": 结束身份, "分析": 分析}
            失败指纹 = 字节哈希(规范JSON(失败材料))
            清单 = {
                "schema_version": 1,
                "状态": "INPUT-DRIFT",
                "模式": 模式,
                "候选指纹": 失败指纹,
                "输入身份": 开始身份,
                "发布前输入身份": 结束身份,
                "正式图谱已修改": False,
            }
            路径 = 发布批次(状态根目录, "失败批次", 清单, 分析)
            return "输入漂移", 路径

        清单 = {
            "schema_version": 1,
            "状态": "CANDIDATE-READY" if not 分析["图谱问题"] else "GRAPH-VALIDATION-FAILED",
            "模式": 模式,
            "候选指纹": 候选指纹,
            "生成时间": 日期时间.datetime.now().astimezone().isoformat(),
            "输入身份": 开始身份,
            "正式图谱已修改": False,
            "声明边界": "候选批次不是正式流程图、正式调用图、代码事实发布或能力完成证明",
        }
        类别 = "候选批次" if not 分析["图谱问题"] else "失败批次"
        路径 = 发布批次(状态根目录, 类别, 清单, 分析)
        return 清单["状态"], 路径
    finally:
        shutil.rmtree(租约目录, ignore_errors=True)


def 建立自检仓库(根目录: Path) -> Path:
    (根目录 / "src").mkdir(parents=True)
    (根目录 / "graph").mkdir()
    (根目录 / "src/a.cpp").write_text("int a() { return b(); }\n", encoding="utf-8")
    (根目录 / "src/b.cpp").write_text("int b() { return 1; }\n", encoding="utf-8")
    (根目录 / "src/c.cpp").write_text("int c() { return 2; }\n", encoding="utf-8")
    (根目录 / "project.vcxproj").write_text("<Project/>\n", encoding="utf-8")
    身份 = {
        "code_commit": "待回填",
        "functions": [
            {"function_id": "F1", "complete_signature": "int a()", "file": "src/a.cpp", "definition_line": 1, "definition_end_line": 1},
            {"function_id": "F2", "complete_signature": "int b()", "file": "src/b.cpp", "definition_line": 1, "definition_end_line": 1},
            {"function_id": "F3", "complete_signature": "int c()", "file": "src/c.cpp", "definition_line": 1, "definition_end_line": 1},
        ],
    }
    边 = {"edges": [{"edge_id": "E1", "caller": "F1", "callee": "F2"}]}
    外部 = {"external_boundaries": []}
    队列 = {
        "entries": [
            {"function_id": "F1", "chart_status": "已制图", "chart_path": "f1.md", "mapping_path": "f1-map.md"},
            {"function_id": "F2", "chart_status": "已制图", "chart_path": "f2.md", "mapping_path": "f2-map.md"},
            {"function_id": "F3", "chart_status": "待绘制", "chart_path": "", "mapping_path": ""},
        ]
    }
    for 名称, 内容 in (("identity.json", 身份), ("edges.json", 边), ("external.json", 外部), ("queue.json", 队列)):
        (根目录 / "graph" / 名称).write_text(json.dumps(内容, ensure_ascii=False), encoding="utf-8")
    subprocess.run(["git", "init", "-q", str(根目录)], check=True)
    subprocess.run(["git", "-C", str(根目录), "config", "user.email", "self-test@example.invalid"], check=True)
    subprocess.run(["git", "-C", str(根目录), "config", "user.name", "self-test"], check=True)
    subprocess.run(["git", "-C", str(根目录), "add", "."], check=True)
    subprocess.run(["git", "-C", str(根目录), "commit", "-qm", "baseline"], check=True)
    提交 = 当前提交(根目录)
    身份["code_commit"] = 提交
    (根目录 / "graph/identity.json").write_text(json.dumps(身份, ensure_ascii=False), encoding="utf-8")
    subprocess.run(["git", "-C", str(根目录), "add", "graph/identity.json"], check=True)
    subprocess.run(["git", "-C", str(根目录), "commit", "-qm", "freeze graph"], check=True)
    配置 = {
        "schema_version": 1,
        "源码根目录": ["src"],
        "源码扩展名": [".cpp"],
        "工程配置文件": ["project.vcxproj"],
        "图谱输入": {
            "函数身份表": "graph/identity.json",
            "项目调用边表": "graph/edges.json",
            "外部边界表": "graph/external.json",
            "制图队列": "graph/queue.json",
        },
        "本地状态目录": ".state",
        "模式": {
            "event": {"调用者闭包深度": 1, "检查全部图谱端点": False},
            "daily": {"调用者闭包深度": 4, "检查全部图谱端点": True},
            "full": {"调用者闭包深度": -1, "检查全部图谱端点": True},
        },
    }
    配置路径 = 根目录 / "config.json"
    配置路径.write_text(json.dumps(配置, ensure_ascii=False), encoding="utf-8")
    return 配置路径


def 自检() -> int:
    with tempfile.TemporaryDirectory(prefix="海中鱼巣-流程图自动维护-") as 临时文本:
        根目录 = Path(临时文本)
        配置路径 = 建立自检仓库(根目录)
        (根目录 / "src/b.cpp").write_text("int b() { return 3; }\n", encoding="utf-8")
        状态, 批次 = 执行维护(根目录, 配置路径, "event")
        assert 状态 == "CANDIDATE-READY" and 批次
        直接 = 读取JSON(批次 / "02_直接影响函数.json")
        闭包 = 读取JSON(批次 / "03_调用者影响闭包.json")
        assert {项["function_id"] for 项 in 直接} == {"F2"}
        assert {项["函数身份"] for 项 in 闭包} == {"F1", "F2"}

        重复状态, 重复批次 = 执行维护(根目录, 配置路径, "event")
        assert 重复状态 == "重复候选" and 重复批次 == 批次

        (根目录 / "src/new.cpp").write_text("int new_function() { return 4; }\n", encoding="utf-8")
        (根目录 / "src/c.cpp").unlink()
        状态2, 批次2 = 执行维护(根目录, 配置路径, "daily")
        assert 状态2 == "CANDIDATE-READY" and 批次2
        新增 = 读取JSON(批次2 / "05_新增待绘制候选.json")
        删除 = 读取JSON(批次2 / "06_删除函数候选.json")
        assert any(项.get("路径") == "src/new.cpp" for 项 in 新增)
        assert any(项.get("函数身份") == "F3" for 项 in 删除)

        租约 = 根目录 / ".state/维护租约"
        租约.mkdir()
        try:
            try:
                执行维护(根目录, 配置路径, "event")
                raise AssertionError("并发租约未阻止第二实例")
            except 维护错误 as 异常:
                assert "维护租约已被占用" in str(异常)
        finally:
            租约.rmdir()

        正式队列哈希 = 文件哈希(根目录 / "graph/queue.json")
        def 制造漂移() -> None:
            (根目录 / "src/a.cpp").write_text("int a() { return b() + 1; }\n", encoding="utf-8")

        漂移状态, 漂移批次 = 执行维护(根目录, 配置路径, "full", 制造漂移)
        assert 漂移状态 == "输入漂移" and 漂移批次
        assert 文件哈希(根目录 / "graph/queue.json") == 正式队列哈希
        assert not (根目录 / ".state/维护租约").exists()
    print("代码流程图自动维护工具自检: PASS")
    return 0


def 构建参数() -> argparse.Namespace:
    解析器 = argparse.ArgumentParser(description="生成不可覆盖的代码流程图增量维护候选")
    解析器.add_argument("--配置", "--config", dest="配置", default=str(默认配置路径))
    解析器.add_argument("--模式", "--mode", dest="模式", choices=("event", "daily", "full"), default="event")
    解析器.add_argument("--self-test", action="store_true", dest="自检")
    return 解析器.parse_args()


def 主函数() -> int:
    参数 = 构建参数()
    if 参数.自检:
        return 自检()
    配置路径 = Path(参数.配置).resolve()
    try:
        状态, 路径 = 执行维护(仓库根目录, 配置路径, 参数.模式)
    except 维护错误 as 异常:
        print(f"代码流程图自动维护: BLOCKED: {异常}", file=sys.stderr)
        return 3
    print(f"代码流程图自动维护: {状态}")
    if 路径:
        print(f"候选批次: {路径}")
    return 2 if 状态 in {"输入漂移", "GRAPH-VALIDATION-FAILED"} else 0


if __name__ == "__main__":
    sys.exit(主函数())
