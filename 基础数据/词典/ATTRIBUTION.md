# 词典数据署名与修改说明

主要作者为中文维基词典贡献者。每行 `来源页URL` 指向原始词页；把该 URL 的 `/wiki/` 改为 `/w/index.php?title=` 并使用原始词面 URL 编码、追加 `&action=history` 可查看贡献者历史。例如：

- 苹果：https://zh.wiktionary.org/wiki/%E8%98%8B%E6%9E%9C
- 苹果页面历史：https://zh.wiktionary.org/w/index.php?title=%E8%98%8B%E6%9E%9C&action=history
- water：https://zh.wiktionary.org/wiki/water
- water 页面历史：https://zh.wiktionary.org/w/index.php?title=water&action=history

现网页及历史可能继续更新；本包精确输入以 `来源清单.json` 中原始 gzip 文件 SHA256、dump 日期及提取版本确定，逐行定位以原始文件名、解压 JSONL 行号和 sense 序号确定。不将当前网页版本冒充本次 dump 的不可变修订。

提取与发布来源：

- Kaikki.org / Tatu Ylonen：https://kaikki.org/zhwiktionary/rawdata.html
- Wiktextract：https://github.com/tatuylonen/wiktextract ，来源页所示提交 `19f9a3006a405fb7bfb40193dca41c7117a009a5`。
- Wikitextprocessor：https://github.com/tatuylonen/wikitextprocessor ，来源页所示提交 `6f96ddf5f3466707f1869fe6618390fd77dfd109`。
- 如用于学术研究，来源建议引用：Tatu Ylonen, “Wiktextract: Wiktionary as Machine-Readable Structured Data”, LREC 2022, pp. 1317–1325：https://aclanthology.org/2022.lrec-1.140/ 。

海中鱼巣首包修改：保留 `lang_code=zh/en` 且含汉字的非空 `glosses`；同一 sense 的 gloss 按原顺序以换行连接，不跨 sense 合并；词面保持原大小写、繁简和 Unicode 表示；不改写或翻译释义；将指定读音字段与原 tags 编码为 JSON；移除其它原始字段，包括例句、音频与图片。生成记录键、源定位和 13 列 TSV 四转义。本包是上述来源的筛选与格式整理，不是独立撰写的词典。

简体包修改（2026-11-13）：`词典.简体.tsv` 用 OpenCC（Open Chinese Convert，https://github.com/BYVoid/OpenCC ，ver.1.4.2，提交 `025f371dc76b598d77384fbdab90c937471844d8`，Apache License 2.0）的繁转简词组表 `TSPhrases.txt` 与字表 `TSCharacters.txt` 把词面、释义和例句转为简体，其余内容不变。OpenCC 许可原文随原始目录保留于 `原始/opencc-1.4.2/LICENSE`；转换结果仍是上述词典数据的修改版本，数据许可不因转换改变。

原始数据及大型 TSV 本地保留且不进 Git。重新分发数据必须保留本署名、许可说明和来源清单，不能只复制代码许可证。

完整批次分发同时把 OpenCC 许可原字节复制为批次内的 `OpenCC-LICENSE.txt`；不要求接收者取得被忽略的原始目录才能读取许可。批次构建前按来源清单核对 OpenCC 输入，发布清单保存许可摘要。
