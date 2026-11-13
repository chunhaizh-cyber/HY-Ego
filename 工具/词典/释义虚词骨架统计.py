#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""词典释义虚词骨架统计（离线实验，第二次）。

做法：取句、繁简统一、切分沿用同目录《释义句式比较统计.py》；先清掉维基词典版式标记；
句中属于虚词表的词条按原顺序保留作骨架，其间每一段连续的非虚词词条记为一个位置 □+；
同骨架的句子归为一组。只输出统计，不下结论。

仅用 Python 标准库，不联网，不改写任何输入数据。默认从项目根目录运行。
"""
from __future__ import annotations

import argparse
import collections
import importlib.util
import os
import random
import re
import sys
import time

_此目录 = os.path.dirname(os.path.abspath(__file__))
_规 = importlib.util.spec_from_file_location("释义句式比较统计", os.path.join(_此目录, "释义句式比较统计.py"))
前次 = importlib.util.module_from_spec(_规)
_规.loader.exec_module(前次)

槽 = "□+"

# 口径 A：上一次的补充表全表（85 词）。
虚词表A = set(前次.补充功能词)
# 口径 B：只留严格的虚词——助词、介词、连词、否定与程度副词（施工者按常识初判）。
虚词B_助词 = "的 之 地 得 了 着 过 所 等".split()
虚词B_介词 = "在 于 於 从 向 由 对 把 被 以 为 给 跟 同 比 按 将 因".split()
虚词B_连词 = "和 与 或 及 而 则 但 并 且 以及 并且 而且 因为 所以 如果 或者".split()
虚词B_副词 = "不 也 都 很 最".split()
虚词表B = set(虚词B_助词 + 虚词B_介词 + 虚词B_连词 + 虚词B_副词)
assert 虚词表B <= 虚词表A

逗顿 = {"，", "、", ","}
关注实词 = ["形", "容", "比喻", "指", "表示", "泛指", "一种"]
先天结构 = [
    ("一种N", "以“一种”开头", lambda s: s[0] == "一种" and len(s) > 1),
    ("N的一种", "以“的 一种”结尾", lambda s: len(s) > 2 and s[-2:] == ("的", "一种")),
    ("N的一类", "以“的 一类”结尾", lambda s: len(s) > 2 and s[-2:] == ("的", "一类")),
]
对照词 = ["有", "能", "用於", "位於", "像", "形容", "比喻", "总称", "统称"]

_方括 = re.compile(r"\[[^\[\]]*\]")
_波括 = re.compile(r"^\s*[(（][^()（）]*[～〜~][^()（）]*[)）]")
_行首 = re.compile(r"^[\s*#:：^‡†]+")


def 清洗(句):
    """返回（清洗后的句子或空串, 命中的规则集合）。"""
    命中 = set()
    新 = 句
    if _行首.match(新):
        新2 = _行首.sub("", 新)
        if 新2 != 新:
            命中.add("行首列表符号")
            新 = 新2
    新2 = _方括.sub("", 新)
    if 新2 != 新:
        命中.add("方括号标签")
        新 = 新2
    新2 = _波括.sub("", 新)
    if 新2 != 新:
        命中.add("带～的括注")
        新 = 新2
    新 = 新.strip()
    while 新 and (前次.是标点(新[-1]) or 新[-1].isspace()):
        新 = 新[:-1]
    while 新 and (新[0].isspace() or 新[0] in "*#:：^‡†"):
        新 = 新[1:]
    return 新, 命中


def 取骨架(串, 虚词集, 固定标点):
    """返回（骨架元组, 各位置填充元组列表）。骨架里位置记为 None。"""
    骨, 填, 当前 = [], [], []
    for 词 in 串:
        if 词 in 虚词集 or 词 in 固定标点:
            if 当前:
                骨.append(None)
                填.append(tuple(当前))
                当前 = []
            骨.append(词)
        else:
            当前.append(词)
    if 当前:
        骨.append(None)
        填.append(tuple(当前))
    return tuple(骨), 填


def 骨文(骨):
    return " ".join(槽 if w is None else w for w in 骨)


def 主程序():
    解析 = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    解析.add_argument("--词典目录", default="基础数据/词典")
    解析.add_argument("--报告目录", default="验证记录/词典句式比较")
    解析.add_argument("--全量输出目录", default="日志/词典句式比较", help="全量骨架表，位置应被 Git 忽略")
    解析.add_argument("--摘录条数", type=int, default=500)
    解析.add_argument("--抽样条数", type=int, default=100)
    解析.add_argument("--随机种子", type=int, default=20261113)
    参 = 解析.parse_args()
    起 = time.time()
    随机 = random.Random(参.随机种子)
    目录 = 参.词典目录
    os.makedirs(参.全量输出目录, exist_ok=True)
    os.makedirs(参.报告目录, exist_ok=True)

    繁简, _ = 前次.读繁简(os.path.join(目录, "繁简字表.tsv"))

    def 简(文):
        return 文.translate(繁简)

    词头集 = set()
    with open(os.path.join(目录, "词头表.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            词 = 行.rstrip("\n")
            if 词:
                词头集.add(简(词))
    行号词性 = {}
    with open(os.path.join(目录, "词性表.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            a, b = 行.rstrip("\n").split("\t")
            行号词性[a] = b

    词面词性 = collections.defaultdict(collections.Counter)
    原句集 = set()
    原句数 = 0
    with open(os.path.join(目录, "词典.tsv"), encoding="utf-8") as f:
        next(f)
        for 行 in f:
            列 = 行.split("\t", 5)
            if 列[1] != "zh":
                continue
            性 = 行号词性.get(列[0].rsplit(":", 1)[-1])
            if 性:
                词面词性[简(列[2])][性] += 1
            for 句 in 前次.拆句(简(列[4])):
                原句数 += 1
                原句集.add(句)

    def 词的词性(词):
        计 = 词面词性.get(词)
        if not 计:
            return "未收"
        有 = [(n, p) for p, n in 计.items() if p != "unknown"]
        return max(有)[1] if 有 else "unknown"

    # 清洗
    规则计 = collections.Counter()
    改动句 = 0
    清空句 = 0
    句集 = {}
    for 句 in sorted(原句集):
        新, 命中 = 清洗(句)
        for r in 命中:
            规则计[r] += 1
        if 新 != 句:
            改动句 += 1
        if not 新:
            清空句 += 1
            continue
        句集.setdefault(新, None)
    句文 = list(句集)
    切 = 前次.切分器(词头集)
    句词 = [tuple(切.切(s)) for s in 句文]
    保 = [i for i, s in enumerate(句词) if s]
    句文 = [句文[i] for i in 保]
    句词 = [句词[i] for i in 保]
    总句 = len(句文)
    读切耗时 = time.time() - 起

    def 是标点词(词):
        return all(前次.是标点(c) for c in 词)

    口径们 = [
        ("A", "全表（%d 词）" % len(虚词表A), 虚词表A, set()),
        ("B", "严格虚词（%d 词）" % len(虚词表B), 虚词表B, set()),
        ("A逗", "全表，句内逗号、顿号也作固定项", 虚词表A, 逗顿),
        ("B逗", "严格虚词，句内逗号、顿号也作固定项", 虚词表B, 逗顿),
    ]
    结果 = {}
    for 名, 说, 虚, 标 in 口径们:
        组 = collections.defaultdict(list)
        无虚 = 0
        无虚长1 = 0
        多虚 = 0
        for i, 串 in enumerate(句词):
            骨, 填 = 取骨架(串, 虚, 标)
            n虚 = sum(1 for w in 串 if w in 虚)
            if n虚 == 0:
                无虚 += 1
                if len(串) == 1:
                    无虚长1 += 1
            if n虚 >= 2:
                多虚 += 1
            组[骨].append(i)
        结果[名] = dict(说=说, 虚=虚, 标=标, 组=组, 无虚=无虚, 无虚长1=无虚长1, 多虚=多虚)

    def 位置统计(骨, 句号们, 虚, 标):
        n位 = sum(1 for w in 骨 if w is None)
        填计 = [collections.Counter() for _ in range(n位)]
        for i in 句号们:
            _, 填 = 取骨架(句词[i], 虚, 标)
            for k, t in enumerate(填):
                填计[k][t] += 1
        出 = []
        for c in 填计:
            总 = sum(c.values())
            长1 = sum(n for t, n in c.items() if len(t) == 1)
            长2 = sum(n for t, n in c.items() if len(t) == 2)
            性 = collections.Counter()
            for t in c:
                性[词的词性(t[-1])] += 1
            出.append(dict(不同=len(c), 长1=长1 / 总, 长2=长2 / 总, 长3=(总 - 长1 - 长2) / 总, 性=性,
                         常=c.most_common(4)))
        return 出

    def 位置文(统):
        段 = []
        for k, d in enumerate(统, 1):
            段.append("位置%d：不同填充 %d；长度 1词 %.0f%%／2词 %.0f%%／3词及以上 %.0f%%；末词词性（代理，按不同填充计）%s；最常见：%s" % (
                k, d["不同"], 100 * d["长1"], 100 * d["长2"], 100 * d["长3"],
                "，".join("%s %d" % (p, n) for p, n in d["性"].most_common(4)),
                "、".join("%s×%d" % ("".join(t), n) for t, n in d["常"])))
        return 段

    # 骨架条数：不计“只有一个位置、没有任何固定项”的骨架（即不含虚词的句子）
    def 条数(名):
        组 = 结果[名]["组"]
        有效 = [(len(v), k) for k, v in 组.items() if k != (None,)]
        return len(有效), sum(1 for n, _ in 有效 if n >= 2), sum(1 for n, _ in 有效 if n >= 5), sum(1 for n, _ in 有效 if n >= 20), \
            sum(n for n, _ in 有效 if n >= 5)

    出 = []
    写 = 出.append
    写("# 词典释义虚词骨架统计报告（第二次）\n")
    写("本文件由 `工具/词典/释义虚词骨架统计.py` 生成（“人工观察”一节除外，来自同目录 `第二次_人工观察.md`）。只陈述统计事实和观察，不对办法是否可行、是否写入规范下结论。上一次的报告是 `20261113_词典释义句式比较统计报告.md`，未改动。\n")

    # 全量输出与摘录
    摘录们 = []
    for 名 in ("A", "B", "A逗", "B逗"):
        r = 结果[名]
        全路 = os.path.join(参.全量输出目录, "第二次_骨架全量_口径%s.tsv" % 名)
        with open(全路, "w", encoding="utf-8", newline="\n") as f:
            f.write("骨架\t支持句数\t位置数\t固定项数\n")
            for 骨, v in sorted(r["组"].items(), key=lambda kv: (-len(kv[1]), 骨文(kv[0]))):
                f.write("%s\t%d\t%d\t%d\n" % (骨文(骨), len(v), sum(1 for w in 骨 if w is None), sum(1 for w in 骨 if w is not None)))
        if 名 in ("A", "B"):
            摘路 = os.path.join(参.报告目录, "第二次_骨架摘录_口径%s_支持句数前%d.tsv" % (名, 参.摘录条数))
            摘录们.append(摘路)
            with open(摘路, "w", encoding="utf-8", newline="\n") as f:
                f.write("序\t骨架\t支持句数\t各位置不同填充数\t各位置长度分布(1词/2词/3词及以上)\t例句（至多5条，以｜分隔）\n")
                排 = sorted(((len(v), 骨) for 骨, v in r["组"].items() if 骨 != (None,)), key=lambda x: (-x[0], 骨文(x[1])))
                for n, (N, 骨) in enumerate(排[:参.摘录条数], 1):
                    统 = 位置统计(骨, r["组"][骨], r["虚"], r["标"])
                    f.write("%d\t%s\t%d\t%s\t%s\t%s\n" % (
                        n, 骨文(骨), N, ",".join(str(d["不同"]) for d in 统),
                        " ‖ ".join("%.0f/%.0f/%.0f" % (100 * d["长1"], 100 * d["长2"], 100 * d["长3"]) for d in 统),
                        "｜".join(句文[i] for i in r["组"][骨][:5])))

    # 抽样：口径 A、支持句不少于 5 的骨架等概率抽
    rA = 结果["A"]
    候选 = sorted((骨 for 骨, v in rA["组"].items() if 骨 != (None,) and len(v) >= 5), key=骨文)
    抽 = 随机.sample(候选, min(参.抽样条数, len(候选)))
    抽路 = os.path.join(参.报告目录, "第二次_噪声抽样_口径A支持句至少5_%d条.tsv" % len(抽))
    with open(抽路, "w", encoding="utf-8", newline="\n") as f:
        f.write("序\t骨架\t支持句数\t各位置不同填充数\t例句（至多6条，以｜分隔）\n")
        for n, 骨 in enumerate(抽, 1):
            v = rA["组"][骨]
            统 = 位置统计(骨, v, rA["虚"], rA["标"])
            例 = 随机.sample(v, min(6, len(v)))
            f.write("%d\t%s\t%d\t%s\t%s\n" % (n, 骨文(骨), len(v), ",".join(str(d["不同"]) for d in 统), "｜".join(句文[i] for i in 例)))

    写("## 1. 运行参数与耗时\n")
    写("- 运行命令：`python 工具/词典/释义虚词骨架统计.py`（项目根目录，参数均为默认值）；新建脚本，取句、繁简统一、切分直接调用上一次的 `释义句式比较统计.py` 里的函数，上一次的脚本未改")
    写("- 不限句长，所有清洗后非空的句子都参与")
    写("- 随机种子：%d（只用于噪声抽样和例句抽取）" % 参.随机种子)
    写("- 全量骨架表写在 `%s/第二次_骨架全量_口径*.tsv`（被 Git 忽略，不提交）" % 参.全量输出目录.replace("\\", "/"))
    写("- 耗时见文末\n")

    写("## 2. 清洗\n")
    写("在拆句、繁简统一之后，切分之前，对每个句子依次做：\n")
    写("1. **行首列表符号**：去掉句首连续的 `*`、`#`、`:`、`：`、`^`、`‡`、`†` 和空白。")
    写("2. **方括号标签**：去掉句中所有不嵌套的半角方括号及其内容，如 `[T:化学]`、`[正:自由软体]`、`[通讯`（未闭合的不去）。")
    写("3. **带～的括注**：去掉句首一对圆括号及其内容，条件是括号里含 `～`（或 `〜`、`~`），如 `(～镇)`、`(～市)`。")
    写("4. 再去首尾空白和句末标点；清洗后为空的句子排除；清洗后文本相同的句子合并为一句。\n")
    写("歇后语的“前半 —— 后半”、字形说明套话（“此字是「X」的简化字”）、读音括注等不属于上述三种标记，没有清。\n")
    写("| 项 | 数 |\n| --- | ---: |")
    写("| 拆出的释义句（含重复） | %d |" % 原句数)
    写("| 去重后的释义句（清洗前） | %d |" % len(原句集))
    for r in ("行首列表符号", "方括号标签", "带～的括注"):
        写("| 命中“%s”的句子 | %d |" % (r, 规则计[r]))
    写("| 被清洗改动的句子（含只去掉首尾符号的） | %d |" % 改动句)
    写("| 清洗后为空而排除的句子 | %d |" % 清空句)
    写("| **清洗并再次去重后参与统计的句子** | **%d** |\n" % 总句)

    写("## 3. 虚词表与骨架的定义\n")
    写("**口径 A（全表，%d 词）**：上一次的补充表原样：%s\n" % (len(虚词表A), " ".join(sorted(虚词表A))))
    写("**口径 B（严格虚词，%d 词）**，施工者按常识初判：\n" % len(虚词表B))
    写("- 助词：%s" % " ".join(虚词B_助词))
    写("- 介词：%s" % " ".join(虚词B_介词))
    写("- 连词：%s" % " ".join(虚词B_连词))
    写("- 否定与程度副词：%s\n" % " ".join(虚词B_副词))
    写("**从全表里剔出、口径 B 不算虚词的 %d 个词**：%s\n" % (len(虚词表A - 虚词表B), " ".join(sorted(虚词表A - 虚词表B))))
    写("其中“是、有、没有、像、能、可、可以、用、指、使、让、用于、用於、位于、位於、用来、如、即”是动词或系词性的；“一种、一类、一个、一些、之一、所有、各种、某种、这种、那种”是数量或指量结构；“这、那、其、此、该、某、什么”是代词；“者、及其”分别是名词性后缀和“连词加代词”。“为”兼作“是”和介词，“等”兼作助词和动词，“同、比、跟、给、将、因、对、以”也有实词用法，初判仍留在 B 里。\n")
    写("**骨架**：句中属于虚词表的词条按原顺序保留；其间每一段连续的非虚词词条（至少 1 个，长度不限）记为一个位置 □+。同骨架的句子归为一组，支持句数就是组内句子数。\n")
    写("**标点**：基本口径（A、B）里标点不是固定项，留在所在位置的填充里（所以“记号，标记”整句是一个位置）。另出两个口径（A逗、B逗）：句内的“，”“、”（及半角逗号）也作固定项，其它标点仍留在填充里。\n")
    写("**骨架条数的算法**：不含任何固定项的句子（骨架就是一个 □+）单独计为“不含虚词的句子”，不算进骨架条数。没有位置、全由虚词组成的骨架算进条数。\n")

    写("## 4. 报告须回答的数字\n")
    写("### 4.1 不含虚词的句子\n")
    写("| 口径 | 参与句子 | 不含任何虚词的句子 | 占比 | 其中句长为 1 个词条的 | 占不含虚词句子的比例 | 含 2 个及以上虚词的句子 | 占比 |\n| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    for 名 in ("A", "B"):
        r = 结果[名]
        写("| %s %s | %d | %d | %.1f%% | %d | %.1f%% | %d | %.1f%% |" % (
            名, r["说"], 总句, r["无虚"], 100 * r["无虚"] / 总句, r["无虚长1"], 100 * r["无虚长1"] / max(1, r["无虚"]), r["多虚"], 100 * r["多虚"] / 总句))
    写("")
    写("### 4.2 骨架条数\n")
    写("| 口径 | 骨架总数 | 支持句 ≥2 | 支持句 ≥5 | 支持句 ≥20 | 支持句 ≥5 的骨架覆盖的句子数 | 占参与句子 |\n| --- | ---: | ---: | ---: | ---: | ---: | ---: |")
    for 名 in ("A", "B", "A逗", "B逗"):
        t = 条数(名)
        写("| %s %s | %d | %d | %d | %d | %d | %.1f%% |" % (名, 结果[名]["说"], t[0], t[1], t[2], t[3], t[4], 100 * t[4] / 总句))
    写("")
    写("上一次的同档条数（摘自上次报告，口径不同，仅并列）：\n")
    写("| 上一次 | 支持句 ≥2 | 支持句 ≥5 | 支持句 ≥20 |\n| --- | ---: | ---: | ---: |")
    写("| 第一轮（等长比较，各□不同词数 ≥2） | 26731 | 4263 | 559 |")
    写("| 第二轮（合并变长，按并集重判，各槽不同填充 ≥2） | 22501 | 4510 | 627 |")
    写("| 第一轮，各□不同词数 ≥5／≥20 | - | 2456 | 274 |")
    写("| 第二轮，各槽不同填充 ≥5／≥20 | - | 3749 | 480 |\n")
    写("上一次只有句长 2–12 的 83468 句参与，且同一句可以支持多条句式；这一次每句只属于一个骨架。\n")

    for 名 in ("A", "B"):
        r = 结果[名]
        写("## %s 口径 %s 支持句最多的前 50 个骨架\n" % ("5." if 名 == "A" else "6.", 名))
        排 = sorted(((len(v), 骨) for 骨, v in r["组"].items() if 骨 != (None,)), key=lambda x: (-x[0], 骨文(x[1])))
        for n, (N, 骨) in enumerate(排[:50], 1):
            写("**%d. `%s`**　支持 %d 句" % (n, 骨文(骨), N))
            for 行 in 位置文(位置统计(骨, r["组"][骨], r["虚"], r["标"])):
                写("- " + 行)
            写("- 例句：" + "｜".join(句文[i] for i in r["组"][骨][:5]))
            写("")
    写("## 7. 逗号、顿号作固定项时支持句最多的前 20 个骨架\n")
    for 名 in ("A逗", "B逗"):
        r = 结果[名]
        写("口径 %s：\n" % 名)
        写("| 骨架 | 支持句数 | 例句 |\n| --- | ---: | --- |")
        排 = sorted(((len(v), 骨) for 骨, v in r["组"].items() if 骨 != (None,)), key=lambda x: (-x[0], 骨文(x[1])))
        for N, 骨 in 排[:20]:
            写("| `%s` | %d | %s |" % (骨文(骨), N, "｜".join(句文[i] for i in r["组"][骨][:3])))
        写("")

    # 骨架词稳定性
    写("## 8. 虚词表里每个词出现的位置\n")
    写("对口径 A 表里的每个词，按它在句中的每次出现归类：句首（句子第一个词条）、句末（最后一个词条）、两侧都是实词（左右相邻词条都不是口径 A 的虚词、也不是标点）、其它（至少一侧是虚词或标点）。表中是句数（同一句里出现多次只在相应类别各计一次）。相邻实词的不同词数分左、右两列。只给数，不判定。是否属于口径 B 另列一栏。\n")
    写("| 词 | 口径B | 出现句数 | 句首 | 句末 | 两侧都是实词 | 其它 | 左邻不同实词数 | 右邻不同实词数 |\n| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |")
    位 = {w: [set(), set(), set(), set(), set(), set(), set()] for w in 虚词表A}
    for i, 串 in enumerate(句词):
        n = len(串)
        for j, w in enumerate(串):
            d = 位.get(w)
            if d is None:
                continue
            d[0].add(i)
            左 = 串[j - 1] if j > 0 else None
            右 = 串[j + 1] if j + 1 < n else None
            左实 = 左 is not None and 左 not in 虚词表A and not 是标点词(左)
            右实 = 右 is not None and 右 not in 虚词表A and not 是标点词(右)
            if j == 0:
                d[1].add(i)
            if j == n - 1:
                d[2].add(i)
            if 左实 and 右实:
                d[3].add(i)
            elif 0 < j < n - 1:
                d[4].add(i)
            if 左实:
                d[5].add(左)
            if 右实:
                d[6].add(右)
    for w, d in sorted(位.items(), key=lambda kv: -len(kv[1][0])):
        写("| %s | %s | %d | %d | %d | %d | %d | %d | %d |" % (w, "是" if w in 虚词表B else "", *(len(x) for x in d)))
    写("")

    # 找应当算骨架的实词（口径 B 的填充）
    写("## 9. 填充里位于开头或结尾、可换邻词很多的实词\n")
    rB = 结果["B"]
    写("做法：取口径 B 下所有位置的填充（不含虚词的句子整句就是一个填充），按标点再切开成小段，只看 2 个词条及以上的小段，去重。对每个词条统计：它作小段第一个词时后面紧跟的不同词条数（“开头·后接”），它作小段最后一个词时前面紧邻的不同词条数（“结尾·前接”）。标点和非汉字串不计入。用口径 B 是因为“指、一种、有”等在口径 A 里已经是骨架词，不会出现在填充里。按“开头·后接”降序列前 100；“结尾·前接”降序的前 100 另列。\n")
    小段 = set()
    for 串 in 句词:
        _, 填 = 取骨架(串, 虚词表B, set())
        for t in 填:
            当 = []
            for w in t:
                if 是标点词(w):
                    if len(当) >= 2:
                        小段.add(tuple(当))
                    当 = []
                else:
                    当.append(w)
            if len(当) >= 2:
                小段.add(tuple(当))
    首后 = collections.defaultdict(set)
    尾前 = collections.defaultdict(set)
    首次 = collections.Counter()
    尾次 = collections.Counter()
    for t in 小段:
        首后[t[0]].add(t[1])
        尾前[t[-1]].add(t[-2])
        首次[t[0]] += 1
        尾次[t[-1]] += 1

    def 汉(w):
        return all(前次.是汉字(c) for c in w)
    首排 = sorted(((len(v), w) for w, v in 首后.items() if 汉(w)), reverse=True)
    尾排 = sorted(((len(v), w) for w, v in 尾前.items() if 汉(w)), reverse=True)
    首名 = {w: n for n, (_, w) in enumerate(首排, 1)}
    尾名 = {w: n for n, (_, w) in enumerate(尾排, 1)}
    写("不同小段共 %d 个。\n" % len(小段))
    写("关注词的名次（“形容”被切成“形”“容”，看“形”；“一种”在口径 A 里是骨架词，在口径 B 里是实词）：\n")
    写("| 词 | 开头·后接不同词数 | 名次 | 是否进前 100 | 结尾·前接不同词数 | 名次 |\n| --- | ---: | ---: | --- | ---: | ---: |")
    for w in 关注实词:
        写("| %s | %d | %s | %s | %d | %s |" % (w, len(首后.get(w, ())), 首名.get(w, "-"), "是" if 首名.get(w, 10 ** 9) <= 100 else "否", len(尾前.get(w, ())), 尾名.get(w, "-")))
    写("")
    写("“开头·后接”前 100（括号内：后接不同词数／作开头的小段数；★ 为关注词，△ 为在口径 A 表里的词）：\n")
    写("；".join("%d %s%s（%d／%d）" % (n, w, "★" if w in 关注实词 else ("△" if w in 虚词表A else ""), k, 首次[w]) for n, (k, w) in enumerate(首排[:100], 1)) + "\n")
    写("“结尾·前接”前 100（括号内：前接不同词数／作结尾的小段数）：\n")
    写("；".join("%d %s%s（%d／%d）" % (n, w, "★" if w in 关注实词 else ("△" if w in 虚词表A else ""), k, 尾次[w]) for n, (k, w) in enumerate(尾排[:100], 1)) + "\n")

    # 套叠
    写("## 10. 套叠的迹象\n")
    写("含 2 个及以上虚词的句子：口径 A %d 句（%.1f%%），口径 B %d 句（%.1f%%）。\n" % (
        结果["A"]["多虚"], 100 * 结果["A"]["多虚"] / 总句, 结果["B"]["多虚"], 100 * 结果["B"]["多虚"] / 总句))
    for 名 in ("A", "B"):
        r = 结果[名]
        写("口径 %s 支持句最多的 30 个多虚词骨架（固定项里虚词不少于 2 个）：\n" % 名)
        写("| 骨架 | 支持句数 | 例句 |\n| --- | ---: | --- |")
        排 = sorted(((len(v), 骨) for 骨, v in r["组"].items() if sum(1 for w in 骨 if w is not None) >= 2), key=lambda x: (-x[0], 骨文(x[1])))
        for N, 骨 in 排[:30]:
            写("| `%s` | %d | %s |" % (骨文(骨), N, "｜".join(句文[i] for i in r["组"][骨][:3])))
        写("")
    写("指定骨架的例句（口径 A，各随机取 10 条）：\n")
    for 骨 in [(None, "的", None, "的", None), ("有", None, "的", None), ("没有", None, "的", None)]:
        v = rA["组"].get(骨, [])
        写("**`%s`**　支持 %d 句" % (骨文(骨), len(v)))
        for i in 随机.sample(v, min(10, len(v))):
            _, 填 = 取骨架(句词[i], 虚词表A, set())
            写("- %s　〔%s〕" % (句文[i], "｜".join(" ".join(t) for t in 填)))
        写("")

    # 对照
    写("## 11. 对照检查\n")
    写("### 11.1 三种先天释义结构\n")
    for 口 in ("A", "B"):
        r = 结果[口]
        写("口径 %s：\n" % 口)
        写("| 结构 | 判定 | 符合的句子数 | 落在的骨架（句数，至多 6 个） |\n| --- | --- | ---: | --- |")
        for 名, 判, 函 in 先天结构:
            计 = collections.Counter()
            for i, 串 in enumerate(句词):
                if 函(串):
                    计[取骨架(串, r["虚"], r["标"])[0]] += 1
            写("| %s | %s | %d | %s |" % (名, 判, sum(计.values()), "；".join("`%s` %d" % (骨文(k), n) for k, n in 计.most_common(6))))
        骨 = (None, "的", None)
        写("| M的N | 骨架 `□+ 的 □+` | %d | 例：%s |" % (len(r["组"].get(骨, [])), "｜".join(句文[i] for i in r["组"].get(骨, [])[:5])))
        写("")
    写("### 11.2 “另行裁决”的句型\n")
    写("对每个词（先按同一办法切分，“形容”切为“形 容”），找出含这串词条的句子，看它们落在哪些骨架。繁简字表没有“於”，所以看“用於、位於”。\n")
    for w in 对照词:
        串w = tuple(切.切(w))
        m = len(串w)
        for 口 in ("A", "B"):
            r = 结果[口]
            计 = collections.Counter()
            for 串 in 句词:
                if any(串[i:i + m] == 串w for i in range(len(串) - m + 1)):
                    计[取骨架(串, r["虚"], r["标"])[0]] += 1
            写("- **%s**（口径 %s，%s）：%d 句，落在 %d 个骨架；最多的：%s" % (
                w, 口, "骨架词" if all(x in r["虚"] for x in 串w) else "实词", sum(计.values()), len(计),
                "；".join("`%s` %d" % (骨文(k), n) for k, n in 计.most_common(8))))
    写("")

    人工 = os.path.join(参.报告目录, "第二次_人工观察.md")
    if os.path.exists(人工):
        with open(人工, encoding="utf-8") as f:
            写(f.read().rstrip() + "\n")
    else:
        写("## 12. 人工观察\n\n（尚未填写）\n")
    写("## 附：耗时与随报告提交的文件\n")
    写("- 读入、清洗、切分 %.0f 秒；合计 %.0f 秒；Python %s" % (读切耗时, time.time() - 起, sys.version.split()[0]))
    for p in 摘录们 + [抽路]:
        写("- `%s`" % os.path.basename(p))
    写("")
    报告路径 = os.path.join(参.报告目录, "20261113_词典释义虚词骨架统计报告_第二次.md")
    with open(报告路径, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(出))
    print("完成：%s，合计 %.0f 秒" % (报告路径.replace("\\", "/"), time.time() - 起), file=sys.stderr)


if __name__ == "__main__":
    主程序()
