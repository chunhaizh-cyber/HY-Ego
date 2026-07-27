#!/usr/bin/env python3
# 文件规则：本工具只把 Clang AST JSON 转成候选证据，不修改正式流程图、规范、计划或代码。
from __future__ import annotations

import argparse
import bisect
import json
from pathlib import Path
from typing import Any, Iterable


函数种类 = {
    "FunctionDecl",
    "CXXMethodDecl",
    "CXXConstructorDecl",
    "CXXDestructorDecl",
    "CXXConversionDecl",
}

作用域种类 = {
    "NamespaceDecl",
    "CXXRecordDecl",
    "RecordDecl",
    "ClassTemplateDecl",
}

节点类型映射 = {
    "CallExpr": "函数调用候选",
    "CXXMemberCallExpr": "函数调用候选",
    "CXXOperatorCallExpr": "函数调用候选",
    "CUDAKernelCallExpr": "函数调用候选",
    "IfStmt": "判断候选",
    "SwitchStmt": "判断候选",
    "CaseStmt": "判断候选",
    "DefaultStmt": "判断候选",
    "ConditionalOperator": "判断候选",
    "BinaryConditionalOperator": "判断候选",
    "ForStmt": "循环候选",
    "CXXForRangeStmt": "循环候选",
    "WhileStmt": "循环候选",
    "DoStmt": "循环候选",
    "ReturnStmt": "返回候选",
    "CXXThrowExpr": "抛出候选",
    "CXXTryStmt": "异常处理候选",
    "CXXCatchStmt": "异常处理候选",
    "BreakStmt": "循环或跳转控制候选",
    "ContinueStmt": "循环或跳转控制候选",
    "GotoStmt": "循环或跳转控制候选",
    "DeclStmt": "局部声明候选",
    "BinaryOperator": "表达式候选",
    "CompoundAssignOperator": "表达式候选",
    "UnaryOperator": "表达式候选",
    "LambdaExpr": "Lambda候选",
}


def 读取连续_json(路径: Path) -> list[dict[str, Any]]:
    文本 = 路径.read_text(encoding="utf-8")
    解码器 = json.JSONDecoder()
    结果: list[dict[str, Any]] = []
    位置 = 0
    while 位置 < len(文本):
        while 位置 < len(文本) and 文本[位置].isspace():
            位置 += 1
        if 位置 >= len(文本):
            break
        值, 位置 = 解码器.raw_decode(文本, 位置)
        if isinstance(值, dict):
            结果.append(值)
    return 结果


class 源码位置换算:
    def __init__(self, 路径: Path) -> None:
        self.路径 = str(路径.resolve())
        self.字节 = 路径.read_bytes()
        self.换行位置 = [-1]
        self.换行位置.extend(
            索引 for 索引, 值 in enumerate(self.字节) if 值 == 0x0A
        )

    def 位置(self, 材料: dict[str, Any] | None) -> dict[str, Any]:
        材料 = 材料 or {}
        偏移 = int(材料.get("offset", 0))
        行 = 材料.get("line")
        列 = 材料.get("col")
        if 行 is None:
            行索引 = bisect.bisect_left(self.换行位置, 偏移) - 1
            行 = max(1, 行索引 + 1)
            行首 = self.换行位置[max(0, 行索引)] + 1
            列 = max(1, 偏移 - 行首 + 1)
        return {
            "file": self.路径,
            "line": int(行),
            "column": int(列 or 0),
            "offset": 偏移,
        }

    def 范围(self, 节点: dict[str, Any]) -> dict[str, Any]:
        范围 = 节点.get("range") or {}
        开始 = 范围.get("begin") or 节点.get("loc") or {}
        结束 = 范围.get("end") or 开始
        return {
            "start": self.位置(开始),
            "end": self.位置(结束),
        }


def 子节点(节点: dict[str, Any]) -> list[dict[str, Any]]:
    return [项 for 项 in 节点.get("inner", []) if isinstance(项, dict)]


