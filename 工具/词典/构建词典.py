#!/usr/bin/env python3
"""将本地中文维基词典 JSONL（可 gzip 压缩）构建为只读词典材料包。

这是生产导入入口，不联网，不建立正式词条或概念，不进行翻译。
完整候选在目标目录生成并关闭后，才原子替换词典.tsv；错误向命令行送出。
"""

import argparse
from collections import Counter
from datetime import datetime, timezone
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import sys
import tempfile
from urllib.parse import quote


汉字模式 = re.compile("[\u3400-\u4dbf\u4e00-\u9fff\uf900-\ufaff\U00020000-\U000323af]")
许可标识 = "CC-BY-SA-4.0; GFDL-1.2-or-later; see source notices"
署名 = "中文维基词典贡献者（见来源页历史）；Kaikki.org / Tatu Ylonen / Wiktextract"


def 检查文本(文本: str, 身份: bool = False) -> None:
    """保证输出字段符合冻结格式；不暗改原文或删除非法字符。"""
    文本.encode("utf-8", errors="strict")
    for 字符 in 文本:
        码点 = ord(字符)
        if 码点 in (0, 0xFEFF, 0xFFFD, 0x7F):
            raise ValueError(f"字段含禁止字符 U+{码点:04X}")
        if 码点 < 0x20 and (身份 or 字符 not in "\t\n\r"):
            raise ValueError(f"字段含禁止控制字符 U+{码点:04X}")


def 转义(文本: str) -> str:
    return 文本.replace("\\", "\\\\").replace("\t", "\\t").replace("\n", "\\n").replace("\r", "\\r")


def 计算摘要(路径: Path) -> str:
    摘要 = hashlib.sha256()
    with 路径.open("rb") as 文件:
        for 字节块 in iter(lambda: 文件.read(1024 * 1024), b""):
            摘要.update(字节块)
    return 摘要.hexdigest()


def 提取读音(记录: dict) -> str:
    读音组 = []
    for 项 in 记录.get("sounds", []):
        if not isinstance(项, dict):
            raise ValueError("sounds 成员不是对象")
        保留项 = {键: 项[键] for 键 in ("ipa", "zh_pron", "enpr", "roman") if 项.get(键)}
        if 保留项:
            if 项.get("tags"):
                保留项["tags"] = 项["tags"]
            读音组.append(保留项)
    return json.dumps(读音组, ensure_ascii=False, separators=(",", ":")) if 读音组 else ""


