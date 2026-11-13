#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""词典释义句式比较统计（离线实验）。

做法：把词典里汉语词头的释义拆成单句，以词头表为边界做正向最长匹配切分，
词条数相同的句子逐位置比较——相同位置保留原词、不同位置记为符号□——得到句式；
再把固定词序列相同、只是□位置长度不同的句式合并。只输出统计，不下结论。

仅用 Python 标准库，不联网，不改写任何输入数据。
用法见 --help；默认从项目根目录运行。
"""
from __future__ import annotations

import argparse
import collections
import itertools
import os
import random
import sys
import time
import unicodedata

空 = "□"
多空 = "□+"

# 口径 b：词性表里视为功能词（封闭类）的词性标签。
功能词性 = {"particle", "prep", "conj", "postp", "det", "pron", "classifier", "num", "suffix", "prefix"}
# 补充口径 b2：词性表没有给出词性的常用单字／短功能词，人工列出，仅作对照，不来自词性表。
补充功能词 = set(
    "的 之 是 为 有 在 于 和 与 或 及 而 以 所 被 把 对 从 向 由 等 者 地 得 了 着 过 也 不 其 此 该 某 "
    "即 如 像 能 可 用 指 因 则 但 并 且 都 很 最 将 使 让 给 跟 同 比 按 用于 位于 一种 一类 一个 一些 "
    "於 用於 位於 这 那 这种 那种 各种 某种 之一 所有 没有 可以 用来 或者 以及 及其 并且 而且 因为 所以 如果 什么".split()
)

对照固定词 = ["有", "能", "用于", "用於", "位于", "位於", "像", "形容", "比喻", "总称", "统称"]
先天结构 = [
    ("一种N", ("一种", 空)),
    ("N的一种", (空, "的", "一种")),
    ("N的一类", (空, "的", "一类")),
    ("M的N", (空, "的", 空)),
]


def 读繁简(路径):
    表, 多 = {}, set()
    with open(路径, encoding="utf-8") as f:
        next(f)
        for 行 in f:
            列 = 行.rstrip("\n").split("\t")
            if len(列) != 2 or len(列[0]) != 1 or len(列[1]) != 1:
                continue
            if 列[0] in 表 and 表[列[0]] != 列[1]:
                多.add(列[0])
            表[列[0]] = 列[1]
    for 字 in 多:  # 一繁对多简保留原字
        del 表[字]
    return {ord(k): v for k, v in 表.items()}, len(多)


def 是汉字(字):
    c = ord(字)
    return (0x3400 <= c <= 0x9FFF) or (0x20000 <= c <= 0x3134F) or (0xF900 <= c <= 0xFAFF) or 字 == "〇"


def 是标点(字):
    return unicodedata.category(字)[0] in ("P", "S")


def 反转义(文):
    if "\\" not in 文:
        return 文
    出, i, n = [], 0, len(文)
    while i < n:
        c = 文[i]
        if c == "\\" and i + 1 < n:
            d = 文[i + 1]
            出.append({"n": "\n", "t": "\t", "r": "\r", "\\": "\\"}.get(d, d))
            i += 2
        else:
            出.append(c)
            i += 1
    return "".join(出)


class 切分器:
    def __init__(self, 词头集):
        self.词头 = 词头集
        self.首字最长 = {}
        for 词 in 词头集:
            if len(词) > self.首字最长.get(词[0], 0):
                self.首字最长[词[0]] = len(词)
        self.未命中单字 = 0
        self.非汉字串 = 0
        self.词总数 = 0
        self.标点词 = 0

    def 切(self, 句):
        出, i, n = [], 0, len(句)
        while i < n:
            c = 句[i]
            if c.isspace():
                i += 1
                continue
            if 是标点(c):
                出.append(c)
                self.标点词 += 1
                i += 1
                continue
            if not 是汉字(c):
                # 连续的非汉字、非标点、非空白字符（拉丁字母、数字、假名等）合成一个词
                j = i + 1
                while j < n and not 句[j].isspace() and not 是标点(句[j]) and not 是汉字(句[j]):
                    j += 1
                出.append(句[i:j])
                self.非汉字串 += 1
                i = j
                continue
            最长 = min(self.首字最长.get(c, 1), n - i)
            命中 = 0
            for 长 in range(最长, 0, -1):
                if 句[i:i + 长] in self.词头:
                    命中 = 长
                    break
            if 命中 == 0:
                命中 = 1
                self.未命中单字 += 1
            出.append(句[i:i + 命中])
            i += 命中
        self.词总数 += len(出)
        return 出


def 拆句(释义):
    for 段 in 反转义(释义).split("\n"):
        for 句 in 段.replace("。", "；").split("；"):
            句 = 句.strip()
            while 句 and (是标点(句[-1]) or 句[-1].isspace()):
                句 = 句[:-1]
            if 句:
                yield 句


def 有相异伙伴(t, 去重组, 位数, 预算):
    """是否存在另一条填充，在全部□位置上都与 t 不同（即两两比较恰好得到本句式）。"""
    for u in 去重组:
        预算[0] += 1
        for k in range(位数):
            if u[k] == t[k]:
                break
        else:
            return True
    return False


def 主程序():
    解析 = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    解析.add_argument("--词典目录", default="基础数据/词典")
    解析.add_argument("--报告目录", default="验证记录/词典句式比较")
    解析.add_argument("--全量输出目录", default="日志/词典句式比较", help="全量句式表，位置应被 Git 忽略")
    解析.add_argument("--最短句长", type=int, default=2)
    解析.add_argument("--最长句长", type=int, default=12)
    解析.add_argument("--空位上限", type=int, default=3, help="每条句式□位置数上限（第一轮）")
    解析.add_argument("--摘录条数", type=int, default=500)
    解析.add_argument("--抽样条数", type=int, default=100)
    解析.add_argument("--随机种子", type=int, default=20261113)
    参 = 解析.parse_args()
    起 = time.time()
    计时 = {}

    目录 = 参.词典目录
    繁简, 一繁多简数 = 读繁简(os.path.join(目录, "繁简字表.tsv"))

    def 简(文):
        return 文.translate(繁简)

    词头集 = set()
    原词头数 = 0
    with open(os.path.join(目录, "词头表.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            词 = 行.rstrip("\n")
            if 词:
                原词头数 += 1
                词头集.add(简(词))

    行号词性 = {}
    with open(os.path.join(目录, "词性表.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            a, b = 行.rstrip("\n").split("\t")
            行号词性[a] = b

    # 读词典：只取词头语种 zh
    词面词性 = collections.defaultdict(collections.Counter)
    句表 = {}       # 规范化句文本 -> 句号
    句文 = []       # 句号 -> 文本
    句词头 = []     # 句号 -> 第一个出处词头（简体）
    句出现 = []     # 句号 -> 出现次数（不同义项里的重复）
    义项数 = 0
    原句数 = 0
    with open(os.path.join(目录, "词典.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            列 = 行.split("\t", 5)
            if 列[1] != "zh":
                continue
            义项数 += 1
            词面 = 简(列[2])
            性 = 行号词性.get(列[0].rsplit(":", 1)[-1])
            if 性:
                词面词性[词面][性] += 1
            for 句 in 拆句(简(列[4])):
                原句数 += 1
                号 = 句表.get(句)
                if 号 is None:
                    句表[句] = len(句文)
                    句文.append(句)
                    句词头.append(词面)
                    句出现.append(1)
                else:
                    句出现[号] += 1
    计时["读入"] = time.time() - 起

    def 词的词性(词):
        计 = 词面词性.get(词)
        if not 计:
            return "未收"
        有 = [(n, p) for p, n in 计.items() if p != "unknown"]
        if not 有:
            return "unknown"
        return max(有)[1]

    def 是功能词_词性表(词):
        计 = 词面词性.get(词)
        return bool(计) and any(p in 功能词性 for p in 计)

    # 切分
    t0 = time.time()
    切 = 切分器(词头集)
    词号 = {}
    号词 = [None]  # 0 留给□
    句词 = []
    for 句 in 句文:
        串 = []
        for 词 in 切.切(句):
            号 = 词号.get(词)
            if 号 is None:
                号 = len(号词)
                词号[词] = 号
                号词.append(词)
            串.append(号)
        句词.append(tuple(串))
    计时["切分"] = time.time() - t0
    del 句表

    长度分布 = collections.Counter(len(s) for s in 句词)
    功能号_表 = {i for i, w in enumerate(号词) if w and 是功能词_词性表(w)}
    功能号_补 = 功能号_表 | {i for i, w in enumerate(号词) if w in 补充功能词}
    标点号 = {i for i, w in enumerate(号词) if w and all(是标点(c) for c in w)}

    档 = [(2, 2), (5, 2), (5, 5), (20, 2), (20, 5), (20, 20)]
    一轮计数 = {d: [0, 0, 0] for d in 档}  # 全部 / 功能词(词性表) / 功能词(含补充)
    二轮前 = {d: 0 for d in 档}
    # 第二轮：合并键 -> [支持句集合, 各槽填充集合列表, 来源一轮句式数, 来源句长集合]
    合并 = {}
    一轮 = []  # (支持数, 句式键, 各位不同词数, 支持句号抽样, 各位填充计数)
    未精确 = 0
    预算 = [0]
    随机 = random.Random(参.随机种子)
    os.makedirs(参.全量输出目录, exist_ok=True)
    os.makedirs(参.报告目录, exist_ok=True)
    全量路径 = os.path.join(参.全量输出目录, "第一轮句式全量.tsv")
    全量 = open(全量路径, "w", encoding="utf-8", newline="\n")
    全量.write("句式\t句长\t空位数\t支持句数\t各空位不同词数\t含功能词_词性表\t含功能词_含补充\n")

    t0 = time.time()
    按长 = collections.defaultdict(list)
    for 号, 串 in enumerate(句词):
        if 参.最短句长 <= len(串) <= 参.最长句长:
            按长[len(串)].append(号)
    参与句数 = sum(len(v) for v in 按长.values())

    for 长 in sorted(按长):
        句号组 = 按长[长]
        for 空数 in range(1, min(参.空位上限, 长 - 1) + 1):  # 至少留一个固定词
            for 位 in itertools.combinations(range(长), 空数):
                位集 = set(位)
                桶 = collections.defaultdict(list)
                for 号 in 句号组:
                    串 = 句词[号]
                    键 = tuple(0 if i in 位集 else 串[i] for i in range(长))
                    桶[键].append(号)
                for 键, 组 in 桶.items():
                    if len(组) < 2:
                        continue
                    填 = [tuple(句词[号][i] for i in 位) for 号 in 组]
                    if 空数 == 1:
                        if len(set(填)) < 2:
                            continue
                        支持 = 组
                        支持填 = 填
                    else:
                        if any(len({t[k] for t in 填}) < 2 for k in range(空数)):
                            continue
                        去重 = list(set(填))
                        随机.shuffle(去重)
                        有伴 = {}
                        for t in 去重:
                            if 预算[0] > 4_000_000_000:
                                有伴[t] = True
                                未精确 += 1
                            else:
                                有伴[t] = 有相异伙伴(t, 去重, 空数, 预算)
                        支持, 支持填 = [], []
                        for 号, t in zip(组, 填):
                            if 有伴[t]:
                                支持.append(号)
                                支持填.append(t)
                        if len(支持) < 2:
                            continue
                    位计 = [collections.Counter(t[k] for t in 支持填) for k in range(空数)]
                    if any(len(c) < 2 for c in 位计):
                        continue
                    N = len(支持)
                    K = min(len(c) for c in 位计)
                    固定 = [w for w in 键 if w]
                    含表 = any(w in 功能号_表 for w in 固定)
                    含补 = any(w in 功能号_补 for w in 固定)
                    for d in 档:
                        if N >= d[0] and K >= d[1]:
                            一轮计数[d][0] += 1
                            一轮计数[d][1] += 含表
                            一轮计数[d][2] += 含补
                    全量.write("%s\t%d\t%d\t%d\t%s\t%d\t%d\n" % (
                        " ".join(号词[w] if w else 空 for w in 键), 长, 空数, N,
                        ",".join(str(len(c)) for c in 位计), 含表, 含补))
                    if N >= 5:
                        一轮.append((N, 键, 位, [c if len(c) <= 4000 else collections.Counter(dict(c.most_common(4000))) for c in 位计],
                                   支持[:400], [len(c) for c in 位计]))
                    # 第二轮合并：连续□折叠为一个可变长槽
                    合键, 槽界 = [], []
                    i = 0
                    while i < 长:
                        if 键[i]:
                            合键.append(键[i])
                            i += 1
                        else:
                            j = i
                            while j < 长 and not 键[j]:
                                j += 1
                            合键.append(0)
                            槽界.append((i, j))
                            i = j
                    合键 = tuple(合键)
                    项 = 合并.get(合键)
                    if 项 is None:
                        项 = 合并[合键] = [set(), [set() for _ in 槽界], 0, set(), [0, 0, 0, 0, 0, 0]]
                    项[0].update(支持)
                    for s, (a, b) in zip(项[1], 槽界):
                        if len(s) < 64:  # 只需判断是否达到 K 档，上限以内精确
                            for 号 in 支持:
                                s.add(句词[号][a:b])
                                if len(s) >= 64:
                                    break
                    项[2] += 1
                    项[3].add(长)
                    for n, d in enumerate(档):
                        if N >= d[0] and K >= d[1]:
                            项[4][n] += 1
        print("句长 %d 完成，累计 %.0f 秒" % (长, time.time() - t0), file=sys.stderr)
    全量.close()
    计时["第一轮与合并"] = time.time() - t0

    # 第二轮计数
    二轮后_来源达标 = {d: 0 for d in 档}   # 由第一轮达标句式折叠而来的合并句式数
    二轮后_合并达标 = {d: 0 for d in 档}   # 合并后按并集支持数、槽填充数重新判定
    二轮后_合并达标_功能 = {d: [0, 0] for d in 档}
    变长合并条 = 0
    for 合键, 项 in 合并.items():
        N = len(项[0])
        K = min(len(s) for s in 项[1])
        固定 = [w for w in 合键 if w]
        含表 = any(w in 功能号_表 for w in 固定)
        含补 = any(w in 功能号_补 for w in 固定)
        if len(项[3]) > 1:
            变长合并条 += 1
        for n, d in enumerate(档):
            if 项[4][n]:
                二轮后_来源达标[d] += 1
            if N >= d[0] and K >= d[1]:
                二轮后_合并达标[d] += 1
                二轮后_合并达标_功能[d][0] += 含表
                二轮后_合并达标_功能[d][1] += 含补

    def 式文(键):
        return " ".join(号词[w] if w else 空 for w in 键)

    def 合式文(键):
        return " ".join(号词[w] if w else 多空 for w in 键)

    def 词性分布(计数):
        """按不同填充词计：每个词取其在词性表里最多的非 unknown 词性。"""
        分 = collections.Counter()
        for w in 计数:
            分[词的词性(号词[w])] += 1
        return 分

    def 分布文(分, 总不同):
        项 = ["%s %d" % (p, n) for p, n in 分.most_common(6)]
        return "不同词 %d：%s" % (总不同, "，".join(项))

    def 两类(分):
        已知 = [(n, p) for p, n in 分.items() if p not in ("未收", "unknown")]
        已知.sort(reverse=True)
        总 = sum(n for n, _ in 已知)
        if len(已知) >= 2 and 已知[0][0] >= 5 and 已知[1][0] >= 5 and 已知[1][0] / 总 >= 0.3:
            return "%s／%s" % (已知[0][1], 已知[1][1])
        return ""

    一轮.sort(key=lambda r: (-r[0], r[1]))

    # 摘录 TSV
    摘录路径 = os.path.join(参.报告目录, "第一轮句式摘录_支持句数前%d.tsv" % 参.摘录条数)
    with open(摘录路径, "w", encoding="utf-8", newline="\n") as f:
        f.write("序\t句式\t句长\t支持句数\t各空位不同词数\t含功能词_词性表\t含功能词_含补充\t各空位词性分布（代理）\t两类候选\t例句（至多5条，以｜分隔）\n")
        for n, (N, 键, 位, 位计, 例, 位不同) in enumerate(一轮[:参.摘录条数], 1):
            固定 = [w for w in 键 if w]
            分们 = [词性分布(c) for c in 位计]
            f.write("%d\t%s\t%d\t%d\t%s\t%d\t%d\t%s\t%s\t%s\n" % (
                n, 式文(键), len(键), N, ",".join(map(str, 位不同)),
                any(w in 功能号_表 for w in 固定), any(w in 功能号_补 for w in 固定),
                " ‖ ".join(分布文(a, b) for a, b in zip(分们, 位不同)),
                " ‖ ".join(两类(a) or "-" for a in 分们),
                "｜".join(句文[号] for 号 in 例[:5])))

    合并表 = sorted(((len(项[0]), 合键, 项) for 合键, 项 in 合并.items() if len(项[0]) >= 5 and min(len(s) for s in 项[1]) >= 5),
                 key=lambda r: (-r[0], r[1]))
    合并摘录路径 = os.path.join(参.报告目录, "第二轮合并句式摘录_支持句数前%d.tsv" % 参.摘录条数)
    with open(合并摘录路径, "w", encoding="utf-8", newline="\n") as f:
        f.write("序\t合并句式\t支持句数\t来源第一轮句式数\t来源句长\t含功能词_词性表\t含功能词_含补充\t例句（至多5条，以｜分隔）\n")
        for n, (N, 合键, 项) in enumerate(合并表[:参.摘录条数], 1):
            固定 = [w for w in 合键 if w]
            例 = sorted(项[0])[:5]
            f.write("%d\t%s\t%d\t%d\t%s\t%d\t%d\t%s\n" % (
                n, 合式文(合键), N, 项[2], ",".join(map(str, sorted(项[3]))),
                any(w in 功能号_表 for w in 固定), any(w in 功能号_补 for w in 固定),
                "｜".join(句文[号] for 号 in 例)))

    # 抽样（供人工看噪声）：从 N>=5 且 K>=5 的第一轮句式中等概率抽
    候选 = [r for r in 一轮 if min(r[5]) >= 5]
    抽 = 随机.sample(候选, min(参.抽样条数, len(候选)))
    抽样路径 = os.path.join(参.报告目录, "噪声抽样_第一轮N5K5_%d条.tsv" % len(抽))
    with open(抽样路径, "w", encoding="utf-8", newline="\n") as f:
        f.write("序\t句式\t支持句数\t各空位不同词数\t例句（至多5条，以｜分隔）\n")
        for n, (N, 键, 位, 位计, 例, 位不同) in enumerate(抽, 1):
            f.write("%d\t%s\t%d\t%s\t%s\n" % (n, 式文(键), N, ",".join(map(str, 位不同)), "｜".join(句文[号] for 号 in 例[:5])))

    # 自动可判的一类噪声：固定词里没有任何功能词（含补充表）、也没有标点——只剩内容词
    仅内容词 = sum(1 for r in 候选 if not any((w in 功能号_补) for w in r[1] if w))
    两类条 = [(r, [两类(词性分布(c)) for c in r[3]]) for r in 候选]
    两类条 = [(r, x) for r, x in 两类条 if any(x)]

    # 对照检查
    键索引 = {r[1]: r for r in 一轮}
    全量索引需 = {}

    def 查式(词们):
        键 = tuple(0 if w == 空 else 词号.get(w, -1) for w in 词们)
        return 键, 键索引.get(键)

    总耗时 = time.time() - 起

    # ---------- 报告 ----------
    出 = []
    写 = 出.append
    写("# 词典释义句式比较统计报告\n")
    写("本文件由 `工具/词典/释义句式比较统计.py` 生成（“人工观察”一节除外，来自同目录 `人工观察.md`）。只陈述统计事实和观察，不对办法是否可行、是否写入规范下结论。\n")
    写("## 1. 运行参数与耗时\n")
    写("- 运行命令：`python 工具/词典/释义句式比较统计.py`（项目根目录，参数均为默认值）")
    写("- 句长范围（词条数）：%d–%d；每条句式□位置上限：%d；每条句式至少保留 1 个固定词" % (参.最短句长, 参.最长句长, 参.空位上限))
    写("- 随机种子：%d（只用于噪声抽样和内部遍历顺序）" % 参.随机种子)
    写("- 耗时：读入 %.0f 秒，切分 %.0f 秒，第一轮与合并 %.0f 秒，合计 %.0f 秒" % (计时["读入"], 计时["切分"], 计时["第一轮与合并"], 总耗时))
    写("- Python %s" % sys.version.split()[0])
    写("- 全量第一轮句式表写在 `%s`（被 Git 忽略，不提交）\n" % 全量路径.replace("\\", "/"))
    写("## 2. 规模\n")
    写("| 项 | 数 |\n| --- | ---: |")
    写("| 汉语词头义项行 | %d |" % 义项数)
    写("| 拆出的释义句（含重复） | %d |" % 原句数)
    写("| 去重后的释义句（文本完全相同只算一句） | %d |" % len(句文))
    写("| 句长在 %d–%d 内、参与比较的句子 | %d |" % (参.最短句长, 参.最长句长, 参与句数))
    写("| 词头表词头数（原表／统一简体后） | %d／%d |" % (原词头数, len(词头集)))
    写("| 繁简字表中一繁对多简、保留原字的字数 | %d |" % 一繁多简数)
    写("| 切出的词条总数 | %d |" % 切.词总数)
    写("| 其中标点 | %d（%.2f%%） |" % (切.标点词, 100 * 切.标点词 / 切.词总数))
    写("| 其中未命中词头表的单个汉字 | %d（%.3f%%） |" % (切.未命中单字, 100 * 切.未命中单字 / 切.词总数))
    写("| 其中非汉字串（拉丁字母、数字、假名等连写合成一词） | %d（%.2f%%） |" % (切.非汉字串, 100 * 切.非汉字串 / 切.词总数))
    写("| 不同词条数 | %d |\n" % (len(号词) - 1))
    写("句长分布（去重后的句子，按词条数）：\n")
    写("| 句长 | 句数 | 占比 |\n| ---: | ---: | ---: |")
    总 = len(句文)
    for L in range(1, 16):
        写("| %d | %d | %.1f%% |" % (L, 长度分布.get(L, 0), 100 * 长度分布.get(L, 0) / 总))
    余 = sum(n for L, n in 长度分布.items() if L > 15)
    写("| 16 及以上 | %d | %.1f%% |\n" % (余, 100 * 余 / 总))
    写("## 3. 做法的实际定义\n")
    写("1. 释义按转义换行、“；”“。”拆句，去首尾空白和句末标点；释义和词头按繁简字表逐字统一为简体，一繁对多简的字保留原字。文本完全相同的句子合并为一句（否则同一句话在多个义项里重复出现会被算成多句支持）。")
    写("2. 切分：正向最长匹配，边界依据为统一简体后的词头表。标点（Unicode P、S 类）单独成词；空白丢弃；连续的非汉字、非标点字符合成一个词；未命中的汉字单独成词并计数。")
    写("3. 第一轮：对每个句长，枚举□位置组合（1 至上限个，且至少留一个固定词），把其余位置词条完全相同的句子归为一组。一条句式成立的最低条件与两两比较等价：支持句是组内能找到“在所有□位置上都与自己不同”的另一句的句子（两句逐位置比较恰好得到这条句式），支持句不少于 2 句。因此某位置所有句子填同一个词时，该位置不会是□。")
    写("4. 支持句数 N 是上述支持句的条数，K 是各□位置不同填充词数的最小值。")
    写("5. 第二轮：把第一轮句式里连续的□折叠成一个可容纳多个词的槽（记作 □+），折叠后相同的句式合并，支持句取并集。")
    if 未精确:
        写("6. 有 %d 个填充组合因比较次数超出预算未做精确伙伴判定，按成立计入。" % 未精确)
    写("")
    写("功能词判定：口径 b 用词性表，词条在词性表里出现过以下任一标签即算功能词：%s。口径 b2 在 b 之上加一张人工列出的补充表（%d 个词），仅作对照，不来自词性表。\n" % ("、".join(sorted(功能词性)), len(补充功能词)))
    写("词性表覆盖情况：切分得到的 %d 个不同词条中，%d 个在词性表里有非 unknown 词性，%d 个只有 unknown，%d 个没有对应的汉语词头记录。按词性表算作功能词的不同词条 %d 个；“的、是、有、在、和、之、以、不、了、等、人、地”等常用单字在词性表里只有 unknown，口径 b 认不出它们。\n" % (
        len(号词) - 1,
        sum(1 for w in 号词[1:] if 词的词性(w) not in ("未收", "unknown")),
        sum(1 for w in 号词[1:] if 词的词性(w) == "unknown"),
        sum(1 for w in 号词[1:] if 词的词性(w) == "未收"),
        len(功能号_表)))
    写("补充表全文：%s\n" % " ".join(sorted(补充功能词)))

    写("## 4. 各口径句式条数\n")
    写("第一轮（等长比较）：\n")
    写("| 支持句数 N ≥ | 各□不同词数 K ≥ | a 全部 | b 固定词含功能词（词性表） | b2 固定词含功能词（含补充表） |\n| ---: | ---: | ---: | ---: | ---: |")
    for d in 档:
        写("| %d | %d | %d | %d | %d |" % (d[0], d[1], *一轮计数[d]))
    写("")
    写("第二轮（合并变长）：\n")
    写("| N ≥ | K ≥ | 合并前（第一轮达标条数） | 合并后（由达标句式折叠而成的条数） | 合并后按并集重新判定达标的条数 | 其中含功能词（词性表） | 其中含功能词（含补充表） |\n| ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    for d in 档:
        写("| %d | %d | %d | %d | %d | %d | %d |" % (d[0], d[1], 一轮计数[d][0], 二轮后_来源达标[d], 二轮后_合并达标[d], *二轮后_合并达标_功能[d]))
    写("")
    写("合并后的句式共 %d 条，其中 %d 条由不同句长的第一轮句式合并而来（即确实发生了变长合并）。“按并集重新判定”一列的槽填充数只精确统计到 64，足够判断各档 K。\n" % (len(合并), 变长合并条))

    写("## 5. 支持句数最高的前 50 条句式（第一轮）\n")
    写("词性分布是代理：离线没有系统概念树，这里用词性表里填充词的词性代替，按不同填充词计数（每个词取它在词性表里出现最多的非 unknown 词性；“未收”表示没有这个词面的汉语词头记录，“unknown”表示有记录但词性未知）。它不等于概念根归属。单个位置不同填充词超过 4000 个时只统计出现最多的 4000 个。\n")
    for n, (N, 键, 位, 位计, 例, 位不同) in enumerate(一轮[:50], 1):
        固定 = [w for w in 键 if w]
        写("**%d. `%s`**　支持 %d 句；功能词：词性表 %s，含补充表 %s" % (
            n, 式文(键), N, "有" if any(w in 功能号_表 for w in 固定) else "无", "有" if any(w in 功能号_补 for w in 固定) else "无"))
        for k, c in enumerate(位计):
            分 = 词性分布(c)
            标 = 两类(分)
            常 = "、".join("%s×%d" % (号词[w], m) for w, m in c.most_common(5))
            写("- 第 %d 个□（位置 %d）：%s；最常见填充：%s%s" % (k + 1, 位[k] + 1, 分布文(分, 位不同[k]), 常, "；**两类候选：%s**" % 标 if 标 else ""))
        写("- 例句：" + "｜".join(句文[号] for 号 in 例[:5]))
        写("")

    写("## 6. 同一位置填充词分成两类的句式\n")
    写("判定（代理）：某个□位置的不同填充词按词性表词性计数，已知词性中最多的两类各不少于 5 个词，且第二类占已知词性的 30%% 以上。在 N≥5、K≥5 的 %d 条第一轮句式中，有 %d 条至少一个位置满足。支持句数最高的 30 条：\n" % (len(候选), len(两类条)))
    写("| 句式 | 支持句数 | 各□位置的两类 |\n| --- | ---: | --- |")
    for r, x in 两类条[:30]:
        写("| `%s` | %d | %s |" % (式文(r[1]), r[0], "；".join(v or "-" for v in x)))
    写("")

    写("## 7. 对照检查\n")
    写("### 7.1 现行三种先天释义结构\n")
    写("| 结构 | 第一轮句式 | 是否比出 | 支持句数 | 各□不同词数 | 例句 |\n| --- | --- | --- | ---: | --- | --- |")
    for 名, 词们 in 先天结构:
        键, r = 查式(词们)
        if r:
            写("| %s | `%s` | 是 | %d | %s | %s |" % (名, " ".join(词们), r[0], ",".join(map(str, r[5])), "｜".join(句文[号] for 号 in r[4][:5])))
        else:
            写("| %s | `%s` | 否（支持句不足 5 或未出现） | - | - | - |" % (名, " ".join(词们)))
    写("")
    写("同一固定词序列在第二轮的合并句式：\n")
    写("| 结构 | 合并句式 | 支持句数 | 来源句长 | 例句 |\n| --- | --- | ---: | --- | --- |")
    for 名, 词们 in 先天结构:
        键 = tuple(0 if w == 空 else 词号.get(w, -1) for w in 词们)
        项 = 合并.get(键)
        if 项:
            写("| %s | `%s` | %d | %s | %s |" % (名, 合式文(键), len(项[0]), ",".join(map(str, sorted(项[3]))), "｜".join(句文[号] for 号 in sorted(项[0])[-5:])))
        else:
            写("| %s | `%s` | 未出现 | - | - |" % (名, " ".join(词们).replace(空, 多空)))
    写("")
    写("词头表里是否有这些固定词：" + "，".join("%s %s" % (w, "有" if w in 词头集 else "无") for w in ["一种", "一类", "的"] + 对照固定词) + "。\n")
    写("### 7.2 规范写“另行裁决”的句型\n")
    写("下面按固定词列出第一轮（N≥5）里含该词的句式，按支持句数取前 12 条。固定词先按同一切分办法切开（词头表没有的词会被切成单字，如“形容”切为“形 容”），再找固定词里连续含有这串词条的句式。繁简字表没有“於→于”，释义里的“用於、位於”保持原字，所以“用于、位于”两行为空，实际情况看“用於、位於”两行。“……的总称／统称”另看 `的 总称`、`的 统称` 相邻出现的句式。\n")
    def 含串(序, 串):
        m = len(串)
        return any(序[i:i + m] == 串 for i in range(len(序) - m + 1))

    for w in 对照固定词:
        串 = tuple(词号.get(x, -1) for x in 切分器(词头集).切(w))
        含 = [r for r in 一轮 if 含串(r[1], 串)]
        写("**%s**（切分为 `%s`）：切分后出现在 %d 个句子里；含它的第一轮句式（N≥5）%d 条。" % (
            w, " ".join(号词[x] if x > 0 else "?" for x in 串), sum(1 for s in 句词 if 含串(s, 串)), len(含)))
        if 含:
            写("\n| 句式 | 支持句数 | 各□不同词数 | 例句 |\n| --- | ---: | --- | --- |")
            for r in 含[:12]:
                写("| `%s` | %d | %s | %s |" % (式文(r[1]), r[0], ",".join(map(str, r[5])), "｜".join(句文[x] for x in r[4][:3])))
        写("")
    for w in ("总称", "统称"):
        a, b = 词号.get("的"), 词号.get(w)
        含 = [r for r in 一轮 if any(r[1][i] == a and r[1][i + 1] == b for i in range(len(r[1]) - 1))] if a and b else []
        写("**的 %s**：相邻出现的第一轮句式（N≥5）%d 条。" % (w, len(含)))
        if 含:
            写("\n| 句式 | 支持句数 | 各□不同词数 | 例句 |\n| --- | ---: | --- | --- |")
            for r in 含[:12]:
                写("| `%s` | %d | %s | %s |" % (式文(r[1]), r[0], ",".join(map(str, r[5])), "｜".join(句文[x] for x in r[4][:3])))
        写("")

    写("## 8. 噪声的自动统计部分\n")
    写("在 N≥5、K≥5 的 %d 条第一轮句式中，固定词里没有任何功能词（按含补充表的口径）的有 %d 条（%.1f%%）；这一类包括只留下话题词或内容词的句式，也包括固定词只有标点的句式。固定词全部是标点的有 %d 条。人工抽样分类见下一节，抽样表为 `%s`。\n" % (
        len(候选), 仅内容词, 100 * 仅内容词 / max(1, len(候选)),
        sum(1 for r in 候选 if all(w in 标点号 for w in r[1] if w)), os.path.basename(抽样路径)))

    人工 = os.path.join(参.报告目录, "人工观察.md")
    if os.path.exists(人工):
        with open(人工, encoding="utf-8") as f:
            写(f.read().rstrip() + "\n")
    else:
        写("## 9. 人工观察\n\n（尚未填写）\n")

    写("## 附：随报告提交的文件\n")
    for p in (摘录路径, 合并摘录路径, 抽样路径):
        写("- `%s`" % os.path.basename(p))
    写("")
    报告路径 = os.path.join(参.报告目录, "20261113_词典释义句式比较统计报告.md")
    with open(报告路径, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(出))
    print("完成：%s，合计 %.0f 秒" % (报告路径.replace("\\", "/"), time.time() - 起), file=sys.stderr)


if __name__ == "__main__":
    主程序()
