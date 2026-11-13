#!/usr/bin/env python3
"""生成简体词典与读词典释义所需的辅助表（7250 §3.4；2026-11-13 用户裁决：现有词典整体转成简体）。

输入：
  --input     本地中文维基词典 raw JSONL（可 gzip 压缩），取词头与词性；
  --dictionary 已构建的 词典.tsv（HY-EGO-DICTIONARY/1，13列）；
  --opencc    OpenCC 词典目录（固定 ver.1.4.2：TSPhrases.txt、TSCharacters.txt、LICENSE）；
  --source-manifest 受控来源清单，固定输入摘要、长度、来源版本及包标识。
输出（八文件在独有暂存目录全部完成后发布不可变批次，最后原子切换当前批次.txt）：
  词典.简体.tsv：词典.tsv 按 OpenCC 繁转简规则转换后的同格式包；记录键、源定位不变，可追溯原始行；
  繁简表.tsv：OpenCC 繁转简词组表与字表，供运行时规范化残留繁体（P 行词组、C 行单字）；
  词头表.tsv：全部中文词头（含无释义页，不含重定向）转简体后去重，作读释义切分边界依据；
  词性表.tsv：每条中文记录的原始词性（按记录行号），包头带原始包 SHA256；
  来源清单.json、LICENSE.md、ATTRIBUTION.md、OpenCC-LICENSE.txt：同批来源和许可原文。

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
import stat
import sys
import tempfile


def 计算摘要(路径: Path) -> str:
    摘要 = hashlib.sha256()
    with 路径.open("rb") as 文件:
        for 块 in iter(lambda: 文件.read(1024 * 1024), b""):
            摘要.update(块)
    return 摘要.hexdigest()


def 核对文件(路径: Path, 字节数: int, 摘要: str) -> None:
    if (type(字节数) is not int or 字节数 <= 0 or not isinstance(摘要, str)
            or len(摘要) != 64 or any(字 not in "0123456789abcdef" for 字 in 摘要)):
        raise ValueError(f"来源清单中的长度或 SHA256 不合法：{路径.name}")
    属性 = 路径.stat()
    if not stat.S_ISREG(属性.st_mode) or 属性.st_size != 字节数 or 计算摘要(路径) != 摘要:
        raise ValueError(f"文件不是指定来源的完整普通文件：{路径}")


def 核对构建输入(输入: Path, 词典: Path, opencc: Path, 来源清单: Path) -> tuple:
    """生成前核对受控来源；说明原字节随候选保留，错误只向主函数送出。"""
    清单字节 = 来源清单.read_bytes()
    清单 = json.loads(清单字节.decode("utf-8"))
    try:
        if (清单["元数据格式"] != "HY-EGO-DICTIONARY-SOURCE/1"
                or 清单["包格式"] != "HY-EGO-DICTIONARY/1"
                or 清单["繁转简"]["版本"] != "ver.1.4.2"):
            raise ValueError("来源清单格式或 OpenCC 版本不符")
        for 名, 值 in (("包标识", 清单["包标识"]), ("来源版本", 清单["来源"]["来源版本"])):
            if (not isinstance(值, str) or not 值
                    or any(ord(字) < 0x20 or 字 in "\x7f\ufeff\ufffd\\" for 字 in 值)):
                raise ValueError(f"来源清单的{名}不合法")
        核对文件(输入, 清单["来源"]["原始字节数"], 清单["来源"]["原始SHA256"])
        核对文件(词典, 清单["构建"]["输出字节数"], 清单["构建"]["输出SHA256"])
        for 名 in ("TSPhrases.txt", "TSCharacters.txt", "LICENSE"):
            键 = 名 if 名 == "LICENSE" else "data/dictionary/" + 名
            项 = 清单["繁转简"]["文件"][键]
            核对文件(opencc / 名, 项["字节"], 项["SHA256"])
    except (KeyError, TypeError) as 错误:
        raise ValueError(f"来源清单缺少合法的构建字段：{错误}") from 错误
    说明 = {"来源清单.json": 清单字节,
            "LICENSE.md": (来源清单.parent / "LICENSE.md").read_bytes(),
            "ATTRIBUTION.md": (来源清单.parent / "ATTRIBUTION.md").read_bytes(),
            "OpenCC-LICENSE.txt": (opencc / "LICENSE").read_bytes()}
    for 名, 内容 in 说明.items():
        if not 内容:
            raise ValueError(f"来源说明为空：{名}")
    if hashlib.sha256(说明["OpenCC-LICENSE.txt"]).hexdigest() != 清单["繁转简"]["文件"]["LICENSE"]["SHA256"]:
        raise ValueError("OpenCC 许可在核对后发生变化")
    return 清单, 说明


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


def 发布批次(输出目录: Path, 暂存: Path, 简体包标识: str, 原始摘要: str, 行数表: dict) -> dict:
    """只安装完整新目录；相同内容只读核对后复用，最后切换 current。"""
    文件项 = []
    for 名 in sorted(行数表):
        文件 = 暂存 / 名
        文件项.append((名, 文件.stat().st_size, 计算摘要(文件), 行数表[名]))
    清单行 = [f"HY-EGO-DICTIONARY-BATCH\t1\t{简体包标识}\t{原始摘要}", "OpenCC\tver.1.4.2"]
    清单行.extend("\t".join(map(str, 项)) for 项 in 文件项)
    清单字节 = ("\n".join(清单行) + "\n").encode("utf-8")
    with (暂存 / "批次清单.tsv").open("xb") as 文件:
        文件.write(清单字节)
        文件.flush()
        os.fsync(文件.fileno())
    标识 = hashlib.sha256(清单字节).hexdigest()
    批次根 = 输出目录 / "批次"
    批次根.mkdir(exist_ok=True)
    目标 = 批次根 / 标识
    # 安装前形成返回材料，避免完整批次已落盘后才因构造报告失败而丢失位置。
    统计 = {"批次标识": 标识, "批次目录": str(目标.resolve()),
            "当前批次": str((输出目录 / "当前批次.txt").resolve()),
            "文件": {名: {"字节数": 大小, "SHA256": 摘要, "数据行数": 数}
                     for 名, 大小, 摘要, 数 in 文件项}}
    # 用 stat 区分不存在和属性故障；从不修补、覆盖已经安装的批次。
    try:
        属性 = 目标.stat()
    except FileNotFoundError:
        暂存.rename(目标)
    else:
        if not stat.S_ISDIR(属性.st_mode) or 目标.is_symlink():
            raise ValueError(f"既有批次不是普通目录：{目标}")
        if {项.name for 项 in 目标.iterdir()} != set(行数表) | {"批次清单.tsv"}:
            raise ValueError(f"既有批次文件集合不符，未覆盖：{目标}")
        if (目标 / "批次清单.tsv").read_bytes() != 清单字节:
            raise ValueError(f"既有批次清单不符，未覆盖：{目标}")
        for 名, 字节数, 摘要, _ in 文件项:
            核对文件(目标 / 名, 字节数, 摘要)
        # 只清除本次独有暂存目录中的固定叶文件，不递归触碰其它数据。
        for 名 in (*行数表, "批次清单.tsv"):
            (暂存 / 名).unlink()
        暂存.rmdir()
    try:
        原子写出(输出目录, "当前批次.txt", None, [标识])
    except (OSError, ValueError, MemoryError) as 错误:
        raise RuntimeError(f"当前批次未切换；完整批次保留于 {目标}：{错误}") from 错误
    return 统计


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


def 构建(输入: Path, 词典: Path, opencc: Path, 输出目录: Path, 来源清单: Path) -> dict:
    清单, 说明 = 核对构建输入(输入, 词典, opencc, 来源清单)
    原始摘要 = 清单["来源"]["原始SHA256"]
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
        if 包头 != ["HY-EGO-DICTIONARY", "1", "zh", 清单["包标识"]]:
            raise ValueError("词典.tsv 包头不符")
        简体包标识 = 包头[3] + "-s-opencc-1.4.2"
        简体包头 = "\t".join(包头[:3] + [简体包标识])
        for 行 in 文件:
            列 = 行.rstrip("\r\n").split("\t")
            if len(列) != 13:
                raise ValueError(f"词典.tsv 第{行数 + 2}行不是13列")
            if not 列[0].startswith(原始摘要 + ":") or 列[9] != 清单["来源"]["来源版本"]:
                raise ValueError(f"词典.tsv 第{行数 + 2}行来源与本次原始输入不符")
            if 列[1] == "zh":
                列[2] = 转.转换(列[2])
                列[5] = 转.转换(列[5])
            列[4] = 转.转换(列[4])
            列[6] = 转.转换(列[6])
            简体行组.append("\t".join(列))
            行数 += 1

    行数表 = {"词典.简体.tsv": 行数, "繁简表.tsv": len(转.词组) + len(转.单字),
              "词头表.tsv": len(词头), "词性表.tsv": len(词性组)}
    if any(数 <= 0 for 数 in 行数表.values()):
        raise ValueError("词典或辅助表为空，不能发布可信批次")
    行数表.update({名: 0 for 名 in 说明})
    输出目录.mkdir(parents=True, exist_ok=True)
    暂存 = Path(tempfile.mkdtemp(prefix=".词典批次-", dir=输出目录))
    try:
        原子写出(暂存, "词典.简体.tsv", 简体包头, 简体行组)
        原子写出(暂存, "繁简表.tsv", "HY-EGO-TRAD-SIMP\t2\topencc-1.4.2",
                 [f"P\t{甲}\t{乙}" for 甲, 乙 in sorted(转.词组.items())]
                 + [f"C\t{甲}\t{乙}" for 甲, 乙 in sorted(转.单字.items())])
        原子写出(暂存, "词头表.tsv", "HY-EGO-HEADWORDS\t2\tzh-s", sorted(词头))
        原子写出(暂存, "词性表.tsv", "HY-EGO-POS\t1\t" + 原始摘要, 词性组)
        for 名, 内容 in 说明.items():
            with (暂存 / 名).open("xb") as 文件:
                文件.write(内容)
                文件.flush()
                os.fsync(文件.fileno())
        return 发布批次(输出目录, 暂存, 简体包标识, 原始摘要, 行数表)
    except (OSError, ValueError, TypeError, MemoryError, RuntimeError) as 错误:
        # 保留本次候选作人工处理，不用清理失败掩盖原故障，更不删除既有批次。
        残留 = f"；本次暂存保留于 {暂存}" if 暂存.exists() else ""
        raise RuntimeError(f"构建或安装批次失败{残留}：{错误}") from 错误


def 主函数() -> int:
    根 = Path(__file__).resolve().parents[2] / "基础数据" / "词典"
    参数器 = argparse.ArgumentParser(description=__doc__)
    参数器.add_argument("--input", required=True, type=Path, help="本地中文 Wiktionary raw JSONL 或 .gz")
    参数器.add_argument("--dictionary", type=Path, default=根 / "词典.tsv")
    参数器.add_argument("--opencc", type=Path, default=根 / "原始" / "opencc-1.4.2")
    参数器.add_argument("--output-dir", type=Path, default=根)
    参数器.add_argument("--source-manifest", type=Path, default=根 / "来源清单.json")
    参数 = 参数器.parse_args()
    try:
        统计 = 构建(参数.input, 参数.dictionary, 参数.opencc, 参数.output_dir, 参数.source_manifest)
    except (OSError, ValueError, TypeError, MemoryError, RuntimeError) as 错误:
        print(f"词典批次构建失败，当前批次未切换：{错误}", file=sys.stderr)
        return 1
    try:
        print(json.dumps(统计, ensure_ascii=False, indent=2))
        sys.stdout.flush()
    except (OSError, ValueError, MemoryError) as 错误:
        print(f"词典批次已发布，但统计输出失败：{错误}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