def 构建词典(输入: Path, 输出目录: Path, 包标识: str, 来源版本: str) -> dict:
    if not 输入.is_file():
        raise ValueError(f"输入不是普通文件：{输入}")
    if not 包标识 or not 来源版本:
        raise ValueError("包标识和来源版本不得为空")
    检查文本(包标识, True)
    检查文本(来源版本)
    原始摘要 = 计算摘要(输入)
    输出目录.mkdir(parents=True, exist_ok=True)
    输出文件 = 输出目录 / "词典.tsv"
    if 输入.resolve() == 输出文件.resolve():
        raise ValueError("输入不能是输出词典文件")
    输入行数 = 0
    跳过 = Counter({原因: 0 for 原因 in ("其它语种记录", "空词面记录", "重定向记录", "无义项记录",
                                        "无释义义项", "释义不含汉字义项")})
    词面组 = {"zh": set(), "en": set()}
    义项数 = Counter()
    创建时间 = datetime.now(timezone.utc).isoformat()
    临时路径 = None
    try:
        with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", newline="\n", dir=输出目录,
                                         prefix=".词典-", suffix=".tmp", delete=False) as 输出:
            临时路径 = Path(输出.name)
            输出.write("HY-EGO-DICTIONARY\t1\tzh\t" + 转义(包标识) + "\n")
            打开输入 = gzip.open if 输入.suffix.lower() == ".gz" else open
            with 打开输入(输入, "rt", encoding="utf-8", errors="strict", newline="") as 原始:
                for 行号, 原行 in enumerate(原始, 1):
                    输入行数 = 行号
                    try:
                        记录 = json.loads(原行)
                        if not isinstance(记录, dict):
                            raise ValueError("JSONL 记录不是对象")
                        if "redirect" in 记录 or "redirects" in 记录:
                            跳过["重定向记录"] += 1
                            continue
                        语种 = 记录.get("lang_code")
                        if 语种 not in 词面组:
                            跳过["其它语种记录"] += 1
                            continue
                        词面 = 记录.get("word")
                        if not isinstance(词面, str) or not 词面:
                            跳过["空词面记录"] += 1
                            continue
                        义项组 = 记录.get("senses", [])
                        if not isinstance(义项组, list):
                            raise ValueError("senses 不是列表")
                        if not 义项组:
                            跳过["无义项记录"] += 1
                            continue
                        检查文本(词面, True)
                        读音 = 提取读音(记录)
                        来源页 = "https://zh.wiktionary.org/wiki/" + quote(词面, safe="")
                        for 序号, 义项 in enumerate(义项组, 1):
                            if not isinstance(义项, dict):
                                raise ValueError("sense 不是对象")
                            释义组 = 义项.get("glosses", [])
                            if not isinstance(释义组, list) or any(not isinstance(项, str) for 项 in 释义组):
                                raise ValueError("glosses 不是字符串列表")
                            if not 释义组 or not any(项.strip() for 项 in 释义组):
                                跳过["无释义义项"] += 1
                                continue
                            释义 = "\n".join(释义组)
                            if not 汉字模式.search(释义):
                                跳过["释义不含汉字义项"] += 1
                                continue
                            字段组 = [f"{原始摘要}:{行号}", 语种, 词面, str(序号), 释义,
                                      "", "", 读音, 来源页, 来源版本, 许可标识, 署名,
                                      f"{输入.name}:line={行号}:sense={序号}"]
                            for 字段序号, 字段 in enumerate(字段组):
                                检查文本(字段, 字段序号 in (0, 1, 2, 3))
                            输出.write("\t".join(转义(字段) for 字段 in 字段组) + "\n")
                            词面组[语种].add(词面)
                            义项数[语种] += 1
                        if 行号 % 100000 == 0:
                            print(f"已读取 {行号} 行；收录 {sum(义项数.values())} 义项", file=sys.stderr, flush=True)
                    except (ValueError, TypeError, UnicodeError) as 错误:
                        raise ValueError(f"{输入.name} 解压行 {行号}：{错误}") from 错误
            if not all(义项数[语种] for 语种 in 词面组):
                raise ValueError("本包须同时包含中文与英文的汉语释义，未发布不完整包")
            输出.flush()
            os.fsync(输出.fileno())
        输出字节数 = 临时路径.stat().st_size
        输出摘要 = 计算摘要(临时路径)
        统计 = {
            "包格式": "HY-EGO-DICTIONARY/1", "包标识": 包标识, "来源版本": 来源版本,
            "构建开始UTC": 创建时间, "构建完成UTC": datetime.now(timezone.utc).isoformat(),
            "输入文件": str(输入.resolve()), "输入字节数": 输入.stat().st_size, "输入SHA256": 原始摘要,
            "输入JSONL行数": 输入行数,
            "语种统计": {语种: {"词面数": len(词面组[语种]), "义项数": 义项数[语种]} for 语种 in ("zh", "en")},
            "总词面数": sum(map(len, 词面组.values())), "总义项数": sum(义项数.values()),
            "跳过": dict(跳过), "输出文件": str(输出文件.resolve()),
            "输出字节数": 输出字节数, "输出SHA256": 输出摘要,
        }
        os.replace(临时路径, 输出文件)
        临时路径 = None
        return 统计
    finally:
        if 临时路径 is not None:
            临时路径.unlink(missing_ok=True)


def 主函数() -> int:
    参数器 = argparse.ArgumentParser(description=__doc__)
    参数器.add_argument("--input", required=True, type=Path, help="本地中文 Wiktionary raw JSONL 或 .gz")
    参数器.add_argument("--output-dir", type=Path, default=Path(__file__).resolve().parents[2] / "基础数据" / "词典")
    参数器.add_argument("--package-id", required=True)
    参数器.add_argument("--source-version", required=True)
    参数 = 参数器.parse_args()
    try:
        统计 = 构建词典(参数.input, 参数.output_dir, 参数.package_id, 参数.source_version)
    except (OSError, ValueError, TypeError, UnicodeError, MemoryError) as 错误:
        print(f"词典构建失败，未发布候选包：{错误}", file=sys.stderr)
        return 1
    print(json.dumps(统计, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(主函数())