def 查找函数体(节点: dict[str, Any]) -> dict[str, Any] | None:
    for 子项 in 子节点(节点):
        if 子项.get("kind") == "CompoundStmt":
            return 子项
    return None


def 函数返回类型(类型: str, 种类: str) -> str:
    if 种类 in {"CXXConstructorDecl", "CXXDestructorDecl"}:
        return "void"
    标记 = 类型.find(" (")
    return 类型[:标记] if 标记 >= 0 else 类型


def 查找调用引用(节点: dict[str, Any]) -> dict[str, str] | None:
    引用 = 节点.get("referencedDecl")
    if isinstance(引用, dict):
        return {
            "id": str(引用.get("id", "")),
            "kind": str(引用.get("kind", "")),
            "name": str(引用.get("name", "")),
            "signature": str((引用.get("type") or {}).get("qualType", "")),
        }
    成员引用 = 节点.get("referencedMemberDecl")
    if 成员引用:
        return {
            "id": str(成员引用),
            "kind": "MemberDecl",
            "name": str(节点.get("name", "")),
            "signature": str((节点.get("type") or {}).get("qualType", "")),
        }
    for 子项 in 子节点(节点):
        结果 = 查找调用引用(子项)
        if 结果 and 结果.get("name"):
            return 结果
    return None


def 节点显示名称(节点: dict[str, Any]) -> str:
    for 字段 in ("name", "opcode", "value", "castKind"):
        值 = 节点.get(字段)
        if 值 not in (None, ""):
            return str(值)
    return ""


def 遍历流程节点(
    根: dict[str, Any],
    换算: 源码位置换算,
    函数身份: str,
    函数名称: str,
) -> tuple[list[dict[str, Any]], list[dict[str, Any]], list[dict[str, Any]]]:
    节点组: list[dict[str, Any]] = []
    调用组: list[dict[str, Any]] = []
    未解析组: list[dict[str, Any]] = []

    def 递归(节点: dict[str, Any], 父编号: str, 深度: int) -> None:
        种类 = str(节点.get("kind", ""))
        if 种类 in 函数种类:
            return
        当前父编号 = 父编号
        当前深度 = 深度
        类型 = 节点类型映射.get(种类)
        if 类型:
            编号 = f"N{len(节点组) + 1:04d}"
            节点组.append(
                {
                    "id": 编号,
                    "parent_id": 父编号,
                    "depth": 深度,
                    "cursor_kind": 种类,
                    "node_type": 类型,
                    "name": 节点显示名称(节点),
                    "extent": 换算.范围(节点),
                }
            )
            当前父编号 = 编号
            当前深度 = 深度 + 1
            if 类型 == "函数调用候选":
                引用 = 查找调用引用(节点)
                位置 = 换算.位置((节点.get("range") or {}).get("begin"))
                if 引用 and 引用.get("name"):
                    调用组.append(
                        {
                            "caller": 函数身份,
                            "caller_name": 函数名称,
                            "callee_usr": 引用.get("id", ""),
                            "callee_kind": 引用.get("kind", ""),
                            "callee_name": 引用.get("name", ""),
                            "callee_signature": 引用.get("signature", ""),
                            "resolution": "clang-ast-referenced-decl",
                            "location": 位置,
                        }
                    )
                else:
                    未解析组.append(
                        {
                            "caller": 函数身份,
                            "caller_name": 函数名称,
                            "referenced_kind": 种类,
                            "reason": "Clang AST 未提供可唯一确认的引用声明",
                            "location": 位置,
                        }
                    )
        for 子项 in 子节点(节点):
            递归(子项, 当前父编号, 当前深度)

    for 子项 in 子节点(根):
        递归(子项, "", 0)
    return 节点组, 调用组, 未解析组


