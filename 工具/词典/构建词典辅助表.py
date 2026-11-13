#!/usr/bin/env python3
"""从本地中文维基词典 JSONL（可 gzip 压缩）生成读词典释义所需的两张辅助表（7250 §3.4第3、6项）。

词头表.tsv：全部中文词头（含无释义页，不含重定向），作读释义时的切分边界依据；
繁简字表.tsv：由词典自身繁简字形对生成的字级繁简对照表，不引入外部对照表；
词性表.tsv：每条中文记录的原始词性（按记录行号，与词典.tsv记录键的行号一致），供读释义按词性选用结构。

繁简：只取标为简体（且非 alternative）的字形，与同一词条的繁体词面逐字对齐，长度不同的跳过；
占该字全部对齐不足5%的目标视为对齐噪声；其余目标中含原字本身（曾保持不变）或多于一个时，
视为一繁对多简，保留原字不转、不输出。
不联网；两张表各自完整写入并关闭后才原子替换；错误向命令行送出。
"""

import argparse
from collections import Counter, defaultdict
import gzip
import hashlib
import json
import os
from pathlib import Path
import sys
import tempfile


def 简体字形(记录: dict):
    """产出 (繁体词面, 简体词面)；标为 Traditional-Chinese 的是繁体异体，不是繁简对。"""
    词面 = 记录.get("word")
    if not isinstance(词面, str) or not 词面:
        return
    for 形 in 记录.get("forms", []) or []:
        if not isinstance(形, dict):
            continue
        文本, 标签 = 形.get("form"), 形.get("tags") or []
        if (isinstance(文本, str) and 文本 and 文本 != 词面
                and "Simplified-Chinese" in 标签 and "alternative" not in 标签):
            yield 词面, 文本


def 计算摘要(路径: Path) -> str:
    摘要 = hashlib.sha256()
    with 路径.open("rb") as 文件:
        for 块 in iter(lambda: 文件.read(1024 * 1024), b""):
            摘要.update(块)
    return 摘要.hexdigest()


def 原子写出(输出目录: Path, 文件名: str, 包头: str, 行组) -> Path:
    输出文件 = 输出目录 / 文件名
    临时路径 = None
    try:
        with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", newline="\n", dir=输出目录,
                                         prefix=".辅助表-", suffix=".tmp", delete=False) as 输出:
            临时路径 = Path(输出.name)
            输出.write(包头 + "\n")
            for 行 in 行组:
                输出.write(行 + "\n")
            输出.flush()
            os.fsync(输出.fileno())
        os.replace(临时路径, 输出文件)
        临时路径 = None
    finally:
        if 临时路径 is not None:
            临时路径.unlink(missing_ok=True)
    return 输出文件


def 构建(输入: Path, 输出目录: Path) -> dict:
    if not 输入.is_file():
        raise ValueError(f"输入不是普通文件：{输入}")
    对应 = defaultdict(Counter)
    词头 = set()
    对数 = 跳过 = 0
    词性组 = []
    打开输入 = gzip.open if 输入.suffix.lower() == ".gz" else open
    with 打开输入(输入, "rt", encoding="utf-8", errors="strict", newline="") as 原始:
        for 行号, 原行 in enumerate(原始, 1):
            try:
                记录 = json.loads(原行)
            except ValueError as 错误:
                raise ValueError(f"{输入.name} 解压行 {行号}：{错误}") from 错误
            if not isinstance(记录, dict) or 记录.get("lang_code") != "zh":
                continue
            词性 = 记录.get("pos")
            if isinstance(词性, str) and 词性 and 词性.isascii() and 词性.replace("-", "").isalnum():
                词性组.append(f"{行号}\t{词性}")
            词面 = 记录.get("word")
            if ("redirect" not in 记录 and "redirects" not in 记录 and isinstance(词面, str)
                    and 词面 and not any(字 in 词面 for 字 in "\t\r\n")):
                词头.add(词面)
            for 繁, 简 in 简体字形(记录):
                if len(繁) != len(简):
                    跳过 += 1
                    continue
                对数 += 1
                for 甲, 乙 in zip(繁, 简):
                    对应[甲][乙] += 1
    表 = []
    for 甲, 计数 in 对应.items():
        总 = sum(计数.values())
        乙组 = {乙 for 乙, 次 in 计数.items() if 次 * 20 >= 总}
        if len(乙组) == 1 and 甲 not in 乙组:
            表.append((甲, next(iter(乙组))))
    表.sort()
    输出目录.mkdir(parents=True, exist_ok=True)
    繁简文件 = 原子写出(输出目录, "繁简字表.tsv", "HY-EGO-TRAD-SIMP\t1", (f"{甲}\t{乙}" for 甲, 乙 in 表))
    词性文件 = 原子写出(输出目录, "词性表.tsv", "HY-EGO-POS\t1\t" + 计算摘要(输入), 词性组)
    词头文件 = 原子写出(输出目录, "词头表.tsv", "HY-EGO-HEADWORDS\t1\tzh", sorted(词头))
    return {"字形对数": 对数, "长度不同跳过": 跳过, "输出字数": len(表),
            "一繁多简或保持不变未转": len(对应) - len(表), "词头数": len(词头), "词性记录数": len(词性组), "词性表": str(词性文件.resolve()),
            "繁简字表": str(繁简文件.resolve()), "词头表": str(词头文件.resolve())}


def 主函数() -> int:
    参数器 = argparse.ArgumentParser(description=__doc__)
    参数器.add_argument("--input", required=True, type=Path, help="本地中文 Wiktionary raw JSONL 或 .gz")
    参数器.add_argument("--output-dir", type=Path, default=Path(__file__).resolve().parents[2] / "基础数据" / "词典")
    参数 = 参数器.parse_args()
    try:
        统计 = 构建(参数.input, 参数.output_dir)
    except (OSError, ValueError, TypeError, UnicodeError, MemoryError) as 错误:
        print(f"词典辅助表构建失败，未发布：{错误}", file=sys.stderr)
        return 1
    print(json.dumps(统计, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
