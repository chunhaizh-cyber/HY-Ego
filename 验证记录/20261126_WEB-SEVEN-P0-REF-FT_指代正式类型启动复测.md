# 指代正式类型启动复测

- 工作区 D:/海中鱼巣，main。S0 141c96857640c4432bea1d9290a4909774a50f3a，计划 blob 1cd82c5a18db47222469470c602d44829aa3249d。[计划](<https://github.com/chunhaizh-cyber/HY-Ego/blob/141c96857640c4432bea1d9290a4909774a50f3a/%E8%AE%A1%E5%88%92/20261126_WEB-SEVEN-P0-REF-FT_%E6%8C%87%E4%BB%A3%E6%AD%A3%E5%BC%8F%E7%B1%BB%E5%9E%8B%E5%90%AF%E5%8A%A8%E8%AF%BB%E5%9B%9E%E4%BF%AE%E5%A4%8D%E5%AE%9E%E6%96%BD%E8%AE%A1%E5%88%92_v0.1.md>)
- 原实际失败 P0。根因是指代关系槽首次发布的正式 FT 引用未获同一严格解码承认。隔离 PID 35740 因致命弹窗无法用正常 UI 结束，在路径、启动时间、可执行摘要匹配后单独强制回收，退出 −1。[失败证据](D:/TEMP/海中鱼巣/WEB-SEVEN-ACCEPTANCE-0DB83/run-20261126-071217/P0-failure.md)
- 修复复用正式特征所有者读取，只在概念总根唯一登记的 1511 槽允许单一正式 FT 引用，检查同域、身份与形状，纳入槽及正式 FT 保护。普通信息引用形状仍沿原白名单。完整保护读取和通用概念读取同用严格解码。
- 实际 Release x64 编译和链接退出 0，0 错误、0 警告。原始工程输出 D:/TEMP/海中鱼巣/WEB-SEVEN-P0-REF-FT/release-x64-20261126-072120。
- 干净隔离普通应用 PID 8648 越过槽／正式 FT 启动读回及装配核验，监听正式网页，HTTP 200。实际 Chrome 显示运行控制台，点击「结束」后显示「交互宿主已结束」，进程真实退出 0，监听释放。启动守卫作实际核验，界面未单独展示槽及 FT 稳定编码。致命弹窗抑制选项只影响异常响应。原始材料 D:/TEMP/海中鱼巣/WEB-SEVEN-P0-REF-FT/run-20261126-073002。本轮仅为 P0，七组在独立同构建会话接续。

| 实际源码 | Git blob |
| --- | --- |
| 海中鱼巣/领域/新_特征类.cpp | 01e57dd61b95b46732309c9bbdb6f2a2151e06b4 |
| 海中鱼巣/领域/新_特征类.h | 394a96dcdd6fd4b7a5e1f039ccc970545c386a25 |
| 海中鱼巣/领域/概念_特征类.cpp | 1713043c49586847464a100aa9d30e9aefc72d9e |

| 原始证据 | SHA-256 |
| --- | --- |
| msbuild.log | 35ca6cc478e6b99d75ebd3ddc79efcdda7aeffd038fa85c2a239e71df379ce9f |
| inputs.txt | a7d2c5abec2df05df7ac39db425f86f3250189cce0ed65e8a5490ea7b5e2e12c |
| 海中鱼巣.exe | 4bd94e7d2c6024f675d2656a305530b93d1cb08a91444a4a7f8efd9f060632b0 |
| runtime-inputs.json | 6e6eaf491c9c7f731f9ee3dbf62c92ce7fda0f9b2fdf398a02fe08dc0f05044e |
| http-ready.json | 0fcaa80a1ab9d72d5b0c26c3ba93dd44ea2579ad3b8ada3f4f44ef0b5fa5919a |
| exit.json | 6c8899ff4b493da350a485737ad54d6bd9401fa05c9e940eef544a51a9b9e4c8 |
