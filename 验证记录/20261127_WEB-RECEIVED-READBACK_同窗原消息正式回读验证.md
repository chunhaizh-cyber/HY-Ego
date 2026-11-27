# WEB-RECEIVED-READBACK 同窗原消息正式回读验证

冻结计划 54bcb2e4a，计划 blob 4ab44ddf0453828dae3a266a5c99c64397c3e335；执行登记 5c489d532。依据 [7241](../规范/7241_子规范_句子转换与交互场景同步.md) 的同源消息及持久解释合同。

已被外设事实接纳的网页 POST 回执带 ingressSequence。指定读取以本宿主会话、对话框和该入站号请求 GET /api/control/received?id=序号。在后台复用外设确认、原消息角色及语言解释所有者的正式读口。HTTP 线程只取有界回执。结果区分当前解释和独立采用；错误与等待读取不回传正式资料。原最近观察接口及所有业务写权维持原合同。

## 实证

实际海中鱼巣.vcxproj Release x64，258 项源输入，MSBuild 0 错误、0 警告。构建 D:/TEMP/海中鱼巣/WEB-INTERACTION-ENTRY/release-20261127-113739，SHA256 5C060B2E71BF7DB4B1F7D2DBB0CBFECB36F397E14F0B44C26E81208A0122BDC3。

普通网页真实 HTTP 验证原件 D:/TEMP/海中鱼巣/WEB-INTERACTION-ENTRY/run-20261127-113827。A 窗两次入站分别是 M34986、M35914，分别读回定义 35445、36679 和采用 35902、37136。第二条已正式采用后，凭第一条原回执独立重读到相同的第一条 M、解释及采用。从未入站、未确认本窗、未确认异窗、确认后的异窗及确认前旧 M 均返回 HTTP 404 与单一 notFound 状态；不符来源返回 403。PID 49148 经普通关闭 HTTP 200 后自然退出 0。

原件含 independent-source-result.json、两份入站回执、逐次读取与反例响应、start.json、close.json、exit.json。验收脚本位于 D:/TEMP/海中鱼巣/WEB-INTERACTION-ENTRY/verify-web-received-readback.ps1。此片只验证同窗原消息的观察读口，没有复跑第 2 组或改变第 1 组待裁决的概念规则，原七组通过数未变。

## 复用回写候选

宿主.网页对话.cpp 的 处理控制台指定接收请求 只验证传输范围；装配.普通应用.cpp 的 指定接收材料 由既有采集线程组合查找外设消息、读正式角色和读取接收语言解释；缓存仅保留单请求值式回执，可复用于同合同的接收验证。
