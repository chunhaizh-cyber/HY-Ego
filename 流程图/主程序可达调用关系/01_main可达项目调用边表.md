# main 可达项目调用边表

代码基线：<code>03a8b7ac099ccea83e57f2696e2290b7bcdfacaf</code>；配置：<code>Debug\|x64</code>；项目调用边：4109。

| 边 ID | 调用方 | 被调方 | 调用点 | 类别 | 参数绑定 | 结果绑定 | 可达条件 | 解析依据 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| E0002 | F0001 | F0003 | <code>海中鱼巣/入口.cpp:10</code> | direct_const_member | this=&amp;解析 | 解析成功 | after E0001 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0003 | F0001 | F0004 | <code>海中鱼巣/入口.cpp:11, 海中鱼巣/入口.cpp:14</code> | direct_free_function+unique_name | 解析, nullptr | 进程退出码 | 解析成功 == false；after E0004；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0004 | F0001 | F0005 | <code>海中鱼巣/入口.cpp:13</code> | direct_free_function+unique_name | 解析.选项 | 运行 | 解析成功 == true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0001 | F0001 | F0002 | <code>海中鱼巣/入口.cpp:9</code> | direct_free_function+unique_name | 参数数量, 参数组 | 解析 | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0006 | F0004 | F0003 | <code>海中鱼巣/启动.程序入口.ixx:74</code> | direct_const_member | this=&amp;解析 | 解析成功 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0007 | F0004 | F0006 | <code>海中鱼巣/启动.程序入口.ixx:80</code> | direct_const_member | this=运行 | 运行成功 | 解析已接受 &amp;&amp; 运行 != nullptr | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0008 | F0005 | F0007 | <code>海中鱼巣/启动.应用程序.ixx:448</code> | direct_free_function | 选项.模式 | 普通结果 | 普通控制面板 \|\| 无窗口常驻 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0009 | F0005 | F0008 | <code>海中鱼巣/启动.应用程序.ixx:450</code> | direct_free_function |  | 生产结果 | 生产运行期 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0010 | F0005 | F0009 | <code>海中鱼巣/启动.应用程序.ixx:452</code> | direct_free_function |  | 自检结果 | 完整自检 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0011 | F0005 | F0010 | <code>海中鱼巣/启动.应用程序.ixx:454</code> | direct_free_function | 覆盖端口=nullptr | 数据库结果 | 数据库专项 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0012 | F0005 | F0011 | <code>海中鱼巣/启动.应用程序.ixx:456</code> | direct_free_function |  | 性能结果 | 关系仓库性能专项 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0022 | F0007 | F0021 | <code>海中鱼巣/启动.应用程序.ixx:101</code> | direct_free_function+unique_name | *装配.上下文, *信号.租约 | 运行结果 | 信号成功 &amp;&amp; 无窗口常驻；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0026 | F0007 | F0025 | <code>海中鱼巣/启动.应用程序.ixx:102, 海中鱼巣/启动.应用程序.ixx:107, 海中鱼巣/启动.应用程序.ixx:110</code> | compiler_implicit_destructor | this=信号.租约.get() |  | 无窗口宿主返回后；投影失败返回前；控制面板宿主返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0023 | F0007 | F0022 | <code>海中鱼巣/启动.应用程序.ixx:103</code> | direct_free_function+unique_name | *装配.上下文, 初始化 | 投影 | 信号成功 &amp;&amp; 普通控制面板；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0024 | F0007 | F0023 | <code>海中鱼巣/启动.应用程序.ixx:104</code> | direct_const_member | this=&amp;投影 | 投影成功 | after E0023 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0025 | F0007 | F0024 | <code>海中鱼巣/启动.应用程序.ixx:108</code> | direct_free_function+unique_name | 上下文, 初始化, 投影, 审计, 信号租约 | 运行结果 | 投影成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0014 | F0007 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:51</code> | direct_free_function+unique_name | 配置 | 装配 | 普通模式有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0015 | F0007 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:52</code> | direct_const_member | this=&amp;装配 | 装配成功 | after E0014 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0001 | F0007 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:59</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0002 | F0007 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:60</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0003 | F0007 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:61</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0016 | F0007 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:64</code> | direct_free_function+unique_name | 系统端口, 初始化请求 | 初始化 | 装配成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0017 | F0007 | F0016 | <code>海中鱼巣/启动.应用程序.ixx:66</code> | direct_const_member | this=&amp;初始化 | 初始化成功 | after E0016 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0018 | F0007 | F0017 | <code>海中鱼巣/启动.应用程序.ixx:67</code> | lambda_direct | 捕获初始化 | 失败阶段 | 初始化失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0004 | F0007 | F0053 | <code>海中鱼巣/启动.应用程序.ixx:82</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0005 | F0007 | F0054 | <code>海中鱼巣/启动.应用程序.ixx:87</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0019 | F0007 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:95</code> | direct_free_function+unique_name | 审计端口, 最佳努力 | 审计 | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0020 | F0007 | F0019 | <code>海中鱼巣/启动.应用程序.ixx:96</code> | direct_free_function+unique_name | 默认安装函数 | 信号 | after E0019；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0021 | F0007 | F0020 | <code>海中鱼巣/启动.应用程序.ixx:97</code> | direct_const_member | this=&amp;信号 | 信号成功 | after E0020 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0029 | F0008 | F0026 | <code>海中鱼巣/启动.应用程序.ixx:37</code> | direct_constructor |  | 会话 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0030 | F0008 | F0027 | <code>海中鱼巣/启动.应用程序.ixx:38</code> | direct_member | this=&amp;会话, 生产运行期启动请求{} | 结果 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0031 | F0008 | F0028 | <code>海中鱼巣/启动.应用程序.ixx:39</code> | direct_const_member | this=&amp;结果 | 启动成功 | after E0030 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0032 | F0009 | F0029 | <code>海中鱼巣/启动.应用程序.ixx:381</code> | direct_free_function+unique_name | 运行器, 空配置 | 入口登记成功 | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0033 | F0009 | F0030 | <code>海中鱼巣/启动.应用程序.ixx:382-383, 海中鱼巣/启动.应用程序.ixx:384-385</code> | direct_member | this=&amp;运行器, 150, F27编号名称, 回调 | 端口登记成功 | 入口登记成功；入口登记成功 &amp;&amp; 端口登记成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0036 | F0009 | F0032 | <code>海中鱼巣/启动.应用程序.ixx:383</code> | callback_bind+lsp_direct | 无捕获转发回调 | F27回调槽 | 端口自检登记；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0037 | F0009 | F0033 | <code>海中鱼巣/启动.应用程序.ixx:385</code> | callback_bind+lsp_direct | 无捕获转发回调 | F29回调槽 | 隔离自检登记；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0035 | F0009 | F0031 | <code>海中鱼巣/启动.应用程序.ixx:390</code> | direct_member+unique_name | this=&amp;运行器 | 批次 | 三段登记均成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0040 | F0010 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:114</code> | direct_free_function+unique_name | 配置 | 装配 | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0041 | F0010 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:115</code> | direct_const_member | this=&amp;装配 | 装配成功 | after E0040 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0006 | F0010 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:123</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0007 | F0010 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:124</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0008 | F0010 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:125</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0042 | F0010 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:128, 海中鱼巣/启动.应用程序.ixx:128-129</code> | direct_free_function+unique_name | 系统端口, 初始化请求 | 初始化 | 装配成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0043 | F0010 | F0016 | <code>海中鱼巣/启动.应用程序.ixx:130</code> | direct_const_member | this=&amp;初始化 | 初始化成功 | after E0042 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0044 | F0010 | F0034 | <code>海中鱼巣/启动.应用程序.ixx:131-140</code> | lambda_direct | 捕获初始化 | 失败阶段 | 初始化失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0009 | F0010 | F0053 | <code>海中鱼巣/启动.应用程序.ixx:146</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0010 | F0010 | F0054 | <code>海中鱼巣/启动.应用程序.ixx:151</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0045 | F0010 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:159, 海中鱼巣/启动.应用程序.ixx:159-161</code> | direct_free_function+unique_name | 选中端口, 必须往返一致 | 审计 | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0046 | F0010 | F0035 | <code>海中鱼巣/启动.应用程序.ixx:163, 海中鱼巣/启动.应用程序.ixx:167</code> | direct_const_member | this=&amp;审计 | 摘要成功 | after E0045；always after summary | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0050 | F0011 | F0037 | <code>海中鱼巣/启动.应用程序.ixx:402</code> | direct_free_function+unique_name |  | 结果 | 性能能力宏开启；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0054 | F0013 | F0039 | <code>海中鱼巣/装配.普通应用.ixx:129</code> | direct_const_member | this=&amp;配置 | 配置有效 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0055 | F0013 | F0040 | <code>海中鱼巣/装配.普通应用.ixx:133</code> | external_template_project_constructor | this=新分配存储 | 完整上下文 | 配置有效 &amp;&amp; make_unique分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0061 | F0015 | F0046 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_normal |  | 快照候选 | 等待回调返回 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0197 | F0015 | F0123 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 快照候选 | 端口绑定来自F0032且等待回调已执行；四轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0989 | F0015 | F0473 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0254；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1008 | F0015 | F0480 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0255；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1030 | F0015 | F0486 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0256；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1051 | F0015 | F0492 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0257；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1084 | F0015 | F0501 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0258；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1103 | F0015 | F0507 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0259；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1125 | F0015 | F0513 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0260；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1153 | F0015 | F0520 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0261；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1175 | F0015 | F0527 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0262；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1196 | F0015 | F0535 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0263；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1218 | F0015 | F0541 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0264；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1238 | F0015 | F0548 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0265；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1271 | F0015 | F0559 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test |  | 初始化快照候选 | 端口绑定来自F0266；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0062 | F0015 | F0047 | <code>海中鱼巣/领域/初始化.系统.ixx:103</code> | direct_const_member | this=快照对象 | 快照成功 | 等待成功 &amp;&amp; 快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0063 | F0015 | F0048 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_normal |  | 概念图候选 | 快照成功 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0198 | F0015 | F0124 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图候选 | 端口绑定来自F0032且快照成功；两轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0990 | F0015 | F0474 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0254；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1009 | F0015 | F0481 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0255；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1031 | F0015 | F0487 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0256；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1052 | F0015 | F0493 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0257；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1085 | F0015 | F0502 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0258；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1104 | F0015 | F0508 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0259；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1126 | F0015 | F0514 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0260；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1154 | F0015 | F0521 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0261；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1176 | F0015 | F0528 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0262；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1197 | F0015 | F0536 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0263；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1219 | F0015 | F0542 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0264；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1239 | F0015 | F0549 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0265；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1272 | F0015 | F0560 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test |  | 概念图初始化结果 | 端口绑定来自F0266；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0064 | F0015 | F0049 | <code>海中鱼巣/领域/初始化.系统.ixx:108</code> | direct_const_member | this=&amp;结果.概念图 | 概念图成功 | after E0063 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0065 | F0015 | F0050 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_normal | 自我存在节点 | 支持候选 | 概念图成功 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0199 | F0015 | F0125 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 支持候选 | 端口绑定来自F0032且概念图成功；失败点5一轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0991 | F0015 | F0475 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0254；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1010 | F0015 | F0482 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0255；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1032 | F0015 | F0488 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0256；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1053 | F0015 | F0494 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0257；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1086 | F0015 | F0503 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0258；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1105 | F0015 | F0509 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0259；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1127 | F0015 | F0515 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0260；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1155 | F0015 | F0522 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0261；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1177 | F0015 | F0529 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0262；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1198 | F0015 | F0537 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0263；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1220 | F0015 | F0543 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0264；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1240 | F0015 | F0550 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0265；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1273 | F0015 | F0561 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 自我存在节点 | 存在根支持候选 | 端口绑定来自F0266；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0066 | F0015 | F0051 | <code>海中鱼巣/领域/初始化.系统.ixx:115</code> | direct_operator | 支持根, 存在根 | 根相等 | 支持有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0056 | F0015 | F0041 | <code>海中鱼巣/领域/初始化.系统.ixx:79</code> | direct_const_member | this=&amp;请求 | 请求有效 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0057 | F0015 | F0042 | <code>海中鱼巣/领域/初始化.系统.ixx:83</code> | direct_const_member | this=&amp;端口 | 端口完整 | 请求有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0058 | F0015 | F0043 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_normal | 请求.方法登记根稳定键, 状态服务 | 方法根候选 | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0194 | F0015 | F0120 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法根候选 | 端口绑定来自F0032；六轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0986 | F0015 | F0470 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0254；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1005 | F0015 | F0477 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0255；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1027 | F0015 | F0483 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0256；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1048 | F0015 | F0489 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0257；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1081 | F0015 | F0498 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0258；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1100 | F0015 | F0504 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0259；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1122 | F0015 | F0510 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0260；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1150 | F0015 | F0517 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0261；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1172 | F0015 | F0524 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0262；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1193 | F0015 | F0532 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0263；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1215 | F0015 | F0538 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0264；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1235 | F0015 | F0545 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0265；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1268 | F0015 | F0556 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 请求.方法登记根稳定键 | 方法登记根候选 | 端口绑定来自F0266；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0059 | F0015 | F0044 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_normal | 请求.根需求参数 | 启动候选 | 方法根有值 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0195 | F0015 | F0121 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动候选 | 端口绑定来自F0032且方法根有值；五轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0987 | F0015 | F0471 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0254；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1006 | F0015 | F0478 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0255；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1028 | F0015 | F0484 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0256；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1049 | F0015 | F0490 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0257；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1082 | F0015 | F0499 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0258；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1101 | F0015 | F0505 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0259；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1123 | F0015 | F0511 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0260；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1151 | F0015 | F0518 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0261；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1173 | F0015 | F0525 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0262；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1194 | F0015 | F0533 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0263；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1216 | F0015 | F0539 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0264；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1236 | F0015 | F0546 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0265；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1269 | F0015 | F0557 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 请求.根需求参数 | 启动结果 | 端口绑定来自F0266；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0060 | F0015 | F0045 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_normal | 10000ms | 已等待 | 首次启动成功 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0196 | F0015 | F0122 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 已等待 | 端口绑定来自F0032且启动成功；四轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0988 | F0015 | F0472 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0254；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1007 | F0015 | F0479 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0255；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1029 | F0015 | F0485 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0256；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1050 | F0015 | F0491 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0257；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1083 | F0015 | F0500 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0258；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1102 | F0015 | F0506 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0259；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1124 | F0015 | F0512 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0260；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1152 | F0015 | F0519 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0261；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1174 | F0015 | F0526 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0262；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1195 | F0015 | F0534 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0263；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1217 | F0015 | F0540 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0264；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1237 | F0015 | F0547 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0265；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1270 | F0015 | F0558 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 10000ms | 等待结果 | 端口绑定来自F0266；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0067 | F0016 | F0047 | <code>海中鱼巣/领域/初始化.系统.ixx:69</code> | direct_const_member | this=自我初始化快照 | 快照成功 | 状态已初始化 &amp;&amp; 方法根有值 &amp;&amp; 快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0068 | F0016 | F0049 | <code>海中鱼巣/领域/初始化.系统.ixx:70</code> | direct_const_member | this=&amp;概念图 | 概念图成功 | 前项全部为真 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0069 | F0018 | F0052 | <code>海中鱼巣/适配/审计.数据库启动.ixx:59</code> | direct_const_member | this=&amp;端口 | 端口完整 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0070 | F0018 | F0053 | <code>海中鱼巣/适配/审计.数据库启动.ixx:62</code> | callback_invoke_resolved_normal | 生产命名空间规格 | 命名空间候选 | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0206 | F0018 | F0126 | <code>海中鱼巣/适配/审计.数据库启动.ixx:62</code> | callback_invoke_resolved_test |  | 命名空间候选 | 端口来自F0032；写失败与不一致路径各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0071 | F0018 | F0054 | <code>海中鱼巣/适配/审计.数据库启动.ixx:72</code> | callback_invoke_resolved_normal | 命名空间, 三仓库 | 快照 | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0207 | F0018 | F0127 | <code>海中鱼巣/适配/审计.数据库启动.ixx:72</code> | callback_invoke_resolved_test | 命名空间 | 结构统计快照 | 端口来自F0032；两条路径各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0072 | F0018 | F0055 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_normal | 快照, 启动结构统计说明 | 写入 | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0208 | F0018 | F0128 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_test | 快照, 启动结构统计说明 | 默认写失败结果 | 写入失败端口路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0209 | F0018 | F0129 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_test | 快照, 启动结构统计说明 | 成功写入替身结果 | F0010覆盖端口来自F0032 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0073 | F0018 | F0056 | <code>海中鱼巣/适配/审计.数据库启动.ixx:78</code> | callback_invoke_resolved_normal | 1 | 读回 | 写入成功 &amp;&amp; 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0210 | F0018 | F0130 | <code>海中鱼巣/适配/审计.数据库启动.ixx:78</code> | callback_invoke_resolved_test | 1 | 单记录读回替身 | F0010覆盖端口来自F0032且写入成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0074 | F0019 | F0057 | <code>海中鱼巣/启动.程序运行宿主.ixx:116</code> | external_template_project_constructor | this=新分配存储 | 新租约 | 前置通过 &amp;&amp; make_unique分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0075 | F0019 | F0058 | <code>海中鱼巣/启动.程序运行宿主.ixx:118, 海中鱼巣/启动.程序运行宿主.ixx:123, 海中鱼巣/启动.程序运行宿主.ixx:129</code> | callback_register | SIGINT回调地址 | 注册槽 | 安装SIGINT；安装SIGTERM；Windows编译 &amp;&amp; 安装SIGBREAK | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0211 | F0019 | F0067 | <code>海中鱼巣/启动.程序运行宿主.ixx:118, 海中鱼巣/启动.程序运行宿主.ixx:123</code> | callback_invoke_resolved_test | SIGINT, 接收程序停止信号 | SIG_DFL | 安装函数实例来自F0032首次调用；安装函数实例来自F0032且SIGINT安装成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0076 | F0019 | F0025 | <code>海中鱼巣/启动.程序运行宿主.ixx:120, 海中鱼巣/启动.程序运行宿主.ixx:125, 海中鱼巣/启动.程序运行宿主.ixx:131, 海中鱼巣/启动.程序运行宿主.ixx:137-139</code> | compiler_implicit_destructor | this=租约 |  | SIGINT安装失败；SIGTERM安装失败；SIGBREAK安装失败；已构造租约后的异常展开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0082 | F0022 | F0016 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:262</code> | direct_const_member | this=&amp;初始化 | 初始化成功 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0083 | F0022 | F0059 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:267</code> | direct_const_member+unique_name | 快照.世界树.自我存在节点 | 坐标 | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0084 | F0022 | F0060 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:268-269</code> | direct_free_function | 上下文.节点, 上下文.关系, 快照.世界树, 快照.语素, 坐标 | 结果.世界树 | 初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0085 | F0022 | F0061 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:274</code> | direct_free_function | 快照.语素, 初始化.概念图 | 结果.语素树 | 世界树可展示且只读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0086 | F0024 | F0016 | <code>海中鱼巣/启动.程序运行宿主.ixx:148</code> | direct_const_member | this=&amp;初始化 | 初始化成功 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0087 | F0024 | F0023 | <code>海中鱼巣/启动.程序运行宿主.ixx:148</code> | direct_const_member_short_circuit | this=&amp;投影 | 投影成功 | 初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0088 | F0024 | F0062 | <code>海中鱼巣/启动.程序运行宿主.ixx:153-164</code> | direct_constructor | 上下文八项引用, 投影两树, 两个根需求句柄 | 控制面板 | 初始化和投影成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0089 | F0024 | F0063 | <code>海中鱼巣/启动.程序运行宿主.ixx:165</code> | direct_constructor | 控制面板, 上下文.数据库, 审计.快照 | 窗口 | after E0088 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0090 | F0024 | F0064 | <code>海中鱼巣/启动.程序运行宿主.ixx:166</code> | direct_member | this=&amp;窗口, &amp;信号.停止请求 | 窗口结果 | after E0089 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0091 | F0024 | F0065 | <code>海中鱼巣/启动.程序运行宿主.ixx:172</code> | compiler_implicit_destructor | this=&amp;窗口 |  | 函数返回前 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0095 | F0027 | F0069 | <code>海中鱼巣/启动.生产运行期.ixx:103, 海中鱼巣/启动.生产运行期.ixx:56</code> | direct_const_member | this=&amp;宿主_ | 当前租约 | 请求有效且已取得启动锁；发布状态被协议允许 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0096 | F0027 | F0066 | <code>海中鱼巣/启动.生产运行期.ixx:104, 海中鱼巣/启动.生产运行期.ixx:57</code> | direct_const_member | this=&amp;当前租约 | 租约有效 | after E0095；after E0106 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0108 | F0027 | F0079 | <code>海中鱼巣/启动.生产运行期.ixx:106</code> | direct_const_member | this=&amp;当前租约 | 已发布上下文只读指针 | 租约有效且发布状态为已发布 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0094 | F0027 | F0068 | <code>海中鱼巣/启动.生产运行期.ixx:51</code> | direct_const_member | this=&amp;请求 | 请求有效 | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0097 | F0027 | F0070 | <code>海中鱼巣/启动.生产运行期.ixx:66</code> | direct_free_function+unique_name |  | 配置 | 无现有有效租约；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0098 | F0027 | F0071 | <code>海中鱼巣/启动.生产运行期.ixx:67</code> | direct_const_member | this=&amp;配置 | 配置有效 | after E0097 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0099 | F0027 | F0072 | <code>海中鱼巣/启动.生产运行期.ixx:71</code> | external_template_project_constructor | 配置.域编号, 配置.仓库编号, 配置.动作键 | 候选共享对象 | 配置有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0100 | F0027 | F0073 | <code>海中鱼巣/启动.生产运行期.ixx:72</code> | direct_member | this=候选.get(), 配置.系统角色 | 系统角色结果 | 候选构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0101 | F0027 | F0074 | <code>海中鱼巣/启动.生产运行期.ixx:73</code> | direct_const_member | this=&amp;系统角色结果 | 系统角色成功 | after E0100 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0102 | F0027 | F0075 | <code>海中鱼巣/启动.生产运行期.ixx:79</code> | direct_member | this=候选.get(), 配置.概念活动 | 概念活动结果 | 系统角色初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0103 | F0027 | F0076 | <code>海中鱼巣/启动.生产运行期.ixx:80</code> | direct_const_member | this=&amp;概念活动结果 | 概念活动成功 | after E0102 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0104 | F0027 | F0077 | <code>海中鱼巣/启动.生产运行期.ixx:86</code> | direct_const_member | this=候选.get() | 候选完整 | 概念活动初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0105 | F0027 | F0078 | <code>海中鱼巣/启动.生产运行期.ixx:93</code> | direct_member+unique_name | this=&amp;宿主_, 候选共享所有权副本 | 发布状态 | 候选完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0092 | F0028 | F0066 | <code>海中鱼巣/启动.生产运行期.ixx:40</code> | direct_const_member | this=&amp;租约 | 租约有效结果 | 状态为已启动或已有当前上下文 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0109 | F0029 | F0030 | <code>海中鱼巣/自检.入口初始化.ixx:658-659, 海中鱼巣/自检.入口初始化.ixx:660-661, 海中鱼巣/自检.入口初始化.ixx:662-663, 海中鱼巣/自检.入口初始化.ixx:664-665, 海中鱼巣/自检.入口初始化.ixx:666-667, 海中鱼巣/自检.入口初始化.ixx:668-669, 海中鱼巣/自检.入口初始化.ixx:670-671, 海中鱼巣/自检.入口初始化.ixx:672-673, 海中鱼巣/自检.入口初始化.ixx:674-675, 海中鱼巣/自检.入口初始化.ixx:676-677, 海中鱼巣/自检.入口初始化.ixx:678-679, 海中鱼巣/自检.入口初始化.ixx:680-681, 海中鱼巣/自检.入口初始化.ixx:682-683, 海中鱼巣/自检.入口初始化.ixx:684-685, 海中鱼巣/自检.入口初始化.ixx:686-687</code> | direct_member | S01-S15顺序、编号、名称与对应回调 | S01登记结果 | 前序登记均成功时继续；S15要求S01-S14均成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0123 | F0029 | F0080 | <code>海中鱼巣/自检.入口初始化.ixx:659</code> | callback_bind | 配置按值捕获 | S01回调槽 | 构造S01登记参数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0124 | F0029 | F0081 | <code>海中鱼巣/自检.入口初始化.ixx:661</code> | callback_bind | 配置按值捕获 | S02回调槽 | S01成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0125 | F0029 | F0082 | <code>海中鱼巣/自检.入口初始化.ixx:663</code> | callback_bind | 配置按值捕获 | S03回调槽 | S01-S02成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0126 | F0029 | F0083 | <code>海中鱼巣/自检.入口初始化.ixx:665</code> | callback_bind | 配置按值捕获 | S04回调槽 | S01-S03成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0127 | F0029 | F0084 | <code>海中鱼巣/自检.入口初始化.ixx:667</code> | callback_bind | 配置按值捕获 | S05回调槽 | S01-S04成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0128 | F0029 | F0085 | <code>海中鱼巣/自检.入口初始化.ixx:669</code> | callback_bind | 配置按值捕获 | S06回调槽 | S01-S05成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0129 | F0029 | F0086 | <code>海中鱼巣/自检.入口初始化.ixx:671</code> | callback_bind | 配置按值捕获 | S07回调槽 | S01-S06成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0130 | F0029 | F0087 | <code>海中鱼巣/自检.入口初始化.ixx:673</code> | callback_bind | 配置按值捕获 | S08回调槽 | S01-S07成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0131 | F0029 | F0088 | <code>海中鱼巣/自检.入口初始化.ixx:675</code> | callback_bind | 配置按值捕获 | S09回调槽 | S01-S08成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0132 | F0029 | F0089 | <code>海中鱼巣/自检.入口初始化.ixx:677</code> | callback_bind | 配置按值捕获 | S10回调槽 | S01-S09成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0133 | F0029 | F0090 | <code>海中鱼巣/自检.入口初始化.ixx:679</code> | callback_bind | 配置按值捕获 | S11回调槽 | S01-S10成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0134 | F0029 | F0091 | <code>海中鱼巣/自检.入口初始化.ixx:681</code> | callback_bind | 配置按值捕获 | S12回调槽 | S01-S11成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0135 | F0029 | F0092 | <code>海中鱼巣/自检.入口初始化.ixx:683</code> | callback_bind | 配置按值捕获 | S13回调槽 | S01-S12成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0136 | F0029 | F0093 | <code>海中鱼巣/自检.入口初始化.ixx:685</code> | callback_bind | 配置按值捕获 | S14回调槽 | S01-S13成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0137 | F0030 | F0094 | <code>海中鱼巣/自检.运行器.ixx:50-52</code> | standard_algorithm_callback | 当前登记项, 捕获顺序和编号引用 | find_if命中判定 | 入口参数合法且遍历尚未命中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0038 | F0031 | F0032 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | 登记.回调() | 单元 | 执行到顺序150 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0039 | F0031 | F0033 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | 登记.回调() | 单元 | 执行到顺序160 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0138 | F0031 | F0080 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S01登记回调 | 单元 | 执行到S01 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0139 | F0031 | F0081 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S02登记回调 | 单元 | 执行到S02 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0140 | F0031 | F0082 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S03登记回调 | 单元 | 执行到S03 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0141 | F0031 | F0083 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S04登记回调 | 单元 | 执行到S04 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0142 | F0031 | F0084 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S05登记回调 | 单元 | 执行到S05 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0143 | F0031 | F0085 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S06登记回调 | 单元 | 执行到S06 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0144 | F0031 | F0086 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S07登记回调 | 单元 | 执行到S07 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0145 | F0031 | F0087 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S08登记回调 | 单元 | 执行到S08 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0146 | F0031 | F0088 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S09登记回调 | 单元 | 执行到S09 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0147 | F0031 | F0089 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S10登记回调 | 单元 | 执行到S10 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0148 | F0031 | F0090 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S11登记回调 | 单元 | 执行到S11 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0149 | F0031 | F0091 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S12登记回调 | 单元 | 执行到S12 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0150 | F0031 | F0092 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S13登记回调 | 单元 | 执行到S13 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0151 | F0031 | F0093 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | S14登记回调 | 单元 | 执行到S14 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0182 | F0032 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:197</code> | direct_free_function+unique_name | 配置 | 装配 | 六轮各一次；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0183 | F0032 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:198</code> | direct_const_member | this=&amp;装配 | 装配成功 | 每轮装配后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0026 | F0032 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:221</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0027 | F0032 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:227</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0028 | F0032 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:233</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0184 | F0032 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:242, 海中鱼巣/启动.应用程序.ixx:242-243</code> | direct_free_function+unique_name | 故障注入端口, {配置.方法登记根稳定键,配置.根需求参数} | 结果 | 装配成功；六轮；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0185 | F0032 | F0131 | <code>海中鱼巣/启动.应用程序.ixx:246</code> | compiler_implicit_member_destructor | this=&amp;上下文.自我线程实例 |  | 每轮上下文作用域退出；成员已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0186 | F0032 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:260-261, 海中鱼巣/启动.应用程序.ixx:261</code> | direct_free_function+unique_name | 写入失败端口, 数据库审计要求::最佳努力 | 写入失败 | 六轮结束；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0187 | F0032 | F0010 | <code>海中鱼巣/启动.应用程序.ixx:280</code> | direct_free_function+lsp_direct | &amp;不一致端口 | 专项 | 写入失败状态验收后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0188 | F0032 | F0019 | <code>海中鱼巣/启动.应用程序.ixx:293, 海中鱼巣/启动.应用程序.ixx:294</code> | direct_free_function+unique_name | F0067信号安装替身 | 信号失败 | 数据库验收后；after E0188；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0190 | F0032 | F0020 | <code>海中鱼巣/启动.应用程序.ixx:296, 海中鱼巣/启动.应用程序.ixx:301</code> | direct_const_member | this=&amp;信号失败 | 成功谓词并取反 | 状态为SIGTERM失败；前置短路项全部为真 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0193 | F0032 | F0025 | <code>海中鱼巣/启动.应用程序.ixx:320</code> | compiler_implicit_destructor | this=后续可安装.租约.get() |  | 根函数退出且租约存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0213 | F0033 | F0132 | <code>海中鱼巣/启动.应用程序.ixx:327-333</code> | callback_bind | 捕获第一编号组,第一地址全非零,第一初始为零 | 第一配置.观察回调 | 构造第一配置 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0214 | F0033 | F0029 | <code>海中鱼巣/启动.应用程序.ixx:335, 海中鱼巣/启动.应用程序.ixx:353</code> | direct_free_function+unique_name | 第一运行器,第一配置 | 第一登记 | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0215 | F0033 | F0031 | <code>海中鱼巣/启动.应用程序.ixx:336, 海中鱼巣/启动.应用程序.ixx:354</code> | direct_member+unique_name | this=&amp;第一运行器 | 第一批次 | 第一登记结果无论真假；第二登记结果无论真假；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0216 | F0033 | F0133 | <code>海中鱼巣/启动.应用程序.ixx:350-351</code> | callback_bind | 捕获第二观察数量 | 第二配置.观察回调 | 构造第二配置 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0153 | F0037 | F0096 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:877-880</code> | lambda_direct |  | 无效参数写前拒绝 | try开始后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0154 | F0037 | F0097 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:885</code> | direct_free_function | 规模 | 规模报告 | 对1000/10000/100000各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0179 | F0037 | F0118 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:893-895</code> | standard_algorithm_callback | 当前规模报告 | 随机种子与结构签名判定 | 验收1前置短路均通过且all_of遍历 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0155 | F0037 | F0098 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:902-911</code> | standard_algorithm_callback | 当前规模报告 | 参考模型及查询、写入、并发结果一致判定 | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0180 | F0037 | F0119 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:912-915</code> | standard_algorithm_callback | 当前规模报告 | 指标数量与并发吞吐判定 | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0156 | F0037 | F0099 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:922</code> | trivial_adapter_direct | 当前规模报告 | 报告完整判定 | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0220 | F0039 | F0134 | <code>海中鱼巣/装配.普通应用.ixx:48</code> | direct_const_member | this=&amp;根需求参数 | 配置有效短路右项 | 方法登记根稳定键!=0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0221 | F0040 | F0135 | <code>海中鱼巣/装配.普通应用.ixx:54-55</code> | member_constructor | 1, 默认结构事务接线 | 主信息 | 构造开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0222 | F0040 | F0136 | <code>海中鱼巣/装配.普通应用.ixx:55</code> | member_constructor | 主信息,1,默认接线 | 节点 | E0221成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0223 | F0040 | F0137 | <code>海中鱼巣/装配.普通应用.ixx:56</code> | member_constructor | 节点,1,默认接线 | 关系 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0224 | F0040 | F0138 | <code>海中鱼巣/装配.普通应用.ixx:57</code> | member_constructor | 节点,默认接线 | 索引 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0225 | F0040 | F0139 | <code>海中鱼巣/装配.普通应用.ixx:58</code> | member_constructor | 主信息,节点,关系 | 存在 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0226 | F0040 | F0140 | <code>海中鱼巣/装配.普通应用.ixx:59</code> | member_constructor | 主信息,节点,关系 | 场景 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0227 | F0040 | F0141 | <code>海中鱼巣/装配.普通应用.ixx:60</code> | member_constructor | 主信息,节点,关系 | 状态 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0228 | F0040 | F0142 | <code>海中鱼巣/装配.普通应用.ixx:61</code> | member_constructor | 主信息,节点,关系 | 动态 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0229 | F0040 | F0143 | <code>海中鱼巣/装配.普通应用.ixx:62</code> | member_constructor | 主信息,节点,关系 | 二次特征 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0230 | F0040 | F0144 | <code>海中鱼巣/装配.普通应用.ixx:63</code> | member_constructor | 主信息,节点,关系,索引 | 语素 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0231 | F0040 | F0145 | <code>海中鱼巣/装配.普通应用.ixx:64</code> | member_constructor | 主信息,节点,默认接线 | 特征值 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0232 | F0040 | F0146 | <code>海中鱼巣/装配.普通应用.ixx:65</code> | member_constructor | 主信息,节点 | 因果 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0233 | F0040 | F0147 | <code>海中鱼巣/装配.普通应用.ixx:66</code> | member_constructor | 节点,关系 | 概念图 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0234 | F0040 | F0148 | <code>海中鱼巣/装配.普通应用.ixx:67</code> | member_constructor | 概念图,存在,动态,二次特征,因果,状态,语素 | 概念图初始化 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0235 | F0040 | F0149 | <code>海中鱼巣/装配.普通应用.ixx:68</code> | member_constructor | 主信息,节点,关系,二次特征,特征值 | 特征 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0236 | F0040 | F0150 | <code>海中鱼巣/装配.普通应用.ixx:69</code> | member_constructor | 主信息,节点,存在,场景 | 世界 | 前序成功 | 世界服务当前源码与装配调用点复核 |
| E0237 | F0040 | F0151 | <code>海中鱼巣/装配.普通应用.ixx:70</code> | member_constructor | 主信息,节点,关系,世界,场景,存在 | 世界树初始化 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0238 | F0040 | F0152 | <code>海中鱼巣/装配.普通应用.ixx:71</code> | member_constructor | 主信息,节点,关系,世界,语素 | 语素初始化 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0239 | F0040 | F0153 | <code>海中鱼巣/装配.普通应用.ixx:72</code> | member_constructor | 主信息,节点,关系 | 需求 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0240 | F0040 | F0154 | <code>海中鱼巣/装配.普通应用.ixx:73</code> | member_constructor | 特征,语素,需求,状态 | 需求初始化 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0241 | F0040 | F0155 | <code>海中鱼巣/装配.普通应用.ixx:74</code> | member_constructor | 语素初始化,世界树初始化,需求初始化 | 自我线程实例 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0242 | F0040 | F0156 | <code>海中鱼巣/装配.普通应用.ixx:75</code> | member_constructor | 主信息,节点,关系 | 任务 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0243 | F0040 | F0157 | <code>海中鱼巣/装配.普通应用.ixx:76</code> | member_constructor | 主信息,节点,关系 | 方法 | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0244 | F0040 | F0158 | <code>海中鱼巣/装配.普通应用.ixx:77</code> | static_member_direct |  | SQL数据库配置临时值 | 统计和显示已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0245 | F0040 | F0159 | <code>海中鱼巣/装配.普通应用.ixx:77</code> | member_constructor | 移动F0158返回配置 | 数据库 | E0244成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0246 | F0041 | F0134 | <code>海中鱼巣/领域/初始化.系统.ixx:24</code> | direct_const_member | this=&amp;根需求参数 | 请求有效短路右项 | 方法登记根稳定键!=0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0247 | F0043 | F0160 | <code>海中鱼巣/领域/方法服务.h:263, 海中鱼巣/领域/方法服务.h:295</code> | direct_const_member+unique_name | this, 状态 | 既有登记根完整性 | 已有材料且稳定键相同；私有材料与仓库编号已发布；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0248 | F0043 | F0161 | <code>海中鱼巣/领域/方法服务.h:264, 海中鱼巣/领域/方法服务.h:273, 海中鱼巣/领域/方法服务.h:280, 海中鱼巣/领域/方法服务.h:289, 海中鱼巣/领域/方法服务.h:298</code> | direct_member_template_instance | 锁, 重复初始化异常说明 | void | 既有同键登记根不完整；登记根无效或角色绑定失败；状态无效或相同；关系无效或仓库编号不同；发布后完整性失败且私有发布已撤回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0249 | F0043 | F0162 | <code>海中鱼巣/领域/方法服务.h:270</code> | direct_member+unique_name | this | 登记根 | 无既有材料；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0250 | F0043 | F0163 | <code>海中鱼巣/领域/方法服务.h:271, 海中鱼巣/领域/方法服务.h:278</code> | direct_free | 登记根 | 登记根有效性 | 登记方法返回；两个状态调用均返回；活跃状态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0261 | F0043 | F0168 | <code>海中鱼巣/领域/方法服务.h:271, 海中鱼巣/领域/方法服务.h:278, 海中鱼巣/领域/方法服务.h:287</code> | direct_free+unique_name | 活跃关系 | 活跃关系有效性 | 两条关系调用均返回；活跃关系有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0251 | F0043 | F0164 | <code>海中鱼巣/领域/方法服务.h:272</code> | direct_member+unique_name | 登记根, 方法角色状态::方法登记根, 状态 | 角色绑定结果 | 登记根有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0253 | F0043 | F0165 | <code>海中鱼巣/领域/方法服务.h:276, 海中鱼巣/领域/方法服务.h:277</code> | direct_member | static_cast&lt;std::int64_t&gt;(方法生命周期状态::活跃) | 活跃状态 | 登记根创建完整；活跃状态调用返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0257 | F0043 | F0051 | <code>海中鱼巣/领域/方法服务.h:279</code> | direct_free_operator | 活跃状态, 失效状态 | 状态句柄相等性 | 两个状态均有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0259 | F0043 | F0167 | <code>海中鱼巣/领域/方法服务.h:283-284, 海中鱼巣/领域/方法服务.h:285-286</code> | direct_member | 关系类型::模板, 登记根, 活跃状态, 40 | 活跃关系 | 两个状态有效且不同；活跃关系调用返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0266 | F0044 | F0046 | <code>海中鱼巣/线程/自我线程.ixx:183</code> | direct_const_member+unique_name | this | 已有快照 | 进入启动；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0267 | F0044 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:184</code> | direct_const_member | this=&amp;*已有快照 | 已有快照成功性 | 已有快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0268 | F0044 | F0169 | <code>海中鱼巣/线程/自我线程.ixx:188, 海中鱼巣/线程/自我线程.ixx:191, 海中鱼巣/线程/自我线程.ixx:196, 海中鱼巣/线程/自我线程.ixx:210</code> | direct_static_member+suffix_match | 自我线程拒绝原因::重复启动 | 拒绝结果 | 状态非未启动或线程可汇合；参数无效；邮箱与处理器接入状态不一致；捕获std::system_error；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0269 | F0044 | F0134 | <code>海中鱼巣/线程/自我线程.ixx:190</code> | direct_const_member | this=&amp;参数 | 参数有效性 | 非重复启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0272 | F0044 | F0170 | <code>海中鱼巣/线程/自我线程.ixx:201</code> | direct_member+unique_name | this | void | 全部准入通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0273 | F0044 | F0171 | <code>海中鱼巣/线程/自我线程.ixx:204-206</code> | thread_callback | 捕获this与参数副本 | void异步回调 | std::thread构造成功后异步调度 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0275 | F0045 | F0172 | <code>海中鱼巣/线程/自我线程.ixx:217-224</code> | condition_variable_predicate | 捕获this | wait_for终止谓词 | wait_for初检或唤醒后，可重复 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0276 | F0045 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:227</code> | direct_const_member | this=&amp;*初始化快照 | 初始化快照成功性 | 等待结束且快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0277 | F0047 | F0173 | <code>海中鱼巣/线程/自我线程.ixx:57</code> | direct_const_member | this=&amp;语素 | 语素初始化成功性 | 进入快照成功检查 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0278 | F0047 | F0174 | <code>海中鱼巣/线程/自我线程.ixx:58</code> | direct_const_member | this=&amp;世界树 | 世界树初始化成功性 | 语素初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0279 | F0047 | F0175 | <code>海中鱼巣/线程/自我线程.ixx:59</code> | direct_const_member | this=&amp;根需求 | 根需求初始化成功性 | 语素和世界树初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0280 | F0048 | F0049 | <code>海中鱼巣/领域/初始化.概念图.ixx:100, 海中鱼巣/领域/初始化.概念图.ixx:135</code> | direct_const_member | this=&amp;已发布结果_ | 已发布结果完整性 | 取得初始化锁；四根名称均成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0281 | F0048 | F0176 | <code>海中鱼巣/领域/初始化.概念图.ixx:101, 海中鱼巣/领域/初始化.概念图.ixx:135</code> | direct_const_member+unique_name | 已发布结果_ | 已发布结构可读性 | 已发布结果成功；候选成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1633 | F0048 | F0177 | <code>海中鱼巣/领域/初始化.概念图.ixx:104-105</code> | direct_member_template_instance |  |  | 初始化四个根按源码顺序调用 F0177；对应 lambda@105 | 专项源码静态类型与实际装配人工复核 |
| RCE1634 | F0048 | F0178 | <code>海中鱼巣/领域/初始化.概念图.ixx:106-107</code> | direct_member_template_instance |  |  | 初始化四个根按源码顺序调用 F0178；对应 lambda@107 | 专项源码静态类型与实际装配人工复核 |
| RCE1635 | F0048 | F0179 | <code>海中鱼巣/领域/初始化.概念图.ixx:108-109</code> | direct_member_template_instance |  |  | 初始化四个根按源码顺序调用 F0179；对应 lambda@109 | 专项源码静态类型与实际装配人工复核 |
| E0282 | F0048 | F0180 | <code>海中鱼巣/领域/初始化.概念图.ixx:110-111</code> | direct_member_template_instance |  |  | 初始化四个根按源码顺序调用 F0180；对应 lambda@111 | 专项源码静态类型与实际装配人工复核 |
| E0290 | F0048 | F0181 | <code>海中鱼巣/领域/初始化.概念图.ixx:115, 海中鱼巣/领域/初始化.概念图.ixx:115-117, 海中鱼巣/领域/初始化.概念图.ixx:118, 海中鱼巣/领域/初始化.概念图.ixx:118-120, 海中鱼巣/领域/初始化.概念图.ixx:121, 海中鱼巣/领域/初始化.概念图.ixx:121-123</code> | direct_member+unique_name | 活跃状态槽,概念生命周期阶段::活跃 | 活跃状态确保结果 | 四根均确保成功；活跃状态确保成功；冷却状态确保成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0293 | F0048 | F0182 | <code>海中鱼巣/领域/初始化.概念图.ixx:124</code> | direct_member+unique_name | this=&amp;概念图_ | 活动生命周期初始化结果 | 三个状态均确保成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0294 | F0048 | F0183 | <code>海中鱼巣/领域/初始化.概念图.ixx:128, 海中鱼巣/领域/初始化.概念图.ixx:129, 海中鱼巣/领域/初始化.概念图.ixx:130, 海中鱼巣/领域/初始化.概念图.ixx:131</code> | direct_member+unique_name | 存在根项,L存在 | 存在根名称结果 | 生命周期初始化成功；存在根名称成功；动态根名称成功；关系根名称成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0300 | F0048 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:135, 海中鱼巣/领域/初始化.概念图.ixx:135-136</code> | direct_free+unique_name | 候选成功且结构仍可读,具名说明 | 最终内部检查 | 名称阶段成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0301 | F0049 | F0189 | <code>海中鱼巣/领域/初始化.概念图.ixx:50, 海中鱼巣/领域/初始化.概念图.ixx:51, 海中鱼巣/领域/初始化.概念图.ixx:52, 海中鱼巣/领域/初始化.概念图.ixx:53</code> | direct_const_member | this=&amp;存在根 | 存在根项成功性 | 进入结果检查；存在根项成功；存在和动态根项成功；前三根项成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0305 | F0049 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:54, 海中鱼巣/领域/初始化.概念图.ixx:55, 海中鱼巣/领域/初始化.概念图.ixx:56</code> | direct_free | 活跃状态 | 活跃状态有效性 | 四根项成功；活跃状态有效；前两个状态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0033 | F0049 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:54, 海中鱼巣/领域/初始化.概念图.ixx:55, 海中鱼巣/领域/初始化.概念图.ixx:56</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0308 | F0049 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:57, 海中鱼巣/领域/初始化.概念图.ixx:58, 海中鱼巣/领域/初始化.概念图.ixx:59</code> | rewritten_not_equal | 活跃状态,冷却状态 | 相等性并对!=取反 | 三个状态有效；活跃与冷却不同；活跃分别与另两状态不同 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0311 | F0050 | F0190 | <code>海中鱼巣/领域/概念图服务.h:1053</code> | direct_const_member | 存在实例 | 实例记录 | 进入函数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0312 | F0050 | F0191 | <code>海中鱼巣/领域/概念图服务.h:1058</code> | direct_member+unique_name | 存在实例,概念根类别::存在 | 存在根支持确保结果 | 节点记录存在且类型为存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0313 | F0053 | F0192 | <code>海中鱼巣/领域/统计服务.h:245</code> | direct_const_member+unique_name | 类型 | 缓存类型有效性 | 进入准入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0314 | F0053 | F0193 | <code>海中鱼巣/领域/统计服务.h:246</code> | direct_const_member+unique_name | 来源 | 来源结构类型有效性 | 缓存类型有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0315 | F0053 | F0194 | <code>海中鱼巣/领域/统计服务.h:249</code> | direct_const_member+unique_name | 值类型 | 缓存值类型有效性 | 来源和两个版本有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0316 | F0053 | F0195 | <code>海中鱼巣/领域/统计服务.h:250</code> | direct_const_member+unique_name | 失效条件 | 缓存失效条件有效性 | 前五项有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0320 | F0054 | F0196 | <code>海中鱼巣/领域/统计服务.h:692</code> | direct_const_member+unique_name | 命名空间,节点数,关系数,索引数 | 结构统计快照 | 全部实参求值完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0317 | F0054 | F0197 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | this=&amp;节点 | 节点数 | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0318 | F0054 | F0198 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | this=&amp;关系 | 关系数 | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0319 | F0054 | F0199 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | this=&amp;索引 | 索引数 | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0321 | F0055 | F0200 | <code>海中鱼巣/适配/SQL数据库适配.cpp:382</code> | direct_free+direct-free | 快照 | 写入准入短路条件 | 总准入首项；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0322 | F0055 | F0201 | <code>海中鱼巣/适配/SQL数据库适配.cpp:383</code> | direct_free+direct-free+direct-free+direct-free | 快照.节点数 | 写入准入短路条件 | 前置准入均通过；节点数可写入；关系数可写入；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0325 | F0055 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:384, 海中鱼巣/适配/SQL数据库适配.cpp:394</code> | direct_free+direct-free+direct-free | L"结构统计审计写入准入",L"快照命名空间、来源入口或结构数量不符合写入约束。" | 函数返回 | 任一写入准入失败；连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0326 | F0055 | F0203 | <code>海中鱼巣/适配/SQL数据库适配.cpp:386</code> | direct_const_member+unique_name+direct-member | this | 初始化结果 | 写入准入通过；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0369 | F0055 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:392-469</code> | implicit_destructor | 连接 | 关闭连接并释放环境 | 连接构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0327 | F0055 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:392</code> | direct_constructor+constructor | 无 | 连接局部对象 | 初始化成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0328 | F0055 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:393</code> | direct_member+direct-member | 配置_,配置_.数据库,诊断 | 连接打开结果 | 连接对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0368 | F0055 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396-469</code> | implicit_destructor | 语句 | 释放语句句柄 | 语句构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0330 | F0055 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396</code> | direct_const_member+direct-member | 连接 | ODBC语句构造实参 | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0331 | F0055 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396</code> | direct_constructor+constructor | 连接.句柄() | 语句局部对象 | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0332 | F0055 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:397</code> | direct_const_member+direct-member | 语句 | 语句可用性 | 语句对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0333 | F0055 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:398, 海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:453, 海中鱼巣/适配/SQL数据库适配.cpp:457, 海中鱼巣/适配/SQL数据库适配.cpp:466</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | L"分配语句",诊断 | 函数返回 | 语句不可用；准备失败；绑定失败；执行失败；结果集定位失败；Fetch失败；整数列读取失败或编号为0；最近记录与本次写入不一致；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0334 | F0055 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:407, 海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:421, 海中鱼巣/适配/SQL数据库适配.cpp:426, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:440, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:445, 海中鱼巣/适配/SQL数据库适配.cpp:446, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:451, 海中鱼巣/适配/SQL数据库适配.cpp:453, 海中鱼巣/适配/SQL数据库适配.cpp:456</code> | direct_const_member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member | 语句 | SQLPrepareW句柄实参 | 准备INSERT批次；准备失败；准备成功；绑定仍成功且索引小于5；绑定失败；全部参数绑定成功；执行失败；定位审计编号结果集；列数读取成功且列数为0；更多结果失败或无更多结果；已定位含列结果集；Fetch失败；Fetch成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0335 | F0055 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:407, 海中鱼巣/适配/SQL数据库适配.cpp:424, 海中鱼巣/适配/SQL数据库适配.cpp:437, 海中鱼巣/适配/SQL数据库适配.cpp:440, 海中鱼巣/适配/SQL数据库适配.cpp:445, 海中鱼巣/适配/SQL数据库适配.cpp:447, 海中鱼巣/适配/SQL数据库适配.cpp:452</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | SQLPrepareW返回码 | 准备成功判断 | SQLPrepareW已返回；首次来源参数已绑定或上一数值参数已绑定；数值参数循环结束；SQLExecute已返回；SQLNumResultCols已返回；更多结果不是SQL_NO_DATA；SQLFetch已返回；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0337 | F0055 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:453</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free | SQL_HANDLE_STMT,语句.句柄() | 准备失败诊断 | 准备失败；绑定失败；执行失败；更多结果失败或无更多结果；Fetch失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0364 | F0055 | F0213 | <code>海中鱼巣/适配/SQL数据库适配.cpp:456</code> | direct_free+direct-free | 语句.句柄(),1,审计编号 | 审计编号读取成功判断 | Fetch成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0366 | F0055 | F0056 | <code>海中鱼巣/适配/SQL数据库适配.cpp:460</code> | direct_const_member+suffix_match+direct-member | 1 | 读回 | 取得非零审计编号；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0370 | F0056 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:474, 海中鱼巣/适配/SQL数据库适配.cpp:486</code> | direct_free+direct-free+direct-free | L"审计查询准入",拒绝说明 | 返回结果.操作 | 数量上限不在1到100；连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0371 | F0056 | F0203 | <code>海中鱼巣/适配/SQL数据库适配.cpp:477</code> | direct_const_member+unique_name+direct-member | this | 初始化结果 | 查询准入通过；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0407 | F0056 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:484-531</code> | implicit_destructor | 连接 | 关闭连接并释放环境 | 连接构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0372 | F0056 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:484</code> | direct_constructor+constructor | 无 | 连接局部对象 | 初始化成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0373 | F0056 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:485</code> | direct_member+direct-member | 配置_,配置_.数据库,诊断 | 连接打开结果 | 连接对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0406 | F0056 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489-531</code> | implicit_destructor | 语句 | 释放语句句柄 | 语句构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0375 | F0056 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489</code> | direct_const_member+direct-member | 连接 | ODBC语句构造实参 | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0376 | F0056 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489</code> | direct_constructor+constructor | 连接.句柄() | 语句局部对象 | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0377 | F0056 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:490</code> | direct_const_member+direct-member | 语句 | 语句可用性 | 语句对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0378 | F0056 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:491, 海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:510, 海中鱼巣/适配/SQL数据库适配.cpp:523</code> | direct_free+direct-free+direct-free+direct-free+direct-free | L"分配审计查询语句",说明 | 返回结果.操作 | 语句不可用；查询执行失败；Fetch失败；任一列转换失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0379 | F0056 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:499, 海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:505, 海中鱼巣/适配/SQL数据库适配.cpp:510, 海中鱼巣/适配/SQL数据库适配.cpp:515, 海中鱼巣/适配/SQL数据库适配.cpp:516, 海中鱼巣/适配/SQL数据库适配.cpp:517, 海中鱼巣/适配/SQL数据库适配.cpp:518, 海中鱼巣/适配/SQL数据库适配.cpp:519, 海中鱼巣/适配/SQL数据库适配.cpp:520, 海中鱼巣/适配/SQL数据库适配.cpp:521, 海中鱼巣/适配/SQL数据库适配.cpp:522</code> | direct_const_member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member | 语句 | SQLExecDirectW句柄实参 | 查询文本已构造；查询执行失败；每轮结果获取；Fetch失败；Fetch成功；第一列转换成功；前两列转换成功；前三列转换成功；前四列转换成功；前五列转换成功；前六列转换成功；前七列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0380 | F0056 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:499, 海中鱼巣/适配/SQL数据库适配.cpp:509</code> | direct_free+direct-free+direct-free | SQLExecDirectW返回码 | 执行成功判断 | 查询已执行；获取结果不是SQL_NO_DATA；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0382 | F0056 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:510</code> | direct_free+direct-free+direct-free | SQL_HANDLE_STMT,语句.句柄() | 执行失败诊断 | 查询执行失败；Fetch失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0390 | F0056 | F0213 | <code>海中鱼巣/适配/SQL数据库适配.cpp:515, 海中鱼巣/适配/SQL数据库适配.cpp:518, 海中鱼巣/适配/SQL数据库适配.cpp:519, 海中鱼巣/适配/SQL数据库适配.cpp:520, 海中鱼巣/适配/SQL数据库适配.cpp:521, 海中鱼巣/适配/SQL数据库适配.cpp:522</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | 句柄,1,记录.审计编号 | 列转换短路条件 | Fetch成功；前三列转换成功；前四列转换成功；前五列转换成功；前六列转换成功；前七列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0392 | F0056 | F0216 | <code>海中鱼巣/适配/SQL数据库适配.cpp:516, 海中鱼巣/适配/SQL数据库适配.cpp:517</code> | direct_free+direct-free+direct-free | 句柄,2,记录.记录时间 | 列转换短路条件 | 第一列转换成功；前两列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0408 | F0059 | F0190 | <code>海中鱼巣/领域/初始化.世界树.ixx:89</code> | direct_const_member | 自我存在节点 | 记录 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0409 | F0059 | F0217 | <code>海中鱼巣/领域/初始化.世界树.ixx:93, 海中鱼巣/领域/初始化.世界树.ixx:94, 海中鱼巣/领域/初始化.世界树.ixx:95</code> | direct_const_member | 记录-&gt;主信息,0 | 横向 | 记录存在且节点类型为存在；横向读取完成；纵向读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0413 | F0060 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:135</code> | direct_const_member | this=&amp;语素初始化读数 | 语素初始化前置 | 世界树初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0412 | F0060 | F0174 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:135</code> | direct_const_member | this=&amp;初始化结果 | 世界树初始化前置 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0414 | F0060 | F0218 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:139, 海中鱼巣/界面/投影.控制面板启动.ixx:139-141</code> | direct_free+lsp_direct | 节点,关系,世界树根节点,初始化结果,语素初始化读数,自我相对坐标,当前父链 | 根材料 | 两项初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0416 | F0061 | F0049 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:192</code> | direct_const_member | this=&amp;概念图初始化读数 | 概念图初始化前置 | 语素初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0415 | F0061 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:192</code> | direct_const_member | this=&amp;语素初始化读数 | 语素初始化前置 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0417 | F0061 | F0219 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:196, 海中鱼巣/界面/投影.控制面板启动.ixx:214</code> | direct_free+lsp_direct | 语素类型,语素类型 | 根材料 | 初始化前置通过；当前语素子项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0418 | F0061 | F0220 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:210</code> | direct_const_member | this=*子项 | 语素子项循环准入 | 当前子项指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0420 | F0061 | F0189 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:221</code> | direct_const_member | this=*概念 | 概念循环准入 | 当前概念指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0421 | F0061 | F0221 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:225</code> | direct_free+lsp_direct | *概念,语素类型 | 概念名称节点追加实参 | 当前概念初始化项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0422 | F0063 | F0222 | <code>海中鱼巣/界面/控制面板窗口.cpp:1701-1702</code> | constructor_via_make_unique | 控制面板,数据库,启动结构统计副本 | make_unique拥有的窗口实现 | PImpl存储分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0423 | F0064 | F0223 | <code>海中鱼巣/界面/控制面板窗口.cpp:1708</code> | direct_private_member+direct-member | 停止请求 | 函数返回值 | 构造后实现指针有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0424 | F0065 | F0224 | <code>海中鱼巣/界面/控制面板窗口.cpp:1705</code> | implicit_delete_destructor | *实现 | 被拥有PImpl析构 | unique_ptr非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0425 | F0066 | F0077 | <code>海中鱼巣/启动.运行期上下文.ixx:195</code> | direct_const_member | this=上下文_.get() | 函数返回值 | shared_ptr非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0426 | F0069 | F0226 | <code>海中鱼巣/启动.运行期上下文.ixx:235</code> | private_constructor | 当前上下文_的const共享副本 | 返回租约 | 发布锁已持有 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0034 | F0071 | F0450 | <code>海中鱼巣/生产运行期配置.数据.h:77, 海中鱼巣/生产运行期配置.数据.h:79</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0035 | F0071 | F0452 | <code>海中鱼巣/生产运行期配置.数据.h:77, 海中鱼巣/生产运行期配置.数据.h:79</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0427 | F0071 | F0070 | <code>海中鱼巣/生产运行期配置.数据.h:78</code> | direct_free | 无 | 固定配置 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0428 | F0071 | F0227 | <code>海中鱼巣/生产运行期配置.数据.h:78</code> | direct_const_member | 固定配置 | 首项等值条件 | 固定配置已形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0429 | F0071 | F0228 | <code>海中鱼巣/生产运行期配置.数据.h:79</code> | direct_const_member | this=&amp;系统角色 | 第二项短路条件 | 配置与固定配置相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0430 | F0071 | F0229 | <code>海中鱼巣/生产运行期配置.数据.h:79</code> | direct_const_member | this=&amp;概念活动 | 第三项短路条件 | 系统角色有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0431 | F0071 | F0230 | <code>海中鱼巣/生产运行期配置.数据.h:82</code> | direct_const_member | this=&amp;系统角色 | 系统键组 | 三个前置均通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0432 | F0071 | F0231 | <code>海中鱼巣/生产运行期配置.数据.h:83</code> | direct_const_member | this=&amp;概念活动 | 活动键组 | 系统键组已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0433 | F0072 | F0232 | <code>海中鱼巣/启动.运行期上下文.ixx:45</code> | direct_constructor | 域编号 | 协调器_ | 成员初始化开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0434 | F0072 | F0233 | <code>海中鱼巣/启动.运行期上下文.ixx:46</code> | direct_member | this=&amp;协调器_ | 接线_ | 协调器_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0435 | F0072 | F0135 | <code>海中鱼巣/启动.运行期上下文.ixx:47</code> | direct_constructor | 仓库编号,接线_副本 | 主信息_ | 接线_初始化完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0436 | F0072 | F0136 | <code>海中鱼巣/启动.运行期上下文.ixx:48</code> | direct_constructor | 主信息_,仓库编号,接线_副本 | 节点_ | 主信息_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0437 | F0072 | F0137 | <code>海中鱼巣/启动.运行期上下文.ixx:49</code> | direct_constructor | 节点_,仓库编号,接线_副本 | 关系_ | 节点_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0438 | F0072 | F0138 | <code>海中鱼巣/启动.运行期上下文.ixx:50</code> | direct_constructor | 节点_,接线_副本 | 索引_ | 关系_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0439 | F0072 | F0238 | <code>海中鱼巣/启动.运行期上下文.ixx:51-52</code> | direct_constructor | 接线_,主信息_,节点_,关系_,索引_,仓库编号,稳定动作键 | 业务装配_ | 四仓库构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0443 | F0073 | F0241 | <code>海中鱼巣/启动.运行期上下文.ixx:100</code> | direct_const_member | this=*系统角色清单_,参数 | 幂等参数匹配 | 缓存存在且持有系统角色锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0444 | F0073 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:103</code> | direct_const_member | *系统角色清单_ | 当前 | 缓存存在且参数匹配，仍持锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0445 | F0073 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:104, 海中鱼巣/启动.运行期上下文.ixx:109</code> | direct_const_member | this=&amp;当前 | 是否改写幂等读回 | 缓存复核已返回；下层初始化已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0446 | F0073 | F0243 | <code>海中鱼巣/启动.运行期上下文.ixx:108</code> | direct_member | 参数 | 结果 | 首次缓存为空且第一把锁已释放 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0448 | F0073 | F0244 | <code>海中鱼巣/启动.运行期上下文.ixx:111</code> | defaulted_const_member | this=*系统角色清单_,结果.清单 | 并发缓存一致性 | 第二次加锁后缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0442 | F0073 | F0228 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member | this=&amp;参数 | 第三项准入 | 业务装配完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0440 | F0073 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member+unique_name | this | 首项准入 | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0441 | F0073 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member | this=&amp;业务装配_ | 第二项准入 | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0449 | F0074 | F0245 | <code>海中鱼巣/领域/系统角色清单.数据.h:297</code> | direct_const_member | this=&amp;清单 | 函数返回值 | 状态为已提交或幂等读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0452 | F0075 | F0229 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member | this=&amp;参数 | 第三项准入 | 业务装配完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0450 | F0075 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member+unique_name | this | 首项准入 | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0451 | F0075 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member | this=&amp;业务装配_ | 第二项准入 | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0453 | F0075 | F0246 | <code>海中鱼巣/启动.运行期上下文.ixx:131</code> | direct_const_member+unique_name | this | 清单 | 三项准入通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0454 | F0075 | F0245 | <code>海中鱼巣/启动.运行期上下文.ixx:132</code> | direct_const_member | this=&amp;清单.value | 清单完整性 | 清单存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0456 | F0075 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:133</code> | direct_const_member | this=&amp;系统角色复核临时结果 | 系统角色准入 | 复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0455 | F0075 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:133</code> | direct_const_member | *清单 | 系统角色复核临时结果 | 清单完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0457 | F0075 | F0247 | <code>海中鱼巣/启动.运行期上下文.ixx:137</code> | direct_const_member | this=*概念活动材料_,参数 | 缓存参数匹配 | 持锁且缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0458 | F0075 | F0248 | <code>海中鱼巣/启动.运行期上下文.ixx:140</code> | direct_const_member+unique_name | *清单,*概念活动材料_ | 当前 | 缓存参数匹配，仍持锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0459 | F0075 | F0076 | <code>海中鱼巣/启动.运行期上下文.ixx:141, 海中鱼巣/启动.运行期上下文.ixx:146</code> | direct_const_member | this=&amp;当前 | 是否改写幂等读回 | 缓存复核已返回；下层初始化已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0460 | F0075 | F0249 | <code>海中鱼巣/启动.运行期上下文.ixx:145</code> | direct_member | *清单,参数 | 结果 | 缓存为空且第一把锁已释放 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0462 | F0075 | F0250 | <code>海中鱼巣/启动.运行期上下文.ixx:148</code> | direct_const_member | this=*概念活动材料_,结果.材料 | 并发缓存一致性 | 第二次加锁后缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0463 | F0076 | F0251 | <code>海中鱼巣/领域/概念活动状态.数据.h:213</code> | direct_const_member | this=&amp;材料 | 函数返回值 | 状态为已提交或幂等读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0464 | F0077 | F0246 | <code>海中鱼巣/启动.运行期上下文.ixx:75</code> | direct_const_member+unique_name | this | 清单 | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0465 | F0077 | F0252 | <code>海中鱼巣/启动.运行期上下文.ixx:76</code> | direct_const_member+unique_name | this | 活动 | 清单已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0466 | F0077 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:77</code> | direct_const_member+unique_name | this | 第一项短路条件 | 两份材料均已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0467 | F0077 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:77</code> | direct_const_member | this=&amp;业务装配_ | 第二项短路条件 | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0470 | F0077 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | this=&amp;系统角色复核临时结果 | 系统角色复核条件 | 系统角色复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0469 | F0077 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | *清单 | 系统角色复核临时结果 | 清单完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0468 | F0077 | F0245 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | this=&amp;清单.value | 清单完整性 | 装配完整且清单存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0471 | F0077 | F0251 | <code>海中鱼巣/启动.运行期上下文.ixx:79</code> | direct_const_member | this=&amp;活动.value | 活动完整性 | 系统角色复核成功且活动存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0473 | F0077 | F0076 | <code>海中鱼巣/启动.运行期上下文.ixx:80</code> | direct_const_member | this=&amp;概念活动复核临时结果 | 函数返回值 | 概念活动复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0472 | F0077 | F0248 | <code>海中鱼巣/启动.运行期上下文.ixx:80</code> | direct_const_member+unique_name | *清单,*活动 | 概念活动复核临时结果 | 活动完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0474 | F0078 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:213</code> | direct_const_member | this=候选.get() | 结构核心预检 | 候选非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0475 | F0078 | F0253 | <code>海中鱼巣/启动.运行期上下文.ixx:216</code> | direct_const_member | this=候选.get() | 服务装配预检 | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0476 | F0078 | F0077 | <code>海中鱼巣/启动.运行期上下文.ixx:219, 海中鱼巣/启动.运行期上下文.ixx:226</code> | direct_const_member | this=候选.get() | 锁外综合完整性 | 服务装配完整；持有发布锁且当前上下文为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0479 | F0080 | F0254 | <code>海中鱼巣/自检.入口初始化.ixx:659</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0481 | F0081 | F0255 | <code>海中鱼巣/自检.入口初始化.ixx:661</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0483 | F0082 | F0256 | <code>海中鱼巣/自检.入口初始化.ixx:663</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0485 | F0083 | F0257 | <code>海中鱼巣/自检.入口初始化.ixx:665</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0487 | F0084 | F0258 | <code>海中鱼巣/自检.入口初始化.ixx:667</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0489 | F0085 | F0259 | <code>海中鱼巣/自检.入口初始化.ixx:669</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0491 | F0086 | F0260 | <code>海中鱼巣/自检.入口初始化.ixx:671</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0493 | F0087 | F0261 | <code>海中鱼巣/自检.入口初始化.ixx:673</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0495 | F0088 | F0262 | <code>海中鱼巣/自检.入口初始化.ixx:675</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0497 | F0089 | F0263 | <code>海中鱼巣/自检.入口初始化.ixx:677</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0499 | F0090 | F0264 | <code>海中鱼巣/自检.入口初始化.ixx:679</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0501 | F0091 | F0265 | <code>海中鱼巣/自检.入口初始化.ixx:681</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0503 | F0092 | F0266 | <code>海中鱼巣/自检.入口初始化.ixx:683</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0505 | F0093 | F0267 | <code>海中鱼巣/自检.入口初始化.ixx:685</code> | direct_free | 捕获配置 | 结果 | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0511 | F0096 | F0272 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:878</code> | constructor | 0,固定随机种子 | 无效夹具 | F0096执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0512 | F0096 | F0273 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:879</code> | direct_member_nonvirtual | this=&amp;无效夹具 | 构建成功并取反为F0096返回值 | 夹具构造成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0513 | F0097 | F0272 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:761</code> | constructor | 规模,固定随机种子 | 夹具 | 报告基本字段已写 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0514 | F0097 | F0273 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:763</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 构建成功 | 构建开始时间已记录；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0515 | F0097 | F0274 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:766</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 报告.结构签名 | 夹具构建成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0516 | F0097 | F0275 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:767</code> | direct_member_nonvirtual | this=&amp;夹具 | 报告.初始当前记录数量 | 结构签名已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0517 | F0097 | F0276 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:768</code> | direct_member_nonvirtual | this=&amp;夹具 | 报告.分布完整 | 初始数量已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0518 | F0097 | F0277 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:769</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 报告.结构边界完整 | 分布结果已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0519 | F0097 | F0278 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:771</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 查询定义组 | 夹具摘要字段已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0520 | F0097 | F0279 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:782, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:788</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具,定义 | 临时测量.正确 | 预热批内循环；正式批内循环；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0522 | F0097 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:797</code> | direct_free | 本轮汇总样本,0.50 | 报告.轮中位数纳秒[轮] | 本轮全部查询测量完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0523 | F0097 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:801-805</code> | direct_free | 查询名称,查询样本,初始当前记录数量,累计正确 | 追加到报告.查询指标组 | 指标索引有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0524 | F0097 | F0282 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:812</code> | direct_free | 夹具,查询定义组[1],报告.查询指标组[1].中位数纳秒 | 报告.并发 | 查询指标形成完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0525 | F0097 | F0283 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:813</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 报告.参考模型一致 | 并发基线返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0526 | F0097 | F0284 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:815</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 追加创建指标 | 参考模型一致位已计算，不要求为true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0527 | F0097 | F0285 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:816</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 追加失效指标 | 创建测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0528 | F0097 | F0286 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:817</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 追加重挂指标 | 失效测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0529 | F0097 | F0287 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:818</code> | direct_member_nonvirtual+unique_name | this=&amp;夹具 | 追加删除指标 | 重挂测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0531 | F0097 | F0198 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:819</code> | direct_member_nonvirtual | this=&amp;夹具关系仓库 | 有效数量并与规模比较 | 关系仓库引用已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0530 | F0097 | F0288 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:819</code> | direct_member_nonvirtual | this=&amp;夹具 | 关系仓库左值引用 | 四项写入测量完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0532 | F0098 | F0289 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:904-906</code> | external_callback | 当前查询分位指标const引用 | 查询all_of谓词bool | 参考模型一致且标准算法遍历未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0533 | F0098 | F0290 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:907-909</code> | external_callback | 当前写入分位指标const引用 | 写入all_of谓词bool | 参考模型与查询指标一致且标准算法遍历未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0534 | F0099 | F0291 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:741-743</code> | external_callback | 当前查询分位指标const引用 | 查询all_of谓词bool | 基础字段和查询写入指标数量门禁通过且算法未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0535 | F0099 | F0292 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:744-746</code> | external_callback | 当前写入分位指标const引用 | 写入all_of谓词bool | 基础字段和两组数量门禁通过、查询指标全部完整且算法未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0536 | F0099 | F0293 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:747</code> | direct_const_member | this=&amp;并发 | 并发完整bool | 查询和写入指标全部完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0200 | F0120 | F0043 | <code>海中鱼巣/启动.应用程序.ixx:209</code> | captured_member_direct+direct_member | 键, 上下文.状态 | 方法登记根材料候选 | 失败点!=0；实际五轮；失败点 != 0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0201 | F0121 | F0044 | <code>海中鱼巣/启动.应用程序.ixx:215</code> | captured_member_direct+direct_member | 参数 | 自我线程操作结果 | 失败点!=1；实际四轮；失败点 != 1 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0202 | F0122 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:221</code> | captured_member_direct+direct_member | 时限 | 等待结果 | 失败点!=2；实际三轮；失败点 != 2 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0203 | F0123 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:227</code> | captured_member_direct+direct_member | 上下文.自我线程实例隐式this | 初始化快照候选 | 失败点!=3；实际三轮；失败点 != 3 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0204 | F0124 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:233</code> | captured_member_direct+direct_member | 上下文.概念图初始化隐式this | 概念图初始化结果 | 失败点!=4；实际一轮；失败点 != 4 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0611 | F0125 | F0050 | <code>海中鱼巣/启动.应用程序.ixx:239</code> | direct_member | 上下文.概念图隐式this,实例 | 支持句柄optional直接返回 | 失败点 != 5 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0613 | F0135 | F0321 | <code>海中鱼巣/核心/主信息仓库.cpp:33</code> | direct_member+unique_name | 事务接线_隐式this | 接线形态有效bool | 全部成员构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0614 | F0136 | F0321 | <code>海中鱼巣/核心/节点仓库.cpp:45</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0616 | F0136 | F0322 | <code>海中鱼巣/核心/节点仓库.cpp:48</code> | direct_free_function+direct-free | 事务接线_,主信息_.事务接线_ | 接线一致 | 两个接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0617 | F0137 | F0321 | <code>海中鱼巣/核心/关系仓库.cpp:171</code> | direct_member+unique_name | 事务接线_隐式this | 本仓库接线形态有效bool | 全部成员构造完成；本仓库接线形态有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0619 | F0137 | F0323 | <code>海中鱼巣/核心/关系仓库.cpp:174</code> | direct_free+direct-free | 事务接线_,节点_.事务接线_ | 接线一致bool | 两份接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0620 | F0138 | F0321 | <code>海中鱼巣/核心/索引仓库.cpp:94</code> | direct_member+unique_name | 事务接线_隐式this | 本仓库接线形态有效bool | 全部成员构造完成；本仓库接线形态有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0622 | F0138 | F0324 | <code>海中鱼巣/核心/索引仓库.cpp:97</code> | direct_free+direct-free | 事务接线_,节点_.事务接线_ | 接线一致bool | 两份接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0623 | F0158 | F0325 | <code>海中鱼巣/适配/SQL数据库适配.cpp:316, 海中鱼巣/适配/SQL数据库适配.cpp:319</code> | direct_free+direct-free+direct-free | L"HY_EGO_SQL_SERVER" | 服务器 | 默认配置已构造；服务器覆盖分支已汇合；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0625 | F0160 | F0326 | <code>海中鱼巣/领域/方法服务.h:1337</code> | direct_member | *登记根材料_ | 材料完整bool | optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0626 | F0160 | F0327 | <code>海中鱼巣/领域/方法服务.h:1338</code> | direct_member+unique_name | 登记根材料_-&gt;登记根,状态 | 登记根角色状态 | 材料完整且仓库编号非零；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0627 | F0160 | F0328 | <code>海中鱼巣/领域/方法服务.h:1339, 海中鱼巣/领域/方法服务.h:1340</code> | direct_member+unique_name | 登记根材料_-&gt;活跃状态 | 活跃是否实例bool | 登记根角色匹配；活跃不是实例状态；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0629 | F0160 | F0329 | <code>海中鱼巣/领域/方法服务.h:1343, 海中鱼巣/领域/方法服务.h:1344</code> | direct_member | this=&amp;状态,登记根材料_-&gt;活跃状态；this=&amp;状态,登记根材料_-&gt;失效状态 | 活跃状态值；失效状态值 | 1337—1340前置全部通过；1343读取活跃状态值后，1344无条件读取失效状态值；第二次不依赖第一次返回内容 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0631 | F0160 | F0330 | <code>海中鱼巣/领域/方法服务.h:1345-1346, 海中鱼巣/领域/方法服务.h:1347-1348</code> | direct_member | this=&amp;方法服务,登记根材料_-&gt;登记根,关系类型::模板,节点类型::状态,根活跃状态顺序号；this=&amp;方法服务,登记根材料_-&gt;登记根,关系类型::模板,节点类型::状态,根失效状态顺序号 | 活跃状态目标；失效状态目标 | E0629两次状态值读取完成；1345—1346读取活跃状态目标后，1347—1348无条件读取失效状态目标；第二次不依赖第一次返回内容 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0633 | F0161 | F0184 | <code>海中鱼巣/领域/方法服务.h:1304</code> | direct_free | false,说明 | 检查结果后丢弃 | 可能的解锁已完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0634 | F0162 | F0331 | <code>海中鱼巣/领域/方法服务.h:310</code> | direct_member | this=&amp;主信息_ | 新主信息 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0635 | F0162 | F0332 | <code>海中鱼巣/领域/方法服务.h:310</code> | direct_member | 节点类型::方法,新主信息 | 新方法节点 | 主信息创建调用完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0636 | F0164 | F0333 | <code>海中鱼巣/领域/方法服务.h:1645</code> | direct_member | 方法节点,节点类型::方法 | 类型匹配bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0637 | F0164 | F0334 | <code>海中鱼巣/领域/方法服务.h:1645</code> | direct_member+unique_name | 角色 | 角色有效bool | 方法节点类型匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0638 | F0164 | F0165 | <code>海中鱼巣/领域/方法服务.h:1648</code> | direct_member | static_cast&lt;int64_t&gt;(角色) | 角色状态 | 输入准入通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0639 | F0164 | F0163 | <code>海中鱼巣/领域/方法服务.h:1649</code> | direct_free | 角色状态 | 状态句柄有效bool | 抽象状态创建完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0641 | F0164 | F0168 | <code>海中鱼巣/领域/方法服务.h:1649, 海中鱼巣/领域/方法服务.h:1653</code> | direct_free+unique_name | 角色关系 | 关系句柄有效bool | 关系创建完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0640 | F0164 | F0167 | <code>海中鱼巣/领域/方法服务.h:1652</code> | direct_member | 模板,方法节点,角色状态,0 | 角色关系 | 状态句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0642 | F0165 | F0335 | <code>海中鱼巣/领域/状态服务.h:158</code> | direct_member+unique_name | 状态值,std::nullopt | 状态节点 | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0643 | F0165 | F0329 | <code>海中鱼巣/领域/状态服务.h:159</code> | direct_member+suffix_match | 状态节点 | 读回状态值 | 状态节点创建调用完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0645 | F0165 | F0184 | <code>海中鱼巣/领域/状态服务.h:160, 海中鱼巣/领域/状态服务.h:160-161</code> | direct_free+unique_name | 复核条件,固定说明 | 复核结果 | 短路复核完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0644 | F0165 | F0328 | <code>海中鱼巣/领域/状态服务.h:160</code> | direct_member+unique_name | 状态节点 | 是否实例状态bool | 读回值存在且等于输入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0662 | F0167 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:185-275</code> | direct_destructor | 自动令牌范围 | void | 已构造令牌范围离开作用域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0663 | F0167 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:185-275</code> | direct_destructor | 自动许可 | void | 已取得许可离开作用域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0646 | F0167 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | 事务接线_隐式this | 已接域bool | 许可宏进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0647 | F0167 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:185, 海中鱼巣/核心/关系仓库.cpp:205, 海中鱼巣/核心/关系仓库.cpp:212, 海中鱼巣/核心/关系仓库.cpp:220</code> | direct_free+direct-free+direct-free+direct-free | *this | 当前关系令牌指针 | 已接域；任务方法选择类型；方法动作场景类型；用途观察证据角色类型；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0648 | F0167 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | 自动许可 | 许可有效bool | 动态取得许可完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0649 | F0167 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | 自动许可 | 令牌引用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0650 | F0167 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_constructor | *this,自动许可.读取令牌() | 自动令牌范围 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0651 | F0167 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:186</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 许可语境建立完成；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0655 | F0167 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:191</code> | direct_free | 源节点 | 源句柄有效bool | 关系类型有效；源句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0657 | F0167 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:196</code> | direct_member+direct-member+direct-member | 源节点 | 源节点有效bool | 端点句柄有效；源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0665 | F0167 | F0190 | <code>海中鱼巣/核心/关系仓库.cpp:206, 海中鱼巣/核心/关系仓库.cpp:207, 海中鱼巣/核心/关系仓库.cpp:213, 海中鱼巣/核心/关系仓库.cpp:214, 海中鱼巣/核心/关系仓库.cpp:221, 海中鱼巣/核心/关系仓库.cpp:222</code> | direct_member | 源节点 | 源记录 | 任务方法选择且无令牌；方法动作场景且无令牌；方法动作场景且无令牌；用途观察且无令牌；用途观察且无令牌 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0664 | F0167 | F0346 | <code>海中鱼巣/核心/关系仓库.cpp:206, 海中鱼巣/核心/关系仓库.cpp:207, 海中鱼巣/核心/关系仓库.cpp:213, 海中鱼巣/核心/关系仓库.cpp:214, 海中鱼巣/核心/关系仓库.cpp:221, 海中鱼巣/核心/关系仓库.cpp:222</code> | direct_member | 源节点,*当前令牌 | 源记录 | 任务方法选择且有令牌；方法动作场景且有令牌；方法动作场景且有令牌；用途观察且有令牌；用途观察且有令牌 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0676 | F0167 | F0347 | <code>海中鱼巣/核心/关系仓库.cpp:223</code> | direct_free+direct-free | 顺序号 | 观察角色有效bool | 用途观察类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0677 | F0167 | F0348 | <code>海中鱼巣/核心/关系仓库.cpp:225</code> | direct_free+direct-free | 顺序号,目标记录-&gt;类型 | 观察目标类型有效bool | 角色、源与目标记录前置成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0678 | F0171 | F0349 | <code>海中鱼巣/线程/自我线程.ixx:205</code> | lambda_direct_member | 捕获参数副本 | void | 工作线程入口执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0679 | F0173 | F0220 | <code>海中鱼巣/领域/初始化.语素.ixx:68, 海中鱼巣/领域/初始化.语素.ixx:69, 海中鱼巣/领域/初始化.语素.ixx:70, 海中鱼巣/领域/初始化.语素.ixx:71, 海中鱼巣/领域/初始化.语素.ixx:72, 海中鱼巣/领域/初始化.语素.ixx:73, 海中鱼巣/领域/初始化.语素.ixx:74, 海中鱼巣/领域/初始化.语素.ixx:75, 海中鱼巣/领域/初始化.语素.ixx:76, 海中鱼巣/领域/初始化.语素.ixx:77, 海中鱼巣/领域/初始化.语素.ixx:78, 海中鱼巣/领域/初始化.语素.ixx:79, 海中鱼巣/领域/初始化.语素.ixx:80, 海中鱼巣/领域/初始化.语素.ixx:81, 海中鱼巣/领域/初始化.语素.ixx:82, 海中鱼巣/领域/初始化.语素.ixx:83, 海中鱼巣/领域/初始化.语素.ixx:84, 海中鱼巣/领域/初始化.语素.ixx:85</code> | direct_member | 通用节点名称 | 成功bool | 函数进入；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0697 | F0174 | F0163 | <code>海中鱼巣/领域/初始化.世界树.ixx:34, 海中鱼巣/领域/初始化.世界树.ixx:35, 海中鱼巣/领域/初始化.世界树.ixx:36</code> | direct_free | 世界树根节点 | 根有效bool | 函数进入；根有效；场景有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0700 | F0174 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:34, 海中鱼巣/领域/初始化.世界树.ixx:35, 海中鱼巣/领域/初始化.世界树.ixx:36, 海中鱼巣/领域/初始化.世界树.ixx:37, 海中鱼巣/领域/初始化.世界树.ixx:38, 海中鱼巣/领域/初始化.世界树.ixx:39</code> | direct_free+unique_name | 根到场景关系 | 关系有效bool | 自我节点有效；根场景关系有效；父子关系有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0705 | F0175 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | direct_free | 概念命名目标状态 | 目标状态句柄有效bool | 两个单根需求均成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0037 | F0175 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0703 | F0175 | F0350 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | direct_const_member | this=&amp;安全根需求 | 安全根需求成功bool | 函数进入；安全根需求成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0706 | F0176 | F0189 | <code>海中鱼巣/领域/初始化.概念图.ixx:224</code> | direct_const_member | this=根 | 根项成功bool | 当前根指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0707 | F0176 | F0351 | <code>海中鱼巣/领域/初始化.概念图.ixx:224</code> | direct_const_member+unique_name | this=概念图初始化服务,*根 | 名称关系可读bool | 当前根项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0708 | F0176 | F0352 | <code>海中鱼巣/领域/初始化.概念图.ixx:227</code> | direct_const_member+unique_name | this=&amp;概念图_,类别=根-&gt;根类别 | 登记optional | 根项与名称关系均有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0709 | F0176 | F0353 | <code>海中鱼巣/领域/初始化.概念图.ixx:233</code> | direct_const_member+unique_name | this=&amp;概念图_,概念=根-&gt;根节点 | 生命周期optional | 登记材料匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0710 | F0176 | F0354 | <code>海中鱼巣/领域/初始化.概念图.ixx:246</code> | direct_const_member+unique_name | this=&amp;概念图_,阶段 | 已登记optional | 进入当前状态循环项；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0711 | F0176 | F0329 | <code>海中鱼巣/领域/初始化.概念图.ixx:247</code> | direct_const_member | this=&amp;状态_,状态节点 | 状态值optional | 读取生命周期状态之后无条件执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0712 | F0176 | F0328 | <code>海中鱼巣/领域/初始化.概念图.ixx:252</code> | direct_const_member+unique_name | this=&amp;状态_,状态节点 | 实例状态bool | 登记与状态值前四项均匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0713 | F0176 | F0355 | <code>海中鱼巣/领域/初始化.概念图.ixx:256</code> | direct_const_member+unique_name | this=&amp;概念图_ | 全部概念根临时vector | 四根与三状态循环均完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE1636 | F0177 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member |  |  | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1637 | F0177 | F0185 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke |  |  | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1638 | F0177 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free |  |  | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1639 | F0177 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member |  |  | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1640 | F0177 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free |  |  | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1641 | F0177 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator |  |  | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1642 | F0178 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member |  |  | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1643 | F0178 | F0186 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke |  |  | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1644 | F0178 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free |  |  | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1645 | F0178 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member |  |  | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1646 | F0178 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free |  |  | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1647 | F0178 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator |  |  | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1648 | F0179 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member |  |  | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1649 | F0179 | F0187 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke |  |  | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1650 | F0179 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free |  |  | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1651 | F0179 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member |  |  | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1652 | F0179 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free |  |  | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1653 | F0179 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator |  |  | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| E0714 | F0180 | F0352 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | this=&amp;概念图_,类别=存在 | 已登记optional | 函数进入；F0177成功后进入F0178；F0177和F0178成功后进入F0179；前三根均成功后进入F0180 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE1654 | F0180 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member |  |  | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| E0733 | F0180 | F0188 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke | 创建回调，无参数 | 因果根节点 | 因果根未登记；不存在已登记根时调用本实例绑定的创建回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0716 | F0180 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free | 根节点 | 根有效bool | 创建回调返回；F0186返回；F0187返回；F0188返回；创建回调返回后复核根节点句柄 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0717 | F0180 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member | this=&amp;概念图_,材料={存在,固定稳定键,根节点} | 登记结果optional | 根节点有效；根节点有效后登记概念根 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0719 | F0180 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free | 登记有值且同根,L"概念根创建后登记不符合内部预期。" | 检查通过bool | 登记完成；条件真假均调用；登记完成后复核发布读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0718 | F0180 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | direct_operator+operator | 登记结果.value(),根节点 | 同根bool | 登记结果有值；登记结果有值后比较根节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0738 | F0181 | F0354 | <code>海中鱼巣/领域/初始化.概念图.ixx:187</code> | direct_const_member+unique_name | this=&amp;概念图_,阶段 | 已登记optional | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0739 | F0181 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:192, 海中鱼巣/领域/初始化.概念图.ixx:195</code> | direct_free | 状态节点 | 候选句柄有效bool | 无已登记状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0038 | F0181 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:192, 海中鱼巣/领域/初始化.概念图.ixx:195</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0740 | F0181 | F0165 | <code>海中鱼巣/领域/初始化.概念图.ixx:193</code> | direct_member | this=&amp;状态_,状态值=int64(阶段) | 新状态节点并写回 | 无登记且候选句柄无效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0742 | F0181 | F0357 | <code>海中鱼巣/领域/初始化.概念图.ixx:198</code> | direct_member+unique_name | this=&amp;概念图_,阶段,状态节点,状态_ | 登记结果optional | 状态节点有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0743 | F0181 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:199</code> | direct_operator | 登记结果.value(),状态节点 | 同状态bool | 登记结果有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0744 | F0181 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:199, 海中鱼巣/领域/初始化.概念图.ixx:199-200, 海中鱼巣/领域/初始化.概念图.ixx:205, 海中鱼巣/领域/初始化.概念图.ixx:205-209</code> | direct_free+unique_name | 登记有值且同状态,固定说明 | 登记检查bool | 登记尝试后总调用；读回状态值后总调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0745 | F0181 | F0329 | <code>海中鱼巣/领域/初始化.概念图.ixx:204</code> | direct_const_member | this=&amp;状态_,状态节点 | 状态值optional | 复用或登记路径汇合 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0746 | F0181 | F0328 | <code>海中鱼巣/领域/初始化.概念图.ixx:208</code> | direct_const_member+unique_name | this=&amp;状态_,状态节点 | 实例状态bool | 状态值存在且等于阶段；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0752 | F0182 | F0360 | <code>海中鱼巣/领域/概念图服务.h:1007</code> | direct_const_member | 根节点 | 生命周期optional | 四根确保全部成功后的逐根读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0753 | F0182 | F0361 | <code>海中鱼巣/领域/概念图服务.h:1016</code> | direct_member+unique_name | 本轮新增关系组 | 清理完成bool | 任一确保或读回失败；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0755 | F0182 | F0184 | <code>海中鱼巣/领域/概念图服务.h:1017, 海中鱼巣/领域/概念图服务.h:1017-1019, 海中鱼巣/领域/概念图服务.h:1023, 海中鱼巣/领域/概念图服务.h:1023-1024</code> | direct_free+unique_name | 清理完成且关系数恢复,固定说明 | 诊断bool并丢弃 | 失败清理路径总调用；活动快照存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0750 | F0182 | F0198 | <code>海中鱼巣/领域/概念图服务.h:1018, 海中鱼巣/领域/概念图服务.h:992</code> | direct_const_member | this=&amp;关系_ | 原关系数量 | 根数量等于4且双锁已取得；清理函数返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0756 | F0182 | F0362 | <code>海中鱼巣/领域/概念图服务.h:1023</code> | direct_const_member+unique_name | 活动快照_.value() | 快照有效bool | 全部就绪且活动快照存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0748 | F0182 | F0358 | <code>海中鱼巣/领域/概念图服务.h:982</code> | direct_const_member+unique_name | this | 登记完整bool | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0749 | F0182 | F0355 | <code>海中鱼巣/领域/概念图服务.h:985</code> | direct_const_member+unique_name | this | 根组vector | 生命周期状态登记完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0751 | F0182 | F0359 | <code>海中鱼巣/领域/概念图服务.h:996, 海中鱼巣/领域/概念图服务.h:996-1000</code> | direct_member+unique_name | 根节点,活跃,true,本轮新增关系组&amp; | 生命周期关系optional | 逐根确保循环；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0758 | F0183 | F0363 | <code>海中鱼巣/领域/初始化.概念图.ixx:177, 海中鱼巣/领域/初始化.概念图.ixx:182</code> | direct_const_member | this=&amp;项.名称语素入口 | 已有材料成功bool | 函数进入；创建和字段写入后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0759 | F0183 | F0351 | <code>海中鱼巣/领域/初始化.概念图.ixx:178, 海中鱼巣/领域/初始化.概念图.ixx:182</code> | direct_const_member+unique_name | this=概念图初始化服务,项 | 已有名称关系可读bool并返回 | 已有名称材料成功；新材料成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0760 | F0183 | F0364 | <code>海中鱼巣/领域/初始化.概念图.ixx:180</code> | direct_member+unique_name | this=&amp;语素_,名称,项.根节点,项.根节点,0 | 创建结果并写项.名称语素入口 | 无成功名称材料；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0763 | F0183 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:182, 海中鱼巣/领域/初始化.概念图.ixx:182-183</code> | direct_free+unique_name | 新材料成功且关系可读,固定说明 | 检查bool并返回 | 创建路径总调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0765 | F0185 | F0365 | <code>海中鱼巣/领域/初始化.概念图.ixx:105</code> | lambda_direct | this=&amp;捕获this-&gt;存在_,无显式参数 | 存在根节点并作为lambda返回 | F0177未读到存在根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0766 | F0186 | F0366 | <code>海中鱼巣/领域/初始化.概念图.ixx:107</code> | lambda_direct | this=&amp;捕获this-&gt;动态_,无显式参数 | 动态根节点并作为lambda返回 | F0180链到达F0178且F0178未读到动态根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0767 | F0187 | F0367 | <code>海中鱼巣/领域/初始化.概念图.ixx:109</code> | lambda_direct | this=&amp;捕获this-&gt;二次特征_,无显式参数 | 关系概念根句柄并作为lambda返回 | F0179到达且未读到关系根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0768 | F0188 | F0368 | <code>海中鱼巣/领域/初始化.概念图.ixx:111</code> | lambda_direct | this=&amp;捕获this-&gt;因果_,无显式参数 | 因果概念根句柄并作为lambda返回 | F0180到达且未读到因果根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0769 | F0189 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:34</code> | direct_free | 根节点 | 根句柄有效bool | 类别范围和稳定键均通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0040 | F0189 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:34</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0770 | F0189 | F0363 | <code>海中鱼巣/领域/初始化.概念图.ixx:35</code> | direct_const_member | this=&amp;名称语素入口 | 名称材料成功bool | 根句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0771 | F0190 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:234</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0775 | F0190 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:235-237</code> | implicit_destructor | this=&amp;许可 | void | 已接域路径退出；返回对象形成之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0041 | F0190 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:235</code> | resolved-function-pointer |  |  | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0772 | F0190 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member | this=&amp;许可 | 许可有效bool | 已接域且许可对象已形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0773 | F0190 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member | this=&amp;许可 | 结构事务令牌const引用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0774 | F0190 | F0346 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member+direct-member | this=节点仓库,节点,许可令牌 | 节点记录optional返回对象 | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0776 | F0191 | F0369 | <code>海中鱼巣/领域/概念图服务.h:4406</code> | direct_const_member+unique_name | this,实例 | 实例类别optional | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0777 | F0191 | F0352 | <code>海中鱼巣/领域/概念图服务.h:4407</code> | direct_const_member+unique_name | this,类别 | 根材料optional | F0369调用后无条件执行；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0778 | F0191 | F0051 | <code>海中鱼巣/领域/概念图服务.h:4411, 海中鱼巣/领域/概念图服务.h:4419, 海中鱼巣/领域/概念图服务.h:4420</code> | direct_operator+external_callback+external_callback | 实例,根材料-&gt;根节点 | 实例等于根bool | 实例类别匹配且根材料存在；第一组find迭代；第一组命中后的第二组find迭代 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0779 | F0191 | F0370 | <code>海中鱼巣/领域/概念图服务.h:4414</code> | direct_member+unique_name | this,实例,根材料-&gt;根节点 | 绑定成功bool | 实例不是根；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0780 | F0191 | F0371 | <code>海中鱼巣/领域/概念图服务.h:4417</code> | direct_const_member+unique_name | this,实例 | 实例目标组 | 绑定成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0781 | F0191 | F0372 | <code>海中鱼巣/领域/概念图服务.h:4418</code> | direct_const_member+unique_name | this,根材料-&gt;根节点 | 根来源组 | 实例目标组读取后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0784 | F0191 | F0184 | <code>海中鱼巣/领域/概念图服务.h:4421</code> | direct_free+unique_name | 双向可读,固定说明 | 检查bool | 两次find组合条件形成后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0785 | F0196 | F0373 | <code>海中鱼巣/领域/统计服务.h:258</code> | direct_const_member+unique_name | this,命名空间 | 通用命名空间有效bool | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0786 | F0197 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:454</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0790 | F0197 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:455-457</code> | implicit_destructor | this=&amp;许可 | void | 接域路径退出；返回值形成后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0042 | F0197 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:455</code> | resolved-function-pointer |  |  | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0787 | F0197 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member | this=&amp;许可 | 许可有效bool | 已接域且许可对象形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0788 | F0197 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member | this=&amp;许可 | 令牌const引用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0789 | F0197 | F0374 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member+direct-member | this,许可令牌 | 令牌路径有效节点数量 | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0800 | F0198 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1013-1025</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0801 | F0198 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:1013-1025</code> | implicit_destructor | this=&amp;自动许可.value() | void | 退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0793 | F0198 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:1013</code> | move-constructor+macro-expanded-move-constructor |  |  | 共享许可宏的 optional.emplace 以许可临时量构造承载值；关系共享许可范围宏在 line 120 以 optional.emplace 接收函数返回值并移动构造结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| E0798 | F0198 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:1019, 海中鱼巣/核心/关系仓库.cpp:1020</code> | direct_const_member+direct-member+direct-member | this,记录.源节点 | 源节点有效bool | 记录状态有效；记录状态和源节点均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0802 | F0199 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:403</code> | direct_const_member | this=&amp;事务接线_ | 是否已接域 | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0806 | F0199 | F0345 | <code>海中鱼巣/核心/索引仓库.cpp:404-406</code> | implicit_destructor | this=&amp;许可 | void | 接域分支退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0043 | F0199 | F0397 | <code>海中鱼巣/核心/索引仓库.cpp:404</code> | resolved-function-pointer |  |  | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0803 | F0199 | F0338 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member | this=&amp;许可 | 许可是否有效 | 事务接线已接域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0804 | F0199 | F0339 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member | this=&amp;许可 | 结构事务令牌const引用 | 事务接线已接域且许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0805 | F0199 | F0376 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member+direct-member | this,令牌=许可.读取令牌() | 有效主键数量 | 事务接线已接域且许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0807 | F0199 | F0377 | <code>海中鱼巣/核心/索引仓库.cpp:414</code> | loop_direct_const_member | this=&amp;节点_,节点=候选 | 候选节点是否有效 | 未接域路径每个候选 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0808 | F0203 | F0378 | <code>海中鱼巣/适配/SQL数据库适配.cpp:330</code> | direct_free+source_audited | 第一次字段=配置_.服务器；第二次字段=配置_.数据库 | 服务器字段可用bool；数据库字段可用bool | 第一次总是调用；仅第一次返回true时调用第二次 | 当前源码逐调用点与Clang AST左到右短路表达式复核 |
| E0810 | F0203 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:332, 海中鱼巣/适配/SQL数据库适配.cpp:339</code> | direct_free+direct-free+direct-free | L"数据库配置准入",固定诊断 | 函数返回 | 任一配置准入失败；master连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0825 | F0203 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:337-348, 海中鱼巣/适配/SQL数据库适配.cpp:350-379</code> | implicit_destructor | this=&amp;主连接 | void | 主连接作用域任一路径退出；函数任一路径退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0812 | F0203 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:337, 海中鱼巣/适配/SQL数据库适配.cpp:350</code> | direct_constructor+constructor+constructor | 无 | 主连接 | 配置准入通过；建库SQL成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0814 | F0203 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:338, 海中鱼巣/适配/SQL数据库适配.cpp:351</code> | direct_member+direct-member+direct-member | 配置_,L"master",诊断 | 主连接打开结果并写诊断 | 配置准入通过；建库SQL成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0816 | F0203 | F0379 | <code>海中鱼巣/适配/SQL数据库适配.cpp:341</code> | direct_free+direct-free | 文本=配置_.数据库 | 数据库字符串 | master连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0817 | F0203 | F0380 | <code>海中鱼巣/适配/SQL数据库适配.cpp:342</code> | direct_free+direct-free | 文本=配置_.数据库 | 数据库标识符 | 数据库字符串已形成；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0818 | F0203 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:345, 海中鱼巣/适配/SQL数据库适配.cpp:375</code> | indeterminately_sequenced_argument+direct-member+direct-member | this=&amp;主连接 | 建库SQL连接句柄实参 | 建库SQL已形成；与move实参求值顺序不固定；建表SQL已形成；与move实参求值顺序不固定；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0819 | F0203 | F0381 | <code>海中鱼巣/适配/SQL数据库适配.cpp:345, 海中鱼巣/适配/SQL数据库适配.cpp:375</code> | direct_free+direct-free+direct-free | 主连接句柄,std::move(建库SQL),诊断 | 建库SQL执行结果并写诊断 | 全部实参求值完成；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0822 | F0203 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:346, 海中鱼巣/适配/SQL数据库适配.cpp:352, 海中鱼巣/适配/SQL数据库适配.cpp:376</code> | direct_free+direct-free+direct-free+direct-free | L"初始化数据库",std::move(诊断),0 | 函数返回 | 建库SQL失败；数据连接打开失败；建表SQL失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0827 | F0205 | F0382 | <code>海中鱼巣/适配/SQL数据库适配.cpp:133</code> | direct_member+suffix_match+direct-member | this | void | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0828 | F0205 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:134, 海中鱼巣/适配/SQL数据库适配.cpp:138, 海中鱼巣/适配/SQL数据库适配.cpp:138-139, 海中鱼巣/适配/SQL数据库适配.cpp:150, 海中鱼巣/适配/SQL数据库适配.cpp:181</code> | nested_direct_free+loop_nested_direct_free+loop_direct_free+direct-free+direct-free+direct-free+direct-free | ENV分配SQLRETURN | ENV分配成功 | F0382完成；ENV分配成功；每个驱动；DBC分配成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0830 | F0205 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:140, 海中鱼巣/适配/SQL数据库适配.cpp:188</code> | direct_free+loop_indeterminately_sequenced_argument+direct-free+direct-free | SQL_HANDLE_ENV,环境_ | 输出诊断 | 环境属性设置失败；连接失败；与驱动名前缀求值顺序不固定；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0834 | F0207 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:224</code> | nested_direct_free+direct-free | SQLAllocHandle语句分配结果 | 语句分配成功 | 总是；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0835 | F0211 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:52</code> | loop_direct_free+lsp_direct+direct-free | 当前SQLGetDiagRecW结果 | 当前诊断记录可读取 | 结果不等于SQL_NO_DATA；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0836 | F0213 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:301</code> | direct_free+lsp_direct+direct-free | SQLGetData结果 | 整数列读取成功 | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0837 | F0215 | F0382 | <code>海中鱼巣/适配/SQL数据库适配.cpp:126</code> | direct_member+suffix_match+direct-member | this | void | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0838 | F0216 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:290</code> | direct_free+lsp_direct+direct-free | SQLGetData结果 | 文本列读取成功 | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0839 | F0217 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:431</code> | direct_const_member | this=&amp;事务接线_ | 是否已接域 | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0843 | F0217 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:432-434</code> | implicit_destructor | this=&amp;许可 | void | 接域分支退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0044 | F0217 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:432</code> | resolved-function-pointer |  |  | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0840 | F0217 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member | this=&amp;许可 | 许可是否有效 | 已接域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0841 | F0217 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member | this=&amp;许可 | 令牌const引用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0842 | F0217 | F0383 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member+direct-member | 主信息,值索引,许可令牌 | 委托optional | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0844 | F0217 | F0384 | <code>海中鱼巣/核心/主信息仓库.cpp:435</code> | direct_const_member+direct-member | this,主信息 | 记录快照 | 未接域；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0850 | F0218 | F0386 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:103, 海中鱼巣/界面/投影.控制面板启动.ixx:103-108</code> | direct_free+lsp_direct | 当前节点,显示语义.value(),选定坐标optional | 材料.显示名称 | 全部实参求值完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0846 | F0218 | F0051 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:106, 海中鱼巣/界面/投影.控制面板启动.ixx:79</code> | algorithm_callback+indeterminately_sequenced_argument | 父链元素,当前节点 | std::find相等回调 | 句柄有效且深度未超；生成显示名称实参求值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0851 | F0218 | F0387 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:111</code> | direct_const_member | this=&amp;关系,当前节点 | 子节点组 | 当前节点已压入父链 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0852 | F0218 | F0218 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:116-117</code> | recursive | 同一只读依赖,子节点,同一父链引用 | 子材料 | 每个排序后子节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0845 | F0218 | F0163 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:78</code> | direct_free | 当前节点 | 句柄有效 | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0045 | F0218 | F0168 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:78</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0847 | F0218 | F0190 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:82</code> | direct_const_member | this=&amp;节点,当前节点 | 节点记录 | 不在父链 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0848 | F0218 | F0385 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:86-87, 海中鱼巣/界面/投影.控制面板启动.ixx:87</code> | direct_free+lsp_direct | 当前节点,记录.类型,初始化结果,语素初始化读数 | 显示语义 | 节点记录存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0853 | F0220 | F0163 | <code>海中鱼巣/领域/初始化.语素.ixx:25</code> | direct_free | 基础信息 | 基础信息句柄形态有效 | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0046 | F0220 | F0168 | <code>海中鱼巣/领域/初始化.语素.ixx:25</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0854 | F0220 | F0363 | <code>海中鱼巣/领域/初始化.语素.ixx:26</code> | direct_const_member | this=&amp;语素入口结果 | 语素入口结果成功 | 基础信息句柄形态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0855 | F0223 | F0388 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | this,false | 只读材料成功 | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0856 | F0223 | F0389 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | this | 窗口类注册成功 | 只读材料成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0857 | F0223 | F0390 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | this | 主窗口创建成功 | 前两项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0858 | F0223 | F0391 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | this | 菜单栏创建成功 | 前三项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0859 | F0223 | F0392 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | this | 快捷键表创建成功 | 前四项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0860 | F0223 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1564, 海中鱼巣/界面/控制面板窗口.cpp:1582</code> | direct_member+unique_name+direct-member+direct-member | 计时器创建失败阶段 | void | SetTimer失败；GetMessageW返回负值；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0864 | F0223 | F0396 | <code>海中鱼巣/界面/控制面板窗口.cpp:1578</code> | resolved-win32-callback |  |  | 窗口过程已由注册窗口类在 line 435 登记为 WNDPROC；消息循环调用 DispatchMessageW 后由 Win32 同步调度 | 专项源码静态类型与实际装配人工复核 |
| E0862 | F0223 | F0394 | <code>海中鱼巣/界面/控制面板窗口.cpp:1587</code> | direct_member+unique_name+direct-member | this | void | 消息循环退出；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0863 | F0223 | F0395 | <code>海中鱼巣/界面/控制面板窗口.cpp:1588</code> | direct_member+unique_name+direct-member | this | void | 字体清理完成；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0865 | F0224 | F0395 | <code>海中鱼巣/界面/控制面板窗口.cpp:310</code> | direct_member+unique_name+direct-member | this | void | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0866 | F0224 | F0394 | <code>海中鱼巣/界面/控制面板窗口.cpp:311</code> | direct_member+unique_name+direct-member | this | void | 快捷键清理完成；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0867 | F0227 | F0230 | <code>海中鱼巣/生产运行期配置.数据.h:68</code> | direct_const_member |  |  | 配置比较在基础字段相等后读取左侧与右侧系统角色稳定键组 | 专项源码静态类型与实际装配人工复核 |
| E0952 | F0227 | F0460 | <code>海中鱼巣/生产运行期配置.数据.h:74</code> | defaulted_const_member | this=&amp;this-&gt;概念活动,右=&amp;右.概念活动 | 概念活动相等 | 前十一项配置比较均为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0869 | F0228 | F0230 | <code>海中鱼巣/领域/系统角色清单.数据.h:98</code> | direct_const_member+suffix_match | this | 键组 | 版本与四项数值前置均通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0870 | F0229 | F0231 | <code>海中鱼巣/领域/概念活动状态.数据.h:46</code> | direct_const_member | this | 键组 | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0871 | F0233 | F0397 | <code>海中鱼巣/核心/协调.结构事务.ixx:152</code> | callback_bind | 无捕获lambda转换函数指针 | 接线.取得共享许可 | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0872 | F0233 | F0398 | <code>海中鱼巣/核心/协调.结构事务.ixx:153</code> | callback_bind | 无捕获lambda转换函数指针 | 接线.取得独占许可 | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0873 | F0233 | F0399 | <code>海中鱼巣/核心/协调.结构事务.ixx:154</code> | callback_bind | 无捕获lambda转换函数指针 | 接线.验证共享路径令牌 | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0874 | F0233 | F0400 | <code>海中鱼巣/核心/协调.结构事务.ixx:155</code> | callback_bind | 无捕获lambda转换函数指针 | 接线.验证独占令牌 | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0875 | F0233 | F0401 | <code>海中鱼巣/核心/协调.结构事务.ixx:156</code> | callback_bind | 无捕获lambda转换函数指针 | 接线.标记撤销失败隔离 | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0910 | F0238 | F0432 | <code>海中鱼巣/装配.运行期业务.ixx:100-101</code> | direct_constructor | 任务服务_,方法服务_,状态动态组合器_,稳定动作键 | 任务执行组合器_ | P31完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0911 | F0238 | F0433 | <code>海中鱼巣/装配.运行期业务.ixx:102-103</code> | direct_constructor | 需求服务_,任务服务_,方法服务_,状态服务_,动态服务_ | 任务结果结算组合器_ | P32完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0912 | F0238 | F0434 | <code>海中鱼巣/装配.运行期业务.ixx:104-106</code> | direct_constructor | 概念活动服务_,语素服务_,任务服务_,方法服务_,状态服务_,动态服务_ | 运行期只读查询组合器_ | P33完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0913 | F0238 | F0435 | <code>海中鱼巣/装配.运行期业务.ixx:107-111</code> | direct_constructor | 十四个已构造服务或组合器引用 | 系统角色初始化器_ | P34完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0914 | F0238 | F0436 | <code>海中鱼巣/装配.运行期业务.ixx:112-117</code> | direct_constructor | 八个组合器与四个业务服务引用 | 业务操作_ | P35完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0879 | F0238 | F0145 | <code>海中鱼巣/装配.运行期业务.ixx:65</code> | direct_constructor | 主信息,节点,接线副本01 | 特征值_ | 构造函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0880 | F0238 | F0402 | <code>海中鱼巣/装配.运行期业务.ixx:66</code> | direct_constructor | 接线副本02,四仓库,关系仓库编号 | 存在场景数据操作_ | P01完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0881 | F0238 | F0403 | <code>海中鱼巣/装配.运行期业务.ixx:67</code> | direct_constructor | 接线副本03,四仓库,关系仓库编号 | 状态动态数据操作_ | P02完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0882 | F0238 | F0404 | <code>海中鱼巣/装配.运行期业务.ixx:68</code> | direct_constructor | 接线副本04,四仓库,特征值_,关系仓库编号 | 特征体系数据操作_ | P03完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0883 | F0238 | F0405 | <code>海中鱼巣/装配.运行期业务.ixx:69</code> | direct_constructor | 接线副本05,四仓库,关系仓库编号 | 语素基础数据操作_ | P04完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0884 | F0238 | F0406 | <code>海中鱼巣/装配.运行期业务.ixx:70</code> | direct_constructor | 接线副本06,四仓库 | 轻量因果数据操作_ | P05完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0885 | F0238 | F0407 | <code>海中鱼巣/装配.运行期业务.ixx:71</code> | direct_constructor | 接线副本07,四仓库,关系仓库编号 | 概念图结构数据操作_ | P06完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0886 | F0238 | F0408 | <code>海中鱼巣/装配.运行期业务.ixx:72-74</code> | direct_constructor | 接线副本08,四仓库,关系仓库编号,存在场景数据操作_,状态动态数据操作_ | 需求任务方法数据操作_ | P07完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0887 | F0238 | F0409 | <code>海中鱼巣/装配.运行期业务.ixx:75-76</code> | direct_constructor | 接线副本09,四仓库,关系仓库编号 | 系统角色数据操作_ | P08完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0888 | F0238 | F0410 | <code>海中鱼巣/装配.运行期业务.ixx:77-78</code> | direct_constructor | 接线副本10,四仓库,关系仓库编号,状态动态数据操作_ | 概念活动数据操作_ | P09完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0889 | F0238 | F0411 | <code>海中鱼巣/装配.运行期业务.ixx:79</code> | direct_constructor | 存在场景数据操作_ | 存在服务_ | P10完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0890 | F0238 | F0412 | <code>海中鱼巣/装配.运行期业务.ixx:80</code> | direct_constructor | 存在场景数据操作_ | 场景服务_ | P11完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0891 | F0238 | F0413 | <code>海中鱼巣/装配.运行期业务.ixx:81</code> | direct_constructor | 状态动态数据操作_ | 状态服务_ | P12完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0892 | F0238 | F0414 | <code>海中鱼巣/装配.运行期业务.ixx:82</code> | direct_constructor | 状态服务_,概念活动数据操作_ | 概念活动服务_ | P13完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0893 | F0238 | F0415 | <code>海中鱼巣/装配.运行期业务.ixx:83</code> | direct_constructor | 状态服务_,状态动态数据操作_ | 动态服务_ | P14完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0894 | F0238 | F0416 | <code>海中鱼巣/装配.运行期业务.ixx:84</code> | direct_constructor | 特征体系数据操作_ | 特征服务_ | P15完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0895 | F0238 | F0417 | <code>海中鱼巣/装配.运行期业务.ixx:85</code> | direct_constructor | 特征体系数据操作_ | 二次特征服务_ | P16完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0896 | F0238 | F0418 | <code>海中鱼巣/装配.运行期业务.ixx:86</code> | direct_constructor | 语素基础数据操作_ | 基础信息服务_ | P17完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0897 | F0238 | F0419 | <code>海中鱼巣/装配.运行期业务.ixx:87</code> | direct_constructor | 语素基础数据操作_ | 语素服务_ | P18完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0898 | F0238 | F0420 | <code>海中鱼巣/装配.运行期业务.ixx:88</code> | direct_constructor | 动态服务_,轻量因果数据操作_ | 轻量因果服务_ | P19完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0899 | F0238 | F0421 | <code>海中鱼巣/装配.运行期业务.ixx:89</code> | direct_constructor | 概念图结构数据操作_ | 概念图结构服务_ | P20完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0900 | F0238 | F0422 | <code>海中鱼巣/装配.运行期业务.ixx:90</code> | direct_constructor | 需求任务方法数据操作_,状态服务_ | 需求服务_ | P21完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0901 | F0238 | F0423 | <code>海中鱼巣/装配.运行期业务.ixx:91</code> | direct_constructor | 需求任务方法数据操作_,需求服务_,存在服务_,状态服务_ | 任务服务_ | P22完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0902 | F0238 | F0424 | <code>海中鱼巣/装配.运行期业务.ixx:92</code> | direct_constructor | 需求任务方法数据操作_,存在服务_,状态服务_ | 方法服务_ | P23完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0903 | F0238 | F0425 | <code>海中鱼巣/装配.运行期业务.ixx:93</code> | direct_constructor | 需求服务_,任务服务_,方法服务_,特征服务_,状态服务_ | 方法召回服务_ | P24完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0904 | F0238 | F0426 | <code>海中鱼巣/装配.运行期业务.ixx:94</code> | direct_constructor | 存在场景数据操作_ | 存在场景组合器_ | P25完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0905 | F0238 | F0427 | <code>海中鱼巣/装配.运行期业务.ixx:95</code> | direct_constructor | 状态服务_,状态动态数据操作_ | 状态动态组合器_ | P26完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0906 | F0238 | F0428 | <code>海中鱼巣/装配.运行期业务.ixx:96</code> | direct_constructor | 特征服务_,特征体系数据操作_ | 特征状态组合器_ | P27完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0907 | F0238 | F0429 | <code>海中鱼巣/装配.运行期业务.ixx:97</code> | direct_constructor | 语素服务_,语素基础数据操作_ | 语素概念入口组合器_ | P28完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0908 | F0238 | F0430 | <code>海中鱼巣/装配.运行期业务.ixx:98</code> | direct_constructor | 概念图结构服务_ | 概念结构发布组合器_ | P29完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0909 | F0238 | F0431 | <code>海中鱼巣/装配.运行期业务.ixx:99</code> | direct_constructor | 需求服务_,任务服务_,方法召回服务_ | 需求任务方法组合器_ | P30完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0915 | F0239 | F0336 | <code>海中鱼巣/启动.运行期上下文.ixx:60</code> | direct_const_member+unique_name | this=&amp;接线_ | 已接域 | 仓库编号_非零；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0916 | F0239 | F0437 | <code>海中鱼巣/启动.运行期上下文.ixx:61</code> | direct_const_member+unique_name | this=&amp;协调器_ | 协调器域编号 | 已接域；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0917 | F0239 | F0438 | <code>海中鱼巣/启动.运行期上下文.ixx:62</code> | direct_const_member | this=&amp;节点_ | 节点仓库编号 | 协调器域编号等于接线域编号 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0918 | F0239 | F0397 | <code>海中鱼巣/启动.运行期上下文.ixx:65</code> | callback_invoke_resolved_normal | 接线_.运行期状态 | 许可 | 四项入口条件通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0919 | F0239 | F0338 | <code>海中鱼巣/启动.运行期上下文.ixx:66</code> | direct_const_member | this=&amp;许可 | 许可有效 | 许可构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0920 | F0239 | F0339 | <code>海中鱼巣/启动.运行期上下文.ixx:67, 海中鱼巣/启动.运行期上下文.ixx:68, 海中鱼巣/启动.运行期上下文.ixx:69, 海中鱼巣/启动.运行期上下文.ixx:70, 海中鱼巣/启动.运行期上下文.ixx:71</code> | direct_const_member+unique_name | this=&amp;许可 | 令牌引用01 | 许可有效；共享路径验证通过；主信息读取为空；节点读取为空；关系读取为空；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0921 | F0239 | F0399 | <code>海中鱼巣/启动.运行期上下文.ixx:67</code> | callback_invoke_resolved_normal | 接线_.运行期状态,令牌引用01 | 共享路径验证结果 | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0923 | F0239 | F0439 | <code>海中鱼巣/启动.运行期上下文.ixx:68</code> | direct_const_member | 空主信息句柄,令牌引用02 | 主信息读取结果 | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0925 | F0239 | F0346 | <code>海中鱼巣/启动.运行期上下文.ixx:69</code> | direct_const_member | 空节点句柄,令牌引用03 | 节点读取结果 | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0927 | F0239 | F0440 | <code>海中鱼巣/启动.运行期上下文.ixx:70</code> | direct_const_member | 空关系句柄,令牌引用04 | 关系读取结果 | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0930 | F0239 | F0345 | <code>海中鱼巣/启动.运行期上下文.ixx:71-72</code> | direct_destructor | this=&amp;许可 | 释放共享许可 | 返回表达式求值完成或异常离开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0929 | F0239 | F0441 | <code>海中鱼巣/启动.运行期上下文.ixx:71</code> | direct_const_member+unique_name | 主键0,令牌引用05 | 索引读取结果 | 令牌读取完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0931 | F0240 | F0442 | <code>海中鱼巣/装配.运行期业务.ixx:125</code> | direct_const_member | this=&amp;存在场景数据操作_ | 第一项完整条件 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0932 | F0240 | F0443 | <code>海中鱼巣/装配.运行期业务.ixx:126</code> | direct_const_member | this=&amp;状态动态数据操作_ | 第二项完整条件 | E0931返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0933 | F0240 | F0444 | <code>海中鱼巣/装配.运行期业务.ixx:127</code> | direct_const_member | this=&amp;特征体系数据操作_ | 第三项完整条件 | E0932返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0934 | F0240 | F0445 | <code>海中鱼巣/装配.运行期业务.ixx:128</code> | direct_const_member | this=&amp;语素基础数据操作_ | 第四项完整条件 | E0933返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0935 | F0240 | F0446 | <code>海中鱼巣/装配.运行期业务.ixx:129</code> | direct_const_member | this=&amp;轻量因果数据操作_ | 第五项完整条件 | E0934返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0936 | F0240 | F0447 | <code>海中鱼巣/装配.运行期业务.ixx:130</code> | direct_const_member | this=&amp;概念图结构数据操作_ | 第六项完整条件 | E0935返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0937 | F0240 | F0448 | <code>海中鱼巣/装配.运行期业务.ixx:131</code> | direct_const_member | this=&amp;需求任务方法数据操作_ | 第七项完整条件 | E0936返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0938 | F0240 | F0449 | <code>海中鱼巣/装配.运行期业务.ixx:132</code> | direct_const_member | this=&amp;系统角色数据操作_ | 第八项完整条件 | E0937返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0939 | F0240 | F0450 | <code>海中鱼巣/装配.运行期业务.ixx:133</code> | direct_const_member | this=&amp;概念活动数据操作_ | 第九项完整条件 | E0938返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0940 | F0240 | F0451 | <code>海中鱼巣/装配.运行期业务.ixx:134</code> | direct_const_member | this=&amp;概念活动服务_ | 第十项完整条件 | E0939返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0941 | F0240 | F0452 | <code>海中鱼巣/装配.运行期业务.ixx:135</code> | direct_const_member | this=&amp;系统角色初始化器_ | 第十一项完整条件 | E0940返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0942 | F0240 | F0453 | <code>海中鱼巣/装配.运行期业务.ixx:137</code> | direct_const_member | this=&amp;业务操作_ | 最终完整条件 | E0941返回true且稳定动作键_非零 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0944 | F0241 | F0228 | <code>海中鱼巣/领域/系统角色清单.数据.h:235</code> | direct_const_member | this=&amp;参数 | 参数有效 | E0943返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0943 | F0241 | F0245 | <code>海中鱼巣/领域/系统角色清单.数据.h:235</code> | direct_const_member | this=&amp;当前清单 | 清单完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0945 | F0242 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:153</code> | direct_const_member | this=&amp;当前运行期业务装配 | 装配完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0946 | F0242 | F0454 | <code>海中鱼巣/装配.运行期业务.ixx:153</code> | direct_const_member | this=&amp;系统角色初始化器_,清单=const只读借用 | 复核结果 | E0945返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0947 | F0243 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:149</code> | direct_const_member | this=&amp;当前运行期业务装配 | 装配完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0948 | F0243 | F0455 | <code>海中鱼巣/装配.运行期业务.ixx:149</code> | direct_member | this=&amp;系统角色初始化器_,参数=const只读借用 | 初始化结果 | E0947返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0949 | F0244 | F0456 | <code>海中鱼巣/领域/系统角色清单.数据.h:264</code> | implicit-defaulted-member+defaulted-member-comparison |  |  | 默认比较依次比较主体字段；系统角色清单默认等值比较逐成员比较主体材料 | 专项源码静态类型与实际装配人工复核 |
| E0950 | F0244 | F0457 | <code>海中鱼巣/领域/系统角色清单.数据.h:264</code> | implicit-defaulted-member+defaulted-member-comparison |  |  | 默认比较依次比较两项关系材料字段；系统角色清单默认等值比较逐成员比较两项系统角色关系材料 | 专项源码静态类型与实际装配人工复核 |
| E0953 | F0245 | F0458 | <code>海中鱼巣/领域/系统角色清单.数据.h:222</code> | direct_const_member | this=&amp;主体 | 主体完整 | 版本相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0954 | F0245 | F0459 | <code>海中鱼巣/领域/系统角色清单.数据.h:223</code> | direct_const_member | this=&amp;世界根到场景关系 | 第一关系完整 | E0953返回true；E0954返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0956 | F0245 | F0051 | <code>海中鱼巣/领域/系统角色清单.数据.h:225, 海中鱼巣/领域/系统角色清单.数据.h:226, 海中鱼巣/领域/系统角色清单.数据.h:229, 海中鱼巣/领域/系统角色清单.数据.h:230</code> | direct_free_operator | 世界根到场景关系.源节点,主体.世界根.节点 | 第一关系源端点相等 | E0955返回true且第一关系类型正确；E0956返回true；第一关系顺序号和第二关系类型正确；E0958返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0961 | F0247 | F0229 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | direct_const_member | this=&amp;参数 | 参数有效 | E0960返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0960 | F0247 | F0251 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | direct_const_member | this=&amp;当前材料 | 材料完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0962 | F0247 | F0460 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | defaulted_const_member | this=&amp;初始化参数,右=&amp;参数 | 参数相等 | E0961返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0963 | F0248 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:166</code> | direct_const_member | this=&amp;当前装配 | 装配完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0964 | F0248 | F0461 | <code>海中鱼巣/装配.运行期业务.ixx:166</code> | direct_const_member | this=&amp;概念活动服务_,系统角色,预期材料=&amp;材料 | 下层结果 | E0963返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0965 | F0249 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:159</code> | direct_const_member | this=&amp;当前装配 | 装配完整 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0966 | F0249 | F0462 | <code>海中鱼巣/装配.运行期业务.ixx:159</code> | direct_member | this=&amp;概念活动服务_,系统角色,参数 | 下层结果 | E0965返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0967 | F0250 | F0460 | <code>海中鱼巣/领域/概念活动状态.数据.h:188</code> | defaulted_const_member | this=&amp;初始化参数,右=&amp;右.初始化参数 | 参数相等 | 材料版本和活动版本相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0968 | F0250 | F0463 | <code>海中鱼巣/领域/概念活动状态.数据.h:188</code> | std_array_element_compare | this=&amp;状态角色组[元素],右=&amp;右.状态角色组[元素] | 状态角色组相等 | E0967返回true；由X0180按元素短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0969 | F0250 | F0464 | <code>海中鱼巣/领域/概念活动状态.数据.h:189</code> | direct_const_member | this=&amp;重建视图,右=&amp;右.重建视图 | 重建视图相等 | E0968聚合结果为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0970 | F0250 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:191</code> | direct_const_member | this=&amp;根组[索引],右=&amp;右.根组[索引] | 全部根材料相等 | E0969返回true；循环短路0..4次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0971 | F0251 | F0229 | <code>海中鱼巣/领域/概念活动状态.数据.h:162</code> | direct_const_member | this=&amp;初始化参数 | 初始化参数有效 | 材料版本正确且活动版本非零 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0972 | F0251 | F0466 | <code>海中鱼巣/领域/概念活动状态.数据.h:162</code> | direct_const_member | this=&amp;重建视图 | 重建视图完整 | E0971返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0973 | F0251 | F0463 | <code>海中鱼巣/领域/概念活动状态.数据.h:163</code> | std_array_element_compare | this=&amp;状态角色组[元素],右=&amp;重建视图.状态角色组[元素] | 两组状态角色相等 | E0972返回true；由X0182按元素短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0974 | F0251 | F0231 | <code>海中鱼巣/领域/概念活动状态.数据.h:164</code> | direct_const_member | this=&amp;初始化参数 | 稳定键组副本 | E0973聚合结果为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0975 | F0251 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:169</code> | direct_const_member | this=&amp;根组[索引],右=&amp;重建视图.根组[索引] | 全部根投影相等 | 三项稳定键一致；循环短路0..4次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0976 | F0251 | F0467 | <code>海中鱼巣/领域/概念活动状态.数据.h:173</code> | direct_const_member | this=&amp;当前状态角色 | 全部状态角色完整 | 根投影均相等；范围遍历短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0977 | F0253 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:84</code> | direct_const_member | this=&amp;业务装配_ | 根函数布尔返回 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0999 | F0254 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:101</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0978 | F0254 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:76</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0979 | F0254 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:77</code> | direct_const_member | this=&amp;环境 | 通过 | E0978返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0047 | F0254 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:83</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0048 | F0254 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:84</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0049 | F0254 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:85</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0980 | F0254 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:88</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0981 | F0254 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:93</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0982 | F0254 | F0476 | <code>海中鱼巣/自检.入口初始化.ixx:94, 海中鱼巣/自检.入口初始化.ixx:95, 海中鱼巣/自检.入口初始化.ixx:96, 海中鱼巣/自检.入口初始化.ixx:97</code> | local_lambda_direct | 存在根,节点类型::存在 | 存在根类型匹配 | E0981返回true；E0982返回true；E0983返回true；E0984返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1000 | F0255 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:105</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1001 | F0255 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:106</code> | direct_const_member | this=&amp;环境 | 通过 | E1000返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0050 | F0255 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:112</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0051 | F0255 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:113</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0052 | F0255 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:114</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1002 | F0255 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:117</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1003 | F0255 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:119</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1004 | F0255 | F0355 | <code>海中鱼巣/自检.入口初始化.ixx:119</code> | direct_const_member+unique_name | 上下文.概念图隐式this | 根登记组 | E1003返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1017 | F0255 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:129</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1018 | F0256 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:133</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1019 | F0256 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:134</code> | direct_const_member | this=&amp;环境 | 通过 | E1018返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0053 | F0256 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:140</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0054 | F0256 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:141</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0055 | F0256 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:142</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1020 | F0256 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:145</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1021 | F0256 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:146, 海中鱼巣/自检.入口初始化.ixx:151</code> | direct_const_member | this=&amp;结果 | 别名分支初始化成功 | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1022 | F0256 | F0364 | <code>海中鱼巣/自检.入口初始化.ixx:147, 海中鱼巣/自检.入口初始化.ixx:147-148</code> | direct_member+unique_name | L实体,存在根.根节点,存在根.根节点；第4形参主键采用默认值0 | 别名 | E1021返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1023 | F0256 | F0352 | <code>海中鱼巣/自检.入口初始化.ixx:150</code> | direct_const_member+unique_name | 概念根类别::存在 | 读回 | 环境成功；不依赖E1021/E1022结果；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1025 | F0256 | F0363 | <code>海中鱼巣/自检.入口初始化.ixx:151</code> | direct_const_member | this=&amp;别名 | 别名成功 | E1024返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1026 | F0256 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:153</code> | direct_free_operator | 读回.根节点,结果.概念图.存在根.根节点 | 节点句柄相等 | 读回有值且前项均true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1039 | F0256 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:158</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1040 | F0257 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:162</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1041 | F0257 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:163</code> | direct_const_member | this=&amp;环境 | 通过 | E1040返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0056 | F0257 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:169</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0057 | F0257 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:170</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0058 | F0257 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:171</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1042 | F0257 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:174</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0059 | F0257 | F0496 | <code>海中鱼巣/自检.入口初始化.ixx:176</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0060 | F0257 | F0497 | <code>海中鱼巣/自检.入口初始化.ixx:177</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1043 | F0257 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:181</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1044 | F0257 | F0495 | <code>海中鱼巣/自检.入口初始化.ixx:186, 海中鱼巣/自检.入口初始化.ixx:187</code> | local_lambda_direct | 存在根 | 绑定可读 | E1043及四个预置名称比较均为true；E1044返回true；E1045返回true；E1046返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1064 | F0257 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:191</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1065 | F0258 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:195</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1066 | F0258 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:196</code> | direct_const_member | this=&amp;环境 | 通过 | E1065返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0061 | F0258 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:202</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0062 | F0258 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:203</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1071 | F0258 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:204, 海中鱼巣/自检.入口初始化.ixx:211</code> | direct_member+unique_name | this=&amp;上下文.概念图初始化 | 再次 | 环境成功；不依赖首次成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1067 | F0258 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:207</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 首次 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1068 | F0258 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:208, 海中鱼巣/自检.入口初始化.ixx:217</code> | direct_const_member | this=&amp;上下文.节点 | 前节点 | 环境成功；不依赖首次成功；E1077返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1069 | F0258 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:209, 海中鱼巣/自检.入口初始化.ixx:218</code> | direct_const_member | this=&amp;上下文.关系 | 前关系 | E1068返回；前节点等于后节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0063 | F0258 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:209, 海中鱼巣/自检.入口初始化.ixx:218</code> | unique_name | this=&amp;上下文.关系 | std::uint64_t | 当前可达调用点为零实参有效关系数量重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| E1070 | F0258 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:210, 海中鱼巣/自检.入口初始化.ixx:219</code> | direct_const_member | this=&amp;上下文.索引 | 前索引 | E1069返回；前关系等于后关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1072 | F0258 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:212</code> | direct_const_member | this=&amp;首次 | 首次成功 | E1071返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1073 | F0258 | F0049 | <code>海中鱼巣/自检.入口初始化.ixx:212</code> | direct_const_member | this=&amp;再次 | 再次成功 | E1072返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1074 | F0258 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:213, 海中鱼巣/自检.入口初始化.ixx:214, 海中鱼巣/自检.入口初始化.ixx:215, 海中鱼巣/自检.入口初始化.ixx:216</code> | direct_operator | 再次.存在根.根节点,首次.概念图.存在根.根节点 | 句柄相等 | E1073返回true；E1074返回true；E1075返回true；E1076返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1093 | F0258 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:223</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1094 | F0259 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:227</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1095 | F0259 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:228</code> | direct_const_member | this=&amp;环境 | 通过 | E1094返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0065 | F0259 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:234</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0066 | F0259 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:235</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0067 | F0259 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:236</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1096 | F0259 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:239</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1097 | F0259 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:240</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1098 | F0259 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:241</code> | std_optional_element_compare | 结果.自我存在根支持.value(),结果.概念图.存在根.根节点 | 句柄相等 | E1097返回true且两侧optional均有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1099 | F0259 | F0371 | <code>海中鱼巣/自检.入口初始化.ixx:242, 海中鱼巣/自检.入口初始化.ixx:242-243</code> | direct_const_member+unique_name | this=&amp;上下文.概念图,结果.自我初始化-&gt;世界树.自我存在节点 | 支持概念组 | E1098返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1112 | F0259 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:247</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1113 | F0260 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:251</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1114 | F0260 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:252</code> | direct_const_member | this=&amp;环境 | 通过 | E1113返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0068 | F0260 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:258</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0069 | F0260 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:259</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0070 | F0260 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:260</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1115 | F0260 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:263</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1116 | F0260 | F0516 | <code>海中鱼巣/自检.入口初始化.ixx:264</code> | direct_member | this=&amp;上下文.存在 | 第二实例 | 环境成功；不依赖E1115成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1117 | F0260 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:265</code> | direct_member | this=&amp;上下文.概念图,第二实例 | 支持 | E1116返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1118 | F0260 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:266</code> | direct_const_member | this=&amp;结果 | 初始化成功 | E1117返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1119 | F0260 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:266</code> | std_optional_element_compare | 支持.value(),结果.概念图.存在根.根节点 | 句柄相等 | E1118返回true且支持有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1120 | F0260 | F0372 | <code>海中鱼巣/自检.入口初始化.ixx:267</code> | direct_const_member+unique_name | this=&amp;上下文.概念图,结果.概念图.存在根.根节点 | 实例组 | E1119返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1121 | F0260 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:271</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1134 | F0261 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:275</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1135 | F0261 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:276</code> | direct_const_member | this=&amp;环境 | 通过 | E1134返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0071 | F0261 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:282</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0072 | F0261 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:283</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0073 | F0261 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:284</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1136 | F0261 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:287</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1137 | F0261 | F0516 | <code>海中鱼巣/自检.入口初始化.ixx:288</code> | direct_member | this=&amp;上下文.存在 | 已删除 | 环境成功；不依赖E1136成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1138 | F0261 | F0523 | <code>海中鱼巣/自检.入口初始化.ixx:289</code> | direct_member | this=&amp;上下文.节点,已删除 | 已删除准备 | E1137返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0074 | F0261 | R0147 | <code>海中鱼巣/自检.入口初始化.ixx:289</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1139 | F0261 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:290, 海中鱼巣/自检.入口初始化.ixx:298</code> | direct_const_member | this=&amp;上下文.节点 | 前节点 | E1138返回；E1145返回true且三个拒绝optional均为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1140 | F0261 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:291, 海中鱼巣/自检.入口初始化.ixx:299</code> | direct_const_member | this=&amp;上下文.关系 | 前关系 | E1139返回；E1146等于前节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0075 | F0261 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:291, 海中鱼巣/自检.入口初始化.ixx:299</code> | unique_name | this=&amp;上下文.关系 | std::uint64_t | 当前可达调用点为零实参有效关系数量重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| E1141 | F0261 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:292, 海中鱼巣/自检.入口初始化.ixx:300</code> | direct_const_member | this=&amp;上下文.索引 | 前索引 | E1140返回；E1147等于前关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1142 | F0261 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:293, 海中鱼巣/自检.入口初始化.ixx:294, 海中鱼巣/自检.入口初始化.ixx:295</code> | direct_member | this=&amp;上下文.概念图,结果.概念图.存在根.根节点 | 根拒绝 | E1141返回；E1142返回；E1143返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1145 | F0261 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:296</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 三次拒绝调用均已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1149 | F0261 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:304</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1162 | F0262 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:308</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1163 | F0262 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:309</code> | direct_const_member | this=&amp;环境 | 通过 | E1162返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0077 | F0262 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:315</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0078 | F0262 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:316</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0079 | F0262 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:317</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1164 | F0262 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:320</code> | direct_free+unique_name | 端口,{方法登记根稳定键,根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1165 | F0262 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:321</code> | direct_const_member | this=&amp;结果 | 初始化成功 | E1164返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1166 | F0262 | F0530 | <code>海中鱼巣/自检.入口初始化.ixx:321</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 已进入 | E1165返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1167 | F0262 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:322</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 线程状态 | E1166返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1168 | F0262 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:323</code> | direct_const_member | this=&amp;上下文.节点 | 节点数量 | E1167返回运行中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1169 | F0262 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:324</code> | direct_const_member | this=&amp;上下文.关系 | 关系数量 | E1168大于初始节点数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0080 | F0262 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:324</code> | unique_name | this=&amp;上下文.关系 | std::uint64_t | 当前可达调用点为零实参有效关系数量重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| E1170 | F0262 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:325</code> | direct_const_member | this=&amp;上下文.索引 | 索引数量 | E1169大于初始关系数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1171 | F0262 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:329</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1184 | F0263 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:333</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1185 | F0263 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:334</code> | direct_const_member | this=&amp;环境 | 通过 | E1184返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0082 | F0263 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:340</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0083 | F0263 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:341</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0084 | F0263 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:342</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1186 | F0263 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:345</code> | direct_free+unique_name | 端口,{默认方法登记根稳定键,默认根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1187 | F0263 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:347, 海中鱼巣/自检.入口初始化.ixx:350</code> | direct_const_member | this=&amp;结果 | 坐标读取门 | E1186返回且E1186结果已在345行无保护value访问；坐标optional已经形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1188 | F0263 | F0059 | <code>海中鱼巣/自检.入口初始化.ixx:348</code> | direct_const_member+unique_name | this=&amp;上下文.世界树初始化,快照.世界树.自我存在节点 | 坐标 | E1187返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1190 | F0263 | F0173 | <code>海中鱼巣/自检.入口初始化.ixx:350</code> | direct_const_member | this=&amp;快照.语素 | 语素成功 | E1189返回true且初始化代次等于1 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1191 | F0263 | F0174 | <code>海中鱼巣/自检.入口初始化.ixx:355</code> | direct_const_member | this=&amp;快照.世界树 | 世界树成功 | E1190及四个名称比较均为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1192 | F0263 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:360</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1205 | F0264 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:364</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0085 | F0264 | F0189 | <code>海中鱼巣/自检.入口初始化.ixx:365, 海中鱼巣/自检.入口初始化.ixx:381</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1206 | F0264 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:365</code> | direct_const_member | this=&amp;环境 | 通过 | E1205返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0086 | F0264 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:372</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0087 | F0264 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:373</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0088 | F0264 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:374</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1207 | F0264 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:377</code> | direct_free+unique_name | 端口,{固定配置.方法登记根稳定键,固定配置.根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0089 | F0264 | F0441 | <code>海中鱼巣/自检.入口初始化.ixx:379, 海中鱼巣/自检.入口初始化.ixx:380</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1208 | F0264 | F0544 | <code>海中鱼巣/自检.入口初始化.ixx:379, 海中鱼巣/自检.入口初始化.ixx:380</code> | direct_const_member | this=&amp;上下文.索引,固定配置.根需求参数.安全值语素稳定键 | 安全索引 | E1207返回且377行已无保护解引用自我初始化；E1208返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1210 | F0264 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:381</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 两次索引读取已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1211 | F0264 | F0175 | <code>海中鱼巣/自检.入口初始化.ixx:381</code> | direct_const_member | this=&amp;根 | 根需求初始化成功 | E1210返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1212 | F0264 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:382, 海中鱼巣/自检.入口初始化.ixx:383</code> | std_optional_element_compare | 安全索引.value(),根.安全根需求.语素入口结果.语素入口 | 安全句柄相等 | E1211返回true且optional两侧有值；E1212返回true且optional两侧有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1214 | F0264 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:387</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1227 | F0265 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:391</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0090 | F0265 | F0350 | <code>海中鱼巣/自检.入口初始化.ixx:392, 海中鱼巣/自检.入口初始化.ixx:407, 海中鱼巣/自检.入口初始化.ixx:418</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1228 | F0265 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:392</code> | direct_const_member | this=&amp;环境 | 通过 | E1227返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0091 | F0265 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:398</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0092 | F0265 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:399</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0093 | F0265 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:400</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1229 | F0265 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:403</code> | direct_free+unique_name | 端口,{默认方法登记根稳定键,默认根需求参数} | 结果 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0094 | F0265 | F0552 | <code>海中鱼巣/自检.入口初始化.ixx:406</code> | source-audited-direct | this=&amp;上下文.需求；需求节点=单根.根需求 | 需求承接材料可选值 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE0095 | F0265 | F0553 | <code>海中鱼巣/自检.入口初始化.ixx:408</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0096 | F0265 | F0554 | <code>海中鱼巣/自检.入口初始化.ixx:410</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0097 | F0265 | F0555 | <code>海中鱼巣/自检.入口初始化.ixx:413</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1230 | F0265 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:418</code> | direct_const_member | this=&amp;结果 | 初始化成功 | 403行已无保护解引用自我初始化且F0551已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1231 | F0265 | F0551 | <code>海中鱼巣/自检.入口初始化.ixx:418, 海中鱼巣/自检.入口初始化.ixx:419</code> | local_lambda_direct | 根.安全根需求,1,10 | 安全根完整 | E1230返回true；E1231返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1233 | F0265 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:420</code> | rewritten_not_equal | 根.安全根需求.根需求,根.服务根需求.根需求 | 句柄相等后取反 | E1232返回true；C++20把!=重写为==后取反 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1234 | F0265 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:424</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1255 | F0266 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:428</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1256 | F0266 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:429</code> | direct_const_member | this=&amp;环境 | 通过 | E1255返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0098 | F0266 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:436</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0099 | F0266 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:437</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0100 | F0266 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:438</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1257 | F0266 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:441</code> | direct_free+unique_name | 端口,{固定配置.方法登记根稳定键,固定配置.根需求参数} | 首次 | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1258 | F0266 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:442, 海中鱼巣/自检.入口初始化.ixx:449</code> | direct_const_member | this=&amp;上下文.节点 | 前节点 | E1257返回，不要求成功；E1263等于运行中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1259 | F0266 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:443, 海中鱼巣/自检.入口初始化.ixx:450</code> | direct_const_member | this=&amp;上下文.关系 | 前关系 | E1258返回；E1264等于前节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0101 | F0266 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:443, 海中鱼巣/自检.入口初始化.ixx:450</code> | unique_name | this=&amp;上下文.关系 | std::uint64_t | 当前可达调用点为零实参有效关系数量重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| E1260 | F0266 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:444, 海中鱼巣/自检.入口初始化.ixx:451</code> | direct_const_member | this=&amp;上下文.索引 | 前索引 | E1259返回；E1265等于前关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1261 | F0266 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:445</code> | direct_member | this=&amp;上下文.自我线程实例,固定配置.根需求参数 | 再次 | E1260返回，不要求首次成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1262 | F0266 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:446</code> | direct_const_member | this=&amp;首次 | 首次成功 | E1261返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1263 | F0266 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:448</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 当前状态 | 首次成功且再次成功、复用并拒绝原因为重复启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1267 | F0266 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:455</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1280 | F0267 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:460</code> | direct_free | 配置,编号 | 环境 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1281 | F0267 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:461</code> | direct_const_member | this=&amp;环境 | 通过 | E1280返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1282 | F0267 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:465-466, 海中鱼巣/自检.入口初始化.ixx:467-469</code> | direct_member | this=&amp;上下文.自我线程实例,{0,服务值语素稳定键,1,10,1,10} | 零键 | 环境成功；E1282返回后无条件执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1284 | F0267 | F0530 | <code>海中鱼巣/自检.入口初始化.ixx:472</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 已进入 | 两个拒绝结果均完全符合 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1285 | F0267 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:473</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 当前状态 | E1284返回false | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1286 | F0267 | F0562 | <code>海中鱼巣/自检.入口初始化.ixx:474</code> | direct_const_member | this=&amp;上下文.自我线程实例 | 可收口 | E1285等于未启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1287 | F0267 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:475</code> | direct_const_member | this=&amp;上下文.节点 | 节点数量 | E1286返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1288 | F0267 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:476</code> | direct_const_member | this=&amp;上下文.关系 | 关系数量 | E1287等于环境初始节点数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0103 | F0267 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:476</code> | unique_name | this=&amp;上下文.关系 | std::uint64_t | 当前可达调用点为零实参有效关系数量重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| E1289 | F0267 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:477</code> | direct_const_member | this=&amp;上下文.索引 | 索引数量 | E1288等于环境初始关系数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1290 | F0267 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:481</code> | compiler_implicit_member_destructor | this=&amp;环境.上下文-&gt;自我线程实例 |  | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1291 | F0268 | F0563 | <code>海中鱼巣/核心/日志系统.h:101, 海中鱼巣/核心/日志系统.h:104, 海中鱼巣/核心/日志系统.h:107, 海中鱼巣/核心/日志系统.h:109, 海中鱼巣/核心/日志系统.h:88, 海中鱼巣/核心/日志系统.h:90, 海中鱼巣/核心/日志系统.h:92, 海中鱼巣/核心/日志系统.h:98</code> | direct_free+lsp_direct |  | 根路径临时值 | 类别为逻辑错误；类别为运行；类别为事件；切片合法且等于SELF_TEST_ENTRY_INITIALIZATION；切片合法且等于SELF_TEST_DATABASE；切片合法且等于SELF_TEST_WAREHOUSE_PERFORMANCE；切片合法且等于SELF_TEST_D455；切片合法但不等于四个预置名；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E1294 | F0268 | F0564 | <code>海中鱼巣/核心/日志系统.h:94</code> | direct_free+lsp_direct | 调试切片名 | 可用 | 类别为调试；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E1300 | F0272 | F0135 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:148</code> | member_constructor | 790000+有效关系规模,默认结构事务接线 | 主信息_ | 规模和随机状态成员已初始化 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1301 | F0272 | F0136 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:149</code> | member_constructor | 主信息_,791000+有效关系规模,默认结构事务接线 | 节点_ | 主信息_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1302 | F0272 | F0137 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:150</code> | member_constructor | 节点_,792000+有效关系规模,默认结构事务接线 | 关系_ | 节点_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1303 | F0273 | F0331 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:161</code> | direct_member_nonvirtual | this=&amp;主信息_ | 共用主信息 | 规模准入通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1312 | F0273 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:162, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:166, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:180</code> | direct_free+unique_name | E1311返回关系 | 句柄有效 | 添加关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1304 | F0273 | F0565 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:162</code> | direct_free | 共用主信息 | 句柄有效 | 主信息创建返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1305 | F0273 | F0332 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:165</code> | direct_member_nonvirtual | 基础信息,共用主信息 | 节点 | 节点循环每轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1306 | F0273 | F0163 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:166</code> | direct_free | 节点 | 句柄有效 | 节点创建返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1307 | F0273 | F0566 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:170</code> | direct_member_nonvirtual+unique_name | this | 固定结构成功 | 1024个节点全部形成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1308 | F0273 | F0567 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:176, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:177, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:179</code> | direct_member_nonvirtual+unique_name | this | 源随机值 | 随机填充循环每次尝试；源随机值形成；源目标索引不同；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1311 | F0273 | F0568 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:180</code> | direct_member_nonvirtual+unique_name | 引用,源索引,目标索引,顺序号,true | 关系句柄 | 非自环随机候选；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1313 | F0273 | F0569 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:186</code> | direct_member_nonvirtual+unique_name | this | 固定变更成功 | 参考表数量达到目标；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1314 | F0273 | F0198 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:188</code> | direct_const_member | this=&amp;关系_ | 有效关系数量 | 固定变更成功并写摘要 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1315 | F0273 | F0277 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:189</code> | direct_const_member+unique_name | this | 结构边界完整 | 有效关系数量等于规模；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1316 | F0273 | F0283 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:190</code> | direct_const_member+unique_name | this | 参考表当前状态一致 | 有效关系数量等于有效关系规模_且验证结构边界返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1317 | F0274 | F0570 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:208, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:209, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:210, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:211, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:212, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:213, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:214</code> | local_lambda_direct | 记录.关系编号 |  | 每条排序记录；E1317完成；E1318完成；E1319完成；E1320完成；E1321完成；E1322完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1324 | F0276 | F0278 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:220</code> | direct_const_member+unique_name | this | 查询组 | F0276被调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1325 | F0277 | F0571 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:235, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:236</code> | direct_const_member+unique_name | 节点组_[1002],关系类型::普通父子,false | 普通父组 | F0277被调用；E1325成功返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1327 | F0277 | F0572 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:237, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:238</code> | direct_const_member+unique_name | 节点组_[996],关系类型::概念上下位,false | 第一源关系组 | E1326成功返回；E1327结果非空；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1329 | F0277 | F0573 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:239, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:240, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:241</code> | direct_const_member+unique_name | 关系类型::概念上下位,节点组_[993],节点组_[994] | 第一环边存在 | 两棵根布尔值已形成；E1329返回true；E1329与E1330均返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1332 | F0278 | F0572 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:275, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:277, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:291, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:293</code> | direct_const_member+unique_name | 节点组_[1010],关系类型::引用,false | 0号预期记录组 | 构造正向稀疏；0号构造完成；7号构造完成；8号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1334 | F0278 | F0571 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:279, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:281, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:285</code> | direct_const_member+unique_name | 节点组_[1013],关系类型::引用,false | 2号预期记录组 | 1号构造完成；2号构造完成；4号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1336 | F0278 | F0573 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:283</code> | direct_const_member+unique_name | 关系类型::引用,节点组_[1010],节点组_[1011] | 4号预期存在 | 3号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1338 | F0278 | F0574 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:287, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:289</code> | direct_const_member+unique_name | 节点组_[1014] | 6号预期记录组 | 5号构造完成；6号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1342 | F0279 | F0575 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:299</code> | direct_const_member | 定义.源节点,定义.类型 | 实际记录组 | 正向记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1343 | F0279 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:301, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:306, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:311, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:316, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:321, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:325</code> | direct_free | 结束-开始 | 耗时纳秒 | 正向查询返回；反向查询返回；精确存在查询返回；目标存在查询返回；节点相关查询返回；审计查询返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1344 | F0279 | F0577 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:301, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:306, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:321, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:325</code> | direct_free | 实际,定义.预期记录组 | 正确 | 正向查询返回；反向查询返回；节点相关查询返回；审计查询返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1345 | F0279 | F0578 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:304</code> | direct_const_member | 定义.目标节点,定义.类型 | 实际记录组 | 反向记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1348 | F0279 | F0579 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:309</code> | direct_const_member | 定义.类型,定义.源节点,定义.目标节点 | 实际存在 | 精确存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1350 | F0279 | F0580 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:314</code> | direct_const_member | 定义.类型,定义.目标节点 | 实际存在 | 目标存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1352 | F0279 | F0581 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:319</code> | direct_const_member | 定义.源节点 | 实际记录组 | 节点相关 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1355 | F0279 | F0582 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:323</code> | direct_const_member | 定义.源节点,定义.类型 | 实际记录组 | 当前审计或未声明值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1358 | F0281 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:107, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:108, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:109</code> | direct_free | 样本,0.50 | 分位指标.中位数纳秒 | 名称、样本数量和候选上界字段已初始化；E1358正常返回；E1359正常返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1361 | F0282 | F0583 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:644</code> | direct_const_member+unique_name | this=&amp;夹具 | 写材料组const借用 | 进入并发基线；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1362 | F0282 | F0584 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:658-687</code> | thread_callback | 线程索引按值；写材料组、线程读取样本、开始、窗口结束、正确、夹具、密集正向、完成操作数量、最终关系组按引用捕获 | void异步回调 | 每次std::thread构造成功后异步调度 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1363 | F0282 | F0585 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:695</code> | direct_member_nonvirtual+unique_name | 索引,最终关系组[索引] | 本项更新正确 | 全部工作线程已join；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1364 | F0282 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:702</code> | direct_free | 合并样本,0.50 | 并发中位数 | 线程样本合并完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1368 | F0283 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:254, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:262</code> | direct_const_member | 当前句柄 | 实际optional | 参考记录状态有效；参考记录状态已删除 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1369 | F0283 | F0588 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:255, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:259</code> | direct_free | *实际,记录 | 字段一致 | 有效记录普通读取有值；失效记录审计读取有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1370 | F0283 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:258, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:263</code> | direct_const_member | 当前句柄 | 实际optional | 参考记录状态已失效；删除记录普通读取无值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1374 | F0284 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:347-348</code> | direct_member_nonvirtual | 关系类型::运行期临时,节点组_[940],节点组_[941],1 | 新关系 | 每轮开始时间已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1375 | F0284 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:350</code> | direct_free+unique_name | 新关系 | 句柄有效位 | 累计正确仍为true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1376 | F0284 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:350, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:351</code> | direct_const_member | 新关系 | 创建后记录optional | 累计正确且句柄有效；累计正确且删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1377 | F0284 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:351</code> | direct_member_nonvirtual | 新关系 | 已删除 | 创建后累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1379 | F0284 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:352</code> | direct_free | 结束-开始 | 创建耗时纳秒 | 正式轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1380 | F0284 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:354</code> | direct_free | "创建",样本,初始当前记录数量_+样本.size(),正确 | 返回分位指标 | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1381 | F0285 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:382-383</code> | direct_member_nonvirtual | 关系类型::运行期临时,节点组_[940],节点组_[941],3 | 新关系 | 进入当前循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1382 | F0285 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:384</code> | direct_free+unique_name | 新关系 | 句柄有效位 | 创建关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1383 | F0285 | F0591 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:389</code> | direct_member_nonvirtual | 新关系 | 失效结果 | 新关系句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1384 | F0285 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:391</code> | direct_const_member | 失效结果.当前关系 | 审计optional | 失效入口返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1385 | F0285 | F0592 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:392</code> | direct_const_member | this=&amp;失效结果 | 完整位 | 累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1386 | F0285 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:394</code> | direct_const_member | 新关系 | 旧句柄普通读取optional | 累计正确、结果完整且审计状态已失效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1387 | F0285 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:395</code> | direct_free | 结束-开始 | 失效耗时纳秒 | 正式轮且创建成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1388 | F0285 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:397</code> | direct_free | "失效",样本,初始当前记录数量_+660,正确 | 返回分位指标 | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1389 | F0286 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:404-405</code> | direct_member_nonvirtual | 关系类型::因果来源,节点组_[942],节点组_[943],4 | 当前关系 | 进入重挂测量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1390 | F0286 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:410</code> | direct_member_nonvirtual | 当前关系,节点组_[942],新目标 | 新关系optional | 当前循环计时已开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1391 | F0286 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:416, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:417</code> | direct_const_member | *新关系 | 新记录optional | 重挂返回有值；重挂成功且累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1393 | F0286 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:418</code> | direct_free | 记录-&gt;目标节点,新目标 | 目标相等位 | 旧记录为空且新记录存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1394 | F0286 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:421</code> | direct_free | 结束-开始 | 重挂耗时纳秒 | 正式轮且重挂成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1395 | F0286 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:423</code> | direct_member_nonvirtual | 当前关系 | 删除成功位 | 全部循环结束且累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1396 | F0286 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:424</code> | direct_free | "重挂",样本,初始当前记录数量_+661,正确 | 返回分位指标 | 最终删除短路完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1397 | F0287 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:362-363</code> | direct_member_nonvirtual | 关系类型::运行期临时,节点组_[940],节点组_[941],2 | 新关系 | 进入当前循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1398 | F0287 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:364</code> | direct_free+unique_name | 新关系 | 句柄有效位 | 创建关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1399 | F0287 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:369</code> | direct_member_nonvirtual | 新关系 | 已删除 | 新关系句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1400 | F0287 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:371</code> | direct_const_member | 新关系 | 删除后记录optional | 累计正确且删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1401 | F0287 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:372</code> | direct_free | 结束-开始 | 删除耗时纳秒 | 正式轮且创建成功；不要求删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1402 | F0287 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:374</code> | direct_free | "删除",样本,初始当前记录数量_+440,正确 | 返回分位指标 | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1403 | F0291 | F0593 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:742</code> | direct_const_member | this=&amp;指标 | 查询指标完整位 | F0291回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1404 | F0292 | F0593 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:745</code> | direct_const_member | this=&amp;指标 | 写入指标完整位 | F0292回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1639 | F0321 | F0336 | <code>海中鱼巣/核心/结构事务接线.数据.h:83</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1638 | F0321 | F0618 | <code>海中鱼巣/核心/结构事务接线.数据.h:83</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1640 | F0322 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:17, 海中鱼巣/核心/节点仓库.cpp:18</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1644 | F0323 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:82, 海中鱼巣/核心/关系仓库.cpp:83</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1648 | F0324 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:13, 海中鱼巣/核心/索引仓库.cpp:14</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1652 | F0326 | F0163 | <code>海中鱼巣/领域/方法服务.h:139, 海中鱼巣/领域/方法服务.h:140, 海中鱼巣/领域/方法服务.h:141</code> | direct_free | 登记根 | 登记根句柄有效bool | 稳定非名称键非零；登记根句柄有效；活跃状态句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0105 | F0326 | F0168 | <code>海中鱼巣/领域/方法服务.h:139, 海中鱼巣/领域/方法服务.h:140, 海中鱼巣/领域/方法服务.h:141</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1655 | F0326 | F0051 | <code>海中鱼巣/领域/方法服务.h:142, 海中鱼巣/领域/方法服务.h:143, 海中鱼巣/领域/方法服务.h:144</code> | direct_free_operator | 登记根,活跃状态 | 登记根与活跃状态相等bool并取反 | 三个句柄均有效；登记根与活跃状态不等；登记根分别与两个状态不等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1658 | F0327 | F0333 | <code>海中鱼巣/领域/方法服务.h:1748</code> | direct_const_member | this=&amp;方法服务,方法节点,节点类型::方法 | 方法节点类型匹配bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1659 | F0327 | F0330 | <code>海中鱼巣/领域/方法服务.h:1751</code> | direct_const_member | this=&amp;方法服务,方法节点,关系类型::模板,节点类型::状态,方法角色顺序号 | 角色状态节点optional | 方法节点类型匹配 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1660 | F0327 | F0329 | <code>海中鱼巣/领域/方法服务.h:1755</code> | direct_const_member | this=&amp;状态,角色状态节点.value() | 角色状态I64 optional | 角色状态目标存在且唯一 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1661 | F0327 | F0334 | <code>海中鱼巣/领域/方法服务.h:1760</code> | direct_const_member+unique_name | this=&amp;方法服务,角色 | 方法角色白名单有效bool | 角色状态I64存在并完成枚举转换；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1662 | F0328 | F0190 | <code>海中鱼巣/领域/状态服务.h:206</code> | direct_const_member | this=&amp;节点_,状态节点 | 节点记录optional | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1663 | F0328 | F0580 | <code>海中鱼巣/领域/状态服务.h:208</code> | direct_const_member | this=&amp;关系_,关系类型::运行期临时,状态节点 | 存在有效目标临时关系bool并决定最终返回 | 节点记录存在且类型为状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1665 | F0329 | F0190 | <code>海中鱼巣/领域/状态服务.h:186</code> | direct_const_member | this=&amp;节点_,状态节点 | 节点记录optional | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1666 | F0329 | F0619 | <code>海中鱼巣/领域/状态服务.h:190</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息 | 零号I64值optional并直接返回 | 节点记录存在且类型为状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1668 | F0330 | F0333 | <code>海中鱼巣/领域/方法服务.h:1713, 海中鱼巣/领域/方法服务.h:1721</code> | direct_const_member | 1713:this=&amp;方法服务,源节点,节点类型::方法；1721:this=&amp;方法服务,目标,目标类型 | 1713:源节点类型匹配bool；1721:目标节点类型匹配bool | 1713函数进入时调用一次；1721仅在源节点类型匹配并取得目标组后，对每个目标调用0..N次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1669 | F0330 | F0620 | <code>海中鱼巣/领域/方法服务.h:1717</code> | direct_const_member | this=&amp;关系_,源节点,类型,顺序号.value() | 目标节点组 | 源节点类型匹配且顺序号有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1670 | F0330 | F0621 | <code>海中鱼巣/领域/方法服务.h:1718</code> | direct_const_member | this=&amp;关系_,源节点,类型 | 目标节点组 | 源节点类型匹配且顺序号为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1672 | F0331 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:39</code> | direct_const_member | this=&amp;事务接线_ | 接域bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0106 | F0331 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:40</code> | resolved-function-pointer | this=&amp;事务接线_,事务接线_.运行期状态 | 结构事务许可 | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E1676 | F0331 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:40-42</code> | implicit_destructor | this=&amp;许可 | void | 仅事务接线已接域且许可对象已在40行形成；正常路径在41行返回对象形成后析构，异常路径在许可形成后的展开阶段析构；不覆盖43—50未接域分支 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1673 | F0331 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_const_member | this=&amp;许可 | 许可有效bool | U0008已形成许可对象 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1674 | F0331 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_const_member | this=&amp;许可 | const结构事务令牌借用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1675 | F0331 | F0622 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_member | this=&amp;主信息仓库,许可.读取令牌() | 主信息句柄返回对象 | 许可有效且E1674已取得令牌借用；调用方可达且libclang直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1677 | F0332 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:54</code> | direct_const_member | this=&amp;事务接线_ | 接域bool | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0107 | F0332 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:55</code> | resolved-function-pointer |  |  | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E1681 | F0332 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:56-57</code> | implicit_destructor | this=&amp;许可 | void | 接域分支返回对象形成后或许可形成后的异常展开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1678 | F0332 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_const_member | this=&amp;许可 | 许可有效bool | U0009已形成许可对象 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1679 | F0332 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_const_member | this=&amp;许可 | const结构事务令牌借用 | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1680 | F0332 | F0623 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_member+direct-member | this=&amp;节点仓库,类型,主信息,许可.读取令牌() | 节点句柄返回对象 | 许可有效且令牌借用已取得；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1682 | F0332 | F0624 | <code>海中鱼巣/核心/节点仓库.cpp:58</code> | direct_const_member | this=&amp;主信息_,主信息 | 主信息有效bool | 未接域路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1683 | F0332 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:58</code> | direct_free_internal_linkage+direct-free | 类型 | 类型已定义bool | 主信息有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1684 | F0333 | F0190 | <code>海中鱼巣/领域/方法服务.h:1835</code> | direct_const_member | this=&amp;节点_,节点句柄值 | optional节点记录 | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1685 | F0335 | F0626 | <code>海中鱼巣/领域/状态服务.h:297</code> | direct_const_member+suffix_match | this=&amp;状态服务,发生时间戳.value() | 时间戳可用bool | 发生时间戳有值；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1686 | F0335 | F0331 | <code>海中鱼巣/领域/状态服务.h:300</code> | direct_member | this=&amp;主信息_ | 主信息句柄 | 时间戳未提供或校验通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1688 | F0335 | F0184 | <code>海中鱼巣/领域/状态服务.h:301, 海中鱼巣/领域/状态服务.h:305, 海中鱼巣/领域/状态服务.h:305-306</code> | direct_free+unique_name | 状态值写入bool,L"创建状态节点时状态值写入不及预期。" | 检查通过bool | F0627返回后；真假均调用；F0628返回后；真假均调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1687 | F0335 | F0627 | <code>海中鱼巣/领域/状态服务.h:301</code> | direct_member | this=&amp;主信息_,主信息句柄,状态值 | 状态值写入bool | F0331返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1689 | F0335 | F0628 | <code>海中鱼巣/领域/状态服务.h:305</code> | direct_member | this=&amp;主信息_,主信息句柄,发生时间戳槽位,static_cast&lt;I64&gt;(发生时间戳.value()) | 时间戳写入bool | 状态值写入检查通过且发生时间戳有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1691 | F0335 | F0332 | <code>海中鱼巣/领域/状态服务.h:309</code> | direct_member | this=&amp;节点_,节点类型::状态,主信息句柄 | 状态节点句柄并直接返回 | 状态值及可选时间戳写入检查通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1692 | F0343 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:180</code> | direct_free+direct-free | 仓库 | 当前关系令牌指针 | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1783 | F0343 | F0377 | <code>海中鱼巣/核心/关系仓库.cpp:181</code> | direct_const_member | this=&amp;节点_,节点 | 节点有效bool | 当前关系令牌为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1782 | F0343 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:181</code> | direct_const_member | this=&amp;节点_,节点,*令牌 | 节点有效bool | 当前关系令牌非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1799 | F0345 | F0631 | <code>海中鱼巣/核心/结构事务接线.数据.h:40</code> | direct_member+unique_name | this | void | 析构函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1801 | F0346 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:251</code> | direct_free_internal_linkage+direct-free | 事务接线_,令牌 | 共享令牌有效bool | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1833 | F0349 | F0633 | <code>海中鱼巣/线程/自我线程.ixx:303</code> | direct_member+unique_name | this=&amp;语素初始化_ | 语素结果 | 首次停止请求为false；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1834 | F0349 | F0173 | <code>海中鱼巣/线程/自我线程.ixx:304</code> | direct_const_member | this=&amp;语素结果 | 语素结果成功bool | E1833返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1835 | F0349 | F0634 | <code>海中鱼巣/线程/自我线程.ixx:305, 海中鱼巣/线程/自我线程.ixx:310, 海中鱼巣/线程/自我线程.ixx:316</code> | direct_member+unique_name | this=当前自我线程 | void | 语素结果不成功；世界树结果不成功；新快照不成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1836 | F0349 | F0635 | <code>海中鱼巣/线程/自我线程.ixx:308</code> | direct_member+unique_name | this=&amp;世界树初始化_ | 世界树结果 | 语素结果成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1837 | F0349 | F0174 | <code>海中鱼巣/线程/自我线程.ixx:309</code> | direct_const_member | this=&amp;世界树结果 | 世界树结果成功bool | E1836返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1839 | F0349 | F0636 | <code>海中鱼巣/线程/自我线程.ixx:313</code> | direct_member+unique_name | this=&amp;需求初始化_,世界树结果,参数 | 根需求结果 | 世界树结果成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1840 | F0349 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:315</code> | direct_const_member | this=&amp;新快照 | 初始化快照成功bool | 根需求结果返回并构造新快照后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1843 | F0349 | F0638 | <code>海中鱼巣/线程/自我线程.ixx:343-345</code> | condition_variable_predicate | 捕获this | 停止谓词bool | condition_variable::wait初检或唤醒后，可重复 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1844 | F0350 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:48, 海中鱼巣/领域/初始化.需求.ixx:50, 海中鱼巣/领域/初始化.需求.ixx:51, 海中鱼巣/领域/初始化.需求.ixx:52, 海中鱼巣/领域/初始化.需求.ixx:53</code> | direct_free | this-&gt;特征定义 | 特征定义句柄有效bool | 函数进入；E1845返回true；E1846返回true；E1847返回true；E1848返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0108 | F0350 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:48, 海中鱼巣/领域/初始化.需求.ixx:50, 海中鱼巣/领域/初始化.需求.ixx:51, 海中鱼巣/领域/初始化.需求.ixx:52, 海中鱼巣/领域/初始化.需求.ixx:53</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1845 | F0350 | F0363 | <code>海中鱼巣/领域/初始化.需求.ixx:49</code> | direct_const_member | this=&amp;语素入口结果 | 语素入口结果成功bool | E1844返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1850 | F0351 | F0496 | <code>海中鱼巣/领域/初始化.概念图.ixx:213</code> | direct_const_member+unique_name | this=&amp;语素_,项.名称语素入口.语素入口 | 入口绑定目标组 | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1851 | F0351 | F0497 | <code>海中鱼巣/领域/初始化.概念图.ixx:214</code> | direct_const_member+unique_name | this=&amp;语素_,项.名称语素入口.语素入口 | 入口概念追溯组 | E1850返回后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1852 | F0352 | F0639 | <code>海中鱼巣/领域/概念图服务.h:893</code> | direct_static+unique_name | 类别 | 类别有效bool | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1853 | F0352 | F0640 | <code>海中鱼巣/领域/概念图服务.h:897</code> | direct_static+unique_name | 类别 | 根登记数组索引 | E1852返回true且已取得根登记共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1854 | F0352 | F0377 | <code>海中鱼巣/领域/概念图服务.h:898</code> | direct_const_member | this=&amp;节点_,材料-&gt;根节点 | 根节点有效bool | 登记槽有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1855 | F0353 | F0369 | <code>海中鱼巣/领域/概念图服务.h:1863</code> | direct_const_member+unique_name | this=当前概念图服务,概念 | 概念类别optional | 已取得活动图共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1856 | F0353 | F0360 | <code>海中鱼巣/领域/概念图服务.h:1866</code> | direct_const_member | this=当前概念图服务,概念 | 生命周期optional | E1855返回有值且仍持活动图共享锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1857 | F0354 | F0641 | <code>海中鱼巣/领域/概念图服务.h:970</code> | direct_static+unique_name | 阶段 | 阶段有效bool | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1858 | F0354 | F0642 | <code>海中鱼巣/领域/概念图服务.h:974</code> | direct_static+unique_name | 阶段 | 生命周期状态数组索引 | E1857返回true且已取得生命周期状态共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1859 | F0354 | F0377 | <code>海中鱼巣/领域/概念图服务.h:975</code> | direct_const_member | this=&amp;节点_,状态.value() | 状态节点有效bool | 登记optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1860 | F0355 | F0377 | <code>海中鱼巣/领域/概念图服务.h:909</code> | direct_const_member | this=&amp;节点_,材料-&gt;根节点 | 当前登记根节点有效bool | 每个有值登记槽；循环中至多四次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE1655 | F0356 | F0190 | <code>海中鱼巣/领域/概念图服务.h:850</code> | direct_const_member |  |  | 函数进入读取根节点记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0110 | F0356 | R0521 | <code>海中鱼巣/领域/概念图服务.h:851</code> | direct_static |  |  | 根记录存在时映射根类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0111 | F0356 | F0639 | <code>海中鱼巣/领域/概念图服务.h:852</code> | direct_static |  |  | 入口短路首先复核根类别 | 专项源码静态类型与实际装配人工复核 |
| RCE1656 | F0356 | R0088 | <code>海中鱼巣/领域/概念图服务.h:860</code> | direct_const_member |  |  | 入口通过并取得图写锁后读取上位来源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1657 | F0356 | F0051 | <code>海中鱼巣/领域/概念图服务.h:870, 海中鱼巣/领域/概念图服务.h:875, 海中鱼巣/领域/概念图服务.h:885</code> | operator |  |  | 稳定键前置成立后比较根节点；发布读回前三项成立后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0112 | F0356 | F0640 | <code>海中鱼巣/领域/概念图服务.h:880, 海中鱼巣/领域/概念图服务.h:881</code> | direct_static |  |  | 唯一性遍历完成后写入并读回根登记槽 | 专项源码静态类型与实际装配人工复核 |
| RCE0113 | F0356 | F0184 | <code>海中鱼巣/领域/概念图服务.h:882</code> | direct_free |  |  | 发布后读回完整条件形成 | 专项源码静态类型与实际装配人工复核 |
| E1707 | F0357 | F0190 | <code>海中鱼巣/领域/概念图服务.h:938</code> | direct_const_member | this=&amp;节点_,状态节点 | 状态记录optional | F0357函数进入；函数进入读取状态节点记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1667 | F0357 | F0329 | <code>海中鱼巣/领域/概念图服务.h:939</code> | direct_const_member | this=&amp;状态,状态节点 | 状态值optional | E1707节点记录读取完成；不要求记录有值，且位于if短路判断之前；节点记录读取完成后无条件读取状态值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0114 | F0357 | F0641 | <code>海中鱼巣/领域/概念图服务.h:940</code> | direct_static |  |  | 入口短路首先复核生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| E1664 | F0357 | F0328 | <code>海中鱼巣/领域/概念图服务.h:945</code> | direct_const_member | this=&amp;状态,状态节点 | 状态是否实例状态bool | 阶段有效、节点可读且类型为状态、状态值存在并匹配阶段；阶段、节点类型和状态值前置全部成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0115 | F0357 | F0642 | <code>海中鱼巣/领域/概念图服务.h:950</code> | direct_static |  |  | 取得生命周期状态独占锁后计算槽索引 | 专项源码静态类型与实际装配人工复核 |
| RCE1658 | F0357 | F0051 | <code>海中鱼巣/领域/概念图服务.h:953, 海中鱼巣/领域/概念图服务.h:956, 海中鱼巣/领域/概念图服务.h:962</code> | operator |  |  | 各 optional has_value 前置成立后比较状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0116 | F0357 | F0184 | <code>海中鱼巣/领域/概念图服务.h:962</code> | direct_free |  |  | 发布后读回完整条件形成 | 专项源码静态类型与实际装配人工复核 |
| RCE0117 | F0358 | F0377 | <code>海中鱼巣/领域/概念图服务.h:3005</code> | direct_const_member+direct-const-member |  |  | 逐个已登记状态复核节点有效；optional 状态有值后复核节点有效；单实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0118 | F0358 | R0518 | <code>海中鱼巣/领域/概念图服务.h:3010</code> | standard-library-callback |  |  | std::sort 按需调用具名节点句柄小于；std::sort 注册同类静态比较函数 | 专项源码静态类型与实际装配人工复核 |
| RCE1659 | F0358 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3011</code> | standard-library-callback |  |  | std::adjacent_find 默认相等谓词按需比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0119 | F0359 | F0369 | <code>海中鱼巣/领域/概念图服务.h:3047</code> | direct_const_member |  |  | 函数进入读取概念类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0120 | F0359 | F0641 | <code>海中鱼巣/领域/概念图服务.h:3048</code> | direct_static |  |  | 预期阶段有值时复核阶段有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0121 | F0359 | F0354 | <code>海中鱼巣/领域/概念图服务.h:3054</code> | direct_const_member |  |  | 预期阶段有值时读取预期状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1660 | F0359 | F0575 | <code>海中鱼巣/领域/概念图服务.h:3060</code> | direct_const_member |  |  | 入口通过后读取概念生命周期关系组 | 专项源码静态类型与实际装配人工复核 |
| RCE0122 | F0359 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3062, 海中鱼巣/领域/概念图服务.h:3068, 海中鱼巣/领域/概念图服务.h:3072, 海中鱼巣/领域/概念图服务.h:3090, 海中鱼巣/领域/概念图服务.h:3114, 海中鱼巣/领域/概念图服务.h:3121, 海中鱼巣/领域/概念图服务.h:3128</code> | direct_free |  |  | 各具名内部一致性与清理结果检查按源码短路成立 | 专项源码静态类型与实际装配人工复核 |
| RCE1661 | F0359 | R0605 | <code>海中鱼巣/领域/概念图服务.h:3067</code> | direct_const_member |  |  | 已有唯一关系时读取非令牌生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| RCE1662 | F0359 | R0606 | <code>海中鱼巣/领域/概念图服务.h:3078</code> | standard-library-callback |  |  | std::count_if 注册并调用非平凡完整匹配 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1663 | F0359 | R0607 | <code>海中鱼巣/领域/概念图服务.h:3095</code> | standard-library-callback |  |  | std::find_if 注册并调用非平凡编号版本匹配 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1664 | F0359 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3111, 海中鱼巣/领域/概念图服务.h:3124, 海中鱼巣/领域/概念图服务.h:3125</code> | operator |  |  | any_of 简单 lambda 或写后读回短路前置成立后比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1665 | F0359 | F0167 | <code>海中鱼巣/领域/概念图服务.h:3119</code> | direct_member |  |  | 允许创建、预期状态存在且无残留登记 | 专项源码静态类型与实际装配人工复核 |
| RCE1666 | F0359 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3120, 海中鱼巣/领域/概念图服务.h:3129</code> | direct_const_member |  |  | 创建后无条件读回；删除失败后再次读回 | 专项源码静态类型与实际装配人工复核 |
| RCE0124 | F0359 | F0168 | <code>海中鱼巣/领域/概念图服务.h:3121, 海中鱼巣/领域/概念图服务.h:3127</code> | direct_free |  |  | 写后读回检查及失败清理前复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1667 | F0359 | F0590 | <code>海中鱼巣/领域/概念图服务.h:3129</code> | direct_member |  |  | 新关系句柄有效且写后检查失败 | 专项源码静态类型与实际装配人工复核 |
| RCE1668 | F0360 | F0575 | <code>海中鱼巣/领域/概念图服务.h:3140</code> | direct_const_member |  |  | 函数进入读取概念生命周期关系组 | 专项源码静态类型与实际装配人工复核 |
| RCE1669 | F0360 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3144, 海中鱼巣/领域/概念图服务.h:3149, 海中鱼巣/领域/概念图服务.h:3165, 海中鱼巣/领域/概念图服务.h:3170</code> | direct_free |  |  | 唯一关系、阶段、登记一致性和最终命中检查 | 专项源码静态类型与实际装配人工复核 |
| RCE1670 | F0360 | R0605 | <code>海中鱼巣/领域/概念图服务.h:3148</code> | direct_const_member |  |  | 唯一记录已借用后读取非令牌生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| RCE1671 | F0360 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3154, 海中鱼巣/领域/概念图服务.h:3162, 海中鱼巣/领域/概念图服务.h:3163</code> | operator |  |  | 登记/记录节点比较按短路顺序执行 | 专项源码静态类型与实际装配人工复核 |
| RCE1672 | F0360 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3157</code> | direct_const_member |  |  | 登记概念与状态匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1673 | F0360 | R0608 | <code>海中鱼巣/领域/概念图服务.h:3174</code> | direct_const_member |  |  | 组合生命周期材料后复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1674 | F0361 | R0598 | <code>海中鱼巣/领域/概念图服务.h:3226</code> | operator |  |  | std::find_if 简单 lambda 比较登记关系与待清理关系 | 专项源码静态类型与实际装配人工复核 |
| RCE0125 | F0361 | F0590 | <code>海中鱼巣/领域/概念图服务.h:3231</code> | direct_member+direct-member |  |  | 生命周期登记命中后删除待清理关系；反向清理已登记生命周期关系；单实参写重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0126 | F0361 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3232</code> | direct_const_member+direct-const-member |  |  | 删除返回 false 后复核关系已不可读；删除失败后复核关系已不可读；单实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0127 | F0362 | R0531 | <code>海中鱼巣/领域/概念图服务.h:4202</code> | direct_const_member |  |  | 逐根复核概念根登记 | 专项源码静态类型与实际装配人工复核 |
| RCE1675 | F0362 | F0051 | <code>海中鱼巣/领域/概念图服务.h:4203, 海中鱼巣/领域/概念图服务.h:4216, 海中鱼巣/领域/概念图服务.h:4235, 海中鱼巣/领域/概念图服务.h:4236, 海中鱼巣/领域/概念图服务.h:4267, 海中鱼巣/领域/概念图服务.h:4270, 海中鱼巣/领域/概念图服务.h:4281, 海中鱼巣/领域/概念图服务.h:4285, 海中鱼巣/领域/概念图服务.h:4290, 海中鱼巣/领域/概念图服务.h:4296</code> | operator+standard-library-callback |  |  | std::find/unique 默认比较、显式端点比较和简单 any_of lambda 比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0128 | F0362 | F0358 | <code>海中鱼巣/领域/概念图服务.h:4208</code> | direct_const_member |  |  | 根与概念集合通过后复核生命周期状态登记完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1676 | F0362 | F0360 | <code>海中鱼巣/领域/概念图服务.h:4212</code> | direct_const_member |  |  | 逐个快照概念读取非令牌生命周期材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0130 | F0362 | F0369 | <code>海中鱼巣/领域/概念图服务.h:4223, 海中鱼巣/领域/概念图服务.h:4249</code> | direct_const_member |  |  | 读取签名概念与实例来源概念类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0131 | F0362 | R0532 | <code>海中鱼巣/领域/概念图服务.h:4226</code> | direct_static |  |  | 类别匹配前置后规范化签名 | 专项源码静态类型与实际装配人工复核 |
| RCE1677 | F0362 | F0587 | <code>海中鱼巣/领域/概念图服务.h:4231</code> | direct_const_member |  |  | 逐个快照关系读取当前关系记录 | 专项源码静态类型与实际装配人工复核 |
| RCE1678 | F0362 | R0604 | <code>海中鱼巣/领域/概念图服务.h:4232</code> | direct_const_member |  |  | 读取关系后复核概念活动关系材料完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0132 | F0362 | R0523 | <code>海中鱼巣/领域/概念图服务.h:4240, 海中鱼巣/领域/概念图服务.h:4241, 海中鱼巣/领域/概念图服务.h:4250, 海中鱼巣/领域/概念图服务.h:4259</code> | direct_static |  |  | 按关系角色从快照读取源或目标概念签名 | 专项源码静态类型与实际装配人工复核 |
| RCE0133 | F0362 | R0533 | <code>海中鱼巣/领域/概念图服务.h:4244</code> | direct_static |  |  | 上下位两份签名均存在后复核一般性 | 专项源码静态类型与实际装配人工复核 |
| RCE1679 | F0362 | F0190 | <code>海中鱼巣/领域/概念图服务.h:4258</code> | direct_const_member |  |  | 其它关系角色读取目标材料节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0134 | F0362 | R0522 | <code>海中鱼巣/领域/概念图服务.h:4261</code> | direct_static |  |  | 源签名和目标材料记录存在后复核类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1680 | F0362 | R0518 | <code>海中鱼巣/领域/概念图服务.h:4295</code> | standard-library-callback |  |  | std::sort 按需调用具名节点句柄小于 | 专项源码静态类型与实际装配人工复核 |
| RCE0135 | F0363 | F0168 | <code>海中鱼巣/领域/语素服务.h:37, 海中鱼巣/领域/语素服务.h:38</code> | direct_free | 对应关系；概念关系 | 对应关系句柄有效bool；概念关系句柄有效bool | F0163@35 与 F0565@36 均返回 true 后调用对应关系；对应关系也返回 true 后调用概念关系 | 源码逐行与重载类型审计 PASS |
| RCE0136 | F0364 | R0571 | <code>海中鱼巣/领域/语素服务.h:101</code> | direct_const_member | this=&amp;语素服务,对应信息节点 | 对应信息可绑定bool | R0575@96 返回可作为最小词单元 | 源码逐行与静态接收者审计 PASS |
| RCE0137 | F0364 | R0572 | <code>海中鱼巣/领域/语素服务.h:106</code> | direct_const_member | this=&amp;语素服务,概念节点 | 概念节点可追溯bool | R0571@101 返回 true | 源码逐行、实参数量与重载类型审计 PASS |
| RCE0138 | F0364 | R0573 | <code>海中鱼巣/领域/语素服务.h:111</code> | direct_member | this=&amp;语素服务,对应信息节点,主键 | 语素入口创建结果 | R0572@106 返回 true | 源码逐行与静态接收者审计 PASS |
| RCE0139 | F0364 | F0168 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_free | 概念关系 | 概念关系句柄有效bool | F0167@115 已返回概念关系 | 源码逐行与重载类型审计 PASS |
| RCE0140 | F0364 | F0497 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_const_member | this=&amp;语素服务,结果.语素入口 | 概念追溯目标组 | F0168@116 返回 true；逻辑与短路继续 | 源码逐行与短路顺序审计 PASS |
| RCE0141 | F0364 | R0570 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_const_member | this=&amp;语素服务,F0497返回的概念追溯目标组,概念节点 | 目标组包含概念节点bool | F0168@116 返回 true 且 F0497 已返回目标组 | 源码逐行与短路顺序审计 PASS |
| RCE0142 | F0364 | R0574 | <code>海中鱼巣/领域/语素服务.h:119</code> | direct_member | this=&amp;语素服务,结果 | void | 概念关系无效，或概念追溯目标组不包含概念节点 | 源码逐行与失败分支审计 PASS |
| RCE0143 | F0364 | R0575 | <code>海中鱼巣/领域/语素服务.h:96</code> | direct_const_member | this=&amp;语素服务,文本 | 语素准入状态 | F0364函数进入 | 源码逐行与静态接收者审计 PASS |
| RCE0144 | F0365 | F0184 | <code>海中鱼巣/领域/存在服务.h:107</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0145 | F0366 | F0184 | <code>海中鱼巣/领域/动态服务.h:290</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0146 | F0366 | R0610 | <code>海中鱼巣/领域/动态服务.h:294</code> | unique_name | this, 根节点 | 实例动态判定并取反进入概念根读回条件 | 记录存在、类型为动态、主信息有效且普通动态材料不存在 | 逐调用点源码复核：海中鱼巣/领域/动态服务.h:294 仅传入一个节点句柄，匹配 R0610；排除两参数事务重载 R0250 |
| RCE0147 | F0367 | R0206 | <code>海中鱼巣/领域/二次特征服务.h:50</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0148 | F0367 | F0184 | <code>海中鱼巣/领域/二次特征服务.h:52</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0149 | F0367 | R0204 | <code>海中鱼巣/领域/二次特征服务.h:55</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0151 | F0368 | F0184 | <code>海中鱼巣/领域/因果服务.h:59</code> | unique_name | this=&amp;因果服务, 记录.has_value() &amp;&amp; 记录-&gt;类型==因果引用 &amp;&amp; 主信息_.主信息是否有效(记录-&gt;主信息), 固定说明 | 后验检查bool并取反进入失败分支 | 记录读取返回后总是形成短路条件并调用 | 当前源码逐调用点与静态接收者复核 |
| RCE0152 | F0369 | R0521 | <code>海中鱼巣/领域/概念图服务.h:931</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0153 | F0370 | F0369 | <code>海中鱼巣/领域/概念图服务.h:1032, 海中鱼巣/领域/概念图服务.h:1033</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0154 | F0370 | R0531 | <code>海中鱼巣/领域/概念图服务.h:1038</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0155 | F0370 | R0526 | <code>海中鱼巣/领域/概念图服务.h:1041</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0156 | F0371 | F0621 | <code>海中鱼巣/领域/概念图服务.h:1045</code> | direct-const-member |  |  | 实例到实例支持概念；二实参目标节点组重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0157 | F0372 | R0088 | <code>海中鱼巣/领域/概念图服务.h:1049</code> | direct-const-member |  |  | 概念到实例支持概念；二实参来源节点组重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0158 | F0373 | F0192 | <code>海中鱼巣/领域/统计服务.h:959</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0159 | F0373 | F0193 | <code>海中鱼巣/领域/统计服务.h:960</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0160 | F0373 | F0194 | <code>海中鱼巣/领域/统计服务.h:963</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0161 | F0373 | F0195 | <code>海中鱼巣/领域/统计服务.h:964</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1802 | F0374 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:469</code> | direct_free_internal_linkage+direct-free | 事务接线_,令牌 | 共享令牌有效bool | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE1681 | F0375 | R0600 | <code>海中鱼巣/核心/结构事务接线.数据.h:28</code> | direct-operator |  |  | 移动构造函数体委托移动赋值 | 专项源码静态类型与实际装配人工复核 |
| RCE0162 | F0376 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:419</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0163 | F0377 | F0630 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | this=F0377接收者, 节点=节点, 令牌=RCE1740返回引用 | 接域分支返回bool | RCE1739返回true且RCE1740完成后调用；RCE1739返回false时短路不调用 | 当前源码、接收者静态类型、二实参重载与&amp;&amp;左到右顺序复核 |
| RCE0164 | F0377 | F0190 | <code>海中鱼巣/核心/节点仓库.cpp:446</code> | direct_const_member+source_audited | this=F0377接收者, 节点=节点 | std::optional&lt;节点记录&gt;，随后由X02297 has_value形成F0377返回bool | RCE1737返回false；源码上同时覆盖完全未接域与接线不完整形态 | 当前源码、接收者静态类型与单实参重载复核 |
| RCE0165 | F0381 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:251</code> | constructor |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0166 | F0381 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:252</code> | field_type_hint+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0167 | F0381 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:257, 海中鱼巣/适配/SQL数据库适配.cpp:261</code> | field_type_hint+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0168 | F0381 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:260</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0169 | F0381 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:261</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0170 | F0383 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:447</code> | suffix_match+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0171 | F0384 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0172 | F0385 | R0247 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:52</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0173 | F0385 | R0149 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:49</code> | lsp_direct |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0174 | F0385 | R0248 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:49</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0175 | F0385 | R0240 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:50</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1790 | F0387 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:733-758</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1818 | F0387 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:734, 海中鱼巣/核心/关系仓库.cpp:733-758</code> | implicit_destructor | this=&amp;取得共享许可返回的已移空临时许可; this=&amp;自动许可.value() | void | 共享许可返回临时量经RCE1756移动构造后的完整表达式末；或正常/异常退出且自动许可已承载，后者严格发生在F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；当前宏展开与C++临时量/RAII生命周期复核 |
| E1765 | F0387 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:736, 海中鱼巣/核心/关系仓库.cpp:753</code> | direct_const_member+direct-member+direct-member | this,父节点 | 父节点有效bool | 函数入口许可范围建立；当前记录匹配父节点；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0176 | F0388 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:387</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0177 | F0388 | R0304 | <code>海中鱼巣/界面/控制面板窗口.cpp:387</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0178 | F0388 | R0306 | <code>海中鱼巣/界面/控制面板窗口.cpp:388</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0179 | F0388 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:406</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0180 | F0388 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:411</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0181 | F0389 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:428, 海中鱼巣/界面/控制面板窗口.cpp:445</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0182 | F0390 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:470</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0183 | F0391 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:480, 海中鱼巣/界面/控制面板窗口.cpp:488</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0184 | F0391 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:493</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0185 | F0392 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:509</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0186 | F0396 | R0151 | <code>海中鱼巣/界面/控制面板窗口.cpp:1609</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0187 | F0396 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1617</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0188 | F0396 | R0185 | <code>海中鱼巣/界面/控制面板窗口.cpp:1626</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0189 | F0396 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:1633</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0190 | F0396 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1634</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0191 | F0396 | R0160 | <code>海中鱼巣/界面/控制面板窗口.cpp:1644</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0192 | F0396 | R0201 | <code>海中鱼巣/界面/控制面板窗口.cpp:1652</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0193 | F0396 | R0166 | <code>海中鱼巣/界面/控制面板窗口.cpp:1660</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0194 | F0397 | R0101 | <code>海中鱼巣/核心/协调.结构事务.ixx:152</code> | source-audited |  |  | 正式接线共享许可 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0195 | F0398 | R0101 | <code>海中鱼巣/核心/协调.结构事务.ixx:153</code> | source-audited |  |  | 正式接线独占许可 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0196 | F0399 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:154</code> | source-audited |  |  | 正式接线共享令牌验证 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0197 | F0400 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:155</code> | source-audited |  |  | 正式接线独占令牌验证 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0198 | F0401 | R0099 | <code>海中鱼巣/核心/协调.结构事务.ixx:156</code> | source-audited |  |  | 正式接线撤销失败隔离 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0199 | F0403 | R0114 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:321</code> | constructor |  |  | 状态动态数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0200 | F0404 | R0114 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:468</code> | constructor |  |  | 特征体系数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0201 | F0404 | R0265 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:468</code> | constructor |  |  | 特征体系数据操作构造原始材料侧表访问器 | 专项源码静态类型与实际装配人工复核 |
| RCE0202 | F0404 | R0272 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:469</code> | constructor |  |  | 特征体系数据操作构造批次发布记录只读访问器；只构造访问器，不使未接线批次写入路径可达 | 专项源码静态类型与实际装配人工复核 |
| RCE0203 | F0405 | R0114 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:215</code> | constructor |  |  | 语素基础数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0204 | F0407 | R0114 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:254</code> | constructor |  |  | 概念图结构数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0205 | F0408 | R0114 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:875</code> | constructor |  |  | 需求任务方法数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0206 | F0410 | R0114 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:34</code> | constructor |  |  | 概念活动数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0207 | F0439 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:213</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| E1798 | F0440 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1430-1433</code> | implicit_destructor | this=&amp;令牌范围 | void | 共享令牌验证通过且范围已构造，随后正常或异常退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0209 | F0440 | F0587 | <code>海中鱼巣/核心/关系仓库.cpp:1432</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0210 | F0441 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:189</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0211 | F0442 | F0336 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:154</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0212 | F0443 | F0336 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:325</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0213 | F0444 | F0336 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:472</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0214 | F0445 | F0336 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:218</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0215 | F0446 | F0336 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:105</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0216 | F0447 | F0336 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:257</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0217 | F0448 | F0336 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:878</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0218 | F0449 | F0336 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:39</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0219 | F0450 | F0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:38</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0220 | F0454 | F0074 | <code>海中鱼巣/领域/初始化.系统角色.ixx:178</code> | direct_const_member+source-corrected | this=&amp;结构 | 成功bool | F0454入口有效性与清单完整性通过，复核系统角色返回后 | F0454局部变量静态类型、F0074重载与源码调用点复核；纠正旧R0330误绑 |
| RCE0221 | F0454 | R0239 | <code>海中鱼巣/领域/初始化.系统角色.ixx:179</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0222 | F0454 | R0236 | <code>海中鱼巣/领域/初始化.系统角色.ixx:180</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0223 | F0455 | R0229 | <code>海中鱼巣/领域/初始化.系统角色.ixx:103, 海中鱼巣/领域/初始化.系统角色.ixx:111</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0224 | F0455 | R0499 | <code>海中鱼巣/领域/初始化.系统角色.ixx:146</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0225 | F0455 | R0231 | <code>海中鱼巣/领域/初始化.系统角色.ixx:154</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0226 | F0455 | R0236 | <code>海中鱼巣/领域/初始化.系统角色.ixx:156, 海中鱼巣/领域/初始化.系统角色.ixx:162</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0227 | F0455 | R0409 | <code>海中鱼巣/领域/初始化.系统角色.ixx:160</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0228 | F0455 | R0422 | <code>海中鱼巣/领域/初始化.系统角色.ixx:70, 海中鱼巣/领域/初始化.系统角色.ixx:152</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0229 | F0455 | R0232 | <code>海中鱼巣/领域/初始化.系统角色.ixx:72</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0230 | F0455 | R0496 | <code>海中鱼巣/领域/初始化.系统角色.ixx:77</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0231 | F0455 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:86, 海中鱼巣/领域/初始化.系统角色.ixx:87</code> | direct_const_member |  |  | 系统角色初始化读取写前预检中的既有身份与稳定键 | 专项源码静态类型与实际装配人工复核 |
| RCE0232 | F0458 | R0563 | <code>海中鱼巣/领域/系统角色清单.数据.h:189, 海中鱼巣/领域/系统角色清单.数据.h:191, 海中鱼巣/领域/系统角色清单.数据.h:192, 海中鱼巣/领域/系统角色清单.数据.h:193, 海中鱼巣/领域/系统角色清单.数据.h:194, 海中鱼巣/领域/系统角色清单.数据.h:199</code> | source-audited-direct-const-member | 十二个系统角色身份材料接收者，无显式参数 | bool进入短路完整性判断 | 按源码从左到右短路；循环调用仅在前置完整性门禁通过后发生 | 系统角色清单.数据.h:189-199 逐调用点静态接收者复核 |
| RCE0233 | F0458 | R0564 | <code>海中鱼巣/领域/系统角色清单.数据.h:196</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0234 | F0459 | F0168 | <code>海中鱼巣/领域/系统角色清单.数据.h:134</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0235 | F0461 | R0331 | <code>海中鱼巣/领域/服务.概念活动.ixx:73</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0236 | F0461 | R0500 | <code>海中鱼巣/领域/服务.概念活动.ixx:74</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0237 | F0461 | R0539 | <code>海中鱼巣/领域/服务.概念活动.ixx:75</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0238 | F0462 | R0500 | <code>海中鱼巣/领域/服务.概念活动.ixx:33</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0239 | F0462 | R0331 | <code>海中鱼巣/领域/服务.概念活动.ixx:38</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0240 | F0462 | R0340 | <code>海中鱼巣/领域/服务.概念活动.ixx:59</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0241 | F0464 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:146</code> | direct-const-member |  |  | 逐项比较两侧概念活动根材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0242 | F0465 | R0542 | <code>海中鱼巣/领域/概念活动状态.数据.h:117, 海中鱼巣/领域/概念活动状态.数据.h:118</code> | same_module |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE1683 | F0466 | R0538 | <code>海中鱼巣/领域/概念活动状态.数据.h:138</code> | direct-const-member |  |  | 重建视图逐根复核给定活跃角色 | 专项源码静态类型与实际装配人工复核 |
| RCE0244 | F0467 | F0163 | <code>海中鱼巣/领域/概念活动状态.数据.h:65</code> | direct-free |  |  | 状态节点静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0245 | F0467 | F0565 | <code>海中鱼巣/领域/概念活动状态.数据.h:65</code> | direct-free |  |  | 主信息静态类型为主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0246 | F0468 | F0013 | <code>海中鱼巣/自检.入口初始化.ixx:56</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0247 | F0468 | F0014 | <code>海中鱼巣/自检.入口初始化.ixx:57</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0248 | F0468 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:62</code> | source-audited-direct-const-member | this=&amp;装配.上下文-&gt;关系；无显式参数 | uint64_t 转为 size_t 后写入计数.关系数量 | 装配成功且前一项节点数量读取完成 | 自检.入口初始化.ixx:61 接收者静态类型与零参数重载复核 |
| RCE0249 | F0468 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:63</code> | direct_const_member+source_audited |  | 有效主键数量 | 调用方可达且初始化仓库自检路径进入数量读取 | 当前源码调用点、接收者类型与重载复核 |
| E0992 | F0470 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:81</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 方法登记根候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0993 | F0471 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:82</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 自我线程操作结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0994 | F0472 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:83</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 等待结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0995 | F0473 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:84</code> | captured_member_direct | 上下文.自我线程实例隐式this | 初始化快照候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0996 | F0474 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:85</code> | captured_member_direct | 上下文.概念图初始化隐式this | 概念图初始化结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0997 | F0475 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:86</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 存在根支持候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0998 | F0476 | F0190 | <code>海中鱼巣/自检.入口初始化.ixx:90</code> | captured_member_direct | 上下文.节点隐式this,根.根节点 | 节点记录候选 | F0254调用类型匹配 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1011 | F0477 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:110</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 方法登记根候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1012 | F0478 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:111</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 自我线程操作结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1013 | F0479 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:112</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 等待结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1014 | F0480 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:113</code> | captured_member_direct | 上下文.自我线程实例隐式this | 初始化快照候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1015 | F0481 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:114</code> | captured_member_direct | 上下文.概念图初始化隐式this | 概念图初始化结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1016 | F0482 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:115</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 存在根支持候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1033 | F0483 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:138</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 方法登记根候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1034 | F0484 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:139</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 自我线程操作结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1035 | F0485 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:140</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 等待结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1036 | F0486 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:141</code> | captured_member_direct | 上下文.自我线程实例隐式this | 初始化快照候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1037 | F0487 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:142</code> | captured_member_direct | 上下文.概念图初始化隐式this | 概念图初始化结果 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1038 | F0488 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:143</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 存在根支持候选 | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1054 | F0489 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:167</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1055 | F0490 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:168</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1056 | F0491 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:169</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1057 | F0492 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:170</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1058 | F0493 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:171</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1059 | F0494 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:172</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1060 | F0495 | F0496 | <code>海中鱼巣/自检.入口初始化.ixx:176</code> | captured_member_direct | 上下文.语素隐式this,根.名称语素入口.语素入口 | 绑定 | F0257调用绑定可读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1061 | F0495 | F0497 | <code>海中鱼巣/自检.入口初始化.ixx:177</code> | captured_member_direct | 上下文.语素隐式this,根.名称语素入口.语素入口 | 追溯 | F0257调用绑定可读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1062 | F0495 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:178, 海中鱼巣/自检.入口初始化.ixx:179</code> | std_find_element_compare | 绑定组元素,根.根节点 | 句柄相等位 | 第一组std::find逐元素调用0..n次；E1062所在第一组命中后，第二组std::find逐元素调用0..n次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0250 | F0496 | R0728 | <code>海中鱼巣/领域/语素服务.h:189</code> | source-audited-overload-corrected | this=&amp;当前语素服务；节点句柄值=语素入口 | bool进入入口类型门禁 | F0496入口后必达 | 语素服务.h:189 调用实参数量与两个重载完整签名复核 |
| RCE0251 | F0497 | R0728 | <code>海中鱼巣/领域/语素服务.h:196</code> | source-audited-overload-corrected | this=&amp;当前语素服务；节点句柄值=语素入口 | bool进入入口类型门禁 | F0497入口后必达 | 语素服务.h:196 调用实参数量与两个重载完整签名复核 |
| E1087 | F0498 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:200</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1088 | F0499 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:201</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1089 | F0500 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:202</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1090 | F0501 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:203</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1091 | F0502 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:204</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1092 | F0503 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:205</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1106 | F0504 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:232</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1107 | F0505 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:233</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1108 | F0506 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:234</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1109 | F0507 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:235</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1110 | F0508 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:236</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1111 | F0509 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:237</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1128 | F0510 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:256</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1129 | F0511 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:257</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1130 | F0512 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:258</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1131 | F0513 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:259</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1132 | F0514 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:260</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1133 | F0515 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:261</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1156 | F0517 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:280</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1157 | F0518 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:281</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1158 | F0519 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:282</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1159 | F0520 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:283</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1160 | F0521 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:284</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1161 | F0522 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:285</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0252 | F0523 | R0147 | <code>海中鱼巣/核心/节点仓库.cpp:334</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1178 | F0524 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:313</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1179 | F0525 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:314</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1180 | F0526 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:315</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1181 | F0527 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:316</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1182 | F0528 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:317</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1183 | F0529 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:318</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1199 | F0532 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:338</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1200 | F0533 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:339</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1201 | F0534 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:340</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1202 | F0535 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:341</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1203 | F0536 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:342</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1204 | F0537 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:343</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1221 | F0538 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:370</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1222 | F0539 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:371</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1223 | F0540 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:372</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1224 | F0541 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:373</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1225 | F0542 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:374</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1226 | F0543 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:375</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0253 | F0544 | F0441 | <code>海中鱼巣/核心/索引仓库.cpp:175</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1241 | F0545 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:396</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1242 | F0546 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:397</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1243 | F0547 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:398</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1244 | F0548 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:399</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1245 | F0549 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:400</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1246 | F0550 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:401</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1247 | F0551 | F0552 | <code>海中鱼巣/自检.入口初始化.ixx:406</code> | captured_member_direct | this=&amp;上下文.需求,单根.根需求 | 承接 | F0551进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1248 | F0551 | F0350 | <code>海中鱼巣/自检.入口初始化.ixx:407</code> | direct_const_member | this=&amp;单根 | 单根成功 | E1247返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1250 | F0551 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:408-409, 海中鱼巣/自检.入口初始化.ixx:410-412</code> | std_optional_element_compare | 目标特征候选.value(),单根.特征定义 | 句柄相等 | E1249返回optional有值；E1251返回optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1249 | F0551 | F0553 | <code>海中鱼巣/自检.入口初始化.ixx:408</code> | captured_member_direct | this=&amp;上下文.需求,单根.根需求 | 目标特征候选 | E1248返回true且承接有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1251 | F0551 | F0554 | <code>海中鱼巣/自检.入口初始化.ixx:410-411</code> | captured_member_direct | this=&amp;上下文.特征,结果.自我初始化-&gt;世界树.自我存在节点,单根.特征定义 | 槽位候选 | E1250返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1253 | F0551 | F0555 | <code>海中鱼巣/自检.入口初始化.ixx:413</code> | captured_member_direct | this=&amp;上下文.特征,单根.实例特征槽位,单根.当前特征值 | 当前值候选 | E1252返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1254 | F0551 | F0329 | <code>海中鱼巣/自检.入口初始化.ixx:415</code> | captured_member_direct | this=&amp;上下文.状态,单根.目标状态 | 目标值候选 | E1253等于当前参数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0254 | F0553 | R0576 | <code>海中鱼巣/领域/需求服务.h:544, 海中鱼巣/领域/需求服务.h:548</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0255 | F0554 | R0559 | <code>海中鱼巣/领域/特征服务.h:263</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0256 | F0554 | R0561 | <code>海中鱼巣/领域/特征服务.h:263, 海中鱼巣/领域/特征服务.h:268</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0257 | F0555 | R0558 | <code>海中鱼巣/领域/特征服务.h:364</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1274 | F0556 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:434</code> | captured_member_direct | 上下文.方法隐式this,键,上下文.状态 | 原样返回 | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1275 | F0557 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:435</code> | captured_member_direct | 上下文.自我线程实例隐式this,参数 | 原样返回 | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1276 | F0558 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:436</code> | captured_member_direct | 上下文.自我线程实例隐式this,时限 | 原样返回 | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1277 | F0559 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:437</code> | captured_member_direct | 上下文.自我线程实例隐式this | 原样返回 | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1278 | F0560 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:438</code> | captured_member_direct | 上下文.概念图初始化隐式this | 原样返回 | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1279 | F0561 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:439</code> | captured_member_direct | 上下文.概念图隐式this,实例 | 原样返回 | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0258 | F0566 | F0568 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:459, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:460, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:470-471, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:475, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:478, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:481, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:484, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:486, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:487, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:488, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:490, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:491, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:492, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:498-499</code> | source-audited-direct-member | this,固定关系类型/源索引/目标索引/顺序号/引用键跟踪标志 | 固定关系句柄候选 | 对应固定结构顺序或循环分支到达 | 本批逐调用点与同类成员函数唯一身份复核 |
| RCE0259 | F0566 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:461, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:462, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:470, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:475, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:478, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:481, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:484, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:486, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:487, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:488, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:493, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:494, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:495, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:500</code> | source-audited-direct-free-function | 各次F0568或F0167返回的关系句柄 | 关系句柄有效bool | 每项固定关系创建后按源码顺序立即检查 | 本批逐调用点与强类型重载复核 |
| RCE0260 | F0568 | R0140 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:445</code> | source-audited-direct-member | this,源索引,目标索引 | 引用键uint64 | 源目标索引范围检查通过 | 本批源码逐调用点与唯一成员身份复核 |
| RCE0261 | F0568 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:448</code> | source-audited-direct-free-function | 关系 | 关系句柄有效bool | RCE2260已返回关系句柄候选 | 本批源码逐调用点与强类型重载复核 |
| RCE0262 | F0569 | F0591 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:511</code> | source-audited-direct-member | this=&amp;关系_,待失效关系_ | 关系状态变更材料 | F0569入口后首先执行 | 本批源码逐调用点复核 |
| RCE0263 | F0569 | F0592 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:512</code> | source-audited-direct-const-member | this=&amp;失效 | 变更材料完整bool | RCE0262已返回失效材料 | 本批源码逐调用点复核 |
| RCE0264 | F0569 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:513</code> | source-audited-direct-const-member | this=&amp;关系_,失效.当前关系 | 失效关系记录optional | RCE0263返回true | 本批源码逐调用点与完整重载复核 |
| RCE0265 | F0569 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:518</code> | source-audited-direct-member | this=&amp;关系_,待删除关系_ | 删除成功bool | X01113已命中参考表删除前记录 | 本批源码逐调用点复核 |
| RCE0266 | F0569 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:524</code> | source-audited-direct-member | this=&amp;关系_,待重挂关系_,节点组_[954],节点组_[956] | 新关系句柄optional | 失效与删除参考投影均已完成 | 本批源码逐调用点与完整重载复核 |
| RCE0267 | F0569 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:527</code> | source-audited-direct-const-member | this=&amp;关系_,*重挂 | 重挂后关系记录optional | X02577返回true | 本批源码逐调用点与完整重载复核 |
| E1793 | F0575 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:843-863</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1824 | F0575 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:843-863</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1755 | F0575 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:846</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1771 | F0575 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:846, 海中鱼巣/核心/关系仓库.cpp:855</code> | direct_const_member+direct-member+direct-member | this,源节点 | 源节点有效bool | 函数入口许可范围建立；记录有效且类型和源节点匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0268 | F0577 | F0588 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:61</code> | source-audited-direct-free-function | 左[索引],右[索引] | 当前记录一致bool | 左右记录组大小相等且当前索引在范围内 | 本批源码逐调用点与同命名空间函数身份复核 |
| E1794 | F0578 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:919-947</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1826 | F0578 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:919-947</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1756 | F0578 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:922</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 目标节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1773 | F0578 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:922, 海中鱼巣/核心/关系仓库.cpp:939</code> | direct_const_member+direct-member+direct-member | this,目标节点；this,记录.源节点 | 输入目标及候选源节点有效bool | 入口短路校验；候选记录筛选后逐项复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1796 | F0579 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:980-994</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1830 | F0579 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:980-994</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1757 | F0579 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:982</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 源、目标节点均当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1778 | F0579 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:982</code> | direct_const_member+direct-member+direct-member | this,源节点；this,目标节点 | 两个输入节点有效bool | 入口必达；第二次受第一次true短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1780 | F0580 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:1005, 海中鱼巣/核心/关系仓库.cpp:998</code> | direct_const_member+direct-member+direct-member | this,目标节点；this,记录.源节点 | 输入目标及候选源节点有效bool | 入口校验；关系筛选后候选源节点复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1797 | F0580 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:996-1010</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1832 | F0580 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:996-1010</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1758 | F0580 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:998</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 目标节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1795 | F0581 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:949-978</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1828 | F0581 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:949-978</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1775 | F0581 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:952, 海中鱼巣/核心/关系仓库.cpp:970, 海中鱼巣/核心/关系仓库.cpp:971</code> | direct_const_member+lambda_direct_const_member+lambda_direct_const_member+direct-member+direct-member+direct-member | this,节点；this,记录.源节点；this,记录.目标节点 | 输入节点及候选两端点有效bool | 入口校验；remove_if谓词逐候选短路复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1786 | F0582 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:314-337</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1810 | F0582 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:314-337</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1752 | F0582 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:319</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 源节点句柄有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0269 | F0582 | R0085 | <code>海中鱼巣/核心/关系仓库.cpp:326</code> | source-audited-direct-free-function | 记录.状态 | 关系状态可审计bool | 关系表当前条目进入筛选 | 专项源码静态类型与实际装配人工复核 |
| E1365 | F0584 | F0279 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:666</code> | lambda_direct_const_member | this=&amp;夹具,密集正向 | 测量 | 窗口内且正确位为true的九读循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1367 | F0584 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:676-677</code> | lambda_direct_member | 写材料.关系,写材料.源节点,新目标 | 新关系optional | 关系仓库借用已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1366 | F0584 | F0288 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:676</code> | lambda_direct_member | this=&amp;夹具 | 关系仓库非const借用 | 九次读取成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0270 | F0585 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:333</code> | source-audited-direct-free-function | 当前关系 | 当前关系句柄有效bool | 索引未越界后短路右侧求值 | 逐边源码/静态类型审计 PASS |
| E1789 | F0586 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:431-479</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1816 | F0586 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:431-479</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1763 | F0586 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:441</code> | direct_member+direct-member+direct-member | this,新源节点；this,新目标节点 | 两个新端点节点当前有效bool | 两个新端点句柄有效；第二次受第一次true短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1784 | F0587 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:277-293</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1806 | F0587 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:277-293</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1759 | F0587 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:289</code> | direct_const_member+direct-member+direct-member | this,记录.源节点；this,记录.目标节点 | 关系记录两个端点当前有效bool | 句柄仓库/版本/状态复核通过；第二次受第一次true短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1785 | F0589 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:295-312</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1808 | F0589 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:295-312</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0271 | F0589 | R0085 | <code>海中鱼巣/核心/关系仓库.cpp:308</code> | source-audited-direct-free-function | 记录.状态 | 关系状态可审计bool | 句柄版本与记录版本一致 | 专项源码静态类型与实际装配人工复核 |
| E1788 | F0590 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:409-424</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1814 | F0590 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:409-424</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1787 | F0591 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:339-407</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1812 | F0591 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:339-407</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1761 | F0591 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:366</code> | direct_member+direct-member+direct-member | this,记录.源节点；this,记录.目标节点 | 关系记录两个端点当前有效bool | 记录存在且用途观察证据角色特殊顺序门禁通过；第二次受第一次true短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0272 | F0592 | F0168 | <code>海中鱼巣/核心/关系仓库.h:116-117</code> | source-audited-direct-free-function | 原关系；当前关系 | 两个关系句柄有效bool | 状态允许；第二次受第一次true短路 | 逐边源码/静态类型审计 PASS |
| RCE1684 | F0592 | R0598 | <code>海中鱼巣/核心/关系仓库.h:134</code> | source-audited-direct-free-function | 当前关系,原关系 | 当前关系等于原关系bool | 状态为已在目标状态，前一有效态递增分支未成立且原状态为已失效 | 专项源码静态类型与实际装配人工复核 |
| RCE0273 | F0619 | F0383 | <code>海中鱼巣/核心/主信息仓库.cpp:425</code> | source-audited-direct-const-member | this,主信息,0,许可.读取令牌() | I64值optional | 已接域且RCE2355许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0274 | F0619 | F0217 | <code>海中鱼巣/核心/主信息仓库.cpp:427</code> | source-audited-direct-const-member | this,主信息,0 | I64值optional | RCE2353返回false | 专项源码静态类型与实际装配人工复核 |
| E1792 | F0620 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:823-841</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1822 | F0620 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:823-841</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1754 | F0620 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:826</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1769 | F0620 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:826, 海中鱼巣/核心/关系仓库.cpp:833</code> | direct_const_member+direct-member+direct-member | this,源节点；this,记录.目标节点 | 输入源与候选目标当前有效bool | 入口校验；记录状态/类型/源/顺序均匹配后复核目标 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1791 | F0621 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:790-821</code> | implicit_destructor | this=&amp;自动令牌范围.value() | void | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1820 | F0621 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:790-821</code> | implicit_destructor | this=&amp;自动许可.value() | void | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1753 | F0621 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:798</code> | direct_free+direct-free | 类型 | 关系类型有效bool | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1767 | F0621 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:798, 海中鱼巣/核心/关系仓库.cpp:805</code> | direct_const_member+direct-member+direct-member | this,源节点；this,记录.目标节点 | 输入源与候选目标当前有效bool | 入口校验；记录状态/类型/源均匹配后复核目标 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0275 | F0622 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:54</code> | same_module+direct-free | 事务接线_,令牌 | 共享路径令牌有效bool | F0622函数进入 | 本批源码逐调用点与静态接收者复核 |
| RCE0276 | F0623 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | same_module+direct-free | 类型 | 节点类型已定义bool | 共享令牌与主信息有效后按短路顺序求值 | 本批源码逐调用点与静态接收者复核 |
| E1803 | F0623 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | direct_free_internal_linkage+direct-free | 事务接线_,令牌 | 共享令牌有效bool | 函数进入；后继与主信息有效和节点类型门禁短路；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0277 | F0623 | R0009 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | unique_name | this=&amp;主信息_,主信息,令牌 | 主信息有效bool | 共享令牌有效后按短路顺序求值 | 本批源码逐调用点与静态接收者复核 |
| RCE0278 | F0624 | R0009 | <code>海中鱼巣/核心/主信息仓库.cpp:366</code> | direct-member | this=this,主信息,许可.读取令牌() | 主信息有效bool | 已接域且许可有效 | 本批源码逐调用点与静态接收者复核 |
| RCE0279 | F0624 | F0384 | <code>海中鱼巣/核心/主信息仓库.cpp:368</code> | direct-member | this=this,主信息 | 主信息记录optional | 事务接线未接域 | 本批源码逐调用点与静态接收者复核 |
| RCE0280 | F0627 | R0010 | <code>海中鱼巣/核心/主信息仓库.cpp:378</code> | direct-member | this=this,主信息,0,值,许可.读取令牌() | 写入成功bool | 已接域且许可有效 | 本批源码逐调用点与静态接收者复核 |
| RCE0281 | F0627 | F0628 | <code>海中鱼巣/核心/主信息仓库.cpp:380</code> | direct-member | this=this,主信息,0,值 | 写入成功bool | 事务接线未接域 | 本批源码逐调用点与静态接收者复核 |
| RCE0282 | F0628 | R0010 | <code>海中鱼巣/核心/主信息仓库.cpp:386</code> | direct-member | this=this,主信息,值索引,值,许可.读取令牌() | 写入成功bool | 已接域且许可有效 | 本批源码逐调用点与静态接收者复核 |
| E1708 | F0629 | F0330 | <code>海中鱼巣/领域/方法服务.h:1708</code> | direct_const_member | this=&amp;方法服务,源节点,类型,目标类型,std::nullopt | 唯一目标optional并直接返回 | F0629函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0283 | F0630 | F0346 | <code>海中鱼巣/核心/节点仓库.cpp:450</code> | suffix_match+direct-member | this=this,节点,令牌 | 节点记录optional | F0630函数进入 | 本批源码逐调用点与静态接收者复核 |
| RCE0284 | F0631 | F0338 | <code>海中鱼巣/核心/结构事务接线.数据.h:59</code> | suffix_match | this=this | 许可有效bool | F0631函数进入 | 本批源码逐调用点与静态接收者复核 |
| RCE0285 | F0631 | R0100 | <code>海中鱼巣/核心/结构事务接线.数据.h:59</code> | source-audited | 运行期状态_,令牌_ | void | RCE0284返回true | 本批源码函数指针字段、唯一装配目标与调用点复核 |
| E1804 | F0632 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:24</code> | direct_const_member+unique_name | this=&amp;接线 | 接线已接域bool | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0286 | F0632 | F0399 | <code>海中鱼巣/核心/节点仓库.cpp:24</code> | resolved-function-pointer | 接线.运行期状态,令牌 | 共享路径令牌有效bool | E1804返回true | 本批源码函数指针字段、唯一装配目标与调用点复核 |
| RCE0287 | F0633 | R0241 | <code>海中鱼巣/领域/初始化.语素.ixx:146, 海中鱼巣/领域/初始化.语素.ixx:147, 海中鱼巣/领域/初始化.语素.ixx:148, 海中鱼巣/领域/初始化.语素.ixx:149, 海中鱼巣/领域/初始化.语素.ixx:150, 海中鱼巣/领域/初始化.语素.ixx:151, 海中鱼巣/领域/初始化.语素.ixx:152, 海中鱼巣/领域/初始化.语素.ixx:153, 海中鱼巣/领域/初始化.语素.ixx:154, 海中鱼巣/领域/初始化.语素.ixx:155, 海中鱼巣/领域/初始化.语素.ixx:156, 海中鱼巣/领域/初始化.语素.ixx:157, 海中鱼巣/领域/初始化.语素.ixx:158, 海中鱼巣/领域/初始化.语素.ixx:159, 海中鱼巣/领域/初始化.语素.ixx:160, 海中鱼巣/领域/初始化.语素.ixx:161, 海中鱼巣/领域/初始化.语素.ixx:162, 海中鱼巣/领域/初始化.语素.ixx:163</code> | unique_name | L"节点"…L"二次"（18项） | 18项语素初始化显示项 | 按146—163行顺序逐项执行 | 本批源码逐调用点与静态接收者复核 |
| RCE0288 | F0633 | R0246 | <code>海中鱼巣/领域/初始化.语素.ixx:165</code> | suffix_match | 结果 | void | RCE2393返回false | 本批源码逐调用点与静态接收者复核 |
| RCE0289 | F0635 | R0203 | <code>海中鱼巣/领域/初始化.世界树.ixx:54</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0290 | F0635 | R0275 | <code>海中鱼巣/领域/初始化.世界树.ixx:56</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0291 | F0635 | R0277 | <code>海中鱼巣/领域/初始化.世界树.ixx:57</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0292 | F0635 | R0225 | <code>海中鱼巣/领域/初始化.世界树.ixx:58, 海中鱼巣/领域/初始化.世界树.ixx:69, 海中鱼巣/领域/初始化.世界树.ixx:81</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0293 | F0635 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:65, 海中鱼巣/领域/初始化.世界树.ixx:66, 海中鱼巣/领域/初始化.世界树.ixx:67</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0294 | F0635 | R0223 | <code>海中鱼巣/领域/初始化.世界树.ixx:68</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0295 | F0635 | F0059 | <code>海中鱼巣/领域/初始化.世界树.ixx:73</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0296 | F0636 | F0184 | <code>海中鱼巣/领域/初始化.需求.ixx:104</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0297 | F0636 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:105</code> | direct_free+overload_resolved | 结果.概念命名目标状态 | 节点句柄有效bool | 追根因复核表达式进入且按短路顺序求值 | 本批源码逐调用点、实参静态类型与完整重载复核 |
| RCE0298 | F0636 | F0328 | <code>海中鱼巣/领域/初始化.需求.ixx:107</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0299 | F0636 | F0134 | <code>海中鱼巣/领域/初始化.需求.ixx:76</code> | direct_const_member | this=&amp;参数 | 参数有效bool | 世界树结果成功后按短路顺序求值 | 本批源码逐调用点与静态接收者复核 |
| RCE0300 | F0636 | R0249 | <code>海中鱼巣/领域/初始化.需求.ixx:81, 海中鱼巣/领域/初始化.需求.ixx:92</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0302 | R0002 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:12</code> | direct-const-member | this=&amp;接线 | 接线已接域 bool | R0002 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0303 | R0003 | R0013 | <code>海中鱼巣/核心/主信息仓库.cpp:132</code> | direct-member | this=&amp;主信息仓库, 候选, 令牌 | 核心未发布候选操作状态 → 局部状态 | 独占令牌有效、仓库与令牌匹配，且阶段既非已确认也非已撤销 | 本批源码逐调用点与静态类型复核 |
| RCE0304 | R0003 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:114</code> | direct-const-member | this=&amp;候选 | 主信息句柄 → 局部主信息 | R0003 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0305 | R0003 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:116</code> | direct-free | 事务接线_, 令牌 | 独占令牌有效 bool | 已读取候选主信息 | 本批源码逐调用点与静态类型复核 |
| RCE0306 | R0003 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:120</code> | direct-free | 候选.令牌_, 令牌 | 令牌一致 bool | 独占令牌有效且候选.仓库_ == this | 本批源码逐调用点与静态类型复核 |
| RCE0307 | R0004 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:144</code> | direct-free | 事务接线_, 令牌 | 共享令牌有效 bool | R0004 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0308 | R0004 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:146</code> | direct-free | 候选.令牌_, 令牌 | 令牌一致 bool | 共享令牌有效且候选.仓库_ == this | 本批源码逐调用点与静态类型复核 |
| RCE0309 | R0005 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:16</code> | direct-const-member | this=&amp;接线 | 接线已接域 bool | R0005 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0310 | R0006 | R0004 | <code>海中鱼巣/核心/主信息仓库.cpp:186</code> | direct-member | this=&amp;主信息仓库, 候选, 令牌 | 核心未发布候选操作状态 → 局部状态 | 独占令牌有效、仓库与令牌匹配且候选阶段非已撤销 | 本批源码逐调用点与静态类型复核 |
| RCE0311 | R0006 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:172</code> | direct-const-member | this=&amp;候选 | 主信息句柄 → 局部主信息 | R0006 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0312 | R0006 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:174</code> | direct-free | 事务接线_, 令牌 | 独占令牌有效 bool | 已读取候选主信息 | 本批源码逐调用点与静态类型复核 |
| RCE0313 | R0006 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:178</code> | direct-free | 候选.令牌_, 令牌 | 令牌一致 bool | 独占令牌有效且候选.仓库_ == this | 本批源码逐调用点与静态类型复核 |
| RCE0314 | R0006 | R0138 | <code>海中鱼巣/核心/主信息仓库.cpp:194</code> | direct-const-member | this=&amp;结果 | 结构写入结果成功 bool | 撤销状态已映射进结果 | 本批源码逐调用点与静态类型复核 |
| RCE0316 | R0008 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:313</code> | direct-free | 事务接线_, 令牌 | 独占令牌有效 bool | R0008 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0317 | R0009 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:372</code> | direct-const-member | this=&amp;主信息仓库, 主信息, 令牌 | std::optional&lt;主信息记录&gt; → 临时结果 | R0009 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0318 | R0010 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:409</code> | direct-free | 事务接线_, 令牌 | 共享令牌有效 bool | R0010 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0319 | R0011 | R0012 | <code>海中鱼巣/核心/主信息仓库.cpp:69</code> | direct-member | this=&amp;主信息仓库, 令牌 | std::optional&lt;主信息未发布候选&gt; → 局部候选 | 独占令牌有效 | 本批源码逐调用点与静态类型复核 |
| RCE0320 | R0011 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:65</code> | direct-free | 事务接线_, 令牌 | 独占令牌有效 bool | R0011 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0321 | R0011 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:74</code> | direct-const-member | this=&amp;*候选 | 主信息句柄 → 局部主信息 | 候选.has_value() == true | 本批源码逐调用点与静态类型复核 |
| RCE0322 | R0012 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:82</code> | direct-free | 事务接线_, 令牌 | 共享令牌有效 bool | R0012 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0323 | R0013 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:93</code> | direct-free | 事务接线_, 令牌 | 共享令牌有效 bool | R0013 进入 | 本批源码逐调用点与静态类型复核 |
| RCE0324 | R0013 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:95</code> | direct-free | 候选.令牌_, 令牌 | 令牌一致 bool | 共享令牌有效且候选.仓库_ == this | 本批源码逐调用点与静态类型复核 |
| RCE0325 | R0015 | F0565 | <code>海中鱼巣/核心/主信息仓库.h:44</code> | direct-free-overload | 主信息_（const 主信息句柄&amp;） | 主信息句柄形态有效 bool | 仓库_ != nullptr；&amp;&amp; 短路第二项 | 本批源码逐调用点、静态实参类型与重载身份复核 |
| RCE0326 | R0016 | R0047 | <code>海中鱼巣/核心/会话.结构写入.ixx:1033</code> | direct-const-member | this=会话_; 主信息=主信息 | bool 直接作为 R0016 返回值 | 会话_ != nullptr | 本批源码逐调用点与静态类型复核 |
| RCE0327 | R0017 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:1037</code> | direct-const-member | this=会话_; 节点=节点 | bool 直接作为 R0017 返回值 | 会话_ != nullptr | 本批源码逐调用点与静态类型复核 |
| RCE0328 | R0018 | R0049 | <code>海中鱼巣/核心/会话.结构写入.ixx:1041</code> | direct-const-member | this=会话_; 节点=节点 | std::optional&lt;节点类型&gt; 直接作为 R0018 返回值 | 会话_ != nullptr | 本批源码逐调用点与静态类型复核 |
| RCE0329 | R0019 | R0050 | <code>海中鱼巣/核心/会话.结构写入.ixx:1045</code> | direct-const-member | this=会话_; 节点=节点 | std::optional&lt;主信息句柄&gt; 直接作为 R0019 返回值 | 会话_ != nullptr | 本批源码逐调用点与静态类型复核 |
| RCE0330 | R0020 | R0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:1051</code> | direct-const-member | this=会话_; 主信息=主信息; 槽位=槽位 | std::optional&lt;std::int64_t&gt; 直接作为 R0020 返回值 | 会话_ != nullptr | 本批源码逐调用点与静态类型复核 |
| RCE0331 | R0021 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:236</code> | suffix_match | this=this | bool 用于取反早退 | R0021 入口 | 本批源码逐调用点与静态类型复核 |
| RCE0332 | R0021 | R0011 | <code>海中鱼巣/核心/会话.结构写入.ixx:237</code> | unique_name | this=&amp;主信息_; 令牌=令牌_ | 带值结构写入结果&lt;主信息未发布候选&gt; → 局部结果 | RCE0331 返回 true | 本批源码逐调用点与静态类型复核 |
| RCE0333 | R0021 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:240, 海中鱼巣/核心/会话.结构写入.ixx:245, 海中鱼巣/核心/会话.结构写入.ixx:255</code> | suffix_match | this=this；结果依次为结果.操作、输出.操作、输出.操作 | void | RCE2419=false；或 RCE2419=true 且 X02767=false；或 X02769 抛出异常 | 本批源码逐调用点与静态类型复核 |
| RCE0334 | R0021 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:249</code> | direct_const_member | this=&amp;候选 | 主信息句柄 → 局部主信息 | RCE2419=true、X02767=true 且 X01355 移动完成 | 本批源码逐调用点与静态类型复核 |
| RCE0335 | R0022 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:264</code> | suffix_match | this=this | bool 用于取反早退 | R0022 入口 | 本批源码逐调用点与静态类型复核 |
| RCE0336 | R0022 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:268, 海中鱼巣/核心/会话.结构写入.ixx:273, 海中鱼巣/核心/会话.结构写入.ixx:283</code> | suffix_match | this=this；结果依次为结果.操作、输出.操作、输出.操作 | void | RCE2422=false；或 RCE2422=true 且 X02771=false；或 X02773 抛出异常 | 本批源码逐调用点与静态类型复核 |
| RCE0337 | R0023 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:296</code> | suffix_match | this=this | bool 用于取反早退 | R0023 入口 | 本批源码逐调用点与静态类型复核 |
| RCE0338 | R0023 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:298, 海中鱼巣/核心/会话.结构写入.ixx:304, 海中鱼巣/核心/会话.结构写入.ixx:313</code> | suffix_match | this=this; 结果=输出.操作 | void | 类型==普通父子；或 RCE2426=false；或 X02776 抛出异常 | 本批源码逐调用点与静态类型复核 |
| RCE0339 | R0023 | R0066 | <code>海中鱼巣/核心/会话.结构写入.ixx:301</code> | unique_name | this=&amp;关系_; 类型=类型; 源节点=源节点; 目标节点=目标节点; 顺序号=顺序号; 令牌=令牌_ | 带值结构写入结果&lt;关系句柄&gt; → 局部结果 | RCE0337=true 且 类型!=普通父子 | 本批源码逐调用点与静态类型复核 |
| RCE0340 | R0023 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:311</code> | unique_name | this=&amp;关系_; 关系=*输出.值; 令牌=令牌_ | 结构写入结果；显式丢弃 | RCE2426=true、RCE2427=true 且 X02776 抛出异常 | 本批源码逐调用点与静态类型复核 |
| RCE0341 | R0024 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:379</code> | suffix_match | this=this | bool 用于取反早退 | R0024 入口 | 本批源码逐调用点与静态类型复核 |
| RCE0342 | R0024 | R0068 | <code>海中鱼巣/核心/会话.结构写入.ixx:380</code> | unique_name | this=&amp;关系_; 节点=节点; 新父节点=新父节点; 令牌=令牌_ | 结构化节点挂载结果 → 局部结果 | RCE0341=true | 本批源码逐调用点与静态类型复核 |
| RCE0343 | R0024 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:383, 海中鱼巣/核心/会话.结构写入.ixx:394, 海中鱼巣/核心/会话.结构写入.ixx:408</code> | suffix_match | this=this；结果依次为结果.操作、内部不一致新建失败材料、内部不一致重挂失败材料 | void | RCE2428=false；或新建关系写集登记抛出异常；或重挂能力链未全真/其它状态组合 | 本批源码逐调用点与静态类型复核 |
| RCE0344 | R0024 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:392</code> | unique_name | this=&amp;关系_; 关系=*输出.父关系; 令牌=令牌_ | 结构写入结果；显式丢弃 | 已创建、父关系有值且 X02780 抛出异常 | 本批源码逐调用点与静态类型复核 |
| RCE0345 | R0024 | R0054 | <code>海中鱼巣/核心/会话.结构写入.ixx:404</code> | unique_name | this=this; 能力=std::move(*结果.能力) | bool 作为重挂成功短路链最后条件 | 输出状态==已重挂且 X02781=true | 本批源码逐调用点与静态类型复核 |
| RCE0346 | R0025 | F0168 | <code>海中鱼巣/核心/会话.结构写入.ixx:417</code> | unique_name+direct-free |  |  | 调用方可达且源码分支条件成立；关系审计入口复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0347 | R0025 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:417</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0348 | R0025 | R0073 | <code>海中鱼巣/核心/会话.结构写入.ixx:418</code> | direct-const-member |  |  | 会话审计读取令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0349 | R0025 | R0053 | <code>海中鱼巣/核心/会话.结构写入.ixx:419</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0350 | R0026 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:426</code> | direct-free |  |  | 关系审计记录组入口复核源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0351 | R0026 | R0074 | <code>海中鱼巣/核心/会话.结构写入.ixx:427</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0352 | R0026 | R0053 | <code>海中鱼巣/核心/会话.结构写入.ixx:428</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0353 | R0027 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:452</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0354 | R0027 | R0136 | <code>海中鱼巣/核心/会话.结构写入.ixx:452</code> | source-audited |  |  | 绑定主键入口复核请求显式完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0355 | R0027 | R0119 | <code>海中鱼巣/核心/会话.结构写入.ixx:453</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0356 | R0027 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:455, 海中鱼巣/核心/会话.结构写入.ixx:464</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0357 | R0027 | R0125 | <code>海中鱼巣/核心/会话.结构写入.ixx:462</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0358 | R0028 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:474</code> | direct-free |  |  | 候选 I64 写入入口复核主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0359 | R0028 | R0015 | <code>海中鱼巣/核心/会话.结构写入.ixx:478</code> | direct-const-member |  |  | 遍历本会话主信息候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0360 | R0028 | R0010 | <code>海中鱼巣/核心/会话.结构写入.ixx:491</code> | direct-const-member |  |  | 确认候选属于本会话后写入 I64 | 专项源码静态类型与实际装配人工复核 |
| RCE0361 | R0028 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:493, 海中鱼巣/核心/会话.结构写入.ixx:500</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0362 | R0029 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:510</code> | direct-free |  |  | 候选 I64 读回入口复核主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0363 | R0029 | F0383 | <code>海中鱼巣/核心/会话.结构写入.ixx:514</code> | direct-const-member | this=&amp;主信息_, 主信息=主信息, 值索引=槽位, 令牌=令牌_ | optional&lt;int64_t&gt;左比较操作数 | 命中同主信息同槽后；与右操作数无固定先后求值 | 现状单函数图、逐行映射、源码实参类型与重载定义交叉复核 |
| RCE0364 | R0029 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:515</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0365 | R0030 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:525</code> | direct-free |  |  | 读取节点类型入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0366 | R0031 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:531</code> | direct-free |  |  | 读取节点主信息入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0367 | R0032 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:537</code> | unique_name | 主信息=主信息 | 句柄有效bool | R0044成立后的第二短路项 | 现状单函数图、逐行映射、源码实参类型与重载定义交叉复核 |
| RCE0368 | R0032 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:537</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0369 | R0033 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:542</code> | direct-free |  |  | 读取节点主键组入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0370 | R0034 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:547</code> | direct-free |  |  | 读取关系记录组入口复核源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0371 | R0034 | R0078 | <code>海中鱼巣/核心/会话.结构写入.ixx:548</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0372 | R0035 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:554</code> | direct-free |  |  | 读取来源关系记录组入口复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0373 | R0035 | R0079 | <code>海中鱼巣/核心/会话.结构写入.ixx:555</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0374 | R0036 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:567</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0375 | R0036 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:570</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0376 | R0036 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:573</code> | direct-const-member |  |  | 逐候选读取主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0377 | R0037 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:578</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0378 | R0037 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:581</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0379 | R0038 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:589</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0380 | R0038 | F0440 | <code>海中鱼巣/核心/会话.结构写入.ixx:590</code> | direct-const-member |  |  | 当前线程可访问后令牌读取关系 | 专项源码静态类型与实际装配人工复核 |
| RCE0381 | R0038 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:592</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0382 | R0038 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:597</code> | direct-const-member |  |  | 逐变更能力读取写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0387 | R0041 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:648</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0388 | R0041 | R0055 | <code>海中鱼巣/核心/会话.结构写入.ixx:648</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0389 | R0044 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:733</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0390 | R0045 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:738</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0391 | R0046 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:743</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0392 | R0047 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:750</code> | unique_name | 主信息=主信息 | 句柄有效bool | R0045返回true后的第二短路项 | 现状单函数图、逐行映射、源码实参类型与重载定义交叉复核 |
| RCE0393 | R0047 | R0045 | <code>海中鱼巣/核心/会话.结构写入.ixx:750</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0394 | R0048 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:758</code> | unique_name | 节点=节点 | 句柄有效bool | R0045返回true后的第二短路项 | 现状单函数图、逐行映射、源码实参类型与重载定义交叉复核 |
| RCE0395 | R0048 | R0045 | <code>海中鱼巣/核心/会话.结构写入.ixx:758</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0396 | R0049 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:766</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0397 | R0050 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:772</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0398 | R0051 | R0047 | <code>海中鱼巣/核心/会话.结构写入.ixx:780</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0399 | R0053 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:807</code> | direct-const-member | this=&amp;记录.能力 | 关系记录临时值，作为RCE2482左参数 | 关系变更写集循环每轮一次 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0400 | R0054 | R0094 | <code>海中鱼巣/核心/会话.结构写入.ixx:814</code> | source-audited | this=&amp;能力 | 关系记录，按值绑定写前记录 | 登记关系变更能力入口执行 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0401 | R0054 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:815</code> | source-audited | this=&amp;能力 | 关系记录，按值绑定写后记录 | RCE0400后执行 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0402 | R0054 | R0072 | <code>海中鱼巣/核心/会话.结构写入.ixx:820</code> | direct-member | this=&amp;关系_, 能力=能力, 令牌=令牌_ | 结构写入结果，绑定撤销 | 已发布关系变更写集emplace_back抛异常 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0403 | R0054 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:821</code> | direct-member | this=this, 结果={内部不一致,写后记录.关系编号,写后记录.版本,RCE2484?写前记录.版本:写后记录.版本} | void | catch中完成RCE0402撤销后 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0404 | R0060 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | unique_name+direct-const-member | this=this | bool | 完成确认入口短路第一项；false触发内部不一致返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0405 | R0060 | R0056 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | suffix_match+direct-const-member | this=this | bool并取反 | 线程可访问且阶段为写入中时求值；false触发内部不一致返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0406 | R0060 | R0058 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | suffix_match+direct-const-member | this=this | bool | 前三项通过时求值；true触发内部不一致返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0407 | R0060 | R0142 | <code>海中鱼巣/核心/会话.结构写入.ixx:851</code> | direct-const-member | this=&amp;节点_, 候选=记录.候选, 令牌=令牌_ | 结构写入结果，绑定结果 | 节点候选循环每轮一次 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0408 | R0060 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:852, 海中鱼巣/核心/会话.结构写入.ixx:859, 海中鱼巣/核心/会话.结构写入.ixx:866</code> | direct-const-member | this=&amp;各局部结果 | bool并取反 | 分别在节点、主信息、关系变更确认调用后 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0409 | R0060 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:853, 海中鱼巣/核心/会话.结构写入.ixx:860, 海中鱼巣/核心/会话.结构写入.ixx:867</code> | suffix_match+direct-const-member | this=this, 结果={内部不一致,结果.结构编号,结果.请求版本,结果.当前版本} | void | 对应RCE0408返回false | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0410 | R0060 | R0003 | <code>海中鱼巣/核心/会话.结构写入.ixx:858</code> | direct-const-member | this=&amp;主信息_, 候选=记录.候选, 令牌=令牌_ | 结构写入结果，绑定结果 | 主信息候选循环每轮一次 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0411 | R0060 | R0071 | <code>海中鱼巣/核心/会话.结构写入.ixx:865</code> | unique_name+direct-const-member | this=&amp;关系_, 能力=记录.能力, 令牌=令牌_ | 结构写入结果，绑定结果 | 关系变更写集循环每轮一次 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0412 | R0062 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:931</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；完成撤销入口复核当前线程 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0413 | R0062 | R0072 | <code>海中鱼巣/核心/会话.结构写入.ixx:938</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；逆序撤销关系变更 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0414 | R0062 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:938, 海中鱼巣/核心/会话.结构写入.ixx:942, 海中鱼巣/核心/会话.结构写入.ixx:945, 海中鱼巣/核心/会话.结构写入.ixx:948, 海中鱼巣/核心/会话.结构写入.ixx:951</code> | direct-const-member |  |  | 每项撤销或严格删除后复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE0415 | R0062 | R0125 | <code>海中鱼巣/核心/会话.结构写入.ixx:941, 海中鱼巣/核心/会话.结构写入.ixx:942</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；逆序严格删除索引绑定 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0416 | R0062 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:945</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；逆序严格删除新关系 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0417 | R0062 | R0144 | <code>海中鱼巣/核心/会话.结构写入.ixx:948</code> | direct-const-member |  |  | 逆序撤销节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0418 | R0062 | R0006 | <code>海中鱼巣/核心/会话.结构写入.ixx:951</code> | direct-const-member |  |  | 逆序撤销主信息候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0419 | R0063 | R0046 | <code>海中鱼巣/核心/会话.结构写入.ixx:101</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0420 | R0063 | R0124 | <code>海中鱼巣/核心/会话.结构写入.ixx:102</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0421 | R0063 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:113</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0422 | R0064 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:105, 海中鱼巣/核心/关系仓库.cpp:106</code> | operator |  |  | 逐字段比较源/目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0424 | R0065 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1032</code> | constructor |  |  | 共享令牌有效后建立令牌范围 | 专项源码静态类型与实际装配人工复核 |
| RCE0423 | R0065 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1032</code> | direct-free |  |  | 令牌薄包装入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0426 | R0065 | F0167 | <code>海中鱼巣/核心/关系仓库.cpp:1033</code> | direct-const-member+direct-member |  |  | 令牌范围建立后调用无令牌重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0427 | R0066 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1042</code> | source-audited+direct-free |  |  | 结构化创建关系首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0428 | R0066 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:1046</code> | source-audited+direct-free |  |  | 独占令牌有效后复核关系类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0429 | R0066 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:1047, 海中鱼巣/核心/关系仓库.cpp:1048</code> | direct-free |  |  | 结构化创建复核源/目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0430 | R0066 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1049, 海中鱼巣/核心/关系仓库.cpp:1050</code> | direct-const-member |  |  | 句柄前置通过后复核两个节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0431 | R0066 | R0065 | <code>海中鱼巣/核心/关系仓库.cpp:1054</code> | direct-const-member+direct-member |  |  | 入口材料通过后调用令牌创建重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0432 | R0066 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:1055</code> | direct-free |  |  | 创建返回后复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0433 | R0068 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1227</code> | source-audited+direct-free |  |  | 结构化挂载或重挂首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0434 | R0068 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1231, 海中鱼巣/核心/关系仓库.cpp:1232, 海中鱼巣/核心/关系仓库.cpp:1249, 海中鱼巣/核心/关系仓库.cpp:1254</code> | direct-const-member |  |  | 挂载写前端点与父链逐节点复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0435 | R0068 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:1235, 海中鱼巣/核心/关系仓库.cpp:1274, 海中鱼巣/核心/关系仓库.cpp:1283, 海中鱼巣/核心/关系仓库.cpp:1284, 海中鱼巣/核心/关系仓库.cpp:1329, 海中鱼巣/核心/关系仓库.cpp:1331</code> | operator |  |  | 节点/父节点/关系记录端点比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0436 | R0068 | R0086 | <code>海中鱼巣/核心/关系仓库.cpp:1245, 海中鱼巣/核心/关系仓库.cpp:1268</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0437 | R0068 | R0087 | <code>海中鱼巣/核心/关系仓库.cpp:1246, 海中鱼巣/核心/关系仓库.cpp:1269</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0438 | R0068 | R0069 | <code>海中鱼巣/核心/关系仓库.cpp:1250</code> | standard-library-callback |  |  | std::all_of 注册父链端点验证回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0439 | R0068 | F0184 | <code>海中鱼巣/核心/关系仓库.cpp:1263, 海中鱼巣/核心/关系仓库.cpp:1360</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0440 | R0068 | R0067 | <code>海中鱼巣/核心/关系仓库.cpp:1288, 海中鱼巣/核心/关系仓库.cpp:1318, 海中鱼巣/核心/关系仓库.cpp:1347</code> | source-audited+direct-free+direct-free+direct-free |  |  | 三个发布分支形成关系句柄；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0441 | R0068 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1314</code> | source-audited+direct-free | 左=插入结果.first指向的记录, 右=本地构造记录 | bool | 新建关系分支关系表_.emplace成功且插入结果.second=true时 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0442 | R0068 | R0096 | <code>海中鱼巣/核心/关系仓库.cpp:1340, 海中鱼巣/核心/关系仓库.cpp:1341</code> | constructor |  |  | 重挂分支形成变更能力 | 专项源码静态类型与实际装配人工复核 |
| RCE0443 | R0068 | R0080 | <code>海中鱼巣/核心/关系仓库.cpp:1342</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0444 | R0068 | R0093 | <code>海中鱼巣/核心/关系仓库.cpp:1355</code> | move-constructor |  |  | optional emplace 移入能力 | 专项源码静态类型与实际装配人工复核 |
| RCE0445 | R0069 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1254</code> | direct-const-member |  |  | 逐个父链节点复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0446 | R0070 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:136</code> | source-audited+direct-free |  |  | 已发布关系变更能力复核写前关系类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0447 | R0070 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:138, 海中鱼巣/核心/关系仓库.cpp:139, 海中鱼巣/核心/关系仓库.cpp:140, 海中鱼巣/核心/关系仓库.cpp:141</code> | direct-free |  |  | 能力完整性复核写前/写后源目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0448 | R0070 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:151, 海中鱼巣/核心/关系仓库.cpp:152, 海中鱼巣/核心/关系仓库.cpp:157, 海中鱼巣/核心/关系仓库.cpp:158</code> | direct-free-operator | 依次比较写前/写后源节点与目标节点句柄 | bool（重挂分支差异表达式按需取反） | 失效种类分支执行相等合取，或重挂分支执行差异短路析取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0449 | R0071 | R0095 | <code>海中鱼巣/核心/关系仓库.cpp:1368</code> | direct-const-member |  |  | 结构化确认先读取能力写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0450 | R0071 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1374</code> | direct-free |  |  | 结构化确认入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0451 | R0071 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:1375</code> | direct-const-member+direct-member |  |  | 复核关系变更能力完整；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0452 | R0071 | R0092 | <code>海中鱼巣/核心/关系仓库.cpp:1377</code> | direct-free |  |  | 复核能力令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0453 | R0071 | R0080 | <code>海中鱼巣/核心/关系仓库.cpp:1381</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0454 | R0071 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1385</code> | direct-free |  |  | 锁内复核写后关系记录；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0455 | R0072 | R0094 | <code>海中鱼巣/核心/关系仓库.cpp:1397</code> | source-audited |  |  | 撤销已发布关系变更先读取写前记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0456 | R0072 | R0095 | <code>海中鱼巣/核心/关系仓库.cpp:1398</code> | source-audited |  |  | 读取写前记录后读取写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0457 | R0072 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1404</code> | source-audited+direct-free |  |  | 撤销前验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0458 | R0072 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:1405</code> | direct-const-member+direct-member |  |  | 撤销前复核能力完整；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0459 | R0072 | R0092 | <code>海中鱼巣/核心/关系仓库.cpp:1407</code> | source-audited+direct-free |  |  | 能力完整后复核令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0460 | R0072 | R0084 | <code>海中鱼巣/核心/关系仓库.cpp:1411</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0461 | R0072 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1415, 海中鱼巣/核心/关系仓库.cpp:1420</code> | source-audited+direct-free+direct-free |  |  | 撤销前后两处关系记录一致性复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0462 | R0073 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1435</code> | direct-free |  |  | 令牌读取入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0464 | R0073 | F0589 | <code>海中鱼巣/核心/关系仓库.cpp:1436</code> | direct-const-member+direct-member |  |  | 令牌有效后调用无令牌审计重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0467 | R0074 | F0582 | <code>海中鱼巣/核心/关系仓库.cpp:1440</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0470 | R0075 | F0590 | <code>海中鱼巣/核心/关系仓库.cpp:1448</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0471 | R0076 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1454</code> | source-audited+direct-free |  |  | 严格删除关系首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0472 | R0076 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:1458</code> | unique_name+direct-free | 句柄=关系 | bool | R0091=true后执行入口有效性检查 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0473 | R0076 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1482, 海中鱼巣/核心/关系仓库.cpp:1483</code> | direct-const-member |  |  | 严格删除前复核关系端点 | 专项源码静态类型与实际装配人工复核 |
| RCE0476 | R0077 | F0620 | <code>海中鱼巣/核心/关系仓库.cpp:1544</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0479 | R0078 | F0575 | <code>海中鱼巣/核心/关系仓库.cpp:1548</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0482 | R0079 | F0578 | <code>海中鱼巣/核心/关系仓库.cpp:1560</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0483 | R0080 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:162</code> | suffix_match+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0489 | R0082 | F0580 | <code>海中鱼巣/核心/关系仓库.cpp:1645</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0493 | R0084 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:166</code> | suffix_match+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0494 | R0086 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:689</code> | operator |  |  | 普通父关系复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0495 | R0087 | R0086 | <code>海中鱼巣/核心/关系仓库.cpp:710</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0496 | R0087 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:721, 海中鱼巣/核心/关系仓库.cpp:724</code> | operator |  |  | 父链目标比较和循环检测 | 专项源码静态类型与实际装配人工复核 |
| RCE0505 | R0088 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:873</code> | direct-free |  |  | 目标节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0506 | R0088 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:873, 海中鱼巣/核心/关系仓库.cpp:880</code> | direct-const-member+direct-member+direct-member |  |  | 入口；目标节点；读取有效来源节点；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0507 | R0088 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:879</code> | direct-free |  |  | 记录目标等于目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0508 | R0088 | R0090 | <code>海中鱼巣/核心/关系仓库.cpp:884</code> | standard-library-callback |  |  | std::sort 注册的唯一比较回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0509 | R0089 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:88</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；短路复核接线已接域 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0510 | R0089 | F0399 | <code>海中鱼巣/核心/关系仓库.cpp:88</code> | resolved-function-pointer |  |  | 正式生产接线唯一共享令牌验证目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0511 | R0091 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:92</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0512 | R0091 | F0400 | <code>海中鱼巣/核心/关系仓库.cpp:92</code> | resolved-function-pointer |  |  | 正式生产接线唯一独占令牌验证目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0513 | R0098 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:45</code> | same_module+source-audited |  |  | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0514 | R0099 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:59</code> | same_module+source-audited |  |  | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0515 | R0099 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:60</code> | same_module+source-audited |  |  | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0516 | R0100 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:67</code> | source-audited |  |  | 结构事务内部直接调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0517 | R0101 | R0137 | <code>海中鱼巣/核心/协调.结构事务.ixx:130</code> | unique_name+source-audited |  |  | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0518 | R0101 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:85</code> | same_module+source-audited |  |  | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0519 | R0102 | R0104 | <code>海中鱼巣/核心/执行器.结构写入.ixx:116</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0520 | R0102 | R0115 | <code>海中鱼巣/核心/执行器.结构写入.ixx:116</code> | source-audited |  |  | 无对象限定调用当前结构写入执行器::有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0521 | R0102 | F0398 | <code>海中鱼巣/核心/执行器.结构写入.ixx:119</code> | resolved-function-pointer | 状态=接线_.运行期状态 | 结构事务许可，绑定局部许可 | 有效、有效参与者组和回调operator bool全部通过；生产接线唯一绑定协调.结构事务.ixx:153 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE0522 | R0102 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:120</code> | source-audited |  |  | 取得独占许可后复核结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE0523 | R0102 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:122</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1685 | R0102 | R0391 | <code>海中鱼巣/核心/执行器.结构写入.ixx:124</code> | resolved-std-function-callback | 回调(会话) | void | R0389 前置通过并在 885 行注册 R0391，R0103 转入 R0102 且会话构造完成 | 冻结源码 3920a746 + lambda 范围、std::function 静态目标、执行器两重载及实际调度点复核 |
| RCE0524 | R0102 | R0109 | <code>海中鱼巣/核心/执行器.结构写入.ixx:126, 海中鱼巣/核心/执行器.结构写入.ixx:135, 海中鱼巣/核心/执行器.结构写入.ixx:142, 海中鱼巣/核心/执行器.结构写入.ixx:155, 海中鱼巣/核心/执行器.结构写入.ixx:164, 海中鱼巣/核心/执行器.结构写入.ixx:173</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0525 | R0102 | R0062 | <code>海中鱼巣/核心/执行器.结构写入.ixx:127, 海中鱼巣/核心/执行器.结构写入.ixx:136, 海中鱼巣/核心/执行器.结构写入.ixx:143, 海中鱼巣/核心/执行器.结构写入.ixx:156, 海中鱼巣/核心/执行器.结构写入.ixx:165, 海中鱼巣/核心/执行器.结构写入.ixx:174</code> | source-audited |  |  | 六个撤销收口点调用会话完成撤销 | 专项源码静态类型与实际装配人工复核 |
| RCE0526 | R0102 | R0108 | <code>海中鱼巣/核心/执行器.结构写入.ixx:128, 海中鱼巣/核心/执行器.结构写入.ixx:137, 海中鱼巣/核心/执行器.结构写入.ixx:157, 海中鱼巣/核心/执行器.结构写入.ixx:166, 海中鱼巣/核心/执行器.结构写入.ixx:175</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0527 | R0102 | R0058 | <code>海中鱼巣/核心/执行器.结构写入.ixx:133</code> | source-audited |  |  | 会话静态类型复核失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0528 | R0102 | R0059 | <code>海中鱼巣/核心/执行器.结构写入.ixx:134</code> | source-audited |  |  | 会话静态类型读取首次失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0529 | R0102 | R0057 | <code>海中鱼巣/核心/执行器.结构写入.ixx:141</code> | source-audited |  |  | 会话静态类型复核撤销请求 | 专项源码静态类型与实际装配人工复核 |
| RCE0530 | R0102 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:144</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0531 | R0102 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:145</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0532 | R0102 | R0110 | <code>海中鱼巣/核心/执行器.结构写入.ixx:153</code> | direct-template-member |  |  | 旧结构写入执行器按结构写入事务参与者静态层级准备提交 | 专项源码静态类型与实际装配人工复核 |
| RCE0533 | R0102 | R0060 | <code>海中鱼巣/核心/执行器.结构写入.ixx:162</code> | source-audited |  |  | 会话静态类型完成确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0534 | R0102 | R0112 | <code>海中鱼巣/核心/执行器.结构写入.ixx:171</code> | direct-template-member |  |  | 旧结构写入执行器按结构写入事务参与者静态层级确认待发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0535 | R0102 | R0061 | <code>海中鱼巣/核心/执行器.结构写入.ixx:180</code> | source-audited |  |  | 会话静态类型完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0536 | R0102 | R0113 | <code>海中鱼巣/核心/执行器.结构写入.ixx:181</code> | direct-template-member |  |  | 旧结构写入执行器按结构写入事务参与者静态层级完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0538 | R0103 | R0102 | <code>海中鱼巣/核心/执行器.结构写入.ixx:191</code> | direct-template-member |  |  | 单个参与者提升为 span 后调用多参与者重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0539 | R0106 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:367</code> | direct-const-member |  |  | 隔离入口复核许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0540 | R0106 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:368</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；许可有效且隔离函数字段存在 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0541 | R0106 | F0401 | <code>海中鱼巣/核心/执行器.结构写入.ixx:368</code> | resolved-function-pointer |  |  | 生产运行期唯一撤销失败隔离目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0542 | R0107 | R0106 | <code>海中鱼巣/核心/执行器.结构写入.ixx:372</code> | suffix_match+source-audited |  |  | 调用方可达且源码分支条件成立；撤销失败进入隔离标记 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0543 | R0108 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:382</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0544 | R0108 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:383</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0545 | R0109 | R0111 | <code>海中鱼巣/核心/执行器.结构写入.ixx:390</code> | direct-template-member |  |  | 逆序逐项完成旧结构写入参与者撤销 | 专项源码静态类型与实际装配人工复核 |
| RCE0546 | R0110 | R0039 | <code>海中鱼巣/核心/执行器.结构写入.ixx:402</code> | constructor |  |  | 安全准备参与者显式构造只读准备视图 | 专项源码静态类型与实际装配人工复核 |
| RCE0547 | R0110 | R0260 | <code>海中鱼巣/核心/执行器.结构写入.ixx:403</code> | resolved-virtual |  |  | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0548 | R0111 | R0263 | <code>海中鱼巣/核心/执行器.结构写入.ixx:413</code> | resolved-virtual |  |  | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0549 | R0112 | R0261 | <code>海中鱼巣/核心/执行器.结构写入.ixx:423</code> | resolved-virtual |  |  | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0550 | R0113 | R0262 | <code>海中鱼巣/核心/执行器.结构写入.ixx:432</code> | resolved-virtual |  |  | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0551 | R0115 | F0336 | <code>海中鱼巣/核心/执行器.结构写入.ixx:70</code> | unique_name+source-audited |  |  | 调用方可达且源码分支条件成立；执行器有效性首先复核事务接线已接域 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0552 | R0116 | R0061 | <code>海中鱼巣/核心/执行器.结构写入.ixx:103</code> | direct-const-member |  |  | 确认成功后完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0553 | R0116 | R0057 | <code>海中鱼巣/核心/执行器.结构写入.ixx:107</code> | direct-const-member |  |  | 未提交分支读取显式撤销决定 | 专项源码静态类型与实际装配人工复核 |
| RCE0554 | R0116 | R0115 | <code>海中鱼巣/核心/执行器.结构写入.ixx:75</code> | direct-const-member |  |  | 执行入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0556 | R0116 | F0398 | <code>海中鱼巣/核心/执行器.结构写入.ixx:76</code> | resolved-function-pointer |  |  | 执行器有效；生产运行期唯一正式独占许可目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0557 | R0116 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:77</code> | direct-const-member |  |  | 取得独占许可后复核许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0558 | R0116 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:80</code> | direct-const-member |  |  | 许可有效后读取事务令牌 | 专项源码静态类型与实际装配人工复核 |
| RCE0559 | R0116 | R0042 | <code>海中鱼巣/核心/执行器.结构写入.ixx:80</code> | constructor |  |  | 执行器有效且独占许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0560 | R0116 | R0341 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback |  |  | 本次执行调用语境由 海中鱼巣/领域/数据操作.概念活动.ixx:42 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0561 | R0116 | R0410 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback |  |  | 本次执行调用语境由 海中鱼巣/领域/数据操作.系统角色.ixx:124 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0562 | R0116 | R0433 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback |  |  | 本次执行调用语境由 海中鱼巣/领域/数据操作.需求任务方法.ixx:1619 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0563 | R0116 | R0455 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback |  |  | 本次执行调用语境由 海中鱼巣/领域/数据操作.需求任务方法.ixx:2876 注册该回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0564 | R0116 | R0062 | <code>海中鱼巣/核心/执行器.结构写入.ixx:84, 海中鱼巣/核心/执行器.结构写入.ixx:92, 海中鱼巣/核心/执行器.结构写入.ixx:100, 海中鱼巣/核心/执行器.结构写入.ixx:108</code> | direct-const-member |  |  | 异常、失败、确认失败或未提交分支撤销会话 | 专项源码静态类型与实际装配人工复核 |
| RCE0565 | R0116 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:85, 海中鱼巣/核心/执行器.结构写入.ixx:93, 海中鱼巣/核心/执行器.结构写入.ixx:101, 海中鱼巣/核心/执行器.结构写入.ixx:109</code> | direct-const-member |  |  | 撤销后复核撤销结果 | 专项源码静态类型与实际装配人工复核 |
| RCE0566 | R0116 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:87, 海中鱼巣/核心/执行器.结构写入.ixx:95, 海中鱼巣/核心/执行器.结构写入.ixx:101, 海中鱼巣/核心/执行器.结构写入.ixx:109</code> | direct-const-member |  |  | 撤销不完整时隔离并返回内部不一致 | 专项源码静态类型与实际装配人工复核 |
| RCE0567 | R0116 | R0058 | <code>海中鱼巣/核心/执行器.结构写入.ixx:90</code> | direct-const-member |  |  | 回调正常返回后复核会话失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0568 | R0116 | R0059 | <code>海中鱼巣/核心/执行器.结构写入.ixx:91</code> | direct-const-member |  |  | 会话已有失败时读取首次失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0569 | R0116 | R0056 | <code>海中鱼巣/核心/执行器.结构写入.ixx:97</code> | direct-const-member |  |  | 无失败时复核是否请求提交 | 专项源码静态类型与实际装配人工复核 |
| RCE0570 | R0116 | R0060 | <code>海中鱼巣/核心/执行器.结构写入.ixx:98</code> | direct-const-member |  |  | 已请求提交时完成确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0571 | R0116 | R0138 | <code>海中鱼巣/核心/执行器.结构写入.ixx:99</code> | direct-const-member |  |  | 确认返回后复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE0573 | R0119 | R0121 | <code>海中鱼巣/核心/索引仓库.cpp:132</code> | source-audited+direct-free |  |  | 结构化绑定主键首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0574 | R0119 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:136</code> | direct-const-member |  |  | 结构化绑定主键复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0575 | R0119 | R0128 | <code>海中鱼巣/核心/索引仓库.cpp:136</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0576 | R0119 | R0122 | <code>海中鱼巣/核心/索引仓库.cpp:162</code> | source-audited+direct-free |  |  | 校验及加锁完成后绑定主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0577 | R0120 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:19</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0578 | R0120 | F0399 | <code>海中鱼巣/核心/索引仓库.cpp:19</code> | resolved-function-pointer |  |  | 运行期上下文把结构事务协调器::生成接线的共享验证 lambda 按值装入索引仓库接线 | 专项源码静态类型与实际装配人工复核 |
| RCE0579 | R0121 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:23</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0580 | R0121 | F0400 | <code>海中鱼巣/核心/索引仓库.cpp:23</code> | resolved-function-pointer |  |  | 运行期上下文把结构事务协调器::生成接线的独占验证 lambda 按值装入索引仓库接线 | 专项源码静态类型与实际装配人工复核 |
| RCE0581 | R0122 | R0127 | <code>海中鱼巣/核心/索引仓库.cpp:34, 海中鱼巣/核心/索引仓库.cpp:52</code> | operator |  |  | 比较现有/插入主键绑定记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0583 | R0123 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:299</code> | direct+direct-free |  |  | 删除主键令牌重载进入后验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0584 | R0123 | R0126 | <code>海中鱼巣/核心/索引仓库.cpp:301</code> | direct+direct-free |  |  | 共享令牌有效并取得锁后删除主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0585 | R0124 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:307</code> | direct+direct-free |  |  | 读取主键令牌重载进入后验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0586 | R0125 | R0121 | <code>海中鱼巣/核心/索引仓库.cpp:362</code> | source-audited+direct-free |  |  | 严格删除主键首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0587 | R0125 | F0163 | <code>海中鱼巣/核心/索引仓库.cpp:366</code> | direct-free |  |  | 严格删除主键入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0588 | R0125 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:367</code> | direct-const-member |  |  | 严格删除主键令牌复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0589 | R0125 | F0051 | <code>海中鱼巣/核心/索引仓库.cpp:376</code> | operator |  |  | 索引绑定节点与预期节点比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0590 | R0125 | R0126 | <code>海中鱼巣/核心/索引仓库.cpp:393</code> | source-audited+direct-free |  |  | 正反索引一致且加锁后删除主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0591 | R0127 | F0051 | <code>海中鱼巣/核心/索引仓库.h:52</code> | operator |  |  | 比较绑定节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0592 | R0127 | R0132 | <code>海中鱼巣/核心/索引仓库.h:53</code> | operator |  |  | 比较所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0593 | R0128 | F0163 | <code>海中鱼巣/核心/索引所有权.数据.h:103</code> | source-audited |  |  | 兼容绑定请求复核目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0594 | R0128 | R0131 | <code>海中鱼巣/核心/索引所有权.数据.h:104</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0595 | R0128 | R0135 | <code>海中鱼巣/核心/索引所有权.数据.h:104</code> | source-audited |  |  | 兼容完整性复核所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0596 | R0129 | R0134 | <code>海中鱼巣/核心/索引所有权.数据.h:119</code> | lsp_direct |  |  | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0597 | R0134 | R0133 | <code>海中鱼巣/核心/索引所有权.数据.h:83</code> | lsp_direct |  |  | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0598 | R0135 | R0130 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | source-audited |  |  | 所有者声明符合规范首先复核显式完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0599 | R0135 | R0132 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | source-audited |  |  | 显式完整后比较声明与规范声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0600 | R0135 | R0134 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | lsp_direct |  |  | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0601 | R0136 | F0163 | <code>海中鱼巣/核心/索引所有权.数据.h:98</code> | source-audited |  |  | 绑定请求复核目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0602 | R0136 | R0135 | <code>海中鱼巣/核心/索引所有权.数据.h:98</code> | source-audited |  |  | 物理键和目标句柄有效后复核所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE1686 | R0137 | R0599 | <code>海中鱼巣/核心/结构事务接线.数据.h:47</code> | constructor |  |  | 创建工厂先默认构造结果对象 | 专项源码静态类型与实际装配人工复核 |
| RCE0603 | R0141 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:128</code> | direct-free |  |  | 确认候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0604 | R0141 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:130</code> | direct-free |  |  | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0605 | R0142 | R0141 | <code>海中鱼巣/核心/节点仓库.cpp:167</code> | suffix_match+direct-const-member+direct-member |  |  | 调用方可达且源码分支条件成立；候选处于未发布阶段时执行确认；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0606 | R0142 | R0148 | <code>海中鱼巣/核心/节点仓库.cpp:149</code> | direct-const-member |  |  | 结构化确认先读取候选节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0607 | R0142 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:151</code> | direct-free |  |  | 结构化确认入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0608 | R0142 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:155</code> | direct-free |  |  | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0609 | R0143 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:179</code> | direct-const-member+direct-free |  |  | 撤销候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0610 | R0143 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:181</code> | direct-const-member+direct-free |  |  | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0611 | R0144 | R0143 | <code>海中鱼巣/核心/节点仓库.cpp:221</code> | suffix_match+direct-const-member+direct-member |  |  | 调用方可达且源码分支条件成立；候选状态允许撤销；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0612 | R0144 | R0148 | <code>海中鱼巣/核心/节点仓库.cpp:207</code> | direct-const-member |  |  | 形成撤销结果前读取候选句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0613 | R0144 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:209</code> | direct-free |  |  | 结构化撤销入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0614 | R0144 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:213</code> | direct-const-member+direct-free |  |  | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0615 | R0144 | R0138 | <code>海中鱼巣/核心/节点仓库.cpp:229</code> | direct-const-member |  |  | 撤销结果成功后归零当前版本 | 专项源码静态类型与实际装配人工复核 |
| RCE0616 | R0145 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:28</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0618 | R0147 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:351</code> | direct+direct-free |  |  | 删除节点令牌重载进入后验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0619 | R0150 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:105</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0620 | R0151 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1017, 海中鱼巣/界面/控制面板窗口.cpp:1086, 海中鱼巣/界面/控制面板窗口.cpp:1121</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0621 | R0151 | R0183 | <code>海中鱼巣/界面/控制面板窗口.cpp:1097</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0622 | R0151 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1100</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0623 | R0151 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1100</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0624 | R0151 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1103</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0625 | R0151 | R0177 | <code>海中鱼巣/界面/控制面板窗口.cpp:1126</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0626 | R0151 | R0152 | <code>海中鱼巣/界面/控制面板窗口.cpp:1127</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0627 | R0151 | R0155 | <code>海中鱼巣/界面/控制面板窗口.cpp:1128</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0628 | R0151 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:1129</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0629 | R0151 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:1130</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0630 | R0151 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1131</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0631 | R0151 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1132</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0632 | R0153 | R0186 | <code>海中鱼巣/界面/控制面板窗口.cpp:1146</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0633 | R0153 | R0154 | <code>海中鱼巣/界面/控制面板窗口.cpp:1155, 海中鱼巣/界面/控制面板窗口.cpp:1188</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0634 | R0153 | R0327 | <code>海中鱼巣/界面/控制面板窗口.cpp:1167</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0635 | R0156 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1251</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0636 | R0156 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:1323, 海中鱼巣/界面/控制面板窗口.cpp:1331</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0637 | R0156 | R0157 | <code>海中鱼巣/界面/控制面板窗口.cpp:1338</code> | direct-local-lambda |  |  | 当前分页为系统信息且当前系统信息导航为数据库审计 | 专项源码静态类型与实际装配人工复核 |
| RCE0638 | R0158 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:1362</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0639 | R0158 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1367, 海中鱼巣/界面/控制面板窗口.cpp:1370, 海中鱼巣/界面/控制面板窗口.cpp:1377</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0640 | R0158 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1370, 海中鱼巣/界面/控制面板窗口.cpp:1377</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0641 | R0158 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1378</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0642 | R0158 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1382</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0643 | R0158 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1383</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0644 | R0158 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:1384</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0645 | R0158 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1385</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0646 | R0158 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1386</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0647 | R0160 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1393</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0648 | R0160 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1400</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0649 | R0161 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1411</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0650 | R0163 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1424</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0651 | R0164 | F0388 | <code>海中鱼巣/界面/控制面板窗口.cpp:1434</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0652 | R0164 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:1440</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0653 | R0164 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1451, 海中鱼巣/界面/控制面板窗口.cpp:1458</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0654 | R0164 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1451, 海中鱼巣/界面/控制面板窗口.cpp:1458</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0655 | R0164 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1459</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0656 | R0164 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1463</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0657 | R0164 | R0177 | <code>海中鱼巣/界面/控制面板窗口.cpp:1464</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0658 | R0164 | R0152 | <code>海中鱼巣/界面/控制面板窗口.cpp:1465</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0659 | R0164 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:1466</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0660 | R0164 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1467</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0661 | R0164 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1468</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0662 | R0164 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1469</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0663 | R0165 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:1476</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0664 | R0165 | R0308 | <code>海中鱼巣/界面/控制面板窗口.cpp:1476</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0665 | R0165 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1480</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0666 | R0165 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1484</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0667 | R0166 | R0199 | <code>海中鱼巣/界面/控制面板窗口.cpp:1492, 海中鱼巣/界面/控制面板窗口.cpp:1494, 海中鱼巣/界面/控制面板窗口.cpp:1496</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0668 | R0166 | R0200 | <code>海中鱼巣/界面/控制面板窗口.cpp:1498, 海中鱼巣/界面/控制面板窗口.cpp:1500</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0669 | R0166 | R0164 | <code>海中鱼巣/界面/控制面板窗口.cpp:1502</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0670 | R0166 | R0163 | <code>海中鱼巣/界面/控制面板窗口.cpp:1504</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0671 | R0166 | R0161 | <code>海中鱼巣/界面/控制面板窗口.cpp:1506</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0672 | R0166 | R0165 | <code>海中鱼巣/界面/控制面板窗口.cpp:1508</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0673 | R0170 | F0268 | <code>海中鱼巣/界面/控制面板窗口.cpp:173</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0674 | R0171 | F0268 | <code>海中鱼巣/界面/控制面板窗口.cpp:193</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0675 | R0171 | F0270 | <code>海中鱼巣/界面/控制面板窗口.cpp:194</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0676 | R0171 | R0170 | <code>海中鱼巣/界面/控制面板窗口.cpp:198</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| RCE0677 | R0172 | R0172 | <code>海中鱼巣/界面/控制面板窗口.cpp:205</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0678 | R0173 | R0172 | <code>海中鱼巣/界面/控制面板窗口.cpp:213</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0679 | R0174 | F0168 | <code>海中鱼巣/界面/控制面板窗口.cpp:219</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0680 | R0174 | R0174 | <code>海中鱼巣/界面/控制面板窗口.cpp:229</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0681 | R0175 | R0173 | <code>海中鱼巣/界面/控制面板窗口.cpp:240</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0682 | R0175 | R0174 | <code>海中鱼巣/界面/控制面板窗口.cpp:246</code> | lsp_direct+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0683 | R0176 | R0307 | <code>海中鱼巣/界面/控制面板窗口.cpp:322</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0684 | R0176 | R0150 | <code>海中鱼巣/界面/控制面板窗口.cpp:323</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0685 | R0176 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:328, 海中鱼巣/界面/控制面板窗口.cpp:372</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0686 | R0176 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:332</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0687 | R0176 | R0175 | <code>海中鱼巣/界面/控制面板窗口.cpp:342</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0688 | R0176 | R0303 | <code>海中鱼巣/界面/控制面板窗口.cpp:350</code> | field_type_hint |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0689 | R0177 | F0056 | <code>海中鱼巣/界面/控制面板窗口.cpp:419</code> | direct-const-member |  |  | 控制面板读取最近五十条结构统计审计材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0690 | R0183 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:561, 海中鱼巣/界面/控制面板窗口.cpp:568, 海中鱼巣/界面/控制面板窗口.cpp:574</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0691 | R0183 | R0178 | <code>海中鱼巣/界面/控制面板窗口.cpp:565</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0692 | R0183 | R0180 | <code>海中鱼巣/界面/控制面板窗口.cpp:567</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0693 | R0183 | R0179 | <code>海中鱼巣/界面/控制面板窗口.cpp:572</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0694 | R0186 | R0185 | <code>海中鱼巣/界面/控制面板窗口.cpp:598</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0695 | R0187 | R0184 | <code>海中鱼巣/界面/控制面板窗口.cpp:607</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0696 | R0187 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:610</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0697 | R0187 | R0187 | <code>海中鱼巣/界面/控制面板窗口.cpp:614</code> | direct-member |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0698 | R0188 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:624, 海中鱼巣/界面/控制面板窗口.cpp:635</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0699 | R0188 | R0184 | <code>海中鱼巣/界面/控制面板窗口.cpp:632</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0700 | R0189 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:643, 海中鱼巣/界面/控制面板窗口.cpp:666, 海中鱼巣/界面/控制面板窗口.cpp:676, 海中鱼巣/界面/控制面板窗口.cpp:685</code> | unique_name+direct-member+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0701 | R0189 | R0188 | <code>海中鱼巣/界面/控制面板窗口.cpp:659</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0702 | R0189 | R0190 | <code>海中鱼巣/界面/控制面板窗口.cpp:661, 海中鱼巣/界面/控制面板窗口.cpp:667, 海中鱼巣/界面/控制面板窗口.cpp:671, 海中鱼巣/界面/控制面板窗口.cpp:677, 海中鱼巣/界面/控制面板窗口.cpp:686, 海中鱼巣/界面/控制面板窗口.cpp:689</code> | implicit-local-destructor-of-explicit-project-definition |  |  | line 653 局部收口器已构造；后续任一 return 或正常作用域退出均调用源码显式定义析构 | 专项源码静态类型与实际装配人工复核 |
| RCE0703 | R0189 | R0187 | <code>海中鱼巣/界面/控制面板窗口.cpp:670</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0704 | R0191 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:696</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0705 | R0192 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:707, 海中鱼巣/界面/控制面板窗口.cpp:716, 海中鱼巣/界面/控制面板窗口.cpp:728, 海中鱼巣/界面/控制面板窗口.cpp:736</code> | unique_name+direct-member+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0706 | R0192 | R0303 | <code>海中鱼巣/界面/控制面板窗口.cpp:722</code> | direct-const-member |  |  | 逐项发布概念根下拉选项前复核选项完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0707 | R0194 | R0168 | <code>海中鱼巣/界面/控制面板窗口.cpp:760</code> | same_module+direct-free |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0708 | R0195 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:769, 海中鱼巣/界面/控制面板窗口.cpp:770</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0709 | R0195 | R0169 | <code>海中鱼巣/界面/控制面板窗口.cpp:772, 海中鱼巣/界面/控制面板窗口.cpp:773, 海中鱼巣/界面/控制面板窗口.cpp:774, 海中鱼巣/界面/控制面板窗口.cpp:775, 海中鱼巣/界面/控制面板窗口.cpp:776, 海中鱼巣/界面/控制面板窗口.cpp:777, 海中鱼巣/界面/控制面板窗口.cpp:778, 海中鱼巣/界面/控制面板窗口.cpp:779, 海中鱼巣/界面/控制面板窗口.cpp:780, 海中鱼巣/界面/控制面板窗口.cpp:781, 海中鱼巣/界面/控制面板窗口.cpp:782</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0710 | R0196 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:787, 海中鱼巣/界面/控制面板窗口.cpp:803, 海中鱼巣/界面/控制面板窗口.cpp:822</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0711 | R0197 | F0563 | <code>海中鱼巣/界面/控制面板窗口.cpp:866, 海中鱼巣/界面/控制面板窗口.cpp:880</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0712 | R0197 | R0182 | <code>海中鱼巣/界面/控制面板窗口.cpp:872, 海中鱼巣/界面/控制面板窗口.cpp:874, 海中鱼巣/界面/控制面板窗口.cpp:876</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0713 | R0197 | R0171 | <code>海中鱼巣/界面/控制面板窗口.cpp:873, 海中鱼巣/界面/控制面板窗口.cpp:875, 海中鱼巣/界面/控制面板窗口.cpp:877</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0714 | R0198 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:892</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0715 | R0198 | R0196 | <code>海中鱼巣/界面/控制面板窗口.cpp:895</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0716 | R0198 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:896</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0717 | R0198 | R0155 | <code>海中鱼巣/界面/控制面板窗口.cpp:897</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0718 | R0198 | R0197 | <code>海中鱼巣/界面/控制面板窗口.cpp:901</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0719 | R0199 | R0159 | <code>海中鱼巣/界面/控制面板窗口.cpp:907, 海中鱼巣/界面/控制面板窗口.cpp:923</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0720 | R0199 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:908</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0721 | R0199 | R0183 | <code>海中鱼巣/界面/控制面板窗口.cpp:912</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0722 | R0199 | R0191 | <code>海中鱼巣/界面/控制面板窗口.cpp:915</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0723 | R0199 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:918</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0724 | R0199 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:919</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0725 | R0199 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:920</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0726 | R0199 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:921</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0727 | R0199 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:925</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0728 | R0200 | R0159 | <code>海中鱼巣/界面/控制面板窗口.cpp:931</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0729 | R0200 | R0162 | <code>海中鱼巣/界面/控制面板窗口.cpp:933</code> | direct-free |  |  | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0730 | R0200 | R0199 | <code>海中鱼巣/界面/控制面板窗口.cpp:933</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0731 | R0201 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1000, 海中鱼巣/界面/控制面板窗口.cpp:980, 海中鱼巣/界面/控制面板窗口.cpp:989</code> | unique_name+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0732 | R0201 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1003, 海中鱼巣/界面/控制面板窗口.cpp:940, 海中鱼巣/界面/控制面板窗口.cpp:968, 海中鱼巣/界面/控制面板窗口.cpp:984, 海中鱼巣/界面/控制面板窗口.cpp:993</code> | unique_name+direct-member+direct-member+direct-member+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0733 | R0201 | R0191 | <code>海中鱼巣/界面/控制面板窗口.cpp:944, 海中鱼巣/界面/控制面板窗口.cpp:973</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0734 | R0201 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:947, 海中鱼巣/界面/控制面板窗口.cpp:979</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0735 | R0201 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:954</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0736 | R0201 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:963, 海中鱼巣/界面/控制面板窗口.cpp:967</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0737 | R0201 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:963, 海中鱼巣/界面/控制面板窗口.cpp:967</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0738 | R0201 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:976</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0739 | R0201 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:977, 海中鱼巣/界面/控制面板窗口.cpp:997</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0740 | R0201 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:978, 海中鱼巣/界面/控制面板窗口.cpp:999</code> | unique_name+direct-member+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0741 | R0201 | R0197 | <code>海中鱼巣/界面/控制面板窗口.cpp:988</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0742 | R0201 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:998</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0743 | R0204 | F0620 | <code>海中鱼巣/领域/二次特征服务.h:111</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0744 | R0204 | R0205 | <code>海中鱼巣/领域/二次特征服务.h:113</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0745 | R0206 | F0622 | <code>海中鱼巣/领域/二次特征服务.h:39</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0746 | R0206 | F0623 | <code>海中鱼巣/领域/二次特征服务.h:40</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0747 | R0206 | R0009 | <code>海中鱼巣/领域/二次特征服务.h:43</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0748 | R0207 | R0218 | <code>海中鱼巣/领域/任务服务.h:414</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0749 | R0207 | R0219 | <code>海中鱼巣/领域/任务服务.h:417</code> | direct-const-member |  |  | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0750 | R0208 | R0218 | <code>海中鱼巣/领域/任务服务.h:421</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0751 | R0208 | R0219 | <code>海中鱼巣/领域/任务服务.h:424</code> | direct-const-member |  |  | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0752 | R0209 | R0218 | <code>海中鱼巣/领域/任务服务.h:428</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0753 | R0209 | R0219 | <code>海中鱼巣/领域/任务服务.h:431</code> | direct-const-member |  |  | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0754 | R0210 | R0218 | <code>海中鱼巣/领域/任务服务.h:435</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0755 | R0210 | R0219 | <code>海中鱼巣/领域/任务服务.h:438</code> | direct-const-member |  |  | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0756 | R0211 | R0222 | <code>海中鱼巣/领域/任务服务.h:442</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0757 | R0211 | R0221 | <code>海中鱼巣/领域/任务服务.h:445</code> | direct-const-member |  |  | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0758 | R0212 | R0222 | <code>海中鱼巣/领域/任务服务.h:449</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0759 | R0212 | R0221 | <code>海中鱼巣/领域/任务服务.h:452</code> | direct-const-member |  |  | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0760 | R0213 | R0218 | <code>海中鱼巣/领域/任务服务.h:456</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0761 | R0213 | R0219 | <code>海中鱼巣/领域/任务服务.h:459, 海中鱼巣/领域/任务服务.h:460, 海中鱼巣/领域/任务服务.h:461, 海中鱼巣/领域/任务服务.h:462</code> | direct-const-member |  |  | 任务承接壳完整；四类承接目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0762 | R0213 | R0221 | <code>海中鱼巣/领域/任务服务.h:463</code> | direct-const-member |  |  | 任务承接壳完整；读取已创建状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0763 | R0214 | R0222 | <code>海中鱼巣/领域/任务服务.h:482, 海中鱼巣/领域/任务服务.h:486</code> | direct-const-member |  |  | 需求入口及每个来源节点类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0764 | R0214 | R0088 | <code>海中鱼巣/领域/任务服务.h:485</code> | direct-const-member |  |  | 需求类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0765 | R0214 | R0213 | <code>海中鱼巣/领域/任务服务.h:489</code> | direct-const-member |  |  | 来源节点为任务 | 专项源码静态类型与实际装配人工复核 |
| RCE0766 | R0214 | F0051 | <code>海中鱼巣/领域/任务服务.h:490, 海中鱼巣/领域/任务服务.h:503</code> | direct_free+standard_library_callback+source_audited | line490: 承接材料-&gt;来源需求, 需求节点；line503: std::unique相邻节点句柄对 | line490 bool进入if短路条件；line503 bool由std::unique消费 | line490在承接材料存在时调用；line503在任务组排序完成后由std::unique零到多次调用 | 当前源码直接比较、标准算法默认相等合同、节点句柄静态类型与RCB0003复核 |
| RCE0767 | R0214 | R0215 | <code>海中鱼巣/领域/任务服务.h:494</code> | standard-library-callback |  |  | std::sort 注册的唯一比较回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0768 | R0216 | R0218 | <code>海中鱼巣/领域/任务服务.h:517</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0769 | R0216 | R0220 | <code>海中鱼巣/领域/任务服务.h:520</code> | direct-const-member |  |  | 任务承接壳完整；四参数顺序重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0770 | R0217 | R0218 | <code>海中鱼巣/领域/任务服务.h:544</code> | direct-const-member |  |  | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0771 | R0217 | R0220 | <code>海中鱼巣/领域/任务服务.h:547</code> | direct-const-member |  |  | 任务承接壳完整；四参数顺序重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0772 | R0218 | R0219 | <code>海中鱼巣/领域/任务服务.h:727, 海中鱼巣/领域/任务服务.h:728, 海中鱼巣/领域/任务服务.h:729, 海中鱼巣/领域/任务服务.h:730</code> | direct-const-member |  |  | 短路读取四类承接目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0773 | R0218 | R0221 | <code>海中鱼巣/领域/任务服务.h:731</code> | direct-const-member |  |  | 四类承接目标均存在 | 专项源码静态类型与实际装配人工复核 |
| RCE0774 | R0219 | R0220 | <code>海中鱼巣/领域/任务服务.h:735</code> | direct-const-member |  |  | 显式第四参数 std::nullopt | 专项源码静态类型与实际装配人工复核 |
| RCE0775 | R0220 | R0222 | <code>海中鱼巣/领域/任务服务.h:740, 海中鱼巣/领域/任务服务.h:748</code> | direct-const-member |  |  | 源任务及每个关系目标类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0776 | R0220 | F0620 | <code>海中鱼巣/领域/任务服务.h:744</code> | source-audited |  |  | 顺序号有值；三实参仓库重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0777 | R0220 | F0621 | <code>海中鱼巣/领域/任务服务.h:745</code> | source-audited |  |  | 顺序号无值；两实参仓库重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0778 | R0221 | R0222 | <code>海中鱼巣/领域/任务服务.h:761, 海中鱼巣/领域/任务服务.h:767</code> | direct-const-member |  |  | 任务及每个关系目标类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0779 | R0221 | F0621 | <code>海中鱼巣/领域/任务服务.h:764</code> | source-audited |  |  | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0780 | R0221 | F0329 | <code>海中鱼巣/领域/任务服务.h:770</code> | source-audited | this=&amp;状态,目标 | 状态值optional | 遍历任务引用目标且目标类型为状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0781 | R0222 | F0190 | <code>海中鱼巣/领域/任务服务.h:784</code> | source-audited | this=&amp;方法服务,方法首节点,关系类型::引用,节点类型::任务,来源任务顺序号 | 来源任务节点optional并直接返回 | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0782 | R0224 | R0147 | <code>海中鱼巣/领域/初始化.世界树.ixx:132</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0783 | R0224 | R0008 | <code>海中鱼巣/领域/初始化.世界树.ixx:133</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0784 | R0225 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:137, 海中鱼巣/领域/初始化.世界树.ixx:140, 海中鱼巣/领域/初始化.世界树.ixx:143</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0785 | R0225 | R0075 | <code>海中鱼巣/领域/初始化.世界树.ixx:138, 海中鱼巣/领域/初始化.世界树.ixx:141, 海中鱼巣/领域/初始化.世界树.ixx:144</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0786 | R0225 | R0224 | <code>海中鱼巣/领域/初始化.世界树.ixx:146, 海中鱼巣/领域/初始化.世界树.ixx:147, 海中鱼巣/领域/初始化.世界树.ixx:148</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0787 | R0229 | R0501 | <code>海中鱼巣/领域/初始化.系统角色.ixx:220</code> | direct-const-member | this=&amp;方法服务,方法首节点,关系类型::引用,节点类型::方法,父方法顺序号 | 父方法节点optional并直接返回 | 特征服务字段静态类型为特征业务服务&amp; | 专项源码静态类型与实际装配人工复核 |
| RCE0789 | R0229 | R0369 | <code>海中鱼巣/领域/初始化.系统角色.ixx:221, 海中鱼巣/领域/初始化.系统角色.ixx:228</code> | direct-const-member | this=&amp;方法服务,动作入口节点,关系类型::引用,节点类型::场景,动作输入规格顺序号 | 动作输入规格场景optional并直接返回 | 两个返回值静态类型均为特征体系业务结果 | 专项源码静态类型与实际装配人工复核 |
| RCE1687 | R0229 | R0601 | <code>海中鱼巣/领域/初始化.系统角色.ixx:221, 海中鱼巣/领域/初始化.系统角色.ixx:228</code> | direct-static-member | this=&amp;方法服务,动作入口节点,关系类型::引用,节点类型::场景,动作输出规格顺序号 | 动作输出规格场景optional并直接返回 | 特征结果状态静态类型为特征体系业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0790 | R0229 | R0370 | <code>海中鱼巣/领域/初始化.系统角色.ixx:222, 海中鱼巣/领域/初始化.系统角色.ixx:229</code> | direct-const-member | this=&amp;方法服务,动作入口节点,关系类型::模板,节点类型::状态,动作入口状态顺序号 | 状态节点optional | 两个返回值静态类型均为特征体系业务结果 | 专项源码静态类型与实际装配人工复核 |
| RCE0791 | R0229 | R0566 | <code>海中鱼巣/领域/初始化.系统角色.ixx:224</code> | direct-const-member |  |  | 特征状态组合器字段静态类型为特征状态组合器&amp; | 专项源码静态类型与实际装配人工复核 |
| RCE0792 | R0229 | R0514 | <code>海中鱼巣/领域/初始化.系统角色.ixx:231</code> | direct-const-member |  |  | 需求服务字段静态类型为需求业务服务&amp; | 专项源码静态类型与实际装配人工复核 |
| RCE0794 | R0229 | R0465 | <code>海中鱼巣/领域/初始化.系统角色.ixx:234</code> | direct-const-member |  |  | 需求返回值静态类型为需求提交结果 | 专项源码静态类型与实际装配人工复核 |
| RCE1688 | R0229 | R0602 | <code>海中鱼巣/领域/初始化.系统角色.ixx:234</code> | direct-static-member |  |  | 需求结果状态静态类型为需求任务方法业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1689 | R0229 | R0603 | <code>海中鱼巣/领域/初始化.系统角色.ixx:235</code> | direct-static-member |  |  | 需求结果状态静态类型为需求任务方法业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0796 | R0230 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:242</code> | direct_const_member |  |  | 形成身份进入后查找具名系统角色键占用材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0797 | R0231 | R0230 | <code>海中鱼巣/领域/初始化.系统角色.ixx:253, 海中鱼巣/领域/初始化.系统角色.ixx:254, 海中鱼巣/领域/初始化.系统角色.ixx:255, 海中鱼巣/领域/初始化.系统角色.ixx:260, 海中鱼巣/领域/初始化.系统角色.ixx:261, 海中鱼巣/领域/初始化.系统角色.ixx:262, 海中鱼巣/领域/初始化.系统角色.ixx:263, 海中鱼巣/领域/初始化.系统角色.ixx:264, 海中鱼巣/领域/初始化.系统角色.ixx:267, 海中鱼巣/领域/初始化.系统角色.ixx:268, 海中鱼巣/领域/初始化.系统角色.ixx:269, 海中鱼巣/领域/初始化.系统角色.ixx:270, 海中鱼巣/领域/初始化.系统角色.ixx:271, 海中鱼巣/领域/初始化.系统角色.ixx:273, 海中鱼巣/领域/初始化.系统角色.ixx:274, 海中鱼巣/领域/初始化.系统角色.ixx:275, 海中鱼巣/领域/初始化.系统角色.ixx:276, 海中鱼巣/领域/初始化.系统角色.ixx:278, 海中鱼巣/领域/初始化.系统角色.ixx:279, 海中鱼巣/领域/初始化.系统角色.ixx:280, 海中鱼巣/领域/初始化.系统角色.ixx:281</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0798 | R0232 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:288, 海中鱼巣/领域/初始化.系统角色.ixx:299, 海中鱼巣/领域/初始化.系统角色.ixx:306, 海中鱼巣/领域/初始化.系统角色.ixx:312, 海中鱼巣/领域/初始化.系统角色.ixx:313</code> | direct_const_member |  |  | 按短路顺序查找世界根、动态根、因果根、关系根和存在根用途 | 专项源码静态类型与实际装配人工复核 |
| RCE0799 | R0232 | R0233 | <code>海中鱼巣/领域/初始化.系统角色.ixx:294, 海中鱼巣/领域/初始化.系统角色.ixx:296</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0800 | R0232 | R0398 | <code>海中鱼巣/领域/初始化.系统角色.ixx:302</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0801 | R0232 | R0512 | <code>海中鱼巣/领域/初始化.系统角色.ixx:308</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0802 | R0232 | R0234 | <code>海中鱼巣/领域/初始化.系统角色.ixx:334</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0803 | R0233 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:355, 海中鱼巣/领域/初始化.系统角色.ixx:356, 海中鱼巣/领域/初始化.系统角色.ixx:357, 海中鱼巣/领域/初始化.系统角色.ixx:358, 海中鱼巣/领域/初始化.系统角色.ixx:359, 海中鱼巣/领域/初始化.系统角色.ixx:360, 海中鱼巣/领域/初始化.系统角色.ixx:361</code> | direct_const_member |  |  | 完成安全组和服务组用途选择后查找七类占用材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0804 | R0233 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:385</code> | direct_const_member |  |  | 目标占用材料存在且已占用，并已读取主键状态材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0805 | R0233 | R0434 | <code>海中鱼巣/领域/初始化.系统角色.ixx:393</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0806 | R0234 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:406, 海中鱼巣/领域/初始化.系统角色.ixx:407, 海中鱼巣/领域/初始化.系统角色.ixx:408, 海中鱼巣/领域/初始化.系统角色.ixx:409</code> | direct_const_member |  |  | 已有方法根语义匹配后查找登记根和三状态角色用途 | 专项源码静态类型与实际装配人工复核 |
| RCE0807 | R0234 | R0235 | <code>海中鱼巣/领域/初始化.系统角色.ixx:410, 海中鱼巣/领域/初始化.系统角色.ixx:411, 海中鱼巣/领域/初始化.系统角色.ixx:412</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0808 | R0234 | R0497 | <code>海中鱼巣/领域/初始化.系统角色.ixx:418</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0809 | R0235 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:430</code> | direct_const_member |  |  | 占用材料非空且已占用，并已读取主键状态材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0810 | R0236 | R0237 | <code>海中鱼巣/领域/初始化.系统角色.ixx:441, 海中鱼巣/领域/初始化.系统角色.ixx:442</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0811 | R0236 | R0512 | <code>海中鱼巣/领域/初始化.系统角色.ixx:445</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0812 | R0236 | R0398 | <code>海中鱼巣/领域/初始化.系统角色.ixx:447</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0813 | R0236 | R0497 | <code>海中鱼巣/领域/初始化.系统角色.ixx:462</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0814 | R0236 | R0238 | <code>海中鱼巣/领域/初始化.系统角色.ixx:468, 海中鱼巣/领域/初始化.系统角色.ixx:471, 海中鱼巣/领域/初始化.系统角色.ixx:474</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0815 | R0237 | R0390 | <code>海中鱼巣/领域/初始化.系统角色.ixx:492</code> | direct_const_member |  |  | 复核根需求组完成五类材料读取后进入合取链第一项 | 专项源码静态类型与实际装配人工复核 |
| RCE0816 | R0237 | R0343 | <code>海中鱼巣/领域/初始化.系统角色.ixx:494</code> | direct_const_member |  |  | 定义材料完整且身份匹配后复核槽位材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0817 | R0237 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:502</code> | direct_const_member |  |  | 根需求组特征材料复核通过后复核目标抽象状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0818 | R0237 | R0434 | <code>海中鱼巣/领域/初始化.系统角色.ixx:504</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0819 | R0238 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:518</code> | direct_const_member |  |  | 复核抽象状态进入并读取身份稳定主键材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0820 | R0241 | F0163 | <code>海中鱼巣/领域/初始化.语素.ixx:178</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0821 | R0241 | R0242 | <code>海中鱼巣/领域/初始化.语素.ixx:178</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0822 | R0241 | R0244 | <code>海中鱼巣/领域/初始化.语素.ixx:179</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0823 | R0241 | F0364 | <code>海中鱼巣/领域/初始化.语素.ixx:182</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0824 | R0241 | R0243 | <code>海中鱼巣/领域/初始化.语素.ixx:184</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0825 | R0241 | R0245 | <code>海中鱼巣/领域/初始化.语素.ixx:185</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0826 | R0242 | F0624 | <code>海中鱼巣/领域/初始化.语素.ixx:195</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0827 | R0243 | F0496 | <code>海中鱼巣/领域/初始化.语素.ixx:199</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0828 | R0243 | F0497 | <code>海中鱼巣/领域/初始化.语素.ixx:200</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0829 | R0244 | F0523 | <code>海中鱼巣/领域/初始化.语素.ixx:220</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0830 | R0244 | R0761 | <code>海中鱼巣/领域/初始化.语素.ixx:221</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0831 | R0245 | F0168 | <code>海中鱼巣/领域/初始化.语素.ixx:225, 海中鱼巣/领域/初始化.语素.ixx:228</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0832 | R0245 | F0590 | <code>海中鱼巣/领域/初始化.语素.ixx:226, 海中鱼巣/领域/初始化.语素.ixx:229</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0833 | R0245 | F0523 | <code>海中鱼巣/领域/初始化.语素.ixx:232</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0834 | R0245 | R0761 | <code>海中鱼巣/领域/初始化.语素.ixx:235</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0835 | R0245 | R0244 | <code>海中鱼巣/领域/初始化.语素.ixx:237</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0836 | R0246 | R0245 | <code>海中鱼巣/领域/初始化.语素.ixx:264</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0837 | R0247 | F0220 | <code>海中鱼巣/领域/初始化.语素.ixx:36</code> | direct-const-member |  |  | 名称和类型两字段依次短路调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0838 | R0249 | R0560 | <code>海中鱼巣/领域/初始化.需求.ixx:123</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0839 | R0249 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:124, 海中鱼巣/领域/初始化.需求.ixx:138, 海中鱼巣/领域/初始化.需求.ixx:142, 海中鱼巣/领域/初始化.需求.ixx:156</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状图、逐行映射与重载调用点专项复核 |
| RCE0840 | R0249 | F0184 | <code>海中鱼巣/领域/初始化.需求.ixx:124, 海中鱼巣/领域/初始化.需求.ixx:130, 海中鱼巣/领域/初始化.需求.ixx:138, 海中鱼巣/领域/初始化.需求.ixx:142, 海中鱼巣/领域/初始化.需求.ixx:156, 海中鱼巣/领域/初始化.需求.ixx:178</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0841 | R0249 | F0364 | <code>海中鱼巣/领域/初始化.需求.ixx:128</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0842 | R0249 | R0562 | <code>海中鱼巣/领域/初始化.需求.ixx:143</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0843 | R0249 | R0579 | <code>海中鱼巣/领域/初始化.需求.ixx:148</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0844 | R0249 | R0581 | <code>海中鱼巣/领域/初始化.需求.ixx:159</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0845 | R0249 | F0554 | <code>海中鱼巣/领域/初始化.需求.ixx:161</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0846 | R0249 | F0555 | <code>海中鱼巣/领域/初始化.需求.ixx:162</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0847 | R0249 | F0552 | <code>海中鱼巣/领域/初始化.需求.ixx:164</code> | source-audited-direct | this=&amp;需求_；需求节点=结果.根需求 | 需求承接材料可选值 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE0848 | R0249 | F0496 | <code>海中鱼巣/领域/初始化.需求.ixx:165</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0849 | R0249 | F0497 | <code>海中鱼巣/领域/初始化.需求.ixx:166</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0866 | R0260 | R0017 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:111</code> | source-audited |  |  | 准备视图形参复核值节点属于本会话候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0867 | R0260 | R0016 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:112</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0868 | R0260 | R0018 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:113</code> | source-audited |  |  | 准备视图形参读取值节点候选类型 | 专项源码静态类型与实际装配人工复核 |
| RCE0869 | R0260 | R0019 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:115</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0870 | R0260 | R0020 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:118</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0871 | R0260 | R0554 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:129</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0872 | R0260 | R0556 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:130</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0873 | R0260 | R0264 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:166</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0874 | R0263 | R0264 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:197</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0875 | R0264 | R0546 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:207</code> | direct-member |  |  | 非 const 服务引用选择 Vec 记录非 const 查找重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0876 | R0264 | R0550 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:215</code> | direct-member |  |  | 非 const 服务引用选择 I64 记录非 const 查找重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0877 | R0266 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:247</code> | direct-free |  |  | 侧表读取入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0878 | R0266 | R0554 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:254</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；const 服务读取 Vec 记录数量 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0879 | R0266 | R0556 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:255</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；const 服务读取 I64 版本记录数量 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0880 | R0266 | R0552 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:261</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；const 服务选择 I64 记录 const 查找重载 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0881 | R0266 | R0545 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:262</code> | unique_name+direct-static-member |  |  | 调用方可达且源码分支条件成立；复核 I64 版本记录内部一致 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0882 | R0266 | R0548 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:272</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；const 服务选择 Vec 记录 const 查找重载 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0883 | R0266 | R0544 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:273</code> | unique_name+direct-static-member |  |  | 调用方可达且源码分支条件成立；复核 Vec 记录内部一致 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0884 | R0267 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:31</code> | direct-free |  |  | 特征值节点形参静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0885 | R0267 | F0565 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:31</code> | direct-free |  |  | 主信息形参静态类型为主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0886 | R0268 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:63</code> | direct-free |  |  | 快照中的特征值节点静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0887 | R0270 | R0267 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:83</code> | source-audited |  |  | 材料形参静态类型复核字段自洽 | 专项源码静态类型与实际装配人工复核 |
| RCE0888 | R0270 | R0271 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:84</code> | standard-library-callback |  |  | std::find_if 注册并调用本地唯一重复身份谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE0889 | R0271 | R0259 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:85</code> | direct-static-member |  |  | find_if 谓词调用同类静态节点身份比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0892 | R0275 | R0276 | <code>海中鱼巣/领域/场景服务.h:53</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0893 | R0278 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1019</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0894 | R0278 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:1048</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0895 | R0278 | R0328 | <code>海中鱼巣/领域/控制面板服务.h:1050</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0896 | R0279 | R0280 | <code>海中鱼巣/领域/控制面板服务.h:1064</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0897 | R0279 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1077, 海中鱼巣/领域/控制面板服务.h:1127</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0898 | R0279 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1083, 海中鱼巣/领域/控制面板服务.h:1133</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0900 | R0279 | R0527 | <code>海中鱼巣/领域/控制面板服务.h:1090</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0901 | R0280 | R0310 | <code>海中鱼巣/领域/控制面板服务.h:1068</code> | direct-static-member |  |  | 投影深度相等 | 专项源码静态类型与实际装配人工复核 |
| RCE0902 | R0281 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1192</code> | direct-const-member |  |  | 目标存在 | 专项源码静态类型与实际装配人工复核 |
| RCE0903 | R0281 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1193</code> | source-static-type+direct-free |  |  | 控制面板按需树路径；字段静态类型/重载专项复核；子项已生成 | 专项源码静态类型与实际装配人工复核 |
| RCE0904 | R0282 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1204</code> | direct-const-member |  |  | 把具体句柄提升为 optional 重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0905 | R0283 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1211, 海中鱼巣/领域/控制面板服务.h:1232</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0906 | R0283 | R0580 | <code>海中鱼巣/领域/控制面板服务.h:1218</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0907 | R0283 | R0214 | <code>海中鱼巣/领域/控制面板服务.h:1222</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0908 | R0283 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1224</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0909 | R0284 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1248, 海中鱼巣/领域/控制面板服务.h:1257</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0910 | R0284 | R0217 | <code>海中鱼巣/领域/控制面板服务.h:1253</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0911 | R0284 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1265</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0912 | R0284 | R0285 | <code>海中鱼巣/领域/控制面板服务.h:1266</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0913 | R0285 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1267, 海中鱼巣/领域/控制面板服务.h:1270, 海中鱼巣/领域/控制面板服务.h:1273</code> | direct-static-member |  |  | 方法来源任务组稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0914 | R0286 | R0580 | <code>海中鱼巣/领域/控制面板服务.h:1289</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0915 | R0286 | R0593 | <code>海中鱼巣/领域/控制面板服务.h:1293</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0916 | R0286 | F0552 | <code>海中鱼巣/领域/控制面板服务.h:1294</code> | direct-const-member |  |  | 需求有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0917 | R0286 | F0552 | <code>海中鱼巣/领域/控制面板服务.h:1294</code> | source-audited-direct | this=&amp;需求_；需求节点 | 需求承接材料可选值 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE0918 | R0286 | R0581 | <code>海中鱼巣/领域/控制面板服务.h:1295</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0919 | R0286 | R0582 | <code>海中鱼巣/领域/控制面板服务.h:1296</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0920 | R0286 | R0583 | <code>海中鱼巣/领域/控制面板服务.h:1297</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0921 | R0286 | R0584 | <code>海中鱼巣/领域/控制面板服务.h:1298</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0922 | R0286 | R0585 | <code>海中鱼巣/领域/控制面板服务.h:1299</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0923 | R0286 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1316, 海中鱼巣/领域/控制面板服务.h:1329</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0924 | R0286 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1317, 海中鱼巣/领域/控制面板服务.h:1336</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0926 | R0286 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1318, 海中鱼巣/领域/控制面板服务.h:1319, 海中鱼巣/领域/控制面板服务.h:1320, 海中鱼巣/领域/控制面板服务.h:1321, 海中鱼巣/领域/控制面板服务.h:1322, 海中鱼巣/领域/控制面板服务.h:1337, 海中鱼巣/领域/控制面板服务.h:1338, 海中鱼巣/领域/控制面板服务.h:1339, 海中鱼巣/领域/控制面板服务.h:1351</code> | direct-const-member |  |  | optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0927 | R0286 | R0587 | <code>海中鱼巣/领域/控制面板服务.h:1327</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；需求承接材料完整 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0928 | R0286 | R0588 | <code>海中鱼巣/领域/控制面板服务.h:1330</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0929 | R0286 | R0589 | <code>海中鱼巣/领域/控制面板服务.h:1331</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0930 | R0286 | R0590 | <code>海中鱼巣/领域/控制面板服务.h:1332</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0931 | R0286 | R0591 | <code>海中鱼巣/领域/控制面板服务.h:1333</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0932 | R0286 | R0592 | <code>海中鱼巣/领域/控制面板服务.h:1334</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0933 | R0286 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1335, 海中鱼巣/领域/控制面板服务.h:1359</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0934 | R0286 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1345, 海中鱼巣/领域/控制面板服务.h:1361</code> | direct-const-member |  |  | 具体句柄目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0935 | R0286 | R0214 | <code>海中鱼巣/领域/控制面板服务.h:1358</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0936 | R0287 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1373</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0937 | R0287 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1374</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0938 | R0287 | R0286 | <code>海中鱼巣/领域/控制面板服务.h:1386</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0939 | R0288 | R0213 | <code>海中鱼巣/领域/控制面板服务.h:1399</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0940 | R0288 | R0207 | <code>海中鱼巣/领域/控制面板服务.h:1400</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0941 | R0288 | R0208 | <code>海中鱼巣/领域/控制面板服务.h:1401</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0942 | R0288 | R0209 | <code>海中鱼巣/领域/控制面板服务.h:1402</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0943 | R0288 | R0210 | <code>海中鱼巣/领域/控制面板服务.h:1403</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0944 | R0288 | R0211 | <code>海中鱼巣/领域/控制面板服务.h:1404</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0945 | R0288 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1419</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0946 | R0288 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1420</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0948 | R0288 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1421, 海中鱼巣/领域/控制面板服务.h:1422, 海中鱼巣/领域/控制面板服务.h:1423, 海中鱼巣/领域/控制面板服务.h:1424, 海中鱼巣/领域/控制面板服务.h:1425, 海中鱼巣/领域/控制面板服务.h:1433, 海中鱼巣/领域/控制面板服务.h:1438, 海中鱼巣/领域/控制面板服务.h:1443</code> | direct-const-member |  |  | 任务 optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0949 | R0288 | R0212 | <code>海中鱼巣/领域/控制面板服务.h:1429</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0950 | R0288 | R0216 | <code>海中鱼巣/领域/控制面板服务.h:1430</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0951 | R0288 | R0217 | <code>海中鱼巣/领域/控制面板服务.h:1431</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0952 | R0289 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1455</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0953 | R0289 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1456</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0954 | R0289 | R0288 | <code>海中鱼巣/领域/控制面板服务.h:1468</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0955 | R0290 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1489, 海中鱼巣/领域/控制面板服务.h:1501, 海中鱼巣/领域/控制面板服务.h:1514, 海中鱼巣/领域/控制面板服务.h:1525, 海中鱼巣/领域/控制面板服务.h:1536</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0956 | R0290 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1490, 海中鱼巣/领域/控制面板服务.h:1502, 海中鱼巣/领域/控制面板服务.h:1516, 海中鱼巣/领域/控制面板服务.h:1527, 海中鱼巣/领域/控制面板服务.h:1537</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0958 | R0290 | R0468 | <code>海中鱼巣/领域/控制面板服务.h:1496</code> | unique_name | (方法节点, 状态_) |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0959 | R0290 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1503</code> | direct-const-member |  |  | 方法虚拟存在具体句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0960 | R0290 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1511, 海中鱼巣/领域/控制面板服务.h:1512, 海中鱼巣/领域/控制面板服务.h:1579, 海中鱼巣/领域/控制面板服务.h:1580</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0961 | R0290 | R0492 | <code>海中鱼巣/领域/控制面板服务.h:1515</code> | unique_name+source-audited | (方法节点, 条件, 状态_) |  | 调用方可达且源码分支条件成立；方法树节点包含方法条件 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0962 | R0290 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1517, 海中鱼巣/领域/控制面板服务.h:1528, 海中鱼巣/领域/控制面板服务.h:1545, 海中鱼巣/领域/控制面板服务.h:1551, 海中鱼巣/领域/控制面板服务.h:1560</code> | direct-const-member |  |  | 方法 optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0963 | R0290 | R0493 | <code>海中鱼巣/领域/控制面板服务.h:1526</code> | unique_name | (方法节点, 结果节点, 状态_) |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0964 | R0290 | R0474 | <code>海中鱼巣/领域/控制面板服务.h:1542</code> | unique_name | (候选材料-&gt;动作入口) |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0965 | R0290 | R0475 | <code>海中鱼巣/领域/控制面板服务.h:1543</code> | unique_name | (候选材料-&gt;动作入口) |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0966 | R0290 | R0473 | <code>海中鱼巣/领域/控制面板服务.h:1558</code> | unique_name | (方法节点, 状态_) |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0967 | R0290 | R0291 | <code>海中鱼巣/领域/控制面板服务.h:1575, 海中鱼巣/领域/控制面板服务.h:1582, 海中鱼巣/领域/控制面板服务.h:1588</code> | direct-lambda |  |  | 父方法/前置/后续递归投影 | 专项源码静态类型与实际装配人工复核 |
| RCE0968 | R0291 | R0290 | <code>海中鱼巣/领域/控制面板服务.h:1567</code> | direct-const-member |  |  | 关联方法递归展开 | 专项源码静态类型与实际装配人工复核 |
| RCE0969 | R0292 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1603</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0970 | R0292 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1604</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0971 | R0292 | R0293 | <code>海中鱼巣/领域/控制面板服务.h:1615</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0972 | R0292 | R0290 | <code>海中鱼巣/领域/控制面板服务.h:1631</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0973 | R0292 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1647</code> | direct-const-member |  |  | 方法来源任务具体句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0974 | R0293 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1616, 海中鱼巣/领域/控制面板服务.h:1619, 海中鱼巣/领域/控制面板服务.h:1622</code> | direct-static-member |  |  | 方法来源任务组稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0975 | R0294 | R0294 | <code>海中鱼巣/领域/控制面板服务.h:1661</code> | direct-recursive |  |  | 对子节点递归排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0976 | R0294 | R0295 | <code>海中鱼巣/领域/控制面板服务.h:1663</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0977 | R0295 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1668, 海中鱼巣/领域/控制面板服务.h:1671</code> | direct-static-member |  |  | 关系角色相同 | 专项源码静态类型与实际装配人工复核 |
| RCE0978 | R0296 | R0294 | <code>海中鱼巣/领域/控制面板服务.h:1680</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0979 | R0296 | R0297 | <code>海中鱼巣/领域/控制面板服务.h:1682</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0980 | R0297 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1683</code> | direct-static-member |  |  | 根节点稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0981 | R0298 | R0298 | <code>海中鱼巣/领域/控制面板服务.h:1696</code> | direct-recursive |  |  | 对子节点递归标记 | 专项源码静态类型与实际装配人工复核 |
| RCE0982 | R0299 | R0298 | <code>海中鱼巣/领域/控制面板服务.h:1703</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0983 | R0300 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1718, 海中鱼巣/领域/控制面板服务.h:1727</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0984 | R0300 | R0300 | <code>海中鱼巣/领域/控制面板服务.h:1734</code> | direct-recursive |  |  | 对子节点递归复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0985 | R0301 | R0300 | <code>海中鱼巣/领域/控制面板服务.h:1757</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0986 | R0303 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:203</code> | source-static-type+direct-free |  |  | 控制面板按需树路径；字段静态类型/重载专项复核；根类别在范围内 | 专项源码静态类型与实际装配人工复核 |
| RCE0988 | R0304 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:280</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0989 | R0304 | R0495 | <code>海中鱼巣/领域/控制面板服务.h:285</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0990 | R0306 | R0305 | <code>海中鱼巣/领域/控制面板服务.h:302, 海中鱼巣/领域/控制面板服务.h:303, 海中鱼巣/领域/控制面板服务.h:304, 海中鱼巣/领域/控制面板服务.h:305, 海中鱼巣/领域/控制面板服务.h:306, 海中鱼巣/领域/控制面板服务.h:307</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0991 | R0307 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:313</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0992 | R0307 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:315, 海中鱼巣/领域/控制面板服务.h:319, 海中鱼巣/领域/控制面板服务.h:322, 海中鱼巣/领域/控制面板服务.h:325, 海中鱼巣/领域/控制面板服务.h:331, 海中鱼巣/领域/控制面板服务.h:372, 海中鱼巣/领域/控制面板服务.h:375, 海中鱼巣/领域/控制面板服务.h:378, 海中鱼巣/领域/控制面板服务.h:391, 海中鱼巣/领域/控制面板服务.h:397, 海中鱼巣/领域/控制面板服务.h:427</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0993 | R0307 | R0321 | <code>海中鱼巣/领域/控制面板服务.h:328</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0994 | R0307 | R0318 | <code>海中鱼巣/领域/控制面板服务.h:339, 海中鱼巣/领域/控制面板服务.h:342</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0995 | R0307 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:346, 海中鱼巣/领域/控制面板服务.h:380</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0996 | R0307 | R0283 | <code>海中鱼巣/领域/控制面板服务.h:352, 海中鱼巣/领域/控制面板服务.h:359</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0997 | R0307 | R0284 | <code>海中鱼巣/领域/控制面板服务.h:361</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0998 | R0307 | R0317 | <code>海中鱼巣/领域/控制面板服务.h:381</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0999 | R0307 | R0326 | <code>海中鱼巣/领域/控制面板服务.h:406</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1000 | R0307 | R0278 | <code>海中鱼巣/领域/控制面板服务.h:409</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1001 | R0307 | R0287 | <code>海中鱼巣/领域/控制面板服务.h:412</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1002 | R0307 | R0289 | <code>海中鱼巣/领域/控制面板服务.h:415</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1003 | R0307 | R0292 | <code>海中鱼巣/领域/控制面板服务.h:418</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1004 | R0307 | R0320 | <code>海中鱼巣/领域/控制面板服务.h:434</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1005 | R0308 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:555</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1006 | R0312 | R0311 | <code>海中鱼巣/领域/控制面板服务.h:614</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1007 | R0313 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:626</code> | standard-library-callback |  |  | std::sort 注册具名比较函数 | 专项源码静态类型与实际装配人工复核 |
| RCE1008 | R0317 | R0316 | <code>海中鱼巣/领域/控制面板服务.h:665, 海中鱼巣/领域/控制面板服务.h:666, 海中鱼巣/领域/控制面板服务.h:667, 海中鱼巣/领域/控制面板服务.h:669, 海中鱼巣/领域/控制面板服务.h:670, 海中鱼巣/领域/控制面板服务.h:671, 海中鱼巣/领域/控制面板服务.h:674, 海中鱼巣/领域/控制面板服务.h:675, 海中鱼巣/领域/控制面板服务.h:676</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1009 | R0318 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:696</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1011 | R0318 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:703</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1012 | R0320 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:732, 海中鱼巣/领域/控制面板服务.h:740</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1013 | R0320 | R0296 | <code>海中鱼巣/领域/控制面板服务.h:734</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1014 | R0320 | R0299 | <code>海中鱼巣/领域/控制面板服务.h:735</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1015 | R0320 | R0301 | <code>海中鱼巣/领域/控制面板服务.h:738</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1016 | R0321 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:763</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1017 | R0321 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:765, 海中鱼巣/领域/控制面板服务.h:771, 海中鱼巣/领域/控制面板服务.h:792, 海中鱼巣/领域/控制面板服务.h:811, 海中鱼巣/领域/控制面板服务.h:822, 海中鱼巣/领域/控制面板服务.h:827, 海中鱼巣/领域/控制面板服务.h:834, 海中鱼巣/领域/控制面板服务.h:836, 海中鱼巣/领域/控制面板服务.h:839, 海中鱼巣/领域/控制面板服务.h:844, 海中鱼巣/领域/控制面板服务.h:850, 海中鱼巣/领域/控制面板服务.h:854, 海中鱼巣/领域/控制面板服务.h:859</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1018 | R0321 | R0515 | <code>海中鱼巣/领域/控制面板服务.h:767</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1019 | R0321 | F0355 | <code>海中鱼巣/领域/控制面板服务.h:768</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1020 | R0321 | R0516 | <code>海中鱼巣/领域/控制面板服务.h:769</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1021 | R0321 | R0322 | <code>海中鱼巣/领域/控制面板服务.h:773</code> | standard-library-callback |  |  | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1022 | R0321 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:780, 海中鱼巣/领域/控制面板服务.h:806</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1023 | R0321 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:788</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1024 | R0321 | R0315 | <code>海中鱼巣/领域/控制面板服务.h:795</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1025 | R0321 | R0323 | <code>海中鱼巣/领域/控制面板服务.h:808</code> | standard-library-callback |  |  | std::all_of 注册局部判定 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1026 | R0321 | R0317 | <code>海中鱼巣/领域/控制面板服务.h:824</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1027 | R0321 | R0517 | <code>海中鱼巣/领域/控制面板服务.h:829</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1028 | R0321 | R0530 | <code>海中鱼巣/领域/控制面板服务.h:831</code> | source-static-type |  |  | 抽象树读回已返回 | 专项源码静态类型与实际装配人工复核 |
| RCE1029 | R0321 | R0279 | <code>海中鱼巣/领域/控制面板服务.h:857</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1030 | R0321 | R0320 | <code>海中鱼巣/领域/控制面板服务.h:868</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1031 | R0322 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:778</code> | direct-static-member |  |  | 根类别相等 | 专项源码静态类型与实际装配人工复核 |
| RCE1032 | R0323 | R0303 | <code>海中鱼巣/领域/控制面板服务.h:809</code> | direct-const-member |  |  | 逐个概念根选项 | 专项源码静态类型与实际装配人工复核 |
| RCE1033 | R0324 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:879</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1034 | R0324 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:883</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1035 | R0324 | R0312 | <code>海中鱼巣/领域/控制面板服务.h:891</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1036 | R0324 | R0327 | <code>海中鱼巣/领域/控制面板服务.h:896</code> | direct-free |  |  | 形成关系角色人读文本 | 专项源码静态类型与实际装配人工复核 |
| RCE1037 | R0325 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:908, 海中鱼巣/领域/控制面板服务.h:911, 海中鱼巣/领域/控制面板服务.h:916</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1039 | R0325 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:914</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1040 | R0325 | R0312 | <code>海中鱼巣/领域/控制面板服务.h:917</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1041 | R0325 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:920</code> | direct-recursive |  |  | 对子节点递归规范化 | 专项源码静态类型与实际装配人工复核 |
| RCE1042 | R0326 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:938</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1043 | R0326 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:967</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1044 | R0328 | F0496 | <code>海中鱼巣/领域/控制面板服务.h:986</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1045 | R0328 | F0497 | <code>海中鱼巣/领域/控制面板服务.h:987</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1046 | R0328 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:988, 海中鱼巣/领域/控制面板服务.h:989</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1047 | R0328 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:991, 海中鱼巣/领域/控制面板服务.h:998</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1048 | R0328 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:992, 海中鱼巣/领域/控制面板服务.h:999</code> | source-static-type |  |  | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1050 | R0331 | R0333 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:135</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1051 | R0331 | F0231 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:138</code> | direct_const_member |  |  | 四项入口前置通过后读取参数稳定键组 | 专项源码静态类型与实际装配人工复核 |
| RCE1052 | R0331 | R0407 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:142</code> | direct-member |  |  | 逐个主键状态材料复核完整抽象状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1053 | R0331 | R0334 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:146</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1054 | R0331 | F0339 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:151</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1055 | R0331 | R0332 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:152</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1056 | R0331 | R0338 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:155</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1057 | R0331 | R0078 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:158</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1058 | R0331 | R0073 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:169</code> | direct-const-member |  |  | 概念活动审计读取令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1059 | R0333 | R0542 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:198</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1060 | R0335 | F0231 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:216</code> | direct_const_member |  |  | 函数进入后读取参数稳定键组并逐主键复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1061 | R0335 | R0063 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:217, 海中鱼巣/领域/数据操作.概念活动.ixx:227</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1062 | R0335 | R0030 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:228</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1063 | R0335 | R0031 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:229</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1064 | R0335 | R0540 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:230</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1065 | R0335 | R0541 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:231</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1066 | R0335 | R0036 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:236</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1067 | R0335 | R0037 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:236</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1068 | R0335 | R0026 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:239</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1069 | R0336 | R0025 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:254</code> | direct-member |  |  | 读取关系材料会话入口读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1070 | R0336 | R0038 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:259</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1071 | R0337 | R0034 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:270</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1072 | R0337 | R0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:280</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1073 | R0338 | R0540 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:293</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1074 | R0338 | R0541 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:294</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1075 | R0338 | F0441 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:298</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1076 | R0340 | R0339 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:126</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1077 | R0340 | R0333 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:57</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1078 | R0340 | R0332 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:59</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1079 | R0340 | R0116 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:63</code> | direct-const-member |  |  | 外层入口前置通过并形成 std::function&lt;void(结构写入会话&amp;)&gt; | 专项源码静态类型与实际装配人工复核 |
| RCE1080 | R0341 | R0337 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:110</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1081 | R0341 | R0041 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:119</code> | direct-member |  |  | 候选材料完整后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1082 | R0341 | R0335 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:64</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1083 | R0341 | R0403 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:69, 海中鱼巣/领域/数据操作.概念活动.ixx:70, 海中鱼巣/领域/数据操作.概念活动.ixx:71</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1084 | R0341 | R0334 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:81, 海中鱼巣/领域/数据操作.概念活动.ixx:82, 海中鱼巣/领域/数据操作.概念活动.ixx:83</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1085 | R0341 | R0023 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:87</code> | direct-member |  |  | 全新前置及三项状态写入成功后创建根组关系 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1086 | R0341 | R0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:96</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1087 | R0343 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:104, 海中鱼巣/领域/数据操作.特征体系.ixx:105</code> | direct |  |  | 复核宿主、特征定义和实例槽位节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1088 | R0343 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:105</code> | direct |  |  | 复核槽位主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1089 | R0343 | R0387 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:106</code> | direct_const_member |  |  | 复核宿主关系和模板关系证据 | 专项源码静态类型与实际装配人工复核 |
| RCE1090 | R0344 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1028</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1091 | R0344 | R0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1035</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1092 | R0345 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1044</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1093 | R0346 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1054, 海中鱼巣/领域/数据操作.特征体系.ixx:1055</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1094 | R0347 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1073</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1095 | R0348 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1085</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1096 | R0348 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1086</code> | source-audited |  |  | 身份静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1097 | R0348 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1094</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1098 | R0348 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1098</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1099 | R0348 | R0354 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1099</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1100 | R0349 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1112, 海中鱼巣/领域/数据操作.特征体系.ixx:1139</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1101 | R0349 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1113, 海中鱼巣/领域/数据操作.特征体系.ixx:1140</code> | source-audited |  |  | 身份与宿主静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1102 | R0349 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1121</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1103 | R0349 | R0355 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1122, 海中鱼巣/领域/数据操作.特征体系.ixx:1131, 海中鱼巣/领域/数据操作.特征体系.ixx:1133, 海中鱼巣/领域/数据操作.特征体系.ixx:1136, 海中鱼巣/领域/数据操作.特征体系.ixx:1138, 海中鱼巣/领域/数据操作.特征体系.ixx:1141</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1104 | R0349 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1124</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1105 | R0349 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1130, 海中鱼巣/领域/数据操作.特征体系.ixx:1137</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1106 | R0349 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1132</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1107 | R0349 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1133</code> | source-audited |  |  | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1108 | R0349 | R0079 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1135</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1109 | R0349 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1140</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1110 | R0350 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1161, 海中鱼巣/领域/数据操作.特征体系.ixx:1177</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1111 | R0350 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1166</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1112 | R0350 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1167</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1113 | R0350 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1168</code> | source-audited |  |  | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1114 | R0350 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1174</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1115 | R0350 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1175</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1116 | R0350 | R0355 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1176, 海中鱼巣/领域/数据操作.特征体系.ixx:1178, 海中鱼巣/领域/数据操作.特征体系.ixx:1184</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1117 | R0350 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1180</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1118 | R0350 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1182</code> | source-audited |  |  | 槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1119 | R0351 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1199</code> | source-audited |  |  | 槽位形参静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1120 | R0351 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1203</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1121 | R0351 | R0356 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1209, 海中鱼巣/领域/数据操作.特征体系.ixx:1211, 海中鱼巣/领域/数据操作.特征体系.ixx:1214, 海中鱼巣/领域/数据操作.特征体系.ixx:1217, 海中鱼巣/领域/数据操作.特征体系.ixx:1224, 海中鱼巣/领域/数据操作.特征体系.ixx:1235, 海中鱼巣/领域/数据操作.特征体系.ixx:1238</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1122 | R0351 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1210</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1123 | R0351 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1212</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1124 | R0351 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1213</code> | source-audited |  |  | 读取身份结果静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1125 | R0351 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1216</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1126 | R0351 | R0266 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1219</code> | direct-const-member |  |  | 读取当前值时读取原始材料侧表 | 专项源码静态类型与实际装配人工复核 |
| RCE1127 | R0351 | R0268 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1224</code> | direct-const-member |  |  | 侧表快照静态类型复核可读性 | 专项源码静态类型与实际装配人工复核 |
| RCE1128 | R0352 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:125, 海中鱼巣/领域/数据操作.特征体系.ixx:126</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1129 | R0353 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1311, 海中鱼巣/领域/数据操作.特征体系.ixx:1313</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1130 | R0353 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1313</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1131 | R0353 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1314, 海中鱼巣/领域/数据操作.特征体系.ixx:1316, 海中鱼巣/领域/数据操作.特征体系.ixx:1319, 海中鱼巣/领域/数据操作.特征体系.ixx:1321, 海中鱼巣/领域/数据操作.特征体系.ixx:1323</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1132 | R0353 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1314</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1133 | R0353 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1316</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1134 | R0353 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1317</code> | source-audited |  |  | 读取身份结果静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1135 | R0353 | R0079 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1319</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1136 | R0353 | R0356 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1320, 海中鱼巣/领域/数据操作.特征体系.ixx:1325</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1137 | R0353 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1321</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1138 | R0353 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1322</code> | source-audited |  |  | 读取槽位结果静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1139 | R0353 | R0357 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1322</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1140 | R0353 | R0351 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1323</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1141 | R0353 | R0352 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1324</code> | source-audited |  |  | 输出静态类型为特征原始值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1142 | R0354 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1387</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1143 | R0355 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1395</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1144 | R0356 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1403</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1145 | R0358 | R0021 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1446</code> | direct_member | this=&amp;会话 | 带值结构写入结果&lt;主信息句柄&gt;绑定主结果 | 进入函数并完成形参绑定后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1146 | R0358 | R0022 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1449</code> | direct_member | this=&amp;会话; 类型, 新主信息 | 带值结构写入结果&lt;节点句柄&gt;绑定节点结果 | R0640 返回 true 且新主信息已赋值 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1147 | R0358 | R0027 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1452-1453</code> | direct_member | this=&amp;会话; 形成索引绑定请求(...) | 结构写入结果临时值供 R0138 检查成功 | R0129 已形成特征体系索引绑定请求 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1148 | R0358 | R0129 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1452-1453</code> | direct_free_function | 主键, 新节点, 索引所有者::特征体系, 0 | 索引绑定请求直接传给 R0027 | R0641 返回 true 且新节点已赋值 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1149 | R0358 | R0036 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1454</code> | direct_member | this=&amp;会话; 新主信息 | bool 作为最终 &amp;&amp; 第一项 | 绑定主键成功 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1150 | R0358 | R0037 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1454</code> | direct_member | this=&amp;会话; 新节点 | bool 作为最终 &amp;&amp; 第二项 | R0036 返回 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1152 | R0359 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1462</code> | direct_const_member | this=&amp;定义 | bool 作为 &amp;&amp; 第二项 | R0388 返回 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1153 | R0359 | R0030 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1463, 海中鱼巣/领域/数据操作.特征体系.ixx:1465</code> | direct_member | this=&amp;会话; 宿主.节点; this=&amp;会话; 定义.特征定义 | 两个 std::optional&lt;节点类型&gt; 分别参与相等比较 | 按 &amp;&amp; 顺序分别在前序项全部为 true 时到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1154 | R0359 | R0031 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1464, 海中鱼巣/领域/数据操作.特征体系.ixx:1466</code> | direct_member | this=&amp;会话; 宿主.节点; this=&amp;会话; 定义.特征定义 | 两个 std::optional&lt;主信息句柄&gt; 分别参与相等比较 | 按 &amp;&amp; 顺序分别在前序项全部为 true 时到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1156 | R0359 | R0034 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1468</code> | direct_member | this=&amp;会话; 定义.特征定义, 关系类型::模板 | std::vector&lt;关系记录&gt; 临时值供 empty() 检查 | R0638 返回 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1157 | R0360 | R0034 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1475, 海中鱼巣/领域/数据操作.特征体系.ixx:1478</code> | direct_member | this=&amp;会话; 宿主, 关系类型::归属; this=&amp;会话; 宿主关系.目标节点, 关系类型::模板 | 外层临时 vector 供范围遍历；内层 vector 绑定 const 模板组 | 入口即取外层组；内层仅目标节点类型为特征时到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1158 | R0360 | R0030 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1476</code> | direct_member | this=&amp;会话; 宿主关系.目标节点 | std::optional&lt;节点类型&gt; 与特征类型 optional 比较 | 外层范围遍历取得当前宿主关系 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1159 | R0361 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1505-1506</code> | direct_static_member | 会话, 节点类型::特征, 规格.读取幂等主键(), 新主信息, 新槽位 | bool 取反用于 R0361 失败返回 false | R0361 进入首个写入步骤 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1160 | R0361 | R0371 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1506, 海中鱼巣/领域/数据操作.特征体系.ixx:1515</code> | direct_const_member | this=&amp;规格 | uint64_t 分别供 R0358 主键与输出幂等主键 | 入口写入步骤；输出赋值仅完整关系复核成功后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1161 | R0361 | R0023 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1507-1508, 海中鱼巣/领域/数据操作.特征体系.ixx:1510-1511</code> | direct_member | this=&amp;会话; 关系类型::归属, 规格.读取宿主(), 新槽位; this=&amp;会话; 关系类型::模板, 新槽位, 规格.读取特征定义() | 两个带值结构写入结果&lt;关系句柄&gt;分别绑定宿主关系与模板关系 | R0358 成功；模板关系仅宿主关系成功后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1162 | R0361 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1508, 海中鱼巣/领域/数据操作.特征体系.ixx:1516</code> | direct_const_member | this=&amp;规格 | 节点句柄分别作为宿主关系源节点与输出宿主 | 按调用顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1163 | R0361 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1511, 海中鱼巣/领域/数据操作.特征体系.ixx:1517</code> | direct_const_member | this=&amp;规格 | 节点句柄分别作为模板关系目标节点与输出特征定义 | 按调用顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1164 | R0361 | R0038 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1513</code> | direct_member | this=&amp;会话; *宿主关系.值; this=&amp;会话; *模板关系.值 | 两个 bool 参与 \|\|/&amp;&amp; 短路检查 | 两个带值关系结果均成功；第二次另受第一次关系可读返回 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1165 | R0362 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1533-1534</code> | direct_static_member | 会话, 节点类型::特征值, 规格.读取幂等主键(), 新主信息, 新值 | bool 取反用于失败返回 false | 进入 R0362 写入步骤 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1166 | R0362 | R0375 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1534, 海中鱼巣/领域/数据操作.特征体系.ixx:1549</code> | direct_const_member | this=&amp;规格 | uint64_t 分别供 R0358 主键和输出幂等主键 | 入口写入步骤；1549 仅完整复核成功后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1167 | R0362 | R0023 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1535</code> | direct_member | this=&amp;会话; 关系类型::归属, 实例槽位, 新值 | 带值结构写入结果&lt;关系句柄&gt;绑定当前关系 | R0358 成功后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1168 | R0362 | R0376 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1537, 海中鱼巣/领域/数据操作.特征体系.ixx:1540, 海中鱼巣/领域/数据操作.特征体系.ixx:1544, 海中鱼巣/领域/数据操作.特征体系.ixx:1554</code> | direct_const_member | this=&amp;规格 | 特征值原始类型分别用于 I64 分支、材料构造、复核分支与输出 | 按前序短路与赋值顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1169 | R0362 | R0028 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1538</code> | direct_member | this=&amp;会话; 新主信息, 0, *规格.读取I64值() | 结构写入结果临时值供 R0138 检查成功 | 原始类型为 I64 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1170 | R0362 | R0377 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1538, 海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1545, 海中鱼巣/领域/数据操作.特征体系.ixx:1556</code> | direct_const_member | this=&amp;规格 | const optional&lt;int64_t&gt;&amp; 分别供候选写入解引用、材料构造、候选匹配解引用与输出 | 按前序短路与赋值顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1171 | R0362 | R0378 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1557</code> | direct_const_member | this=&amp;规格 | const vector&lt;int64_t&gt;&amp; 供材料构造和输出 | 按赋值顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1172 | R0362 | R0379 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1558</code> | direct_const_member | this=&amp;规格 | const vector&lt;uint64_t&gt;&amp; 供材料构造和输出 | 按赋值顺序到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1173 | R0362 | R0270 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1542</code> | direct_member | this=&amp;参与者; std::move(材料) | bool 取反用于登记失败返回 false | 材料已构造 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1174 | R0362 | R0038 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1543</code> | direct_member | this=&amp;会话; *当前关系.值 | bool 绑定完整 | 材料登记成功 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1175 | R0362 | R0029 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1545</code> | direct_member | this=&amp;会话; 新主信息, 0, *规格.读取I64值() | bool 作为 &amp;&amp; 左项并与完整合并 | 原始类型为 I64 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1176 | R0363 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1565</code> | direct_const_member | this=&amp;材料 | bool 作为 &amp;&amp; 首项 | 进入返回表达式 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1177 | R0363 | R0371 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1565</code> | direct_const_member | this=&amp;规格 | uint64_t 与材料幂等主键比较 | 材料完整 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1178 | R0363 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1566</code> | direct_const_member | this=&amp;规格 | 节点句柄与材料宿主比较 | 材料完整且幂等主键相等 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1179 | R0363 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1567</code> | direct_const_member | this=&amp;规格 | 节点句柄与材料特征定义比较 | 前序宿主比较为 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1180 | R0364 | R0352 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1573</code> | direct_const_member | this=&amp;材料 | bool 作为 &amp;&amp; 首项 | 进入返回表达式 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1181 | R0364 | R0375 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1573</code> | direct_const_member | this=&amp;规格 | uint64_t 与材料幂等主键比较 | 材料完整 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1182 | R0364 | R0376 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1574</code> | direct_const_member | this=&amp;规格 | 原始类型与材料类型比较 | 材料完整且主键相等 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1183 | R0364 | R0377 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1575</code> | direct_const_member | this=&amp;规格 | const optional&lt;int64_t&gt;&amp; 供标准库相等比较 | 完整、主键、原始类型匹配且版本为 1 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1184 | R0364 | R0378 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1576</code> | direct_const_member | this=&amp;规格 | const vector&lt;int64_t&gt;&amp; 供标准库相等比较 | 前序含 I64 相等的全部项为 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1185 | R0364 | R0379 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1577</code> | direct_const_member | this=&amp;规格 | const vector&lt;uint64_t&gt;&amp; 供标准库相等比较 | 前序含 VecI64 相等的全部项为 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1186 | R0368 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1650, 海中鱼巣/领域/数据操作.特征体系.ixx:1653, 海中鱼巣/领域/数据操作.特征体系.ixx:1656</code> | direct_free_function | false, L"特征体系业务准入后结构会话未形成提交决定。"; false, L"特征体系结构写入或参与者收口内部不一致。"; false, L"特征体系结构会话返回不允许的收口状态。" | 三处 bool 返回值均以 (void) 显式丢弃 | 结果状态分别为入口拒绝、内部不一致及 switch default | 冻结源码 3920a746 + 1643-1657 行逐调用点、同命名空间自由函数与 F0184 完整签名复核 |
| RCE1187 | R0374 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:273</code> | direct_free_function | 宿主_（第一次）, 特征定义_（第二次） | bool（两次） | 幂等主键非零后按短路顺序复核宿主和特征定义句柄 | 冻结源码 3920a746 + R0374 现状图与逐行映射静态重载复核 |
| RCE1188 | R0381 | R0374 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:480</code> | direct_const_member | this=&amp;局部特征槽位写入规格 规格 | bool，控制返回非空 optional 或 nullopt | 局部规格已由幂等主键、宿主和特征定义构造 | 冻结源码 3920a746 + 接收者静态类型与完整签名复核 |
| RCE1189 | R0382 | R0380 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:493</code> | direct_const_member | this=&amp;局部初始特征值写入规格 规格 | bool，控制返回非空 optional 或 nullopt | 局部规格已由五项输入构造 | 冻结源码 3920a746 + 接收者静态类型与完整签名复核 |
| RCE1190 | R0383 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:564</code> | direct_free_function | 目标 | bool，取反参与入口拒绝 | F0444 返回 true 后 | 冻结源码 3920a746 + 实参静态类型与重载签名复核 |
| RCE1191 | R0383 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:564</code> | direct_const_member | this | bool，取反参与入口拒绝 | 进入 R0383 前置表达式 | 冻结源码 3920a746 + 调用点接收者静态类型复核 |
| RCE1192 | R0383 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:566</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1193 | R0383 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:567</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1194 | R0383 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:567</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1195 | R0384 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:594</code> | direct_const_member | this | bool，取反参与入口拒绝 | 进入 R0384 前置表达式 | 冻结源码 3920a746 + 调用点接收者静态类型复核 |
| RCE1196 | R0384 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:596</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1197 | R0384 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:597, 海中鱼巣/领域/数据操作.特征体系.ixx:599</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1198 | R0384 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:597</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1199 | R0384 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:599</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1201 | R0385 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:603</code> | direct_const_member | this | bool，取反参与入口拒绝 | 进入 R0385 前置表达式 | 冻结源码 3920a746 + 调用点接收者静态类型复核 |
| RCE1202 | R0385 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:605</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1203 | R0385 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:606</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1204 | R0385 | R0350 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:606</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1206 | R0386 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:610</code> | direct_const_member | this | bool，取反参与入口拒绝 | 进入 R0386 前置表达式 | 冻结源码 3920a746 + 调用点接收者静态类型复核 |
| RCE1207 | R0386 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:612</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1208 | R0386 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:613, 海中鱼巣/领域/数据操作.特征体系.ixx:615</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1209 | R0386 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:613</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1210 | R0386 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:614</code> | source-audited |  |  | 槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1211 | R0386 | R0357 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:614</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1212 | R0386 | R0351 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:615</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1213 | R0387 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:64</code> | direct |  |  | 特征关系证据复核源和目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1214 | R0387 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:64</code> | unique_name+direct |  |  | 调用方可达且源码分支条件成立；特征关系证据复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1216 | R0389 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1217 | R0389 | R0374 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | source-audited |  |  | 槽位规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1218 | R0389 | R0380 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | source-audited |  |  | 初始值规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1219 | R0389 | R0383 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:837</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1220 | R0389 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:839</code> | source-audited |  |  | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1221 | R0389 | R0366 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:840</code> | direct_static_member | 宿主身份, 定义 | R0366 的特征体系业务结果直接作为 R0389 当前 return 返回 | !宿主身份.当前可读() \|\| !定义.完整() | 冻结源码 3920a746 + 调用方 837-840 行、同类静态成员与 R0366 完整签名复核 |
| RCE1222 | R0389 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:842</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1223 | R0389 | R0385 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:844, 海中鱼巣/领域/数据操作.特征体系.ixx:860, 海中鱼巣/领域/数据操作.特征体系.ixx:900</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1224 | R0389 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:846, 海中鱼巣/领域/数据操作.特征体系.ixx:862, 海中鱼巣/领域/数据操作.特征体系.ixx:902</code> | source-audited |  |  | 写前与写后槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1225 | R0389 | R0386 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:847, 海中鱼巣/领域/数据操作.特征体系.ixx:863, 海中鱼巣/领域/数据操作.特征体系.ixx:903</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1226 | R0389 | R0363 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:848, 海中鱼巣/领域/数据操作.特征体系.ixx:865, 海中鱼巣/领域/数据操作.特征体系.ixx:905</code> | direct_static_member | 已有槽位, 槽位规格; 并发槽位, 槽位规格; 当前槽位, 槽位规格 | bool 作为后续 R0364 的短路左项 | 分别位于幂等读、并发读与写后复核分支 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1227 | R0389 | R0364 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:848, 海中鱼巣/领域/数据操作.特征体系.ixx:865, 海中鱼巣/领域/数据操作.特征体系.ixx:905</code> | direct_static_member | 当前值, 值规格; 并发值, 值规格; 当前值, 值规格 | bool 作为组合匹配结果 | 对应 R0363 匹配返回 true 后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1228 | R0389 | R0365 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:849, 海中鱼巣/领域/数据操作.特征体系.ixx:866, 海中鱼巣/领域/数据操作.特征体系.ixx:906</code> | direct_static_member | 幂等读回, 已有槽位, 当前值; 幂等读回, 并发槽位, 并发值; (结构结果已提交?已提交:幂等读回), 当前槽位, 当前值 | 特征体系业务结果直接从调用方返回 | 分别在对应两项匹配成功时到达 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1229 | R0389 | R0367 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:854</code> | direct_static_member | 已有槽位.状态 | R0367 结果直接作为 R0389 当前 return 返回 | 已有槽位.状态 != 特征体系读取状态::未找到，且前序已有槽位.完整()分支未返回 | 冻结源码 3920a746 + 调用方 844-854 行、同类静态成员与 R0367 完整签名复核 |
| RCE1230 | R0389 | R0384 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:856</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1231 | R0389 | R0353 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:857</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1232 | R0389 | R0269 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:884</code> | constructor |  |  | 创建槽位并发布初始状态时构造本次原始材料事务参与者 | 专项源码静态类型与实际装配人工复核 |
| RCE1233 | R0389 | R0103 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:885</code> | direct-template-member |  |  | 外层入口前置通过并形成结构写入会话回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1234 | R0389 | R0368 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:915</code> | direct_static_member | 结构结果 | R0368 结果直接作为 R0389 当前 return 返回 | 写后槽位/原始值组合匹配不成立、当前槽位状态非已找到，且版本已经漂移为 false | 冻结源码 3920a746 + 调用方 883-915 行、同类静态成员与 R0368 完整签名复核 |
| RCE1235 | R0390 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:88</code> | direct |  |  | 复核特征定义节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1236 | R0390 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:88</code> | direct |  |  | 复核特征定义主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1237 | R0391 | R0359 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:886</code> | direct_static_member | 会话, 宿主身份, 定义 | bool 作为 if 中首个否定项 | 执行器进入结构写入 lambda | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1238 | R0391 | R0360 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:887-888</code> | direct_static_member | 会话, 槽位规格.读取宿主(), 槽位规格.读取特征定义() | bool 作为 \|\| 第二项并取反控制拒绝 | R0359 返回 true 后 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1239 | R0391 | R0361 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:893</code> | direct_static_member | 会话, 槽位规格, 新槽位 | bool 取反用于 lambda 提前返回 | 验证宿主定义通过且不存在并发槽位 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1240 | R0391 | R0362 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:895</code> | direct_static_member | 会话, 参与者, 新槽位.实例槽位, 值规格, 新值 | bool 取反用于 R0391 lambda 提前返回 | 槽位候选写入成功 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE1241 | R0391 | R0041 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:897</code> | source-audited |  |  | lambda 形参静态类型为结构写入会话 | 专项源码静态类型与实际装配人工复核 |
| RCE1242 | R0392 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1014</code> | direct_free_overload_static_type | 节点 | 节点句柄有效性布尔条件 | 调用方可达且进入节点句柄前置检查 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1243 | R0392 | R0394 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1021</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1244 | R0393 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:103</code> | direct_free_overload_static_type | 场景、主体；同一行两次调用 | 场景与主体句柄有效性布尔条件 | 当前可读、实例状态种类和非零时间戳短路条件成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1245 | R0394 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1030</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1246 | R0395 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1040, 海中鱼巣/领域/数据操作.状态动态.ixx:1041</code> | direct_free_overload_static_type | 记录.源节点、记录.目标节点 | 源节点与目标节点有效性布尔条件 | 调用方可达且检查输入关系记录前置 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1247 | R0396 | R0392 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1058, 海中鱼巣/领域/数据操作.状态动态.ixx:1110</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1248 | R0396 | R0397 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1069, 海中鱼巣/领域/数据操作.状态动态.ixx:1074, 海中鱼巣/领域/数据操作.状态动态.ixx:1088, 海中鱼巣/领域/数据操作.状态动态.ixx:1098, 海中鱼巣/领域/数据操作.状态动态.ixx:1102, 海中鱼巣/领域/数据操作.状态动态.ixx:1109, 海中鱼巣/领域/数据操作.状态动态.ixx:1111, 海中鱼巣/领域/数据操作.状态动态.ixx:1113, 海中鱼巣/领域/数据操作.状态动态.ixx:1116, 海中鱼巣/领域/数据操作.状态动态.ixx:1121, 海中鱼巣/领域/数据操作.状态动态.ixx:1129, 海中鱼巣/领域/数据操作.状态动态.ixx:1130, 海中鱼巣/领域/数据操作.状态动态.ixx:1133</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1249 | R0396 | R0082 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1087</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1250 | R0396 | R0408 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1100</code> | direct_static_member | *低位, *高位 | const auto 槽位主键 | 时间戳为正、低位和高位均存在且各自在 0—0xffff'ffffULL 范围内 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1251 | R0396 | R0078 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1107, 海中鱼巣/领域/数据操作.状态动态.ixx:1125</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1252 | R0396 | R0395 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1108, 海中鱼巣/领域/数据操作.状态动态.ixx:1128</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1253 | R0397 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1326</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1254 | R0398 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:159</code> | direct_free_overload_static_type | 场景、主体；同一行两次调用 | 场景与主体句柄有效性布尔条件 | 当前可读、抽象动态种类和零时间戳短路条件成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1255 | R0401 | R0393 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:306</code> | source-audited |  |  | 状态写入参与结果材料静态类型复核实例状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1256 | R0401 | R0407 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:306</code> | source-audited |  |  | 状态写入参与结果材料静态类型复核抽象状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1257 | R0402 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:377</code> | direct-free |  |  | 入口复核状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1259 | R0402 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:377</code> | direct_free_overload_static_type | 状态节点 | 节点句柄有效性布尔条件 | 调用方可达且进入状态节点前置检查 | 冻结源码逐调用点、静态类型与现状图复核 |
| RCE1260 | R0402 | F0397 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:378</code> | resolved-function-pointer |  |  | 生产运行期唯一共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1261 | R0402 | F0338 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:379</code> | direct-const-member |  |  | 取得共享许可后复核有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1262 | R0402 | F0339 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:380</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；许可有效后读取令牌 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1263 | R0402 | R0396 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:380</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；调用已许可状态材料读取 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1264 | R0403 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:409</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1265 | R0403 | R0405 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:411</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1266 | R0403 | R0400 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:412</code> | direct-member |  |  | 规格完整后读取状态值作为结构写入实参 | 专项源码静态类型与实际装配人工复核 |
| RCE1267 | R0404 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:60</code> | direct |  |  | 关系值式证据复核源和目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1268 | R0404 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:60</code> | unique_name+direct |  |  | 调用方可达且源码分支条件成立；关系值式证据复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1269 | R0405 | R0021 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:902</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1270 | R0405 | R0028 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:905</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1271 | R0405 | R0022 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:906</code> | direct-member |  |  | 主键和主信息候选写入成功后创建节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE1272 | R0405 | R0027 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:909</code> | direct-member |  |  | 节点候选成功后绑定主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1273 | R0405 | R0129 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:909</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1274 | R0405 | R0036 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:911</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1275 | R0405 | R0029 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:912</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1276 | R0405 | R0037 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:913</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1278 | R0405 | R0407 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:923</code> | direct_const_member |  |  | 抽象状态结构候选读回完成后复核输出完整抽象状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1279 | R0406 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:92</code> | direct_free_overload_static_type | 状态节点 | 状态节点句柄有效布尔条件 | 状态为已找到且幂等主键非零 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1280 | R0406 | F0565 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:92</code> | direct_free_overload_static_type | 主信息 | 主信息句柄有效布尔条件 | 状态、幂等主键和状态节点短路条件成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1281 | R0407 | R0406 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:96</code> | direct_const_member | this | 当前可读布尔条件 | 调用方可达并求值完整抽象状态 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1282 | R0407 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:97</code> | direct_free_overload_static_type | 场景、主体；同一行两次调用 | 场景与主体句柄有效性布尔条件，均取反进入合取 | 当前可读、抽象状态种类和零发生时间戳短路条件成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1283 | R0407 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:98</code> | direct_const_member | 场景临时关系、主体关系、场景关系；同一行三次调用 | 三项关系完整性布尔条件，均取反进入合取 | 前序抽象状态短路条件全部成立且场景、主体句柄均无效 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1284 | R0409 | R0116 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:132</code> | direct-const-member |  |  | 外层入口前置通过并形成 std::function&lt;void(结构写入会话&amp;)&gt; | 专项源码静态类型与实际装配人工复核 |
| RCE1285 | R0409 | R0421 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:172</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1286 | R0410 | R0415 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:133</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1287 | R0410 | R0417 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:134</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1288 | R0410 | R0418 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:139, 海中鱼巣/领域/数据操作.系统角色.ixx:145</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1289 | R0410 | R0041 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:152</code> | direct-member |  |  | 世界拓扑结构发生变化后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1290 | R0413 | F0441 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:237</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1291 | R0414 | R0030 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:247</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1292 | R0414 | R0031 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:248</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1293 | R0414 | R0036 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:249</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1294 | R0414 | R0033 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:250</code> | direct-member |  |  | 身份、类型、主信息和可读校验成立后读取节点主键组 | 专项源码静态类型与实际装配人工复核 |
| RCE1296 | R0415 | R0564 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:258</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1297 | R0415 | R0414 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:259</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1298 | R0417 | R0025 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:285</code> | direct-member |  |  | 系统角色关系材料完整后读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1299 | R0417 | R0038 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:289</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1300 | R0418 | R0035 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:298</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1301 | R0418 | R0038 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:311, 海中鱼巣/领域/数据操作.系统角色.ixx:329</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1302 | R0418 | R0024 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:319</code> | direct-member |  |  | 来源普通父子关系组为空时挂载或重挂节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1303 | R0418 | R0025 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:325</code> | direct-member |  |  | 挂载结果已创建且父关系存在后读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1304 | R0419 | R0079 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:342</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1305 | R0419 | R0416 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:351</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1306 | R0420 | R0078 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:360</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1307 | R0421 | F0339 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:378</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1308 | R0421 | R0564 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:379</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1309 | R0421 | R0413 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:380</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1310 | R0421 | R0420 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:384, 海中鱼巣/领域/数据操作.系统角色.ixx:388</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1311 | R0421 | R0416 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:392</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1312 | R0421 | R0419 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:398, 海中鱼巣/领域/数据操作.系统角色.ixx:400</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1313 | R0422 | R0411 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:45</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1314 | R0422 | F0339 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:51</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1315 | R0422 | F0441 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:53</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1316 | R0422 | R0412 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:76, 海中鱼巣/领域/数据操作.系统角色.ixx:77, 海中鱼巣/领域/数据操作.系统角色.ixx:78, 海中鱼巣/领域/数据操作.系统角色.ixx:79, 海中鱼巣/领域/数据操作.系统角色.ixx:97</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1317 | R0422 | R0420 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:88, 海中鱼巣/领域/数据操作.系统角色.ixx:91</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1318 | R0422 | R0079 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:98</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1319 | R0423 | F0446 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:126</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE1320 | R0423 | F0339 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:129, 海中鱼巣/领域/数据操作.轻量因果.ixx:131</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1321 | R0423 | F0441 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:129</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1322 | R0423 | R0425 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:131</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1323 | R0424 | F0441 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:208</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1324 | R0425 | R0426 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:230, 海中鱼巣/领域/数据操作.轻量因果.ixx:234</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1325 | R0425 | R0424 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:232</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1326 | R0426 | F0184 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:243</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1327 | R0428 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:112</code> | overload_resolved_free_function | 节点 | bool；参与当前可读短路合取 | 读取状态已找到 | 冻结源码实参静态类型与完整重载集合复核 |
| RCE1328 | R0429 | F0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1111, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1113</code> | suffix_match+source-audited |  |  | 调用方可达且源码分支条件成立；无对象限定调用当前数据操作::有效 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1329 | R0429 | F0338 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1113</code> | source-audited |  |  | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1330 | R0429 | F0339 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1114, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1116</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1331 | R0429 | F0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1114</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1332 | R0429 | R0442 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1116</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1333 | R0430 | F0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1160</code> | direct_const_member | this | bool；与主键非零组成入口门禁 | R0430入口执行 | 冻结源码隐式接收者、完整签名与调用位置复核 |
| RCE1334 | R0430 | F0339 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1163, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1165</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1335 | R0430 | F0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1163</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1336 | R0430 | R0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1165</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1337 | R0431 | R0454 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1214</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1338 | R0432 | R0430 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1621, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1649</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1339 | R0432 | R0460 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1623, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1650</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1340 | R0432 | R0461 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1628</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1341 | R0432 | R0116 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1632</code> | direct-const-member |  |  | 外层入口前置通过并形成 std::function&lt;void(结构写入会话&amp;)&gt; | 专项源码静态类型与实际装配人工复核 |
| RCE1342 | R0432 | R0462 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1658</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1343 | R0433 | R0451 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1634</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1344 | R0433 | R0403 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1635, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1636, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1637</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1345 | R0433 | R0452 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1643, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1644, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1645</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1346 | R0433 | R0036 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1646</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1347 | R0433 | R0037 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1646</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1349 | R0433 | R0041 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1647</code> | direct-member |  |  | 主信息、节点和主键绑定读回匹配后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1350 | R0434 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:191, 海中鱼巣/领域/数据操作.需求任务方法.ixx:192, 海中鱼巣/领域/数据操作.需求任务方法.ixx:193, 海中鱼巣/领域/数据操作.需求任务方法.ixx:194</code> | overload_resolved_free_function | 主体 / 目标宿主 / 场景 / 目标特征 / 当前特征状态材料 / 目标状态 | bool；参与完整目标状态需求短路合取 | 身份当前可读且类型为需求 | 冻结源码实参静态类型、完整重载集合与六次调用复核 |
| RCE1351 | R0435 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1940</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1352 | R0440 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2005, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2029</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1353 | R0440 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2006, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2030</code> | source-audited |  |  | 身份与目标静态类型均为高级节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1354 | R0440 | R0438 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2019</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1355 | R0440 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2026, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2046</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1356 | R0440 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2027, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2049</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1357 | R0440 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2032, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2036</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1358 | R0440 | R0439 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2066, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2067, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2068</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1359 | R0441 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2080, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2097, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2125, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2139</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1360 | R0441 | R0440 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2085</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1361 | R0441 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2095, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2123, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2137</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1362 | R0441 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2101, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2126, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2140</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1363 | R0441 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2104, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2109, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2114, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2131, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2145</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1364 | R0441 | R0444 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2171</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1365 | R0442 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2188, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2206, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2223, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2235, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2271, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2282</code> | unique_name | this=this；各调用点按源码分别绑定需求/目标/来源/父/结算目标节点、nullopt主键与令牌 | 高级节点身份分别绑定输出身份或局部身份 | 对应身份读取语句执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2188、海中鱼巣/领域/数据操作.需求任务方法.ixx:2206、海中鱼巣/领域/数据操作.需求任务方法.ixx:2223、海中鱼巣/领域/数据操作.需求任务方法.ixx:2235、海中鱼巣/领域/数据操作.需求任务方法.ixx:2271、海中鱼巣/领域/数据操作.需求任务方法.ixx:2282 与被调函数完整身份复核 |
| RCE1366 | R0442 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2203, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2233, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2259, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2281</code> | unique_name | this=&amp;关系_；按调用点绑定需求、关系类型与令牌 | vector&lt;关系记录&gt;进入对应循环或大小判断 | 对应关系聚合阶段执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2203、海中鱼巣/领域/数据操作.需求任务方法.ixx:2233、海中鱼巣/领域/数据操作.需求任务方法.ixx:2259、海中鱼巣/领域/数据操作.需求任务方法.ixx:2281 与被调函数完整身份复核 |
| RCE1367 | R0442 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2204, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2224, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2236, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2262, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2272</code> | suffix_match | this=this；记录与令牌 | optional&lt;高级关系证据&gt;绑定局部证据 | 对应关系记录已取得 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2204、海中鱼巣/领域/数据操作.需求任务方法.ixx:2224、海中鱼巣/领域/数据操作.需求任务方法.ixx:2236、海中鱼巣/领域/数据操作.需求任务方法.ixx:2262、海中鱼巣/领域/数据操作.需求任务方法.ixx:2272 与被调函数完整身份复核 |
| RCE1368 | R0442 | R0443 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2205, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2207, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2209, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2211, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2213, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2215, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2217, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2227, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2239, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2244, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2260, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2263, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2269, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2274, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2283, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2285, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2298</code> | unique_name | 输出 | 需求权威材料直接返回 | 通过身份前置后出现关系缺失、重复或内部结构不一致 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2205、海中鱼巣/领域/数据操作.需求任务方法.ixx:2207、海中鱼巣/领域/数据操作.需求任务方法.ixx:2209、海中鱼巣/领域/数据操作.需求任务方法.ixx:2211、海中鱼巣/领域/数据操作.需求任务方法.ixx:2213、海中鱼巣/领域/数据操作.需求任务方法.ixx:2215、海中鱼巣/领域/数据操作.需求任务方法.ixx:2217、海中鱼巣/领域/数据操作.需求任务方法.ixx:2227、海中鱼巣/领域/数据操作.需求任务方法.ixx:2239、海中鱼巣/领域/数据操作.需求任务方法.ixx:2244、海中鱼巣/领域/数据操作.需求任务方法.ixx:2260、海中鱼巣/领域/数据操作.需求任务方法.ixx:2263、海中鱼巣/领域/数据操作.需求任务方法.ixx:2269、海中鱼巣/领域/数据操作.需求任务方法.ixx:2274、海中鱼巣/领域/数据操作.需求任务方法.ixx:2283、海中鱼巣/领域/数据操作.需求任务方法.ixx:2285、海中鱼巣/领域/数据操作.需求任务方法.ixx:2298 与被调函数完整身份复核 |
| RCE1369 | R0442 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2209, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2211, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2213, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2215, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2217, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2226, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2238</code> | unique_name | 目标optional槽与关系证据 | bool决定继续或内部不一致 | 本轮关系匹配受支持端点槽 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2209、海中鱼巣/领域/数据操作.需求任务方法.ixx:2211、海中鱼巣/领域/数据操作.需求任务方法.ixx:2213、海中鱼巣/领域/数据操作.需求任务方法.ixx:2215、海中鱼巣/领域/数据操作.需求任务方法.ixx:2217、海中鱼巣/领域/数据操作.需求任务方法.ixx:2226、海中鱼巣/领域/数据操作.需求任务方法.ixx:2238 与被调函数完整身份复核 |
| RCE1370 | R0442 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2221, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2268</code> | unique_name | this=&amp;关系_；需求、关系类型与令牌 | vector&lt;关系记录&gt;进入来源关系循环或大小判断 | 需要反向宿主或父需求读取 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2221、海中鱼巣/领域/数据操作.需求任务方法.ixx:2268 与被调函数完整身份复核 |
| RCE1371 | R0442 | R0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2290</code> | unique_name | 需求、输出、唯一正式结算记录、令牌、正式结算输出引用 | 需求任务方法读取状态进入分支判断，正式结算按引用写入 | 存在唯一正式结算状态关系 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2290 与被调函数完整身份复核 |
| RCE1372 | R0444 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2317, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2334, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2351</code> | unique_name | this=this；任务或关系目标节点、nullopt主键、令牌 | 高级节点身份写入输出或局部目标 | 对应身份读取语句执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2317、海中鱼巣/领域/数据操作.需求任务方法.ixx:2334、海中鱼巣/领域/数据操作.需求任务方法.ixx:2351 与被调函数完整身份复核 |
| RCE1373 | R0444 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2333, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2349, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2358, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2387</code> | unique_name | this=&amp;关系_；任务、关系类型与令牌 | vector&lt;关系记录&gt;进入循环、大小或front读取 | 对应任务关系聚合阶段执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2333、海中鱼巣/领域/数据操作.需求任务方法.ixx:2349、海中鱼巣/领域/数据操作.需求任务方法.ixx:2358、海中鱼巣/领域/数据操作.需求任务方法.ixx:2387 与被调函数完整身份复核 |
| RCE1374 | R0444 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2335, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2352, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2367, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2392</code> | suffix_match | this=this；关系记录与令牌 | optional&lt;高级关系证据&gt;绑定局部证据 | 对应关系记录已取得 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2335、海中鱼巣/领域/数据操作.需求任务方法.ixx:2352、海中鱼巣/领域/数据操作.需求任务方法.ixx:2367、海中鱼巣/领域/数据操作.需求任务方法.ixx:2392 与被调函数完整身份复核 |
| RCE1375 | R0444 | R0445 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2336, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2338, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2340, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2342, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2344, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2346, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2355, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2360, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2362, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2368, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2370, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2389, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2393, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2396</code> | unique_name | 输出 | 任务权威材料直接返回 | 通过身份前置后出现关系缺失、重复或内部结构不一致 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2336、海中鱼巣/领域/数据操作.需求任务方法.ixx:2338、海中鱼巣/领域/数据操作.需求任务方法.ixx:2340、海中鱼巣/领域/数据操作.需求任务方法.ixx:2342、海中鱼巣/领域/数据操作.需求任务方法.ixx:2344、海中鱼巣/领域/数据操作.需求任务方法.ixx:2346、海中鱼巣/领域/数据操作.需求任务方法.ixx:2355、海中鱼巣/领域/数据操作.需求任务方法.ixx:2360、海中鱼巣/领域/数据操作.需求任务方法.ixx:2362、海中鱼巣/领域/数据操作.需求任务方法.ixx:2368、海中鱼巣/领域/数据操作.需求任务方法.ixx:2370、海中鱼巣/领域/数据操作.需求任务方法.ixx:2389、海中鱼巣/领域/数据操作.需求任务方法.ixx:2393、海中鱼巣/领域/数据操作.需求任务方法.ixx:2396 与被调函数完整身份复核 |
| RCE1376 | R0444 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2338, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2340, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2342, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2344, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2346, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2354, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2368</code> | unique_name | 目标optional槽与关系证据 | bool决定继续或内部不一致 | 本轮关系匹配受支持任务槽 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2338、海中鱼巣/领域/数据操作.需求任务方法.ixx:2340、海中鱼巣/领域/数据操作.需求任务方法.ixx:2342、海中鱼巣/领域/数据操作.需求任务方法.ixx:2344、海中鱼巣/领域/数据操作.需求任务方法.ixx:2346、海中鱼巣/领域/数据操作.需求任务方法.ixx:2354、海中鱼巣/领域/数据操作.需求任务方法.ixx:2368 与被调函数完整身份复核 |
| RCE1377 | R0444 | R0446 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2361</code> | unique_name | this=this；生命周期组.front()、令牌、默认审计false | optional&lt;任务生命周期材料&gt;绑定生命周期 | 生命周期组恰一条且四项必需关系存在 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2361 与被调函数完整身份复核 |
| RCE1378 | R0444 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2365</code> | unique_name | this=&amp;关系_；任务、归属、令牌 | vector&lt;关系记录&gt;进入授权循环 | 生命周期当前后读取需求拥有任务关系 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2365 与被调函数完整身份复核 |
| RCE1379 | R0444 | R0447 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2394</code> | unique_name | 发布目标节点、方法投影证据、发布证据、令牌 | optional&lt;任务方法选择材料&gt;绑定选择 | 发布组恰一条且方法投影存在 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2394 与被调函数完整身份复核 |
| RCE1380 | R0446 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2412</code> | suffix_match | 记录、令牌、审计 | optional&lt;高级关系证据&gt;绑定证据 | R0446进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2412 与被调函数完整身份复核 |
| RCE1381 | R0446 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2413</code> | unique_name | 记录目标节点、nullopt主键、令牌 | 高级节点身份绑定状态身份 | 证据读取完成 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2413 与被调函数完整身份复核 |
| RCE1382 | R0447 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2456</code> | unique_name | 记录节点、nullopt主键、令牌 | 高级节点身份绑定身份 | R0447进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2456 与被调函数完整身份复核 |
| RCE1383 | R0447 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2457</code> | source-audited | this=&amp;身份 | bool进入入口短路判断 | 身份已由R0435读取 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2457 与被调函数完整身份复核 |
| RCE1384 | R0448 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2517</code> | unique_name | 根、nullopt主键、令牌 | 高级节点身份写入输出身份 | R0448进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2517 与被调函数完整身份复核 |
| RCE1385 | R0448 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2529</code> | unique_name | this=&amp;关系_；根、模板、令牌 | vector&lt;关系记录&gt;进入范围循环 | 根身份当前可读且类型为方法 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2529 与被调函数完整身份复核 |
| RCE1386 | R0448 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2530</code> | suffix_match | 记录、令牌 | optional&lt;高级关系证据&gt;绑定证据 | 模板循环本轮记录 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2530 与被调函数完整身份复核 |
| RCE1387 | R0448 | R0450 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2531</code> | unique_name | 记录目标节点、令牌 | optional&lt;int64_t&gt;绑定值 | 本轮证据形成后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2531 与被调函数完整身份复核 |
| RCE1388 | R0448 | R0449 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2532, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2534, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2536, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2538, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2541</code> | unique_name | 输出 | 方法登记根材料直接返回 | 证据/状态值缺失、关系重复或三项槽不全 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2532、海中鱼巣/领域/数据操作.需求任务方法.ixx:2534、海中鱼巣/领域/数据操作.需求任务方法.ixx:2536、海中鱼巣/领域/数据操作.需求任务方法.ixx:2538、海中鱼巣/领域/数据操作.需求任务方法.ixx:2541 与被调函数完整身份复核 |
| RCE1389 | R0448 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2534, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2536, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2538</code> | unique_name | 角色/活跃/失效optional槽与证据 | bool决定继续或内部不一致 | 顺序号与状态枚举匹配某一槽 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2534、海中鱼巣/领域/数据操作.需求任务方法.ixx:2536、海中鱼巣/领域/数据操作.需求任务方法.ixx:2538 与被调函数完整身份复核 |
| RCE1390 | R0450 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2759</code> | unique_name | 状态、nullopt主键、令牌 | 高级节点身份绑定身份 | R0450进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2759 与被调函数完整身份复核 |
| RCE1391 | R0450 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2761</code> | unique_name | this=&amp;关系_；状态、运行期临时、令牌 | vector&lt;关系记录&gt;立即判空 | 身份当前可读且类型为状态 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2761 与被调函数完整身份复核 |
| RCE1392 | R0450 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2762</code> | unique_name | this=&amp;关系_；状态、运行期临时、令牌 | vector&lt;关系记录&gt;立即判空 | 运行期临时出边为空 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2762 与被调函数完整身份复核 |
| RCE1393 | R0451 | R0021 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2778</code> | unique_name | this=&amp;会话 | 结构写入结果绑定主结果 | R0451进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2778 与被调函数完整身份复核 |
| RCE1394 | R0451 | R0022 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2784</code> | direct-member | this=&amp;会话；类型、新主信息 | 结构写入结果绑定节点结果 | 主信息候选成功且未命中故障 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2784 与被调函数完整身份复核 |
| RCE1395 | R0451 | R0027 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2790</code> | direct-member | this=&amp;会话；形成的索引绑定请求 | 结构写入结果立即调用成功判断 | 节点候选成功且未命中故障 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2790 与被调函数完整身份复核 |
| RCE1396 | R0451 | R0129 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2790</code> | unique_name | 主键、新节点、索引所有者::需求任务方法 | 索引绑定请求传给R0027 | 准备绑定主键 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2790 与被调函数完整身份复核 |
| RCE1397 | R0452 | R0023 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2809</code> | direct-member | this=&amp;会话；类型、源节点、目标节点、顺序号 | 结构写入结果绑定结果 | R0452进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2809 与被调函数完整身份复核 |
| RCE1398 | R0452 | R0038 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2817</code> | unique_name | this=&amp;会话；*结果.值 | bool直接作为R0452返回值 | 创建成功、去重登记完成且未命中故障 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2817 与被调函数完整身份复核 |
| RCE1399 | R0453 | R0452 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2834, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2840, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2846, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2853, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2858, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2863, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2869</code> | unique_name | 会话、各关系类型、源/目标端点、顺序号、写入关系及Debug故障指针 | 七个bool进入短路链或宿主已写变量 | R0453按固定顺序写需求关系组 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2834、海中鱼巣/领域/数据操作.需求任务方法.ixx:2840、海中鱼巣/领域/数据操作.需求任务方法.ixx:2846、海中鱼巣/领域/数据操作.需求任务方法.ixx:2853、海中鱼巣/领域/数据操作.需求任务方法.ixx:2858、海中鱼巣/领域/数据操作.需求任务方法.ixx:2863、海中鱼巣/领域/数据操作.需求任务方法.ixx:2869 与被调函数完整身份复核 |
| RCE1400 | R0454 | R0466 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2882</code> | direct_const_member | this=&amp;规格 | bool；取反后作为规格入口拒绝条件 | 进入R0454后无其它前置 | 冻结源码接收者静态类型、R0466完整签名与调用点复核 |
| RCE1401 | R0454 | R0429 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2883, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2917</code> | direct_const_member | 规格.需求主键_；两处调用实参相同 | 第一次接收为const auto 写前；第二次接收为const auto 当前 | 第一次在规格完整后；第二次在R0116结构执行返回后 | 冻结源码两处调用表达式、实参类型与R0429完整签名复核 |
| RCE1402 | R0454 | R0459 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2885, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2918</code> | direct_const_member | 第一次 当前=&amp;写前, 规格=&amp;规格；第二次 当前=&amp;当前, 规格=&amp;规格 | bool；第一次决定幂等读回/冲突，第二次决定提交后读回是否匹配 | 第一次写前状态为已找到；第二次结构执行后完成当前读回 | 冻结源码两处实参、分支使用与R0459完整签名复核 |
| RCE1403 | R0454 | R0463 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2889</code> | direct_static_member | 写前.状态 | 需求提交结果；直接作为R0454返回值 | 写前状态既非已找到也非未找到 | 冻结源码实参、直接返回表达式与R0463完整签名复核 |
| RCE1404 | R0454 | R0116 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2892</code> | direct_const_member_callback_argument | this=&amp;执行器_；回调=R0455捕获闭包，经std::function&lt;void(结构写入会话&amp;)&gt;绑定 | 结构写入结果；接收为const auto 结构结果 | 规格完整、写前状态为未找到 | 冻结源码闭包范围、std::function形参、R0116签名与RCB0020调度复核 |
| RCE1405 | R0454 | R0464 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2926</code> | direct_static_member | 结果=&amp;结构结果 | 需求提交结果；直接作为R0454最终返回值 | 提交后当前需求不匹配且当前状态不是已找到 | 冻结源码实参、最终返回表达式与R0464完整签名复核 |
| RCE1406 | R0455 | R0458 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2893</code> | direct_const_member | 会话=&amp;会话, 主体=规格.主体_, 目标宿主=规格.目标宿主_, 场景=规格.场景_, 目标特征=规格.目标特征_, 当前特征状态材料=规格.当前特征状态材料_, 目标状态={} | bool；取反后决定lambda立即返回 | R0116调用R0455后无其它前置 | 冻结源码跨行实参、空目标状态值初始化与R0458完整签名复核 |
| RCE1407 | R0455 | R0403 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2895</code> | direct_const_member | this=&amp;状态数据操作_, 会话=&amp;会话, 规格=&amp;规格.目标状态规格_ | 状态写入参与结果；接收为const auto 状态 | 会话需求端点匹配返回true | 冻结源码接收者、实参与R0403完整签名复核 |
| RCE1408 | R0455 | R0401 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2896</code> | direct_const_member | this=&amp;状态 | bool；取反后决定lambda立即返回 | R0403已返回状态写入参与结果 | 冻结源码接收者静态类型、分支使用与R0401完整签名复核 |
| RCE1409 | R0455 | R0451 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2901</code> | direct_const_member | 会话=&amp;会话, 类型=节点类型::需求, 主键=规格.需求主键_, 新主信息=&amp;新主信息, 新节点=&amp;新需求；专项宏启用时故障=故障 | bool；取反后决定lambda立即返回 | 状态写入成功且当前配置下专项故障分支未命中 | 冻结源码跨预编译区实参、输出引用与R0451完整签名复核 |
| RCE1410 | R0455 | R0453 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2907</code> | direct_const_member | 会话=&amp;会话, 需求=新需求, 主体=规格.主体_, 目标宿主=规格.目标宿主_, 场景=规格.场景_, 目标特征=规格.目标特征_, 当前特征状态材料=规格.当前特征状态材料_, 目标状态=状态.材料.状态节点, 写入关系=&amp;写入关系；专项宏启用时故障=故障 | bool；取反后决定lambda立即返回 | R0451已写入需求身份候选 | 冻结源码跨行实参、输出容器引用与R0453完整签名复核 |
| RCE1411 | R0455 | R0036 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2914</code> | direct_member | this=&amp;会话, 主信息=新主信息 | bool；作为请求提交前三项短路合取的第一项 | 需求关系组写入返回true | 冻结源码接收者、实参与R0036完整签名复核 |
| RCE1412 | R0455 | R0037 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2914</code> | direct_member | this=&amp;会话, 节点=新需求 | bool；作为请求提交前三项短路合取的第二项 | R0036返回true | 冻结源码接收者、实参与R0037完整签名复核 |
| RCE1414 | R0455 | R0041 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2915</code> | direct_member | this=&amp;会话 | bool；由显式(void)转换丢弃，仅保留会话请求提交副作用 | 主信息可读、节点可读、主键绑定匹配三项均为true | 冻结源码调用表达式、显式void转换与R0041完整签名复核 |
| RCE1415 | R0456 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:293, 海中鱼巣/领域/数据操作.需求任务方法.ixx:294</code> | overload_resolved_free_function | 角色状态；活跃状态；失效状态，共三次调用 | 三次bool结果参与方法登记根材料完整性短路合取 | 读取状态已找到、身份当前可读且类型为方法 | 冻结源码三次实参、句柄重载集合与F0163完整签名复核 |
| RCE1416 | R0457 | R0030 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3148</code> | direct_const_member | this=&amp;会话, 节点=状态 | std::optional&lt;节点类型&gt;；与状态类型optional比较 | 进入R0457后无其它前置 | 冻结源码接收者、实参与R0030完整签名复核 |
| RCE1417 | R0457 | R0034 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3149</code> | direct_const_member | this=&amp;会话, 源节点=状态, 类型=关系类型::运行期临时 | std::vector&lt;关系记录&gt;临时值；调用empty后析构 | 节点类型为状态 | 冻结源码实参、返回临时使用与R0034完整签名复核 |
| RCE1418 | R0457 | R0035 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3150</code> | direct_const_member | this=&amp;会话, 目标节点=状态, 类型=关系类型::运行期临时 | std::vector&lt;关系记录&gt;临时值；调用empty后析构 | 正向运行期临时关系组为空 | 冻结源码实参、返回临时使用与R0035完整签名复核 |
| RCE1419 | R0457 | R0031 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3151</code> | direct_const_member | this=&amp;会话, 节点=状态 | std::optional&lt;主信息句柄&gt;；接收为const auto 主信息 | 节点类型为状态且正反运行期临时关系组均为空 | 冻结源码实参、局部接收与R0031完整签名复核 |
| RCE1420 | R0457 | R0032 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3152</code> | direct_const_member | this=&amp;会话, 主信息=*主信息, 槽位=状态值槽位 | std::optional&lt;std::int64_t&gt;；直接作为R0457返回值 | 主信息optional.has_value()为true | 冻结源码解引用实参、槽位常量与R0032完整签名复核 |
| RCE1421 | R0458 | R0030 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3306, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3307, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3308, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3309, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3310</code> | direct_const_member | this=&amp;会话；节点依次为主体、目标宿主、场景、目标特征、当前特征状态材料 | 五个std::optional&lt;节点类型&gt;临时值分别与存在、存在、场景、特征、特征值类型比较 | 按短路合取顺序，前一端点类型匹配后进入下一调用 | 冻结源码五处实参、短路顺序与R0030完整签名复核 |
| RCE1422 | R0458 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3311</code> | overload_resolved_free_function | 目标状态 | bool；取反后允许空目标状态直接匹配 | 五个必选端点类型均匹配 | 冻结源码实参静态类型、完整句柄重载集合与F0163签名复核 |
| RCE1423 | R0458 | R0457 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3312</code> | direct_const_member | 会话=&amp;会话, 状态=目标状态 | std::optional&lt;std::int64_t&gt;临时值；调用has_value作为最终条件 | 五个必选端点类型匹配且目标状态句柄有效 | 冻结源码实参、返回使用与R0457完整签名复核 |
| RCE1424 | R0459 | R0434 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3382</code> | direct_const_member | this=&amp;当前 | bool；取反后决定立即返回false | 进入R0459后无其它前置 | 冻结源码接收者静态类型与R0434完整签名复核 |
| RCE1425 | R0460 | R0456 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3468</code> | direct_const_member | this=&amp;当前 | bool；与根主键相等条件共同决定立即返回false | 进入R0460后无其它前置 | 冻结源码接收者静态类型与R0456完整签名复核 |
| RCE1426 | R0460 | R0402 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3469, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3470, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3471</code> | direct_const_member | this=&amp;状态数据操作_；状态节点依次为当前.角色状态、当前.活跃状态、当前.失效状态 | 状态值式材料；分别接收为const auto 角色、活跃、失效 | 当前根材料完整且根主键匹配 | 冻结源码三处实参、局部接收与R0402完整签名复核 |
| RCE1427 | R0460 | R0407 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3472</code> | direct_const_member | this依次为&amp;角色、&amp;活跃、&amp;失效，共三次调用 | 三次bool结果参与短路合取 | 三份状态值式材料已读取 | 冻结源码三次接收者、短路顺序与R0407完整签名复核 |
| RCE1428 | R0460 | R0399 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3473, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3474, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3475</code> | direct_const_member | this依次为&amp;规格.角色状态_、&amp;规格.活跃状态_、&amp;规格.失效状态_ | 三个std::uint64_t结果分别与角色、活跃、失效材料幂等主键比较 | 三份当前状态材料均为完整抽象状态 | 冻结源码三次接收者、比较对象与R0399完整签名复核 |
| RCE1429 | R0460 | R0400 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3476, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3477, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3478</code> | direct_const_member | this依次为&amp;规格.角色状态_、&amp;规格.活跃状态_、&amp;规格.失效状态_ | 三个std::int64_t结果分别与角色、活跃、失效材料状态值比较 | 三个状态幂等主键均匹配 | 冻结源码三次接收者、比较对象与R0400完整签名复核 |
| RCE1430 | R0463 | R0461 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3591</code> | direct_static_member | 状态 | 需求任务方法业务状态；作为需求提交结果聚合初始化的状态字段 | 进入R0463后无其它前置 | 冻结源码实参、聚合初始化与R0461完整签名复核 |
| RCE1431 | R0464 | R0462 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3599</code> | direct_static_member | 结果=&amp;结果 | 需求任务方法业务状态；作为需求提交结果聚合初始化的状态字段 | 进入R0464后无其它前置 | 冻结源码实参、聚合初始化与R0462完整签名复核 |
| RCE1432 | R0466 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:449, 海中鱼巣/领域/数据操作.需求任务方法.ixx:450, 海中鱼巣/领域/数据操作.需求任务方法.ixx:451</code> | overload_resolved_free_function | 主体_、目标宿主_、场景_、目标特征_、当前特征状态材料_，共五次调用 | 五次bool结果参与完整需求写入规格短路合取 | 需求主键非零并按源码短路顺序继续 | 冻结源码五次实参、完整句柄重载集合与F0163签名复核 |
| RCE1433 | R0467 | R0466 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:893</code> | direct_const_member | this=&amp;规格 | bool；作为含值optional或nullopt返回的条件 | R0842已构造局部完整需求写入规格 | 冻结源码接收者静态类型、完整成员集合与R0466签名复核 |
| RCE1434 | R0468 | R0479 | <code>海中鱼巣/领域/方法服务.h:1009</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 进入方法首有效性判断 | R0468 进入后首先执行 | 方法服务.h blob 839dd462；1008-1024 行与 R0479 完整签名复核 |
| RCE1435 | R0468 | R0489 | <code>海中鱼巣/领域/方法服务.h:1012</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | std::optional&lt;节点句柄&gt; 绑定到方法虚拟存在 | R0479 返回 true | 方法服务.h blob 839dd462；1012 行与 R0489 完整签名复核 |
| RCE1436 | R0468 | R0494 | <code>海中鱼巣/领域/方法服务.h:1013</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::optional&lt;节点句柄&gt; 绑定到动作入口 | R0489 调用完成 | 方法服务.h blob 839dd462；1013 行与 R0494 完整签名复核 |
| RCE1437 | R0468 | R0490 | <code>海中鱼巣/领域/方法服务.h:1020</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::vector&lt;节点句柄&gt; 写入材料.条件节点组 | 方法虚拟存在和动作入口 optional 均有值 | 方法服务.h blob 839dd462；1020 行与 R0490 完整签名复核 |
| RCE1438 | R0468 | R0491 | <code>海中鱼巣/领域/方法服务.h:1021</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::vector&lt;节点句柄&gt; 写入材料.结果节点组 | 条件节点组读取完成 | 方法服务.h blob 839dd462；1021 行与 R0491 完整签名复核 |
| RCE1439 | R0469 | R0479 | <code>海中鱼巣/领域/方法服务.h:1046</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 进入方法首有效性判断 | R0469 进入后首先执行 | 方法服务.h blob 839dd462；1045-1050 行与 R0479 完整签名复核 |
| RCE1440 | R0469 | F0330 | <code>海中鱼巣/领域/方法服务.h:1049</code> | source-audited-direct-const-member | this=this, 源节点=方法首节点, 类型=关系类型::引用, 目标类型=节点类型::任务, 顺序号=optional&lt;int64_t&gt;{来源任务顺序号20} | std::optional&lt;节点句柄&gt; 直接作为 R0469 返回值 | R0479 返回 true | 方法服务.h blob 839dd462；1049 行与 F0330 四参数签名复核 |
| RCE1441 | R0470 | R0479 | <code>海中鱼巣/领域/方法服务.h:1063</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 进入方法首有效性判断 | R0470 进入后首先执行 | 方法服务.h blob 839dd462；1062-1067 行与 R0479 完整签名复核 |
| RCE1442 | R0470 | F0330 | <code>海中鱼巣/领域/方法服务.h:1066</code> | source-audited-direct-const-member | this=this, 源节点=方法首节点, 类型=关系类型::引用, 目标类型=节点类型::方法, 顺序号=optional&lt;int64_t&gt;{父方法顺序号21} | std::optional&lt;节点句柄&gt; 直接作为 R0470 返回值 | R0479 返回 true | 方法服务.h blob 839dd462；1066 行与 F0330 四参数签名复核 |
| RCE1443 | R0471 | R0483 | <code>海中鱼巣/领域/方法服务.h:1079</code> | source-audited-direct-const-member | this=this, 方法首节点, 顺序号=前置方法顺序号22, 状态 | std::vector&lt;节点句柄&gt; 直接作为 R0471 返回值 | R0471 进入后无条件执行 | 方法服务.h blob 839dd462；1079 行与 R0483 完整签名复核 |
| RCE1444 | R0472 | R0483 | <code>海中鱼巣/领域/方法服务.h:1092</code> | source-audited-direct-const-member | this=this, 方法首节点, 顺序号=后续方法顺序号23, 状态 | std::vector&lt;节点句柄&gt; 直接作为 R0472 返回值 | R0472 进入后无条件执行 | 方法服务.h blob 839dd462；1092 行与 R0483 完整签名复核 |
| RCE1445 | R0473 | R0479 | <code>海中鱼巣/领域/方法服务.h:1097</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 决定返回默认材料或继续聚合 | 局部方法关系材料默认构造完成 | 方法服务.h blob 839dd462；1097 行与 R0479 完整签名复核 |
| RCE1446 | R0473 | R0469 | <code>海中鱼巣/领域/方法服务.h:1100</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::optional&lt;节点句柄&gt; 移动赋值到材料.来源任务 | R0479 返回 true | 方法服务.h blob 839dd462；1100 行与 R0469 完整签名复核 |
| RCE1447 | R0473 | R0470 | <code>海中鱼巣/领域/方法服务.h:1101</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::optional&lt;节点句柄&gt; 移动赋值到材料.父方法 | 来源任务字段赋值完成 | 方法服务.h blob 839dd462；1101 行与 R0470 完整签名复核 |
| RCE1448 | R0473 | R0471 | <code>海中鱼巣/领域/方法服务.h:1102</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::vector&lt;节点句柄&gt; 移动赋值到材料.前置方法组 | 父方法字段赋值完成 | 方法服务.h blob 839dd462；1102 行与 R0471 完整签名复核 |
| RCE1449 | R0473 | R0472 | <code>海中鱼巣/领域/方法服务.h:1103</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态 | std::vector&lt;节点句柄&gt; 移动赋值到材料.后续方法组 | 前置方法组字段赋值完成 | 方法服务.h blob 839dd462；1103 行与 R0472 完整签名复核 |
| RCE1450 | R0474 | F0330 | <code>海中鱼巣/领域/方法服务.h:1210</code> | source-audited-direct-const-member | this=this, 源节点=动作入口节点, 类型=关系类型::引用, 目标类型=节点类型::场景, 顺序号=optional&lt;int64_t&gt;{动作输入规格顺序号1} | std::optional&lt;节点句柄&gt; 直接作为 R0474 返回值 | R0474 进入后无条件执行 | 方法服务.h blob 839dd462；1210 行与 F0330 四参数签名复核 |
| RCE1451 | R0475 | F0330 | <code>海中鱼巣/领域/方法服务.h:1214</code> | source-audited-direct-const-member | this=this, 源节点=动作入口节点, 类型=关系类型::引用, 目标类型=节点类型::场景, 顺序号=optional&lt;int64_t&gt;{动作输出规格顺序号2} | std::optional&lt;节点句柄&gt; 直接作为 R0475 返回值 | R0475 进入后无条件执行 | 方法服务.h blob 839dd462；1214 行与 F0330 四参数签名复核 |
| RCE1452 | R0476 | F0327 | <code>海中鱼巣/领域/方法服务.h:1358</code> | source-audited-direct-const-member | this=this, 方法节点, 状态 | 方法角色状态与 方法角色状态::方法首 比较 | R0476 进入后首先执行 | 方法服务.h blob 839dd462；1358 行与 F0327 完整签名复核 |
| RCE1453 | R0476 | F0629 | <code>海中鱼巣/领域/方法服务.h:1359</code> | source-audited-direct-const-member | this=this, 源节点=方法节点, 类型=关系类型::引用, 目标类型=节点类型::存在 | std::optional&lt;节点句柄&gt; 立即用于 has_value 判断 | F0327 结果等于方法首 | 方法服务.h blob 839dd462；1359 行与 F0629 完整签名复核 |
| RCE1454 | R0477 | F0160 | <code>海中鱼巣/领域/方法服务.h:1365</code> | source-audited-direct-const-member | this=this, 状态 | bool 作为前置短路判断第一项 | R0477 进入后首先执行 | 方法服务.h blob 839dd462；1365 行与 F0160 完整签名复核 |
| RCE1455 | R0477 | R0476 | <code>海中鱼巣/领域/方法服务.h:1365</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 作为前置短路判断第二项 | F0160 返回 true | 方法服务.h blob 839dd462；1365 行与 R0476 完整签名复核 |
| RCE1456 | R0477 | F0575 | <code>海中鱼巣/领域/方法服务.h:1377, 海中鱼巣/领域/方法服务.h:1394</code> | source-audited-direct-const-member | 调用1:this=&amp;关系_, 源节点=登记根材料_-&gt;登记根, 类型=归属；调用2:this=&amp;关系_, 源节点=方法首节点, 类型=模板 | 分别绑定到 登记关系记录组 与 模板关系记录组 | 调用1仅在未提供已知登记关系时；调用2在唯一登记关系成立后 | 方法服务.h blob 839dd462；1377-1378、1394 行与 F0575 完整签名复核 |
| RCE1457 | R0477 | R0478 | <code>海中鱼巣/领域/方法服务.h:1423</code> | source-audited-direct-const-member | this=&amp;材料 | bool 作为条件运算符条件，选择材料 optional 或 nullopt | 唯一登记关系和唯一生命周期关系均成立并完成材料组装 | 方法服务.h blob 839dd462；1423 行与 R0478 完整签名复核 |
| RCE1458 | R0478 | F0163 | <code>海中鱼巣/领域/方法服务.h:157, 海中鱼巣/领域/方法服务.h:158</code> | source-audited-direct-free-overload | 调用1:登记根；调用2:方法首 | 两个 bool 按源码顺序进入短路合取 | R0478 进入后依次执行，第二调用要求第一调用返回 true | 方法服务.h blob 839dd462；156-164 行逐调用点复核 |
| RCE1459 | R0478 | F0168 | <code>海中鱼巣/领域/方法服务.h:159, 海中鱼巣/领域/方法服务.h:160</code> | source-audited-direct-free-overload | 调用1:登记关系；调用2:生命周期关系 | 两个 bool 按源码顺序进入短路合取 | 前序节点句柄均有效后依次执行 | 方法服务.h blob 839dd462；156-164 行逐调用点复核 |
| RCE1461 | R0479 | R0488 | <code>海中鱼巣/领域/方法服务.h:1669</code> | source-audited-direct-const-member | this=this, 方法首节点=方法节点, 状态 | std::optional&lt;方法登记项材料&gt; 绑定到 const 局部登记项 | R0479 进入后无条件执行 | 方法服务.h blob 839dd462；1668-1671 行复核 |
| RCE1462 | R0480 | R0479 | <code>海中鱼巣/领域/方法服务.h:1674</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 作为短路合取第一项 | R0480 进入后首先执行 | 方法服务.h blob 839dd462；1673-1678 行复核 |
| RCE1463 | R0480 | F0327 | <code>海中鱼巣/领域/方法服务.h:1675</code> | source-audited-direct-const-member | this=this, 方法节点=方法条件节点, 状态 | 方法角色状态与 方法条件 比较 | R0479 返回 true | 方法服务.h blob 839dd462；1675 行复核 |
| RCE1464 | R0480 | F0579 | <code>海中鱼巣/领域/方法服务.h:1676</code> | source-audited-direct-const-member | this=&amp;关系_, 类型=模板, 源节点=方法首节点, 目标节点=方法条件节点 | bool 作为短路合取第三项 | 方法角色为方法条件 | 方法服务.h blob 839dd462；1676 行复核 |
| RCE1465 | R0480 | F0629 | <code>海中鱼巣/领域/方法服务.h:1677</code> | source-audited-direct-const-member | this=this, 源节点=方法条件节点, 类型=引用, 目标类型=场景 | 临时 optional 立即用于 has_value 判断 | 模板关系存在 | 方法服务.h blob 839dd462；1677 行复核 |
| RCE1466 | R0481 | R0479 | <code>海中鱼巣/领域/方法服务.h:1681</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 作为短路合取第一项 | R0481 进入后首先执行 | 方法服务.h blob 839dd462；1680-1685 行复核 |
| RCE1467 | R0481 | F0327 | <code>海中鱼巣/领域/方法服务.h:1682</code> | source-audited-direct-const-member | this=this, 方法节点=方法结果节点, 状态 | 方法角色状态与 方法结果 比较 | R0479 返回 true | 方法服务.h blob 839dd462；1682 行复核 |
| RCE1468 | R0481 | F0579 | <code>海中鱼巣/领域/方法服务.h:1683</code> | source-audited-direct-const-member | this=&amp;关系_, 类型=模板, 源节点=方法首节点, 目标节点=方法结果节点 | bool 作为短路合取第三项 | 方法角色为方法结果 | 方法服务.h blob 839dd462；1683 行复核 |
| RCE1469 | R0481 | F0629 | <code>海中鱼巣/领域/方法服务.h:1684</code> | source-audited-direct-const-member | this=this, 源节点=方法结果节点, 类型=引用, 目标类型=场景 | 临时 optional 立即用于 has_value 判断 | 模板关系存在 | 方法服务.h blob 839dd462；1684 行复核 |
| RCE1470 | R0482 | F0334 | <code>海中鱼巣/领域/方法服务.h:1690</code> | source-audited-direct-const-member | this=this, 角色 | bool 作为入口短路判断第二项 | R0479 返回 true | 方法服务.h blob 839dd462；1690 行复核 |
| RCE1471 | R0482 | R0479 | <code>海中鱼巣/领域/方法服务.h:1690</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 作为入口短路判断第一项 | R0482 完成空结果组构造后 | 方法服务.h blob 839dd462；1690 行复核 |
| RCE1472 | R0482 | F0621 | <code>海中鱼巣/领域/方法服务.h:1693</code> | source-audited-direct-const-member | this=&amp;关系_, 源节点=方法首节点, 类型=模板 | std::vector&lt;节点句柄&gt; 绑定到 const 目标组 | 入口方法首和角色均有效 | 方法服务.h blob 839dd462；1693 行复核 |
| RCE1473 | R0482 | F0327 | <code>海中鱼巣/领域/方法服务.h:1695</code> | source-audited-direct-const-member | this=this, 方法节点=目标, 状态 | 方法角色状态与入口角色比较 | F0333 返回 true | 方法服务.h blob 839dd462；1695 行复核 |
| RCE1474 | R0482 | F0333 | <code>海中鱼巣/领域/方法服务.h:1695</code> | source-audited-direct-const-member | this=this, 节点句柄值=目标, 类型=方法 | bool 作为候选筛选第一项 | 范围循环当前目标已绑定 | 方法服务.h blob 839dd462；1695 行复核 |
| RCE1475 | R0482 | R0480 | <code>海中鱼巣/领域/方法服务.h:1698</code> | source-audited-direct-const-member | this=this, 方法首节点, 方法条件节点=目标, 状态 | bool 决定保留或跳过目标 | 入口角色为方法条件且通用筛选通过 | 方法服务.h blob 839dd462；1698 行复核 |
| RCE1476 | R0482 | R0481 | <code>海中鱼巣/领域/方法服务.h:1699</code> | source-audited-direct-const-member | this=this, 方法首节点, 方法结果节点=目标, 状态 | bool 决定保留或跳过目标 | 入口角色为方法结果且通用筛选通过 | 方法服务.h blob 839dd462；1699 行复核 |
| RCE1477 | R0483 | R0479 | <code>海中鱼巣/领域/方法服务.h:1735, 海中鱼巣/领域/方法服务.h:1740</code> | source-audited-direct-const-member | 调用1:方法节点=方法首节点,状态；调用2:方法节点=目标,状态 | 调用1决定空返回；调用2决定是否追加目标 | 调用1在入口执行；调用2在每轮目标绑定后执行 | 方法服务.h blob 839dd462；1732-1745 行复核 |
| RCE1478 | R0483 | F0620 | <code>海中鱼巣/领域/方法服务.h:1738</code> | source-audited-direct-const-member | this=&amp;关系_, 源节点=方法首节点, 类型=引用, 顺序号 | std::vector&lt;节点句柄&gt; 绑定到 const 目标组 | 入口方法首有效 | 方法服务.h blob 839dd462；1738 行复核 |
| RCE1479 | R0485 | F0327 | <code>海中鱼巣/领域/方法服务.h:1807</code> | source-audited-direct-const-member | this=this, 方法节点=动作入口节点, 状态 | 方法角色状态与 动作入口 比较 | R0485 进入后首先执行 | 方法服务.h blob 839dd462；1806-1809 行复核 |
| RCE1480 | R0485 | R0487 | <code>海中鱼巣/领域/方法服务.h:1808</code> | source-audited-direct-const-member | this=this, 动作入口节点, 状态 | 临时 optional 立即用于 has_value 判断 | 节点角色为动作入口 | 方法服务.h blob 839dd462；1808 行复核 |
| RCE1481 | R0486 | R0479 | <code>海中鱼巣/领域/方法服务.h:1812</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 作为短路合取第一项 | R0486 进入后首先执行 | 方法服务.h blob 839dd462；1811-1816 行复核 |
| RCE1482 | R0486 | R0485 | <code>海中鱼巣/领域/方法服务.h:1813</code> | source-audited-direct-const-member | this=this, 动作入口节点, 状态 | bool 作为短路合取第二项 | R0479 返回 true | 方法服务.h blob 839dd462；1813 行复核 |
| RCE1483 | R0486 | F0579 | <code>海中鱼巣/领域/方法服务.h:1815</code> | source-audited-direct-const-member | this=&amp;关系_, 类型=引用, 源节点=方法首节点, 目标节点=动作入口节点 | bool 作为最终短路合取结果 | 入口角色有效且两个节点不同 | 方法服务.h blob 839dd462；1815 行复核 |
| RCE1484 | R0487 | F0327 | <code>海中鱼巣/领域/方法服务.h:1819</code> | source-audited-direct-const-member | this=this, 方法节点=动作入口节点, 状态 | 方法角色状态与 动作入口 比较 | R0487 进入后首先执行 | 方法服务.h blob 839dd462；1818-1832 行复核 |
| RCE1485 | R0487 | F0330 | <code>海中鱼巣/领域/方法服务.h:1822</code> | source-audited-direct-const-member | this=this, 源节点=动作入口节点, 类型=模板, 目标类型=状态, 顺序号=optional&lt;int64_t&gt;{动作入口状态顺序号1} | std::optional&lt;节点句柄&gt; 绑定到 const 状态节点 | 节点角色为动作入口 | 方法服务.h blob 839dd462；1822 行复核 |
| RCE1486 | R0487 | F0329 | <code>海中鱼巣/领域/方法服务.h:1826</code> | source-audited-direct-const-member | this=&amp;状态, 状态节点=状态节点.value() | std::optional&lt;std::int64_t&gt; 绑定到 const 状态值 | 状态节点 optional 有值 | 方法服务.h blob 839dd462；1826 行复核 |
| RCE1487 | R0487 | R0484 | <code>海中鱼巣/领域/方法服务.h:1831</code> | source-audited-direct-const-member | this=this, 状态值=入口状态 | bool 决定构造有值或空动作入口状态 optional | 状态值 optional 有值并已转换枚举 | 方法服务.h blob 839dd462；1830-1831 行复核 |
| RCE1488 | R0488 | R0477 | <code>海中鱼巣/领域/方法服务.h:455</code> | source-audited-direct-const-member | this=this, 方法首节点, 状态, 已知登记关系=默认 std::nullopt | std::optional&lt;方法登记项材料&gt; 直接作为 R0488 返回值 | shared_lock 已成功构造 | 方法服务.h blob 839dd462；452-456 行复核 |
| RCE1489 | R0489 | R0479 | <code>海中鱼巣/领域/方法服务.h:871</code> | source-audited-direct-const-member | this=this, 方法节点, 状态 | bool 决定空返回或继续读取引用 | R0489 进入后首先执行 | 方法服务.h blob 839dd462；870-875 行复核 |
| RCE1490 | R0489 | F0629 | <code>海中鱼巣/领域/方法服务.h:874</code> | source-audited-direct-const-member | this=this, 源节点=方法节点, 类型=引用, 目标类型=存在 | std::optional&lt;节点句柄&gt; 直接作为 R0489 返回值 | R0479 返回 true | 方法服务.h blob 839dd462；874 行复核 |
| RCE1491 | R0490 | R0482 | <code>海中鱼巣/领域/方法服务.h:878</code> | source-audited-direct-const-member | this=this, 方法首节点, 角色=方法角色状态::方法条件, 状态 | std::vector&lt;节点句柄&gt; 直接作为 R0490 返回值 | R0490 进入后无条件执行 | 方法服务.h blob 839dd462；877-879 行与 R0482 完整签名复核 |
| RCE1492 | R0491 | R0482 | <code>海中鱼巣/领域/方法服务.h:882</code> | source-audited-direct-const-member | this=this, 方法首节点, 角色=方法角色状态::方法结果, 状态 | std::vector&lt;节点句柄&gt; 直接作为 R0491 返回值 | R0491 进入后无条件执行 | 方法服务.h blob 839dd462；881-883 行与 R0482 完整签名复核 |
| RCE1493 | R0492 | R0480 | <code>海中鱼巣/领域/方法服务.h:903</code> | source-audited-direct-const-member | this=this, 方法首节点, 方法条件节点, 状态 | bool 经逻辑非运算决定空 optional 或继续读取 | R0492 进入后首先执行 | 方法服务.h blob 839dd462；901-907 行与 R0480 完整签名复核 |
| RCE1494 | R0492 | F0629 | <code>海中鱼巣/领域/方法服务.h:906</code> | source-audited-direct-const-member | this=this, 源节点=方法条件节点, 类型=关系类型::引用, 目标类型=节点类型::场景 | std::optional&lt;节点句柄&gt; 直接作为 R0492 返回值 | R0480 返回 true | 方法服务.h blob 839dd462；906 行与 F0629 三参数签名复核 |
| RCE1495 | R0493 | R0481 | <code>海中鱼巣/领域/方法服务.h:911</code> | source-audited-direct-const-member | this=this, 方法首节点, 方法结果节点, 状态 | bool 经逻辑非运算决定空 optional 或继续读取 | R0493 进入后首先执行 | 方法服务.h blob 839dd462；909-915 行与 R0481 完整签名复核 |
| RCE1496 | R0493 | F0629 | <code>海中鱼巣/领域/方法服务.h:914</code> | source-audited-direct-const-member | this=this, 源节点=方法结果节点, 类型=关系类型::引用, 目标类型=节点类型::场景 | std::optional&lt;节点句柄&gt; 直接作为 R0493 返回值 | R0481 返回 true | 方法服务.h blob 839dd462；914 行与 F0629 三参数签名复核 |
| RCE1497 | R0494 | R0479 | <code>海中鱼巣/领域/方法服务.h:947</code> | source-audited-direct-const-member | this=this, 方法节点=方法首节点, 状态 | bool 经逻辑非运算决定空 optional 或继续扫描 | R0494 进入后首先执行 | 方法服务.h blob 839dd462；946-966 行与 R0479 完整签名复核 |
| RCE1498 | R0494 | F0621 | <code>海中鱼巣/领域/方法服务.h:950</code> | source-audited-direct-const-member | this=&amp;关系_, 源节点=方法首节点, 类型=关系类型::引用 | std::vector&lt;节点句柄&gt; 绑定到 const 局部目标组 | R0479 返回 true | 方法服务.h blob 839dd462；950 行与 F0621 完整签名复核 |
| RCE1499 | R0494 | R0486 | <code>海中鱼巣/领域/方法服务.h:953</code> | source-audited-direct-const-member | this=this, 方法首节点, 动作入口节点=目标, 状态 | bool 经逻辑非运算决定跳过本轮或继续读取状态 | 目标组当前迭代器未到末尾 | 方法服务.h blob 839dd462；952-955 行与 R0486 完整签名复核 |
| RCE1500 | R0494 | R0487 | <code>海中鱼巣/领域/方法服务.h:956</code> | source-audited-direct-const-member | this=this, 动作入口节点=目标, 状态 | std::optional&lt;动作入口状态&gt; 绑定到 const 局部入口状态 | R0486 返回 true | 方法服务.h blob 839dd462；956-959 行与 R0487 完整签名复核 |
| RCE1501 | R0497 | R0430 | <code>海中鱼巣/领域/服务.方法.ixx:214</code> | source-audited-direct-const-member | this=&amp;数据操作_, 主键 | 方法登记根材料直接作为 R0497 返回值 | 主键 != 0 | 服务.方法.ixx blob 74a839fe；213-215 行与 R0430 完整签名复核 |
| RCE1502 | R0499 | R0427 | <code>海中鱼巣/领域/服务.方法.ixx:100</code> | source-audited-direct-const-member | this=&amp;数据操作_, 根主键=请求.根幂等主键, 角色状态=move(*角色.规格), 活跃状态=move(*活跃.规格), 失效状态=move(*失效.规格) | std::optional&lt;方法登记根写入规格&gt; 绑定到 const 局部写入规格 | 三个 R0510 成功检查均返回 true | 服务.方法.ixx blob 74a839fe；100-102 行与 R0427 完整签名复核 |
| RCE1503 | R0499 | R0432 | <code>海中鱼巣/领域/服务.方法.ixx:104</code> | source-audited-direct-const-member | this=&amp;数据操作_, 规格=*写入规格 | 方法提交结果作为条件运算符真分支并直接返回 | 写入规格.has_value() 返回 true | 服务.方法.ixx blob 74a839fe；103-105 行与 R0432 完整签名复核 |
| RCE1504 | R0499 | R0498 | <code>海中鱼巣/领域/服务.方法.ixx:98</code> | source-audited-static-member | 角色.状态, 活跃.状态, 失效.状态 | 需求任务方法业务状态用于构造方法提交结果并立即返回 | R0510 对三个规格结果的短路检查未全部成功 | 服务.方法.ixx blob 74a839fe；97-99 行与 R0498 完整签名复核 |
| RCE1505 | R0502 | F0163 | <code>海中鱼巣/领域/服务.特征.ixx:118, 海中鱼巣/领域/服务.特征.ixx:119</code> | unique_name | this=请求.宿主 / this=请求.特征定义 | 句柄有效性 bool | 调用方可达且源码分支条件成立 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE1506 | R0502 | R0798 | <code>海中鱼巣/领域/服务.特征.ixx:121</code> | suffix_match | this=&amp;数据操作_, 特征定义=请求.特征定义 | 特征定义值式材料 定义 | 调用方可达且源码分支条件成立 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE1507 | R0502 | R0390 | <code>海中鱼巣/领域/服务.特征.ixx:122</code> | direct-const-member |  |  | 定义材料静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1508 | R0502 | R0507 | <code>海中鱼巣/领域/服务.特征.ixx:123</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1509 | R0502 | R0505 | <code>海中鱼巣/领域/服务.特征.ixx:125</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1510 | R0502 | R0381 | <code>海中鱼巣/领域/服务.特征.ixx:126</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1511 | R0503 | R0506 | <code>海中鱼巣/领域/服务.特征.ixx:138</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1512 | R0503 | R0382 | <code>海中鱼巣/领域/服务.特征.ixx:139</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1514 | R0512 | R0423 | <code>海中鱼巣/领域/服务.轻量因果.ixx:52</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1515 | R0514 | R0431 | <code>海中鱼巣/领域/服务.需求.ixx:104</code> | direct-const-member |  |  | 写入规格有值后创建完整目标状态需求 | 专项源码静态类型与实际装配人工复核 |
| RCE1516 | R0514 | F0163 | <code>海中鱼巣/领域/服务.需求.ixx:91, 海中鱼巣/领域/服务.需求.ixx:91, 海中鱼巣/领域/服务.需求.ixx:92, 海中鱼巣/领域/服务.需求.ixx:92, 海中鱼巣/领域/服务.需求.ixx:93</code> | unique_name | this=主体/目标宿主/场景/目标特征/当前特征状态材料 | 五项节点句柄有效性 bool | 调用方可达且源码分支条件成立 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE1517 | R0514 | R0511 | <code>海中鱼巣/领域/服务.需求.ixx:97</code> | direct-const-member |  |  | 状态服务字段形成目标抽象状态规格 | 专项源码静态类型与实际装配人工复核 |
| RCE1518 | R0514 | R0510 | <code>海中鱼巣/领域/服务.需求.ixx:98</code> | direct-const-member |  |  | 抽象状态规格结果静态类型复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE1519 | R0514 | R0513 | <code>海中鱼巣/领域/服务.需求.ixx:98</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1520 | R0514 | R0467 | <code>海中鱼巣/领域/服务.需求.ixx:99</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；状态规格成功后形成完整需求写入规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1521 | R0517 | R0525 | <code>海中鱼巣/领域/概念图服务.h:2438</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1690 | R0523 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3775</code> | operator |  |  | 逐份签名材料比较概念节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1525 | R0524 | F0579 | <code>海中鱼巣/领域/概念图服务.h:3941</code> | unique_name | this=this, 类型=类型, 源节点=源节点, 目标节点=目标节点 | bool | 确保专用关系_已加锁先查既有关系分支；三实参无令牌重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE1526 | R0524 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3943, 海中鱼巣/领域/概念图服务.h:3947, 海中鱼巣/领域/概念图服务.h:3958</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1527 | R0524 | F0168 | <code>海中鱼巣/领域/概念图服务.h:3958</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1528 | R0525 | R0523 | <code>海中鱼巣/领域/概念图服务.h:4342</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1529 | R0525 | F0184 | <code>海中鱼巣/领域/概念图服务.h:4344, 海中鱼巣/领域/概念图服务.h:4357, 海中鱼巣/领域/概念图服务.h:4387, 海中鱼巣/领域/概念图服务.h:4395</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1530 | R0525 | R0528 | <code>海中鱼巣/领域/概念图服务.h:4395</code> | direct_const_member |  |  | 抽象树遍历正常完成后最终追根因检查；视图静态类型为抽象树视图材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1531 | R0526 | R0524 | <code>海中鱼巣/领域/概念图服务.h:4402</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1532 | R0527 | F0163 | <code>海中鱼巣/领域/概念图服务.h:666</code> | direct |  |  | 抽象树投影项复核概念节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1533 | R0527 | F0051 | <code>海中鱼巣/领域/概念图服务.h:670</code> | operator |  |  | 比较投影路径末尾节点与概念节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1534 | R0528 | F0163 | <code>海中鱼巣/领域/概念图服务.h:685</code> | direct |  |  | 复核根概念节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1535 | R0528 | F0051 | <code>海中鱼巣/领域/概念图服务.h:687</code> | operator |  |  | 比较首个投影项概念与根概念 | 专项源码静态类型与实际装配人工复核 |
| RCE1536 | R0528 | R0529 | <code>海中鱼巣/领域/概念图服务.h:688</code> | standard-library-callback |  |  | std::all_of 注册局部投影项完整性 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1537 | R0529 | R0527 | <code>海中鱼巣/领域/概念图服务.h:689</code> | direct_const_member |  |  | 逐个抽象树投影项复核完整性 | 专项源码静态类型与实际装配人工复核 |
| RCE1691 | R0882 | F0051 | <code>海中鱼巣/领域/概念图服务.h:922</code> | operator | 材料-&gt;根节点, 捕获的节点 -&gt; const 节点句柄&amp; | 节点句柄相等 bool，作为 any_of 谓词结果 | 材料.has_value() 为 true | 概念图服务.h:921-923 局部 lambda 范围与静态类型复核 |
| RCE1538 | R0532 | R0534 | <code>海中鱼巣/领域/概念图算法.h:233</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1539 | R0532 | R0535 | <code>海中鱼巣/领域/概念图算法.h:234</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1540 | R0533 | R0532 | <code>海中鱼巣/领域/概念图算法.h:254, 海中鱼巣/领域/概念图算法.h:255</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1541 | R0533 | R0537 | <code>海中鱼巣/领域/概念图算法.h:258</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1542 | R0535 | R0534 | <code>海中鱼巣/领域/概念图算法.h:632</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1543 | R0536 | R0535 | <code>海中鱼巣/领域/概念图算法.h:677</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1544 | R0537 | R0536 | <code>海中鱼巣/领域/概念图算法.h:689</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1545 | R0538 | F0467 | <code>海中鱼巣/领域/概念活动状态.数据.h:105</code> | direct-const-member |  |  | 活跃角色静态类型为概念活动状态角色材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1546 | R0538 | R0563 | <code>海中鱼巣/领域/概念活动状态.数据.h:105</code> | direct-const-member |  |  | 根身份静态类型为系统角色身份材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1547 | R0538 | R0540 | <code>海中鱼巣/领域/概念活动状态.数据.h:106</code> | direct-free |  |  | 根身份完整后映射系统角色用途 | 专项源码静态类型与实际装配人工复核 |
| RCE1548 | R0538 | R0541 | <code>海中鱼巣/领域/概念活动状态.数据.h:107</code> | direct-free |  |  | 用途匹配后映射节点类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1549 | R0538 | R0542 | <code>海中鱼巣/领域/概念活动状态.数据.h:108</code> | direct-free |  |  | 类型匹配后复核概念根签名 | 专项源码静态类型与实际装配人工复核 |
| RCE1550 | R0538 | F0459 | <code>海中鱼巣/领域/概念活动状态.数据.h:109</code> | direct-const-member |  |  | 签名成立后复核活跃关系材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1551 | R0544 | F0163 | <code>海中鱼巣/领域/特征值服务.h:581</code> | unique_name | 记录.特征值节点 -&gt; const 节点句柄&amp; | 句柄有效 bool，参与前置短路判断 | R0544 执行记录前置一致性检查 | 特征值服务.h:581 与句柄.h 节点句柄重载复核 |
| RCE1552 | R0544 | R0543 | <code>海中鱼巣/领域/特征值服务.h:585, 海中鱼巣/领域/特征值服务.h:588</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1553 | R0545 | F0163 | <code>海中鱼巣/领域/特征值服务.h:594</code> | unique_name | 记录.特征值节点 -&gt; const 节点句柄&amp; | 句柄有效 bool，参与返回合取表达式 | R0545 执行记录一致性检查 | 特征值服务.h:594 与句柄.h 节点句柄重载复核 |
| RCE1554 | R0546 | R0547 | <code>海中鱼巣/领域/特征值服务.h:674</code> | standard-library-callback |  |  | std::find_if 注册非 const Vec 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1555 | R0547 | F0051 | <code>海中鱼巣/领域/特征值服务.h:675</code> | direct-free |  |  | Vec 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1556 | R0548 | R0549 | <code>海中鱼巣/领域/特征值服务.h:680</code> | standard-library-callback |  |  | std::find_if 注册 const Vec 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1557 | R0549 | F0051 | <code>海中鱼巣/领域/特征值服务.h:681</code> | direct-free |  |  | const Vec 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1558 | R0550 | R0551 | <code>海中鱼巣/领域/特征值服务.h:686</code> | standard-library-callback |  |  | std::find_if 注册非 const I64 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1559 | R0551 | F0051 | <code>海中鱼巣/领域/特征值服务.h:687</code> | direct-free |  |  | I64 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1560 | R0552 | R0553 | <code>海中鱼巣/领域/特征值服务.h:692</code> | standard-library-callback |  |  | std::find_if 注册 const I64 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1561 | R0553 | F0051 | <code>海中鱼巣/领域/特征值服务.h:693</code> | direct-free |  |  | const I64 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1562 | R0554 | R0555 | <code>海中鱼巣/领域/特征值服务.h:699</code> | standard-library-callback |  |  | std::count_if 注册 Vec 记录计数谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1563 | R0555 | F0051 | <code>海中鱼巣/领域/特征值服务.h:700</code> | direct-free |  |  | Vec 计数谓词比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1564 | R0556 | R0557 | <code>海中鱼巣/领域/特征值服务.h:706</code> | standard-library-callback |  |  | std::count_if 注册 I64 记录计数谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1565 | R0557 | F0051 | <code>海中鱼巣/领域/特征值服务.h:707</code> | direct-free |  |  | I64 计数谓词比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1566 | R0560 | F0331 | <code>海中鱼巣/领域/特征服务.h:217</code> | unique_name | this=&amp;主信息_ | 主信息句柄作为创建节点实参 | R0560进入返回表达式 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1567 | R0560 | F0332 | <code>海中鱼巣/领域/特征服务.h:217</code> | unique_name | this=&amp;节点_；类型=节点类型::特征；主信息=F0331结果 | 创建的特征节点句柄直接返回 | F0331形成主信息后 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1568 | R0562 | R0558 | <code>海中鱼巣/领域/特征服务.h:357</code> | source-audited-direct | this=&amp;关系_；源节点=需求节点；类型；顺序号 | 目标节点组 | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1569 | R0566 | F0163 | <code>海中鱼巣/领域/组合.特征状态.ixx:33</code> | unique_name | 句柄=请求.宿主；句柄=请求.特征定义 | 两个节点句柄有效性结果参与入口短路 | R0566入口前置判断；调用两次 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1570 | R0566 | R0502 | <code>海中鱼巣/领域/组合.特征状态.ixx:34</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；形成实例特征槽位规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1571 | R0566 | R0508 | <code>海中鱼巣/领域/组合.特征状态.ixx:36</code> | direct-const-member | this=&amp;槽位规格 | 槽位规格成功状态 | 槽位规格形成后 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1572 | R0566 | R0509 | <code>海中鱼巣/领域/组合.特征状态.ixx:39</code> | direct-const-member | this=&amp;值规格 | 初始值规格成功状态 | 初始值规格形成后 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1573 | R0566 | R0567 | <code>海中鱼巣/领域/组合.特征状态.ixx:36, 海中鱼巣/领域/组合.特征状态.ixx:39</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1574 | R0566 | R0503 | <code>海中鱼巣/领域/组合.特征状态.ixx:37</code> | unique_name+direct-const-member |  |  | 调用方可达且源码分支条件成立；形成初始特征值规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1575 | R0566 | R0389 | <code>海中鱼巣/领域/组合.特征状态.ixx:40</code> | direct-const-member |  |  | 两份规格成功后创建槽位并发布初始状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1576 | R0571 | F0580 | <code>海中鱼巣/领域/语素服务.h:287, 海中鱼巣/领域/语素服务.h:288</code> | direct_const_member | this=&amp;关系_,关系类型::运行期临时,节点句柄值 | 是否存在运行期临时目标关系bool | 节点记录存在，且类型为状态或动态时按逻辑或短路调用 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1577 | R0572 | F0580 | <code>海中鱼巣/领域/语素服务.h:304, 海中鱼巣/领域/语素服务.h:305</code> | direct_const_member | this=&amp;关系_,关系类型::运行期临时,节点句柄值 | 是否存在运行期临时目标关系bool | 节点记录存在，且类型为状态或动态时按逻辑或短路调用 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1578 | R0573 | F0544 | <code>海中鱼巣/领域/语素服务.h:331, 海中鱼巣/领域/语素服务.h:362</code> | unique_name | this=&amp;索引_；主键=主键 | optional节点句柄用于既有绑定拒绝和创建后读回 | 主键非零 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1579 | R0573 | F0331 | <code>海中鱼巣/领域/语素服务.h:335</code> | unique_name | this=&amp;主信息_ | 新主信息句柄写入结果.主信息 | 不存在既有主键绑定 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1580 | R0573 | F0168 | <code>海中鱼巣/领域/语素服务.h:347</code> | unique_name | 句柄=结果.对应关系 | 关系句柄有效性决定是否撤销 | 创建对应关系后 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1581 | R0573 | F0332 | <code>海中鱼巣/领域/语素服务.h:340</code> | unique_name | this=&amp;节点_；类型=节点类型::语素；主信息=结果.主信息 | 语素入口节点句柄写入结果.语素入口 | 主信息句柄有效 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1582 | R0573 | R0574 | <code>海中鱼巣/领域/语素服务.h:342, 海中鱼巣/领域/语素服务.h:348, 海中鱼巣/领域/语素服务.h:355, 海中鱼巣/领域/语素服务.h:366</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1583 | R0573 | R0728 | <code>海中鱼巣/领域/语素服务.h:360</code> | unique_name | this；节点句柄值=结果.语素入口 | bool入口可读 | 全部创建和可选主键绑定完成 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1584 | R0573 | F0496 | <code>海中鱼巣/领域/语素服务.h:361</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1585 | R0573 | R0570 | <code>海中鱼巣/领域/语素服务.h:361</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1586 | R0574 | F0544 | <code>海中鱼巣/领域/语素服务.h:374</code> | unique_name | this=&amp;索引_；主键=结果.主键 | optional当前节点 | 本次绑定且主键非零 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1587 | R0574 | R0750 | <code>海中鱼巣/领域/语素服务.h:376</code> | unique_name | this=&amp;索引_; 主键=解析后的主键 | bool用于条件判断 | 调用方可达且源码分支条件成立 | 本批源码静态接收者、实参数量与完整重载身份复核 |
| RCE1588 | R0574 | F0168 | <code>海中鱼巣/领域/语素服务.h:379</code> | unique_name | 句柄=结果.对应关系 | 关系句柄有效性决定是否删除关系 | 清理对应关系前 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1589 | R0574 | F0590 | <code>海中鱼巣/领域/语素服务.h:380</code> | unique_name | this=&amp;关系_；关系=结果.对应关系 | bool删除结果未接收 | 对应关系句柄有效 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1590 | R0574 | F0523 | <code>海中鱼巣/领域/语素服务.h:383</code> | unique_name | this=&amp;节点_；节点=结果.语素入口 | bool删除结果未接收 | 语素入口节点句柄有效 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1591 | R0574 | R0761 | <code>海中鱼巣/领域/语素服务.h:386</code> | unique_name | this=&amp;主信息_；主信息=结果.主信息 | bool删除结果未接收 | 主信息句柄有效 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE1592 | R0575 | R0569 | <code>海中鱼巣/领域/语素服务.h:66</code> | suffix_match |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1593 | R0576 | R0077 | <code>海中鱼巣/领域/需求服务.h:1010</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1594 | R0577 | F0620 | <code>海中鱼巣/领域/需求服务.h:1030</code> | source-audited-direct | this=&amp;关系_；源节点=结算状态；类型；顺序号 | 目标节点组 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1595 | R0578 | F0331 | <code>海中鱼巣/领域/需求服务.h:132</code> | source-audited-direct | this=&amp;主信息_；无 | 主信息句柄传给F0332 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1596 | R0578 | F0332 | <code>海中鱼巣/领域/需求服务.h:132</code> | source-audited-direct | this=&amp;节点_；类型=需求；主信息=F0331返回值 | 需求节点句柄直接返回 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1597 | R0579 | F0554 | <code>海中鱼巣/领域/需求服务.h:300</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1598 | R0579 | F0555 | <code>海中鱼巣/领域/需求服务.h:302</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1599 | R0579 | F0168 | <code>海中鱼巣/领域/需求服务.h:321, 海中鱼巣/领域/需求服务.h:322, 海中鱼巣/领域/需求服务.h:323, 海中鱼巣/领域/需求服务.h:324, 海中鱼巣/领域/需求服务.h:325, 海中鱼巣/领域/需求服务.h:326</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1600 | R0579 | F0184 | <code>海中鱼巣/领域/需求服务.h:307, 海中鱼巣/领域/需求服务.h:311, 海中鱼巣/领域/需求服务.h:321, 海中鱼巣/领域/需求服务.h:339</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1601 | R0579 | R0578 | <code>海中鱼巣/领域/需求服务.h:310</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1602 | R0579 | F0552 | <code>海中鱼巣/领域/需求服务.h:331</code> | source-audited-direct | this；需求节点 | 需求承接材料可选值 | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1603 | R0579 | F0553 | <code>海中鱼巣/领域/需求服务.h:337</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1604 | R0579 | R0585 | <code>海中鱼巣/领域/需求服务.h:338</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1605 | R0580 | F0336 | <code>海中鱼巣/领域/需求服务.h:349</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1606 | R0580 | R0594 | <code>海中鱼巣/领域/需求服务.h:350</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1607 | R0581 | F0621 | <code>海中鱼巣/领域/需求服务.h:357</code> | source-audited-direct | this=&amp;关系_；源节点=需求节点；类型=模板 | 目标节点组 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1608 | R0582 | R0576 | <code>海中鱼巣/领域/需求服务.h:514</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1609 | R0583 | R0576 | <code>海中鱼巣/领域/需求服务.h:519</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1610 | R0583 | R0582 | <code>海中鱼巣/领域/需求服务.h:523</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1611 | R0583 | F0575 | <code>海中鱼巣/领域/需求服务.h:527</code> | source-audited-direct | this=&amp;关系_；源节点=自我主体.value()；类型=引用 | 关系记录组 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1612 | R0584 | R0576 | <code>海中鱼巣/领域/需求服务.h:540</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1613 | R0585 | R0576 | <code>海中鱼巣/领域/需求服务.h:553, 海中鱼巣/领域/需求服务.h:557</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1616 | R0587 | F0575 | <code>海中鱼巣/领域/需求服务.h:708</code> | source-audited-direct | this=&amp;关系_；源节点=需求节点；类型=归属 | 关系记录组 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1617 | R0587 | R0596 | <code>海中鱼巣/领域/需求服务.h:713</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1618 | R0588 | R0597 | <code>海中鱼巣/领域/需求服务.h:749</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1619 | R0589 | R0597 | <code>海中鱼巣/领域/需求服务.h:753</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1620 | R0590 | R0597 | <code>海中鱼巣/领域/需求服务.h:757</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1621 | R0591 | R0577 | <code>海中鱼巣/领域/需求服务.h:761</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1622 | R0592 | R0597 | <code>海中鱼巣/领域/需求服务.h:765</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1623 | R0593 | F0163 | <code>海中鱼巣/领域/需求服务.h:773</code> | source-audited-direct | 父需求 | bool参与父需求短路判断 | 调用方可达且源码分支条件成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE1624 | R0593 | R0581 | <code>海中鱼巣/领域/需求服务.h:776</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1625 | R0593 | F0552 | <code>海中鱼巣/领域/需求服务.h:780</code> | source-audited-direct | this；需求节点 | 需求承接材料可选值 | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1628 | R0596 | R0588 | <code>海中鱼巣/领域/需求服务.h:980</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1629 | R0596 | R0589 | <code>海中鱼巣/领域/需求服务.h:981</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1630 | R0596 | R0590 | <code>海中鱼巣/领域/需求服务.h:982</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1631 | R0596 | R0591 | <code>海中鱼巣/领域/需求服务.h:983</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1632 | R0597 | R0077 | <code>海中鱼巣/领域/需求服务.h:991</code> | unique_name |  |  | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1692 | R0600 | F0631 | <code>海中鱼巣/核心/结构事务接线.数据.h:31</code> | unique_name+direct-member |  |  | 调用方可达且源码分支条件成立；非自赋值分支先释放当前许可 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1693 | R0604 | F0168 | <code>海中鱼巣/领域/概念图服务.h:244</code> | unique_name+direct_free |  |  | 调用方可达且源码分支条件成立；复核关系句柄有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1694 | R0604 | F0163 | <code>海中鱼巣/领域/概念图服务.h:245, 海中鱼巣/领域/概念图服务.h:246</code> | direct_free |  |  | 依次复核源节点和目标节点句柄有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1695 | R0605 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3019</code> | operator |  |  | 登记槽有值后比较状态节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1696 | R0605 | F0377 | <code>海中鱼巣/领域/概念图服务.h:3020</code> | direct_const_member |  |  | 登记状态等于输入后复核节点仍有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1697 | R0606 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3079, 海中鱼巣/领域/概念图服务.h:3087, 海中鱼巣/领域/概念图服务.h:3088</code> | operator |  |  | lambda 先比较登记概念和状态，再比较读回端点 | 专项源码静态类型与实际装配人工复核 |
| RCE1698 | R0606 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3082</code> | direct_const_member |  |  | 登记概念和状态均匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1699 | R0607 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3096</code> | operator |  |  | lambda 比较登记概念和状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1700 | R0607 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3099</code> | direct_const_member |  |  | 登记概念和状态均匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1701 | R0608 | F0163 | <code>海中鱼巣/领域/概念图服务.h:56, 海中鱼巣/领域/概念图服务.h:59</code> | direct_free |  |  | 复核概念和状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1702 | R0608 | F0168 | <code>海中鱼巣/领域/概念图服务.h:60</code> | direct_free |  |  | 阶段和两个节点句柄前置成立后复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1703 | R0571 | F0190 | <code>海中鱼巣/领域/语素服务.h:279</code> | direct_const_member | this=&amp;节点_,节点句柄值 | 节点记录optional | R0571函数进入 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1704 | R0572 | F0190 | <code>海中鱼巣/领域/语素服务.h:297</code> | direct_const_member | this=&amp;节点_,节点句柄值 | 节点记录optional | R0572函数进入 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1705 | F0363 | F0163 | <code>海中鱼巣/领域/语素服务.h:35</code> | direct_free | 语素入口 | 语素入口句柄有效bool | F0363函数进入 | 源码逐行与重载类型审计 PASS |
| RCE1706 | F0363 | F0565 | <code>海中鱼巣/领域/语素服务.h:36</code> | direct_free | 主信息 | 主信息句柄有效bool | F0163@35 返回 true；逻辑与短路继续 | 源码逐行与重载类型审计 PASS |
| RCE1707 | F0364 | F0163 | <code>海中鱼巣/领域/语素服务.h:112</code> | direct_free | 结果.语素入口 | 语素入口句柄有效bool | R0573@111 已返回语素入口创建结果 | 源码逐行与重载类型审计 PASS |
| RCE1708 | F0364 | F0167 | <code>海中鱼巣/领域/语素服务.h:115</code> | direct_member | this=&amp;关系_,关系类型::语素概念追溯,结果.语素入口,概念节点,默认顺序号0 | 概念关系 | F0163@112 返回 true | 源码逐行、默认实参与重载类型审计 PASS |
| RCE1709 | F0365 | F0331 | <code>海中鱼巣/领域/存在服务.h:104</code> | direct_member_nonvirtual | this=&amp;主信息_ | 主信息句柄 | F0365进入 | 源码静态接收者与零参数重载复核 |
| RCE1710 | F0365 | F0332 | <code>海中鱼巣/领域/存在服务.h:105</code> | direct_member_nonvirtual | this=&amp;节点_, 节点类型::存在, 主信息句柄 | 根节点 | 主信息句柄已创建 | 源码静态接收者与二参数重载复核 |
| RCE1711 | F0365 | R0609 | <code>海中鱼巣/领域/存在服务.h:106</code> | direct_const_member | this, 根节点 | 记录 | 根节点已创建 | 同一具名类型一参数重载与定义范围复核 |
| RCE1712 | F0365 | F0624 | <code>海中鱼巣/领域/存在服务.h:107</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 主信息有效判定 | 记录存在 | 源码静态接收者与一参数重载复核 |
| RCE1713 | F0366 | F0331 | <code>海中鱼巣/领域/动态服务.h:286</code> | direct_member_nonvirtual | this=&amp;主信息_ | 主信息句柄 | F0366进入 | 源码静态接收者与零参数重载复核 |
| RCE1714 | F0366 | F0332 | <code>海中鱼巣/领域/动态服务.h:287</code> | direct_member_nonvirtual | this=&amp;节点_, 节点类型::动态, 主信息句柄 | 根节点 | 主信息句柄已创建 | 源码静态接收者与二参数重载复核 |
| RCE1715 | F0366 | F0190 | <code>海中鱼巣/领域/动态服务.h:288</code> | direct_const_member | this=&amp;节点_, 根节点 | 记录 | 根节点已创建 | 源码静态接收者与一参数重载复核 |
| RCE1716 | F0366 | F0619 | <code>海中鱼巣/领域/动态服务.h:289</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 普通动态材料 | 记录存在 | 源码静态接收者与一参数重载复核 |
| RCE1717 | F0366 | F0624 | <code>海中鱼巣/领域/动态服务.h:292</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 主信息有效判定 | 记录存在且类型为动态 | 源码静态接收者与一参数重载复核 |
| RCE1718 | F0367 | F0190 | <code>海中鱼巣/领域/二次特征服务.h:51</code> | direct_const_member | this=&amp;节点_, 根节点 | 记录 | 根节点已创建 | 源码静态接收者与一参数重载复核 |
| RCE1719 | F0367 | F0624 | <code>海中鱼巣/领域/二次特征服务.h:54</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 主信息有效判定 | 记录存在且类型为二次特征 | 源码静态接收者与一参数重载复核 |
| RCE1720 | R0609 | F0190 | <code>海中鱼巣/领域/存在服务.h:132</code> | direct_const_member | this=&amp;节点_, 存在节点 | 记录 | R0609进入 | 源码静态接收者与一参数重载复核 |
| RCE1721 | R0610 | F0190 | <code>海中鱼巣/领域/动态服务.h:316</code> | direct_const_member | this=&amp;节点_, 动态节点 | 记录 | R0610进入 | 源码静态接收者与一参数重载复核 |
| RCE1722 | R0610 | F0580 | <code>海中鱼巣/领域/动态服务.h:318</code> | direct_const_member | this=&amp;关系_, 关系类型::运行期临时, 动态节点 | 存在目标关系判定 | 记录存在且类型为动态 | 源码静态接收者与二参数重载复核 |
| RCE1723 | F0516 | R0609 | <code>海中鱼巣/领域/存在服务.h:96</code> | direct_const_member | this, 存在 | 记录 | 存在节点已创建 | 同一具名类型一参数重载与定义范围复核 |
| RCE1724 | R0277 | R0609 | <code>海中鱼巣/领域/存在服务.h:140</code> | direct_const_member | this, 存在节点 | 存在记录有值判定 | R0277进入 | 同一具名类型一参数重载与定义范围复核 |
| RCE1726 | F0368 | R0611 | <code>海中鱼巣/领域/因果服务.h:57</code> | direct_member_nonvirtual | this=&amp;因果服务, 无显式参数 | 根节点 | F0368进入 | 当前源码重载与调用点复核 |
| RCE1727 | F0368 | F0190 | <code>海中鱼巣/领域/因果服务.h:58</code> | direct_const_member | this=&amp;节点_, 根节点 | 记录 | R0611返回后 | 当前源码逐调用点复核 |
| RCE1728 | F0368 | F0624 | <code>海中鱼巣/领域/因果服务.h:61</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 主信息有效bool | 记录存在且类型为因果引用 | 当前源码逐调用点复核 |
| RCE1729 | R0611 | F0331 | <code>海中鱼巣/领域/因果服务.h:45</code> | direct_member_nonvirtual | this=&amp;主信息_ | 主信息句柄 | R0611进入 | 当前源码逐调用点复核 |
| RCE1730 | R0611 | F0332 | <code>海中鱼巣/领域/因果服务.h:46</code> | direct_member_nonvirtual | this=&amp;节点_, 节点类型::因果引用, 主信息句柄 | 因果引用 | 主信息句柄已创建 | 当前源码逐调用点复核 |
| RCE1731 | R0611 | F0190 | <code>海中鱼巣/领域/因果服务.h:47</code> | direct_const_member | this=&amp;节点_, 因果引用 | 记录 | 因果引用节点已创建 | 当前源码逐调用点复核 |
| RCE1732 | R0611 | F0624 | <code>海中鱼巣/领域/因果服务.h:49</code> | direct_const_member | this=&amp;主信息_, 记录-&gt;主信息 | 主信息有效bool | 记录存在且类型为因果引用 | 当前源码逐调用点复核 |
| RCE1733 | R0611 | F0184 | <code>海中鱼巣/领域/因果服务.h:48-50</code> | direct_free_function | 记录.has_value() &amp;&amp; 记录-&gt;类型==因果引用 &amp;&amp; 主信息_.主信息是否有效(记录-&gt;主信息), 固定说明 | 后验检查bool并取反进入失败分支 | 记录读取返回后总是形成条件并调用 | 当前源码逐调用点复核 |
| RCE1734 | F0369 | F0190 | <code>海中鱼巣/领域/概念图服务.h:927</code> | direct_const_member | this=&amp;节点_, 节点 | 记录 | F0369进入 | 当前源码逐调用点复核 |
| RCE1735 | F0370 | F0051 | <code>海中鱼巣/领域/概念图服务.h:1034</code> | direct_free_operator | 实例, 概念 | 完整句柄相同bool | 实例与概念粗类别读取完成后进入写前短路判断 | 当前源码逐调用点复核 |
| RCE1736 | F0376 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:427</code> | direct_const_member+source_audited | this=&amp;节点_, 候选, 令牌 | 候选节点当前有效bool | 共享令牌有效；候选快照已形成；逐候选循环 | 当前源码逐调用点、接收者类型与重载复核 |
| RCE1737 | F0377 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:442</code> | direct_const_member+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0377进入 | 当前源码逐调用点、Clang AST候选与同仓接域调用模式复核 |
| RCE1738 | F0377 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:443</code> | resolved_function_pointer+source_audited | 状态=事务接线_.运行期状态 | 结构事务许可 | RCE1737返回true | 当前源码逐调用点、现行接线装配与既有RCE0042同域绑定复核 |
| RCE1739 | F0377 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | this=&amp;许可 | 许可是否有效bool | RCE1738已形成局部许可 | 当前源码逐调用点与Clang AST短路表达式复核 |
| RCE1740 | F0377 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | this=&amp;许可 | 结构事务令牌const引用 | RCE1739返回true；逻辑与右侧开始求值 | 当前源码逐调用点与Clang AST左到右短路顺序复核 |
| RCE1741 | F0377 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:443-445</code> | implicit_destructor+source_audited | this=&amp;许可 | void | RCE1738已形成许可；返回值求值完成或异常展开 | 当前源码作用域、C++ RAII与既有E0790同域生命周期复核 |
| RCE1742 | F0381 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:251-265</code> | implicit_destructor+source_audited | this=&amp;语句 | void | F0207已完成局部语句构造；随后正常返回或异常展开 | 当前源码作用域、C++ RAII与F0214已定义析构逐行复核 |
| RCE1743 | F0384 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | this=&amp;事务接线_ | 已接域bool | F0384进入 | 当前源码单行控制流、静态接收者与同仓调用模式复核 |
| RCE1744 | F0384 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | resolved_function_pointer+source_audited | 状态=事务接线_.运行期状态 | 局部结构事务许可 | RCE1743返回true | 当前源码、现行接线装配与既有同域函数指针绑定复核 |
| RCE1745 | F0384 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | this=&amp;许可 | 许可有效bool | RCE1744已形成局部许可 | 当前源码三元表达式左到右求值与静态接收者复核 |
| RCE1746 | F0384 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | this=&amp;许可 | 结构事务令牌const引用 | RCE1745返回true；三元表达式选择读取分支 | 当前源码三元表达式条件分支与静态接收者复核 |
| RCE1747 | F0384 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | implicit_destructor+source_audited | this=&amp;许可 | void | RCE1744已形成许可；返回值求值完成或异常展开 | 当前源码单行作用域、C++ RAII与同仓许可生命周期复核 |
| RCE1748 | F0385 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:45</code> | direct_const_member+source_audited | this=&amp;语素初始化读数 | 语素初始化结果是否成功bool | F0385进入；false时第46行返回std::nullopt | 当前源码逐调用点、静态接收者类型与F0173完整签名复核 |
| RCE1749 | F0388 | R0612 | <code>海中鱼巣/界面/控制面板窗口.cpp:394-401</code> | standard_algorithm_callback+source_audited | 材料=std::all_of当前遍历元素const引用 | 当前树视图材料是否满足三项展示边界bool | F0388执行X01021；std::all_of对尚未短路的当前元素调用谓词 | 当前源码397-401行lambda正文、标准算法回调语义与R0612身份复核 |
| RCE1750 | F0387 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0387进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点734行与同宏调用模式复核 |
| RCE1751 | F0387 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE1750返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点734行与F0337完整签名复核 |
| RCE1752 | F0387 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE1750返回true且RCE1751返回nullptr | 当前源码宏展开、现行生产装配与既有同域函数指针绑定复核 |
| RCE1753 | F0387 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE1752返回许可并经RCE1756移动构造完成 | 当前源码宏定义120-121行、调用点734行与F0338签名复核 |
| RCE1754 | F0387 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE1753返回true | 当前源码宏定义121-122行、调用点734行与F0339签名复核 |
| RCE1755 | F0387 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE1754返回const引用 | 自动令牌范围承载值 | RCE1753返回true且RCE1754已取得令牌引用 | 当前源码宏定义122行、调用点734行与F0340完整构造签名复核 |
| RCE1756 | F0387 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE1752返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE1752返回临时许可；进入optional.emplace承载值构造 | 当前源码宏定义120行、调用点734行、F0375签名与既有E0793同宏先例复核 |
| RCE1757 | F0402 | R0114 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:150</code> | constructor+source-audited | 接线=接线_,主信息=&amp;主信息_,节点=&amp;节点_,关系=&amp;关系_,索引=&amp;索引_ | 成员执行器_ | F0402成员接线、四仓库引用和关系仓库编号完成初始化 | 当前源码第150行成员初始化器、成员声明与R0114构造函数签名专项复核 |
| RCE1758 | F0406 | R0114 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:102</code> | constructor+source-audited | 接线=接线_,主信息=&amp;主信息_,节点=&amp;节点_,关系=&amp;关系_,索引=&amp;索引_ | 成员执行器_ | F0406成员接线和四仓库引用完成初始化 | 当前源码第102行成员初始化器、成员声明与R0114构造函数签名专项复核 |
| RCE1759 | F0409 | R0114 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:35</code> | constructor+source-audited | 接线=接线_,主信息=&amp;主信息_,节点=&amp;节点_,关系=&amp;关系_,索引=&amp;索引_ | 成员执行器_ | F0409成员接线、四仓库引用和关系仓库编号完成初始化 | 当前源码第35行成员初始化器、成员声明与R0114构造函数签名专项复核 |
| RCE1760 | F0440 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1431</code> | direct-free+macro-expanded+source-audited | 接线=事务接线_,令牌=F0440形参 | 共享令牌是否有效bool | F0440进入；false立即返回std::nullopt | 宏体1028行、展开点1431行与R0089完整签名复核 |
| RCE1761 | F0440 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1431</code> | direct-constructor+macro-expanded+source-audited | 仓库=*this,令牌=F0440形参 | 局部关系令牌范围 | RCE1760返回true | 宏体1029行、展开点1431行与F0340构造签名复核 |
| RCE1762 | F0456 | R0613 | <code>海中鱼巣/领域/系统角色清单.数据.h:212</code> | defaulted-member-comparison+source-audited | 11个系统角色身份材料字段按声明顺序比较 | 主体身份字段相等bool | 首字段必达，后续字段仅当前序全部相等；静态11次 | defaulted operator==成员顺序、字段类型与R0613签名复核 |
| RCE1763 | F0456 | F0457 | <code>海中鱼巣/领域/系统角色清单.数据.h:212</code> | defaulted-member-comparison+source-audited | 场景接纳自我关系 | 关系材料相等bool | 前三个身份字段相等后；静态1次 | defaulted operator==成员顺序与F0457签名复核 |
| RCE1764 | F0456 | R0614 | <code>海中鱼巣/领域/系统角色清单.数据.h:212</code> | defaulted-member-comparison+source-audited | 安全根需求、服务根需求 | 根需求角色材料相等bool | 前序字段相等后按声明顺序；静态2次 | defaulted operator==成员顺序与R0614签名复核 |
| RCE1765 | R0614 | R0613 | <code>海中鱼巣/领域/系统角色清单.数据.h:157</code> | defaulted-member-comparison+source-audited | 特征定义、实例槽位、当前特征值、目标状态、根需求 | 身份材料相等bool | 每次R0614按声明顺序短路；静态5次 | defaulted operator==成员顺序与R0613签名复核 |
| RCE1766 | R0613 | F0051 | <code>海中鱼巣/领域/系统角色清单.数据.h:122</code> | defaulted-member-comparison+source-audited | 节点字段 | 节点句柄相等bool | 前三个内建字段相等后 | defaulted operator==字段类型与F0051重载复核 |
| RCE1767 | R0613 | R0615 | <code>海中鱼巣/领域/系统角色清单.数据.h:122</code> | defaulted-member-comparison+source-audited | 主信息字段 | 主信息句柄相等bool | 前序内建字段与节点句柄均相等后 | defaulted operator==字段类型与R0615重载复核 |
| RCE1768 | F0457 | R0598 | <code>海中鱼巣/领域/系统角色清单.数据.h:140</code> | defaulted-member-comparison+source-audited | 关系字段 | 关系句柄相等bool | F0457进入，首字段必达 | defaulted operator==字段类型与R0598重载复核 |
| RCE1769 | F0457 | F0051 | <code>海中鱼巣/领域/系统角色清单.数据.h:140</code> | defaulted-member-comparison+source-audited | 源节点、目标节点 | 节点句柄相等bool | 关系句柄及前序字段相等后按顺序短路；静态2次 | defaulted operator==字段类型与F0051重载复核 |
| RCE1770 | F0454 | F0452 | <code>海中鱼巣/领域/初始化.系统角色.ixx:176</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 同一具名类型内直接调用；首层漏边候选 |
| RCE1771 | F0454 | F0245 | <code>海中鱼巣/领域/初始化.系统角色.ixx:176</code> | direct_const_member | this=&amp;清单 | 被调函数结果按当前调用表达式接收或用于条件判断 | F0452返回true后短路求值 | 清单静态类型为系统角色清单；首层漏边候选 |
| RCE1772 | F0454 | R0616 | <code>海中鱼巣/领域/初始化.系统角色.ixx:177</code> | direct_const_member | this=&amp;数据操作_, 清单=const只读借用 | 被调函数结果按当前调用表达式接收或用于条件判断 | 初始化器有效且清单完整 | 成员静态类型为系统角色数据操作；新函数候选 |
| RCE1773 | F0454 | F0228 | <code>海中鱼巣/领域/初始化.系统角色.ixx:180</code> | direct_const_member | this=&amp;参数 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B001成功且R0239已形成参数 | 参数静态类型为系统角色初始化参数；首层漏边候选 |
| RCE1774 | F0455 | F0452 | <code>海中鱼巣/领域/初始化.系统角色.ixx:68</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 取得初始化互斥锁后 | 同一具名类型内直接调用；首层漏边候选 |
| RCE1775 | F0455 | F0228 | <code>海中鱼巣/领域/初始化.系统角色.ixx:68</code> | direct_const_member | this=&amp;参数 | 被调函数结果按当前调用表达式接收或用于条件判断 | F0452返回true后短路求值 | 参数静态类型为系统角色初始化参数；首层漏边候选 |
| RCE1776 | F0455 | R0617 | <code>海中鱼巣/领域/初始化.系统角色.ixx:71, 海中鱼巣/领域/初始化.系统角色.ixx:153</code> | direct_const_member | this=&amp;写前 或 this=&amp;写后 | 被调函数结果按当前调用表达式接收或用于条件判断 | 每次R0422预检返回后 | 结果静态类型为系统角色预检结果 |
| RCE1777 | F0455 | R0618 | <code>海中鱼巣/领域/初始化.系统角色.ixx:78</code> | direct_const_member | this=&amp;世界根 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0496返回后 | 结果静态类型为语义基础业务结果 |
| RCE1778 | F0455 | R0619 | <code>海中鱼巣/领域/初始化.系统角色.ixx:79</code> | direct_const_member | this=&amp;世界根 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B003返回true | 结果静态类型为语义基础业务结果 |
| RCE1779 | F0455 | R0620 | <code>海中鱼巣/领域/初始化.系统角色.ixx:78</code> | template_instance | 状态=世界根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B003返回false | 实参静态类型冻结模板实例为语义基础业务状态 |
| RCE1780 | F0455 | R0621 | <code>海中鱼巣/领域/初始化.系统角色.ixx:81</code> | direct_const_member | this=&amp;场景服务_, 请求={参数.自我场景主键} | 被调函数结果按当前调用表达式接收或用于条件判断 | 世界根创建成功 | 成员静态类型为场景业务服务 |
| RCE1781 | F0455 | R0622 | <code>海中鱼巣/领域/初始化.系统角色.ixx:82, 海中鱼巣/领域/初始化.系统角色.ixx:100, 海中鱼巣/领域/初始化.系统角色.ixx:120</code> | direct_const_member | this=&amp;自我场景 或 this=&amp;自我存在 或 this=&amp;概念存在根 | 被调函数结果按当前调用表达式接收或用于条件判断 | 各存在场景业务调用返回后 | 三个结果静态类型均为存在场景业务结果 |
| RCE1782 | F0455 | R0623 | <code>海中鱼巣/领域/初始化.系统角色.ixx:83, 海中鱼巣/领域/初始化.系统角色.ixx:101, 海中鱼巣/领域/初始化.系统角色.ixx:121</code> | direct_const_member | this=&amp;自我场景 或 this=&amp;自我存在 或 this=&amp;概念存在根 | 被调函数结果按当前调用表达式接收或用于条件判断 | 对应LF_B007返回true | 三个结果静态类型均为存在场景业务结果 |
| RCE1783 | F0455 | R0624 | <code>海中鱼巣/领域/初始化.系统角色.ixx:82, 海中鱼巣/领域/初始化.系统角色.ixx:100, 海中鱼巣/领域/初始化.系统角色.ixx:120</code> | template_instance | 状态=自我场景.状态 或 自我存在.状态 或 概念存在根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 对应LF_B007返回false | 实参静态类型冻结模板实例为存在场景业务状态 |
| RCE1784 | F0455 | R0625 | <code>海中鱼巣/领域/初始化.系统角色.ixx:88, 海中鱼巣/领域/初始化.系统角色.ixx:89</code> | direct_const_member | this=已有自我场景 或 this=已有自我存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | 对应R0565查找结果非nullptr | 指针目标静态类型为系统角色键占用材料 |
| RCE1785 | F0455 | R0616 | <code>海中鱼巣/领域/初始化.系统角色.ixx:165</code> | direct_const_member | this=&amp;数据操作_, 清单=拓扑.清单 | 被调函数结果按当前调用表达式接收或用于条件判断 | 拓扑发布成功且全部语义复核通过 | 成员静态类型为系统角色数据操作 |
| RCE1786 | R0616 | F0449 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:182</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 同一具名类型内直接调用 |
| RCE1787 | R0616 | F0245 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:182</code> | direct_const_member | this=&amp;清单 | 被调函数结果按当前调用表达式接收或用于条件判断 | F0449返回true后短路求值 | 清单静态类型为系统角色清单 |
| RCE1788 | R0616 | R0421 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:183</code> | direct_const_member | this, 主体=清单.主体 | 被调函数结果按当前调用表达式接收或用于条件判断 | 数据操作有效且清单完整 | 同一具名类型内直接调用 |
| RCE1789 | R0616 | F0074 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:184</code> | direct_const_member | this=&amp;当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0421返回后 | 当前静态类型为系统角色初始化结果 |
| RCE1790 | R0616 | F0244 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:185</code> | operator | 左=当前.清单, 右=清单 | 被调函数结果按当前调用表达式接收或用于条件判断 | F0074返回true | 两侧静态类型均为系统角色清单 |
| RCE1791 | R0625 | F0163 | <code>海中鱼巣/领域/系统角色清单.数据.h:274</code> | direct_free_function | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | always | 节点静态类型为节点句柄 |
| RCE1792 | R0621 | R0626 | <code>海中鱼巣/领域/服务.场景.ixx:30</code> | direct_const_member | this=&amp;数据操作_, 主键=请求.幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 请求主键非0 | 成员静态类型为存在场景数据操作 |
| RCE1793 | R0621 | R0627 | <code>海中鱼巣/领域/服务.场景.ixx:32</code> | direct_const_member | this=&amp;数据操作_, 主键=请求.幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B011返回未找到 | 成员静态类型为存在场景数据操作 |
| RCE1794 | R0621 | R0628 | <code>海中鱼巣/领域/服务.场景.ixx:35</code> | direct_static_member | 状态=当前身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 读取状态既非未找到也非已找到 | 同一具名类型内直接调用 |
| RCE1795 | R0626 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:168</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 同一具名类型内直接调用 |
| RCE1796 | R0626 | F0397 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:169</code> | function_pointer_target | 状态=接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | F0442返回true且主键非0；生产接线 | 生产装配下取得共享许可函数指针唯一目标 |
| RCE1797 | R0626 | F0338 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:170</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 共享许可已形成 | 许可静态类型为结构事务许可 |
| RCE1798 | R0626 | F0441 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:171</code> | direct_const_member | this=&amp;索引_, 主键, 令牌=许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 许可有效 | 参数静态类型锁定令牌重载 |
| RCE1799 | R0626 | F0339 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:171, 海中鱼巣/领域/数据操作.存在场景.ixx:173</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 许可有效；第二次还要求索引命中 | 许可静态类型为结构事务许可 |
| RCE1800 | R0626 | R0630 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:173</code> | direct_const_member | this, 节点=*节点, 令牌=许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 许可有效且索引命中 | 同一具名类型内直接调用 |
| RCE1801 | R0626 | F0345 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:169-174</code> | compiler_implicit_destructor | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 许可形成后的所有正常、提前返回与异常退出路径 | 局部变量静态类型为结构事务许可 |
| RCE1802 | R0627 | R0626 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:290</code> | direct_const_member | this, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 主键非0 | 同一具名类型内直接调用 |
| RCE1803 | R0627 | R0629 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:297</code> | direct_static_member | 状态=当前身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 读取状态既非已找到也非未找到 | 同一具名类型内直接调用 |
| RCE1804 | R0627 | R0633 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:299</code> | direct_const_member | this, 主键, 类型=节点类型::场景, 来源=std::nullopt | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B011返回未找到 | 同一具名类型内直接调用 |
| RCE1805 | R0630 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:446</code> | direct_free_function | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 节点静态类型为节点句柄 |
| RCE1806 | R0630 | F0346 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:447, 海中鱼巣/领域/数据操作.存在场景.ixx:461</code> | direct_const_member | this=&amp;节点_, 节点或引用.目标节点, 令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 入口句柄有效；第二次位于存在引用循环 | 参数静态类型锁定令牌重载 |
| RCE1807 | R0630 | F0439 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:449</code> | direct_const_member | this=&amp;主信息_, 主信息=记录-&gt;主信息, 令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 节点记录可读 | 参数静态类型锁定令牌重载 |
| RCE1808 | R0630 | R0631 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:450, 海中鱼巣/领域/数据操作.存在场景.ixx:463, 海中鱼巣/领域/数据操作.存在场景.ixx:467, 海中鱼巣/领域/数据操作.存在场景.ixx:475</code> | direct_const_member | this, 节点, 具名说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 主信息不可读、来源节点不可读、重复来源或关系权威读回不一致 | 同一具名类型内直接调用 |
| RCE1809 | R0630 | R0078 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:459</code> | direct_const_member | this=&amp;关系_, 源节点=节点, 类型=引用, 令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 节点类型为存在 | 参数静态类型锁定令牌重载 |
| RCE1810 | R0630 | R0632 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:465</code> | direct_static_member | 类型=来源记录-&gt;类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | 引用目标节点可读 | 同一具名类型内直接调用 |
| RCE1811 | R0630 | F0440 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:470</code> | direct_const_member | this=&amp;关系_, 关系=关系句柄值, 令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 来源类型匹配且尚无其它虚拟来源 | 参数静态类型锁定令牌重载 |
| RCE1812 | R0631 | F0184 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:484</code> | direct_free_function | 条件=false, 说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B016被调用 | 全项目唯一具名函数 |
| RCE1813 | R0633 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:623</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 同一具名类型内直接调用 |
| RCE1814 | R0633 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:625</code> | direct_free_function | *来源 | 被调函数结果按当前调用表达式接收或用于条件判断 | 来源.has_value()且类型为存在 | 来源载荷静态类型为节点句柄；F0455场景路径不可满足 |
| RCE1815 | R0633 | R0116 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:633</code> | direct_const_member | this=&amp;执行器_, 回调=LF_B019经std::function转换 | 被调函数结果按当前调用表达式接收或用于条件判断 | 入口材料通过 | 成员静态类型为结构写入执行器 |
| RCE1816 | R0116 | R0634 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | std_function_callback | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116接收的回调来自LF_B018:633并完成会话构造 | std::function目标对象静态来源锁定LF_B019 |
| RCE1817 | R0633 | R0626 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:665</code> | direct_const_member | this, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116未返回已提交且来源未发生版本漂移 | 同一具名类型内直接调用 |
| RCE1818 | R0633 | R0635 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:670</code> | direct_static_member | 材料=当前身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | 写后已找到且来源无值 | 同一具名类型内直接调用 |
| RCE1819 | R0633 | R0636 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:673</code> | direct_static_member | 材料=当前身份, 来源=*来源 | 被调函数结果按当前调用表达式接收或用于条件判断 | 写后已找到且来源有值；F0455场景路径不可满足 | 同一具名类型内直接调用 |
| RCE1820 | R0633 | R0629 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:680</code> | direct_static_member | 状态=当前身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 写后读取状态既非已找到也非未找到 | 同一具名类型内直接调用 |
| RCE1821 | R0633 | R0639 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:682</code> | direct_static_member | 结果=结构结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 写后仍未找到 | 同一具名类型内直接调用 |
| RCE1822 | R0634 | R0037 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:634, 海中鱼巣/领域/数据操作.存在场景.ixx:652</code> | direct_member | 会话, 节点=*来源 或 新节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 第一处仅来源有值；第二处在候选创建与主键绑定成功后 | 会话静态类型为结构写入会话 |
| RCE1823 | R0634 | R0021 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:638</code> | direct_member | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | 来源为空或来源节点可读 | 会话静态类型为结构写入会话 |
| RCE1824 | R0634 | R0640 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:639</code> | template_instance | this=&amp;主信息结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0021返回后 | 返回静态类型冻结值类型为主信息句柄 |
| RCE1825 | R0634 | R0022 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:641</code> | direct_member | 会话, 类型, 新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B025返回true | 会话静态类型为结构写入会话 |
| RCE1826 | R0634 | R0641 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:642</code> | template_instance | this=&amp;节点结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0022返回后 | 返回静态类型冻结值类型为节点句柄 |
| RCE1827 | R0634 | R0129 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:644</code> | direct_free_function | 最终物理键=主键, 目标=新节点, 所有者=存在场景, 探测序号=0 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B026返回true | 全项目唯一具名函数 |
| RCE1828 | R0634 | R0027 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:644-645</code> | direct_member | 会话, 请求=R0129结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | LF_B026返回true | 会话静态类型为结构写入会话 |
| RCE1829 | R0634 | R0138 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:645</code> | direct_const_member | this=R0027临时返回结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0027返回后 | 返回静态类型为结构写入结果 |
| RCE1830 | R0634 | R0023 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:647</code> | direct_member | 会话, 类型=引用, 源节点=新节点, 目标节点=*来源, 顺序号=0 | 被调函数结果按当前调用表达式接收或用于条件判断 | 主键绑定成功且来源有值 | 会话静态类型为结构写入会话；F0455场景路径不可满足 |
| RCE1831 | R0634 | R0642 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:648</code> | template_instance | this=&amp;关系结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0023返回后；来源有值 | 返回静态类型冻结值类型为关系句柄；F0455场景路径不可满足 |
| RCE1832 | R0634 | R0036 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:651</code> | direct_member | 会话, 主信息=新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 候选创建与绑定步骤均未提前返回 | 会话静态类型为结构写入会话 |
| RCE1833 | R0634 | R0638 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:653</code> | direct_member | 会话, 主键, 预期节点=新节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0036与R0037均返回true | 两参数重载由实参静态类型唯一锁定 |
| RCE1834 | R0634 | R0038 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:654</code> | direct_member | 会话, 关系=新关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | 来源有值且LF_B023返回true | 会话静态类型为结构写入会话；F0455场景路径不可满足 |
| RCE1835 | R0634 | R0041 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:655</code> | direct_member | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | 完整读回为true | 会话静态类型为结构写入会话 |
| RCE1836 | R0635 | R0637 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:418</code> | direct_const_member | this=&amp;材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | always | 材料静态类型为节点身份材料 |
| RCE1837 | R0636 | R0637 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:423</code> | direct_const_member | this=&amp;材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | always | 材料静态类型为节点身份材料 |
| RCE1838 | R0637 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:63</code> | direct_free_function | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 状态为已找到后短路求值 | 节点静态类型为节点句柄 |
| RCE1839 | R0638 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:607</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | always at function entry | 同一具名类型内直接调用 |
| RCE1840 | R0638 | F0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:610, 海中鱼巣/核心/会话.结构写入.ixx:616, 海中鱼巣/核心/会话.结构写入.ixx:627</code> | operator | 节点句柄逐处比较 | 被调函数结果按当前调用表达式接收或用于条件判断 | 当前线程可访问；按写集遍历、仓库记录判断与成功后标记分支求值 | 两侧静态类型均为节点句柄 |
| RCE1841 | R0638 | R0124 | <code>海中鱼巣/核心/会话.结构写入.ixx:615</code> | direct_const_member | this=&amp;索引_, 主键, 令牌_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 写集扫描完成 | 参数静态类型锁定令牌重载 |
| RCE1842 | R0638 | R0127 | <code>海中鱼巣/核心/会话.结构写入.ixx:617</code> | operator | 左=*当前记录, 右=由写入记录形成的主键绑定记录 | 被调函数结果按当前调用表达式接收或用于条件判断 | 仓库记录存在且节点匹配，且本会话存在写入记录 | 两侧静态类型均为主键绑定记录 |
| RCE1843 | R0638 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:623</code> | direct_member | this, 结果={内部不一致,主键,预期节点.版本号,0} | 被调函数结果按当前调用表达式接收或用于条件判断 | 完整主键绑定匹配失败 | 同一具名类型内直接调用 |
| RCE1844 | R0639 | F0184 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:525, 海中鱼巣/领域/数据操作.存在场景.ixx:528, 海中鱼巣/领域/数据操作.存在场景.ixx:531</code> | direct_free_function | 条件=false, 对应具名说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 结构状态为入口拒绝、内部不一致或其它不允许的收口状态 | 全项目唯一具名函数 |
| RCE1845 | R0640 | R0138 | <code>海中鱼巣/核心/结果.结构写入.h:44</code> | direct_const_member | this=&amp;操作 | 被调函数结果按当前调用表达式接收或用于条件判断 | always as first conjunct | 操作静态类型为结构写入结果 |
| RCE1846 | R0641 | R0138 | <code>海中鱼巣/核心/结果.结构写入.h:44</code> | direct_const_member | this=&amp;操作 | 被调函数结果按当前调用表达式接收或用于条件判断 | always as first conjunct | 操作静态类型为结构写入结果 |
| RCE1847 | R0642 | R0138 | <code>海中鱼巣/核心/结果.结构写入.h:44</code> | direct_const_member | this=&amp;操作 | 被调函数结果按当前调用表达式接收或用于条件判断 | always as first conjunct；F0455场景路径的来源分支不可满足 | 操作静态类型为结构写入结果 |
| RCE1848 | F0441 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:197</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1849 | F0442 | R0115 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:154</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1850 | F0443 | R0115 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:325</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1851 | F0444 | R0115 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:472</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1852 | F0445 | R0115 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:218</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1853 | F0446 | R0115 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:105</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1854 | F0447 | R0115 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:257</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1855 | F0448 | R0115 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:878</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1856 | F0448 | F0442 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:878</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1857 | F0448 | F0443 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:879</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1858 | F0449 | R0115 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:39</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1859 | F0450 | F0443 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:39</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1860 | F0450 | R0115 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:39</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1861 | F0451 | F0450 | <code>海中鱼巣/领域/服务.概念活动.ixx:26</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1862 | F0452 | F0449 | <code>海中鱼巣/领域/初始化.系统角色.ixx:64</code> | source-audited-direct | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE1863 | F0455 | R0643 | <code>海中鱼巣/领域/初始化.系统角色.ixx:97</code> | direct_member | this=&amp;存在场景组合器_, 请求={自我场景.场景,参数.自我存在主键} | 被调函数结果按当前调用表达式接收或用于条件判断 | 写前未同时提供已占用自我场景、自我存在和接纳关系 | static type and source callsite audited |
| RCE1864 | F0455 | R0646 | <code>海中鱼巣/领域/初始化.系统角色.ixx:119</code> | direct_member | this=&amp;存在服务_, 请求={参数.概念存在根主键} | 被调函数结果按当前调用表达式接收或用于条件判断 | initialization reaches concept-root creation | static type and source callsite audited |
| RCE1865 | F0455 | R0657 | <code>海中鱼巣/领域/初始化.系统角色.ixx:123</code> | direct_member | this=&amp;动态服务_, 请求={参数.概念动态根主键,参数.动态根材料} | 被调函数结果按当前调用表达式接收或用于条件判断 | concept existence root succeeded | static type and source callsite audited |
| RCE1866 | F0455 | R0658 | <code>海中鱼巣/领域/初始化.系统角色.ixx:125</code> | direct_const_member | this=&amp;概念动态根 | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-dynamic call returned | result static type audited |
| RCE1867 | F0455 | R0659 | <code>海中鱼巣/领域/初始化.系统角色.ixx:126</code> | direct_const_member | this=&amp;概念动态根 | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-dynamic result succeeded | result static type audited |
| RCE1868 | F0455 | R0680 | <code>海中鱼巣/领域/初始化.系统角色.ixx:128</code> | direct_member | this=&amp;轻量因果服务_, 请求={参数.概念因果根主键,概念动态根.动态材料.动态节点} | 被调函数结果按当前调用表达式接收或用于条件判断 | concept dynamic root succeeded | static type and source callsite audited |
| RCE1869 | F0455 | R0681 | <code>海中鱼巣/领域/初始化.系统角色.ixx:130</code> | direct_const_member | this=&amp;概念因果根 | 被调函数结果按当前调用表达式接收或用于条件判断 | light-causal call returned | result static type audited |
| RCE1870 | F0455 | R0682 | <code>海中鱼巣/领域/初始化.系统角色.ixx:131</code> | direct_const_member | this=&amp;概念因果根 | 被调函数结果按当前调用表达式接收或用于条件判断 | light-causal result succeeded | result static type audited |
| RCE1871 | F0455 | R0697 | <code>海中鱼巣/领域/初始化.系统角色.ixx:133</code> | direct_member | this=&amp;二次特征服务_, 请求={参数.概念关系根主键,{概念存在根.存在,概念因果根.因果.因果引用}} | 被调函数结果按当前调用表达式接收或用于条件判断 | concept causal root succeeded | static type and source callsite audited |
| RCE1872 | F0455 | R0711 | <code>海中鱼巣/领域/初始化.系统角色.ixx:139</code> | direct_member | this=&amp;概念图结构服务_, four root identities | 被调函数结果按当前调用表达式接收或用于条件判断 | concept secondary root succeeded | static type and source callsite audited |
| RCE1873 | F0455 | R0716 | <code>海中鱼巣/领域/初始化.系统角色.ixx:144</code> | direct_const_member | this=&amp;根角色 | 被调函数结果按当前调用表达式接收或用于条件判断 | root-role verification returned | result static type audited |
| RCE1874 | F0455 | R0721 | <code>海中鱼巣/领域/初始化.系统角色.ixx:149</code> | direct_const_member | this=&amp;方法登记根 | 被调函数结果按当前调用表达式接收或用于条件判断 | method-root creation returned | result static type audited |
| RCE1875 | F0455 | R0722 | <code>海中鱼巣/领域/初始化.系统角色.ixx:169</code> | direct_const_member | this=&amp;拓扑 | 被调函数结果按当前调用表达式接收或用于条件判断 | world-topology publish succeeded and semantic reread passed | result static type audited |
| RCE1876 | F0455 | R0660 | <code>海中鱼巣/领域/初始化.系统角色.ixx:125</code> | function_template_specialization | 概念动态根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-dynamic result is non-success | 状态动态业务状态 specialization selected |
| RCE1877 | F0455 | R0725 | <code>海中鱼巣/领域/初始化.系统角色.ixx:130</code> | function_template_specialization | 概念因果根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | light-causal result is non-success | 轻量因果业务状态 specialization selected |
| RCE1878 | F0455 | R0601 | <code>海中鱼巣/领域/初始化.系统角色.ixx:136</code> | function_template_specialization | 概念关系根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | secondary-feature result is non-success | 特征体系业务状态 specialization selected |
| RCE1879 | F0455 | R0602 | <code>海中鱼巣/领域/初始化.系统角色.ixx:149</code> | function_template_specialization | 方法登记根.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | method-root result is non-success | 需求任务方法业务状态 specialization selected |
| RCE1880 | R0643 | F0163 | <code>海中鱼巣/领域/组合.存在场景.ixx:26</code> | direct_free | 请求.场景 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1881 | R0643 | R0648 | <code>海中鱼巣/领域/组合.存在场景.ixx:27</code> | direct_member | 请求.场景 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | member static type audited |
| RCE1882 | R0643 | R0645 | <code>海中鱼巣/领域/组合.存在场景.ixx:29, 海中鱼巣/领域/组合.存在场景.ixx:39, 海中鱼巣/领域/组合.存在场景.ixx:53</code> | direct_static_member | read status | 被调函数结果按当前调用表达式接收或用于条件判断 | read status is neither expected success nor allowed not-found | same class unique helper |
| RCE1883 | R0643 | R0626 | <code>海中鱼巣/领域/组合.存在场景.ixx:33</code> | direct_member | 请求.存在幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | scene identity is readable and scene-typed | member static type audited |
| RCE1884 | R0643 | R0652 | <code>海中鱼巣/领域/组合.存在场景.ixx:35</code> | direct_member | 请求.场景,请求.存在幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | existence key not found | member static type audited |
| RCE1885 | R0643 | R0649 | <code>海中鱼巣/领域/组合.存在场景.ixx:47</code> | direct_member | 请求.场景,存在材料.节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | existing identity is a non-virtual actual existence | member static type audited |
| RCE1886 | R0643 | R0644 | <code>海中鱼巣/领域/组合.存在场景.ixx:48</code> | direct_const_member | this=&amp;当前归属 | 被调函数结果按当前调用表达式接收或用于条件判断 | membership read returned | value static type audited |
| RCE1887 | R0643 | R0651 | <code>海中鱼巣/领域/组合.存在场景.ixx:55</code> | direct_member | 请求.场景,存在材料.节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | membership not found | member static type audited |
| RCE1888 | R0644 | F0168 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:75</code> | direct_free | *归属关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | status is 已找到 and optional relationship exists | relationship-handle overload is exact |
| RCE1889 | R0646 | R0626 | <code>海中鱼巣/领域/服务.存在.ixx:54</code> | direct_member | 请求.幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | nonzero key | member static type audited |
| RCE1890 | R0646 | R0650 | <code>海中鱼巣/领域/服务.存在.ixx:56</code> | direct_member | 请求.幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | key not found | member static type audited |
| RCE1891 | R0646 | R0647 | <code>海中鱼巣/领域/服务.存在.ixx:59</code> | direct_static_member | 当前身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | read is neither found nor not-found | same class unique helper |
| RCE1892 | R0648 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:177</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class existing stable identity |
| RCE1893 | R0648 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:177</code> | direct_free | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1894 | R0648 | F0338 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:179</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE1895 | R0648 | F0339 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:180</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE1896 | R0648 | R0630 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:180</code> | direct_member | 节点,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | same class helper |
| RCE1897 | R0648 | F0345 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:181</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits after permit acquisition | licensed permit local lifetime |
| RCE1898 | R0649 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:187</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class existing stable identity |
| RCE1899 | R0649 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:187</code> | direct_free | 场景/存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1900 | R0649 | F0338 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:190</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE1901 | R0649 | F0339 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:194</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE1902 | R0649 | R0630 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:195, 海中鱼巣/领域/数据操作.存在场景.ixx:196</code> | direct_member | 场景/存在,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | same class helper |
| RCE1903 | R0649 | R0653 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:199</code> | direct_static_member | 场景材料.状态,存在材料.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | either identity read failed | same class helper |
| RCE1904 | R0649 | R0078 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:207</code> | direct_const_member | 场景,关系类型::归属,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | both endpoint identities accepted | token overload is exact |
| RCE1905 | R0649 | F0051 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:210, 海中鱼巣/领域/数据操作.存在场景.ixx:224, 海中鱼巣/领域/数据操作.存在场景.ixx:225</code> | operator | node handles | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship records examined | node-handle equality overload is exact |
| RCE1906 | R0649 | R0654 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:212, 海中鱼巣/领域/数据操作.存在场景.ixx:226</code> | direct_member | 场景,存在,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | duplicate or non-authoritative relationship | same class helper |
| RCE1907 | R0649 | F0440 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:221</code> | direct_const_member | 关系句柄值,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | one membership record selected | token overload is exact |
| RCE1908 | R0649 | F0345 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:231</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits after permit acquisition | licensed permit local lifetime |
| RCE1909 | R0650 | R0626 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:258</code> | direct_member | 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | nonzero key | same class member |
| RCE1910 | R0650 | R0635 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:260</code> | direct_static_member | 当前身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity found | same class helper |
| RCE1911 | R0650 | R0629 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:265</code> | direct_static_member | 当前身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | read is not not-found | same class helper |
| RCE1912 | R0650 | R0633 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:267</code> | direct_member | 主键,节点类型::存在,std::nullopt | 被调函数结果按当前调用表达式接收或用于条件判断 | identity not found | same class helper |
| RCE1913 | R0651 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:303</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE1914 | R0651 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:303</code> | direct_free | 场景/存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1915 | R0651 | R0648 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:304, 海中鱼巣/领域/数据操作.存在场景.ixx:305</code> | direct_member | 场景/存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | same class member |
| RCE1916 | R0651 | R0653 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:308</code> | direct_static_member | two read statuses | 被调函数结果按当前调用表达式接收或用于条件判断 | either endpoint read failed | same class helper |
| RCE1917 | R0651 | R0629 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:308, 海中鱼巣/领域/数据操作.存在场景.ixx:316, 海中鱼巣/领域/数据操作.存在场景.ixx:340</code> | direct_static_member | read status | 被调函数结果按当前调用表达式接收或用于条件判断 | read failure needs business mapping | same class helper |
| RCE1918 | R0651 | R0649 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:311, 海中鱼巣/领域/数据操作.存在场景.ixx:335</code> | direct_member | 场景,存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-write and fallback readback | same class member |
| RCE1919 | R0651 | R0644 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:312, 海中鱼巣/领域/数据操作.存在场景.ixx:336</code> | direct_const_member | membership material | 被调函数结果按当前调用表达式接收或用于条件判断 | membership read returned | value static type audited |
| RCE1920 | R0651 | R0116 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:320</code> | direct_const_member | callback=F0455C_L001 | 被调函数结果按当前调用表达式接收或用于条件判断 | no existing membership | non-participant execute overload is exact |
| RCE1921 | R0116 | R0655 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | callback_invoke | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116 invoked with callback formed at 数据操作.存在场景.ixx:320 | callback object provenance audited |
| RCE1922 | R0655 | R0037 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:321</code> | direct_member | 场景/存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | session static type audited |
| RCE1923 | R0655 | R0023 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:325</code> | direct_member | 关系类型::归属,场景,存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | both endpoints readable | session static type audited |
| RCE1924 | R0655 | R0038 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:328</code> | direct_member | 新关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship candidate created | session static type audited |
| RCE1925 | R0655 | R0041 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:328</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | new relationship readable | session static type audited |
| RCE1926 | R0651 | R0639 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:342</code> | direct_static_member | 结构结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | write not committed and no idempotent readback | same class helper |
| RCE1927 | R0652 | F0442 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:348</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE1928 | R0652 | F0163 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:348</code> | direct_free | 场景 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1929 | R0652 | R0648 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:349</code> | direct_member | 场景 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | same class member |
| RCE1930 | R0652 | R0629 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:351, 海中鱼巣/领域/数据操作.存在场景.ixx:360, 海中鱼巣/领域/数据操作.存在场景.ixx:404, 海中鱼巣/领域/数据操作.存在场景.ixx:407</code> | direct_static_member | read status | 被调函数结果按当前调用表达式接收或用于条件判断 | read failure needs business mapping | same class helper |
| RCE1931 | R0652 | R0626 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:354, 海中鱼巣/领域/数据操作.存在场景.ixx:393</code> | direct_member | 存在主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-write and fallback readback | same class member |
| RCE1932 | R0652 | R0635 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:356, 海中鱼巣/领域/数据操作.存在场景.ixx:395</code> | direct_static_member | identity material | 被调函数结果按当前调用表达式接收或用于条件判断 | identity found | same class helper |
| RCE1933 | R0652 | R0651 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:357, 海中鱼巣/领域/数据操作.存在场景.ixx:402</code> | direct_member | 场景,current identity node | 被调函数结果按当前调用表达式接收或用于条件判断 | identity exists without confirmed membership | same class member |
| RCE1934 | R0652 | R0116 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:366</code> | direct_const_member | callback=F0455C_L002 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity not found | non-participant execute overload is exact |
| RCE1935 | R0116 | R0656 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | callback_invoke | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116 invoked with callback formed at 数据操作.存在场景.ixx:366 | callback object provenance audited |
| RCE1936 | R0656 | R0037 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:367, 海中鱼巣/领域/数据操作.存在场景.ixx:383</code> | direct_member | 场景/新存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | session static type audited |
| RCE1937 | R0656 | R0021 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:371</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | scene still readable | session static type audited |
| RCE1938 | R0656 | R0022 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:374</code> | direct_member | 节点类型::存在,新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate created | session static type audited |
| RCE1939 | R0656 | R0129 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:377</code> | direct_free | 存在主键,新存在,索引所有者::存在场景 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate created | existing unique helper |
| RCE1940 | R0656 | R0027 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:377</code> | direct_member | formed index binding request | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate created | session static type audited |
| RCE1941 | R0656 | R0023 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:379</code> | direct_member | 关系类型::归属,场景,新存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | primary key bound | session static type audited |
| RCE1942 | R0656 | R0036 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:382</code> | direct_member | 新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE1943 | R0656 | R0638 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:384</code> | direct_member | 存在主键,新存在 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | two-argument session overload audited |
| RCE1944 | R0656 | R0038 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:385</code> | direct_member | 新关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE1945 | R0656 | R0041 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:386</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | all candidates authoritatively readable | session static type audited |
| RCE1946 | R0652 | R0649 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:396</code> | direct_member | 场景,当前身份.节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | fallback identity found and actual | same class member |
| RCE1947 | R0652 | R0644 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:397</code> | direct_const_member | 当前归属 | 被调函数结果按当前调用表达式接收或用于条件判断 | fallback membership read returned | value static type audited |
| RCE1948 | R0652 | R0639 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:409</code> | direct_static_member | 结构结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | write not committed and no identity readback | same class helper |
| RCE1949 | R0636 | F0051 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:424</code> | operator | optional node handle value and 来源 | 被调函数结果按当前调用表达式接收或用于条件判断 | material readable and existence-typed | optional equality delegates to node-handle equality |
| RCE1950 | R0654 | F0184 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:495</code> | direct_free | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | inconsistency helper entered | existing unique diagnostic helper |
| RCE1951 | R0657 | R0668 | <code>海中鱼巣/领域/服务.动态.ixx:36</code> | direct_member | 请求.幂等主键,请求.动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | nonzero key | member static type audited |
| RCE1952 | R0661 | F0163 | <code>海中鱼巣/领域/服务.动态.ixx:62</code> | direct_free | 动态节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1953 | R0661 | R0663 | <code>海中鱼巣/领域/服务.动态.ixx:63</code> | direct_member | 动态节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | node handle valid | member static type audited |
| RCE1954 | R0662 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:384</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE1955 | R0662 | F0338 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:386</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE1956 | R0662 | F0441 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:387</code> | direct_const_member | 主键,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | token overload is exact |
| RCE1957 | R0662 | F0339 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:387, 海中鱼巣/领域/数据操作.状态动态.ixx:389</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE1958 | R0662 | R0669 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:389</code> | direct_member | *节点,主键,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | primary-key lookup found a node | same class helper |
| RCE1959 | R0662 | F0345 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:390</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits | licensed permit local lifetime |
| RCE1960 | R0663 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:393</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE1961 | R0663 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:393</code> | direct_free | 动态节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE1962 | R0663 | F0338 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:395</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE1963 | R0663 | F0339 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:396</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE1964 | R0663 | R0669 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:396</code> | direct_member | 动态节点,std::nullopt,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | same class helper |
| RCE1965 | R0663 | F0345 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:397</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits | licensed permit local lifetime |
| RCE1966 | R0664 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:72</code> | direct_free | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | status is 已找到 | node-handle overload is exact |
| RCE1967 | R0664 | F0565 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:72</code> | direct_free | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | status is 已找到 and node valid | main-information-handle overload is exact |
| RCE1968 | R0665 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:122, 海中鱼巣/领域/数据操作.状态动态.ixx:123</code> | direct_free | 方法/角色状态/入口状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | status is 已找到 | node-handle overload is exact |
| RCE1969 | R0665 | F0565 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:122, 海中鱼巣/领域/数据操作.状态动态.ixx:123</code> | direct_free | 角色主信息/入口主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | status is 已找到 | main-information-handle overload is exact |
| RCE1970 | R0665 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:124</code> | direct_const_member | 角色关系/入口关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | all handles valid | value static type audited |
| RCE1971 | R0666 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:154</code> | direct_free | 动态节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | status and key accepted | node-handle overload is exact |
| RCE1972 | R0666 | F0565 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:154</code> | direct_free | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | status,key,node accepted | main-information-handle overload is exact |
| RCE1973 | R0398 | R0666 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:158</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-dynamic completeness evaluation | value static type audited |
| RCE1974 | R0667 | R0666 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:169</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | instance-dynamic completeness evaluation | same value type |
| RCE1975 | R0667 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:166, 海中鱼巣/领域/数据操作.状态动态.ixx:170, 海中鱼巣/领域/数据操作.状态动态.ixx:171</code> | direct_free | source action/scene/subject/target/states | 被调函数结果按当前调用表达式接收或用于条件判断 | instance-dynamic completeness evaluation | node-handle overload is exact |
| RCE1976 | R0667 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:167, 海中鱼巣/领域/数据操作.状态动态.ixx:168, 海中鱼巣/领域/数据操作.状态动态.ixx:172, 海中鱼巣/领域/数据操作.状态动态.ixx:173, 海中鱼巣/领域/数据操作.状态动态.ixx:174, 海中鱼巣/领域/数据操作.状态动态.ixx:175</code> | direct_const_member | relationship evidence fields | 被调函数结果按当前调用表达式接收或用于条件判断 | instance-dynamic completeness evaluation | value static type audited |
| RCE1977 | R0668 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:517</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE1978 | R0668 | R0662 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:518</code> | direct_member | 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | same class member |
| RCE1979 | R0668 | R0673 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:520</code> | direct_static_member | 写前,主键,动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-read found a dynamic | same class helper |
| RCE1980 | R0668 | R0675 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:521</code> | direct_static_member | 幂等读回,写前 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-read matches abstract dynamic | same class helper |
| RCE1981 | R0668 | R0676 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:527</code> | direct_static_member | 写前.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-read neither found nor not-found/type mismatch | same class helper |
| RCE1982 | R0668 | R0116 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:531</code> | direct_const_member | callback=F0455C_L004 | 被调函数结果按当前调用表达式接收或用于条件判断 | no existing dynamic | non-participant execute overload is exact |
| RCE1983 | R0116 | R0678 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | callback_invoke | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116 invoked with callback formed at 数据操作.状态动态.ixx:531 | callback object provenance audited |
| RCE1984 | R0678 | R0021 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:532</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | session static type audited |
| RCE1985 | R0678 | R0028 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:535</code> | direct_member | 新主信息,动态材料槽位,动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate created | session static type audited |
| RCE1986 | R0678 | R0022 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:536</code> | direct_member | 节点类型::动态,新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | I64 candidate written | session static type audited |
| RCE1987 | R0678 | R0129 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:539</code> | direct_free | 主键,新动态,索引所有者::状态动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate created | existing unique helper |
| RCE1988 | R0678 | R0027 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:539</code> | direct_member | formed index binding request | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate created | session static type audited |
| RCE1989 | R0678 | R0036 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:541</code> | direct_member | 新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE1990 | R0678 | R0029 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:542</code> | direct_member | 新主信息,动态材料槽位,动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE1991 | R0678 | R0037 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:543</code> | direct_member | 新动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE1992 | R0678 | R0638 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:544</code> | direct_member | 主键,新动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | two-argument session overload audited |
| RCE1993 | R0678 | R0041 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:545</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | complete readback | session static type audited |
| RCE1994 | R0668 | R0674 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:547</code> | direct_member | 结构结果,主键,动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | write callback returned | same class helper |
| RCE1995 | R0669 | R0392 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1210, 海中鱼巣/领域/数据操作.状态动态.ixx:1250, 海中鱼巣/领域/数据操作.状态动态.ixx:1299</code> | direct_member | dynamic/endpoint node,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | licensed dynamic material read | same class existing helper |
| RCE1996 | R0669 | R0723 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1220</code> | direct_const_member | 动态节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | dynamic node identity accepted | token overload definition audited |
| RCE1997 | R0669 | F0383 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1230, 海中鱼巣/领域/数据操作.状态动态.ixx:1232, 海中鱼巣/领域/数据操作.状态动态.ixx:1241</code> | direct_const_member | identity main-information,slot,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | dynamic identity and key accepted | token overload is exact |
| RCE1998 | R0669 | R0082 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1233</code> | direct_const_member | 运行期临时,动态节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-material branch | token overload is exact |
| RCE1999 | R0669 | R0671 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1222, 海中鱼巣/领域/数据操作.状态动态.ixx:1234, 海中鱼巣/领域/数据操作.状态动态.ixx:1242, 海中鱼巣/领域/数据操作.状态动态.ixx:1249, 海中鱼巣/领域/数据操作.状态动态.ixx:1251, 海中鱼巣/领域/数据操作.状态动态.ixx:1253, 海中鱼巣/领域/数据操作.状态动态.ixx:1256, 海中鱼巣/领域/数据操作.状态动态.ixx:1261, 海中鱼巣/领域/数据操作.状态动态.ixx:1269, 海中鱼巣/领域/数据操作.状态动态.ixx:1270, 海中鱼巣/领域/数据操作.状态动态.ixx:1273, 海中鱼巣/领域/数据操作.状态动态.ixx:1278, 海中鱼巣/领域/数据操作.状态动态.ixx:1281, 海中鱼巣/领域/数据操作.状态动态.ixx:1283, 海中鱼巣/领域/数据操作.状态动态.ixx:1287, 海中鱼巣/领域/数据操作.状态动态.ixx:1297, 海中鱼巣/领域/数据操作.状态动态.ixx:1300, 海中鱼巣/领域/数据操作.状态动态.ixx:1303</code> | direct_member | 动态节点,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | licensed dynamic material inconsistency | same class helper |
| RCE2000 | R0669 | R0078 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1247, 海中鱼巣/领域/数据操作.状态动态.ixx:1265, 海中鱼巣/领域/数据操作.状态动态.ixx:1276</code> | direct_const_member | source node,relationship type,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | instance-dynamic branch | token overload is exact |
| RCE2001 | R0669 | R0395 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1248, 海中鱼巣/领域/数据操作.状态动态.ixx:1268, 海中鱼巣/领域/数据操作.状态动态.ixx:1282</code> | direct_member | record,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship record examined | same class existing helper |
| RCE2002 | R0669 | R0664 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1251, 海中鱼巣/领域/数据操作.状态动态.ixx:1300</code> | direct_const_member | endpoint identity | 被调函数结果按当前调用表达式接收或用于条件判断 | endpoint identity read returned | value static type audited |
| RCE2003 | R0669 | R0396 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1290, 海中鱼巣/领域/数据操作.状态动态.ixx:1291</code> | direct_member | state endpoint,std::nullopt,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | required causal relations present | same class existing helper |
| RCE2004 | R0669 | R0393 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1292</code> | direct_const_member | front/back state material | 被调函数结果按当前调用表达式接收或用于条件判断 | state materials read | value static type audited |
| RCE2005 | R0669 | R0670 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1302</code> | direct_member | optional source-action node,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | causal slot 4 exists | same class helper |
| RCE2006 | R0669 | R0665 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1303</code> | direct_const_member | 动作材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | source-action material read | value static type audited |
| RCE2007 | R0670 | R0392 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1151</code> | direct_member | 来源动作,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class existing helper |
| RCE2008 | R0670 | R0078 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1163</code> | direct_const_member | 来源动作,关系类型::模板,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | method identity accepted | token overload is exact |
| RCE2009 | R0670 | R0395 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1165</code> | direct_member | 记录,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | template slot 0/1 record examined | same class existing helper |
| RCE2010 | R0670 | R0672 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1166, 海中鱼巣/领域/数据操作.状态动态.ixx:1168, 海中鱼巣/领域/数据操作.状态动态.ixx:1171</code> | direct_member | 来源动作,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | template evidence missing or duplicate | same class helper |
| RCE2011 | R0670 | R0679 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1191, 海中鱼巣/领域/数据操作.状态动态.ixx:1192</code> | local_lambda_invoke | role/entry relationship and output references | 被调函数结果按当前调用表达式接收或用于条件判断 | both required template relations exist | local lambda source range audited |
| RCE2012 | R0679 | R0392 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1182</code> | direct_member | 关系证据.目标节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | lambda invoked | captured this and token audited |
| RCE2013 | R0679 | R0664 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1183</code> | direct_const_member | 状态身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity read returned | value static type audited |
| RCE2014 | R0679 | F0383 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1184</code> | direct_const_member | 状态身份.主信息,状态值槽位,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity current-readable and state-typed | token overload is exact |
| RCE2015 | R0671 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1334</code> | direct_free | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | inconsistency helper entered | existing unique diagnostic helper |
| RCE2016 | R0672 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1342</code> | direct_free | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | inconsistency helper entered | existing unique diagnostic helper |
| RCE2017 | R0673 | R0398 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1660</code> | direct_const_member | 材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | abstract-dynamic match evaluation | value static type audited |
| RCE2018 | R0674 | R0662 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1717</code> | direct_member | 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | post-write closure | same class member |
| RCE2019 | R0674 | R0673 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1718</code> | direct_static_member | 当前,主键,值 | 被调函数结果按当前调用表达式接收或用于条件判断 | readback returned | same class helper |
| RCE2020 | R0674 | R0675 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1719</code> | direct_static_member | 提交/幂等读回 status,当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | readback matches | same class helper |
| RCE2021 | R0674 | R0677 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1728</code> | direct_static_member | 结构结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | readback absent and not collision | same class helper |
| RCE2022 | R0677 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1835, 海中鱼巣/领域/数据操作.状态动态.ixx:1838, 海中鱼巣/领域/数据操作.状态动态.ixx:1841</code> | direct_free | false,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | structure write returned forbidden state | existing unique diagnostic helper |
| RCE2023 | R0680 | F0163 | <code>海中鱼巣/领域/服务.轻量因果.ixx:26</code> | direct_free | 请求.来源动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | node-handle overload is exact |
| RCE2024 | R0680 | R0661 | <code>海中鱼巣/领域/服务.轻量因果.ixx:27</code> | direct_member | 请求.来源动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | service static type audited |
| RCE2025 | R0680 | R0398 | <code>海中鱼巣/领域/服务.轻量因果.ixx:28</code> | direct_const_member | 动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | source material returned | value static type audited |
| RCE2026 | R0680 | R0667 | <code>海中鱼巣/领域/服务.轻量因果.ixx:28</code> | direct_const_member | 动态材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | source is not complete abstract dynamic | value static type audited |
| RCE2027 | R0680 | R0688 | <code>海中鱼巣/领域/服务.轻量因果.ixx:37</code> | direct_member | 请求.幂等主键,动态材料.动态节点,动态材料.主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | source material complete | member static type audited |
| RCE2028 | R0680 | R0689 | <code>海中鱼巣/领域/服务.轻量因果.ixx:40</code> | direct_member | *规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | formed specification optional has value | member static type audited |
| RCE2029 | R0683 | F0163 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:49</code> | direct_free | 因果引用 | 被调函数结果按当前调用表达式接收或用于条件判断 | status and key accepted | node-handle overload is exact |
| RCE2030 | R0683 | F0565 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:49</code> | direct_free | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | status,key,node accepted | main-information-handle overload is exact |
| RCE2031 | R0687 | F0163 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:76</code> | direct_free | 来源动态_ | 被调函数结果按当前调用表达式接收或用于条件判断 | nonzero key | node-handle overload is exact |
| RCE2032 | R0687 | F0565 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:76</code> | direct_free | 来源主信息_ | 被调函数结果按当前调用表达式接收或用于条件判断 | key and source node valid | main-information-handle overload is exact |
| RCE2033 | R0688 | R0687 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:113</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | local specification constructed | value static type audited |
| RCE2034 | R0689 | F0446 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:135</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE2035 | R0689 | R0687 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:135</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | value static type audited |
| RCE2036 | R0689 | R0685 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:137, 海中鱼巣/领域/数据操作.轻量因果.ixx:148, 海中鱼巣/领域/数据操作.轻量因果.ixx:150</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | source verification/write callback | value static type audited |
| RCE2037 | R0689 | R0686 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:137, 海中鱼巣/领域/数据操作.轻量因果.ixx:151</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | source verification/write callback | value static type audited |
| RCE2038 | R0689 | R0690 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:136</code> | direct_member | 规格 source node/main-information | 被调函数结果按当前调用表达式接收或用于条件判断 | entry accepted | same class helper |
| RCE2039 | R0689 | R0693 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:138</code> | direct_static_member | 来源状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | source state not found-success | same class helper |
| RCE2040 | R0689 | R0684 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:140, 海中鱼巣/领域/数据操作.轻量因果.ixx:168</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | 外层创建函数在写前读取或写后读回幂等主键 | 本批逐函数源码函数范围、静态接收者与调用点拆账复核；lambda 内调用已拆为 RCE3325 |
| RCE2041 | R0689 | R0423 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:140, 海中鱼巣/领域/数据操作.轻量因果.ixx:168</code> | direct_member | 规格.幂等主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-write and post-write read | same class existing stable identity |
| RCE2042 | R0689 | R0683 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:141, 海中鱼巣/领域/数据操作.轻量因果.ixx:169</code> | direct_const_member | 写前/当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | read returned material | value static type audited |
| RCE2043 | R0689 | R0692 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:141, 海中鱼巣/领域/数据操作.轻量因果.ixx:170</code> | direct_static_member | business status,material | 被调函数结果按当前调用表达式接收或用于条件判断 | complete pre-read/readback | same class helper |
| RCE2044 | R0689 | R0694 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:142</code> | direct_static_member | 写前.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-read is not not-found | same class helper |
| RCE2045 | R0689 | R0116 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:147</code> | direct_const_member | callback=F0455C_L006 | 被调函数结果按当前调用表达式接收或用于条件判断 | no complete pre-read | non-participant execute overload is exact |
| RCE2046 | R0116 | R0696 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | callback_invoke | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116 invoked with callback formed at 数据操作.轻量因果.ixx:147 | callback object provenance audited |
| RCE2047 | R0696 | R0685 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:148, 海中鱼巣/领域/数据操作.轻量因果.ixx:150</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | captured specification static type audited |
| RCE2048 | R0696 | R0686 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:151</code> | direct_const_member | 规格 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | captured specification static type audited |
| RCE2049 | R0696 | R0030 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:148</code> | direct_member | 来源动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | session static type audited |
| RCE2050 | R0696 | R0031 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:150</code> | direct_member | 来源动态 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback executing | session static type audited |
| RCE2051 | R0696 | R0021 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:155</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | source snapshot matches | session static type audited |
| RCE2052 | R0696 | R0022 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:158</code> | direct_member | 节点类型::因果引用,新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate complete | session static type audited |
| RCE2053 | R0696 | R0129 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:161</code> | direct_free | 规格 key,新因果引用,索引所有者::轻量因果 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate complete | existing unique helper |
| RCE2054 | R0696 | R0027 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:161</code> | direct_member | formed index binding request | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate complete | session static type audited |
| RCE2055 | R0696 | R0036 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:163</code> | direct_member | 新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE2056 | R0696 | R0037 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:163</code> | direct_member | 新因果引用 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | session static type audited |
| RCE2057 | R0696 | R0638 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:164</code> | direct_member | 规格 key,新因果引用 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate set formed | two-argument session overload audited |
| RCE2058 | R0696 | R0041 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:165</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | complete readback | session static type audited |
| RCE2059 | R0689 | R0695 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:177</code> | direct_static_member | 结构结果,当前.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | no complete post-write material and no source drift | same class helper |
| RCE2060 | R0690 | F0338 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:185</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE2061 | R0690 | F0346 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:186</code> | direct_const_member | 来源动态,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | token overload is exact |
| RCE2062 | R0690 | F0339 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:186, 海中鱼巣/领域/数据操作.轻量因果.ixx:191</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE2063 | R0690 | F0439 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:191</code> | direct_const_member | 来源主信息,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | source node record matches dynamic/main-information | token overload is exact |
| RCE2064 | R0690 | R0691 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:193</code> | direct_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | source main-information anchor missing | same class helper |
| RCE2065 | R0690 | F0345 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:194</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits | licensed permit local lifetime |
| RCE2066 | R0691 | F0184 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:197</code> | direct_free | false,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | source inconsistency helper entered | existing unique diagnostic helper |
| RCE2067 | R0694 | R0693 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:271</code> | direct_static_member | 状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | read state is not type mismatch | same class helper |
| RCE2068 | R0695 | F0184 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:284</code> | direct_free | false,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | post-write material absent with forbidden result | existing unique diagnostic helper |
| RCE2069 | R0697 | F0163 | <code>海中鱼巣/领域/服务.二次特征.ixx:29</code> | direct_free | 组成项 | 被调函数结果按当前调用表达式接收或用于条件判断 | component loop | node-handle overload is exact |
| RCE2070 | R0697 | F0051 | <code>海中鱼巣/领域/服务.二次特征.ixx:31</code> | operator | component node handles | 被调函数结果按当前调用表达式接收或用于条件判断 | duplicate scan | node-handle equality overload is exact |
| RCE2071 | R0697 | R0383 | <code>海中鱼巣/领域/服务.二次特征.ixx:33</code> | direct_member | 组成项 | 被调函数结果按当前调用表达式接收或用于条件判断 | component handle valid and not duplicate so far | data-operation static type audited |
| RCE2072 | R0697 | R0388 | <code>海中鱼巣/领域/服务.二次特征.ixx:34</code> | direct_const_member | 身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity read returned | value static type audited |
| RCE2073 | R0697 | R0699 | <code>海中鱼巣/领域/服务.二次特征.ixx:34</code> | direct_static_member | 身份.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity not current-readable | same class helper |
| RCE2074 | R0697 | R0698 | <code>海中鱼巣/领域/服务.二次特征.ixx:35</code> | direct_static_member | 身份.类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity current-readable | same class helper |
| RCE2075 | R0697 | R0701 | <code>海中鱼巣/领域/服务.二次特征.ixx:37</code> | direct_member | 请求.幂等主键,请求.有序组成项组 | 被调函数结果按当前调用表达式接收或用于条件判断 | all components admitted | data-operation static type audited |
| RCE2076 | R0700 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:169, 海中鱼巣/领域/数据操作.特征体系.ixx:174</code> | direct_free | 二次特征/ordered component | 被调函数结果按当前调用表达式接收或用于条件判断 | status,key and vector shape accepted | node-handle overload is exact |
| RCE2077 | R0700 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:169</code> | direct_free | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | status,key,node accepted | main-information-handle overload is exact |
| RCE2078 | R0700 | R0387 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:174</code> | direct_const_member | 组成关系组[索引] | 被调函数结果按当前调用表达式接收或用于条件判断 | component loop | value static type audited |
| RCE2079 | R0700 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:176</code> | operator | relationship target and ordered component | 被调函数结果按当前调用表达式接收或用于条件判断 | component evidence complete | node-handle equality overload is exact |
| RCE2080 | R0701 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:921</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE2081 | R0701 | R0703 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:921</code> | direct_static_member | 有序组成项组 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class helper |
| RCE2082 | R0701 | R0383 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:923</code> | direct_member | 组成项 | 被调函数结果按当前调用表达式接收或用于条件判断 | component loop | same class existing member |
| RCE2083 | R0701 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:924</code> | direct_const_member | 身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity read returned | value static type audited |
| RCE2084 | R0701 | R0367 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:924, 海中鱼巣/领域/数据操作.特征体系.ixx:937</code> | direct_static_member | 身份.状态; 写前.状态 | 两处 R0367 结果均直接作为 R0701 当前 return 返回 | 924: 当前循环项读取后身份不可读；937: 写前状态既非未找到也非类型不匹配 | 冻结源码 3920a746 + 调用方 918-937 行、同类静态成员与 R0367 完整签名复核 |
| RCE2085 | R0701 | R0702 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:925</code> | direct_static_member | 身份.类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity current-readable | same class helper |
| RCE2086 | R0701 | R0704 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:928, 海中鱼巣/领域/数据操作.特征体系.ixx:961</code> | direct_member | 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-write/post-write read | same class member |
| RCE2087 | R0701 | R0700 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:929, 海中鱼巣/领域/数据操作.特征体系.ixx:962</code> | direct_const_member | 写前/当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | read returned | value static type audited |
| RCE2088 | R0701 | R0707 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:931, 海中鱼巣/领域/数据操作.特征体系.ixx:963</code> | direct_static_member | business status,material | 被调函数结果按当前调用表达式接收或用于条件判断 | pre-read/current matches ordered components | same class helper |
| RCE2089 | R0701 | R0116 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:941</code> | direct_const_member | callback=F0455C_L007 | 被调函数结果按当前调用表达式接收或用于条件判断 | no existing secondary feature | non-participant execute overload is exact |
| RCE2090 | R0116 | R0708 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | callback_invoke | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | R0116 invoked with callback formed at 数据操作.特征体系.ixx:941 | callback object provenance audited |
| RCE2091 | R0708 | R0030 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:943</code> | direct_member | 组成项 | 被调函数结果按当前调用表达式接收或用于条件判断 | callback component loop | session static type audited |
| RCE2092 | R0708 | R0702 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:944</code> | direct_static_member | *类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | session node type exists | same class helper |
| RCE2093 | R0708 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:951-952</code> | direct_static_member | 会话, 节点类型::二次特征, 主键, 新主信息, 新二次特征 | bool 取反用于 R0708 lambda 提前返回 | 有序组成项全部类型可读且允许 | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE2094 | R0708 | R0023 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:954</code> | direct_member | 引用,新二次特征,component,index | 被调函数结果按当前调用表达式接收或用于条件判断 | secondary node created | session static type audited |
| RCE2095 | R0708 | R0038 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:957</code> | direct_member | *关系结果.值 | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship candidate created | session static type audited |
| RCE2096 | R0708 | R0041 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:959</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | all relationship candidates readable | session static type audited |
| RCE2097 | R0701 | R0368 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:973</code> | direct_static_member | 结构结果 | R0368 结果直接作为 R0701 当前 return 返回 | 当前完整且有序组成项匹配不成立、当前状态既非已找到也非类型不匹配，且版本已经漂移为 false | 冻结源码 3920a746 + 调用方 940-973 行、同类静态成员与 R0368 完整签名复核 |
| RCE2098 | R0703 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1015</code> | direct_free | 组成项组[左] | 被调函数结果按当前调用表达式接收或用于条件判断 | component loop | node-handle overload is exact |
| RCE2099 | R0703 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1017</code> | operator | component node handles | 被调函数结果按当前调用表达式接收或用于条件判断 | duplicate scan | node-handle equality overload is exact |
| RCE2100 | R0704 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:652</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE2101 | R0704 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:654</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE2102 | R0704 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:655</code> | direct_const_member | 主键,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | token overload is exact |
| RCE2103 | R0704 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:655, 海中鱼巣/领域/数据操作.特征体系.ixx:657</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE2104 | R0704 | R0705 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:657</code> | direct_member | *目标,主键,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | primary-key lookup found node | same class helper |
| RCE2105 | R0704 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:658</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits | licensed permit local lifetime |
| RCE2106 | R0705 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1336, 海中鱼巣/领域/数据操作.特征体系.ixx:1358</code> | direct_member | target/component,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | licensed read | same class existing helper |
| RCE2107 | R0705 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1337, 海中鱼巣/领域/数据操作.特征体系.ixx:1359</code> | direct_const_member | identity | 被调函数结果按当前调用表达式接收或用于条件判断 | identity read returned | value static type audited |
| RCE2108 | R0705 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1345</code> | direct_member | 目标,期望主键,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | target identity secondary-feature typed | same class existing helper |
| RCE2109 | R0705 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1347</code> | direct_const_member | 目标,引用,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | unique key verified | token overload is exact |
| RCE2110 | R0705 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1357</code> | direct_member | record,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | component relationship examined | same class existing helper |
| RCE2111 | R0705 | R0702 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1360</code> | direct_static_member | 项身份.类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | component identity readable | same class helper |
| RCE2112 | R0705 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1360, 海中鱼巣/领域/数据操作.特征体系.ixx:1364</code> | operator | component/target node handles | 被调函数结果按当前调用表达式接收或用于条件判断 | component validation and duplicate scan | node-handle equality overload is exact |
| RCE2113 | R0705 | R0706 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1346, 海中鱼巣/领域/数据操作.特征体系.ixx:1348, 海中鱼巣/领域/数据操作.特征体系.ixx:1353, 海中鱼巣/领域/数据操作.特征体系.ixx:1356, 海中鱼巣/领域/数据操作.特征体系.ixx:1361, 海中鱼巣/领域/数据操作.特征体系.ixx:1365, 海中鱼巣/领域/数据操作.特征体系.ixx:1375</code> | direct_member | 目标,diagnostic | 被调函数结果按当前调用表达式接收或用于条件判断 | secondary-feature structure inconsistency | same class helper |
| RCE2114 | R0706 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1411</code> | direct_free | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | inconsistency helper entered | existing unique diagnostic helper |
| RCE2115 | R0709 | F0163 | <code>海中鱼巣/领域/服务.概念图结构.ixx:20</code> | direct_free | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | stable key nonzero | node-handle overload is exact |
| RCE2116 | R0710 | R0709 | <code>海中鱼巣/领域/服务.概念图结构.ixx:31, 海中鱼巣/领域/服务.概念图结构.ixx:32</code> | direct_const_member | four root identity requests | 被调函数结果按当前调用表达式接收或用于条件判断 | request completeness evaluation | field static type audited |
| RCE2117 | R0711 | R0710 | <code>海中鱼巣/领域/服务.概念图结构.ixx:85</code> | direct_const_member | 请求 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | request static type audited |
| RCE2118 | R0711 | R0717 | <code>海中鱼巣/领域/服务.概念图结构.ixx:86</code> | direct_member | four typed root candidates | 被调函数结果按当前调用表达式接收或用于条件判断 | request complete | data-operation static type audited |
| RCE2119 | R0712 | F0163 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:62</code> | direct_free | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | category defined and key nonzero | node-handle overload is exact |
| RCE2120 | R0713 | R0712 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:73, 海中鱼巣/领域/数据操作.概念图结构.ixx:74, 海中鱼巣/领域/数据操作.概念图结构.ixx:75, 海中鱼巣/领域/数据操作.概念图结构.ixx:76</code> | direct_const_member | four candidates | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate-group completeness evaluation | field static type audited |
| RCE2121 | R0714 | F0163 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:98</code> | direct_free | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | category and key accepted | node-handle overload is exact |
| RCE2122 | R0714 | F0565 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:98</code> | direct_free | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | category,key,node accepted | main-information-handle overload is exact |
| RCE2123 | R0715 | R0714 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:109, 海中鱼巣/领域/数据操作.概念图结构.ixx:110, 海中鱼巣/领域/数据操作.概念图结构.ixx:111, 海中鱼巣/领域/数据操作.概念图结构.ixx:112</code> | direct_const_member | four root roles | 被调函数结果按当前调用表达式接收或用于条件判断 | role-group completeness evaluation | field static type audited |
| RCE2124 | R0715 | F0051 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:113, 海中鱼巣/领域/数据操作.概念图结构.ixx:114, 海中鱼巣/领域/数据操作.概念图结构.ixx:115</code> | operator | six pairwise root node comparisons | 被调函数结果按当前调用表达式接收或用于条件判断 | all four roles complete and category-matched | node-handle equality overload is exact |
| RCE2125 | R0716 | R0715 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:140</code> | direct_const_member | 根角色 | 被调函数结果按当前调用表达式接收或用于条件判断 | read status is 已找到 | field static type audited |
| RCE2126 | R0717 | F0447 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:273</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same class stable identity |
| RCE2127 | R0717 | R0713 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:273</code> | direct_const_member | 候选组 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | value static type audited |
| RCE2128 | R0717 | F0338 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:275</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared permit acquired | permit static type audited |
| RCE2129 | R0717 | F0339 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:276</code> | direct_const_member | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | permit static type audited |
| RCE2130 | R0717 | R0719 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:276</code> | direct_member | 候选组,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | permit valid | same class helper |
| RCE2131 | R0717 | F0345 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:277</code> | implicit_destructor | 许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | function scope exits | licensed permit local lifetime |
| RCE2132 | R0719 | R0720 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:371, 海中鱼巣/领域/数据操作.概念图结构.ixx:373, 海中鱼巣/领域/数据操作.概念图结构.ixx:376, 海中鱼巣/领域/数据操作.概念图结构.ixx:379</code> | direct_member | one candidate,status output,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | preceding root checks remain 已找到 | same class helper |
| RCE2133 | R0719 | R0715 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:385</code> | direct_const_member | 输出.根角色 | 被调函数结果按当前调用表达式接收或用于条件判断 | all four single-root checks returned 已找到 | value static type audited |
| RCE2134 | R0720 | R0712 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:398</code> | direct_const_member | 候选 | 被调函数结果按当前调用表达式接收或用于条件判断 | single-root entry evaluation | value static type audited |
| RCE2135 | R0720 | R0718 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:398, 海中鱼巣/领域/数据操作.概念图结构.ixx:407</code> | direct_static_member | 候选.类别 | 被调函数结果按当前调用表达式接收或用于条件判断 | single-root admission and type comparison | same class helper |
| RCE2136 | R0720 | F0346 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:402</code> | direct_const_member | 候选.节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate complete and category supported | token overload is exact |
| RCE2137 | R0720 | F0439 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:411</code> | direct_const_member | 节点记录-&gt;主信息,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | node record type matches root category | token overload is exact |
| RCE2138 | R0720 | R0723 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:415</code> | direct_const_member | 候选.节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | node and main-information current-readable | token overload definition audited |
| RCE2139 | R0720 | F0441 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:417</code> | direct_const_member | 候选.稳定主键,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | key-set vector equals expected singleton | token overload is exact |
| RCE2140 | R0720 | R0079 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:422, 海中鱼巣/领域/数据操作.概念图结构.ixx:428</code> | direct_const_member | 候选.节点,概念上下位/运行期临时,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | identity and stable key match | token overload is exact |
| RCE2141 | R0723 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:217</code> | direct_free | 事务接线_,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | entry evaluation | same translation-unit helper stable identity |
| RCE2142 | R0723 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:217</code> | direct_const_member | 节点,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | shared token valid | token overload is exact |
| RCE2143 | R0688 | R0724 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:112</code> | direct_constructor | 幂等主键,来源动态,来源主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | forming a write specification | private constructor selected by exact class and arguments |
| RCE2144 | R0655 | R0642 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:326</code> | function_template_specialization | 关系结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship candidate returned | 带值结构写入结果&lt;关系句柄&gt; exact specialization |
| RCE2145 | R0656 | R0640 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:372</code> | function_template_specialization | 主信息结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate returned | 带值结构写入结果&lt;主信息句柄&gt; exact specialization |
| RCE2146 | R0656 | R0641 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:375</code> | function_template_specialization | 节点结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate returned | 带值结构写入结果&lt;节点句柄&gt; exact specialization |
| RCE2147 | R0656 | R0642 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:380</code> | function_template_specialization | 关系结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | relationship candidate returned | 带值结构写入结果&lt;关系句柄&gt; exact specialization |
| RCE2148 | R0678 | R0640 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:533</code> | function_template_specialization | 主结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate returned | 带值结构写入结果&lt;主信息句柄&gt; exact specialization |
| RCE2149 | R0678 | R0641 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:537</code> | function_template_specialization | 节点结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate returned | 带值结构写入结果&lt;节点句柄&gt; exact specialization |
| RCE2150 | R0696 | R0640 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:156</code> | function_template_specialization | 主结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | main-information candidate returned | 带值结构写入结果&lt;主信息句柄&gt; exact specialization |
| RCE2151 | R0696 | R0641 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:159</code> | function_template_specialization | 节点结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | node candidate returned | 带值结构写入结果&lt;节点句柄&gt; exact specialization |
| RCE2152 | R0708 | R0642 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:957</code> | function_template_specialization | 关系结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | component relationship candidate returned | 带值结构写入结果&lt;关系句柄&gt; exact specialization |
| RCE2153 | R0656 | R0138 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:378</code> | direct_const_member | 绑定主键 result | 被调函数结果按当前调用表达式接收或用于条件判断 | index binding returned | return static type is 结构写入结果 |
| RCE2154 | R0678 | R0138 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:535, 海中鱼巣/领域/数据操作.状态动态.ixx:540</code> | direct_const_member | write-I64/index-binding results | 被调函数结果按当前调用表达式接收或用于条件判断 | candidate operation returned | return static type is 结构写入结果 |
| RCE2155 | R0696 | R0138 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:162</code> | direct_const_member | 绑定主键 result | 被调函数结果按当前调用表达式接收或用于条件判断 | index binding returned | return static type is 结构写入结果 |
| RCE2156 | R0063 | R0723 | <code>海中鱼巣/核心/会话.结构写入.ixx:108</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2157 | R0033 | R0723 | <code>海中鱼巣/核心/会话.结构写入.ixx:543</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2158 | R0347 | R0723 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1070</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2159 | R0396 | R0723 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1072</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2160 | R0422 | R0723 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:68</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2161 | R0413 | R0723 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:239</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2162 | R0424 | R0723 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:205</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2163 | R0435 | R0723 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1950</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2164 | R0358 | R0638 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1455</code> | direct_member | this=&amp;会话; 主键, 新节点 | bool 作为最终返回表达式第三项 | R0036 与 R0037 均返回 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE2165 | R0359 | R0638 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1467</code> | direct_member | this=&amp;会话; 定义.幂等主键, 定义.特征定义 | bool 作为 &amp;&amp; 倒数第二项 | 前序宿主/定义类型与主信息比较全部为 true | 冻结源码 3920a746 + 当前调用点、静态类型、完整签名与短路条件复核 |
| RCE2166 | R0405 | R0638 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:914</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2167 | R0414 | R0638 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:252</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2168 | R0433 | R0638 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1647</code> | source-audited-reverse-closure | 按源码实参与静态类型绑定 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方从 F0001 可达且对应源码分支成立 | 当前源码定义、调用点与静态类型复核 |
| RCE2169 | R0455 | R0638 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2915</code> | overload_resolved_direct_member | this=&amp;会话, 主键=规格.需求主键_, 预期节点=新需求 | bool；作为请求提交前三项短路合取的第三项 | R0036与R0037均返回true | 冻结源码双实参静态类型、完整重载集合与R0638签名复核 |
| RCE2170 | F0455 | R0369 | <code>海中鱼巣/领域/初始化.系统角色.ixx:136</code> | source-audited-direct | 概念关系根.成功() | bool结果进入失败判断或改变了结构累计表达式 | F0455 前序创建步骤成功并执行到对应源码行 | 初始化.系统角色.ixx:136-150 源码表达式与现有稳定身份复核 |
| RCE2171 | F0455 | R0370 | <code>海中鱼巣/领域/初始化.系统角色.ixx:137</code> | source-audited-direct | 概念关系根.改变了结构() | bool结果进入失败判断或改变了结构累计表达式 | F0455 前序创建步骤成功并执行到对应源码行 | 初始化.系统角色.ixx:136-150 源码表达式与现有稳定身份复核 |
| RCE2172 | F0455 | R0603 | <code>海中鱼巣/领域/初始化.系统角色.ixx:150</code> | source-audited-direct | 状态已提交(方法登记根.状态) | bool结果进入失败判断或改变了结构累计表达式 | F0455 前序创建步骤成功并执行到对应源码行 | 初始化.系统角色.ixx:136-150 源码表达式与现有稳定身份复核 |
| RCE2173 | F0455 | F0074 | <code>海中鱼巣/领域/初始化.系统角色.ixx:161, 海中鱼巣/领域/初始化.系统角色.ixx:166</code> | source-audited-direct | 拓扑；最终复核 | 成功bool进入提前返回判断 | 拓扑发布或最终复核结果已形成 | 初始化.系统角色.ixx:161,166 与 F0074 完整签名复核 |
| RCE2174 | F0458 | F0459 | <code>海中鱼巣/领域/系统角色清单.数据.h:190</code> | source-audited-direct-const-member | this=&amp;场景接纳自我关系 | bool进入入口短路判断 | 前三个身份材料完整 | 系统角色清单.数据.h:190 静态接收者复核 |
| RCE2175 | F0458 | R0727 | <code>海中鱼巣/领域/系统角色清单.数据.h:190, 海中鱼巣/领域/系统角色清单.数据.h:191</code> | source-audited-direct-const-member | this=&amp;安全根需求；this=&amp;服务根需求 | bool进入入口短路判断 | 按源码左到右短路 | 系统角色清单.数据.h:190-191 静态接收者复核 |
| RCE2176 | F0458 | F0051 | <code>海中鱼巣/领域/系统角色清单.数据.h:202, 海中鱼巣/领域/系统角色清单.数据.h:207, 海中鱼巣/领域/系统角色清单.数据.h:208</code> | source-audited-direct-operator | 节点句柄左右值 | bool进入重复性或归属关系判断 | 循环或最终关系复核到达对应表达式 | 系统角色清单.数据.h:202,207-208 操作数静态类型复核 |
| RCE2177 | F0458 | R0615 | <code>海中鱼巣/领域/系统角色清单.数据.h:203</code> | source-audited-direct-operator | 身份组左右主信息句柄 | bool进入重复性判断 | 同对稳定主键和节点句柄均不相等 | 系统角色清单.数据.h:203 操作数静态类型复核 |
| RCE2178 | F0459 | F0163 | <code>海中鱼巣/领域/系统角色清单.数据.h:134</code> | source-audited-direct-free | 源节点；目标节点 | 两个 bool 进入短路完整性判断 | 关系句柄有效后按源码短路调用 | 系统角色清单.数据.h:134 实参静态类型复核 |
| RCE2179 | F0461 | F0451 | <code>海中鱼巣/领域/服务.概念活动.ixx:71</code> | source-audited-direct-const-member | this=&amp;当前概念活动业务服务 | bool进入入口短路门禁 | 取得活动锁后必达 | 服务.概念活动.ixx:71 源码复核 |
| RCE2180 | F0461 | F0245 | <code>海中鱼巣/领域/服务.概念活动.ixx:71</code> | source-audited-direct-const-member | this=&amp;系统角色 | bool进入入口短路门禁 | F0451 返回 true | 服务.概念活动.ixx:71 源码复核 |
| RCE2181 | F0461 | F0251 | <code>海中鱼巣/领域/服务.概念活动.ixx:71</code> | source-audited-direct-const-member | this=&amp;预期材料 | bool进入入口短路门禁 | 前两项入口门禁通过 | 服务.概念活动.ixx:71 源码复核 |
| RCE2182 | F0461 | F0250 | <code>海中鱼巣/领域/服务.概念活动.ixx:72</code> | source-audited-direct-const-member | this=活动材料_所含材料；右=预期材料 | bool进入入口短路门禁 | 活动材料_有值 | 服务.概念活动.ixx:72 静态接收者复核 |
| RCE2183 | F0461 | F0464 | <code>海中鱼巣/领域/服务.概念活动.ixx:76</code> | source-audited-direct-const-member | this=&amp;预期材料.重建视图；右=重建.视图 | bool决定内部不一致返回 | 重建结果成功 | 服务.概念活动.ixx:76 静态接收者复核 |
| RCE2184 | F0462 | F0451 | <code>海中鱼巣/领域/服务.概念活动.ixx:32</code> | source-audited-direct-const-member | this=&amp;当前概念活动业务服务 | bool进入入口短路门禁 | 取得活动锁后必达 | 服务.概念活动.ixx:32 源码复核 |
| RCE2185 | F0462 | F0245 | <code>海中鱼巣/领域/服务.概念活动.ixx:32</code> | source-audited-direct-const-member | this=&amp;系统角色 | bool进入入口短路门禁 | F0451 返回 true | 服务.概念活动.ixx:32 源码复核 |
| RCE2186 | F0462 | F0229 | <code>海中鱼巣/领域/服务.概念活动.ixx:32</code> | source-audited-direct-const-member | this=&amp;参数 | bool进入入口短路门禁 | 前两项入口门禁通过 | 服务.概念活动.ixx:32 源码复核 |
| RCE2187 | F0462 | F0247 | <code>海中鱼巣/领域/服务.概念活动.ixx:35</code> | source-audited-direct-const-member | this=活动材料_所含材料；参数=F0462参数 | bool决定幂等冲突返回 | 活动材料_有值 | 服务.概念活动.ixx:35 静态接收者复核 |
| RCE2188 | F0462 | R0539 | <code>海中鱼巣/领域/服务.概念活动.ixx:39, 海中鱼巣/领域/服务.概念活动.ixx:40</code> | source-audited-direct-const-member | this=&amp;重建 | bool进入短路判断或三元表达式 | 重建结果已形成；第40行仅失败或视图不等分支 | 服务.概念活动.ixx:39-40 源码复核 |
| RCE2189 | F0462 | F0464 | <code>海中鱼巣/领域/服务.概念活动.ixx:39</code> | source-audited-direct-const-member | this=&amp;活动材料_-&gt;重建视图；右=重建.视图 | bool进入幂等读回一致性判断 | 重建结果成功 | 服务.概念活动.ixx:39 静态接收者复核 |
| RCE2190 | F0462 | R0511 | <code>海中鱼巣/领域/服务.概念活动.ixx:47, 海中鱼巣/领域/服务.概念活动.ixx:50, 海中鱼巣/领域/服务.概念活动.ixx:53</code> | source-audited-direct-const-member | 活跃、冷却、退役三份创建抽象状态请求 | 依次绑定活跃规格、冷却规格、退役规格 | 活动材料_无值 | 服务.概念活动.ixx:47-55 源码复核 |
| RCE2191 | F0462 | R0510 | <code>海中鱼巣/领域/服务.概念活动.ixx:56</code> | source-audited-direct-const-member | this依次为活跃规格、冷却规格、退役规格 | 三个 bool 进入短路门禁 | 无缓存且三份规格结果已形成 | 服务.概念活动.ixx:56 静态接收者复核 |
| RCE2192 | F0462 | F0076 | <code>海中鱼巣/领域/服务.概念活动.ixx:61</code> | source-audited-direct-const-member | this=&amp;结果 | bool进入结果完整性门禁 | 初始化全新活动返回 | 服务.概念活动.ixx:61 静态接收者复核 |
| RCE2193 | F0462 | R0726 | <code>海中鱼巣/领域/服务.概念活动.ixx:61</code> | source-audited-direct-const-member | this=&amp;结果 | bool要求本次结果确实改变结构 | F0076 返回 true | 服务.概念活动.ixx:61 与新身份定义复核 |
| RCE2194 | F0463 | F0051 | <code>海中鱼巣/领域/概念活动状态.数据.h:70</code> | compiler-generated-defaulted-member-compare | 左右状态节点句柄 | bool进入逐字段短路比较 | 前序阶段和稳定主键字段相等 | 默认化 operator== 成员顺序与句柄静态类型复核 |
| RCE2195 | F0463 | R0615 | <code>海中鱼巣/领域/概念活动状态.数据.h:70</code> | compiler-generated-defaulted-member-compare | 左右主信息句柄 | bool进入逐字段短路比较 | 前序阶段、稳定主键和状态节点字段相等 | 默认化 operator== 成员顺序与句柄静态类型复核 |
| RCE2196 | F0464 | F0463 | <code>海中鱼巣/领域/概念活动状态.数据.h:144</code> | standard-template-element-compare | 两侧状态角色数组对应元素 | 元素 bool 由 array 等值比较短路编排 | 入口后数组比较期间动态调用一至三次 | 概念活动状态.数据.h:144 模板元素类型复核 |
| RCE2197 | F0465 | R0613 | <code>海中鱼巣/领域/概念活动状态.数据.h:116</code> | source-audited-direct-const-member | 左右根身份材料 | bool进入短路相等判断 | 类别相等 | 概念活动状态.数据.h:116 静态类型复核 |
| RCE2198 | F0465 | F0457 | <code>海中鱼巣/领域/概念活动状态.数据.h:119</code> | source-audited-direct-const-member | 左右活跃关系材料 | bool作为最终相等结果 | 类别、根身份及两份规范签名均一致 | 概念活动状态.数据.h:119 静态类型复核 |
| RCE2199 | F0466 | F0467 | <code>海中鱼巣/领域/概念活动状态.数据.h:128, 海中鱼巣/领域/概念活动状态.数据.h:129</code> | source-audited-direct-const-member | 状态角色组第0、第1、第2元素 | 三个 bool 进入短路完整性门禁 | 第一个必达，后两个按前序 true 短路到达 | 概念活动状态.数据.h:128-129 静态接收者复核 |
| RCE2200 | F0468 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:61</code> | source-audited-direct-const-member | this=&amp;装配.上下文-&gt;节点；无显式参数 | uint64_t 转为 size_t 后写入计数.节点数量 | 装配成功 | 自检.入口初始化.ixx:60 接收者与零参数重载复核 |
| RCE2201 | R0727 | R0563 | <code>海中鱼巣/领域/系统角色清单.数据.h:153, 海中鱼巣/领域/系统角色清单.数据.h:154</code> | source-audited-direct-const-member | 五个系统角色身份材料接收者，无显式参数 | 五个 bool 进入短路完整性判断 | 按源码左到右短路 | 系统角色清单.数据.h:153-154 静态接收者复核 |
| RCE2202 | F0496 | F0621 | <code>海中鱼巣/领域/语素服务.h:192</code> | source-audited-overload-direct | this=&amp;关系_；源节点=语素入口；类型=语素对应信息 | 目标节点组按值返回 | R0728确认节点是语素入口 | 语素服务.h:192 接收者静态类型、实参数量与关系类型复核 |
| RCE2203 | F0497 | F0621 | <code>海中鱼巣/领域/语素服务.h:199</code> | source-audited-overload-direct | this=&amp;关系_；源节点=语素入口；类型=语素概念追溯 | 目标节点组按值返回 | R0728确认节点是语素入口 | 语素服务.h:199 接收者静态类型、实参数量与关系类型复核 |
| RCE2204 | R0728 | F0190 | <code>海中鱼巣/领域/语素服务.h:253</code> | source-audited-overload-direct | this=&amp;节点_；节点=节点句柄值 | optional节点记录绑定为记录 | R0728入口后必达 | 语素服务.h:253 接收者静态类型与单实参重载复核 |
| RCE2206 | F0516 | F0331 | <code>海中鱼巣/领域/存在服务.h:94</code> | source-audited-overload-direct | this=&amp;主信息_；无显式实参 | 主信息句柄绑定为主信息句柄 | F0516入口后必达 | 存在服务.h:94 接收者静态类型与零实参重载复核 |
| RCE2207 | F0516 | F0332 | <code>海中鱼巣/领域/存在服务.h:95</code> | source-audited-overload-direct | this=&amp;节点_；类型=节点类型::存在；主信息=主信息句柄 | 节点句柄绑定为存在 | 主信息句柄已形成 | 存在服务.h:95 接收者静态类型与两实参重载复核 |
| RCE2208 | F0516 | F0624 | <code>海中鱼巣/领域/存在服务.h:97</code> | source-audited-overload-direct | this=&amp;主信息_；主信息=记录-&gt;主信息 | bool进入短路判断 | 读取存在返回有值 | 存在服务.h:97 接收者静态类型、单实参与短路条件复核 |
| RCE2209 | F0523 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:332</code> | source-audited-direct-const-member | this=&amp;事务接线_ | 已接域bool进入分支判断 | F0523入口后必达 | 节点仓库.cpp:332 静态接收者、成员签名与分支顺序复核 |
| RCE2210 | F0523 | F0398 | <code>海中鱼巣/核心/节点仓库.cpp:333</code> | source-audited-resolved-function-pointer | 状态=事务接线_.运行期状态 | 结构事务许可绑定为许可 | RCE2209返回true | 结构事务接线字段、协调器生成接线与生产装配唯一独占许可目标复核 |
| RCE2211 | F0523 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:334</code> | source-audited-direct-const-member | this=&amp;许可 | 许可有效bool进入短路判断 | RCE2210已形成许可 | 节点仓库.cpp:334 静态接收者与逻辑与短路顺序复核 |
| RCE2212 | F0523 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:334</code> | source-audited-direct-const-member | this=&amp;许可 | const结构事务令牌引用传给R0147 | RCE2211返回true；逻辑与右侧开始求值 | 节点仓库.cpp:334 静态接收者、实参绑定与短路顺序复核 |
| RCE2213 | F0523 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:333-335</code> | source-audited-implicit-destructor | this=&amp;许可 | void | RCE2210已形成许可；返回值求值完成或许可形成后的异常展开 | 节点仓库.cpp:333-335 局部许可生命周期与已登记F0345析构身份复核 |
| RCE2214 | F0544 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:173</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | F0544进入后首先判断接域 | 本批源码逐调用点与静态接收者复核 |
| RCE2215 | F0544 | F0397 | <code>海中鱼巣/核心/索引仓库.cpp:174</code> | resolved-function-pointer | this=&amp;事务接线_,事务接线_.运行期状态 | 结构事务许可 | RCE2214返回true；生产接线唯一共享许可目标 | 本批源码逐调用点与静态接收者复核 |
| RCE2216 | F0544 | F0338 | <code>海中鱼巣/核心/索引仓库.cpp:175</code> | direct_const_member | this=&amp;许可 | 许可有效bool | RCE2215返回许可 | 本批源码逐调用点与静态接收者复核 |
| RCE2217 | F0544 | F0339 | <code>海中鱼巣/核心/索引仓库.cpp:175</code> | direct_const_member | this=&amp;许可 | const结构事务令牌&amp; | RCE2216返回true | 本批源码逐调用点与静态接收者复核 |
| RCE2218 | F0544 | F0345 | <code>海中鱼巣/核心/索引仓库.cpp:173-176</code> | compiler_implicit_destructor | this=&amp;许可 |  | 接域分支任一返回或异常展开退出局部许可作用域 | 本批源码逐调用点与静态接收者复核 |
| RCE2219 | F0544 | F0377 | <code>海中鱼巣/核心/索引仓库.cpp:185</code> | direct_const_member | this=&amp;节点_,候选 | 节点有效bool | 未接域、主键非零、索引命中并释放共享锁后 | 本批源码逐调用点与静态接收者复核 |
| RCE2220 | F0552 | R0729 | <code>海中鱼巣/领域/需求服务.h:561</code> | direct_const_member | this,需求节点,节点类型::需求 | 类型匹配bool | F0552进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2221 | F0552 | R0582 | <code>海中鱼巣/领域/需求服务.h:564</code> | direct_const_member | this,需求节点 | 主体optional | RCE2220返回true | 本批源码逐调用点与静态接收者复核 |
| RCE2222 | F0552 | R0583 | <code>海中鱼巣/领域/需求服务.h:565</code> | direct_const_member | this,需求节点 | 目标宿主optional | RCE2221返回后 | 本批源码逐调用点与静态接收者复核 |
| RCE2223 | F0552 | R0584 | <code>海中鱼巣/领域/需求服务.h:566</code> | direct_const_member | this,需求节点 | 场景optional | RCE2222返回后 | 本批源码逐调用点与静态接收者复核 |
| RCE2224 | F0552 | R0581 | <code>海中鱼巣/领域/需求服务.h:567</code> | direct_const_member | this,需求节点 | 目标状态optional | RCE2223返回后 | 本批源码逐调用点与静态接收者复核 |
| RCE2225 | R0579 | R0729 | <code>海中鱼巣/领域/需求服务.h:295, 海中鱼巣/领域/需求服务.h:296, 海中鱼巣/领域/需求服务.h:297</code> | direct_const_member | this,自我存在/需求场景/目标特征,对应节点类型 | 类型匹配bool | R0579入口短路按源码顺序执行 | 本批源码逐调用点与静态接收者复核 |
| RCE2226 | R0581 | R0729 | <code>海中鱼巣/领域/需求服务.h:354, 海中鱼巣/领域/需求服务.h:360</code> | direct_const_member | this,需求节点或候选目标,需求/状态类型 | 类型匹配bool | R0581入口及候选过滤路径 | 本批源码逐调用点与静态接收者复核 |
| RCE2227 | R0587 | R0729 | <code>海中鱼巣/领域/需求服务.h:705</code> | direct_const_member | this,需求节点,节点类型::需求 | 类型匹配bool | R0587进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2228 | R0593 | R0729 | <code>海中鱼巣/领域/需求服务.h:770, 海中鱼巣/领域/需求服务.h:773</code> | direct_const_member | this,需求节点/父需求,节点类型::需求 | 类型匹配bool | R0593入口与父需求门禁 | 本批源码逐调用点与静态接收者复核 |
| RCE2229 | R0596 | R0729 | <code>海中鱼巣/领域/需求服务.h:979</code> | direct_const_member | this,结算状态,节点类型::状态 | 类型匹配bool | R0596进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2230 | R0597 | R0729 | <code>海中鱼巣/领域/需求服务.h:988, 海中鱼巣/领域/需求服务.h:994</code> | direct_const_member | this,结算状态/目标,状态或目标类型 | 类型匹配bool | R0597入口与候选过滤路径 | 本批源码逐调用点与静态接收者复核 |
| RCE2231 | R0576 | R0729 | <code>海中鱼巣/领域/需求服务.h:1007, 海中鱼巣/领域/需求服务.h:1013</code> | direct_const_member | this,需求节点/目标,需求或目标类型 | 类型匹配bool | R0576入口与候选过滤路径 | 本批源码逐调用点与静态接收者复核 |
| RCE2232 | R0577 | R0729 | <code>海中鱼巣/领域/需求服务.h:1027, 海中鱼巣/领域/需求服务.h:1031</code> | direct_const_member | this,结算状态/目标,状态或目标类型 | 类型匹配bool | R0577入口与候选过滤路径 | 本批源码逐调用点与静态接收者复核 |
| RCE2233 | R0729 | F0190 | <code>海中鱼巣/领域/需求服务.h:1039</code> | direct_const_member | this=&amp;节点_,节点句柄值 | 节点记录optional | R0729进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2234 | F0554 | F0621 | <code>海中鱼巣/领域/特征服务.h:266</code> | direct_const_member | this=关系_,宿主节点,关系类型::归属 | 候选组 | 关系指针非空且宿主/特征类型前置通过 | 本批源码逐调用点与静态接收者复核 |
| RCE2235 | F0554 | F0579 | <code>海中鱼巣/领域/特征服务.h:268</code> | direct_const_member | this=关系_,关系类型::模板,候选,特征类型 | 关系存在bool | 当前候选经RCE0256确认为特征 | 本批源码逐调用点与静态接收者复核 |
| RCE2236 | F0555 | R0730 | <code>海中鱼巣/领域/特征服务.h:367</code> | direct_const_member | this=特征值_,特征值节点 | I64值optional | RCE0257返回true且特征值指针非空 | 本批源码逐调用点与静态接收者复核 |
| RCE2237 | R0730 | F0321 | <code>海中鱼巣/领域/特征值服务.h:269</code> | direct_const_member | this=&amp;事务接线_ | 接线形态有效bool | R0730进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2238 | R0730 | F0336 | <code>海中鱼巣/领域/特征值服务.h:272</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | RCE2237返回true | 本批源码逐调用点与静态接收者复核 |
| RCE2239 | R0730 | F0397 | <code>海中鱼巣/领域/特征值服务.h:273</code> | resolved-function-pointer | this=&amp;事务接线_,事务接线_.运行期状态 | 结构事务许可 | RCE2238返回true；生产接线唯一共享许可目标 | 本批源码逐调用点与静态接收者复核 |
| RCE2240 | R0730 | F0338 | <code>海中鱼巣/领域/特征值服务.h:274</code> | direct_const_member | this=&amp;许可 | 许可有效bool | RCE2239返回许可 | 本批源码逐调用点与静态接收者复核 |
| RCE2241 | R0730 | F0339 | <code>海中鱼巣/领域/特征值服务.h:275</code> | direct_const_member | this=&amp;许可 | const结构事务令牌&amp; | RCE2240返回true | 本批源码逐调用点与静态接收者复核 |
| RCE2242 | R0730 | R0731 | <code>海中鱼巣/领域/特征值服务.h:277</code> | direct_const_member | this,特征值节点,令牌 | 节点记录optional | 接线形态有效且接域许可有效或未接域 | 本批源码逐调用点与静态接收者复核 |
| RCE2243 | R0730 | R0732 | <code>海中鱼巣/领域/特征值服务.h:283</code> | direct_const_member | this,特征值节点,节点记录值-&gt;主信息,令牌 | 原始值状态 | RCE2242返回有值且共享锁已取得 | 本批源码逐调用点与静态接收者复核 |
| RCE2244 | R0730 | F0345 | <code>海中鱼巣/领域/特征值服务.h:268-288</code> | compiler_implicit_destructor | this=&amp;许可 |  | 函数任一返回或异常展开退出许可作用域 | 本批源码逐调用点与静态接收者复核 |
| RCE2245 | R0731 | F0346 | <code>海中鱼巣/领域/特征值服务.h:601</code> | direct_const_member | this=&amp;节点_,特征值节点,*令牌 | 节点记录optional | 令牌指针非空 | 本批源码逐调用点与静态接收者复核 |
| RCE2246 | R0731 | F0190 | <code>海中鱼巣/领域/特征值服务.h:602</code> | direct_const_member | this=&amp;节点_,特征值节点 | 节点记录optional | 令牌指针为空 | 本批源码逐调用点与静态接收者复核 |
| RCE2247 | R0731 | R0009 | <code>海中鱼巣/领域/特征值服务.h:607</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息,*令牌 | 主信息有效bool | 节点记录类型有效且令牌非空 | 本批源码逐调用点与静态接收者复核 |
| RCE2248 | R0731 | F0624 | <code>海中鱼巣/领域/特征值服务.h:608</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息 | 主信息有效bool | 节点记录类型有效且令牌为空 | 本批源码逐调用点与静态接收者复核 |
| RCE2249 | R0731 | F0184 | <code>海中鱼巣/领域/特征值服务.h:609</code> | direct_free_function | 主信息有效,固定说明 | 追根因检查bool | 主信息有效判定完成 | 本批源码逐调用点与静态接收者复核 |
| RCE2250 | R0732 | R0554 | <code>海中鱼巣/领域/特征值服务.h:620</code> | direct_const_member | this,特征值节点 | Vec记录数量 | R0732进入 | 本批源码逐调用点与静态接收者复核 |
| RCE2251 | R0732 | R0556 | <code>海中鱼巣/领域/特征值服务.h:621</code> | direct_const_member | this,特征值节点 | I64版本记录数量 | RCE2250返回后 | 本批源码逐调用点与静态接收者复核 |
| RCE2252 | R0732 | F0184 | <code>海中鱼巣/领域/特征值服务.h:622, 海中鱼巣/领域/特征值服务.h:623, 海中鱼巣/领域/特征值服务.h:630, 海中鱼巣/领域/特征值服务.h:634, 海中鱼巣/领域/特征值服务.h:642, 海中鱼巣/领域/特征值服务.h:646, 海中鱼巣/领域/特征值服务.h:650, 海中鱼巣/领域/特征值服务.h:666</code> | direct_free_function | 各结构一致性条件,对应固定说明 | 追根因检查bool | 按源码短路与分支顺序执行 | 本批源码逐调用点与静态接收者复核 |
| RCE2253 | R0732 | R0548 | <code>海中鱼巣/领域/特征值服务.h:627</code> | direct_const_member | this,特征值节点 | Vec const_iterator | 记录数量检查通过 | 本批源码逐调用点与静态接收者复核 |
| RCE2254 | R0732 | R0552 | <code>海中鱼巣/领域/特征值服务.h:628</code> | direct_const_member | this,特征值节点 | I64版本 const_iterator | RCE2253返回后 | 本批源码逐调用点与静态接收者复核 |
| RCE2255 | R0732 | R0544 | <code>海中鱼巣/领域/特征值服务.h:630</code> | direct_static | *Vec位置 | Vec记录内部一致bool | Vec位置不等于cend | 本批源码逐调用点与静态接收者复核 |
| RCE2256 | R0732 | R0545 | <code>海中鱼巣/领域/特征值服务.h:634</code> | direct_static | *I64版本位置 | I64版本记录内部一致bool | I64版本位置不等于cend | 本批源码逐调用点与静态接收者复核 |
| RCE2257 | R0732 | F0383 | <code>海中鱼巣/领域/特征值服务.h:639</code> | direct_const_member | this=&amp;主信息_,主信息句柄值,0,*令牌 | I64值optional | 令牌指针非空且前序一致性检查通过 | 本批源码逐调用点与静态接收者复核 |
| RCE2258 | R0732 | F0619 | <code>海中鱼巣/领域/特征值服务.h:640</code> | direct_const_member | this=&amp;主信息_,主信息句柄值 | I64值optional | 令牌指针为空且前序一致性检查通过 | 本批源码逐调用点与静态接收者复核 |
| RCE2259 | F0566 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:463</code> | source-audited-direct-member | this=&amp;关系_,关系类型::普通父子,节点组_[1001],节点组_[1002],默认顺序号0 | 第二次普通父挂载关系句柄候选 | 前两项固定普通父关系均已形成有效句柄 | 本批源码逐调用点与完整重载复核 |
| RCE2260 | F0568 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:447</code> | source-audited-direct-member | this=&amp;关系_,类型,节点组_[源索引],节点组_[目标索引],顺序号 | 关系句柄候选 | 源目标索引有效且引用键未重复 | 本批源码逐调用点与完整重载复核 |
| RCE2261 | F0568 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:452</code> | source-audited-direct-const-member | this=&amp;关系_,关系 | 关系记录optional | RCE2260已返回有效关系句柄 | 本批源码逐调用点与完整重载复核 |
| RCE2262 | F0571 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:562</code> | source-audited-direct-free-operator | 记录.目标节点,目标节点 | 目标节点相等bool | 当前参考记录状态与关系类型均匹配 | 本批源码逐调用点与强类型运算符复核 |
| RCE2263 | F0572 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:543</code> | source-audited-direct-free-operator | 记录.源节点,源节点 | 源节点相等bool | 当前参考记录状态与关系类型均匹配 | 本批源码逐调用点与强类型运算符复核 |
| RCE2264 | F0573 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:595, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:596</code> | source-audited-direct-free-operator | 记录.源节点/记录.目标节点,源节点/目标节点 | 源/目标节点相等bool | 当前参考记录有效且关系类型匹配；目标比较受源比较短路 | 本批源码逐调用点与强类型运算符复核 |
| RCE2265 | F0574 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:577</code> | source-audited-direct-free-operator | 记录.源节点/记录.目标节点,节点 | 源或目标节点相等bool | 当前参考记录状态为有效；第二次比较受第一次结果短路 | 本批源码逐调用点与强类型运算符复核 |
| RCE2266 | F0575 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:854</code> | source-audited-direct-free-operator | 记录.源节点,源节点 | 源节点相等bool | 记录有效且关系类型匹配 | 本批源码逐调用点与强类型运算符复核 |
| RCE2267 | F0578 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0578进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2268 | F0578 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2267返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2269 | F0578 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2267返回true且RCE2268返回nullptr | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2270 | F0578 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2269返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2269返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2271 | F0578 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2270完成承载值构造 | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2272 | F0578 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2271返回true | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2273 | F0578 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:920</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2272返回const引用 | 自动令牌范围承载值 | RCE2271返回true且RCE2272已取得令牌引用 | 当前源码宏定义116-123行、调用点920行与已发布F0387同宏先例复核 |
| RCE2274 | F0578 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:931</code> | source-audited-direct-call | 记录.目标节点,目标节点 | 节点相等bool | 记录状态有效且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2275 | F0579 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0579进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2276 | F0579 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2275返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2277 | F0579 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2275返回true且RCE2276返回nullptr | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2278 | F0579 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2277返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2277返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2279 | F0579 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2278完成承载值构造 | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2280 | F0579 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2279返回true | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2281 | F0579 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:981</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2280返回const引用 | 自动令牌范围承载值 | RCE2279返回true且RCE2280已取得令牌引用 | 当前源码宏定义116-123行、调用点981行与已发布F0387同宏先例复核 |
| RCE2282 | F0579 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:988-989</code> | source-audited-direct-call | 记录.源节点/源节点；记录.目标节点/目标节点 | 两个节点相等bool | 状态与类型匹配；第二次受第一次及前序条件短路 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2283 | F0580 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0580进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2284 | F0580 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2283返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2285 | F0580 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2283返回true且RCE2284返回nullptr | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2286 | F0580 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2285返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2285返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2287 | F0580 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2286完成承载值构造 | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2288 | F0580 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2287返回true | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2289 | F0580 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:997</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2288返回const引用 | 自动令牌范围承载值 | RCE2287返回true且RCE2288已取得令牌引用 | 当前源码宏定义116-123行、调用点997行与已发布F0387同宏先例复核 |
| RCE2290 | F0580 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:1004</code> | source-audited-direct-call | 记录.目标节点,目标节点 | 节点相等bool | 记录状态有效且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2291 | F0581 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0581进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2292 | F0581 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2291返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2293 | F0581 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2291返回true且RCE2292返回nullptr | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2294 | F0581 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2293返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2293返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2295 | F0581 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2294完成承载值构造 | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2296 | F0581 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2295返回true | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2297 | F0581 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:950</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2296返回const引用 | 自动令牌范围承载值 | RCE2295返回true且RCE2296已取得令牌引用 | 当前源码宏定义116-123行、调用点950行与已发布F0387同宏先例复核 |
| RCE2298 | F0581 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:960</code> | source-audited-direct-call | 记录.源节点/节点；记录.目标节点/节点 | 两个节点相等bool | 记录有效；第二次为逻辑或右侧 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2299 | F0582 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0582进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2300 | F0582 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2299返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2301 | F0582 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2299返回true且RCE2300返回nullptr | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2302 | F0582 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2301返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2301返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2303 | F0582 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2302完成承载值构造 | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2304 | F0582 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2303返回true | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2305 | F0582 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:317</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2304返回const引用 | 自动令牌范围承载值 | RCE2303返回true且RCE2304已取得令牌引用 | 当前源码宏定义116-123行、调用点317行与已发布F0387同宏先例复核 |
| RCE2306 | F0582 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:319</code> | source-audited-direct-call | 源节点 | 节点句柄有效bool | 入口参数校验 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2307 | F0582 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:328</code> | source-audited-direct-call | 记录.源节点,源节点 | 节点相等bool | 关系状态可审计且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2308 | F0585 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:334</code> | source-audited-direct-call | this=&amp;关系_,当前关系 | 关系记录optional | 索引未越界且当前关系句柄有效 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2309 | F0586 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0586进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2310 | F0586 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2309返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2311 | F0586 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2309返回true且RCE2310返回nullptr | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2312 | F0586 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2311返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2311返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2313 | F0586 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2312完成承载值构造 | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2314 | F0586 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2313返回true | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2315 | F0586 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:435</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2314返回const引用 | 自动令牌范围承载值 | RCE2313返回true且RCE2314已取得令牌引用 | 当前源码宏定义116-123行、调用点435行与已发布F0387同宏先例复核 |
| RCE2316 | F0586 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:436</code> | source-audited-direct-call | 新源节点；新目标节点 | 两个节点句柄有效bool | 入口必达；第二次受第一次true短路 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2317 | F0586 | F0184 | <code>海中鱼巣/核心/关系仓库.cpp:460</code> | source-audited-direct-call | false,L"重挂关系时关系版本已达上限。" | 诊断bool丢弃 | 记录版本达到uint32上限 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2318 | F0586 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:470-471</code> | source-audited-direct-call | 其他源/新源；其他目标/新目标 | 端点相等bool | 遍历其它有效同类型关系并按短路条件执行 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2319 | F0587 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0587进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2320 | F0587 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2319返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2321 | F0587 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2319返回true且RCE2320返回nullptr | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2322 | F0587 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2321返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2321返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2323 | F0587 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2322完成承载值构造 | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2324 | F0587 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2323返回true | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2325 | F0587 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:278</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2324返回const引用 | 自动令牌范围承载值 | RCE2323返回true且RCE2324已取得令牌引用 | 当前源码宏定义116-123行、调用点278行与已发布F0387同宏先例复核 |
| RCE2326 | F0588 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:51-52</code> | source-audited-direct-call | 左.源节点/右.源节点；左.目标节点/右.目标节点 | 两个节点相等bool | 前序字段比较均相等，第二次受第一次短路 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2327 | F0589 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0589进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2328 | F0589 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2327返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2329 | F0589 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2327返回true且RCE2328返回nullptr | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2330 | F0589 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2329返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2329返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2331 | F0589 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2330完成承载值构造 | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2332 | F0589 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2331返回true | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2333 | F0589 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:296</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2332返回const引用 | 自动令牌范围承载值 | RCE2331返回true且RCE2332已取得令牌引用 | 当前源码宏定义116-123行、调用点296行与已发布F0387同宏先例复核 |
| RCE2334 | F0589 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:297</code> | source-audited-direct-call | 当前关系 | 关系句柄有效bool | 入口必达 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2335 | F0590 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0590进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2336 | F0590 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2335返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2337 | F0590 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2335返回true且RCE2336返回nullptr | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2338 | F0590 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2337返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2337返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2339 | F0590 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2338完成承载值构造 | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2340 | F0590 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2339返回true | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2341 | F0590 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:410</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2340返回const引用 | 自动令牌范围承载值 | RCE2339返回true且RCE2340已取得令牌引用 | 当前源码宏定义116-123行、调用点410行与已发布F0387同宏先例复核 |
| RCE2342 | F0591 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0591进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2343 | F0591 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2342返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2344 | F0591 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2342返回true且RCE2343返回nullptr | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2345 | F0591 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2344返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2344返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2346 | F0591 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2345完成承载值构造 | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2347 | F0591 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2346返回true | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2348 | F0591 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:340</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2347返回const引用 | 自动令牌范围承载值 | RCE2346返回true且RCE2347已取得令牌引用 | 当前源码宏定义116-123行、调用点340行与已发布F0387同宏先例复核 |
| RCE2349 | F0591 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:343</code> | source-audited-direct-call | 关系 | 关系句柄有效bool | 入口必达 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2350 | F0591 | F0184 | <code>海中鱼巣/核心/关系仓库.cpp:368, 海中鱼巣/核心/关系仓库.cpp:404</code> | source-audited-direct-call | false,对应内部不一致诊断说明 | 诊断bool丢弃 | 端点无效或写后材料不完整 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2351 | F0591 | F0592 | <code>海中鱼巣/核心/关系仓库.cpp:402</code> | source-audited-direct-call | this=&amp;结果 | 关系状态变更材料完整bool | 关系失效写入完成后 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2352 | F0592 | F0163 | <code>海中鱼巣/核心/关系仓库.h:118-119</code> | source-audited-direct-call | 源节点；目标节点 | 两个节点句柄有效bool | 前序状态及关系句柄校验通过，第二次受第一次短路 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2353 | F0619 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:423</code> | source-audited-direct-call | this=&amp;事务接线_ | 已接域bool | 入口必达 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2354 | F0619 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:424</code> | resolved_function_pointer+source_audited | 事务接线_.运行期状态 | 结构事务许可 | RCE2353返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2355 | F0619 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:425</code> | source-audited-direct-call | this=&amp;许可 | 许可有效bool | RCE2354返回许可 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2356 | F0619 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:425</code> | source-audited-direct-call | this=&amp;许可 | 结构事务令牌const引用 | RCE2355返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2357 | F0619 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:425</code> | implicit_destructor+source_audited | this=&amp;许可 | void | 接域分支返回或异常展开时局部许可离开作用域 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2358 | F0620 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0620进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2359 | F0620 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2358返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2360 | F0620 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2358返回true且RCE2359返回nullptr | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2361 | F0620 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2360返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2360返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2362 | F0620 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2361完成承载值构造 | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2363 | F0620 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2362返回true | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2364 | F0620 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:824</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2363返回const引用 | 自动令牌范围承载值 | RCE2362返回true且RCE2363已取得令牌引用 | 当前源码宏定义116-123行、调用点824行与已发布F0387同宏先例复核 |
| RCE2365 | F0620 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:832</code> | source-audited-direct-call | 记录.源节点,源节点 | 节点相等bool | 记录有效且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2366 | F0621 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | direct_const_member+macro_expanded+source_audited | this=&amp;事务接线_ | 事务接线是否已接域bool | F0621进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2367 | F0621 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | direct_free+macro_expanded+source_audited | 仓库=*this | 当前关系令牌指针 | RCE2366返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2368 | F0621 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | resolved_function_pointer+macro_expanded+source_audited | 状态=事务接线_.运行期状态 | 临时结构事务许可 | RCE2366返回true且RCE2367返回nullptr | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2369 | F0621 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | move-constructor+macro-expanded-move-constructor | 其它=RCE2368返回的临时结构事务许可 | 自动许可optional中的结构事务许可 | RCE2368返回临时许可并进入optional.emplace | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2370 | F0621 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 许可是否有效bool | RCE2369完成承载值构造 | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2371 | F0621 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | direct_const_member+macro_expanded+source_audited | this=&amp;自动许可.value() | 结构事务令牌const引用 | RCE2370返回true | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2372 | F0621 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:791</code> | direct_constructor+macro_expanded+source_audited | 仓库=*this,令牌=RCE2371返回const引用 | 自动令牌范围承载值 | RCE2370返回true且RCE2371已取得令牌引用 | 当前源码宏定义116-123行、调用点791行与已发布F0387同宏先例复核 |
| RCE2373 | F0621 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:804</code> | source-audited-direct-call | 记录.源节点,源节点 | 节点相等bool | 记录有效且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2374 | F0636 | F0174 | <code>海中鱼巣/领域/初始化.需求.ixx:76</code> | direct_const_member | this=&amp;世界树结果 | 世界树初始化成功bool | 函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2375 | F0636 | F0350 | <code>海中鱼巣/领域/初始化.需求.ixx:88, 海中鱼巣/领域/初始化.需求.ixx:99</code> | direct_const_member | this=&amp;结果.安全根需求 / this=&amp;结果.服务根需求 | 单根需求初始化成功bool | 对应RCE0300返回后 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2376 | F0636 | F0165 | <code>海中鱼巣/领域/初始化.需求.ixx:102</code> | direct_member | this=&amp;状态_,1 | 概念命名目标状态节点句柄 | 两个根需求均成功 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2377 | F0636 | F0329 | <code>海中鱼巣/领域/初始化.需求.ixx:103</code> | direct_const_member | this=&amp;状态_,结果.概念命名目标状态 | 状态值optional&lt;int64_t&gt; | RCE2376完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2378 | F0624 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:364</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | F0624函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2379 | F0624 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:365</code> | resolved_function_pointer | 事务接线_.运行期状态 | 结构事务许可 | RCE2378返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2380 | F0624 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:366</code> | direct_const_member | this=&amp;许可 | 许可有效bool | RCE2379完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2381 | F0624 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:366</code> | direct_const_member | this=&amp;许可 | 结构事务令牌const引用 | RCE2380返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2382 | F0624 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:364-367</code> | compiler_implicit_destructor | this=&amp;许可 | void | 接域分支离开许可作用域或异常展开 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2383 | F0627 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:376</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | F0627函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2384 | F0627 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:377</code> | resolved_function_pointer | 事务接线_.运行期状态 | 结构事务许可 | RCE2383返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2385 | F0627 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:378</code> | direct_const_member | this=&amp;许可 | 许可有效bool | RCE2384完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2386 | F0627 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:378</code> | direct_const_member | this=&amp;许可 | 结构事务令牌const引用 | RCE2385返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2387 | F0627 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:376-379</code> | compiler_implicit_destructor | this=&amp;许可 | void | 接域分支离开许可作用域或异常展开 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2388 | F0628 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:384</code> | direct_const_member | this=&amp;事务接线_ | 已接域bool | F0628函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2389 | F0628 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:385</code> | resolved_function_pointer | 事务接线_.运行期状态 | 结构事务许可 | RCE2388返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2390 | F0628 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:386</code> | direct_const_member | this=&amp;许可 | 许可有效bool | RCE2389完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2391 | F0628 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:386</code> | direct_const_member | this=&amp;许可 | 结构事务令牌const引用 | RCE2390返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2392 | F0628 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:384-387</code> | compiler_implicit_destructor | this=&amp;许可 | void | 接域分支离开许可作用域或异常展开 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2393 | F0633 | F0173 | <code>海中鱼巣/领域/初始化.语素.ixx:164</code> | direct_const_member | this=&amp;结果 | 语素初始化成功bool | 18项显示语素创建完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2394 | F0635 | R0733 | <code>海中鱼巣/领域/初始化.世界树.ixx:52</code> | direct_member | this=&amp;世界_ | 世界树根基础信息节点句柄 | F0635函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2395 | F0635 | R0734 | <code>海中鱼巣/领域/初始化.世界树.ixx:53</code> | direct_member | this=&amp;世界_ | 自我所在场景节点句柄 | RCE2394完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2396 | F0635 | R0735 | <code>海中鱼巣/领域/初始化.世界树.ixx:55</code> | direct_const_member | this=this,结果.世界树根节点,节点类型::基础信息 | 节点类型匹配bool | 三类根节点创建完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2397 | F0635 | F0167 | <code>海中鱼巣/领域/初始化.世界树.ixx:62, 海中鱼巣/领域/初始化.世界树.ixx:63</code> | direct_member | this=&amp;关系_,普通父子+对应源目标 | 两条普通父子关系句柄 | 三类根节点验证通过 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2398 | F0635 | R0736 | <code>海中鱼巣/领域/初始化.世界树.ixx:64</code> | direct_member | this=&amp;世界_,结果.自我所在场景,结果.自我存在节点 | 场景接纳自我关系句柄 | RCE2397两次调用完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2399 | F0635 | F0579 | <code>海中鱼巣/领域/初始化.世界树.ixx:78, 海中鱼巣/领域/初始化.世界树.ixx:79, 海中鱼巣/领域/初始化.世界树.ixx:80</code> | direct_const_member | this=&amp;关系_,普通父子/归属+对应源目标 | 关系存在bool | 坐标读回存在且三轴均为零后按短路顺序求值 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2400 | R0241 | R0733 | <code>海中鱼巣/领域/初始化.语素.ixx:177</code> | direct_member | this=&amp;世界_ | 基础信息节点句柄 | 文本准入为可作为最小词单元 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2401 | R0733 | F0331 | <code>海中鱼巣/领域/世界服务.h:18</code> | direct_member | this=&amp;主信息_ | 主信息句柄 | R0733函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2402 | R0733 | F0332 | <code>海中鱼巣/领域/世界服务.h:19</code> | direct_member | this=&amp;节点_,节点类型::基础信息,主信息句柄 | 基础信息节点句柄 | RCE2401完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2403 | R0733 | F0190 | <code>海中鱼巣/领域/世界服务.h:20</code> | direct_const_member | this=&amp;节点_,基础信息 | 节点记录optional | RCE2402完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2404 | R0733 | F0624 | <code>海中鱼巣/领域/世界服务.h:22</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息 | 主信息有效bool | RCE2403有值且节点类型为基础信息 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2405 | R0734 | R0737 | <code>海中鱼巣/领域/世界服务.h:33</code> | direct_member | this=&amp;场景_ | 场景节点句柄 | R0734函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2406 | R0735 | F0190 | <code>海中鱼巣/领域/初始化.世界树.ixx:122</code> | direct_const_member | this=&amp;节点_,节点值 | 节点记录optional | R0735函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2407 | R0735 | F0624 | <code>海中鱼巣/领域/初始化.世界树.ixx:123</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息 | 主信息有效bool | 记录存在且类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2408 | R0736 | R0738 | <code>海中鱼巣/领域/世界服务.h:37</code> | direct_member | this=&amp;场景_,场景,存在 | 归属关系句柄 | R0736函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2409 | R0737 | F0331 | <code>海中鱼巣/领域/场景服务.h:17</code> | direct_member | this=&amp;主信息_ | 主信息句柄 | R0737函数进入 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2410 | R0737 | F0332 | <code>海中鱼巣/领域/场景服务.h:18</code> | direct_member | this=&amp;节点_,节点类型::场景,主信息句柄 | 场景节点句柄 | RCE2409完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2411 | R0737 | F0190 | <code>海中鱼巣/领域/场景服务.h:19</code> | direct_const_member | this=&amp;节点_,场景 | 节点记录optional | RCE2410完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2412 | R0737 | F0624 | <code>海中鱼巣/领域/场景服务.h:21</code> | direct_const_member | this=&amp;主信息_,记录-&gt;主信息 | 主信息有效bool | RCE2411有值且节点类型为场景 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2413 | R0738 | R0276 | <code>海中鱼巣/领域/场景服务.h:28</code> | direct_const_member | this=this,场景/存在,节点类型::场景/存在 | 节点类型匹配bool | 按短路顺序验证两个节点 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2414 | R0738 | F0167 | <code>海中鱼巣/领域/场景服务.h:31</code> | direct_member | this=&amp;关系_,关系类型::归属,场景,存在 | 归属关系句柄 | 两个节点类型匹配 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2415 | R0738 | F0168 | <code>海中鱼巣/领域/场景服务.h:32</code> | direct_free+overload_resolved | 关系 | 关系句柄有效bool | RCE2414完成 | 本批当前源码逐调用点与正式身份表复核 |
| RCE2416 | R0738 | F0579 | <code>海中鱼巣/领域/场景服务.h:32</code> | direct_const_member | this=&amp;关系_,关系类型::归属,场景,存在 | 关系存在bool | RCE2415返回true | 本批当前源码逐调用点与正式身份表复核 |
| RCE2417 | R0002 | F0399 | <code>海中鱼巣/核心/主信息仓库.cpp:12</code> | resolved-function-pointer | 接线.运行期状态, 令牌 | 共享路径令牌有效 bool | RCE0302 返回 true；&amp;&amp; 短路第二项 | 本批源码接线字段与当前装配目标复核 |
| RCE2418 | R0005 | F0400 | <code>海中鱼巣/核心/主信息仓库.cpp:16</code> | resolved-function-pointer | 接线.运行期状态, 令牌 | 独占令牌有效 bool | RCE0309 返回 true；&amp;&amp; 短路第二项 | 本批源码接线字段与当前装配目标复核 |
| RCE2419 | R0021 | R0739 | <code>海中鱼巣/核心/会话.结构写入.ixx:239</code> | direct-const-member | this=&amp;结果 | 带值结构写入结果成功 bool | 已复制结果.操作 | 本批源码逐调用点与静态类型复核 |
| RCE2420 | R0021 | R0015 | <code>海中鱼巣/核心/会话.结构写入.ixx:253</code> | direct-const-member | this=&amp;候选 | 主信息未发布候选完整 bool | emplace_back 抛出异常；catch 路径 | 本批源码逐调用点与静态类型复核 |
| RCE2421 | R0021 | R0006 | <code>海中鱼巣/核心/会话.结构写入.ixx:253</code> | direct-member | this=&amp;主信息_, 候选, 令牌_ | 结构写入结果；显式丢弃 | catch 路径且 RCE2420 返回 true | 本批源码逐调用点与静态类型复核 |
| RCE2422 | R0022 | R0740 | <code>海中鱼巣/核心/会话.结构写入.ixx:267</code> | direct-const-member | this=&amp;结果 | 带值结构写入结果成功 bool | 已复制结果.操作 | 本批源码逐调用点与静态类型复核 |
| RCE2423 | R0022 | R0148 | <code>海中鱼巣/核心/会话.结构写入.ixx:277</code> | direct-const-member | this=&amp;候选 | 节点句柄 → 局部节点 | 结果.值.has_value() == true | 本批源码逐调用点与静态类型复核 |
| RCE2424 | R0022 | R0741 | <code>海中鱼巣/核心/会话.结构写入.ixx:281</code> | direct-const-member | this=&amp;候选 | 节点未发布候选完整 bool | emplace_back 抛出异常；catch 路径 | 本批源码逐调用点与静态类型复核 |
| RCE2425 | R0022 | R0144 | <code>海中鱼巣/核心/会话.结构写入.ixx:281</code> | direct-member | this=&amp;节点_, 候选, 令牌_ | 结构写入结果；显式丢弃 | catch 路径且 RCE2424 返回 true | 本批源码逐调用点与静态类型复核 |
| RCE2426 | R0023 | R0642 | <code>海中鱼巣/核心/会话.结构写入.ixx:303</code> | direct-const-member | this=&amp;输出 | 带值结构写入结果成功 bool | 结构化创建关系结果已移动到输出 | 本批源码逐调用点与静态类型复核 |
| RCE2427 | R0023 | R0742 | <code>海中鱼巣/核心/会话.结构写入.ixx:307</code> | direct-const-member | this=&amp;输出.操作 | 结构是否改变 bool | 输出成功 | 本批源码逐调用点与静态类型复核 |
| RCE2428 | R0024 | R0743 | <code>海中鱼巣/核心/会话.结构写入.ixx:382</code> | direct-const-member | this=&amp;结果 | 结构化节点挂载结果成功 bool | 已复制挂载结果 | 本批源码逐调用点与静态类型复核 |
| RCE2429 | R0024 | R0742 | <code>海中鱼巣/核心/会话.结构写入.ixx:386</code> | direct-const-member | this=&amp;结果.操作 | 结构是否改变 bool | 结构化挂载结果成功 | 本批源码逐调用点与静态类型复核 |
| RCE2430 | R0739 | R0138 | <code>海中鱼巣/核心/结果.结构写入.h:44</code> | direct-const-member | this=&amp;操作 | 结构写入结果成功 bool | R0739 进入；&amp;&amp; 短路第一项 | 本批源码逐调用点与静态类型复核 |
| RCE2431 | R0740 | R0138 | <code>海中鱼巣/核心/结果.结构写入.h:44</code> | direct-const-member | this=&amp;操作 | 结构写入结果成功 bool | R0740 进入；&amp;&amp; 短路第一项 | 本批源码逐调用点与静态类型复核 |
| RCE2432 | R0741 | F0163 | <code>海中鱼巣/核心/节点仓库.h:40</code> | direct-free-overload | 节点_（const 节点句柄&amp;） | 节点句柄形态有效 bool | 仓库_ != nullptr；&amp;&amp; 短路第二项 | 本批源码逐调用点与静态类型复核 |
| RCE2433 | R0743 | R0138 | <code>海中鱼巣/核心/关系仓库.h:176</code> | direct-const-member | this=&amp;操作 | 结构写入结果成功 bool | R0743 进入；&amp;&amp; 短路第一项 | 本批源码逐调用点与静态类型复核 |
| RCE2434 | R0743 | R0744 | <code>海中鱼巣/核心/关系仓库.h:176</code> | direct-const-member | this=&amp;挂载 | 挂载关系已确定 bool | RCE2433 返回 true；&amp;&amp; 短路第二项 | 本批源码逐调用点与静态类型复核 |
| RCE2435 | R0744 | F0163 | <code>海中鱼巣/核心/关系仓库.h:156</code> | direct-free-overload | 节点；新父节点（均为 const 节点句柄&amp;） | 两个节点句柄形态有效 bool | R0744 进入；前两项 &amp;&amp; 短路；同一源码行两次调用 | 本批源码逐调用点与静态类型复核 |
| RCE2436 | R0744 | F0051 | <code>海中鱼巣/核心/关系仓库.h:156, 海中鱼巣/核心/关系仓库.h:163</code> | direct-operator | 节点, 新父节点；旧父节点.value(), 新父节点 | 节点句柄相等 bool | 句柄有效检查通过；已重挂分支分别比较 | 本批源码逐调用点与静态类型复核 |
| RCE2437 | R0022 | R0745 | <code>海中鱼巣/核心/会话.结构写入.ixx:265</code> | direct-member | this=&amp;节点_, 类型, 主信息, 令牌_ | 带值结构写入结果&lt;节点未发布候选&gt; → 局部结果 | RCE0335 可继续写入返回 true | 本批源码逐调用点与静态类型复核 |
| RCE2438 | R0745 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:88</code> | direct-free | 事务接线_, 令牌 | 独占令牌有效 bool | R0745 进入 | 本批源码逐调用点与静态类型复核 |
| RCE2439 | R0745 | F0565 | <code>海中鱼巣/核心/节点仓库.cpp:92</code> | direct-free-overload | 主信息（const 主信息句柄&amp;） | 主信息句柄形态有效 bool | 独占令牌有效 | 本批源码逐调用点与静态类型复核 |
| RCE2440 | R0745 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:93</code> | direct-free | 类型 | 节点类型已定义 bool | 主信息句柄有效 | 本批源码逐调用点与静态类型复核 |
| RCE2441 | R0745 | R0009 | <code>海中鱼巣/核心/节点仓库.cpp:94</code> | direct-const-member | this=&amp;主信息_, 主信息, 令牌 | 主信息有效 bool | 主信息句柄有效且节点类型已定义 | 本批源码逐调用点与静态类型复核 |
| RCE2442 | R0745 | R0746 | <code>海中鱼巣/核心/节点仓库.cpp:98</code> | direct-member | this=&amp;节点仓库, 类型, 主信息, 令牌 | std::optional&lt;节点未发布候选&gt; → 局部候选 | 入口材料检查全部通过 | 本批源码逐调用点与静态类型复核 |
| RCE2443 | R0745 | R0148 | <code>海中鱼巣/核心/节点仓库.cpp:103</code> | direct-const-member | this=&amp;*候选 | 节点句柄 → 局部节点 | 候选.has_value() == true | 本批源码逐调用点与静态类型复核 |
| RCE2444 | R0746 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:113</code> | direct-free | 事务接线_, 令牌 | 共享令牌有效 bool | R0746 进入 | 本批源码逐调用点与静态类型复核 |
| RCE2445 | R0746 | R0009 | <code>海中鱼巣/核心/节点仓库.cpp:114</code> | direct-const-member | this=&amp;主信息_, 主信息, 令牌 | 主信息有效 bool | 共享令牌有效 | 本批源码逐调用点与静态类型复核 |
| RCE2446 | R0746 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:115</code> | direct-free | 类型 | 节点类型已定义 bool | 共享令牌与主信息有效 | 本批源码逐调用点与静态类型复核 |
| RCE2447 | R0026 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:426</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首个短路项；false返回空组 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2448 | R0027 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:454</code> | direct-const-member | this=&amp;结果 | 成功bool | 结构化绑定返回后；false记录失败并返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2449 | R0027 | R0742 | <code>海中鱼巣/核心/会话.结构写入.ixx:458</code> | direct-const-member | this=&amp;结果 | 改变结构bool | 结果成功后；false按幂等无变化返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2450 | R0028 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:474</code> | direct-const-member | this=结构写入会话* | 会话可继续bool | 入口首个短路项；false返回入口拒绝结果 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2451 | R0028 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:478</code> | direct-const-member | this=&amp;记录.候选 | 主信息句柄 | 候选完整后读取并参与目标句柄比较 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2452 | R0028 | R0615 | <code>海中鱼巣/核心/会话.结构写入.ixx:478, 海中鱼巣/核心/会话.结构写入.ixx:486</code> | direct-free-operator | 左=候选主信息句柄, 右=输入主信息句柄 | 句柄相等bool | 两个范围循环中的候选主信息比较 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2453 | R0029 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:510</code> | direct-const-member | this=结构写入会话* | 会话可继续bool | 入口首个短路项；false返回false | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2454 | R0029 | R0615 | <code>海中鱼巣/核心/会话.结构写入.ixx:512</code> | direct-free-operator | 左=记录.主信息, 右=输入主信息 | 句柄相等bool | 候选I64写集每轮首个不匹配短路项 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2455 | R0030 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:525</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首个短路项；false返回nullopt | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2456 | R0030 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:526</code> | direct-const-member | this=&amp;节点_, 节点=输入节点, 令牌=令牌_ | optional&lt;节点记录&gt; | 可继续且节点句柄有效后读取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2457 | R0031 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:531</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首个短路项；false返回nullopt | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2458 | R0031 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:532</code> | direct-const-member | this=&amp;节点_, 节点=输入节点, 令牌=令牌_ | optional&lt;节点记录&gt; | 可继续且节点句柄有效后读取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2459 | R0032 | F0383 | <code>海中鱼巣/核心/会话.结构写入.ixx:538</code> | direct-const-member | this=&amp;主信息_, 主信息=输入主信息, 值索引=槽位, 令牌=令牌_ | optional&lt;int64_t&gt; | 入口门禁通过后直接返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2460 | R0033 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:542</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首个短路项；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2461 | R0034 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:547</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首先求值；false短路并返回空组 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2462 | R0035 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:554</code> | direct-const-member | this=const结构写入会话* | 会话可继续bool | 入口首先求值；false短路并返回空组 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2463 | R0036 | F0439 | <code>海中鱼巣/核心/会话.结构写入.ixx:568</code> | direct-const-member | this=&amp;主信息_, 主信息=输入主信息, 令牌=令牌_ | optional&lt;主信息记录&gt; | 当前线程可访问后读取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2464 | R0036 | R0615 | <code>海中鱼巣/核心/会话.结构写入.ixx:573</code> | direct-free-operator | 左=候选读取主信息结果, 右=输入主信息 | 句柄相等bool | 仓库读回成功后遍历主信息候选 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2465 | R0037 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:579</code> | direct-const-member | this=&amp;节点_, 节点=输入节点, 令牌=令牌_ | optional&lt;节点记录&gt; | 当前线程可访问后读取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2466 | R0037 | R0148 | <code>海中鱼巣/核心/会话.结构写入.ixx:584</code> | direct-const-member | this=&amp;记录.候选 | 节点句柄 | 节点仓库读回成功后遍历节点候选 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2467 | R0037 | F0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:584</code> | direct-free-operator | 左=读取节点结果, 右=输入节点 | 句柄相等bool | 每轮读取候选节点后比较 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2468 | R0038 | R0598 | <code>海中鱼巣/核心/会话.结构写入.ixx:595</code> | direct-free-operator | 左=记录.关系, 右=输入关系 | 句柄相等bool | 关系读回成功后遍历关系写集 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2473 | R0047 | R0015 | <code>海中鱼巣/核心/会话.结构写入.ixx:752</code> | direct-const-member | this=&amp;记录.候选 | 候选完整bool | 主信息候选循环每轮匹配短路第一项 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2474 | R0047 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:752</code> | direct-const-member | this=&amp;记录.候选 | 主信息句柄 | RCE2473为true后求值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2475 | R0047 | R0615 | <code>海中鱼巣/核心/会话.结构写入.ixx:752</code> | direct-free-operator | 左=RCE2474结果, 右=输入主信息 | 句柄相等bool | RCE2473为true；直接决定匹配 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2476 | R0048 | R0741 | <code>海中鱼巣/核心/会话.结构写入.ixx:760</code> | direct-const-member | this=&amp;记录.候选 | 候选完整bool | 节点候选循环每轮匹配短路第一项 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2477 | R0048 | R0148 | <code>海中鱼巣/核心/会话.结构写入.ixx:760</code> | direct-const-member | this=&amp;记录.候选 | 节点句柄 | RCE2476为true后求值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2478 | R0048 | F0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:760</code> | direct-free-operator | 左=RCE2477结果, 右=输入节点 | 句柄相等bool | RCE2476为true；直接决定匹配 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2479 | R0049 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:767</code> | direct-const-member | this=&amp;节点_, 节点=输入节点, 令牌=令牌_ | optional&lt;节点记录&gt; | R0048返回true后执行 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2480 | R0050 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:773</code> | direct-const-member | this=&amp;节点_, 节点=输入节点, 令牌=令牌_ | optional&lt;节点记录&gt; | R0048返回true后执行 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2481 | R0051 | F0383 | <code>海中鱼巣/核心/会话.结构写入.ixx:781</code> | direct-const-member | this=&amp;主信息_, 主信息=输入主信息, 值索引=槽位, 令牌=令牌_ | optional&lt;int64_t&gt; | R0047返回true后直接返回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2482 | R0053 | R0748 | <code>海中鱼巣/核心/会话.结构写入.ixx:807</code> | direct-static-member | 左=RCE0399返回的关系记录临时值, 右=当前记录const借用 | bool | 关系变更写集每轮；true时写记录.已读回=true | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2483 | R0748 | F0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:791, 海中鱼巣/核心/会话.结构写入.ixx:792</code> | direct-free-operator | 依次比较左/右源节点与目标节点 | bool | 七字段短路合取按前序相等结果求值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2484 | R0054 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:825</code> | direct-const-member | this=&amp;撤销 | bool | 仅emplace_back抛异常并调用R0072后，选择失败材料当前版本 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2485 | R0063 | F0346 | <code>海中鱼巣/核心/会话.结构写入.ixx:107</code> | direct-const-member | this=&amp;节点_, 节点=绑定-&gt;节点, 令牌=令牌_ | optional&lt;节点记录&gt;，绑定节点记录值 | RCE0419成功、主键非零且RCE0420返回绑定 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2486 | R0065 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1034</code> | implicit-destructor | this=&amp;令牌范围 | void | R0089通过且F0340构造成功后的正常返回或异常展开 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2487 | R0073 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1435</code> | direct-constructor | 仓库=*this, 令牌=令牌 | 关系令牌范围 | R0089为true后构造令牌范围 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2488 | R0073 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1435-1437</code> | implicit-destructor | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2489 | R0074 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1439</code> | direct-const-member | 接线=事务接线_, 令牌=令牌 | bool | 入口必经；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2490 | R0074 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1439</code> | direct-constructor | 仓库=*this, 令牌=令牌 | 关系令牌范围 | R0089为true后构造令牌范围 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2491 | R0074 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1439-1441</code> | implicit-destructor | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2492 | R0075 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1447</code> | direct-const-member | 接线=事务接线_, 令牌=令牌 | bool | 入口必经；false返回false | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2493 | R0075 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1447</code> | direct-constructor | 仓库=*this, 令牌=令牌 | 关系令牌范围 | R0089为true后构造令牌范围 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2494 | R0075 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1447-1449</code> | implicit-destructor | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2495 | R0076 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:1495, 海中鱼巣/核心/关系仓库.cpp:1496</code> | direct-free-operator | 依次比较锁内记录与快照的源节点、目标节点 | bool并按!=取反 | 锁内记录字段前序一致后短路求值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2496 | R0077 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1543</code> | direct-const-member | 接线=事务接线_, 令牌=令牌 | bool | 入口必经；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2497 | R0077 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1543</code> | direct-constructor | 仓库=*this, 令牌=令牌 | 关系令牌范围 | R0089为true后构造令牌范围 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2498 | R0077 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1543-1545</code> | implicit-destructor | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2499 | R0078 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1547</code> | direct-const-member | 接线=事务接线_, 令牌=令牌 | bool | 入口必经；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2500 | R0078 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1547</code> | direct-constructor | 仓库=*this, 令牌=令牌 | 关系令牌范围 | R0089为true后构造令牌范围 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2501 | R0078 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1547-1549</code> | implicit-destructor | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2502 | R0079 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1559</code> | direct-free(macro-expanded) | 接线=事务接线_, 令牌=令牌 | bool | 进入函数即执行关系令牌入口；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2503 | R0079 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1559</code> | direct-constructor(macro-expanded) | 仓库=*this, 令牌=令牌 | 局部关系令牌范围 | RCE2502=true | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2504 | R0079 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1559-1561</code> | implicit-destructor/lifecycle | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2505 | R0082 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1644</code> | direct-free(macro-expanded) | 接线=事务接线_, 令牌=令牌 | bool | 进入函数即执行关系令牌入口；false返回false | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2506 | R0082 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1644</code> | direct-constructor(macro-expanded) | 仓库=*this, 令牌=令牌 | 局部关系令牌范围 | RCE2505=true | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2507 | R0082 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1644-1646</code> | implicit-destructor/lifecycle | this=&amp;令牌范围 | void | F0340构造成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2508 | R0088 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-const-member(macro-expanded) | this=&amp;事务接线_ | bool | 进入共享许可宏；false短路 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2509 | R0088 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-free(macro-expanded) | 仓库=*this | const 结构事务令牌* | F0336=true；非空时沿已有令牌语境 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2510 | R0088 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | std-function-target/lambda-call(macro-expanded) | 状态=事务接线_.运行期状态 | 结构事务许可临时值 | F0336=true且F0337=nullptr | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2511 | R0088 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-move-constructor(macro-expanded) | 其它=RCE2510结果 | optional内结构事务许可 | RCE2510返回后emplace | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2512 | R0088 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-const-member(macro-expanded) | this=&amp;自动许可.value() | bool | 自动许可已持值；false返回空vector | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2513 | R0088 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-const-member(macro-expanded) | this=&amp;自动许可.value() | const 结构事务令牌&amp; | RCE2512=true | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2514 | R0088 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:866</code> | direct-constructor-via-optional-emplace(macro-expanded) | 仓库=*this, 令牌=RCE2513引用 | optional内关系令牌范围 | RCE2512=true且RCE2513完成 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2515 | R0088 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:866-896</code> | implicit-destructor/lifecycle | this=&amp;自动令牌范围.value() | void | RCE2514成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2516 | R0088 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:866-896</code> | implicit-destructor/lifecycle | this=&amp;自动许可.value() | void | RCE2511成功后的所有退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2517 | R0101 | R0599 | <code>海中鱼巣/核心/协调.结构事务.ixx:87, 海中鱼巣/核心/协调.结构事务.ixx:96, 海中鱼巣/核心/协调.结构事务.ixx:99, 海中鱼巣/核心/协调.结构事务.ixx:104, 海中鱼巣/核心/协调.结构事务.ixx:116, 海中鱼巣/核心/协调.结构事务.ixx:129</code> | constructor | 无 | 空结构事务许可返回对象 | 入口无效、域隔离、同线程已有许可、登记失败、锁异常或锁后隔离 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2518 | R0102 | R0042 | <code>海中鱼巣/核心/执行器.结构写入.ixx:122</code> | constructor | 主信息=*主信息_, 节点=*节点_, 关系=*关系_, 索引=*索引_, 令牌=许可.读取令牌() | 局部结构写入会话 | 许可有效 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2519 | R0102 | R0138 | <code>海中鱼巣/核心/执行器.结构写入.ixx:154, 海中鱼巣/核心/执行器.结构写入.ixx:163, 海中鱼巣/核心/执行器.结构写入.ixx:172</code> | direct-const-member | this=&amp;准备结果 / &amp;会话确认 / &amp;参与者确认 | bool并取反 | 各准备或确认结果已形成 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2520 | R0102 | F0345 | <code>海中鱼巣/核心/执行器.结构写入.ixx:119-183</code> | compiler_implicit_destructor | this=&amp;许可 | void | 许可已构造后的所有正常、早退或异常退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2521 | R0102 | R0749 | <code>海中鱼巣/核心/执行器.结构写入.ixx:122-183</code> | compiler_implicit_destructor | this=&amp;会话 | void | 会话成功构造后的所有退出，逆构造序先于许可析构 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2522 | R0102 | R0056 | <code>海中鱼巣/核心/执行器.结构写入.ixx:140</code> | direct-const-member | this=&amp;会话 | bool并取反进入未提交撤销分支 | 回调正常且会话无失败 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2523 | R0109 | R0138 | <code>海中鱼巣/核心/执行器.结构写入.ixx:391</code> | direct-const-member | this=&amp;撤销结果 | bool用于与累计结果短路合取 | 逆序撤销循环取得当前参与者撤销结果后 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2524 | R0116 | R0749 | <code>海中鱼巣/核心/执行器.结构写入.ixx:80-111</code> | implicit-destructor | this=&amp;会话 | 无 | 局部会话成功构造后任一正常、早退或异常退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2525 | R0116 | F0345 | <code>海中鱼巣/核心/执行器.结构写入.ixx:76-111</code> | implicit-destructor | this=&amp;许可 | 无 | 独占许可成功构造后任一正常、早退或异常退出 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2526 | R0119 | R0127 | <code>海中鱼巣/核心/索引仓库.cpp:145</code> | operator | this=&amp;现有-&gt;second; other=&amp;请求记录 | bool用于不相等分支 | 命中现有主键后比较完整绑定记录 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2527 | R0119 | F0051 | <code>海中鱼巣/核心/索引仓库.cpp:148</code> | operator | 左=现有-&gt;second.节点; 右=请求.目标 | bool用于版本漂移与入口拒绝二分 | 现有绑定记录与请求记录不相等且节点身份字段已比较 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2528 | R0750 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:289</code> | direct-const-member | this=&amp;事务接线_ | bool决定接域路径 | 删除主键无令牌入口首先判断仓库是否接域 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2529 | R0750 | F0397 | <code>海中鱼巣/核心/索引仓库.cpp:290</code> | direct-member | this=&amp;事务接线_; 运行期状态=事务接线_.运行期状态 | 结构事务许可绑定到局部许可 | 仓库已接域 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2530 | R0750 | F0338 | <code>海中鱼巣/核心/索引仓库.cpp:291</code> | direct-const-member | this=&amp;许可 | bool进入短路合取 | 共享许可已经取得 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2531 | R0750 | F0339 | <code>海中鱼巣/核心/索引仓库.cpp:291</code> | direct-const-member | this=&amp;许可 | const 结构事务令牌&amp;传入删除重载 | 许可有效且需调用带令牌重载 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2532 | R0750 | R0123 | <code>海中鱼巣/核心/索引仓库.cpp:291</code> | direct-member | this=this; 主键=主键; 令牌=许可.读取令牌() | bool直接作为接域路径返回值 | 共享许可有效 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2533 | R0750 | F0345 | <code>海中鱼巣/核心/索引仓库.cpp:289-292</code> | implicit-destructor | this=&amp;许可 | 无 | 接域路径局部许可构造后退出作用域 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2534 | R0750 | R0126 | <code>海中鱼巣/核心/索引仓库.cpp:295</code> | direct-free | 主键索引_=主键索引_; 节点主键组_=节点主键组_; 主键=主键 | bool直接作为无接域路径返回值 | 仓库未接域、主键非零并取得独占锁 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE2535 | R0145 | F0400 | <code>海中鱼巣/核心/节点仓库.cpp:28</code> | resolved-function-pointer | 接线.运行期状态, 令牌 | 独占令牌有效 bool | RCE0616 返回 true；&amp;&amp; 短路第二项 | 本批源码接线字段、生产结构事务协调器唯一目标与现状单函数图复核 |
| RCE2536 | R0203 | F0516 | <code>海中鱼巣/领域/世界服务.h:29</code> | direct_member | this=&amp;存在_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2537 | R0204 | R0751 | <code>海中鱼巣/领域/二次特征服务.h:108</code> | direct_const_member | this, 二次特征, 节点类型::二次特征 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2538 | R0751 | F0190 | <code>海中鱼巣/领域/二次特征服务.h:178</code> | direct_const_member | this=&amp;节点_, 节点句柄值 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2539 | R0205 | F0190 | <code>海中鱼巣/领域/二次特征服务.h:183</code> | direct_const_member | this=&amp;节点_, 组成项 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2540 | R0206 | F0190 | <code>海中鱼巣/领域/二次特征服务.h:41</code> | direct_const_member | this=&amp;节点_, 二次特征 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2541 | R0223 | F0190 | <code>海中鱼巣/领域/初始化.世界树.ixx:112</code> | direct_const_member | this=&amp;节点_, 世界树 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2542 | R0223 | F0628 | <code>海中鱼巣/领域/初始化.世界树.ixx:116, 海中鱼巣/领域/初始化.世界树.ixx:117, 海中鱼巣/领域/初始化.世界树.ixx:118</code> | direct_free+overload_resolved | 主信息句柄/关系句柄/节点句柄 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2543 | R0224 | F0190 | <code>海中鱼巣/领域/初始化.世界树.ixx:127</code> | direct_const_member | this=&amp;节点_, 当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2544 | R0232 | R0752 | <code>海中鱼巣/领域/初始化.系统角色.ixx:290</code> | direct_const_member | this=&amp;基础信息服务_, 参数.世界根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2545 | R0232 | R0753 | <code>海中鱼巣/领域/初始化.系统角色.ixx:301</code> | direct_const_member | this=&amp;动态服务_, 参数.概念动态根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2546 | R0232 | R0754 | <code>海中鱼巣/领域/初始化.系统角色.ixx:317</code> | direct_const_member | this=&amp;二次特征服务_, 参数.概念关系根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2547 | R0232 | R0711 | <code>海中鱼巣/领域/初始化.系统角色.ixx:328</code> | direct_const_member | this=&amp;概念图结构服务_, 根角色请求 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2548 | R0233 | R0755 | <code>海中鱼巣/领域/初始化.系统角色.ixx:363</code> | direct_const_member | this=&amp;特征服务_, 定义主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2549 | R0233 | R0756 | <code>海中鱼巣/领域/初始化.系统角色.ixx:369</code> | direct_const_member | this=&amp;特征服务_, 槽位-&gt;节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2550 | R0233 | R0757 | <code>海中鱼巣/领域/初始化.系统角色.ixx:376</code> | direct_const_member | this=&amp;特征服务_, 槽位-&gt;节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2551 | R0233 | R0758 | <code>海中鱼巣/领域/初始化.系统角色.ixx:384</code> | direct_const_member | this=&amp;状态服务_, 目标-&gt;稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2552 | R0233 | R0759 | <code>海中鱼巣/领域/初始化.系统角色.ixx:392</code> | direct_const_member | this=&amp;需求服务_, 需求-&gt;稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2553 | R0235 | R0758 | <code>海中鱼巣/领域/初始化.系统角色.ixx:429</code> | direct_const_member | this=&amp;状态服务_, 占用-&gt;稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2554 | R0236 | R0752 | <code>海中鱼巣/领域/初始化.系统角色.ixx:438</code> | direct_const_member | this=&amp;基础信息服务_, 参数.世界根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2555 | R0236 | R0753 | <code>海中鱼巣/领域/初始化.系统角色.ixx:444</code> | direct_const_member | this=&amp;动态服务_, 参数.概念动态根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2556 | R0236 | R0754 | <code>海中鱼巣/领域/初始化.系统角色.ixx:446</code> | direct_const_member | this=&amp;二次特征服务_, 参数.概念关系根主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2557 | R0236 | R0711 | <code>海中鱼巣/领域/初始化.系统角色.ixx:456</code> | direct_const_member | this=&amp;概念图结构服务_, 根角色请求 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2558 | R0237 | R0755 | <code>海中鱼巣/领域/初始化.系统角色.ixx:487</code> | direct_const_member | this=&amp;特征服务_, 定义主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2559 | R0237 | R0756 | <code>海中鱼巣/领域/初始化.系统角色.ixx:488</code> | direct_const_member | this=&amp;特征服务_, 组.实例槽位.节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2560 | R0237 | R0757 | <code>海中鱼巣/领域/初始化.系统角色.ixx:489</code> | direct_const_member | this=&amp;特征服务_, 组.实例槽位.节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2561 | R0237 | R0758 | <code>海中鱼巣/领域/初始化.系统角色.ixx:490</code> | direct_const_member | this=&amp;状态服务_, 组.目标状态.稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2562 | R0237 | R0759 | <code>海中鱼巣/领域/初始化.系统角色.ixx:491</code> | direct_const_member | this=&amp;需求服务_, 组.根需求.稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2563 | R0238 | R0758 | <code>海中鱼巣/领域/初始化.系统角色.ixx:517</code> | direct_const_member | this=&amp;状态服务_, 身份.稳定主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2564 | R0241 | R0575 | <code>海中鱼巣/领域/初始化.语素.ixx:174</code> | direct_const_member | this=&amp;语素_, 文本 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2565 | R0241 | F0220 | <code>海中鱼巣/领域/初始化.语素.ixx:184</code> | direct_const_member | this=&amp;项 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2566 | R0242 | F0190 | <code>海中鱼巣/领域/初始化.语素.ixx:192</code> | direct_const_member | this=&amp;节点_, 节点值 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2567 | R0243 | R0760 | <code>海中鱼巣/领域/初始化.语素.ixx:201, 海中鱼巣/领域/初始化.语素.ixx:202</code> | direct_const_member | this, 节点组, 目标 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2568 | R0244 | F0190 | <code>海中鱼巣/领域/初始化.语素.ixx:215</code> | direct_const_member | this=&amp;节点_, 节点值 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2569 | R0249 | R0762 | <code>海中鱼巣/领域/初始化.需求.ixx:137</code> | direct_member | this=&amp;特征_, 自我存在, 结果.特征定义 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2570 | R0249 | R0763 | <code>海中鱼巣/领域/初始化.需求.ixx:141</code> | direct_member | this=&amp;特征_, 结果.实例特征槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2571 | R0249 | F0363 | <code>海中鱼巣/领域/初始化.需求.ixx:130</code> | direct_const_member | this=&amp;结果.语素入口结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2572 | R0249 | F0329 | <code>海中鱼巣/领域/初始化.需求.ixx:163</code> | direct_const_member | this=&amp;状态_, 结果.目标状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2573 | R0249 | F0350 | <code>海中鱼巣/领域/初始化.需求.ixx:167</code> | direct_const_member | this=&amp;结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2574 | R0232 | R0769 | <code>海中鱼巣/领域/初始化.系统角色.ixx:291</code> | direct_const_member | this=&amp;材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2575 | R0236 | R0769 | <code>海中鱼巣/领域/初始化.系统角色.ixx:439</code> | direct_const_member | this=&amp;世界根 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2576 | R0752 | R0764 | <code>海中鱼巣/领域/服务.基础信息.ixx:34</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2577 | R0753 | R0662 | <code>海中鱼巣/领域/服务.动态.ixx:68</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2578 | R0754 | R0704 | <code>海中鱼巣/领域/服务.二次特征.ixx:47</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2579 | R0755 | R0765 | <code>海中鱼巣/领域/服务.特征.ixx:239</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2580 | R0756 | F0163 | <code>海中鱼巣/领域/服务.特征.ixx:243</code> | direct_free+overload_resolved | 实例槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2581 | R0756 | R0766 | <code>海中鱼巣/领域/服务.特征.ixx:244</code> | direct_const_member | this=&amp;数据操作_, 实例槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2582 | R0757 | F0163 | <code>海中鱼巣/领域/服务.特征.ixx:255</code> | direct_free+overload_resolved | 实例槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2583 | R0757 | R0386 | <code>海中鱼巣/领域/服务.特征.ixx:256</code> | direct_const_member | this=&amp;数据操作_, 实例槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2584 | R0758 | R0767 | <code>海中鱼巣/领域/服务.状态.ixx:150</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2585 | R0759 | R0429 | <code>海中鱼巣/领域/服务.需求.ixx:223</code> | direct_const_member | this=&amp;数据操作_, 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2586 | R0760 | F0051 | <code>海中鱼巣/领域/初始化.语素.ixx:207</code> | direct_operator | 节点值, 目标 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2587 | R0761 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:294</code> | direct_const_member | this=&amp;事务接线_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2588 | R0761 | F0398 | <code>海中鱼巣/核心/主信息仓库.cpp:295</code> | resolved_function_pointer | 事务接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2589 | R0761 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:296</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2590 | R0761 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:296</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2591 | R0761 | R0008 | <code>海中鱼巣/核心/主信息仓库.cpp:296</code> | direct_member | this, 主信息, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2592 | R0761 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:297, 海中鱼巣/核心/主信息仓库.cpp:310</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2593 | R0762 | R0559 | <code>海中鱼巣/领域/特征服务.h:243</code> | direct_const_member | this, 宿主节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2594 | R0762 | R0561 | <code>海中鱼巣/领域/特征服务.h:243</code> | direct_const_member | this, 特征类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2595 | R0762 | F0554 | <code>海中鱼巣/领域/特征服务.h:244</code> | direct_const_member | this, 宿主节点, 特征类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2596 | R0762 | R0560 | <code>海中鱼巣/领域/特征服务.h:247</code> | direct_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2597 | R0762 | F0184 | <code>海中鱼巣/领域/特征服务.h:248, 海中鱼巣/领域/特征服务.h:252, 海中鱼巣/领域/特征服务.h:256</code> | direct_free | 条件, 说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2598 | R0762 | F0163 | <code>海中鱼巣/领域/特征服务.h:248</code> | direct_free+overload_resolved | 槽位节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2599 | R0762 | F0167 | <code>海中鱼巣/领域/特征服务.h:251, 海中鱼巣/领域/特征服务.h:255</code> | direct_member+overload_resolved | 关系类型, 源节点, 目标节点, 默认顺序号0 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2600 | R0762 | F0168 | <code>海中鱼巣/领域/特征服务.h:252, 海中鱼巣/领域/特征服务.h:256</code> | direct_free+overload_resolved | 关系句柄 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2601 | R0763 | R0776 | <code>海中鱼巣/领域/特征服务.h:334</code> | direct_const_member | this, 特征节点, 节点类型::特征 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2602 | R0763 | R0778 | <code>海中鱼巣/领域/特征服务.h:338</code> | direct_const_member | this, 特征节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2603 | R0763 | R0777 | <code>海中鱼巣/领域/特征服务.h:340</code> | direct_const_member | this, 特征节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2604 | R0763 | R0782 | <code>海中鱼巣/领域/特征服务.h:345</code> | direct_member | this=特征值_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2605 | R0763 | F0184 | <code>海中鱼巣/领域/特征服务.h:346, 海中鱼巣/领域/特征服务.h:350</code> | direct_free | 条件, 说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2606 | R0763 | F0163 | <code>海中鱼巣/领域/特征服务.h:346</code> | direct_free+overload_resolved | 特征值节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2607 | R0763 | F0167 | <code>海中鱼巣/领域/特征服务.h:349</code> | direct_member+overload_resolved | 关系类型::归属, 特征节点, 特征值节点, 默认顺序号0 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2608 | R0763 | F0168 | <code>海中鱼巣/领域/特征服务.h:350</code> | direct_free+overload_resolved | 归属关系 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2609 | R0764 | F0445 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:249</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2610 | R0764 | F0397 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:250</code> | resolved_function_pointer | 接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2611 | R0764 | F0338 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:251</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2612 | R0764 | F0339 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:252, 海中鱼巣/领域/数据操作.语素基础.ixx:254</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2613 | R0764 | F0441 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:252</code> | direct_const_member | this=&amp;索引_, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2614 | R0764 | R0774 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:254</code> | direct_const_member | this, *目标, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2615 | R0764 | F0345 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:253, 海中鱼巣/领域/数据操作.语素基础.ixx:255</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2616 | R0765 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:578</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2617 | R0765 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:579</code> | resolved_function_pointer | 接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2618 | R0765 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:580</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2619 | R0765 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:581, 海中鱼巣/领域/数据操作.特征体系.ixx:583</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2620 | R0765 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:581</code> | direct_const_member | this=&amp;索引_, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2621 | R0765 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:583</code> | direct_const_member | this, *目标, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2622 | R0765 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:582, 海中鱼巣/领域/数据操作.特征体系.ixx:584</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2623 | R0766 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:587</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2624 | R0766 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:587</code> | direct_free+overload_resolved | 实例槽位 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2625 | R0766 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:588</code> | resolved_function_pointer | 接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2626 | R0766 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:589</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2627 | R0766 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:590</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2628 | R0766 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:590</code> | direct_const_member | this, 实例槽位, std::nullopt, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2629 | R0766 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:590, 海中鱼巣/领域/数据操作.特征体系.ixx:591</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2630 | R0767 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:368</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2631 | R0767 | F0397 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:369</code> | resolved_function_pointer | 接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2632 | R0767 | F0338 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:370</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2633 | R0767 | F0339 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:371, 海中鱼巣/领域/数据操作.状态动态.ixx:373</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2634 | R0767 | F0441 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:371</code> | direct_const_member | this=&amp;索引_, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2635 | R0767 | R0396 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:373</code> | direct_const_member | this, *节点, 主键, 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2636 | R0767 | F0345 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:372, 海中鱼巣/领域/数据操作.状态动态.ixx:374</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2637 | R0768 | F0163 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:89</code> | direct_function | 节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2638 | R0768 | F0565 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:90</code> | direct_function | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2639 | R0769 | F0163 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:102</code> | direct_function | 基础信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2640 | R0769 | F0565 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:102</code> | direct_function | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2641 | R0770 | R0082 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:497, 海中鱼巣/领域/数据操作.语素基础.ixx:501</code> | direct_const_member | 关系类型::运行期临时,目标,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2642 | R0771 | F0163 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:514</code> | direct_function | 目标 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2643 | R0771 | F0346 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:515</code> | direct_const_member | 目标,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2644 | R0771 | F0439 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:520</code> | direct_const_member | 记录-&gt;主信息,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2645 | R0771 | R0772 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:521</code> | direct_const_member | 目标,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2646 | R0771 | R0770 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:523</code> | direct_const_member | 目标,记录-&gt;类型,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2647 | R0772 | F0184 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:536</code> | direct_function | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2648 | R0773 | R0723 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:547</code> | direct_const_member | 目标,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2649 | R0773 | F0441 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:550</code> | direct_const_member | 主键组.front(),令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2650 | R0774 | R0771 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:577</code> | direct_const_member | 目标,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2651 | R0774 | R0768 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:578</code> | direct_const_member | this=&amp;身份 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2652 | R0774 | R0773 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:586</code> | direct_const_member | 目标,期望主键,令牌 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2653 | R0774 | R0775 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:587</code> | direct_const_member | 目标,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2654 | R0775 | F0184 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:595</code> | direct_function | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2655 | R0561 | R0776 | <code>海中鱼巣/领域/特征服务.h:228</code> | direct_const_member | 特征节点,节点类型::特征 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2656 | R0558 | R0776 | <code>海中鱼巣/领域/特征服务.h:1011, 海中鱼巣/领域/特征服务.h:1012</code> | direct_const_member | 特征节点/特征值节点及目标节点类型 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2657 | R0776 | F0190 | <code>海中鱼巣/领域/特征服务.h:1205</code> | direct_const_member | 节点句柄值 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2658 | R0777 | R0778 | <code>海中鱼巣/领域/特征服务.h:1241</code> | direct_const_member | 槽位节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2659 | R0777 | F0621 | <code>海中鱼巣/领域/特征服务.h:1245</code> | direct_const_member | 槽位节点,关系类型::归属 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2660 | R0777 | R0776 | <code>海中鱼巣/领域/特征服务.h:1247</code> | direct_const_member | 候选,节点类型::特征值 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2661 | R0777 | R0780 | <code>海中鱼巣/领域/特征服务.h:1252</code> | function_pointer_callback | 左,右 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2662 | R0777 | F0184 | <code>海中鱼巣/领域/特征服务.h:1263</code> | direct_function | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2663 | R0778 | R0561 | <code>海中鱼巣/领域/特征服务.h:1269</code> | direct_const_member | 槽位节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2664 | R0778 | R0779 | <code>海中鱼巣/领域/特征服务.h:1269</code> | direct_const_member | 槽位节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2665 | R0778 | R0088 | <code>海中鱼巣/领域/特征服务.h:1272</code> | direct_const_member | 槽位节点,关系类型::归属 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2666 | R0778 | R0781 | <code>海中鱼巣/领域/特征服务.h:1273-1274</code> | std-algorithm-callback | 宿主候选 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2667 | R0779 | R0561 | <code>海中鱼巣/领域/特征服务.h:1279, 海中鱼巣/领域/特征服务.h:1284</code> | direct_const_member | 槽位节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2668 | R0779 | F0621 | <code>海中鱼巣/领域/特征服务.h:1282</code> | direct_const_member | 槽位节点,关系类型::模板 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2669 | R0781 | R0559 | <code>海中鱼巣/领域/特征服务.h:1274</code> | lambda_direct_const_member | 宿主候选 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2670 | R0782 | F0321 | <code>海中鱼巣/领域/特征值服务.h:201</code> | direct_const_member | this=&amp;事务接线_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2671 | R0782 | F0336 | <code>海中鱼巣/领域/特征值服务.h:202</code> | direct_const_member | this=&amp;事务接线_ | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2672 | R0782 | F0397 | <code>海中鱼巣/领域/特征值服务.h:203</code> | std-function-target/lambda-call | 事务接线_.运行期状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2673 | R0782 | F0338 | <code>海中鱼巣/领域/特征值服务.h:204</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2674 | R0782 | F0622 | <code>海中鱼巣/领域/特征值服务.h:205</code> | direct_member | 许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2675 | R0782 | F0339 | <code>海中鱼巣/领域/特征值服务.h:205, 海中鱼巣/领域/特征值服务.h:207</code> | direct_const_member | this=&amp;许可 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2676 | R0782 | F0565 | <code>海中鱼巣/领域/特征值服务.h:206</code> | direct_function | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2677 | R0782 | F0623 | <code>海中鱼巣/领域/特征值服务.h:207</code> | direct_member | 节点类型::特征值,主信息,许可.读取令牌() | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2678 | R0782 | F0331 | <code>海中鱼巣/领域/特征值服务.h:209</code> | direct_member | this=&amp;主信息_ | 主信息句柄临时值，作为创建节点实参 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2679 | R0782 | F0332 | <code>海中鱼巣/领域/特征值服务.h:209</code> | direct_member | 节点类型::特征值,主信息_.创建主信息() | 函数返回节点句柄 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2680 | R0782 | F0345 | <code>海中鱼巣/领域/特征值服务.h:203-207</code> | implicit_destructor | 许可 |  | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2681 | R0245 | F0163 | <code>海中鱼巣/领域/初始化.语素.ixx:231</code> | direct_function | 基础信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2682 | R0245 | F0565 | <code>海中鱼巣/领域/初始化.语素.ixx:234</code> | direct_function | 主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2683 | R0777 | F0051 | <code>海中鱼巣/领域/特征服务.h:1253</code> | function_pointer_callback | 左,右 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2684 | R0496 | R0783 | <code>海中鱼巣/领域/服务.基础信息.ixx:24</code> | source-audited-direct-const-member | this=&amp;数据操作_, 主键=请求.幂等主键 | 语义基础业务结果直接作为 R0496 返回值 | 请求.幂等主键非零 | 服务.基础信息.ixx blob 06952da7；22-25 行与 R0783 完整签名复核 |
| RCE2685 | R0783 | F0445 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:333</code> | direct_const_member | this | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2686 | R0783 | R0764 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:334, 海中鱼巣/领域/数据操作.语素基础.ixx:345</code> | direct_const_member | 主键 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2687 | R0783 | R0769 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:335, 海中鱼巣/领域/数据操作.语素基础.ixx:346</code> | direct_const_member | this=&amp;写前/当前 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2688 | R0783 | R0786 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:335, 海中鱼巣/领域/数据操作.语素基础.ixx:347</code> | direct_static_member | 状态,材料 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2689 | R0783 | R0787 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:336</code> | direct_static_member | 写前.状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2690 | R0783 | R0116 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:340</code> | direct_const_member | std::function包装lambda | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2691 | R0783 | R0789 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:353</code> | direct_static_member | 结构结果,当前.状态,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2692 | R0116 | R0784 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback | 会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2693 | R0784 | R0785 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:341-342</code> | direct_static_member | 会话,节点类型::基础信息,主键,新主信息,新节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2694 | R0784 | R0041 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:343</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2695 | R0785 | R0021 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:683</code> | direct_member | this=&amp;会话 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2696 | R0785 | R0640 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:684</code> | function_template_specialization | this=&amp;主结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2697 | R0785 | R0022 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:686</code> | direct_member | 类型,新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2698 | R0785 | R0641 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:687</code> | function_template_specialization | this=&amp;节点结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2699 | R0785 | R0129 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:689-690</code> | direct_free_function | 主键,新节点,索引所有者::语素基础,0 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2700 | R0785 | R0027 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:689-690</code> | direct_member | 形成索引绑定请求(...) | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2701 | R0785 | R0138 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:690</code> | direct_const_member | this=&amp;绑定结果 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2702 | R0785 | R0036 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:691</code> | direct_member | 新主信息 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2703 | R0785 | R0037 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:691</code> | direct_member | 新节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2704 | R0785 | R0638 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:692</code> | direct_member | 主键,新节点 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2705 | R0787 | R0788 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:783</code> | direct_static_member | 状态 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2706 | R0789 | F0184 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:799</code> | direct_function | false,说明 | 被调函数结果按当前调用表达式接收或用于条件判断 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 当前静态接收者/重载/调用点专项复核 |
| RCE2707 | R0276 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:58</code> | direct_member | 场景 | 读取节点结果 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2708 | R0278 | R0790 | <code>海中鱼巣/领域/控制面板服务.h:1033</code> | standard-library-callback | 根谓词 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2709 | R0790 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1035</code> | direct_operator | 根.节点, 当前根 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2710 | R0279 | R0791 | <code>海中鱼巣/领域/控制面板服务.h:1106</code> | standard-library-callback | 子项谓词 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2711 | R0791 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1109</code> | direct_operator | 候选.节点, 项.路径[路径索引] | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2712 | R0279 | R0792 | <code>海中鱼巣/领域/控制面板服务.h:1118</code> | standard-library-callback | 重复谓词 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2713 | R0792 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1121</code> | direct_operator | 候选.节点, 项.概念 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2714 | R0292 | R0793 | <code>海中鱼巣/领域/控制面板服务.h:1625</code> | standard-library-callback | 去重谓词 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2715 | R0793 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1626, 海中鱼巣/领域/控制面板服务.h:1626</code> | direct_operator | 左.first/second, 右.first/second | bool（两次短路比较） | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2716 | R0292 | R0794 | <code>海中鱼巣/领域/控制面板服务.h:1640</code> | standard-library-callback | 来源谓词 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2717 | R0794 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1644</code> | direct_operator | 子节点.节点, 来源.second | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2718 | R0331 | F0450 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:135</code> | direct_member | this；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2719 | R0331 | F0245 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:135</code> | direct_member | 系统角色；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2720 | R0331 | F0229 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:135</code> | direct_member | 参数；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2721 | R0331 | F0397 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:149</code> | resolved_function_pointer | 接线_.运行期状态；调用次数=1 | 共享许可 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2722 | R0331 | F0338 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:150</code> | direct_member | 许可；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2723 | R0331 | F0466 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:181</code> | direct_member | 视图；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2724 | R0331 | F0345 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:149-184</code> | implicit_destructor | 许可；调用次数=1 | 无 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2725 | R0335 | R0563 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:230</code> | direct_member | 根；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2726 | R0335 | F0051 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:233</code> | implicit_underlying_equality | 绑定.节点, optional&lt;节点句柄&gt;{根.节点}；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2727 | R0335 | R0615 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:235</code> | implicit_underlying_equality | 主信息, optional&lt;主信息句柄&gt;{根.主信息}；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2728 | R0337 | R0538 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:284</code> | direct_member | 输出, 活跃角色；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2729 | R0338 | F0346 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:292</code> | direct_member | 根.节点,令牌；调用次数=1 | optional&lt;节点记录&gt; | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2730 | R0338 | R0563 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:293</code> | direct_member | 根；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2731 | R0338 | R0615 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:296</code> | operator_equality | 节点记录值-&gt;主信息,根.主信息；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2732 | R0338 | F0439 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:297</code> | direct_member | 根.主信息,令牌；调用次数=1 | 主信息记录 | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2733 | R0338 | F0051 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:298-299</code> | implicit_underlying_equality | 按主键查节点结果,optional&lt;节点句柄&gt;{根.节点}；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2734 | R0340 | F0450 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:49</code> | direct_member | this；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2735 | R0340 | F0245 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:49</code> | direct_member | 系统角色；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2736 | R0340 | F0229 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:49</code> | direct_member | 参数；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2737 | R0340 | R0795 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:50</code> | direct_member | 活跃规格、冷却规格、退役规格；调用次数=3 | bool（三次） | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2738 | R0340 | R0399 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:51, 海中鱼巣/领域/数据操作.概念活动.ixx:52, 海中鱼巣/领域/数据操作.概念活动.ixx:53</code> | direct_member | 三个写入规格；调用次数=3 | 幂等主键（三次） | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2739 | R0340 | R0400 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:54, 海中鱼巣/领域/数据操作.概念活动.ixx:55, 海中鱼巣/领域/数据操作.概念活动.ixx:56</code> | direct_member | 三个写入规格；调用次数=3 | 状态值（三次） | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2740 | R0340 | F0251 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:125</code> | direct_member | 候选材料；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2741 | R0341 | R0401 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:72</code> | direct_member | 活跃、冷却、退役写入结果；调用次数=3 | bool（三处） | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2742 | R0341 | R0642 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:92</code> | direct_member | 关系结果；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2743 | R0341 | F0251 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:119</code> | direct_member | 候选材料；调用次数=1 | bool | 调用方可达且源码分支条件成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2744 | R0279 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1073, 海中鱼巣/领域/控制面板服务.h:1092, 海中鱼巣/领域/控制面板服务.h:1098</code> | direct_operator | 投影根/路径节点句柄比较 | bool | 调用方可达且相应短路条件成立 | 冻结源码 3920a746 + 本批现状图与映射复核 |
| RCE2745 | R0286 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1307, 海中鱼巣/领域/控制面板服务.h:1308, 海中鱼巣/领域/控制面板服务.h:1309, 海中鱼巣/领域/控制面板服务.h:1310, 海中鱼巣/领域/控制面板服务.h:1311</code> | direct_operator | 需求句柄与承接材料句柄逐字段比较 | bool（五次） | 调用方可达且相应短路条件成立 | 冻结源码 3920a746 + 本批现状图与映射复核 |
| RCE2746 | R0288 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1411, 海中鱼巣/领域/控制面板服务.h:1412, 海中鱼巣/领域/控制面板服务.h:1413, 海中鱼巣/领域/控制面板服务.h:1414, 海中鱼巣/领域/控制面板服务.h:1415</code> | direct_operator | 任务句柄与承接材料句柄逐字段比较 | bool（五次） | 调用方可达且相应短路条件成立 | 冻结源码 3920a746 + 本批现状图与映射复核 |
| RCE2747 | R0290 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1497</code> | direct_operator | 方法候选首节点, 方法节点 | bool | 调用方可达且候选材料存在 | 冻结源码 3920a746 + 本批现状图与映射复核 |
| RCE2748 | R0292 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1637</code> | direct_operator | 来源.first, 方法节点 | bool | 调用方可达且遍历方法来源任务组 | 冻结源码 3920a746 + 本批现状图与映射复核 |
| RCE2749 | R0298 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:1690</code> | standard-library-callback | std::find 当前迭代元素, 节点材料.节点 | 作为 std::find 的相等判定结果 | R0298 可达且 std::find 遍历已见节点组中的至少一个元素 | 冻结源码算法实参类型、F0051 完整签名与 std::find 相等比较语义复核 |
| RCE2750 | R0310 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:589</code> | standard-library-callback | lexicographical_compare passes each left/right path element as const 节点句柄&amp; | element strict-order predicate | R0310 executes the path comparison | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2751 | R0313 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:627</code> | standard-library-default-equality-callback | std::unique compares adjacent 节点句柄 values | adjacent duplicate decision | R0313 executes std::unique | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2752 | R0321 | R0796 | <code>海中鱼巣/领域/控制面板服务.h:817-820</code> | standard-library-callback | std::find_if passes each 概念根登记材料 as const reference | iterator to current-root registration | registered roots passed consistency checks | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2753 | R0796 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:819</code> | direct-free | 登记.根节点, 当前根 | predicate result | find_if invokes the predicate | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2754 | R0321 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:807</code> | standard-library-container-comparison-callback | 排序登记根组 and 活动根组 compare 节点句柄 elements | vector inequality result | root groups are compared after sort and dedup | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2755 | R0326 | R0797 | <code>海中鱼巣/领域/控制面板服务.h:952-955</code> | standard-library-callback | std::find_if passes each 控制面板树节点材料 as const reference | iterator to requested root | R0326 iterates requested page roots | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2756 | R0797 | F0051 | <code>海中鱼巣/领域/控制面板服务.h:954</code> | direct-free | 根.节点, 当前根 | predicate result | find_if invokes the predicate | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2757 | R0328 | R0328 | <code>海中鱼巣/领域/控制面板服务.h:977</code> | direct-recursive | 节点材料.子节点组[索引] | child expansion success | each original child before the frozen original-child count | 源码冻结提交 3920a746 的直接调用、标准算法注册点与静态重载复核 |
| RCE2758 | R0344 | F0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1029</code> | direct-member | 目标,令牌 | 节点记录optional | 调用方可达且目标句柄有效 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2759 | R0344 | F0439 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1034</code> | direct-member | 记录-&gt;主信息,令牌 | 主信息记录optional | 节点记录存在 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2760 | R0346 | F0440 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1057</code> | direct-member | 句柄,令牌 | 当前关系optional | 输入记录字段前置有效 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2761 | R0346 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1059, 海中鱼巣/领域/数据操作.特征体系.ixx:1060</code> | implicit-underlying-equality | 当前/记录两端节点 | 布尔比较结果 | 当前关系存在 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2762 | R0347 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1073</code> | implicit-underlying-equality | 反查节点,目标 | 布尔比较结果 | 唯一非零主键及期望匹配 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2763 | R0350 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1183</code> | operator-equality | 候选宿主/定义,请求宿主/定义 | 两个布尔比较结果 | 候选槽位完整 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2764 | R0351 | F0383 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1234, 海中鱼巣/领域/数据操作.特征体系.ixx:1237</code> | direct-member | 身份.主信息,0,令牌 | I64 optional | 侧表可读 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2765 | R0353 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1312</code> | resolved-function-pointer | 接线_.运行期状态 | 共享结构许可 | 入口有效且主键非零 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2766 | R0353 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1312-1328</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2767 | R0374 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:274</code> | direct_operator | 宿主_, 特征定义_ | bool | 主键非零且两个节点句柄均有效 | 冻结源码 3920a746 + R0374 现状图与逐行映射复核 |
| RCE2768 | R0358 | R0640 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1447</code> | function_template_specialization | this=&amp;主结果 | bool 绑定主结果成功判断，取反后失败返回 false | R0021 已返回主结果 | 冻结源码调用点、接收者静态类型与模板实例复核 |
| RCE2769 | R0358 | R0641 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1450</code> | function_template_specialization | this=&amp;节点结果 | bool 绑定节点结果成功判断，取反后失败返回 false | R0022 已返回节点结果 | 冻结源码调用点、接收者静态类型与模板实例复核 |
| RCE2770 | R0358 | R0138 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1453</code> | direct_const_member | this=&amp;会话.绑定主键(...)返回的结构写入结果 | bool 取反用于绑定失败短路返回 | R0129 已形成请求且 R0027 已返回结果 | 冻结源码调用点、R0027 返回类型与成员调用复核 |
| RCE2771 | R0359 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1462</code> | direct_const_member | this=&amp;宿主 | bool 作为 &amp;&amp; 首项，失败时短路返回 false | 进入返回表达式即调用 | 冻结源码调用点、接收者静态类型与唯一成员签名复核 |
| RCE2772 | R0360 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1480</code> | direct_free_operator | 模板.目标节点, 特征定义 | bool；为 true 时立即返回 true | 外层目标节点类型为特征且内层模板迭代取得当前记录 | 冻结源码调用点、两侧静态类型与运算符重载复核 |
| RCE2773 | R0361 | R0642 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1509, 海中鱼巣/领域/数据操作.特征体系.ixx:1512</code> | function_template_specialization | this=&amp;宿主关系; this=&amp;模板关系 | bool 分别绑定宿主关系/模板关系成功判断，取反后失败返回 false | 对应 R0023 已返回带值关系结果 | 冻结源码两个调用点、接收者静态类型与模板实例复核 |
| RCE2774 | R0362 | R0642 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1536</code> | function_template_specialization | this=&amp;当前关系 | bool 并取反用于失败短路 | R0023 返回关系候选结果后 | 冻结源码调用点、局部变量静态类型与模板实例复核 |
| RCE2775 | R0362 | R0138 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1538</code> | direct_const_member | this=&amp;会话.写入候选I64值(...)返回临时结果 | bool 并取反用于失败短路 | 原始类型为 I64 且规格 I64 optional 已解引用 | 冻结源码调用点、R0028 返回类型与成员调用复核 |
| RCE2776 | R0363 | F0051 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1566, 海中鱼巣/领域/数据操作.特征体系.ixx:1567</code> | direct_free_operator | 材料.宿主, 规格.读取宿主(); 材料.特征定义, 规格.读取特征定义() | bool 分别参与宿主与特征定义的 &amp;&amp; 短路比较 | 材料完整、幂等主键相等，且按短路顺序到达各比较项 | 冻结源码两个调用点、字段/返回静态类型与运算符重载复核 |
| RCE2777 | R0383 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:565</code> | resolved-function-pointer | 接线_.运行期状态 | 局部结构事务许可 许可 | 数据操作有效且目标节点句柄有效 | 冻结源码 3920a746 + 调用点、函数指针静态目标与完整签名复核 |
| RCE2778 | R0383 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:565-568</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部许可生命周期与析构身份复核 |
| RCE2779 | R0384 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:595</code> | resolved-function-pointer | 接线_.运行期状态 | 局部结构事务许可 许可 | 数据操作有效且主键非零 | 冻结源码 3920a746 + 调用点、函数指针静态目标与完整签名复核 |
| RCE2780 | R0384 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:595-600</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部许可生命周期与析构身份复核 |
| RCE2781 | R0385 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:604</code> | resolved-function-pointer | 接线_.运行期状态 | 局部结构事务许可 许可 | 数据操作有效且宿主、特征定义两个节点句柄均有效 | 冻结源码 3920a746 + 调用点、函数指针静态目标与完整签名复核 |
| RCE2782 | R0385 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:604-607</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部许可生命周期与析构身份复核 |
| RCE2783 | R0386 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:611</code> | resolved-function-pointer | 接线_.运行期状态 | 局部结构事务许可 许可 | 数据操作有效且实例槽位节点句柄有效 | 冻结源码 3920a746 + 调用点、函数指针静态目标与完整签名复核 |
| RCE2784 | R0386 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:611-616</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部许可生命周期与析构身份复核 |
| RCE2785 | R0388 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:76</code> | direct_free_function | 节点 | bool，作为 &amp;&amp; 短路项 | 状态为已找到 | 冻结源码 3920a746 + 调用点、实参静态类型与重载签名复核 |
| RCE2786 | R0388 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:76</code> | direct_free_function | 主信息 | bool，作为 &amp;&amp; 末项 | 状态为已找到且 F0163 返回 true | 冻结源码 3920a746 + 调用点、实参静态类型与重载签名复核 |
| RCE2787 | R0385 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:603</code> | direct_free_function | 宿主; 特征定义 | 两个 bool 分别参与 \|\| 短路拒绝 | F0444 有效；第二次调用还要求宿主句柄有效 | 冻结源码 3920a746 + 调用点、实参静态类型与重载签名复核 |
| RCE2788 | R0386 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:610</code> | direct_free_function | 实例槽位 | bool，取反参与入口拒绝 | F0444 有效 | 冻结源码 3920a746 + 调用点、实参静态类型与重载签名复核 |
| RCE2789 | R0389 | R0371 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:836, 海中鱼巣/领域/数据操作.特征体系.ixx:856</code> | direct_const_member | this=&amp;槽位规格 | uint64_t，分别参与幂等键比较与主键槽位读取 | 按前序短路到达对应表达式 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2790 | R0389 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:837, 海中鱼巣/领域/数据操作.特征体系.ixx:845, 海中鱼巣/领域/数据操作.特征体系.ixx:861, 海中鱼巣/领域/数据操作.特征体系.ixx:901</code> | direct_const_member | this=&amp;槽位规格 | 节点句柄，供身份读取或宿主定义槽位读取 | 分别位于入口身份读取、首次读取、并发重读和写后重读路径 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2791 | R0389 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:838, 海中鱼巣/领域/数据操作.特征体系.ixx:845, 海中鱼巣/领域/数据操作.特征体系.ixx:861, 海中鱼巣/领域/数据操作.特征体系.ixx:901</code> | direct_const_member | this=&amp;槽位规格 | 节点句柄，供定义读取或宿主定义槽位读取 | 分别位于入口定义读取、首次读取、并发重读和写后重读路径 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2792 | R0389 | R0375 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:836, 海中鱼巣/领域/数据操作.特征体系.ixx:857</code> | direct_const_member | this=&amp;值规格 | uint64_t，分别参与幂等键比较与主键值候选读取 | 按前序短路到达对应表达式 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2793 | R0391 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:887-888</code> | direct_const_member | this=&amp;槽位规格 | 节点句柄，作为 R0360 宿主参数 | R0359 返回 true 后到达 \|\| 第二项 | 冻结源码 3920a746 + 调用点、捕获对象静态类型与完整签名复核 |
| RCE2794 | R0391 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:887-888</code> | direct_const_member | this=&amp;槽位规格 | 节点句柄，作为 R0360 特征定义参数 | R0359 返回 true 后到达 \|\| 第二项 | 冻结源码 3920a746 + 调用点、捕获对象静态类型与完整签名复核 |
| RCE2795 | R0389 | R0798 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:838</code> | direct_const_member | 槽位规格.读取特征定义() | 局部特征定义值式材料 定义 | 入口规格完整且两个幂等主键不同 | 冻结源码 3920a746 + 调用点、实参静态类型与 R0798 完整签名复核 |
| RCE2796 | R0798 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:571</code> | direct_const_member | this | bool，取反参与入口拒绝 | 进入 R0798 返回前置表达式 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2797 | R0798 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:571</code> | direct_free_function | 特征定义 | bool，取反参与入口拒绝 | F0444 返回 true 后 | 冻结源码 3920a746 + 调用点、实参静态类型与重载签名复核 |
| RCE2798 | R0798 | F0397 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:572</code> | resolved-function-pointer | 接线_.运行期状态 | 局部结构事务许可 许可 | 数据操作有效且特征定义节点句柄有效 | 冻结源码 3920a746 + 调用点、函数指针静态目标与完整签名复核 |
| RCE2799 | R0798 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:573</code> | direct_const_member | this=&amp;许可 | bool，取反控制许可拒绝返回 | 共享许可已构造 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2800 | R0798 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:574</code> | direct_const_member | 特征定义, std::nullopt, 许可.读取令牌() | 特征定义值式材料直接作为 R0798 返回 | 许可有效 | 冻结源码 3920a746 + 调用点、实参静态类型与完整签名复核 |
| RCE2801 | R0798 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:574</code> | direct_const_member | this=&amp;许可 | const 结构事务令牌&amp;，作为 R0348 第三实参 | 许可有效且准备调用 R0348 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2802 | R0798 | F0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:572-575</code> | implicit-destructor | 许可对象 | void | 共享许可构造成功后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部许可生命周期与析构身份复核 |
| RCE2803 | R0389 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:839</code> | direct_const_member | this=&amp;宿主身份 | bool，取反作为身份/定义失败分支首项 | 宿主身份与定义材料均已读取 | 冻结源码 3920a746 + 调用点、接收者静态类型与完整签名复核 |
| RCE2804 | R0389 | R0799 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:884-916</code> | implicit-destructor | 参与者对象 | void | 参与者在 884 行成功构造后的任一正常、早退或异常退出 | 冻结源码 3920a746 + 局部参与者生命周期与 R0799 析构身份复核 |
| RCE2811 | R0392 | F0346 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1015</code> | direct_const_member | 节点, 令牌 | 节点记录optional | 节点句柄有效 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2812 | R0392 | F0439 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1020</code> | direct_const_member | 记录-&gt;主信息, 令牌 | 主信息记录optional | 节点记录存在 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2813 | R0393 | R0406 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:102</code> | direct_const_member | this | 当前可读布尔条件 | 调用方可达并求值完整实例状态 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2814 | R0393 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:104</code> | direct_const_member | 场景临时关系、主体关系、场景关系；同一行三次调用 | 三项关系完整性布尔条件 | 前序实例状态短路条件全部成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2815 | R0395 | F0440 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1043</code> | direct_const_member | 句柄, 令牌 | 当前关系记录optional | 输入记录状态有效且两端节点句柄有效 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2816 | R0395 | F0051 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1045, 海中鱼巣/领域/数据操作.状态动态.ixx:1046</code> | implicit_underlying_equality | 当前/输入记录的源节点与目标节点 | 两个节点句柄相等布尔条件 | 当前关系记录存在 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2817 | R0396 | F0383 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1068, 海中鱼巣/领域/数据操作.状态动态.ixx:1082, 海中鱼巣/领域/数据操作.状态动态.ixx:1083, 海中鱼巣/领域/数据操作.状态动态.ixx:1084</code> | direct_const_member | 身份.主信息；状态值槽位/发生时间戳槽位/幂等编号低位槽位/幂等编号高位槽位；令牌 | 状态值、时间、低位和高位四个I64 optional | 节点身份已找到且类型为状态 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2818 | R0396 | R0664 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1111</code> | direct_const_member | this=&amp;目标身份 | 目标身份当前可读布尔条件 | 引用关系证据可读且目标节点身份已经读回 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2819 | R0396 | F0051 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1127</code> | implicit_underlying_equality | 记录.目标节点, 状态节点 | 节点句柄相等布尔条件 | 遍历场景运行期临时关系记录 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2820 | R0398 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:160, 海中鱼巣/领域/数据操作.状态动态.ixx:161, 海中鱼巣/领域/数据操作.状态动态.ixx:162</code> | direct_const_member | 场景临时关系、主体关系、场景关系、目标关系、前状态关系、后状态关系、来源动作关系 | 七项关系完整性布尔条件 | 前序抽象动态短路条件全部成立 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2821 | R0402 | F0345 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:378-381</code> | implicit_destructor | this=&amp;许可 | void；释放共享许可 | 第378行已取得局部共享许可 | 冻结源码、局部静态类型、F0345签名及RAII生命周期复核 |
| RCE2822 | R0403 | R0795 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:409</code> | direct_const_member | this=&amp;规格 | bool；与F0443结果短路合取 | R0403进入入口复核 | 冻结源码接收者静态类型与完整重载签名复核 |
| RCE2823 | R0403 | R0399 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:412</code> | direct_const_member | this=&amp;规格 | std::uint64_t；绑定R0405的主键实参 | 入口复核通过并准备调用R0405 | 冻结源码接收者静态类型、返回类型与实参绑定复核 |
| RCE2824 | R0405 | R0640 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:903</code> | function_template_specialization | this=&amp;主结果 | bool；失败时返回false | R0021已返回主结果 | 冻结源码接收者静态类型与模板实例复核 |
| RCE2825 | R0405 | R0138 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:905, 海中鱼巣/领域/数据操作.状态动态.ixx:910</code> | direct_const_member | this=&amp;写入候选I64值或绑定主键返回的结构写入结果 | bool；失败时返回false | 对应会话写入调用已经返回 | 冻结源码、被调返回类型与成员调用复核 |
| RCE2826 | R0405 | R0641 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:907</code> | function_template_specialization | this=&amp;节点结果 | bool；失败时返回false | R0022已返回节点结果 | 冻结源码接收者静态类型与模板实例复核 |
| RCE2840 | R0409 | F0449 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:126</code> | source-audited-direct-const-member | this=this（系统角色数据操作） | bool 进入入口短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2841 | R0409 | F0458 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:126</code> | source-audited-direct-const-member | this=&amp;主体 | bool 进入入口短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2842 | R0409 | F0074 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:173</code> | source-audited-direct-const-member | this=&amp;当前 | bool 决定原样返回或继续改写状态 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2843 | R0413 | R0563 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:232</code> | source-audited-direct-const-member | this=&amp;身份 | bool 进入身份完整性短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2844 | R0413 | F0346 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:233</code> | source-audited-direct-const-member | this=&amp;节点_, 节点=身份.节点, 令牌 | std::optional&lt;节点记录&gt; 绑定到记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2845 | R0413 | R0615 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:235</code> | direct-free-operator | 左=记录-&gt;主信息, 右=身份.主信息 | bool 进入复核短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2846 | R0413 | F0439 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:236</code> | source-audited-direct-const-member | this=&amp;主信息_, 主信息=身份.主信息, 令牌 | std::optional&lt;主信息记录&gt; 供 has_value 判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2847 | R0413 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:237-238</code> | operator | 左右 optional 内节点句柄 | bool 汇入 optional 相等判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2848 | R0414 | R0563 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:246</code> | source-audited-direct-const-member | this=&amp;身份 | bool 进入会话身份复核短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2849 | R0414 | R0615 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:248</code> | operator | 左右 optional 内主信息句柄 | bool 汇入 optional 相等判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2850 | R0416 | F0440 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:271</code> | source-audited-direct-const-member | this=&amp;关系_, 关系, 令牌 | std::optional&lt;关系记录&gt; 绑定到记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2851 | R0417 | F0459 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:284</code> | source-audited-direct-const-member | this=&amp;关系 | bool 进入关系完整性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2852 | R0417 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:287, 海中鱼巣/领域/数据操作.系统角色.ixx:288</code> | direct-free-operator | 左=当前记录端点, 右=关系材料端点 | 两个 bool 进入短路复核判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2853 | R0418 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:305, 海中鱼巣/领域/数据操作.系统角色.ixx:306, 海中鱼巣/领域/数据操作.系统角色.ixx:327, 海中鱼巣/领域/数据操作.系统角色.ixx:328</code> | direct-free-operator | 左=关系记录端点, 右=父节点或子节点 | 四个 bool 进入短路复核判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2854 | R0419 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:346, 海中鱼巣/领域/数据操作.系统角色.ixx:347</code> | direct-free-operator | 左=记录端点, 右=父节点或子节点 | 两个 bool 进入短路复核判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2855 | R0420 | F0346 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:361</code> | source-audited-direct-const-member | this=&amp;节点_, 节点=记录.目标节点, 令牌 | std::optional&lt;节点记录&gt; 绑定到来源 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2856 | R0420 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:365, 海中鱼巣/领域/数据操作.系统角色.ixx:367</code> | operator | std::find 当前节点句柄与记录.目标节点 | bool 供两个 std::find 匹配判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2857 | R0421 | F0458 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:375</code> | source-audited-direct-const-member | this=&amp;主体 | bool 进入入口完整性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2858 | R0421 | F0397 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:376</code> | resolved-function-pointer | 状态=接线_.运行期状态 | 结构事务许可 绑定到许可 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2859 | R0421 | F0338 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:377</code> | source-audited-direct-const-member | this=&amp;许可 | bool 进入许可有效性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2860 | R0421 | F0345 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:376-410</code> | implicit_destructor | this=&amp;许可 |  | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2861 | R0421 | F0457 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:395</code> | direct_operator | this=&amp;*归属, 右=主体.场景接纳自我关系 | bool 进入关系材料一致性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2862 | R0421 | F0245 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:407</code> | source-audited-direct-const-member | this=&amp;清单 | bool 决定幂等读回或内部不一致 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2863 | R0422 | F0449 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:43</code> | source-audited-direct-const-member | this=this（系统角色数据操作） | bool 进入入口短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2864 | R0422 | F0228 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:43</code> | source-audited-direct-const-member | this=&amp;参数 | bool 进入入口短路判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2865 | R0422 | F0397 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:46</code> | resolved-function-pointer | 状态=接线_.运行期状态 | 结构事务许可 绑定到许可 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2866 | R0422 | F0338 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:47</code> | source-audited-direct-const-member | this=&amp;许可 | bool 进入许可有效性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2867 | R0422 | F0345 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:46-122</code> | implicit_destructor | this=&amp;许可 |  | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2868 | R0422 | F0346 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:55</code> | source-audited-direct-const-member | this=&amp;节点_, 节点=*节点, 令牌 | std::optional&lt;节点记录&gt; 绑定到记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2869 | R0422 | F0439 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:64</code> | source-audited-direct-const-member | this=&amp;主信息_, 主信息=记录-&gt;主信息, 令牌 | std::optional&lt;主信息记录&gt; 供 has_value 判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2870 | R0422 | R0625 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:81, 海中鱼巣/领域/数据操作.系统角色.ixx:84, 海中鱼巣/领域/数据操作.系统角色.ixx:87, 海中鱼巣/领域/数据操作.系统角色.ixx:90, 海中鱼巣/领域/数据操作.系统角色.ixx:96</code> | source-audited-direct-const-member | this=安全根需求/服务根需求/自我存在/概念存在根/自我存在 | 五个 bool 进入占用判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2871 | R0422 | F0440 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:113</code> | source-audited-direct-const-member | this=&amp;关系_, 关系=句柄, 令牌 | std::optional&lt;关系记录&gt; 供 has_value 判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2872 | R0422 | F0051 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:104, 海中鱼巣/领域/数据操作.系统角色.ixx:105</code> | operator | 左=归属记录源/目标节点, 右=自我场景/自我存在节点 | 两个 bool 经 != 重写进入兼容性判断 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码 3920a746 + 本批现状单函数图、逐行映射与调用点复核 |
| RCE2920 | R0423 | F0397 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:127</code> | function_pointer_target | 状态=接线_.运行期状态 | 结构事务许可 | F0446 返回 true 且主键非 0；生产接线成立 | 冻结源码、生产装配函数指针与已发布身份表复核 |
| RCE2921 | R0423 | F0338 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:128</code> | direct_const_member | this=&amp;许可 | 许可有效布尔条件 | 共享许可已形成 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2922 | R0423 | F0345 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:127-132</code> | implicit_destructor | this=&amp;许可 | void；释放共享许可 | 许可形成后的所有正常、提前返回与异常退出路径 | 冻结源码局部静态类型与 RAII 生命周期复核 |
| RCE2923 | R0424 | F0051 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:208</code> | implicit_underlying_equality | 反查节点, 目标 | 布尔比较结果 | 唯一非零主键且期望匹配 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2924 | R0425 | F0346 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:220</code> | direct_const_member | 目标, 令牌 | 节点记录 optional | 调用方可达且目标句柄进入已许可读取 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2925 | R0425 | F0439 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:229</code> | direct_const_member | 记录-&gt;主信息, 令牌 | 主信息记录 optional | 节点记录存在且类型为因果引用 | 冻结源码逐调用点、静态类型与已发布身份表复核 |
| RCE2950 | R0427 | R0832 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1029</code> | direct_const_member | this=&amp;规格 | bool；决定optional返回分支 | R0427已构造局部规格 | 冻结源码接收者静态类型、完整签名与调用位置复核 |
| RCE2951 | R0832 | R0795 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:693, 海中鱼巣/领域/数据操作.需求任务方法.ixx:694</code> | direct_const_member | this=&amp;角色状态_ / &amp;活跃状态_ / &amp;失效状态_ | bool；与根主键非零短路合取 | R0832执行完整性复核 | 冻结源码接收者静态类型、完整重载签名与三次调用复核 |
| RCE2952 | R0428 | F0565 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:112</code> | overload_resolved_free_function | 主信息 | bool；参与当前可读短路合取 | 读取状态已找到且节点句柄有效 | 冻结源码实参静态类型与完整重载集合复核 |
| RCE2953 | R0429 | F0397 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1112</code> | resolved-function-pointer | 接线_.运行期状态 | 结构事务许可；绑定局部许可 | 入口有效且主键非零 | 冻结源码静态接线类型、实际装配目标与调用位置复核 |
| RCE2954 | R0429 | F0345 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1112-1122</code> | implicit_destructor | this=&amp;许可 | void；释放共享许可 | 第1112行已取得局部共享许可 | 冻结源码、局部静态类型、F0345签名及RAII生命周期复核 |
| RCE2955 | R0430 | F0397 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1161</code> | resolved-function-pointer | 接线_.运行期状态 | 结构事务许可；绑定局部许可 | 入口有效且主键非零 | 冻结源码静态接线类型、实际装配目标与调用位置复核 |
| RCE2956 | R0430 | F0345 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1161-1166</code> | implicit_destructor | this=&amp;许可 | void；释放共享许可 | 第1161行已取得局部共享许可 | 冻结源码、局部静态类型、F0345签名及RAII生命周期复核 |
| RCE2957 | R0430 | F0338 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1162</code> | direct_const_member | this=&amp;许可 | bool；无效时返回许可拒绝 | 已取得局部共享许可 | 冻结源码接收者静态类型与F0338完整签名复核 |
| RCE2958 | R0432 | R0832 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1620</code> | direct_const_member | this=&amp;规格 | bool；不完整时入口拒绝 | R0432入口执行 | 冻结源码接收者静态类型、完整签名与调用位置复核 |
| RCE2959 | R0433 | R0401 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1638</code> | direct_const_member | this=&amp;角色 / &amp;活跃 / &amp;失效 | bool；任一失败即回调返回 | 三次抽象状态会话写入已经返回 | 冻结源码接收者静态类型、完整签名与同行三次调用复核 |
| RCE2960 | R0434 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:190</code> | direct_const_member | this=&amp;身份 | bool；参与完整目标状态需求短路合取 | 读取状态为已找到 | 冻结源码接收者静态类型、完整签名与调用位置复核 |
| RCE2961 | R0434 | R0830 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:194, 海中鱼巣/领域/数据操作.需求任务方法.ixx:195, 海中鱼巣/领域/数据操作.需求任务方法.ixx:196, 海中鱼巣/领域/数据操作.需求任务方法.ixx:197</code> | direct_const_member | this=&amp;主体关系 / &amp;目标宿主关系 / &amp;场景关系 / &amp;目标特征关系 / &amp;当前特征状态关系 / &amp;目标状态关系 | bool；参与完整目标状态需求短路合取 | 身份和六个节点句柄前置均有效 | 冻结源码接收者静态类型、完整签名与六次调用复核 |
| RCE2962 | R0830 | R0831 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:99</code> | direct_const_member | this | bool；与记录状态有效短路合取 | R0830执行当前有效复核 | 冻结源码接收者静态类型与完整签名复核 |
| RCE2963 | R0831 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:95</code> | overload_resolved_free_function | 关系 | bool；参与完整性短路合取 | R0831执行完整性复核 | 冻结源码实参静态类型与完整重载集合复核 |
| RCE2964 | R0831 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:95</code> | overload_resolved_free_function | 源节点 / 目标节点 | bool；参与完整性短路合取 | 关系句柄有效 | 冻结源码实参静态类型、完整重载集合与两次调用复核 |
| RCE2965 | R0456 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:292</code> | direct_const_member | this=&amp;身份 | bool；参与方法登记根材料完整性短路合取 | 读取状态为已找到 | 冻结源码接收者静态类型与R0428完整签名复核 |
| RCE2966 | R0456 | R0830 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:296, 海中鱼巣/领域/数据操作.需求任务方法.ixx:297</code> | direct_const_member | this=&amp;角色关系；this=&amp;活跃状态关系；this=&amp;失效状态关系，共三次调用 | 三次bool结果参与方法登记根材料完整性短路合取 | 三个状态节点句柄有效且两两不同 | 冻结源码三次接收者静态类型与R0830完整签名复核 |
| RCE2967 | R0447 | R0830 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2458</code> | direct_const_member | this=&amp;方法投影 / &amp;发布 | 两个bool进入入口短路判断 | 身份当前可读且类型为任务方法选择记录 | 数据操作.需求任务方法.ixx blob 16ac9534；海中鱼巣/领域/数据操作.需求任务方法.ixx:2458 与被调函数完整身份复核 |
| RCE3000 | R0435 | F0346 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1941</code> | direct_member | this=&amp;节点_; 节点=节点; 令牌=令牌 | 记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3001 | R0435 | F0439 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1946</code> | direct_member | this=&amp;主信息_; 主信息=记录的主信息; 令牌=令牌 | 主信息记录可选值 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3002 | R0436 | R0073 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1971</code> | direct_member | this=&amp;关系_; 句柄=句柄; 令牌=令牌 | 当前关系审计记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3003 | R0436 | F0440 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1972</code> | direct_member | this=&amp;关系_; 句柄=句柄; 令牌=令牌 | 当前有效关系记录 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3004 | R0440 | F0383 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2009, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2010, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2011-2012, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2013-2014</code> | direct_member | this=&amp;主信息_; 主信息=身份.主信息; 槽位=对应状态值/时间戳/幂等编号槽位; 令牌=令牌 | 四个 I64 可选值 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3005 | R0440 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2048</code> | direct_operator | 记录.目标节点, 状态 | 是否相等 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3006 | R0440 | R0393 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2069</code> | direct_member | this=&amp;输出 | 是否为完整实例状态 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3007 | R0441 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2082, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2175, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2176, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2177, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2178</code> | direct_operator | 节点句柄操作数来自需求、状态、场景、主体和目标状态 | 是否相等 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3008 | R0441 | R0598 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2099, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2100</code> | direct_operator | 高级关系证据中的关系句柄与结算状态关系句柄 | 是否相等 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3009 | R0441 | R0840 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2173</code> | direct_member | this=&amp;来源任务 | 是否完整承接 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3010 | R0441 | R0841 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2178</code> | direct_member | this=&amp;输出 | 是否完整 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3011 | R0441 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2102, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2127, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2128, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2141, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2142</code> | direct_member | this=&amp;目标或条件表达式中的高级节点身份 | 是否当前可读 | 调用方从 F0001 可达且对应源码分支成立 | 冻结源码逐调用点和静态接收者复核；当前代码自冻结提交零差异 |
| RCE3012 | R0442 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2189, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2207, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2225, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2237, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2273, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2283</code> | source-audited-direct | this分别绑定输出身份、目标、来源、父身份或正式结算目标身份 | bool进入当前可读短路判断 | 对应身份已读取 | 数据操作.需求任务方法.ixx blob 16ac9534；2189、2207、2225、2237、2273、2283 行与 R0428 完整身份复核 |
| RCE3013 | R0442 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2249</code> | direct-free-operator | 宿主关系源节点与需求句柄 | bool选择宿主关系另一端 | 六项必需关系均存在 | 数据操作.需求任务方法.ixx blob 16ac9534；2249 行与 F0051 完整身份复核 |
| RCE3014 | R0444 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2318, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2336, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2353</code> | source-audited-direct | this分别绑定输出身份或目标身份 | bool进入当前可读短路判断 | 对应身份已读取 | 数据操作.需求任务方法.ixx blob 16ac9534；2318、2336、2353 行与 R0428 完整身份复核 |
| RCE3015 | R0444 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2366</code> | direct-free-operator | 归属记录源节点与来源需求目标节点 | bool决定跳过或处理授权关系 | 授权循环本轮记录 | 数据操作.需求任务方法.ixx blob 16ac9534；2366 行与 F0051 完整身份复核 |
| RCE3016 | R0444 | R0843 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2362</code> | source-audited-direct | this=&amp;*生命周期 | bool进入生命周期当前判断 | 生命周期optional有值 | 数据操作.需求任务方法.ixx blob 16ac9534；2362 行与 R0843 完整身份复核 |
| RCE3017 | R0446 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2414</code> | source-audited-direct | this=&amp;状态身份 | bool进入入口复合判断 | 状态身份已读取 | 数据操作.需求任务方法.ixx blob 16ac9534；2414 行与 R0428 完整身份复核 |
| RCE3018 | R0446 | F0383 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2419, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2420</code> | source-audited-direct | this=&amp;主信息_；状态身份.主信息；状态值槽位/状态时间戳槽位；令牌 | 两个optional&lt;int64_t&gt;绑定状态值与时间戳 | 证据、身份、类型和版本范围均成立 | 数据操作.需求任务方法.ixx blob 16ac9534；2419、2420 行与 F0383 完整身份复核 |
| RCE3020 | R0447 | F0383 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2461</code> | source-audited-direct | this=&amp;主信息_；身份.主信息；索引号；令牌 | optional&lt;int64_t&gt;绑定值 | 16槽循环本轮索引 | 数据操作.需求任务方法.ixx blob 16ac9534；2461 行与 F0383 完整身份复核 |
| RCE3021 | R0447 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2480, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2481</code> | direct-free-operator | 方法与方法投影目标；发布目标与记录节点；方法投影源与发布源 | 三个bool进入关联一致性短路判断 | 16槽均通过范围检查 | 数据操作.需求任务方法.ixx blob 16ac9534；2480、2481 行与 F0051 完整身份复核 |
| RCE3022 | R0447 | R0844 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2488</code> | source-audited-direct | this=&amp;输出 | bool决定返回材料optional或nullopt | 任务方法选择材料已组装 | 数据操作.需求任务方法.ixx blob 16ac9534；2488 行与 R0844 完整身份复核 |
| RCE3023 | R0448 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2518</code> | source-audited-direct | this=&amp;输出.身份 | bool进入入口判断 | 根身份已读取 | 数据操作.需求任务方法.ixx blob 16ac9534；2518 行与 R0428 完整身份复核 |
| RCE3024 | R0450 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2760</code> | source-audited-direct | this=&amp;身份 | bool进入身份可读和类型短路判断 | 状态身份已读取 | 数据操作.需求任务方法.ixx blob 16ac9534；2760 行与 R0428 完整身份复核 |
| RCE3025 | R0450 | F0383 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2765</code> | source-audited-direct | this=&amp;主信息_；身份.主信息；状态值槽位；令牌 | optional&lt;int64_t&gt;直接作为返回值 | 身份可读、类型为状态且无运行期临时关系 | 数据操作.需求任务方法.ixx blob 16ac9534；2765 行与 F0383 完整身份复核 |
| RCE3026 | R0451 | R0640 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2779</code> | source-audited-direct | this=&amp;主结果 | bool决定返回false或继续 | 主信息候选创建完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2779 行与 R0640 完整身份复核 |
| RCE3027 | R0451 | R0845 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2782, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2788, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2793</code> | source-audited-direct | 故障 | 三个bool分别决定故障注入返回false | Debug\|x64且对应候选写点完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2782、2788、2793 行与 R0845 完整身份复核 |
| RCE3028 | R0451 | R0641 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2785</code> | source-audited-direct | this=&amp;节点结果 | bool决定返回false或继续 | 节点候选创建完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2785 行与 R0641 完整身份复核 |
| RCE3029 | R0451 | R0138 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2791</code> | source-audited-direct | this=&amp;绑定主键结果 | bool决定返回false或继续 | 绑定主键调用完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2791 行与 R0138 完整身份复核 |
| RCE3030 | R0452 | R0642 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2810</code> | source-audited-direct | this=&amp;结果 | bool决定返回false或继续 | 关系候选创建完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2810 行与 R0642 完整身份复核 |
| RCE3031 | R0452 | R0598 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2811</code> | algorithm-callback | std::find逐元素比较的关系句柄左值与目标句柄 | bool供std::find决定命中或推进 | std::find对0..n个元素按需回调 | 数据操作.需求任务方法.ixx blob 16ac9534；2811 行与 R0598 完整身份复核 |
| RCE3032 | R0452 | R0845 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2815</code> | source-audited-direct | 故障 | bool决定故障注入返回false | Debug\|x64且关系候选去重登记完成 | 数据操作.需求任务方法.ixx blob 16ac9534；2815 行与 R0845 完整身份复核 |
| RCE3033 | R0453 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2839</code> | direct-free-operator | 主体与目标宿主 | bool选择宿主关系方向 | 第一条主体引用关系写入成功 | 数据操作.需求任务方法.ixx blob 16ac9534；2839 行与 F0051 完整身份复核 |
| RCE3034 | R0843 | R0846 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:223</code> | source-audited-direct | this=this | bool进入当前性短路判断 | R0843进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；223 行与 R0846 完整身份复核 |
| RCE3035 | R0843 | R0830 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:224</code> | source-audited-direct | this=&amp;关系 | bool作为当前性短路末项 | 材料完整且记录阶段有效 | 数据操作.需求任务方法.ixx blob 16ac9534；224 行与 R0830 完整身份复核 |
| RCE3036 | R0846 | R0831 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:217</code> | source-audited-direct | this=&amp;关系 | bool进入完整性短路判断 | R0846进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；217 行与 R0831 完整身份复核 |
| RCE3037 | R0846 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:217</code> | direct-free-overload | 状态节点句柄 | bool进入完整性短路判断 | 关系完整 | 数据操作.需求任务方法.ixx blob 16ac9534；217 行与 F0163 完整身份复核 |
| RCE3038 | R0844 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:244</code> | source-audited-direct | this=&amp;记录身份 | bool进入完整性短路判断 | R0844进入后执行 | 数据操作.需求任务方法.ixx blob 16ac9534；244 行与 R0428 完整身份复核 |
| RCE3039 | R0844 | F0163 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:245</code> | direct-free-overload | 任务与方法两个节点句柄 | 两个bool进入完整性短路判断 | 记录身份当前可读且类型正确 | 数据操作.需求任务方法.ixx blob 16ac9534；245 行与 F0163 完整身份复核 |
| RCE3040 | R0844 | R0830 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:246</code> | source-audited-direct | this分别绑定方法投影关系与发布关系 | 两个bool进入完整性短路判断 | 任务、方法句柄有效且幂等编号非零 | 数据操作.需求任务方法.ixx blob 16ac9534；246 行与 R0830 完整身份复核 |
| RCE3041 | R0844 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:247, 海中鱼巣/领域/数据操作.需求任务方法.ixx:248</code> | direct-free-overload | 来源生命周期关系与来源方法生命周期关系两个关系句柄 | 两个bool进入完整性短路判断 | 两项关系证据当前有效 | 数据操作.需求任务方法.ixx blob 16ac9534；247、248 行与 F0168 完整身份复核 |
| RCE3062 | R0456 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:295, 海中鱼巣/领域/数据操作.需求任务方法.ixx:296</code> | cxx20_rewritten_operator_not_equal | (角色状态,活跃状态)；(角色状态,失效状态)；(活跃状态,失效状态) | 三次bool取反结果参与状态句柄两两不同短路合取 | 三个状态节点句柄均有效 | 冻结源码操作数静态类型、C++20重写规则与F0051完整签名复核 |
| RCE3063 | R0459 | R0402 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3383</code> | direct_const_member | this=&amp;状态数据操作_, 状态节点=当前.目标状态 | 状态值式材料；接收为const auto 目标状态 | 当前.完整目标状态需求()返回true | 冻结源码接收者、实参、局部接收与R0402完整签名复核 |
| RCE3064 | R0459 | R0407 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3384</code> | direct_const_member | this=&amp;目标状态 | bool；作为完整需求匹配短路合取第一项 | 目标状态材料已读取 | 冻结源码接收者静态类型与R0407完整签名复核 |
| RCE3065 | R0459 | R0399 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3385</code> | direct_const_member | this=&amp;规格.目标状态规格_ | std::uint64_t；与目标状态.幂等主键比较 | 目标状态材料为完整抽象状态 | 冻结源码接收者、比较对象与R0399完整签名复核 |
| RCE3066 | R0459 | R0400 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3386</code> | direct_const_member | this=&amp;规格.目标状态规格_ | std::int64_t；与目标状态.状态值比较 | 目标状态幂等主键匹配 | 冻结源码接收者、比较对象与R0400完整签名复核 |
| RCE3067 | R0466 | R0795 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:451</code> | direct_const_member | this=&amp;目标状态规格_ | bool；作为完整需求写入规格短路合取最后一项 | 需求主键非零且五个节点句柄均有效 | 冻结源码接收者静态类型与R0795完整签名复核 |
| RCE3068 | R0467 | R0842 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:890</code> | direct_constructor | 需求主键, 主体, 目标宿主, 场景, 目标特征, 当前特征状态材料, std::move(目标状态规格) | 完整需求写入规格；直接构造局部变量规格 | 进入R0467后无其它前置 | 冻结源码构造表达式、友元可访问性、实参类型与R0842完整签名复核 |
| RCE3069 | R0459 | F0051 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3388, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3389, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3390</code> | overload_resolved_free_operator | (当前.主体,规格.主体_)；(当前.目标宿主,规格.目标宿主_)；(当前.场景,规格.场景_)；(当前.目标特征,规格.目标特征_)；(当前.当前特征状态材料,规格.当前特征状态材料_) | 五次bool结果参与完整需求端点短路合取 | 目标状态材料、幂等主键、状态值和需求主键均匹配 | 冻结源码五组操作数静态类型与F0051完整签名复核 |
| RCE3112 | R0477 | F0051 | <code>海中鱼巣/领域/方法服务.h:1372, 海中鱼巣/领域/方法服务.h:1373, 海中鱼巣/领域/方法服务.h:1380, 海中鱼巣/领域/方法服务.h:1399, 海中鱼巣/领域/方法服务.h:1400, 海中鱼巣/领域/方法服务.h:1412</code> | direct-free-operator | 1372:登记关系源节点与登记根；1373/1380:登记关系目标节点与方法首；1399/1400:生命周期目标与活跃/失效状态；1412:生命周期目标与活跃状态 | 六个 bool 分别进入精确关系判断、循环筛选、允许目标判断或生命周期状态三元选择 | 对应 optional 或循环记录已形成且前序短路条件允许执行该比较 | 方法服务.h blob 839dd462；1362-1424 行、F0051 身份与本批图映射交叉复核 |
| RCE3113 | R0486 | F0051 | <code>海中鱼巣/领域/方法服务.h:1814</code> | direct-free-operator | 方法首节点, 动作入口节点 | bool 经逻辑取反后作为短路合取第三项 | R0479 与 R0485 均返回 true | 方法服务.h blob 839dd462；1814 行和 F0051 身份复核 |
| RCE3114 | R0499 | R0511 | <code>海中鱼巣/领域/服务.方法.ixx:94, 海中鱼巣/领域/服务.方法.ixx:95, 海中鱼巣/领域/服务.方法.ixx:96</code> | source-audited-direct-const-member | 调用1:this=&amp;状态服务_, 请求=角色请求；调用2:this=&amp;状态服务_, 请求=活跃请求；调用3:this=&amp;状态服务_, 请求=失效请求 | 三个抽象状态规格结果分别绑定到角色、活跃、失效局部变量 | R0499 入口四主键检查通过后依次执行 | 服务.方法.ixx blob 74a839fe；94-96 行、R0511 身份与本批图映射交叉复核 |
| RCE3115 | R0499 | R0510 | <code>海中鱼巣/领域/服务.方法.ixx:97</code> | source-audited-direct-const-member | 调用1:this=&amp;角色；调用2:this=&amp;活跃；调用3:this=&amp;失效 | 三个 bool 按从左到右短路顺序决定是否进入规格失败分支 | R0511 的三个规格结果局部变量均已构造 | 服务.方法.ixx blob 74a839fe；97 行、R0510 身份与本批图映射交叉复核 |
| RCE3116 | R0501 | R0876 | <code>海中鱼巣/领域/服务.特征.ixx:113</code> | source-audited-direct-const-member | this=&amp;数据操作_, 主键=请求.幂等主键 | 特征体系业务结果直接作为 R0501 返回值 | 请求.幂等主键非零 | 服务.特征.ixx blob 7620a4c6；111-114 行与数据操作.特征体系.ixx:691-719 交叉复核 |
| RCE3117 | F0029 | R0858 | <code>海中鱼巣/自检.入口初始化.ixx:686-687</code> | callback_bind | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3118 | F0031 | R0858 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3119 | R0858 | R0847 | <code>海中鱼巣/自检.入口初始化.ixx:687</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3120 | R0847 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:490</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3121 | R0847 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:491, 海中鱼巣/自检.入口初始化.ixx:492</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3122 | R0847 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:503-504</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3123 | R0847 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:505, 海中鱼巣/自检.入口初始化.ixx:507</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3124 | R0847 | R0560 | <code>海中鱼巣/自检.入口初始化.ixx:509, 海中鱼巣/自检.入口初始化.ixx:510, 海中鱼巣/自检.入口初始化.ixx:511</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3125 | R0847 | R0561 | <code>海中鱼巣/自检.入口初始化.ixx:512, 海中鱼巣/自检.入口初始化.ixx:513, 海中鱼巣/自检.入口初始化.ixx:514</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3126 | R0847 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:515, 海中鱼巣/自检.入口初始化.ixx:516, 海中鱼巣/自检.入口初始化.ixx:517</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3127 | R0847 | F0516 | <code>海中鱼巣/自检.入口初始化.ixx:545</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3128 | R0847 | R0738 | <code>海中鱼巣/自检.入口初始化.ixx:547</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3129 | R0847 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:548</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3130 | R0847 | R0277 | <code>海中鱼巣/自检.入口初始化.ixx:550</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3131 | R0847 | F0168 | <code>海中鱼巣/自检.入口初始化.ixx:551</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3132 | R0847 | F0579 | <code>海中鱼巣/自检.入口初始化.ixx:552-553</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3133 | R0847 | R0855 | <code>海中鱼巣/自检.入口初始化.ixx:559, 海中鱼巣/自检.入口初始化.ixx:562, 海中鱼巣/自检.入口初始化.ixx:563, 海中鱼巣/自检.入口初始化.ixx:564, 海中鱼巣/自检.入口初始化.ixx:615</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3134 | R0847 | R0856 | <code>海中鱼巣/自检.入口初始化.ixx:566, 海中鱼巣/自检.入口初始化.ixx:567, 海中鱼巣/自检.入口初始化.ixx:568</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3135 | R0847 | R0762 | <code>海中鱼巣/自检.入口初始化.ixx:573-574</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3136 | R0847 | R0763 | <code>海中鱼巣/自检.入口初始化.ixx:575-576</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3137 | R0847 | R0860 | <code>海中鱼巣/自检.入口初始化.ixx:579-581, 海中鱼巣/自检.入口初始化.ixx:582-584</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3138 | R0847 | R0862 | <code>海中鱼巣/自检.入口初始化.ixx:585-587</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3139 | R0847 | R0861 | <code>海中鱼巣/自检.入口初始化.ixx:589-590, 海中鱼巣/自检.入口初始化.ixx:591-592</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3140 | R0847 | R0863 | <code>海中鱼巣/自检.入口初始化.ixx:593-594</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3141 | R0847 | R0859 | <code>海中鱼巣/自检.入口初始化.ixx:596</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3142 | R0847 | R0868 | <code>海中鱼巣/自检.入口初始化.ixx:608-609, 海中鱼巣/自检.入口初始化.ixx:621-623</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3143 | R0847 | R0869 | <code>海中鱼巣/自检.入口初始化.ixx:616-618</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3144 | R0847 | F0163 | <code>海中鱼巣/自检.入口初始化.ixx:619</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3145 | R0847 | R0870 | <code>海中鱼巣/自检.入口初始化.ixx:629</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3146 | R0847 | R0857 | <code>海中鱼巣/自检.入口初始化.ixx:641-643</code> | standard-library-callback | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3147 | R0847 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:483-653</code> | compiler_implicit_member_destructor | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3148 | F0015 | R0848 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3149 | F0015 | R0849 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3150 | F0015 | R0850 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3151 | F0015 | R0851 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3152 | F0015 | R0852 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3153 | F0015 | R0853 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3154 | R0848 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:496</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3155 | R0849 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:497</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3156 | R0850 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:498</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3157 | R0851 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:499</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3158 | R0852 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:500</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3159 | R0853 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:501</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3160 | R0855 | R0854 | <code>海中鱼巣/自检.入口初始化.ixx:528</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3161 | R0856 | R0854 | <code>海中鱼巣/自检.入口初始化.ixx:531</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3162 | R0859 | R0559 | <code>海中鱼巣/领域/特征服务.h:277</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3163 | R0859 | F0621 | <code>海中鱼巣/领域/特征服务.h:280</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3164 | R0859 | R0561 | <code>海中鱼巣/领域/特征服务.h:282</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3165 | R0859 | R0779 | <code>海中鱼巣/领域/特征服务.h:282</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3166 | R0860 | R0558 | <code>海中鱼巣/领域/特征服务.h:374</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3167 | R0860 | R0864 | <code>海中鱼巣/领域/特征服务.h:380</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3168 | R0861 | R0558 | <code>海中鱼巣/领域/特征服务.h:386</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3169 | R0861 | R0865 | <code>海中鱼巣/领域/特征服务.h:389</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3170 | R0862 | R0558 | <code>海中鱼巣/领域/特征服务.h:396</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3171 | R0862 | R0866 | <code>海中鱼巣/领域/特征服务.h:402</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3172 | R0863 | R0558 | <code>海中鱼巣/领域/特征服务.h:408</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3173 | R0863 | R0867 | <code>海中鱼巣/领域/特征服务.h:411</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3174 | R0864 | F0321 | <code>海中鱼巣/领域/特征值服务.h:291</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3175 | R0864 | F0336 | <code>海中鱼巣/领域/特征值服务.h:294</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3176 | R0864 | F0398 | <code>海中鱼巣/领域/特征值服务.h:295</code> | resolved-function-pointer | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3177 | R0864 | F0338 | <code>海中鱼巣/领域/特征值服务.h:296</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3178 | R0864 | F0339 | <code>海中鱼巣/领域/特征值服务.h:297</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3179 | R0864 | R0731 | <code>海中鱼巣/领域/特征值服务.h:299</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3180 | R0864 | R0543 | <code>海中鱼巣/领域/特征值服务.h:300</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3181 | R0864 | R0732 | <code>海中鱼巣/领域/特征值服务.h:310, 海中鱼巣/领域/特征值服务.h:336</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3182 | R0864 | R0546 | <code>海中鱼巣/领域/特征值服务.h:321</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3183 | R0864 | F0184 | <code>海中鱼巣/领域/特征值服务.h:322-324, 海中鱼巣/领域/特征值服务.h:337-344</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3184 | R0864 | F0345 | <code>海中鱼巣/领域/特征值服务.h:292-345</code> | compiler_implicit_destructor | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3185 | R0865 | F0321 | <code>海中鱼巣/领域/特征值服务.h:348</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3186 | R0865 | F0336 | <code>海中鱼巣/领域/特征值服务.h:351</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3187 | R0865 | F0397 | <code>海中鱼巣/领域/特征值服务.h:352</code> | resolved-function-pointer | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3188 | R0865 | F0338 | <code>海中鱼巣/领域/特征值服务.h:353</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3189 | R0865 | F0339 | <code>海中鱼巣/领域/特征值服务.h:354</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3190 | R0865 | R0731 | <code>海中鱼巣/领域/特征值服务.h:356</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3191 | R0865 | R0732 | <code>海中鱼巣/领域/特征值服务.h:362</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3192 | R0865 | F0345 | <code>海中鱼巣/领域/特征值服务.h:349-367</code> | compiler_implicit_destructor | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3193 | R0866 | F0321 | <code>海中鱼巣/领域/特征值服务.h:370</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3194 | R0866 | F0336 | <code>海中鱼巣/领域/特征值服务.h:373</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3195 | R0866 | F0398 | <code>海中鱼巣/领域/特征值服务.h:374</code> | resolved-function-pointer | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3196 | R0866 | F0338 | <code>海中鱼巣/领域/特征值服务.h:375</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3197 | R0866 | F0339 | <code>海中鱼巣/领域/特征值服务.h:376</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3198 | R0866 | R0731 | <code>海中鱼巣/领域/特征值服务.h:378</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3199 | R0866 | R0543 | <code>海中鱼巣/领域/特征值服务.h:379</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3200 | R0866 | R0732 | <code>海中鱼巣/领域/特征值服务.h:389, 海中鱼巣/领域/特征值服务.h:415</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3201 | R0866 | R0546 | <code>海中鱼巣/领域/特征值服务.h:400</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3202 | R0866 | F0184 | <code>海中鱼巣/领域/特征值服务.h:401-403, 海中鱼巣/领域/特征值服务.h:416-423</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3203 | R0866 | F0345 | <code>海中鱼巣/领域/特征值服务.h:371-424</code> | compiler_implicit_destructor | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3204 | R0867 | F0321 | <code>海中鱼巣/领域/特征值服务.h:427</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3205 | R0867 | F0336 | <code>海中鱼巣/领域/特征值服务.h:430</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3206 | R0867 | F0397 | <code>海中鱼巣/领域/特征值服务.h:431</code> | resolved-function-pointer | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3207 | R0867 | F0338 | <code>海中鱼巣/领域/特征值服务.h:432</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3208 | R0867 | F0339 | <code>海中鱼巣/领域/特征值服务.h:433</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3209 | R0867 | R0731 | <code>海中鱼巣/领域/特征值服务.h:435</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3210 | R0867 | R0732 | <code>海中鱼巣/领域/特征值服务.h:441</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3211 | R0867 | F0345 | <code>海中鱼巣/领域/特征值服务.h:428-446</code> | compiler_implicit_destructor | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3212 | R0868 | R0874 | <code>海中鱼巣/领域/状态服务.h:227</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3213 | R0868 | F0621 | <code>海中鱼巣/领域/状态服务.h:230</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3214 | R0869 | R0874 | <code>海中鱼巣/领域/状态服务.h:133</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3215 | R0869 | F0626 | <code>海中鱼巣/领域/状态服务.h:134</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3216 | R0869 | F0335 | <code>海中鱼巣/领域/状态服务.h:137</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3217 | R0869 | F0163 | <code>海中鱼巣/领域/状态服务.h:138</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3218 | R0869 | F0168 | <code>海中鱼巣/领域/状态服务.h:144</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3219 | R0869 | F0184 | <code>海中鱼巣/领域/状态服务.h:138-145, 海中鱼巣/领域/状态服务.h:149-151</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3220 | R0869 | F0167 | <code>海中鱼巣/领域/状态服务.h:141, 海中鱼巣/领域/状态服务.h:142, 海中鱼巣/领域/状态服务.h:143</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3221 | R0869 | R0870 | <code>海中鱼巣/领域/状态服务.h:148</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3222 | R0870 | F0328 | <code>海中鱼巣/领域/状态服务.h:240</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3223 | R0870 | R0872 | <code>海中鱼巣/领域/状态服务.h:243</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3224 | R0870 | R0873 | <code>海中鱼巣/领域/状态服务.h:244</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3225 | R0870 | F0329 | <code>海中鱼巣/领域/状态服务.h:245</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3226 | R0870 | R0871 | <code>海中鱼巣/领域/状态服务.h:246</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3227 | R0871 | F0190 | <code>海中鱼巣/领域/状态服务.h:194</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3228 | R0871 | F0217 | <code>海中鱼巣/领域/状态服务.h:198</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3229 | R0872 | F0328 | <code>海中鱼巣/领域/状态服务.h:212</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3230 | R0872 | R0875 | <code>海中鱼巣/领域/状态服务.h:215</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3231 | R0873 | F0328 | <code>海中鱼巣/领域/状态服务.h:219</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3232 | R0873 | R0875 | <code>海中鱼巣/领域/状态服务.h:222</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3233 | R0874 | F0190 | <code>海中鱼巣/领域/状态服务.h:313</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3234 | R0875 | F0621 | <code>海中鱼巣/领域/状态服务.h:318</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3235 | R0875 | R0874 | <code>海中鱼巣/领域/状态服务.h:321</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3236 | R0876 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:692</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3237 | R0876 | R0765 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:693</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3238 | R0876 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:694</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3239 | R0876 | R0877 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:694, 海中鱼巣/领域/数据操作.特征体系.ixx:710</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3240 | R0876 | R0367 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:700</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3241 | R0876 | R0116 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:702-707</code> | callback_bind | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3242 | R0876 | R0368 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:718</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3243 | R0116 | R0878 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:702-707</code> | callback_invoke_resolved | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3244 | R0878 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:705</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3245 | R0878 | R0041 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:706</code> | source-audited-direct | 按列示调用点的当前源码实参绑定 | 被调结果按源码用于局部绑定、条件判断、回调或直接返回 | 调用方当前可达且列示源码分支条件成立 | 当前源码 blob、正式身份表与递归闭包逐调用点复核 |
| RCE3246 | R0502 | F0051 | <code>海中鱼巣/领域/服务.特征.ixx:119</code> | operator | 宿主=请求.宿主, 特征定义=请求.特征定义 | 不相等 bool | 前置句柄校验短路求值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3247 | R0502 | R0383 | <code>海中鱼巣/领域/服务.特征.ixx:120</code> | direct_member | this=&amp;数据操作_, 目标=请求.宿主 | 特征节点身份 宿主 | 入口参数校验通过 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3248 | R0502 | R0388 | <code>海中鱼巣/领域/服务.特征.ixx:122</code> | direct_const_member | this=&amp;宿主 | 当前可读 bool | 宿主材料已读取 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3249 | R0508 | R0374 | <code>海中鱼巣/领域/服务.特征.ixx:85</code> | direct_const_member | this=规格.operator-&gt;() | 规格完整 bool | 状态已形成且 optional 有值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3250 | R0509 | R0380 | <code>海中鱼巣/领域/服务.特征.ixx:94</code> | direct_const_member | this=规格.operator-&gt;() | 规格完整 bool | 状态已形成且 optional 有值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3251 | R0510 | R0795 | <code>海中鱼巣/领域/服务.状态.ixx:52</code> | direct_const_member | this=规格.operator-&gt;() | 规格完整 bool | 状态已形成且 optional 有值 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3252 | R0511 | R0879 | <code>海中鱼巣/领域/服务.状态.ixx:83</code> | direct_const_member | this=&amp;数据操作_, 幂等主键=请求.幂等主键, 状态值=请求.状态值 | optional&lt;抽象状态写入规格&gt; 规格 | 幂等主键非零 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3253 | R0879 | R0880 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:331</code> | direct_constructor | 幂等主键, 状态值 | 抽象状态写入规格 规格 | 调用方可达 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3254 | R0879 | R0795 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:332</code> | direct_const_member | this=&amp;规格 | 完整 bool | 规格已构造 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3255 | R0524 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3946, 海中鱼巣/领域/概念图服务.h:3957</code> | direct_member | this=&amp;关系_, 关系=已登记-&gt;关系 / 新关系 | optional&lt;关系记录&gt; 记录 | 已有关系读回或新建关系读回 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3256 | R0524 | F0167 | <code>海中鱼巣/领域/概念图服务.h:3956</code> | direct_member | this=&amp;关系_, 类型, 源节点, 目标节点, 顺序号=0 | 关系句柄 新关系 | 关系尚不存在 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3257 | R0524 | R0881 | <code>海中鱼巣/领域/概念图服务.h:3936-3940</code> | standard_algorithm_callback | 当前专用关系登记材料；捕获类型和两个节点 | find_if 谓词 bool | std::find_if 遍历期间零到多次同步调用 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3258 | R0881 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3939, 海中鱼巣/领域/概念图服务.h:3939</code> | operator | 材料.源节点==源节点；材料.目标节点==目标节点 | 两项短路相等 bool | find_if 谓词执行 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3259 | R0525 | R0518 | <code>海中鱼巣/领域/概念图服务.h:4383</code> | standard_algorithm_callback | 左,右=下位组排序元素 | 严格小于 bool | std::sort 同步比较期间零到多次调用 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3260 | R0525 | F0051 | <code>海中鱼巣/领域/概念图服务.h:4339, 海中鱼巣/领域/概念图服务.h:4364, 海中鱼巣/领域/概念图服务.h:4384, 海中鱼巣/领域/概念图服务.h:4386</code> | standard_algorithm_callback | 节点句柄元素与查找值/相邻元素 | 相等 bool | 三处 std::find 和一处 std::unique 同步比较 | 本批现状单函数图、逐行映射与源码调用点交叉复核 |
| RCE3261 | R0530 | R0528 | <code>海中鱼巣/领域/概念图服务.h:762</code> | direct_const_member | this=视图.operator-&gt;() | 视图完整 bool，参与正常成功短路返回 | 状态为已生成且视图有值 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3262 | R0531 | F0377 | <code>海中鱼巣/领域/概念图服务.h:917</code> | direct_const_member | this=&amp;节点_; 节点 -&gt; 节点句柄 | 节点有效 bool，控制入口拒绝 | R0531 入口执行 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3263 | R0531 | R0882 | <code>海中鱼巣/领域/概念图服务.h:921</code> | standard-library-callback | 根登记组元素 -&gt; 材料；捕获节点引用 | 谓词 bool 交给 std::any_of | 节点有效且共享锁已取得 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3264 | R0532 | R0883 | <code>海中鱼巣/领域/概念图算法.h:225</code> | direct_const_member | this=&amp;输入 | 输入完整 bool，控制空 optional 返回 | R0532 入口执行 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3265 | R0532 | R0886 | <code>海中鱼巣/领域/概念图算法.h:229</code> | standard-library-callback | 约束组元素对 -&gt; 左,右 | 严格小于 bool 交给 std::sort | 输入完整且局部结果已复制 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3266 | R0534 | F0051 | <code>海中鱼巣/领域/概念图算法.h:628</code> | operator | 左.定义节点, 右.定义节点 | 节点句柄相等 bool，参与约束身份合取 | 维度与角色序号均相等 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3267 | R0537 | R0886 | <code>海中鱼巣/领域/概念图算法.h:693</code> | standard-library-callback | 概念约束元素对 -&gt; 左,右 | 严格小于 bool 交给 std::includes | 类别和签名版本相同且两个签名不相同 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3268 | R0538 | F0051 | <code>海中鱼巣/领域/概念活动状态.数据.h:110, 海中鱼巣/领域/概念活动状态.数据.h:111</code> | operator | 活跃关系.源节点, 根身份.节点；活跃关系.目标节点, 活跃角色.状态节点 | 两次节点句柄相等 bool，依次参与完整性短路判断 | 前序根身份、角色、用途、类型、签名与关系完整性均成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3269 | R0539 | F0466 | <code>海中鱼巣/领域/概念活动状态.数据.h:202</code> | direct_const_member | this=&amp;视图 | 视图完整 bool，参与成功性返回 | 状态为幂等读回 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3270 | R0883 | R0884 | <code>海中鱼巣/领域/概念图算法.h:93</code> | standard-library-callback | 约束组元素 -&gt; 约束 | 约束完整 bool 交给 std::all_of | 类别范围与签名版本前置成立 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3271 | R0884 | R0885 | <code>海中鱼巣/领域/概念图算法.h:94</code> | direct_const_member | this=&amp;约束 | 约束完整 bool，作为 all_of 谓词结果 | std::all_of 调度当前约束 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3272 | R0885 | F0163 | <code>海中鱼巣/领域/概念图算法.h:63, 海中鱼巣/领域/概念图算法.h:77</code> | direct_free_overload | 定义节点 -&gt; const 节点句柄&amp; | 节点句柄有效 bool，参与维度或稳定句柄完整性判断 | 特征维度定义检查；或值类型为稳定句柄 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3273 | R0886 | R0887 | <code>海中鱼巣/领域/概念图算法.h:647, 海中鱼巣/领域/概念图算法.h:650</code> | direct_static_member | 左.定义节点,右.定义节点；右.定义节点,左.定义节点 | 节点句柄严格小于 bool，决定定义节点排序 | 维度与角色序号相同 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3274 | R0558 | F0579 | <code>海中鱼巣/领域/特征服务.h:1013</code> | source-audited-direct | this=&amp;关系_；类型=归属；源=特征节点；目标=特征值节点 | bool存在关系结果参与短路返回 | 关系仓库已接线且两个节点类型匹配 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3275 | R0559 | F0190 | <code>海中鱼巣/领域/特征服务.h:1210</code> | source-audited-direct | this=&amp;节点_；节点=节点句柄值 | optional节点记录绑定为记录 | R0559入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3276 | R0562 | R0888 | <code>海中鱼巣/领域/特征服务.h:360</code> | source-audited-direct | this=特征值_；特征值节点；值 | bool写入结果直接返回 | 归属成立且特征值服务已接线 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3277 | R0563 | F0163 | <code>海中鱼巣/领域/系统角色清单.数据.h:118</code> | source-audited-direct | 句柄=节点 | bool节点句柄有效参与短路 | 用途、主键和类型均有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3278 | R0563 | F0565 | <code>海中鱼巣/领域/系统角色清单.数据.h:118</code> | source-audited-direct | 句柄=主信息 | bool主信息句柄有效参与短路 | 节点句柄有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3279 | R0570 | F0051 | <code>海中鱼巣/领域/语素服务.h:271</code> | source-audited-direct | 左=节点值；右=目标 | bool相等结果决定命中 | 范围循环每个元素 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3280 | R0573 | F0163 | <code>海中鱼巣/领域/语素服务.h:341</code> | source-audited-direct | 句柄=结果.语素入口 | bool节点句柄有效 | 创建语素入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3281 | R0573 | F0565 | <code>海中鱼巣/领域/语素服务.h:336</code> | source-audited-direct | 句柄=结果.主信息 | bool主信息句柄有效 | 创建主信息后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3282 | R0573 | F0167 | <code>海中鱼巣/领域/语素服务.h:346</code> | source-audited-direct | this=&amp;关系_；类型=语素对应信息；源=语素入口；目标=对应信息节点 | 对应关系句柄写入结果.对应关系 | 语素入口有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3283 | R0573 | R0889 | <code>海中鱼巣/领域/语素服务.h:353</code> | source-audited-direct | this=&amp;索引_；主键；节点=结果.语素入口 | bool写入结果.主键由本次绑定 | 主键非零 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3284 | R0573 | F0051 | <code>海中鱼巣/领域/语素服务.h:362</code> | source-audited-direct | 左=按主键查节点结果.value；右=结果.语素入口 | bool主键节点一致性参与读回 | 主键非零且查询有值 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3285 | R0574 | F0163 | <code>海中鱼巣/领域/语素服务.h:382</code> | source-audited-direct | 句柄=结果.语素入口 | bool节点句柄有效 | 清理语素入口前 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3286 | R0574 | F0565 | <code>海中鱼巣/领域/语素服务.h:385</code> | source-audited-direct | 句柄=结果.主信息 | bool主信息句柄有效 | 清理主信息前 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3287 | R0574 | F0051 | <code>海中鱼巣/领域/语素服务.h:375</code> | source-audited-direct | 左=当前节点.value；右=结果.语素入口 | bool一致性决定是否删除主键 | 当前主键查询有值 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3288 | R0888 | F0321 | <code>海中鱼巣/领域/特征值服务.h:213</code> | source-audited-direct | this=&amp;事务接线_ | bool接线形态有效 | R0888入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3289 | R0888 | F0336 | <code>海中鱼巣/领域/特征值服务.h:216</code> | source-audited-direct | this=&amp;事务接线_ | bool是否已接域 | 接线形态有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3290 | R0888 | F0398 | <code>海中鱼巣/领域/特征值服务.h:217</code> | source-audited-direct | this=&amp;事务接线_；状态=运行期状态 | 结构事务许可赋给许可 | 事务接线已接域 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3291 | R0888 | F0338 | <code>海中鱼巣/领域/特征值服务.h:218</code> | source-audited-direct | this=&amp;许可 | bool许可有效 | 取得独占许可后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3292 | R0888 | F0339 | <code>海中鱼巣/领域/特征值服务.h:219</code> | source-audited-direct | this=&amp;许可 | 结构事务令牌地址写入令牌 | 许可有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3293 | R0888 | R0731 | <code>海中鱼巣/领域/特征值服务.h:221</code> | source-audited-direct | this；特征值节点；令牌 | optional节点记录 | 可选事务令牌已确定 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3294 | R0888 | R0732 | <code>海中鱼巣/领域/特征值服务.h:227, 海中鱼巣/领域/特征值服务.h:259</code> | source-audited-direct | this；特征值节点；主信息=节点记录值-&gt;主信息；令牌 | 写入前状态与写入后状态 | 持有原始值写锁 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3295 | R0888 | R0550 | <code>海中鱼巣/领域/特征值服务.h:241</code> | source-audited-direct | this；特征值节点 | I64版本记录迭代器 | 写入前状态允许I64写入 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3296 | R0888 | F0184 | <code>海中鱼巣/领域/特征值服务.h:242-245, 海中鱼巣/领域/特征值服务.h:250-251, 海中鱼巣/领域/特征值服务.h:260-265</code> | source-audited-direct | 条件；诊断文本 | bool追根因检查结果 | 版本对应、主信息写入与写后读回检查 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3297 | R0888 | R0010 | <code>海中鱼巣/领域/特征值服务.h:248</code> | source-audited-direct | this=&amp;主信息_；主信息；槽位=0；值；令牌 | bool已写入 | 存在结构事务令牌 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3298 | R0888 | F0627 | <code>海中鱼巣/领域/特征值服务.h:249</code> | source-audited-direct | this=&amp;主信息_；主信息；值 | bool已写入 | 不存在结构事务令牌 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3299 | R0888 | F0345 | <code>海中鱼巣/领域/特征值服务.h:212-266</code> | source-audited-direct | this=&amp;许可 | 离开函数时释放结构事务许可 | R0888作用域结束 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3300 | R0889 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:103</code> | source-audited-direct | this=&amp;事务接线_ | bool是否已接域 | R0889入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3301 | R0889 | F0397 | <code>海中鱼巣/核心/索引仓库.cpp:104</code> | source-audited-direct | this=&amp;事务接线_；状态=运行期状态 | 结构事务许可赋给许可 | 事务接线已接域 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3302 | R0889 | F0338 | <code>海中鱼巣/核心/索引仓库.cpp:105</code> | source-audited-direct | this=&amp;许可 | bool许可有效参与短路 | 取得共享许可后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3303 | R0889 | F0339 | <code>海中鱼巣/核心/索引仓库.cpp:105</code> | source-audited-direct | this=&amp;许可 | 结构事务令牌传给R0890 | 许可有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3304 | R0889 | R0890 | <code>海中鱼巣/核心/索引仓库.cpp:105</code> | source-audited-direct | this；主键；节点；令牌=许可.读取令牌 | bool结果直接返回 | 事务接线已接域且许可有效 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3305 | R0889 | F0345 | <code>海中鱼巣/核心/索引仓库.cpp:102-111</code> | source-audited-direct | this=&amp;许可 | 离开已接域分支时释放结构事务许可 | 许可局部对象作用域结束 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3306 | R0889 | F0377 | <code>海中鱼巣/核心/索引仓库.cpp:107</code> | source-audited-direct | this=&amp;节点_；节点 | bool节点有效参与入口短路 | 未接域且主键非零 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3307 | R0889 | R0122 | <code>海中鱼巣/核心/索引仓库.cpp:109</code> | source-audited-direct | 主键索引_；节点主键组_；永久保留主键组_；请求 | bool绑定结果直接返回 | 未接域入口有效并持有仓库写锁 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3308 | R0889 | R0891 | <code>海中鱼巣/核心/索引仓库.cpp:110</code> | source-audited-direct | 最终物理键=主键；目标=节点 | 索引绑定请求传给R0122 | 持有仓库写锁 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3309 | R0890 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:114</code> | source-audited-direct | 接线=事务接线_；令牌 | bool令牌有效参与短路 | R0890入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3310 | R0890 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:114</code> | source-audited-direct | this=&amp;节点_；节点；令牌 | bool节点有效参与短路 | 令牌有效且主键非零 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3311 | R0890 | R0122 | <code>海中鱼巣/核心/索引仓库.cpp:116</code> | source-audited-direct | 主键索引_；节点主键组_；永久保留主键组_；请求 | bool绑定结果直接返回 | 入口有效并持有仓库写锁 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3312 | R0890 | R0891 | <code>海中鱼巣/核心/索引仓库.cpp:117</code> | source-audited-direct | 最终物理键=主键；目标=节点 | 索引绑定请求传给R0122 | 持有仓库写锁 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3313 | R0891 | R0133 | <code>海中鱼巣/核心/索引所有权.数据.h:125</code> | source-audited-direct | 无 | 未知兼容索引所有者声明写入请求 | R0891形成聚合返回值 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3314 | R0579 | F0165 | <code>海中鱼巣/领域/需求服务.h:306</code> | source-audited-direct | this=&amp;状态；状态值=目标状态值 | 目标状态节点句柄 | 入口节点类型和当前特征值检查通过 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3315 | R0579 | F0163 | <code>海中鱼巣/领域/需求服务.h:307, 海中鱼巣/领域/需求服务.h:311</code> | source-audited-direct | 目标状态节点句柄；需求节点句柄 | bool分别传给追根因检查 | 对应创建调用返回后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3316 | R0579 | F0167 | <code>海中鱼巣/领域/需求服务.h:314, 海中鱼巣/领域/需求服务.h:316, 海中鱼巣/领域/需求服务.h:317, 海中鱼巣/领域/需求服务.h:318, 海中鱼巣/领域/需求服务.h:319, 海中鱼巣/领域/需求服务.h:320</code> | source-audited-direct | this=&amp;关系_；引用/模板关系类型；需求、自我存在、场景、特征、当前值或目标状态端点；对应顺序号或默认0 | 六个关系句柄 | 需求节点创建成功 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3317 | R0580 | F0400 | <code>海中鱼巣/领域/需求服务.h:349</code> | source-audited-resolved-function-pointer | 接线_.运行期状态；令牌 | bool参与短路 | 接线_.已接域()为真 | 本批逐函数源码、接线注册点、函数指针静态签名与当前装配复核 |
| RCE3318 | R0583 | F0051 | <code>海中鱼巣/领域/需求服务.h:528</code> | source-audited-direct | 左=记录.目标节点；右=需求节点 | bool参与关系记录匹配 | 普通目标宿主缺失且自我主体可读 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3319 | R0587 | R0871 | <code>海中鱼巣/领域/需求服务.h:716</code> | source-audited-direct | this=&amp;状态；状态节点=记录.目标节点 | 发生时间戳可选值 | 关系记录指向完整结算记录 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3320 | R0587 | F0190 | <code>海中鱼巣/领域/需求服务.h:717</code> | source-audited-direct | this=&amp;节点_；节点=记录.目标节点 | 节点记录可选值 | 关系记录指向完整结算记录 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3321 | R0587 | F0051 | <code>海中鱼巣/领域/需求服务.h:732</code> | source-audited-direct | 左=记录.目标节点；右=最近关系-&gt;目标节点 | bool取反后参与无法区分判断 | 已有最近关系且时间戳、顺序号、创建序号均相同 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3322 | R0593 | F0051 | <code>海中鱼巣/领域/需求服务.h:773</code> | source-audited-direct | 左=父需求；右=需求节点 | bool参与自环拒绝判断 | 父需求句柄有效且父需求类型匹配 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3323 | R0594 | F0346 | <code>海中鱼巣/领域/需求服务.h:816</code> | source-audited-direct | this=&amp;节点_；节点=节点句柄值；令牌 | 节点记录可选值 | R0594入口后 | 本批逐函数源码静态类型、完整签名、调用点与当前 Debug\|x64 可达闭包复核 |
| RCE3324 | R0630 | F0051 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:473, 海中鱼巣/领域/数据操作.存在场景.ixx:474</code> | operator | 左=当前关系-&gt;源节点/目标节点；右=节点/引用.目标节点 | bool参与来源关系完整句柄权威读回一致性判断 | 当前关系有值且关系类型为引用 | 本批逐函数源码静态类型、完整签名与调用点复核 |
| RCE3325 | R0696 | R0684 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:162, 海中鱼巣/领域/数据操作.轻量因果.ixx:164</code> | direct_const_member | this=&amp;规格 | 幂等主键值分别传入绑定请求与绑定匹配读回 | 轻量因果结构写入回调已形成节点候选 | 本批逐函数源码函数范围、静态接收者、完整签名与调用点拆账复核 |
| RCE3326 | R0730 | R0599 | <code>海中鱼巣/领域/特征值服务.h:270</code> | constructor | 无 | 局部结构事务许可 许可 | 进入R0730后默认构造许可对象 | 本批现状图、逐行映射与源码生命周期逐调用点复核 |
| RCE3327 | R0730 | R0600 | <code>海中鱼巣/领域/特征值服务.h:273</code> | direct-operator | this=&amp;许可, 其它=事务接线_.取得共享许可()临时 | 许可覆盖为取得的共享许可 | 事务接线已接域并取得共享许可 | 本批现状图、逐行映射与源码生命周期逐调用点复核 |
| RCE3328 | R0745 | R0892 | <code>海中鱼巣/核心/节点仓库.cpp:105</code> | constructor | 其它=std::move(*候选) | 结果.值内的节点未发布候选 | 节点未发布候选存在并写入已提交结果 | 本批现状图、逐行映射与源码显式特殊成员调用复核 |
| RCE3329 | R0746 | R0893 | <code>海中鱼巣/核心/节点仓库.cpp:122</code> | constructor | 仓库=this, 令牌=令牌, 节点={仓库编号_, 编号, 记录.版本号} | 节点未发布候选临时量 | 节点已插入且版本存在，形成未发布候选返回值 | 本批现状图、逐行映射与源码构造表达式复核 |
| RCE3330 | R0746 | R0892 | <code>海中鱼巣/核心/节点仓库.cpp:122</code> | constructor | 其它=R0893形成的节点未发布候选临时量 | 返回optional内的节点未发布候选值 | R0893候选构造完成并转换为optional返回值 | 本批现状图、逐行映射与源码返回值生命周期复核 |