def 遍历函数(
    根组: Iterable[dict[str, Any]],
    换算: 源码位置换算,
) -> tuple[list[dict[str, Any]], list[dict[str, Any]], list[dict[str, Any]]]:
    函数组: list[dict[str, Any]] = []
    调用组: list[dict[str, Any]] = []
    未解析组: list[dict[str, Any]] = []
    已有身份: set[str] = set()

    def 递归(节点: dict[str, Any], 作用域: tuple[str, ...]) -> None:
        种类 = str(节点.get("kind", ""))
        名称 = str(节点.get("name", ""))
        新作用域 = 作用域
        if 种类 in 作用域种类 and 名称:
            新作用域 = (*作用域, 名称)

        if 种类 in 函数种类 and not 节点.get("isImplicit", False):
            函数体 = 查找函数体(节点)
            if 函数体 is not None:
                位置 = 换算.位置(节点.get("loc"))
                限定名称 = "::".join((*作用域, 名称)) if 名称 else "::".join(作用域)
                身份 = str(节点.get("mangledName") or "")
                if 身份:
                    身份 = "候选函数:" + 身份
                else:
                    身份 = (
                        f"候选函数:{换算.路径}:{位置['line']}:{限定名称}"
                    )
                if 身份 not in 已有身份:
                    已有身份.add(身份)
                    类型 = str((节点.get("type") or {}).get("qualType", ""))
                    参数组 = []
                    for 子项 in 子节点(节点):
                        if 子项.get("kind") == "ParmVarDecl":
                            参数组.append(
                                {
                                    "name": str(子项.get("name", "")),
                                    "type": str(
                                        (子项.get("type") or {}).get("qualType", "")
                                    ),
                                }
                            )
                    流程节点, 当前调用, 当前未解析 = 遍历流程节点(
                        函数体, 换算, 身份, 限定名称
                    )
                    函数组.append(
                        {
                            "candidate_id": 身份,
                            "usr": str(节点.get("mangledName", "")),
                            "kind": 种类,
                            "name": 限定名称,
                            "signature": 类型,
                            "result_type": 函数返回类型(类型, 种类),
                            "location": 位置,
                            "extent": 换算.范围(节点),
                            "parameters": 参数组,
                            "flow_nodes": 流程节点,
                        }
                    )
                    调用组.extend(当前调用)
                    未解析组.extend(当前未解析)

        for 子项 in 子节点(节点):
            递归(子项, 新作用域)

    for 根 in 根组:
        递归(根, ())
    函数组.sort(key=lambda 项: (项["location"]["line"], 项["name"]))
    调用组.sort(
        key=lambda 项: (
            项["location"]["line"],
            项["location"]["column"],
            项["callee_name"],
        )
    )
    未解析组.sort(
        key=lambda 项: (
            项["location"]["line"],
            项["location"]["column"],
        )
    )
    return 函数组, 调用组, 未解析组


def main() -> int:
    参数解析器 = argparse.ArgumentParser()
    参数解析器.add_argument("--ast-json", required=True)
    参数解析器.add_argument("--source", required=True)
    参数解析器.add_argument("--project-root", required=True)
    参数解析器.add_argument("--observed-commit", default="")
    参数解析器.add_argument("--output", required=True)
    参数 = 参数解析器.parse_args()

    ast路径 = Path(参数.ast_json).resolve()
    源路径 = Path(参数.source).resolve()
    输出路径 = Path(参数.output).resolve()
    根组 = 读取连续_json(ast路径)
    换算 = 源码位置换算(源路径)
    函数组, 调用组, 未解析组 = 遍历函数(根组, 换算)
    结果 = {
        "schema": "CLANG-AST-FLOW-CANDIDATE/v0.3",
        "tool": "clang AST JSON",
        "observed_commit": 参数.observed_commit,
        "compiler": "clang",
        "working_directory": str(Path.cwd()),
        "project_root": str(Path(参数.project_root).resolve()),
        "main_file_only": True,
        "compile_arguments": [],
        "files": [
            {
                "source": str(源路径),
                "status": "成功",
                "parse_error_code": 0,
                "diagnostics": [],
                "functions": 函数组,
                "calls": 调用组,
                "unresolved_calls": 未解析组,
            }
        ],
    }
    输出路径.parent.mkdir(parents=True, exist_ok=True)
    输出路径.write_text(
        json.dumps(结果, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
