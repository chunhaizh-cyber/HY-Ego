#!/usr/bin/env python3
"""生成简体词典与读词典释义所需的辅助表（7250 §3.4；2026-11-13 用户裁决：现有词典整体转成简体）。

输入：
  --input     本地中文维基词典 raw JSONL（可 gzip 压缩），取词头与词性；
  --dictionary 已构建的 词典.tsv（HY-EGO-DICTIONARY/1，13列）；
  --opencc    OpenCC 词典目录（固定 ver.1.4.2：TSPhrases.txt、TSCharacters.txt、LICENSE）。
输出（同一目录，各自完整写入并关闭后原子替换）：
  词典.简体.tsv：词典.tsv 按 OpenCC 繁转简规则转换后的同格式包；记录键、源定位不变，可追溯原始行；
  繁简表.tsv：OpenCC 繁转简词组表与字表，供运行时规范化残留繁体（P 行词组、C 行单字）；
  词头表.tsv：全部中文词头（含无释义页，不含重定向）转简体后去重，作读释义切分边界依据；
  词性表.tsv：每条中文记录的原始词性（按记录行号），包头带原始包 SHA256。

繁转简与 OpenCC t2s 一致：自左向右，先取 TSPhrases 最长词组，否则按 TSCharacters 逐字（多值取第一个），
都不命中保留原字。TSPhrases 每条词组须与其简体等长，使原文与简体逐标量对齐；不等长时构建失败。
不联网；错误向命令行送出。
"""

import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import sys
import tempfile


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
            if 包头 is not None:
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


def 读OpenCC表(路径: Path) -> dict:
    表 = {}
    for 行号, 行 in enumerate(路径.read_text(encoding="utf-8").splitlines(), 1):
        if not 行 or 行.startswith("#"):
            continue
        键, 制表, 值组 = 行.partition("\t")
        值 = 值组.split(" ")[0] if 制表 else ""
        if not 键 or not 值:
            raise ValueError(f"{路径.name} 第{行号}行格式不合法")
        if 键 in 表:
            raise ValueError(f"{路径.name} 第{行号}行键重复：{键}")
        表[键] = 值
    return 表


class 繁转简:
    def __init__(self, 目录: Path):
        self.词组 = 读OpenCC表(目录 / "TSPhrases.txt")
        self.单字 = 读OpenCC表(目录 / "TSCharacters.txt")
        for 键, 值 in self.词组.items():
            if len(键) != len(值) or len(键) < 2:
                raise ValueError(f"TSPhrases 词组与简体不等长或不足两字：{键} -> {值}")
        for 键, 值 in self.单字.items():
            if len(键) != 1 or len(值) != 1:
                raise ValueError(f"TSCharacters 不是单字对单字：{键} -> {值}")
        self.最长 = max(map(len, self.词组))

    def 转换(self, 文本: str) -> str:
        输出, 位置 = [], 0
        while 位置 < len(文本):
            for 长 in range(min(self.最长, len(文本) - 位置), 1, -1):
                片 = 文本[位置:位置 + 长]
                if 片 in self.词组:
                    输出.append(self.词组[片])
                    位置 += 长
                    break
            else:
                字 = 文本[位置]
                输出.append(self.单字.get(字, 字))
                位置 += 1
        return "".join(输出)


def 构建(输入: Path, 词典: Path, opencc: Path, 输出目录: Path) -> dict:
    for 路径 in (输入, 词典):
        if not 路径.is_file():
            raise ValueError(f"输入不是普通文件：{路径}")
    for 名 in ("TSPhrases.txt", "TSCharacters.txt", "LICENSE"):
        if not (opencc / 名).is_file():
            raise ValueError(f"OpenCC 目录缺 {名}：{opencc}")
    转 = 繁转简(opencc)
    词头, 词性组 = set(), []
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
                词头.add(转.转换(词面))

    # 词典.tsv 逐行转换：zh 行转词面、释义、例句；en 行只转释义与例句汉译。转义符均为 ASCII，不受影响。
    简体行组, 行数 = [], 0
    with 词典.open(encoding="utf-8", newline="") as 文件:
        包头 = 文件.readline().rstrip("\r\n").split("\t")
        if len(包头) != 4 or 包头[:3] != ["HY-EGO-DICTIONARY", "1", "zh"]:
            raise ValueError("词典.tsv 包头不符")
        简体包头 = "\t".join(包头[:3] + [包头[3] + "-s-opencc-1.4.2"])
        for 行 in 文件:
            列 = 行.rstrip("\r\n").split("\t")
            if len(列) != 13:
                raise ValueError(f"词典.tsv 第{行数 + 2}行不是13列")
            if 列[1] == "zh":
                列[2] = 转.转换(列[2])
                列[5] = 转.转换(列[5])
            列[4] = 转.转换(列[4])
            列[6] = 转.转换(列[6])
            简体行组.append("\t".join(列))
            行数 += 1

    输出目录.mkdir(parents=True, exist_ok=True)
    简体文件 = 原子写出(输出目录, "词典.简体.tsv", 简体包头, 简体行组)
    繁简文件 = 原子写出(输出目录, "繁简表.tsv", "HY-EGO-TRAD-SIMP\t2\topencc-1.4.2",
                    [f"P\t{甲}\t{乙}" for 甲, 乙 in sorted(转.词组.items())]
                    + [f"C\t{甲}\t{乙}" for 甲, 乙 in sorted(转.单字.items())])
    词头文件 = 原子写出(输出目录, "词头表.tsv", "HY-EGO-HEADWORDS\t2\tzh-s", sorted(词头))
    词性文件 = 原子写出(输出目录, "词性表.tsv", "HY-EGO-POS\t1\t" + 计算摘要(输入), 词性组)
    旧表 = 输出目录 / "繁简字表.tsv"  # 第一片由词典字形对自建的字表，已由 OpenCC 繁简表取代
    旧表.unlink(missing_ok=True)
    return {"OpenCC": {"版本": "ver.1.4.2", "词组数": len(转.词组), "单字数": len(转.单字),
                       "TSPhrases.txt": 计算摘要(opencc / "TSPhrases.txt"),
                       "TSCharacters.txt": 计算摘要(opencc / "TSCharacters.txt"),
                       "LICENSE": 计算摘要(opencc / "LICENSE")},
            "简体词典": {"行数": 行数, "SHA256": 计算摘要(简体文件), "路径": str(简体文件.resolve())},
            "词头数": len(词头), "词性记录数": len(词性组),
            "繁简表": str(繁简文件.resolve()), "词头表": str(词头文件.resolve()), "词性表": str(词性文件.resolve())}


def 主函数() -> int:
    根 = Path(__file__).resolve().parents[2] / "基础数据" / "词典"
    参数器 = argparse.ArgumentParser(description=__doc__)
    参数器.add_argument("--input", required=True, type=Path, help="本地中文 Wiktionary raw JSONL 或 .gz")
    参数器.add_argument("--dictionary", type=Path, default=根 / "词典.tsv")
    参数器.add_argument("--opencc", type=Path, default=根 / "原始" / "opencc-1.4.2")
    参数器.add_argument("--output-dir", type=Path, default=根)
    参数 = 参数器.parse_args()
    try:
        统计 = 构建(参数.input, 参数.dictionary, 参数.opencc, 参数.output_dir)
    except (OSError, ValueError, TypeError, UnicodeError, MemoryError) as 错误:
        print(f"词典辅助表构建失败，未发布：{错误}", file=sys.stderr)
        return 1
    print(json.dumps(统计, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
