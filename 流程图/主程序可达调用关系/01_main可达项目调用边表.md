# main 可达项目调用边表

项目直接调用边：2778。多调用点保留在同一 caller/callee 边记录中。

| 边 ID | 调用方 | 被调方 | 调用点 | 类别 | 实参 | 可达条件 | 解析来源 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| E0002 | F0001 | F0003 | <code>海中鱼巣/入口.cpp:10</code> | direct_const_member | <code>this=&解析</code> | after E0001 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0003 | F0001 | F0004 | <code>海中鱼巣/入口.cpp:11, 海中鱼巣/入口.cpp:14</code> | direct_free_function+unique_name | <code>解析, nullptr</code> | 解析成功 == false；after E0004；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0004 | F0001 | F0005 | <code>海中鱼巣/入口.cpp:13</code> | direct_free_function+unique_name | <code>解析.选项</code> | 解析成功 == true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0001 | F0001 | F0002 | <code>海中鱼巣/入口.cpp:9</code> | direct_free_function+unique_name | <code>参数数量, 参数组</code> | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0006 | F0004 | F0003 | <code>海中鱼巣/启动.程序入口.ixx:74</code> | direct_const_member | <code>this=&解析</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0007 | F0004 | F0006 | <code>海中鱼巣/启动.程序入口.ixx:80</code> | direct_const_member | <code>this=运行</code> | 解析已接受 && 运行 != nullptr | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0008 | F0005 | F0007 | <code>海中鱼巣/启动.应用程序.ixx:448</code> | direct_free_function | <code>选项.模式</code> | 普通控制面板 \|\| 无窗口常驻 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0009 | F0005 | F0008 | <code>海中鱼巣/启动.应用程序.ixx:450</code> | direct_free_function | <code>未单独冻结；读取源码调用点</code> | 生产运行期 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0010 | F0005 | F0009 | <code>海中鱼巣/启动.应用程序.ixx:452</code> | direct_free_function | <code>未单独冻结；读取源码调用点</code> | 完整自检 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0011 | F0005 | F0010 | <code>海中鱼巣/启动.应用程序.ixx:454</code> | direct_free_function | <code>覆盖端口=nullptr</code> | 数据库专项 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0012 | F0005 | F0011 | <code>海中鱼巣/启动.应用程序.ixx:456</code> | direct_free_function | <code>未单独冻结；读取源码调用点</code> | 关系仓库性能专项 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0022 | F0007 | F0021 | <code>海中鱼巣/启动.应用程序.ixx:101</code> | direct_free_function+unique_name | <code>*装配.上下文, *信号.租约</code> | 信号成功 && 无窗口常驻；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0026 | F0007 | F0025 | <code>海中鱼巣/启动.应用程序.ixx:102, 海中鱼巣/启动.应用程序.ixx:107, 海中鱼巣/启动.应用程序.ixx:110</code> | compiler_implicit_destructor | <code>this=信号.租约.get()</code> | 无窗口宿主返回后；投影失败返回前；控制面板宿主返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0023 | F0007 | F0022 | <code>海中鱼巣/启动.应用程序.ixx:103</code> | direct_free_function+unique_name | <code>*装配.上下文, 初始化</code> | 信号成功 && 普通控制面板；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0024 | F0007 | F0023 | <code>海中鱼巣/启动.应用程序.ixx:104</code> | direct_const_member | <code>this=&投影</code> | after E0023 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0025 | F0007 | F0024 | <code>海中鱼巣/启动.应用程序.ixx:108</code> | direct_free_function+unique_name | <code>上下文, 初始化, 投影, 审计, 信号租约</code> | 投影成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0014 | F0007 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:51</code> | direct_free_function+unique_name | <code>配置</code> | 普通模式有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0015 | F0007 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:52</code> | direct_const_member | <code>this=&装配</code> | after E0014 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0001 | F0007 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:59</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0002 | F0007 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:60</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0003 | F0007 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:61</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0016 | F0007 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:64</code> | direct_free_function+unique_name | <code>系统端口, 初始化请求</code> | 装配成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0017 | F0007 | F0016 | <code>海中鱼巣/启动.应用程序.ixx:66</code> | direct_const_member | <code>this=&初始化</code> | after E0016 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0018 | F0007 | F0017 | <code>海中鱼巣/启动.应用程序.ixx:67</code> | lambda_direct | <code>捕获初始化</code> | 初始化失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0004 | F0007 | F0053 | <code>海中鱼巣/启动.应用程序.ixx:82</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0005 | F0007 | F0054 | <code>海中鱼巣/启动.应用程序.ixx:87</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0019 | F0007 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:95</code> | direct_free_function+unique_name | <code>审计端口, 最佳努力</code> | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0020 | F0007 | F0019 | <code>海中鱼巣/启动.应用程序.ixx:96</code> | direct_free_function+unique_name | <code>默认安装函数</code> | after E0019；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0021 | F0007 | F0020 | <code>海中鱼巣/启动.应用程序.ixx:97</code> | direct_const_member | <code>this=&信号</code> | after E0020 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0029 | F0008 | F0026 | <code>海中鱼巣/启动.应用程序.ixx:37</code> | direct_constructor | <code>未单独冻结；读取源码调用点</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0030 | F0008 | F0027 | <code>海中鱼巣/启动.应用程序.ixx:38</code> | direct_member | <code>this=&会话, 生产运行期启动请求{}</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0031 | F0008 | F0028 | <code>海中鱼巣/启动.应用程序.ixx:39</code> | direct_const_member | <code>this=&结果</code> | after E0030 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0032 | F0009 | F0029 | <code>海中鱼巣/启动.应用程序.ixx:381</code> | direct_free_function+unique_name | <code>运行器, 空配置</code> | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0033 | F0009 | F0030 | <code>海中鱼巣/启动.应用程序.ixx:382-383, 海中鱼巣/启动.应用程序.ixx:384-385</code> | direct_member | <code>this=&运行器, 150, F27编号名称, 回调</code> | 入口登记成功；入口登记成功 && 端口登记成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0036 | F0009 | F0032 | <code>海中鱼巣/启动.应用程序.ixx:383</code> | callback_bind+lsp_direct | <code>无捕获转发回调</code> | 端口自检登记；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0037 | F0009 | F0033 | <code>海中鱼巣/启动.应用程序.ixx:385</code> | callback_bind+lsp_direct | <code>无捕获转发回调</code> | 隔离自检登记；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0035 | F0009 | F0031 | <code>海中鱼巣/启动.应用程序.ixx:390</code> | direct_member+unique_name | <code>this=&运行器</code> | 三段登记均成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0040 | F0010 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:114</code> | direct_free_function+unique_name | <code>配置</code> | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0041 | F0010 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:115</code> | direct_const_member | <code>this=&装配</code> | after E0040 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0006 | F0010 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:123</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0007 | F0010 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:124</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0008 | F0010 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:125</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0042 | F0010 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:128, 海中鱼巣/启动.应用程序.ixx:128-129</code> | direct_free_function+unique_name | <code>系统端口, 初始化请求</code> | 装配成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0043 | F0010 | F0016 | <code>海中鱼巣/启动.应用程序.ixx:130</code> | direct_const_member | <code>this=&初始化</code> | after E0042 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0044 | F0010 | F0034 | <code>海中鱼巣/启动.应用程序.ixx:131-140</code> | lambda_direct | <code>捕获初始化</code> | 初始化失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0009 | F0010 | F0053 | <code>海中鱼巣/启动.应用程序.ixx:146</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0010 | F0010 | F0054 | <code>海中鱼巣/启动.应用程序.ixx:151</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0045 | F0010 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:159, 海中鱼巣/启动.应用程序.ixx:159-161</code> | direct_free_function+unique_name | <code>选中端口, 必须往返一致</code> | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0046 | F0010 | F0035 | <code>海中鱼巣/启动.应用程序.ixx:163, 海中鱼巣/启动.应用程序.ixx:167</code> | direct_const_member | <code>this=&审计</code> | after E0045；always after summary | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0050 | F0011 | F0037 | <code>海中鱼巣/启动.应用程序.ixx:402</code> | direct_free_function+unique_name | <code>未单独冻结；读取源码调用点</code> | 性能能力宏开启；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0054 | F0013 | F0039 | <code>海中鱼巣/装配.普通应用.ixx:129</code> | direct_const_member | <code>this=&配置</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0055 | F0013 | F0040 | <code>海中鱼巣/装配.普通应用.ixx:133</code> | external_template_project_constructor | <code>this=新分配存储</code> | 配置有效 && make_unique分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0061 | F0015 | F0046 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_normal | <code>未单独冻结；读取源码调用点</code> | 等待回调返回 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0197 | F0015 | F0123 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0032且等待回调已执行；四轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0989 | F0015 | F0473 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0254；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1008 | F0015 | F0480 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0255；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1030 | F0015 | F0486 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0256；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1051 | F0015 | F0492 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0257；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1084 | F0015 | F0501 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0258；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1103 | F0015 | F0507 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0259；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1125 | F0015 | F0513 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0260；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1153 | F0015 | F0520 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0261；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1175 | F0015 | F0527 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0262；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1196 | F0015 | F0535 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0263；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1218 | F0015 | F0541 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0264；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1238 | F0015 | F0548 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0265；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1271 | F0015 | F0559 | <code>海中鱼巣/领域/初始化.系统.ixx:100</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0266；等待回调已执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0062 | F0015 | F0047 | <code>海中鱼巣/领域/初始化.系统.ixx:103</code> | direct_const_member | <code>this=快照对象</code> | 等待成功 && 快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0063 | F0015 | F0048 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_normal | <code>未单独冻结；读取源码调用点</code> | 快照成功 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0198 | F0015 | F0124 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0032且快照成功；两轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0990 | F0015 | F0474 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0254；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1009 | F0015 | F0481 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0255；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1031 | F0015 | F0487 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0256；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1052 | F0015 | F0493 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0257；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1085 | F0015 | F0502 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0258；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1104 | F0015 | F0508 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0259；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1126 | F0015 | F0514 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0260；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1154 | F0015 | F0521 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0261；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1176 | F0015 | F0528 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0262；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1197 | F0015 | F0536 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0263；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1219 | F0015 | F0542 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0264；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1239 | F0015 | F0549 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0265；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1272 | F0015 | F0560 | <code>海中鱼巣/领域/初始化.系统.ixx:107</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口绑定来自F0266；等待和快照验收成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0064 | F0015 | F0049 | <code>海中鱼巣/领域/初始化.系统.ixx:108</code> | direct_const_member | <code>this=&结果.概念图</code> | after E0063 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0065 | F0015 | F0050 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_normal | <code>自我存在节点</code> | 概念图成功 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0199 | F0015 | F0125 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0032且概念图成功；失败点5一轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0991 | F0015 | F0475 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0254；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1010 | F0015 | F0482 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0255；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1032 | F0015 | F0488 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0256；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1053 | F0015 | F0494 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0257；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1086 | F0015 | F0503 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0258；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1105 | F0015 | F0509 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0259；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1127 | F0015 | F0515 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0260；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1155 | F0015 | F0522 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0261；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1177 | F0015 | F0529 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0262；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1198 | F0015 | F0537 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0263；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1220 | F0015 | F0543 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0264；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1240 | F0015 | F0550 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0265；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1273 | F0015 | F0561 | <code>海中鱼巣/领域/初始化.系统.ixx:112-113</code> | callback_invoke_resolved_test | <code>自我存在节点</code> | 端口绑定来自F0266；概念图初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0066 | F0015 | F0051 | <code>海中鱼巣/领域/初始化.系统.ixx:115</code> | direct_operator | <code>支持根, 存在根</code> | 支持有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0056 | F0015 | F0041 | <code>海中鱼巣/领域/初始化.系统.ixx:79</code> | direct_const_member | <code>this=&请求</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0057 | F0015 | F0042 | <code>海中鱼巣/领域/初始化.系统.ixx:83</code> | direct_const_member | <code>this=&端口</code> | 请求有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0058 | F0015 | F0043 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_normal | <code>请求.方法登记根稳定键, 状态服务</code> | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0194 | F0015 | F0120 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0032；六轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0986 | F0015 | F0470 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0254；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1005 | F0015 | F0477 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0255；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1027 | F0015 | F0483 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0256；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1048 | F0015 | F0489 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0257；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1081 | F0015 | F0498 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0258；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1100 | F0015 | F0504 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0259；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1122 | F0015 | F0510 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0260；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1150 | F0015 | F0517 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0261；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1172 | F0015 | F0524 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0262；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1193 | F0015 | F0532 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0263；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1215 | F0015 | F0538 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0264；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1235 | F0015 | F0545 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0265；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1268 | F0015 | F0556 | <code>海中鱼巣/领域/初始化.系统.ixx:88</code> | callback_invoke_resolved_test | <code>请求.方法登记根稳定键</code> | 端口绑定来自F0266；请求和端口有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0059 | F0015 | F0044 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_normal | <code>请求.根需求参数</code> | 方法根有值 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0195 | F0015 | F0121 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0032且方法根有值；五轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0987 | F0015 | F0471 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0254；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1006 | F0015 | F0478 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0255；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1028 | F0015 | F0484 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0256；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1049 | F0015 | F0490 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0257；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1082 | F0015 | F0499 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0258；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1101 | F0015 | F0505 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0259；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1123 | F0015 | F0511 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0260；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1151 | F0015 | F0518 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0261；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1173 | F0015 | F0525 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0262；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1194 | F0015 | F0533 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0263；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1216 | F0015 | F0539 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0264；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1236 | F0015 | F0546 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0265；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1269 | F0015 | F0557 | <code>海中鱼巣/领域/初始化.系统.ixx:93</code> | callback_invoke_resolved_test | <code>请求.根需求参数</code> | 端口绑定来自F0266；方法登记根有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0060 | F0015 | F0045 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_normal | <code>10000ms</code> | 首次启动成功 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0196 | F0015 | F0122 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0032且启动成功；四轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0988 | F0015 | F0472 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0254；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1007 | F0015 | F0479 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0255；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1029 | F0015 | F0485 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0256；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1050 | F0015 | F0491 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0257；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1083 | F0015 | F0500 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0258；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1102 | F0015 | F0506 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0259；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1124 | F0015 | F0512 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0260；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1152 | F0015 | F0519 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0261；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1174 | F0015 | F0526 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0262；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1195 | F0015 | F0534 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0263；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1217 | F0015 | F0540 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0264；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1237 | F0015 | F0547 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0265；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1270 | F0015 | F0558 | <code>海中鱼巣/领域/初始化.系统.ixx:98-99</code> | callback_invoke_resolved_test | <code>10000ms</code> | 端口绑定来自F0266；启动成功且非复用 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0067 | F0016 | F0047 | <code>海中鱼巣/领域/初始化.系统.ixx:69</code> | direct_const_member | <code>this=自我初始化快照</code> | 状态已初始化 && 方法根有值 && 快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0068 | F0016 | F0049 | <code>海中鱼巣/领域/初始化.系统.ixx:70</code> | direct_const_member | <code>this=&概念图</code> | 前项全部为真 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0069 | F0018 | F0052 | <code>海中鱼巣/适配/审计.数据库启动.ixx:59</code> | direct_const_member | <code>this=&端口</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0070 | F0018 | F0053 | <code>海中鱼巣/适配/审计.数据库启动.ixx:62</code> | callback_invoke_resolved_normal | <code>生产命名空间规格</code> | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0206 | F0018 | F0126 | <code>海中鱼巣/适配/审计.数据库启动.ixx:62</code> | callback_invoke_resolved_test | <code>未单独冻结；读取源码调用点</code> | 端口来自F0032；写失败与不一致路径各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0071 | F0018 | F0054 | <code>海中鱼巣/适配/审计.数据库启动.ixx:72</code> | callback_invoke_resolved_normal | <code>命名空间, 三仓库</code> | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0207 | F0018 | F0127 | <code>海中鱼巣/适配/审计.数据库启动.ixx:72</code> | callback_invoke_resolved_test | <code>命名空间</code> | 端口来自F0032；两条路径各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0072 | F0018 | F0055 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_normal | <code>快照, 启动结构统计说明</code> | 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0208 | F0018 | F0128 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_test | <code>快照, 启动结构统计说明</code> | 写入失败端口路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0209 | F0018 | F0129 | <code>海中鱼巣/适配/审计.数据库启动.ixx:73</code> | callback_invoke_resolved_test | <code>快照, 启动结构统计说明</code> | F0010覆盖端口来自F0032 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0073 | F0018 | F0056 | <code>海中鱼巣/适配/审计.数据库启动.ixx:78</code> | callback_invoke_resolved_normal | <code>1</code> | 写入成功 && 正常生产端口绑定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0210 | F0018 | F0130 | <code>海中鱼巣/适配/审计.数据库启动.ixx:78</code> | callback_invoke_resolved_test | <code>1</code> | F0010覆盖端口来自F0032且写入成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0074 | F0019 | F0057 | <code>海中鱼巣/启动.程序运行宿主.ixx:116</code> | external_template_project_constructor | <code>this=新分配存储</code> | 前置通过 && make_unique分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0075 | F0019 | F0058 | <code>海中鱼巣/启动.程序运行宿主.ixx:118, 海中鱼巣/启动.程序运行宿主.ixx:123, 海中鱼巣/启动.程序运行宿主.ixx:129</code> | callback_register | <code>SIGINT回调地址</code> | 安装SIGINT；安装SIGTERM；Windows编译 && 安装SIGBREAK | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0211 | F0019 | F0067 | <code>海中鱼巣/启动.程序运行宿主.ixx:118, 海中鱼巣/启动.程序运行宿主.ixx:123</code> | callback_invoke_resolved_test | <code>SIGINT, 接收程序停止信号</code> | 安装函数实例来自F0032首次调用；安装函数实例来自F0032且SIGINT安装成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0076 | F0019 | F0025 | <code>海中鱼巣/启动.程序运行宿主.ixx:120, 海中鱼巣/启动.程序运行宿主.ixx:125, 海中鱼巣/启动.程序运行宿主.ixx:131, 海中鱼巣/启动.程序运行宿主.ixx:137-139</code> | compiler_implicit_destructor | <code>this=租约</code> | SIGINT安装失败；SIGTERM安装失败；SIGBREAK安装失败；已构造租约后的异常展开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0082 | F0022 | F0016 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:262</code> | direct_const_member | <code>this=&初始化</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0083 | F0022 | F0059 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:267</code> | direct_const_member+unique_name | <code>快照.世界树.自我存在节点</code> | 初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0084 | F0022 | F0060 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:268-269</code> | direct_free_function | <code>上下文.节点, 上下文.关系, 快照.世界树, 快照.语素, 坐标</code> | 初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0085 | F0022 | F0061 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:274</code> | direct_free_function | <code>快照.语素, 初始化.概念图</code> | 世界树可展示且只读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0086 | F0024 | F0016 | <code>海中鱼巣/启动.程序运行宿主.ixx:148</code> | direct_const_member | <code>this=&初始化</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0087 | F0024 | F0023 | <code>海中鱼巣/启动.程序运行宿主.ixx:148</code> | direct_const_member_short_circuit | <code>this=&投影</code> | 初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0088 | F0024 | F0062 | <code>海中鱼巣/启动.程序运行宿主.ixx:153-164</code> | direct_constructor | <code>上下文八项引用, 投影两树, 两个根需求句柄</code> | 初始化和投影成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0089 | F0024 | F0063 | <code>海中鱼巣/启动.程序运行宿主.ixx:165</code> | direct_constructor | <code>控制面板, 上下文.数据库, 审计.快照</code> | after E0088 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0090 | F0024 | F0064 | <code>海中鱼巣/启动.程序运行宿主.ixx:166</code> | direct_member | <code>this=&窗口, &信号.停止请求</code> | after E0089 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0091 | F0024 | F0065 | <code>海中鱼巣/启动.程序运行宿主.ixx:172</code> | compiler_implicit_destructor | <code>this=&窗口</code> | 函数返回前 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0095 | F0027 | F0069 | <code>海中鱼巣/启动.生产运行期.ixx:103, 海中鱼巣/启动.生产运行期.ixx:56</code> | direct_const_member | <code>this=&宿主_</code> | 请求有效且已取得启动锁；发布状态被协议允许 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0096 | F0027 | F0066 | <code>海中鱼巣/启动.生产运行期.ixx:104, 海中鱼巣/启动.生产运行期.ixx:57</code> | direct_const_member | <code>this=&当前租约</code> | after E0095；after E0106 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0108 | F0027 | F0079 | <code>海中鱼巣/启动.生产运行期.ixx:106</code> | direct_const_member | <code>this=&当前租约</code> | 租约有效且发布状态为已发布 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0094 | F0027 | F0068 | <code>海中鱼巣/启动.生产运行期.ixx:51</code> | direct_const_member | <code>this=&请求</code> | always | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0011 | F0027 | R0001 | <code>海中鱼巣/启动.生产运行期.ixx:56, 海中鱼巣/启动.生产运行期.ixx:103</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0097 | F0027 | F0070 | <code>海中鱼巣/启动.生产运行期.ixx:66</code> | direct_free_function+unique_name | <code>未单独冻结；读取源码调用点</code> | 无现有有效租约；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0098 | F0027 | F0071 | <code>海中鱼巣/启动.生产运行期.ixx:67</code> | direct_const_member | <code>this=&配置</code> | after E0097 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0099 | F0027 | F0072 | <code>海中鱼巣/启动.生产运行期.ixx:71</code> | external_template_project_constructor | <code>配置.域编号, 配置.仓库编号, 配置.动作键</code> | 配置有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0100 | F0027 | F0073 | <code>海中鱼巣/启动.生产运行期.ixx:72</code> | direct_member | <code>this=候选.get(), 配置.系统角色</code> | 候选构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0101 | F0027 | F0074 | <code>海中鱼巣/启动.生产运行期.ixx:73</code> | direct_const_member | <code>this=&系统角色结果</code> | after E0100 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0102 | F0027 | F0075 | <code>海中鱼巣/启动.生产运行期.ixx:79</code> | direct_member | <code>this=候选.get(), 配置.概念活动</code> | 系统角色初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0103 | F0027 | F0076 | <code>海中鱼巣/启动.生产运行期.ixx:80</code> | direct_const_member | <code>this=&概念活动结果</code> | after E0102 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0104 | F0027 | F0077 | <code>海中鱼巣/启动.生产运行期.ixx:86</code> | direct_const_member | <code>this=候选.get()</code> | 概念活动初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0105 | F0027 | F0078 | <code>海中鱼巣/启动.生产运行期.ixx:93</code> | direct_member+unique_name | <code>this=&宿主_, 候选共享所有权副本</code> | 候选完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0092 | F0028 | F0066 | <code>海中鱼巣/启动.生产运行期.ixx:40</code> | direct_const_member | <code>this=&租约</code> | 状态为已启动或已有当前上下文 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0109 | F0029 | F0030 | <code>海中鱼巣/自检.入口初始化.ixx:485-486, 海中鱼巣/自检.入口初始化.ixx:487-488, 海中鱼巣/自检.入口初始化.ixx:489-490, 海中鱼巣/自检.入口初始化.ixx:491-492, 海中鱼巣/自检.入口初始化.ixx:493-494, 海中鱼巣/自检.入口初始化.ixx:495-496, 海中鱼巣/自检.入口初始化.ixx:497-498, 海中鱼巣/自检.入口初始化.ixx:499-500, 海中鱼巣/自检.入口初始化.ixx:501-502, 海中鱼巣/自检.入口初始化.ixx:503-504, 海中鱼巣/自检.入口初始化.ixx:505-506, 海中鱼巣/自检.入口初始化.ixx:507-508, 海中鱼巣/自检.入口初始化.ixx:509-510, 海中鱼巣/自检.入口初始化.ixx:511-512</code> | direct_member | <code>10, S01编号名称, F0080回调</code> | always；S01成功；S01-S02成功；S01-S03成功；S01-S04成功；S01-S05成功；S01-S06成功；S01-S07成功；S01-S08成功；S01-S09成功；S01-S10成功；S01-S11成功；S01-S12成功；S01-S13成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0123 | F0029 | F0080 | <code>海中鱼巣/自检.入口初始化.ixx:486</code> | callback_bind | <code>配置按值捕获</code> | 构造S01登记参数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0012 | F0029 | F0254 | <code>海中鱼巣/自检.入口初始化.ixx:486</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0124 | F0029 | F0081 | <code>海中鱼巣/自检.入口初始化.ixx:488</code> | callback_bind | <code>配置按值捕获</code> | S01成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0013 | F0029 | F0255 | <code>海中鱼巣/自检.入口初始化.ixx:488</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0125 | F0029 | F0082 | <code>海中鱼巣/自检.入口初始化.ixx:490</code> | callback_bind | <code>配置按值捕获</code> | S01-S02成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0014 | F0029 | F0256 | <code>海中鱼巣/自检.入口初始化.ixx:490</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0126 | F0029 | F0083 | <code>海中鱼巣/自检.入口初始化.ixx:492</code> | callback_bind | <code>配置按值捕获</code> | S01-S03成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0015 | F0029 | F0257 | <code>海中鱼巣/自检.入口初始化.ixx:492</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0127 | F0029 | F0084 | <code>海中鱼巣/自检.入口初始化.ixx:494</code> | callback_bind | <code>配置按值捕获</code> | S01-S04成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0016 | F0029 | F0258 | <code>海中鱼巣/自检.入口初始化.ixx:494</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0128 | F0029 | F0085 | <code>海中鱼巣/自检.入口初始化.ixx:496</code> | callback_bind | <code>配置按值捕获</code> | S01-S05成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0017 | F0029 | F0259 | <code>海中鱼巣/自检.入口初始化.ixx:496</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0129 | F0029 | F0086 | <code>海中鱼巣/自检.入口初始化.ixx:498</code> | callback_bind | <code>配置按值捕获</code> | S01-S06成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0018 | F0029 | F0260 | <code>海中鱼巣/自检.入口初始化.ixx:498</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0130 | F0029 | F0087 | <code>海中鱼巣/自检.入口初始化.ixx:500</code> | callback_bind | <code>配置按值捕获</code> | S01-S07成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0019 | F0029 | F0261 | <code>海中鱼巣/自检.入口初始化.ixx:500</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0131 | F0029 | F0088 | <code>海中鱼巣/自检.入口初始化.ixx:502</code> | callback_bind | <code>配置按值捕获</code> | S01-S08成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0020 | F0029 | F0262 | <code>海中鱼巣/自检.入口初始化.ixx:502</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0132 | F0029 | F0089 | <code>海中鱼巣/自检.入口初始化.ixx:504</code> | callback_bind | <code>配置按值捕获</code> | S01-S09成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0021 | F0029 | F0263 | <code>海中鱼巣/自检.入口初始化.ixx:504</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0133 | F0029 | F0090 | <code>海中鱼巣/自检.入口初始化.ixx:506</code> | callback_bind | <code>配置按值捕获</code> | S01-S10成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0022 | F0029 | F0264 | <code>海中鱼巣/自检.入口初始化.ixx:506</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0134 | F0029 | F0091 | <code>海中鱼巣/自检.入口初始化.ixx:508</code> | callback_bind | <code>配置按值捕获</code> | S01-S11成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0023 | F0029 | F0265 | <code>海中鱼巣/自检.入口初始化.ixx:508</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0135 | F0029 | F0092 | <code>海中鱼巣/自检.入口初始化.ixx:510</code> | callback_bind | <code>配置按值捕获</code> | S01-S12成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0024 | F0029 | F0266 | <code>海中鱼巣/自检.入口初始化.ixx:510</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0136 | F0029 | F0093 | <code>海中鱼巣/自检.入口初始化.ixx:512</code> | callback_bind | <code>配置按值捕获</code> | S01-S13成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0025 | F0029 | F0267 | <code>海中鱼巣/自检.入口初始化.ixx:512</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0137 | F0030 | F0094 | <code>海中鱼巣/自检.运行器.ixx:50-52</code> | standard_algorithm_callback | <code>当前登记项, 捕获顺序和编号引用</code> | 入口参数合法且遍历尚未命中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0038 | F0031 | F0032 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>登记.回调()</code> | 执行到顺序150 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0039 | F0031 | F0033 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>登记.回调()</code> | 执行到顺序160 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0138 | F0031 | F0080 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S01登记回调</code> | 执行到S01 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0139 | F0031 | F0081 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S02登记回调</code> | 执行到S02 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0140 | F0031 | F0082 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S03登记回调</code> | 执行到S03 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0141 | F0031 | F0083 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S04登记回调</code> | 执行到S04 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0142 | F0031 | F0084 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S05登记回调</code> | 执行到S05 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0143 | F0031 | F0085 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S06登记回调</code> | 执行到S06 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0144 | F0031 | F0086 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S07登记回调</code> | 执行到S07 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0145 | F0031 | F0087 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S08登记回调</code> | 执行到S08 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0146 | F0031 | F0088 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S09登记回调</code> | 执行到S09 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0147 | F0031 | F0089 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S10登记回调</code> | 执行到S10 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0148 | F0031 | F0090 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S11登记回调</code> | 执行到S11 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0149 | F0031 | F0091 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S12登记回调</code> | 执行到S12 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0150 | F0031 | F0092 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S13登记回调</code> | 执行到S13 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0151 | F0031 | F0093 | <code>海中鱼巣/自检.运行器.ixx:80</code> | callback_invoke | <code>S14登记回调</code> | 执行到S14 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0182 | F0032 | F0013 | <code>海中鱼巣/启动.应用程序.ixx:197</code> | direct_free_function+unique_name | <code>配置</code> | 六轮各一次；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0183 | F0032 | F0014 | <code>海中鱼巣/启动.应用程序.ixx:198</code> | direct_const_member | <code>this=&装配</code> | 每轮装配后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0026 | F0032 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:221</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0027 | F0032 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:227</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0028 | F0032 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:233</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0184 | F0032 | F0015 | <code>海中鱼巣/启动.应用程序.ixx:242, 海中鱼巣/启动.应用程序.ixx:242-243</code> | direct_free_function+unique_name | <code>故障注入端口, {配置.方法登记根稳定键,配置.根需求参数}</code> | 装配成功；六轮；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0185 | F0032 | F0131 | <code>海中鱼巣/启动.应用程序.ixx:246</code> | compiler_implicit_member_destructor | <code>this=&上下文.自我线程实例</code> | 每轮上下文作用域退出；成员已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0186 | F0032 | F0018 | <code>海中鱼巣/启动.应用程序.ixx:260-261, 海中鱼巣/启动.应用程序.ixx:261</code> | direct_free_function+unique_name | <code>写入失败端口, 数据库审计要求::最佳努力</code> | 六轮结束；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0187 | F0032 | F0010 | <code>海中鱼巣/启动.应用程序.ixx:280</code> | direct_free_function+lsp_direct | <code>&不一致端口</code> | 写入失败状态验收后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0188 | F0032 | F0019 | <code>海中鱼巣/启动.应用程序.ixx:293, 海中鱼巣/启动.应用程序.ixx:294</code> | direct_free_function+unique_name | <code>F0067信号安装替身</code> | 数据库验收后；after E0188；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0190 | F0032 | F0020 | <code>海中鱼巣/启动.应用程序.ixx:296, 海中鱼巣/启动.应用程序.ixx:301</code> | direct_const_member | <code>this=&信号失败</code> | 状态为SIGTERM失败；前置短路项全部为真 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0193 | F0032 | F0025 | <code>海中鱼巣/启动.应用程序.ixx:320</code> | compiler_implicit_destructor | <code>this=后续可安装.租约.get()</code> | 根函数退出且租约存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0213 | F0033 | F0132 | <code>海中鱼巣/启动.应用程序.ixx:327-333</code> | callback_bind | <code>捕获第一编号组,第一地址全非零,第一初始为零</code> | 构造第一配置 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0214 | F0033 | F0029 | <code>海中鱼巣/启动.应用程序.ixx:335, 海中鱼巣/启动.应用程序.ixx:353</code> | direct_free_function+unique_name | <code>第一运行器,第一配置</code> | always；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0215 | F0033 | F0031 | <code>海中鱼巣/启动.应用程序.ixx:336, 海中鱼巣/启动.应用程序.ixx:354</code> | direct_member+unique_name | <code>this=&第一运行器</code> | 第一登记结果无论真假；第二登记结果无论真假；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0216 | F0033 | F0133 | <code>海中鱼巣/启动.应用程序.ixx:350-351</code> | callback_bind | <code>捕获第二观察数量</code> | 构造第二配置 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0153 | F0037 | F0096 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:877-880</code> | lambda_direct | <code>未单独冻结；读取源码调用点</code> | try开始后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0154 | F0037 | F0097 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:885</code> | direct_free_function | <code>规模</code> | 对1000/10000/100000各一次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0179 | F0037 | F0118 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:893-895</code> | standard_algorithm_callback | <code>当前规模报告</code> | 验收1前置短路均通过且all_of遍历 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0155 | F0037 | F0098 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:902-911</code> | standard_algorithm_callback | <code>当前规模报告</code> | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0180 | F0037 | F0119 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:912-915</code> | standard_algorithm_callback | <code>当前规模报告</code> | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0156 | F0037 | F0099 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:922</code> | trivial_adapter_direct | <code>当前规模报告</code> | all_of遍历且尚未失败 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0220 | F0039 | F0134 | <code>海中鱼巣/装配.普通应用.ixx:48</code> | direct_const_member | <code>this=&根需求参数</code> | 方法登记根稳定键!=0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0221 | F0040 | F0135 | <code>海中鱼巣/装配.普通应用.ixx:54-55</code> | member_constructor | <code>1, 默认结构事务接线</code> | 构造开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0222 | F0040 | F0136 | <code>海中鱼巣/装配.普通应用.ixx:55</code> | member_constructor | <code>主信息,1,默认接线</code> | E0221成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0223 | F0040 | F0137 | <code>海中鱼巣/装配.普通应用.ixx:56</code> | member_constructor | <code>节点,1,默认接线</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0224 | F0040 | F0138 | <code>海中鱼巣/装配.普通应用.ixx:57</code> | member_constructor | <code>节点,默认接线</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0225 | F0040 | F0139 | <code>海中鱼巣/装配.普通应用.ixx:58</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0226 | F0040 | F0140 | <code>海中鱼巣/装配.普通应用.ixx:59</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0227 | F0040 | F0141 | <code>海中鱼巣/装配.普通应用.ixx:60</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0228 | F0040 | F0142 | <code>海中鱼巣/装配.普通应用.ixx:61</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0229 | F0040 | F0143 | <code>海中鱼巣/装配.普通应用.ixx:62</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0230 | F0040 | F0144 | <code>海中鱼巣/装配.普通应用.ixx:63</code> | member_constructor | <code>主信息,节点,关系,索引</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0231 | F0040 | F0145 | <code>海中鱼巣/装配.普通应用.ixx:64</code> | member_constructor | <code>主信息,节点,默认接线</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0232 | F0040 | F0146 | <code>海中鱼巣/装配.普通应用.ixx:65</code> | member_constructor | <code>主信息,节点</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0233 | F0040 | F0147 | <code>海中鱼巣/装配.普通应用.ixx:66</code> | member_constructor | <code>节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0234 | F0040 | F0148 | <code>海中鱼巣/装配.普通应用.ixx:67</code> | member_constructor | <code>概念图,存在,动态,二次特征,因果,状态,语素</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0235 | F0040 | F0149 | <code>海中鱼巣/装配.普通应用.ixx:68</code> | member_constructor | <code>主信息,节点,关系,二次特征,特征值</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0236 | F0040 | F0150 | <code>海中鱼巣/装配.普通应用.ixx:69</code> | member_constructor | <code>主信息,节点,存在,场景,特征,状态,动态,二次特征,因果</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0237 | F0040 | F0151 | <code>海中鱼巣/装配.普通应用.ixx:70</code> | member_constructor | <code>主信息,节点,关系,世界,场景,存在</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0238 | F0040 | F0152 | <code>海中鱼巣/装配.普通应用.ixx:71</code> | member_constructor | <code>主信息,节点,关系,世界,语素</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0239 | F0040 | F0153 | <code>海中鱼巣/装配.普通应用.ixx:72</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0240 | F0040 | F0154 | <code>海中鱼巣/装配.普通应用.ixx:73</code> | member_constructor | <code>特征,语素,需求,状态</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0241 | F0040 | F0155 | <code>海中鱼巣/装配.普通应用.ixx:74</code> | member_constructor | <code>语素初始化,世界树初始化,需求初始化</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0242 | F0040 | F0156 | <code>海中鱼巣/装配.普通应用.ixx:75</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0243 | F0040 | F0157 | <code>海中鱼巣/装配.普通应用.ixx:76</code> | member_constructor | <code>主信息,节点,关系</code> | 前序成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0244 | F0040 | F0158 | <code>海中鱼巣/装配.普通应用.ixx:77</code> | static_member_direct | <code>未单独冻结；读取源码调用点</code> | 统计和显示已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0245 | F0040 | F0159 | <code>海中鱼巣/装配.普通应用.ixx:77</code> | member_constructor | <code>移动F0158返回配置</code> | E0244成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0246 | F0041 | F0134 | <code>海中鱼巣/领域/初始化.系统.ixx:24</code> | direct_const_member | <code>this=&根需求参数</code> | 方法登记根稳定键!=0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0247 | F0043 | F0160 | <code>海中鱼巣/领域/方法服务.h:263, 海中鱼巣/领域/方法服务.h:295</code> | direct_const_member+unique_name | <code>this, 状态</code> | 已有材料且稳定键相同；私有材料与仓库编号已发布；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0248 | F0043 | F0161 | <code>海中鱼巣/领域/方法服务.h:264, 海中鱼巣/领域/方法服务.h:273, 海中鱼巣/领域/方法服务.h:280, 海中鱼巣/领域/方法服务.h:289, 海中鱼巣/领域/方法服务.h:298</code> | direct_member_template_instance | <code>锁, 重复初始化异常说明</code> | 既有同键登记根不完整；登记根无效或角色绑定失败；状态无效或相同；关系无效或仓库编号不同；发布后完整性失败且私有发布已撤回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0249 | F0043 | F0162 | <code>海中鱼巣/领域/方法服务.h:270</code> | direct_member+unique_name | <code>this</code> | 无既有材料；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0250 | F0043 | F0163 | <code>海中鱼巣/领域/方法服务.h:271, 海中鱼巣/领域/方法服务.h:278</code> | direct_free | <code>登记根</code> | 登记方法返回；两个状态调用均返回；活跃状态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0261 | F0043 | F0168 | <code>海中鱼巣/领域/方法服务.h:271, 海中鱼巣/领域/方法服务.h:278, 海中鱼巣/领域/方法服务.h:287</code> | direct_free+unique_name | <code>活跃关系</code> | 两条关系调用均返回；活跃关系有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0251 | F0043 | F0164 | <code>海中鱼巣/领域/方法服务.h:272</code> | direct_member+unique_name | <code>登记根, 方法角色状态::方法登记根, 状态</code> | 登记根有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0253 | F0043 | F0165 | <code>海中鱼巣/领域/方法服务.h:276, 海中鱼巣/领域/方法服务.h:277</code> | direct_member | <code>static_cast<std::int64_t>(方法生命周期状态::活跃)</code> | 登记根创建完整；活跃状态调用返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0257 | F0043 | F0051 | <code>海中鱼巣/领域/方法服务.h:279</code> | direct_free_operator | <code>活跃状态, 失效状态</code> | 两个状态均有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0259 | F0043 | F0167 | <code>海中鱼巣/领域/方法服务.h:283-284, 海中鱼巣/领域/方法服务.h:285-286</code> | direct_member | <code>关系类型::模板, 登记根, 活跃状态, 40</code> | 两个状态有效且不同；活跃关系调用返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0266 | F0044 | F0046 | <code>海中鱼巣/线程/自我线程.ixx:183</code> | direct_const_member+unique_name | <code>this</code> | 进入启动；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0267 | F0044 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:184</code> | direct_const_member | <code>this=&*已有快照</code> | 已有快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0268 | F0044 | F0169 | <code>海中鱼巣/线程/自我线程.ixx:188, 海中鱼巣/线程/自我线程.ixx:191, 海中鱼巣/线程/自我线程.ixx:196, 海中鱼巣/线程/自我线程.ixx:210</code> | direct_static_member+suffix_match | <code>自我线程拒绝原因::重复启动</code> | 状态非未启动或线程可汇合；参数无效；邮箱与处理器接入状态不一致；捕获std::system_error；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0269 | F0044 | F0134 | <code>海中鱼巣/线程/自我线程.ixx:190</code> | direct_const_member | <code>this=&参数</code> | 非重复启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0272 | F0044 | F0170 | <code>海中鱼巣/线程/自我线程.ixx:201</code> | direct_member+unique_name | <code>this</code> | 全部准入通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0273 | F0044 | F0171 | <code>海中鱼巣/线程/自我线程.ixx:204-206</code> | thread_callback | <code>捕获this与参数副本</code> | std::thread构造成功后异步调度 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0275 | F0045 | F0172 | <code>海中鱼巣/线程/自我线程.ixx:217-224</code> | condition_variable_predicate | <code>捕获this</code> | wait_for初检或唤醒后，可重复 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0276 | F0045 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:227</code> | direct_const_member | <code>this=&*初始化快照</code> | 等待结束且快照有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0277 | F0047 | F0173 | <code>海中鱼巣/线程/自我线程.ixx:57</code> | direct_const_member | <code>this=&语素</code> | 进入快照成功检查 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0278 | F0047 | F0174 | <code>海中鱼巣/线程/自我线程.ixx:58</code> | direct_const_member | <code>this=&世界树</code> | 语素初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0279 | F0047 | F0175 | <code>海中鱼巣/线程/自我线程.ixx:59</code> | direct_const_member | <code>this=&根需求</code> | 语素和世界树初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0280 | F0048 | F0049 | <code>海中鱼巣/领域/初始化.概念图.ixx:100, 海中鱼巣/领域/初始化.概念图.ixx:135</code> | direct_const_member | <code>this=&已发布结果_</code> | 取得初始化锁；四根名称均成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0281 | F0048 | F0176 | <code>海中鱼巣/领域/初始化.概念图.ixx:101, 海中鱼巣/领域/初始化.概念图.ixx:135</code> | direct_const_member+unique_name | <code>已发布结果_</code> | 已发布结果成功；候选成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1633 | F0048 | F0177 | <code>海中鱼巣/领域/初始化.概念图.ixx:104-105</code> | direct_member_template_instance | <code>未单独冻结；读取源码调用点</code> | 初始化四个根按源码顺序调用 F0177；对应 lambda@105 | 专项源码静态类型与实际装配人工复核 |
| RCE1634 | F0048 | F0178 | <code>海中鱼巣/领域/初始化.概念图.ixx:106-107</code> | direct_member_template_instance | <code>未单独冻结；读取源码调用点</code> | 初始化四个根按源码顺序调用 F0178；对应 lambda@107 | 专项源码静态类型与实际装配人工复核 |
| RCE1635 | F0048 | F0179 | <code>海中鱼巣/领域/初始化.概念图.ixx:108-109</code> | direct_member_template_instance | <code>未单独冻结；读取源码调用点</code> | 初始化四个根按源码顺序调用 F0179；对应 lambda@109 | 专项源码静态类型与实际装配人工复核 |
| E0282 | F0048 | F0180 | <code>海中鱼巣/领域/初始化.概念图.ixx:110-111</code> | direct_member_template_instance | <code>未单独冻结；读取源码调用点</code> | 初始化四个根按源码顺序调用 F0180；对应 lambda@111 | 专项源码静态类型与实际装配人工复核 |
| E0290 | F0048 | F0181 | <code>海中鱼巣/领域/初始化.概念图.ixx:115, 海中鱼巣/领域/初始化.概念图.ixx:115-117, 海中鱼巣/领域/初始化.概念图.ixx:118, 海中鱼巣/领域/初始化.概念图.ixx:118-120, 海中鱼巣/领域/初始化.概念图.ixx:121, 海中鱼巣/领域/初始化.概念图.ixx:121-123</code> | direct_member+unique_name | <code>活跃状态槽,概念生命周期阶段::活跃</code> | 四根均确保成功；活跃状态确保成功；冷却状态确保成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0293 | F0048 | F0182 | <code>海中鱼巣/领域/初始化.概念图.ixx:124</code> | direct_member+unique_name | <code>this=&概念图_</code> | 三个状态均确保成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0294 | F0048 | F0183 | <code>海中鱼巣/领域/初始化.概念图.ixx:128, 海中鱼巣/领域/初始化.概念图.ixx:129, 海中鱼巣/领域/初始化.概念图.ixx:130, 海中鱼巣/领域/初始化.概念图.ixx:131</code> | direct_member+unique_name | <code>存在根项,L存在</code> | 生命周期初始化成功；存在根名称成功；动态根名称成功；关系根名称成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0300 | F0048 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:135, 海中鱼巣/领域/初始化.概念图.ixx:135-136</code> | direct_free+unique_name | <code>候选成功且结构仍可读,具名说明</code> | 名称阶段成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0301 | F0049 | F0189 | <code>海中鱼巣/领域/初始化.概念图.ixx:50, 海中鱼巣/领域/初始化.概念图.ixx:51, 海中鱼巣/领域/初始化.概念图.ixx:52, 海中鱼巣/领域/初始化.概念图.ixx:53</code> | direct_const_member | <code>this=&存在根</code> | 进入结果检查；存在根项成功；存在和动态根项成功；前三根项成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0305 | F0049 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:54, 海中鱼巣/领域/初始化.概念图.ixx:55, 海中鱼巣/领域/初始化.概念图.ixx:56</code> | direct_free | <code>活跃状态</code> | 四根项成功；活跃状态有效；前两个状态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0033 | F0049 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:54, 海中鱼巣/领域/初始化.概念图.ixx:55, 海中鱼巣/领域/初始化.概念图.ixx:56</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0308 | F0049 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:57, 海中鱼巣/领域/初始化.概念图.ixx:58, 海中鱼巣/领域/初始化.概念图.ixx:59</code> | rewritten_not_equal | <code>活跃状态,冷却状态</code> | 三个状态有效；活跃与冷却不同；活跃分别与另两状态不同 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0311 | F0050 | F0190 | <code>海中鱼巣/领域/概念图服务.h:1053</code> | direct_const_member | <code>存在实例</code> | 进入函数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0312 | F0050 | F0191 | <code>海中鱼巣/领域/概念图服务.h:1058</code> | direct_member+unique_name | <code>存在实例,概念根类别::存在</code> | 节点记录存在且类型为存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0313 | F0053 | F0192 | <code>海中鱼巣/领域/统计服务.h:245</code> | direct_const_member+unique_name | <code>类型</code> | 进入准入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0314 | F0053 | F0193 | <code>海中鱼巣/领域/统计服务.h:246</code> | direct_const_member+unique_name | <code>来源</code> | 缓存类型有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0315 | F0053 | F0194 | <code>海中鱼巣/领域/统计服务.h:249</code> | direct_const_member+unique_name | <code>值类型</code> | 来源和两个版本有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0316 | F0053 | F0195 | <code>海中鱼巣/领域/统计服务.h:250</code> | direct_const_member+unique_name | <code>失效条件</code> | 前五项有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0320 | F0054 | F0196 | <code>海中鱼巣/领域/统计服务.h:692</code> | direct_const_member+unique_name | <code>命名空间,节点数,关系数,索引数</code> | 全部实参求值完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0317 | F0054 | F0197 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | <code>this=&节点</code> | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0318 | F0054 | F0198 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | <code>this=&关系</code> | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0319 | F0054 | F0199 | <code>海中鱼巣/领域/统计服务.h:692</code> | indeterminately_sequenced_argument | <code>this=&索引</code> | F0196实参求值，三计数调用顺序不确定 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0321 | F0055 | F0200 | <code>海中鱼巣/适配/SQL数据库适配.cpp:382</code> | direct_free+direct-free | <code>快照</code> | 总准入首项；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0322 | F0055 | F0201 | <code>海中鱼巣/适配/SQL数据库适配.cpp:383</code> | direct_free+direct-free+direct-free+direct-free | <code>快照.节点数</code> | 前置准入均通过；节点数可写入；关系数可写入；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0325 | F0055 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:384, 海中鱼巣/适配/SQL数据库适配.cpp:394</code> | direct_free+direct-free+direct-free | <code>L"结构统计审计写入准入",L"快照命名空间、来源入口或结构数量不符合写入约束。"</code> | 任一写入准入失败；连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0326 | F0055 | F0203 | <code>海中鱼巣/适配/SQL数据库适配.cpp:386</code> | direct_const_member+unique_name+direct-member | <code>this</code> | 写入准入通过；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0369 | F0055 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:392-469</code> | implicit_destructor | <code>连接</code> | 连接构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0327 | F0055 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:392</code> | direct_constructor+constructor | <code>无</code> | 初始化成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0328 | F0055 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:393</code> | direct_member+direct-member | <code>配置_,配置_.数据库,诊断</code> | 连接对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0368 | F0055 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396-469</code> | implicit_destructor | <code>语句</code> | 语句构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0330 | F0055 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396</code> | direct_const_member+direct-member | <code>连接</code> | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0331 | F0055 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:396</code> | direct_constructor+constructor | <code>连接.句柄()</code> | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0332 | F0055 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:397</code> | direct_const_member+direct-member | <code>语句</code> | 语句对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0333 | F0055 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:398, 海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:453, 海中鱼巣/适配/SQL数据库适配.cpp:457, 海中鱼巣/适配/SQL数据库适配.cpp:466</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | <code>L"分配语句",诊断</code> | 语句不可用；准备失败；绑定失败；执行失败；结果集定位失败；Fetch失败；整数列读取失败或编号为0；最近记录与本次写入不一致；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0334 | F0055 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:407, 海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:421, 海中鱼巣/适配/SQL数据库适配.cpp:426, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:440, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:445, 海中鱼巣/适配/SQL数据库适配.cpp:446, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:451, 海中鱼巣/适配/SQL数据库适配.cpp:453, 海中鱼巣/适配/SQL数据库适配.cpp:456</code> | direct_const_member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member | <code>语句</code> | 准备INSERT批次；准备失败；准备成功；绑定仍成功且索引小于5；绑定失败；全部参数绑定成功；执行失败；定位审计编号结果集；列数读取成功且列数为0；更多结果失败或无更多结果；已定位含列结果集；Fetch失败；Fetch成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0335 | F0055 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:407, 海中鱼巣/适配/SQL数据库适配.cpp:424, 海中鱼巣/适配/SQL数据库适配.cpp:437, 海中鱼巣/适配/SQL数据库适配.cpp:440, 海中鱼巣/适配/SQL数据库适配.cpp:445, 海中鱼巣/适配/SQL数据库适配.cpp:447, 海中鱼巣/适配/SQL数据库适配.cpp:452</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | <code>SQLPrepareW返回码</code> | SQLPrepareW已返回；首次来源参数已绑定或上一数值参数已绑定；数值参数循环结束；SQLExecute已返回；SQLNumResultCols已返回；更多结果不是SQL_NO_DATA；SQLFetch已返回；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0337 | F0055 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:408, 海中鱼巣/适配/SQL数据库适配.cpp:438, 海中鱼巣/适配/SQL数据库适配.cpp:441, 海中鱼巣/适配/SQL数据库适配.cpp:448, 海中鱼巣/适配/SQL数据库适配.cpp:453</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free | <code>SQL_HANDLE_STMT,语句.句柄()</code> | 准备失败；绑定失败；执行失败；更多结果失败或无更多结果；Fetch失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0364 | F0055 | F0213 | <code>海中鱼巣/适配/SQL数据库适配.cpp:456</code> | direct_free+direct-free | <code>语句.句柄(),1,审计编号</code> | Fetch成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0366 | F0055 | F0056 | <code>海中鱼巣/适配/SQL数据库适配.cpp:460</code> | direct_const_member+suffix_match+direct-member | <code>1</code> | 取得非零审计编号；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0370 | F0056 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:474, 海中鱼巣/适配/SQL数据库适配.cpp:486</code> | direct_free+direct-free+direct-free | <code>L"审计查询准入",拒绝说明</code> | 数量上限不在1到100；连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0371 | F0056 | F0203 | <code>海中鱼巣/适配/SQL数据库适配.cpp:477</code> | direct_const_member+unique_name+direct-member | <code>this</code> | 查询准入通过；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0407 | F0056 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:484-531</code> | implicit_destructor | <code>连接</code> | 连接构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0372 | F0056 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:484</code> | direct_constructor+constructor | <code>无</code> | 初始化成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0373 | F0056 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:485</code> | direct_member+direct-member | <code>配置_,配置_.数据库,诊断</code> | 连接对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0406 | F0056 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489-531</code> | implicit_destructor | <code>语句</code> | 语句构造完成后的任一离开路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0375 | F0056 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489</code> | direct_const_member+direct-member | <code>连接</code> | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0376 | F0056 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:489</code> | direct_constructor+constructor | <code>连接.句柄()</code> | 连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0377 | F0056 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:490</code> | direct_const_member+direct-member | <code>语句</code> | 语句对象已构造；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0378 | F0056 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:491, 海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:510, 海中鱼巣/适配/SQL数据库适配.cpp:523</code> | direct_free+direct-free+direct-free+direct-free+direct-free | <code>L"分配审计查询语句",说明</code> | 语句不可用；查询执行失败；Fetch失败；任一列转换失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0379 | F0056 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:499, 海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:505, 海中鱼巣/适配/SQL数据库适配.cpp:510, 海中鱼巣/适配/SQL数据库适配.cpp:515, 海中鱼巣/适配/SQL数据库适配.cpp:516, 海中鱼巣/适配/SQL数据库适配.cpp:517, 海中鱼巣/适配/SQL数据库适配.cpp:518, 海中鱼巣/适配/SQL数据库适配.cpp:519, 海中鱼巣/适配/SQL数据库适配.cpp:520, 海中鱼巣/适配/SQL数据库适配.cpp:521, 海中鱼巣/适配/SQL数据库适配.cpp:522</code> | direct_const_member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member+direct-member | <code>语句</code> | 查询文本已构造；查询执行失败；每轮结果获取；Fetch失败；Fetch成功；第一列转换成功；前两列转换成功；前三列转换成功；前四列转换成功；前五列转换成功；前六列转换成功；前七列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0380 | F0056 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:499, 海中鱼巣/适配/SQL数据库适配.cpp:509</code> | direct_free+direct-free+direct-free | <code>SQLExecDirectW返回码</code> | 查询已执行；获取结果不是SQL_NO_DATA；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0382 | F0056 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:500, 海中鱼巣/适配/SQL数据库适配.cpp:510</code> | direct_free+direct-free+direct-free | <code>SQL_HANDLE_STMT,语句.句柄()</code> | 查询执行失败；Fetch失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0390 | F0056 | F0213 | <code>海中鱼巣/适配/SQL数据库适配.cpp:515, 海中鱼巣/适配/SQL数据库适配.cpp:518, 海中鱼巣/适配/SQL数据库适配.cpp:519, 海中鱼巣/适配/SQL数据库适配.cpp:520, 海中鱼巣/适配/SQL数据库适配.cpp:521, 海中鱼巣/适配/SQL数据库适配.cpp:522</code> | direct_free+direct-free+direct-free+direct-free+direct-free+direct-free+direct-free | <code>句柄,1,记录.审计编号</code> | Fetch成功；前三列转换成功；前四列转换成功；前五列转换成功；前六列转换成功；前七列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0392 | F0056 | F0216 | <code>海中鱼巣/适配/SQL数据库适配.cpp:516, 海中鱼巣/适配/SQL数据库适配.cpp:517</code> | direct_free+direct-free+direct-free | <code>句柄,2,记录.记录时间</code> | 第一列转换成功；前两列转换成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0408 | F0059 | F0190 | <code>海中鱼巣/领域/初始化.世界树.ixx:89</code> | direct_const_member | <code>自我存在节点</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0409 | F0059 | F0217 | <code>海中鱼巣/领域/初始化.世界树.ixx:93, 海中鱼巣/领域/初始化.世界树.ixx:94, 海中鱼巣/领域/初始化.世界树.ixx:95</code> | direct_const_member | <code>记录->主信息,0</code> | 记录存在且节点类型为存在；横向读取完成；纵向读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0413 | F0060 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:135</code> | direct_const_member | <code>this=&语素初始化读数</code> | 世界树初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0412 | F0060 | F0174 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:135</code> | direct_const_member | <code>this=&初始化结果</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0414 | F0060 | F0218 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:139, 海中鱼巣/界面/投影.控制面板启动.ixx:139-141</code> | direct_free+lsp_direct | <code>节点,关系,世界树根节点,初始化结果,语素初始化读数,自我相对坐标,当前父链</code> | 两项初始化成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0416 | F0061 | F0049 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:192</code> | direct_const_member | <code>this=&概念图初始化读数</code> | 语素初始化成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0415 | F0061 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:192</code> | direct_const_member | <code>this=&语素初始化读数</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0417 | F0061 | F0219 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:196, 海中鱼巣/界面/投影.控制面板启动.ixx:214</code> | direct_free+lsp_direct | <code>语素类型,语素类型</code> | 初始化前置通过；当前语素子项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0418 | F0061 | F0220 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:210</code> | direct_const_member | <code>this=*子项</code> | 当前子项指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0420 | F0061 | F0189 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:221</code> | direct_const_member | <code>this=*概念</code> | 当前概念指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0421 | F0061 | F0221 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:225</code> | direct_free+lsp_direct | <code>*概念,语素类型</code> | 当前概念初始化项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0422 | F0063 | F0222 | <code>海中鱼巣/界面/控制面板窗口.cpp:1701-1702</code> | constructor_via_make_unique | <code>控制面板,数据库,启动结构统计副本</code> | PImpl存储分配成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0423 | F0064 | F0223 | <code>海中鱼巣/界面/控制面板窗口.cpp:1708</code> | direct_private_member+direct-member | <code>停止请求</code> | 构造后实现指针有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0424 | F0065 | F0224 | <code>海中鱼巣/界面/控制面板窗口.cpp:1705</code> | implicit_delete_destructor | <code>*实现</code> | unique_ptr非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0425 | F0066 | F0077 | <code>海中鱼巣/启动.运行期上下文.ixx:195</code> | direct_const_member | <code>this=上下文_.get()</code> | shared_ptr非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0426 | F0069 | F0226 | <code>海中鱼巣/启动.运行期上下文.ixx:235</code> | private_constructor | <code>当前上下文_的const共享副本</code> | 发布锁已持有 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0034 | F0071 | F0450 | <code>海中鱼巣/生产运行期配置.数据.h:77, 海中鱼巣/生产运行期配置.数据.h:79</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0035 | F0071 | F0452 | <code>海中鱼巣/生产运行期配置.数据.h:77, 海中鱼巣/生产运行期配置.数据.h:79</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0427 | F0071 | F0070 | <code>海中鱼巣/生产运行期配置.数据.h:78</code> | direct_free | <code>无</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0428 | F0071 | F0227 | <code>海中鱼巣/生产运行期配置.数据.h:78</code> | direct_const_member | <code>固定配置</code> | 固定配置已形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0429 | F0071 | F0228 | <code>海中鱼巣/生产运行期配置.数据.h:79</code> | direct_const_member | <code>this=&系统角色</code> | 配置与固定配置相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0430 | F0071 | F0229 | <code>海中鱼巣/生产运行期配置.数据.h:79</code> | direct_const_member | <code>this=&概念活动</code> | 系统角色有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0431 | F0071 | F0230 | <code>海中鱼巣/生产运行期配置.数据.h:82</code> | direct_const_member | <code>this=&系统角色</code> | 三个前置均通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0432 | F0071 | F0231 | <code>海中鱼巣/生产运行期配置.数据.h:83</code> | direct_const_member | <code>this=&概念活动</code> | 系统键组已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0433 | F0072 | F0232 | <code>海中鱼巣/启动.运行期上下文.ixx:45</code> | direct_constructor | <code>域编号</code> | 成员初始化开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0434 | F0072 | F0233 | <code>海中鱼巣/启动.运行期上下文.ixx:46</code> | direct_member | <code>this=&协调器_</code> | 协调器_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0435 | F0072 | F0135 | <code>海中鱼巣/启动.运行期上下文.ixx:47</code> | direct_constructor | <code>仓库编号,接线_副本</code> | 接线_初始化完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0436 | F0072 | F0136 | <code>海中鱼巣/启动.运行期上下文.ixx:48</code> | direct_constructor | <code>主信息_,仓库编号,接线_副本</code> | 主信息_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0437 | F0072 | F0137 | <code>海中鱼巣/启动.运行期上下文.ixx:49</code> | direct_constructor | <code>节点_,仓库编号,接线_副本</code> | 节点_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0438 | F0072 | F0138 | <code>海中鱼巣/启动.运行期上下文.ixx:50</code> | direct_constructor | <code>节点_,接线_副本</code> | 关系_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0439 | F0072 | F0238 | <code>海中鱼巣/启动.运行期上下文.ixx:51-52</code> | direct_constructor | <code>接线_,主信息_,节点_,关系_,索引_,仓库编号,稳定动作键</code> | 四仓库构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0443 | F0073 | F0241 | <code>海中鱼巣/启动.运行期上下文.ixx:100</code> | direct_const_member | <code>this=*系统角色清单_,参数</code> | 缓存存在且持有系统角色锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0444 | F0073 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:103</code> | direct_const_member | <code>*系统角色清单_</code> | 缓存存在且参数匹配，仍持锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0445 | F0073 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:104, 海中鱼巣/启动.运行期上下文.ixx:109</code> | direct_const_member | <code>this=&当前</code> | 缓存复核已返回；下层初始化已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0446 | F0073 | F0243 | <code>海中鱼巣/启动.运行期上下文.ixx:108</code> | direct_member | <code>参数</code> | 首次缓存为空且第一把锁已释放 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0448 | F0073 | F0244 | <code>海中鱼巣/启动.运行期上下文.ixx:111</code> | defaulted_const_member | <code>this=*系统角色清单_,结果.清单</code> | 第二次加锁后缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0442 | F0073 | F0228 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member | <code>this=&参数</code> | 业务装配完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0440 | F0073 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member+unique_name | <code>this</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0441 | F0073 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:96</code> | direct_const_member | <code>this=&业务装配_</code> | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0449 | F0074 | F0245 | <code>海中鱼巣/领域/系统角色清单.数据.h:297</code> | direct_const_member | <code>this=&清单</code> | 状态为已提交或幂等读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0452 | F0075 | F0229 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member | <code>this=&参数</code> | 业务装配完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0450 | F0075 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member+unique_name | <code>this</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0451 | F0075 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:130</code> | direct_const_member | <code>this=&业务装配_</code> | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0453 | F0075 | F0246 | <code>海中鱼巣/启动.运行期上下文.ixx:131</code> | direct_const_member+unique_name | <code>this</code> | 三项准入通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0454 | F0075 | F0245 | <code>海中鱼巣/启动.运行期上下文.ixx:132</code> | direct_const_member | <code>this=&清单.value</code> | 清单存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0456 | F0075 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:133</code> | direct_const_member | <code>this=&系统角色复核临时结果</code> | 复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0455 | F0075 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:133</code> | direct_const_member | <code>*清单</code> | 清单完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0457 | F0075 | F0247 | <code>海中鱼巣/启动.运行期上下文.ixx:137</code> | direct_const_member | <code>this=*概念活动材料_,参数</code> | 持锁且缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0458 | F0075 | F0248 | <code>海中鱼巣/启动.运行期上下文.ixx:140</code> | direct_const_member+unique_name | <code>*清单,*概念活动材料_</code> | 缓存参数匹配，仍持锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0459 | F0075 | F0076 | <code>海中鱼巣/启动.运行期上下文.ixx:141, 海中鱼巣/启动.运行期上下文.ixx:146</code> | direct_const_member | <code>this=&当前</code> | 缓存复核已返回；下层初始化已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0460 | F0075 | F0249 | <code>海中鱼巣/启动.运行期上下文.ixx:145</code> | direct_member | <code>*清单,参数</code> | 缓存为空且第一把锁已释放 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0462 | F0075 | F0250 | <code>海中鱼巣/启动.运行期上下文.ixx:148</code> | direct_const_member | <code>this=*概念活动材料_,结果.材料</code> | 第二次加锁后缓存存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0463 | F0076 | F0251 | <code>海中鱼巣/领域/概念活动状态.数据.h:213</code> | direct_const_member | <code>this=&材料</code> | 状态为已提交或幂等读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0464 | F0077 | F0246 | <code>海中鱼巣/启动.运行期上下文.ixx:75</code> | direct_const_member+unique_name | <code>this</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0465 | F0077 | F0252 | <code>海中鱼巣/启动.运行期上下文.ixx:76</code> | direct_const_member+unique_name | <code>this</code> | 清单已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0466 | F0077 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:77</code> | direct_const_member+unique_name | <code>this</code> | 两份材料均已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0467 | F0077 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:77</code> | direct_const_member | <code>this=&业务装配_</code> | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0470 | F0077 | F0074 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | <code>this=&系统角色复核临时结果</code> | 系统角色复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0469 | F0077 | F0242 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | <code>*清单</code> | 清单完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0468 | F0077 | F0245 | <code>海中鱼巣/启动.运行期上下文.ixx:78</code> | direct_const_member | <code>this=&清单.value</code> | 装配完整且清单存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0471 | F0077 | F0251 | <code>海中鱼巣/启动.运行期上下文.ixx:79</code> | direct_const_member | <code>this=&活动.value</code> | 系统角色复核成功且活动存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0473 | F0077 | F0076 | <code>海中鱼巣/启动.运行期上下文.ixx:80</code> | direct_const_member | <code>this=&概念活动复核临时结果</code> | 概念活动复核已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0472 | F0077 | F0248 | <code>海中鱼巣/启动.运行期上下文.ixx:80</code> | direct_const_member+unique_name | <code>*清单,*活动</code> | 活动完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0474 | F0078 | F0239 | <code>海中鱼巣/启动.运行期上下文.ixx:213</code> | direct_const_member | <code>this=候选.get()</code> | 候选非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0475 | F0078 | F0253 | <code>海中鱼巣/启动.运行期上下文.ixx:216</code> | direct_const_member | <code>this=候选.get()</code> | 结构核心完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0476 | F0078 | F0077 | <code>海中鱼巣/启动.运行期上下文.ixx:219, 海中鱼巣/启动.运行期上下文.ixx:226</code> | direct_const_member | <code>this=候选.get()</code> | 服务装配完整；持有发布锁且当前上下文为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0479 | F0080 | F0254 | <code>海中鱼巣/自检.入口初始化.ixx:486</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0481 | F0081 | F0255 | <code>海中鱼巣/自检.入口初始化.ixx:488</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0483 | F0082 | F0256 | <code>海中鱼巣/自检.入口初始化.ixx:490</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0485 | F0083 | F0257 | <code>海中鱼巣/自检.入口初始化.ixx:492</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0487 | F0084 | F0258 | <code>海中鱼巣/自检.入口初始化.ixx:494</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0489 | F0085 | F0259 | <code>海中鱼巣/自检.入口初始化.ixx:496</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0491 | F0086 | F0260 | <code>海中鱼巣/自检.入口初始化.ixx:498</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0493 | F0087 | F0261 | <code>海中鱼巣/自检.入口初始化.ixx:500</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0495 | F0088 | F0262 | <code>海中鱼巣/自检.入口初始化.ixx:502</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0497 | F0089 | F0263 | <code>海中鱼巣/自检.入口初始化.ixx:504</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0499 | F0090 | F0264 | <code>海中鱼巣/自检.入口初始化.ixx:506</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0501 | F0091 | F0265 | <code>海中鱼巣/自检.入口初始化.ixx:508</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0503 | F0092 | F0266 | <code>海中鱼巣/自检.入口初始化.ixx:510</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0505 | F0093 | F0267 | <code>海中鱼巣/自检.入口初始化.ixx:512</code> | direct_free | <code>捕获配置</code> | 回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0511 | F0096 | F0272 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:878</code> | constructor | <code>0,固定随机种子</code> | F0096执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0512 | F0096 | F0273 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:879</code> | direct_member_nonvirtual | <code>this=&无效夹具</code> | 夹具构造成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0513 | F0097 | F0272 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:761</code> | constructor | <code>规模,固定随机种子</code> | 报告基本字段已写 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0514 | F0097 | F0273 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:763</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 构建开始时间已记录；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0515 | F0097 | F0274 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:766</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 夹具构建成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0516 | F0097 | F0275 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:767</code> | direct_member_nonvirtual | <code>this=&夹具</code> | 结构签名已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0517 | F0097 | F0276 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:768</code> | direct_member_nonvirtual | <code>this=&夹具</code> | 初始数量已读取 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0518 | F0097 | F0277 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:769</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 分布结果已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0519 | F0097 | F0278 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:771</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 夹具摘要字段已读取；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0520 | F0097 | F0279 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:782, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:788</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具,定义</code> | 预热批内循环；正式批内循环；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0522 | F0097 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:797</code> | direct_free | <code>本轮汇总样本,0.50</code> | 本轮全部查询测量完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0523 | F0097 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:801-805</code> | direct_free | <code>查询名称,查询样本,初始当前记录数量,累计正确</code> | 指标索引有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0524 | F0097 | F0282 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:812</code> | direct_free | <code>夹具,查询定义组[1],报告.查询指标组[1].中位数纳秒</code> | 查询指标形成完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0525 | F0097 | F0283 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:813</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 并发基线返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0526 | F0097 | F0284 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:815</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 参考模型一致位已计算，不要求为true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0527 | F0097 | F0285 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:816</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 创建测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0528 | F0097 | F0286 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:817</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 失效测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0529 | F0097 | F0287 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:818</code> | direct_member_nonvirtual+unique_name | <code>this=&夹具</code> | 重挂测量完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0531 | F0097 | F0198 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:819</code> | direct_member_nonvirtual | <code>this=&夹具关系仓库</code> | 关系仓库引用已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0530 | F0097 | F0288 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:819</code> | direct_member_nonvirtual | <code>this=&夹具</code> | 四项写入测量完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0036 | F0097 | R0139 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:819</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E0532 | F0098 | F0289 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:904-906</code> | external_callback | <code>当前查询分位指标const引用</code> | 参考模型一致且标准算法遍历未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0533 | F0098 | F0290 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:907-909</code> | external_callback | <code>当前写入分位指标const引用</code> | 参考模型与查询指标一致且标准算法遍历未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0534 | F0099 | F0291 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:741-743</code> | external_callback | <code>当前查询分位指标const引用</code> | 基础字段和查询写入指标数量门禁通过且算法未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0535 | F0099 | F0292 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:744-746</code> | external_callback | <code>当前写入分位指标const引用</code> | 基础字段和两组数量门禁通过、查询指标全部完整且算法未短路 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0536 | F0099 | F0293 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:747</code> | direct_const_member | <code>this=&并发</code> | 查询和写入指标全部完整 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0200 | F0120 | F0043 | <code>海中鱼巣/启动.应用程序.ixx:209</code> | captured_member_direct+direct_member | <code>键, 上下文.状态</code> | 失败点!=0；实际五轮；失败点 != 0 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0201 | F0121 | F0044 | <code>海中鱼巣/启动.应用程序.ixx:215</code> | captured_member_direct+direct_member | <code>参数</code> | 失败点!=1；实际四轮；失败点 != 1 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0202 | F0122 | F0045 | <code>海中鱼巣/启动.应用程序.ixx:221</code> | captured_member_direct+direct_member | <code>时限</code> | 失败点!=2；实际三轮；失败点 != 2 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0203 | F0123 | F0046 | <code>海中鱼巣/启动.应用程序.ixx:227</code> | captured_member_direct+direct_member | <code>上下文.自我线程实例隐式this</code> | 失败点!=3；实际三轮；失败点 != 3 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0204 | F0124 | F0048 | <code>海中鱼巣/启动.应用程序.ixx:233</code> | captured_member_direct+direct_member | <code>上下文.概念图初始化隐式this</code> | 失败点!=4；实际一轮；失败点 != 4 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0611 | F0125 | F0050 | <code>海中鱼巣/启动.应用程序.ixx:239</code> | direct_member | <code>上下文.概念图隐式this,实例</code> | 失败点 != 5 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0613 | F0135 | F0321 | <code>海中鱼巣/核心/主信息仓库.cpp:33</code> | direct_member+unique_name | <code>事务接线_隐式this</code> | 全部成员构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0614 | F0136 | F0321 | <code>海中鱼巣/核心/节点仓库.cpp:45</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0616 | F0136 | F0322 | <code>海中鱼巣/核心/节点仓库.cpp:48</code> | direct_free_function+direct-free | <code>事务接线_,主信息_.事务接线_</code> | 两个接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0617 | F0137 | F0321 | <code>海中鱼巣/核心/关系仓库.cpp:171</code> | direct_member+unique_name | <code>事务接线_隐式this</code> | 全部成员构造完成；本仓库接线形态有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0619 | F0137 | F0323 | <code>海中鱼巣/核心/关系仓库.cpp:174</code> | direct_free+direct-free | <code>事务接线_,节点_.事务接线_</code> | 两份接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0620 | F0138 | F0321 | <code>海中鱼巣/核心/索引仓库.cpp:94</code> | direct_member+unique_name | <code>事务接线_隐式this</code> | 全部成员构造完成；本仓库接线形态有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0622 | F0138 | F0324 | <code>海中鱼巣/核心/索引仓库.cpp:97</code> | direct_free+direct-free | <code>事务接线_,节点_.事务接线_</code> | 两份接线形态均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0623 | F0158 | F0325 | <code>海中鱼巣/适配/SQL数据库适配.cpp:316, 海中鱼巣/适配/SQL数据库适配.cpp:319</code> | direct_free+direct-free+direct-free | <code>L"HY_EGO_SQL_SERVER"</code> | 默认配置已构造；服务器覆盖分支已汇合；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0625 | F0160 | F0326 | <code>海中鱼巣/领域/方法服务.h:1337</code> | direct_member | <code>*登记根材料_</code> | optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0626 | F0160 | F0327 | <code>海中鱼巣/领域/方法服务.h:1338</code> | direct_member+unique_name | <code>登记根材料_->登记根,状态</code> | 材料完整且仓库编号非零；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0627 | F0160 | F0328 | <code>海中鱼巣/领域/方法服务.h:1339, 海中鱼巣/领域/方法服务.h:1340</code> | direct_member+unique_name | <code>登记根材料_->活跃状态</code> | 登记根角色匹配；活跃不是实例状态；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0629 | F0160 | F0329 | <code>海中鱼巣/领域/方法服务.h:1343, 海中鱼巣/领域/方法服务.h:1344</code> | direct_member | <code>this=&状态,登记根材料_->活跃状态；this=&状态,登记根材料_->失效状态</code> | 1337—1340前置全部通过；1343读取活跃状态值后，1344无条件读取失效状态值；第二次不依赖第一次返回内容 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0631 | F0160 | F0330 | <code>海中鱼巣/领域/方法服务.h:1345-1346, 海中鱼巣/领域/方法服务.h:1347-1348</code> | direct_member | <code>this=&方法服务,登记根材料_->登记根,关系类型::模板,节点类型::状态,根活跃状态顺序号；this=&方法服务,登记根材料_->登记根,关系类型::模板,节点类型::状态,根失效状态顺序号</code> | E0629两次状态值读取完成；1345—1346读取活跃状态目标后，1347—1348无条件读取失效状态目标；第二次不依赖第一次返回内容 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0633 | F0161 | F0184 | <code>海中鱼巣/领域/方法服务.h:1304</code> | direct_free | <code>false,说明</code> | 可能的解锁已完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0634 | F0162 | F0331 | <code>海中鱼巣/领域/方法服务.h:310</code> | direct_member | <code>this=&主信息_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0635 | F0162 | F0332 | <code>海中鱼巣/领域/方法服务.h:310</code> | direct_member | <code>节点类型::方法,新主信息</code> | 主信息创建调用完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0636 | F0164 | F0333 | <code>海中鱼巣/领域/方法服务.h:1645</code> | direct_member | <code>方法节点,节点类型::方法</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0637 | F0164 | F0334 | <code>海中鱼巣/领域/方法服务.h:1645</code> | direct_member+unique_name | <code>角色</code> | 方法节点类型匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0638 | F0164 | F0165 | <code>海中鱼巣/领域/方法服务.h:1648</code> | direct_member | <code>static_cast<int64_t>(角色)</code> | 输入准入通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0639 | F0164 | F0163 | <code>海中鱼巣/领域/方法服务.h:1649</code> | direct_free | <code>角色状态</code> | 抽象状态创建完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0641 | F0164 | F0168 | <code>海中鱼巣/领域/方法服务.h:1649, 海中鱼巣/领域/方法服务.h:1653</code> | direct_free+unique_name | <code>角色关系</code> | 关系创建完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0640 | F0164 | F0167 | <code>海中鱼巣/领域/方法服务.h:1652</code> | direct_member | <code>模板,方法节点,角色状态,0</code> | 状态句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0642 | F0165 | F0335 | <code>海中鱼巣/领域/状态服务.h:158</code> | direct_member+unique_name | <code>状态值,std::nullopt</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0643 | F0165 | F0329 | <code>海中鱼巣/领域/状态服务.h:159</code> | direct_member+suffix_match | <code>状态节点</code> | 状态节点创建调用完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0645 | F0165 | F0184 | <code>海中鱼巣/领域/状态服务.h:160, 海中鱼巣/领域/状态服务.h:160-161</code> | direct_free+unique_name | <code>复核条件,固定说明</code> | 短路复核完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0644 | F0165 | F0328 | <code>海中鱼巣/领域/状态服务.h:160</code> | direct_member+unique_name | <code>状态节点</code> | 读回值存在且等于输入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0662 | F0167 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:185-275</code> | direct_destructor | <code>自动令牌范围</code> | 已构造令牌范围离开作用域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0663 | F0167 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:185-275</code> | direct_destructor | <code>自动许可</code> | 已取得许可离开作用域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0646 | F0167 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | <code>事务接线_隐式this</code> | 许可宏进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0647 | F0167 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:185, 海中鱼巣/核心/关系仓库.cpp:205, 海中鱼巣/核心/关系仓库.cpp:212, 海中鱼巣/核心/关系仓库.cpp:220</code> | direct_free+direct-free+direct-free+direct-free | <code>*this</code> | 已接域；任务方法选择类型；方法动作场景类型；用途观察证据角色类型；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0648 | F0167 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | <code>自动许可</code> | 动态取得许可完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0649 | F0167 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_member | <code>自动许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0650 | F0167 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:185</code> | direct_constructor | <code>*this,自动许可.读取令牌()</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0651 | F0167 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:186</code> | direct_free+direct-free | <code>类型</code> | 许可语境建立完成；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0655 | F0167 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:191</code> | direct_free | <code>源节点</code> | 关系类型有效；源句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0657 | F0167 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:196</code> | direct_member+direct-member+direct-member | <code>源节点</code> | 端点句柄有效；源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0665 | F0167 | F0190 | <code>海中鱼巣/核心/关系仓库.cpp:206, 海中鱼巣/核心/关系仓库.cpp:207, 海中鱼巣/核心/关系仓库.cpp:213, 海中鱼巣/核心/关系仓库.cpp:214, 海中鱼巣/核心/关系仓库.cpp:221, 海中鱼巣/核心/关系仓库.cpp:222</code> | direct_member | <code>源节点</code> | 任务方法选择且无令牌；方法动作场景且无令牌；方法动作场景且无令牌；用途观察且无令牌；用途观察且无令牌 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0664 | F0167 | F0346 | <code>海中鱼巣/核心/关系仓库.cpp:206, 海中鱼巣/核心/关系仓库.cpp:207, 海中鱼巣/核心/关系仓库.cpp:213, 海中鱼巣/核心/关系仓库.cpp:214, 海中鱼巣/核心/关系仓库.cpp:221, 海中鱼巣/核心/关系仓库.cpp:222</code> | direct_member | <code>源节点,*当前令牌</code> | 任务方法选择且有令牌；方法动作场景且有令牌；方法动作场景且有令牌；用途观察且有令牌；用途观察且有令牌 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0676 | F0167 | F0347 | <code>海中鱼巣/核心/关系仓库.cpp:223</code> | direct_free+direct-free | <code>顺序号</code> | 用途观察类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0677 | F0167 | F0348 | <code>海中鱼巣/核心/关系仓库.cpp:225</code> | direct_free+direct-free | <code>顺序号,目标记录->类型</code> | 角色、源与目标记录前置成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0678 | F0171 | F0349 | <code>海中鱼巣/线程/自我线程.ixx:205</code> | lambda_direct_member | <code>捕获参数副本</code> | 工作线程入口执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0679 | F0173 | F0220 | <code>海中鱼巣/领域/初始化.语素.ixx:68, 海中鱼巣/领域/初始化.语素.ixx:69, 海中鱼巣/领域/初始化.语素.ixx:70, 海中鱼巣/领域/初始化.语素.ixx:71, 海中鱼巣/领域/初始化.语素.ixx:72, 海中鱼巣/领域/初始化.语素.ixx:73, 海中鱼巣/领域/初始化.语素.ixx:74, 海中鱼巣/领域/初始化.语素.ixx:75, 海中鱼巣/领域/初始化.语素.ixx:76, 海中鱼巣/领域/初始化.语素.ixx:77, 海中鱼巣/领域/初始化.语素.ixx:78, 海中鱼巣/领域/初始化.语素.ixx:79, 海中鱼巣/领域/初始化.语素.ixx:80, 海中鱼巣/领域/初始化.语素.ixx:81, 海中鱼巣/领域/初始化.语素.ixx:82, 海中鱼巣/领域/初始化.语素.ixx:83, 海中鱼巣/领域/初始化.语素.ixx:84, 海中鱼巣/领域/初始化.语素.ixx:85</code> | direct_member | <code>通用节点名称</code> | 函数进入；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功；前项成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0697 | F0174 | F0163 | <code>海中鱼巣/领域/初始化.世界树.ixx:34, 海中鱼巣/领域/初始化.世界树.ixx:35, 海中鱼巣/领域/初始化.世界树.ixx:36</code> | direct_free | <code>世界树根节点</code> | 函数进入；根有效；场景有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0700 | F0174 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:34, 海中鱼巣/领域/初始化.世界树.ixx:35, 海中鱼巣/领域/初始化.世界树.ixx:36, 海中鱼巣/领域/初始化.世界树.ixx:37, 海中鱼巣/领域/初始化.世界树.ixx:38, 海中鱼巣/领域/初始化.世界树.ixx:39</code> | direct_free+unique_name | <code>根到场景关系</code> | 自我节点有效；根场景关系有效；父子关系有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0705 | F0175 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | direct_free | <code>概念命名目标状态</code> | 两个单根需求均成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0037 | F0175 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0703 | F0175 | F0350 | <code>海中鱼巣/领域/初始化.需求.ixx:63</code> | direct_const_member | <code>this=&安全根需求</code> | 函数进入；安全根需求成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0706 | F0176 | F0189 | <code>海中鱼巣/领域/初始化.概念图.ixx:224</code> | direct_const_member | <code>this=根</code> | 当前根指针非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0707 | F0176 | F0351 | <code>海中鱼巣/领域/初始化.概念图.ixx:224</code> | direct_const_member+unique_name | <code>this=概念图初始化服务,*根</code> | 当前根项成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0708 | F0176 | F0352 | <code>海中鱼巣/领域/初始化.概念图.ixx:227</code> | direct_const_member+unique_name | <code>this=&概念图_,类别=根->根类别</code> | 根项与名称关系均有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0709 | F0176 | F0353 | <code>海中鱼巣/领域/初始化.概念图.ixx:233</code> | direct_const_member+unique_name | <code>this=&概念图_,概念=根->根节点</code> | 登记材料匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0710 | F0176 | F0354 | <code>海中鱼巣/领域/初始化.概念图.ixx:246</code> | direct_const_member+unique_name | <code>this=&概念图_,阶段</code> | 进入当前状态循环项；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0711 | F0176 | F0329 | <code>海中鱼巣/领域/初始化.概念图.ixx:247</code> | direct_const_member | <code>this=&状态_,状态节点</code> | 读取生命周期状态之后无条件执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0712 | F0176 | F0328 | <code>海中鱼巣/领域/初始化.概念图.ixx:252</code> | direct_const_member+unique_name | <code>this=&状态_,状态节点</code> | 登记与状态值前四项均匹配；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0713 | F0176 | F0355 | <code>海中鱼巣/领域/初始化.概念图.ixx:256</code> | direct_const_member+unique_name | <code>this=&概念图_</code> | 四根与三状态循环均完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE1636 | F0177 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1637 | F0177 | F0185 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke | <code>未单独冻结；读取源码调用点</code> | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1638 | F0177 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1639 | F0177 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member | <code>未单独冻结；读取源码调用点</code> | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1640 | F0177 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1641 | F0177 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator | <code>未单独冻结；读取源码调用点</code> | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1642 | F0178 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1643 | F0178 | F0186 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke | <code>未单独冻结；读取源码调用点</code> | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1644 | F0178 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1645 | F0178 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member | <code>未单独冻结；读取源码调用点</code> | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1646 | F0178 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1647 | F0178 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator | <code>未单独冻结；读取源码调用点</code> | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1648 | F0179 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| RCE1649 | F0179 | F0187 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke | <code>未单独冻结；读取源码调用点</code> | 不存在已登记根时调用本实例绑定的创建回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1650 | F0179 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 创建回调返回后复核根节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1651 | F0179 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member | <code>未单独冻结；读取源码调用点</code> | 根节点有效后登记概念根 | 专项源码静态类型与实际装配人工复核 |
| RCE1652 | F0179 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 登记完成后复核发布读回 | 专项源码静态类型与实际装配人工复核 |
| RCE1653 | F0179 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | operator | <code>未单独冻结；读取源码调用点</code> | 登记结果有值后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| E0714 | F0180 | F0352 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | <code>this=&概念图_,类别=存在</code> | 函数进入；F0177成功后进入F0178；F0177和F0178成功后进入F0179；前三根均成功后进入F0180 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE1654 | F0180 | R0531 | <code>海中鱼巣/领域/初始化.概念图.ixx:150</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取同类别已登记根 | 专项源码静态类型与实际装配人工复核 |
| E0733 | F0180 | F0188 | <code>海中鱼巣/领域/初始化.概念图.ixx:161</code> | callback_invoke | <code>创建回调，无参数</code> | 因果根未登记；不存在已登记根时调用本实例绑定的创建回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0716 | F0180 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:162</code> | direct_free | <code>根节点</code> | 创建回调返回；F0186返回；F0187返回；F0188返回；创建回调返回后复核根节点句柄 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0717 | F0180 | F0356 | <code>海中鱼巣/领域/初始化.概念图.ixx:165</code> | direct_member | <code>this=&概念图_,材料={存在,固定稳定键,根节点}</code> | 根节点有效；根节点有效后登记概念根 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0719 | F0180 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:166-167</code> | direct_free | <code>登记有值且同根,L"概念根创建后登记不符合内部预期。"</code> | 登记完成；条件真假均调用；登记完成后复核发布读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0718 | F0180 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:166</code> | direct_operator+operator | <code>登记结果.value(),根节点</code> | 登记结果有值；登记结果有值后比较根节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0738 | F0181 | F0354 | <code>海中鱼巣/领域/初始化.概念图.ixx:187</code> | direct_const_member+unique_name | <code>this=&概念图_,阶段</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0739 | F0181 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:192, 海中鱼巣/领域/初始化.概念图.ixx:195</code> | direct_free | <code>状态节点</code> | 无已登记状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0038 | F0181 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:192, 海中鱼巣/领域/初始化.概念图.ixx:195</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0740 | F0181 | F0165 | <code>海中鱼巣/领域/初始化.概念图.ixx:193</code> | direct_member | <code>this=&状态_,状态值=int64(阶段)</code> | 无登记且候选句柄无效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0742 | F0181 | F0357 | <code>海中鱼巣/领域/初始化.概念图.ixx:198</code> | direct_member+unique_name | <code>this=&概念图_,阶段,状态节点,状态_</code> | 状态节点有效；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0743 | F0181 | F0051 | <code>海中鱼巣/领域/初始化.概念图.ixx:199</code> | direct_operator | <code>登记结果.value(),状态节点</code> | 登记结果有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0744 | F0181 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:199, 海中鱼巣/领域/初始化.概念图.ixx:199-200, 海中鱼巣/领域/初始化.概念图.ixx:205, 海中鱼巣/领域/初始化.概念图.ixx:205-209</code> | direct_free+unique_name | <code>登记有值且同状态,固定说明</code> | 登记尝试后总调用；读回状态值后总调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0745 | F0181 | F0329 | <code>海中鱼巣/领域/初始化.概念图.ixx:204</code> | direct_const_member | <code>this=&状态_,状态节点</code> | 复用或登记路径汇合 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0746 | F0181 | F0328 | <code>海中鱼巣/领域/初始化.概念图.ixx:208</code> | direct_const_member+unique_name | <code>this=&状态_,状态节点</code> | 状态值存在且等于阶段；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0752 | F0182 | F0360 | <code>海中鱼巣/领域/概念图服务.h:1007</code> | direct_const_member | <code>根节点</code> | 四根确保全部成功后的逐根读回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0753 | F0182 | F0361 | <code>海中鱼巣/领域/概念图服务.h:1016</code> | direct_member+unique_name | <code>本轮新增关系组</code> | 任一确保或读回失败；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0755 | F0182 | F0184 | <code>海中鱼巣/领域/概念图服务.h:1017, 海中鱼巣/领域/概念图服务.h:1017-1019, 海中鱼巣/领域/概念图服务.h:1023, 海中鱼巣/领域/概念图服务.h:1023-1024</code> | direct_free+unique_name | <code>清理完成且关系数恢复,固定说明</code> | 失败清理路径总调用；活动快照存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0750 | F0182 | F0198 | <code>海中鱼巣/领域/概念图服务.h:1018, 海中鱼巣/领域/概念图服务.h:992</code> | direct_const_member | <code>this=&关系_</code> | 根数量等于4且双锁已取得；清理函数返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0756 | F0182 | F0362 | <code>海中鱼巣/领域/概念图服务.h:1023</code> | direct_const_member+unique_name | <code>活动快照_.value()</code> | 全部就绪且活动快照存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0748 | F0182 | F0358 | <code>海中鱼巣/领域/概念图服务.h:982</code> | direct_const_member+unique_name | <code>this</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0749 | F0182 | F0355 | <code>海中鱼巣/领域/概念图服务.h:985</code> | direct_const_member+unique_name | <code>this</code> | 生命周期状态登记完整；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0751 | F0182 | F0359 | <code>海中鱼巣/领域/概念图服务.h:996, 海中鱼巣/领域/概念图服务.h:996-1000</code> | direct_member+unique_name | <code>根节点,活跃,true,本轮新增关系组&</code> | 逐根确保循环；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0758 | F0183 | F0363 | <code>海中鱼巣/领域/初始化.概念图.ixx:177, 海中鱼巣/领域/初始化.概念图.ixx:182</code> | direct_const_member | <code>this=&项.名称语素入口</code> | 函数进入；创建和字段写入后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0759 | F0183 | F0351 | <code>海中鱼巣/领域/初始化.概念图.ixx:178, 海中鱼巣/领域/初始化.概念图.ixx:182</code> | direct_const_member+unique_name | <code>this=概念图初始化服务,项</code> | 已有名称材料成功；新材料成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0760 | F0183 | F0364 | <code>海中鱼巣/领域/初始化.概念图.ixx:180</code> | direct_member+unique_name | <code>this=&语素_,名称,项.根节点,项.根节点,0</code> | 无成功名称材料；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0763 | F0183 | F0184 | <code>海中鱼巣/领域/初始化.概念图.ixx:182, 海中鱼巣/领域/初始化.概念图.ixx:182-183</code> | direct_free+unique_name | <code>新材料成功且关系可读,固定说明</code> | 创建路径总调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0765 | F0185 | F0365 | <code>海中鱼巣/领域/初始化.概念图.ixx:105</code> | lambda_direct | <code>this=&捕获this->存在_,无显式参数</code> | F0177未读到存在根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0766 | F0186 | F0366 | <code>海中鱼巣/领域/初始化.概念图.ixx:107</code> | lambda_direct | <code>this=&捕获this->动态_,无显式参数</code> | F0180链到达F0178且F0178未读到动态根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0767 | F0187 | F0367 | <code>海中鱼巣/领域/初始化.概念图.ixx:109</code> | lambda_direct | <code>this=&捕获this->二次特征_,无显式参数</code> | F0179到达且未读到关系根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0768 | F0188 | F0368 | <code>海中鱼巣/领域/初始化.概念图.ixx:111</code> | lambda_direct | <code>this=&捕获this->因果_,无显式参数</code> | F0180到达且未读到因果根登记 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0769 | F0189 | F0163 | <code>海中鱼巣/领域/初始化.概念图.ixx:34</code> | direct_free | <code>根节点</code> | 类别范围和稳定键均通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0040 | F0189 | F0168 | <code>海中鱼巣/领域/初始化.概念图.ixx:34</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0770 | F0189 | F0363 | <code>海中鱼巣/领域/初始化.概念图.ixx:35</code> | direct_const_member | <code>this=&名称语素入口</code> | 根句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0771 | F0190 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:234</code> | direct_const_member | <code>this=&事务接线_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0775 | F0190 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:235-237</code> | implicit_destructor | <code>this=&许可</code> | 已接域路径退出；返回对象形成之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0041 | F0190 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:235</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0772 | F0190 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member | <code>this=&许可</code> | 已接域且许可对象已形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0773 | F0190 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member | <code>this=&许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0774 | F0190 | F0346 | <code>海中鱼巣/核心/节点仓库.cpp:236</code> | direct_const_member+direct-member | <code>this=节点仓库,节点,许可令牌</code> | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0776 | F0191 | F0369 | <code>海中鱼巣/领域/概念图服务.h:4406</code> | direct_const_member+unique_name | <code>this,实例</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0777 | F0191 | F0352 | <code>海中鱼巣/领域/概念图服务.h:4407</code> | direct_const_member+unique_name | <code>this,类别</code> | F0369调用后无条件执行；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0778 | F0191 | F0051 | <code>海中鱼巣/领域/概念图服务.h:4411, 海中鱼巣/领域/概念图服务.h:4419, 海中鱼巣/领域/概念图服务.h:4420</code> | direct_operator+external_callback+external_callback | <code>实例,根材料->根节点</code> | 实例类别匹配且根材料存在；第一组find迭代；第一组命中后的第二组find迭代 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0779 | F0191 | F0370 | <code>海中鱼巣/领域/概念图服务.h:4414</code> | direct_member+unique_name | <code>this,实例,根材料->根节点</code> | 实例不是根；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0780 | F0191 | F0371 | <code>海中鱼巣/领域/概念图服务.h:4417</code> | direct_const_member+unique_name | <code>this,实例</code> | 绑定成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0781 | F0191 | F0372 | <code>海中鱼巣/领域/概念图服务.h:4418</code> | direct_const_member+unique_name | <code>this,根材料->根节点</code> | 实例目标组读取后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0784 | F0191 | F0184 | <code>海中鱼巣/领域/概念图服务.h:4421</code> | direct_free+unique_name | <code>双向可读,固定说明</code> | 两次find组合条件形成后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0785 | F0196 | F0373 | <code>海中鱼巣/领域/统计服务.h:258</code> | direct_const_member+unique_name | <code>this,命名空间</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0786 | F0197 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:454</code> | direct_const_member | <code>this=&事务接线_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0790 | F0197 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:455-457</code> | implicit_destructor | <code>this=&许可</code> | 接域路径退出；返回值形成后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0042 | F0197 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:455</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0787 | F0197 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member | <code>this=&许可</code> | 已接域且许可对象形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0788 | F0197 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member | <code>this=&许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0789 | F0197 | F0374 | <code>海中鱼巣/核心/节点仓库.cpp:456</code> | direct_const_member+direct-member | <code>this,许可令牌</code> | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0800 | F0198 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1013-1025</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0801 | F0198 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:1013-1025</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0793 | F0198 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:1013</code> | move-constructor+macro-expanded-move-constructor | <code>未单独冻结；读取源码调用点</code> | 共享许可宏的 optional.emplace 以许可临时量构造承载值；关系共享许可范围宏在 line 120 以 optional.emplace 接收函数返回值并移动构造结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| E0798 | F0198 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:1019, 海中鱼巣/核心/关系仓库.cpp:1020</code> | direct_const_member+direct-member+direct-member | <code>this,记录.源节点</code> | 记录状态有效；记录状态和源节点均有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0802 | F0199 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:403</code> | direct_const_member | <code>this=&事务接线_</code> | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0806 | F0199 | F0345 | <code>海中鱼巣/核心/索引仓库.cpp:404-406</code> | implicit_destructor | <code>this=&许可</code> | 接域分支退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0043 | F0199 | F0397 | <code>海中鱼巣/核心/索引仓库.cpp:404</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0803 | F0199 | F0338 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member | <code>this=&许可</code> | 事务接线已接域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0804 | F0199 | F0339 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member | <code>this=&许可</code> | 事务接线已接域且许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0805 | F0199 | F0376 | <code>海中鱼巣/核心/索引仓库.cpp:405</code> | direct_const_member+direct-member | <code>this,令牌=许可.读取令牌()</code> | 事务接线已接域且许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0807 | F0199 | F0377 | <code>海中鱼巣/核心/索引仓库.cpp:414</code> | loop_direct_const_member | <code>this=&节点_,节点=候选</code> | 未接域路径每个候选 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0808 | F0203 | F0378 | <code>海中鱼巣/适配/SQL数据库适配.cpp:330</code> | direct_free+source_audited | <code>第一次字段=配置_.服务器；第二次字段=配置_.数据库</code> | 第一次总是调用；仅第一次返回true时调用第二次 | 当前源码逐调用点与Clang AST左到右短路表达式复核 |
| E0810 | F0203 | F0202 | <code>海中鱼巣/适配/SQL数据库适配.cpp:332, 海中鱼巣/适配/SQL数据库适配.cpp:339</code> | direct_free+direct-free+direct-free | <code>L"数据库配置准入",固定诊断</code> | 任一配置准入失败；master连接打开失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0825 | F0203 | F0215 | <code>海中鱼巣/适配/SQL数据库适配.cpp:337-348, 海中鱼巣/适配/SQL数据库适配.cpp:350-379</code> | implicit_destructor | <code>this=&主连接</code> | 主连接作用域任一路径退出；函数任一路径退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0812 | F0203 | F0204 | <code>海中鱼巣/适配/SQL数据库适配.cpp:337, 海中鱼巣/适配/SQL数据库适配.cpp:350</code> | direct_constructor+constructor+constructor | <code>无</code> | 配置准入通过；建库SQL成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0814 | F0203 | F0205 | <code>海中鱼巣/适配/SQL数据库适配.cpp:338, 海中鱼巣/适配/SQL数据库适配.cpp:351</code> | direct_member+direct-member+direct-member | <code>配置_,L"master",诊断</code> | 配置准入通过；建库SQL成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0816 | F0203 | F0379 | <code>海中鱼巣/适配/SQL数据库适配.cpp:341</code> | direct_free+direct-free | <code>文本=配置_.数据库</code> | master连接打开成功；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0817 | F0203 | F0380 | <code>海中鱼巣/适配/SQL数据库适配.cpp:342</code> | direct_free+direct-free | <code>文本=配置_.数据库</code> | 数据库字符串已形成；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0818 | F0203 | F0206 | <code>海中鱼巣/适配/SQL数据库适配.cpp:345, 海中鱼巣/适配/SQL数据库适配.cpp:375</code> | indeterminately_sequenced_argument+direct-member+direct-member | <code>this=&主连接</code> | 建库SQL已形成；与move实参求值顺序不固定；建表SQL已形成；与move实参求值顺序不固定；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0819 | F0203 | F0381 | <code>海中鱼巣/适配/SQL数据库适配.cpp:345, 海中鱼巣/适配/SQL数据库适配.cpp:375</code> | direct_free+direct-free+direct-free | <code>主连接句柄,std::move(建库SQL),诊断</code> | 全部实参求值完成；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0822 | F0203 | F0212 | <code>海中鱼巣/适配/SQL数据库适配.cpp:346, 海中鱼巣/适配/SQL数据库适配.cpp:352, 海中鱼巣/适配/SQL数据库适配.cpp:376</code> | direct_free+direct-free+direct-free+direct-free | <code>L"初始化数据库",std::move(诊断),0</code> | 建库SQL失败；数据连接打开失败；建表SQL失败；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0827 | F0205 | F0382 | <code>海中鱼巣/适配/SQL数据库适配.cpp:133</code> | direct_member+suffix_match+direct-member | <code>this</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0828 | F0205 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:134, 海中鱼巣/适配/SQL数据库适配.cpp:138, 海中鱼巣/适配/SQL数据库适配.cpp:138-139, 海中鱼巣/适配/SQL数据库适配.cpp:150, 海中鱼巣/适配/SQL数据库适配.cpp:181</code> | nested_direct_free+loop_nested_direct_free+loop_direct_free+direct-free+direct-free+direct-free+direct-free | <code>ENV分配SQLRETURN</code> | F0382完成；ENV分配成功；每个驱动；DBC分配成功；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0830 | F0205 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:140, 海中鱼巣/适配/SQL数据库适配.cpp:188</code> | direct_free+loop_indeterminately_sequenced_argument+direct-free+direct-free | <code>SQL_HANDLE_ENV,环境_</code> | 环境属性设置失败；连接失败；与驱动名前缀求值顺序不固定；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0834 | F0207 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:224</code> | nested_direct_free+direct-free | <code>SQLAllocHandle语句分配结果</code> | 总是；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0835 | F0211 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:52</code> | loop_direct_free+lsp_direct+direct-free | <code>当前SQLGetDiagRecW结果</code> | 结果不等于SQL_NO_DATA；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0836 | F0213 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:301</code> | direct_free+lsp_direct+direct-free | <code>SQLGetData结果</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0837 | F0215 | F0382 | <code>海中鱼巣/适配/SQL数据库适配.cpp:126</code> | direct_member+suffix_match+direct-member | <code>this</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0838 | F0216 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:290</code> | direct_free+lsp_direct+direct-free | <code>SQLGetData结果</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| E0839 | F0217 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:431</code> | direct_const_member | <code>this=&事务接线_</code> | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0843 | F0217 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:432-434</code> | implicit_destructor | <code>this=&许可</code> | 接域分支退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0044 | F0217 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:432</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E0840 | F0217 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member | <code>this=&许可</code> | 已接域 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0841 | F0217 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member | <code>this=&许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0842 | F0217 | F0383 | <code>海中鱼巣/核心/主信息仓库.cpp:433</code> | direct_const_member+direct-member | <code>主信息,值索引,许可令牌</code> | 许可有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0844 | F0217 | F0384 | <code>海中鱼巣/核心/主信息仓库.cpp:435</code> | direct_const_member+direct-member | <code>this,主信息</code> | 未接域；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E0850 | F0218 | F0386 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:103, 海中鱼巣/界面/投影.控制面板启动.ixx:103-108</code> | direct_free+lsp_direct | <code>当前节点,显示语义.value(),选定坐标optional</code> | 全部实参求值完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0846 | F0218 | F0051 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:106, 海中鱼巣/界面/投影.控制面板启动.ixx:79</code> | algorithm_callback+indeterminately_sequenced_argument | <code>父链元素,当前节点</code> | 句柄有效且深度未超；生成显示名称实参求值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0851 | F0218 | F0387 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:111</code> | direct_const_member | <code>this=&关系,当前节点</code> | 当前节点已压入父链 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0852 | F0218 | F0218 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:116-117</code> | recursive | <code>同一只读依赖,子节点,同一父链引用</code> | 每个排序后子节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0845 | F0218 | F0163 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:78</code> | direct_free | <code>当前节点</code> | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0045 | F0218 | F0168 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:78</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0847 | F0218 | F0190 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:82</code> | direct_const_member | <code>this=&节点,当前节点</code> | 不在父链 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0848 | F0218 | F0385 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:86-87, 海中鱼巣/界面/投影.控制面板启动.ixx:87</code> | direct_free+lsp_direct | <code>当前节点,记录.类型,初始化结果,语素初始化读数</code> | 节点记录存在；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E0853 | F0220 | F0163 | <code>海中鱼巣/领域/初始化.语素.ixx:25</code> | direct_free | <code>基础信息</code> | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0046 | F0220 | F0168 | <code>海中鱼巣/领域/初始化.语素.ixx:25</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0854 | F0220 | F0363 | <code>海中鱼巣/领域/初始化.语素.ixx:26</code> | direct_const_member | <code>this=&语素入口结果</code> | 基础信息句柄形态有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0855 | F0223 | F0388 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | <code>this,false</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0856 | F0223 | F0389 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | <code>this</code> | 只读材料成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0857 | F0223 | F0390 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | <code>this</code> | 前两项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0858 | F0223 | F0391 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | <code>this</code> | 前三项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0859 | F0223 | F0392 | <code>海中鱼巣/界面/控制面板窗口.cpp:1553</code> | direct_member+unique_name+direct-member | <code>this</code> | 前四项成功；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0860 | F0223 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1564, 海中鱼巣/界面/控制面板窗口.cpp:1582</code> | direct_member+unique_name+direct-member+direct-member | <code>计时器创建失败阶段</code> | SetTimer失败；GetMessageW返回负值；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E0864 | F0223 | F0396 | <code>海中鱼巣/界面/控制面板窗口.cpp:1578</code> | resolved-win32-callback | <code>未单独冻结；读取源码调用点</code> | 窗口过程已由注册窗口类在 line 435 登记为 WNDPROC；消息循环调用 DispatchMessageW 后由 Win32 同步调度 | 专项源码静态类型与实际装配人工复核 |
| E0862 | F0223 | F0394 | <code>海中鱼巣/界面/控制面板窗口.cpp:1587</code> | direct_member+unique_name+direct-member | <code>this</code> | 消息循环退出；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0863 | F0223 | F0395 | <code>海中鱼巣/界面/控制面板窗口.cpp:1588</code> | direct_member+unique_name+direct-member | <code>this</code> | 字体清理完成；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0865 | F0224 | F0395 | <code>海中鱼巣/界面/控制面板窗口.cpp:310</code> | direct_member+unique_name+direct-member | <code>this</code> | 总是；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0866 | F0224 | F0394 | <code>海中鱼巣/界面/控制面板窗口.cpp:311</code> | direct_member+unique_name+direct-member | <code>this</code> | 快捷键清理完成；调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| E0867 | F0227 | F0230 | <code>海中鱼巣/生产运行期配置.数据.h:68</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 配置比较在基础字段相等后读取左侧与右侧系统角色稳定键组 | 专项源码静态类型与实际装配人工复核 |
| E0952 | F0227 | F0460 | <code>海中鱼巣/生产运行期配置.数据.h:74</code> | defaulted_const_member | <code>this=&this->概念活动,右=&右.概念活动</code> | 前十一项配置比较均为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0869 | F0228 | F0230 | <code>海中鱼巣/领域/系统角色清单.数据.h:98</code> | direct_const_member+suffix_match | <code>this</code> | 版本与四项数值前置均通过；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E0870 | F0229 | F0231 | <code>海中鱼巣/领域/概念活动状态.数据.h:46</code> | direct_const_member | <code>this</code> | 总是 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0871 | F0233 | F0397 | <code>海中鱼巣/核心/协调.结构事务.ixx:152</code> | callback_bind | <code>无捕获lambda转换函数指针</code> | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0872 | F0233 | F0398 | <code>海中鱼巣/核心/协调.结构事务.ixx:153</code> | callback_bind | <code>无捕获lambda转换函数指针</code> | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0873 | F0233 | F0399 | <code>海中鱼巣/核心/协调.结构事务.ixx:154</code> | callback_bind | <code>无捕获lambda转换函数指针</code> | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0874 | F0233 | F0400 | <code>海中鱼巣/核心/协调.结构事务.ixx:155</code> | callback_bind | <code>无捕获lambda转换函数指针</code> | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0875 | F0233 | F0401 | <code>海中鱼巣/核心/协调.结构事务.ixx:156</code> | callback_bind | <code>无捕获lambda转换函数指针</code> | 状态_非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0910 | F0238 | F0432 | <code>海中鱼巣/装配.运行期业务.ixx:100-101</code> | direct_constructor | <code>任务服务_,方法服务_,状态动态组合器_,稳定动作键</code> | P31完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0911 | F0238 | F0433 | <code>海中鱼巣/装配.运行期业务.ixx:102-103</code> | direct_constructor | <code>需求服务_,任务服务_,方法服务_,状态服务_,动态服务_</code> | P32完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0912 | F0238 | F0434 | <code>海中鱼巣/装配.运行期业务.ixx:104-106</code> | direct_constructor | <code>概念活动服务_,语素服务_,任务服务_,方法服务_,状态服务_,动态服务_</code> | P33完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0913 | F0238 | F0435 | <code>海中鱼巣/装配.运行期业务.ixx:107-111</code> | direct_constructor | <code>十四个已构造服务或组合器引用</code> | P34完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0914 | F0238 | F0436 | <code>海中鱼巣/装配.运行期业务.ixx:112-117</code> | direct_constructor | <code>八个组合器与四个业务服务引用</code> | P35完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0879 | F0238 | F0145 | <code>海中鱼巣/装配.运行期业务.ixx:65</code> | direct_constructor | <code>主信息,节点,接线副本01</code> | 构造函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0880 | F0238 | F0402 | <code>海中鱼巣/装配.运行期业务.ixx:66</code> | direct_constructor | <code>接线副本02,四仓库,关系仓库编号</code> | P01完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0881 | F0238 | F0403 | <code>海中鱼巣/装配.运行期业务.ixx:67</code> | direct_constructor | <code>接线副本03,四仓库,关系仓库编号</code> | P02完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0882 | F0238 | F0404 | <code>海中鱼巣/装配.运行期业务.ixx:68</code> | direct_constructor | <code>接线副本04,四仓库,特征值_,关系仓库编号</code> | P03完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0883 | F0238 | F0405 | <code>海中鱼巣/装配.运行期业务.ixx:69</code> | direct_constructor | <code>接线副本05,四仓库,关系仓库编号</code> | P04完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0884 | F0238 | F0406 | <code>海中鱼巣/装配.运行期业务.ixx:70</code> | direct_constructor | <code>接线副本06,四仓库</code> | P05完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0885 | F0238 | F0407 | <code>海中鱼巣/装配.运行期业务.ixx:71</code> | direct_constructor | <code>接线副本07,四仓库,关系仓库编号</code> | P06完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0886 | F0238 | F0408 | <code>海中鱼巣/装配.运行期业务.ixx:72-74</code> | direct_constructor | <code>接线副本08,四仓库,关系仓库编号,存在场景数据操作_,状态动态数据操作_</code> | P07完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0887 | F0238 | F0409 | <code>海中鱼巣/装配.运行期业务.ixx:75-76</code> | direct_constructor | <code>接线副本09,四仓库,关系仓库编号</code> | P08完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0888 | F0238 | F0410 | <code>海中鱼巣/装配.运行期业务.ixx:77-78</code> | direct_constructor | <code>接线副本10,四仓库,关系仓库编号,状态动态数据操作_</code> | P09完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0889 | F0238 | F0411 | <code>海中鱼巣/装配.运行期业务.ixx:79</code> | direct_constructor | <code>存在场景数据操作_</code> | P10完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0890 | F0238 | F0412 | <code>海中鱼巣/装配.运行期业务.ixx:80</code> | direct_constructor | <code>存在场景数据操作_</code> | P11完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0891 | F0238 | F0413 | <code>海中鱼巣/装配.运行期业务.ixx:81</code> | direct_constructor | <code>状态动态数据操作_</code> | P12完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0892 | F0238 | F0414 | <code>海中鱼巣/装配.运行期业务.ixx:82</code> | direct_constructor | <code>状态服务_,概念活动数据操作_</code> | P13完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0893 | F0238 | F0415 | <code>海中鱼巣/装配.运行期业务.ixx:83</code> | direct_constructor | <code>状态服务_,状态动态数据操作_</code> | P14完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0894 | F0238 | F0416 | <code>海中鱼巣/装配.运行期业务.ixx:84</code> | direct_constructor | <code>特征体系数据操作_</code> | P15完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0895 | F0238 | F0417 | <code>海中鱼巣/装配.运行期业务.ixx:85</code> | direct_constructor | <code>特征体系数据操作_</code> | P16完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0896 | F0238 | F0418 | <code>海中鱼巣/装配.运行期业务.ixx:86</code> | direct_constructor | <code>语素基础数据操作_</code> | P17完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0897 | F0238 | F0419 | <code>海中鱼巣/装配.运行期业务.ixx:87</code> | direct_constructor | <code>语素基础数据操作_</code> | P18完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0898 | F0238 | F0420 | <code>海中鱼巣/装配.运行期业务.ixx:88</code> | direct_constructor | <code>动态服务_,轻量因果数据操作_</code> | P19完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0899 | F0238 | F0421 | <code>海中鱼巣/装配.运行期业务.ixx:89</code> | direct_constructor | <code>概念图结构数据操作_</code> | P20完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0900 | F0238 | F0422 | <code>海中鱼巣/装配.运行期业务.ixx:90</code> | direct_constructor | <code>需求任务方法数据操作_,状态服务_</code> | P21完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0901 | F0238 | F0423 | <code>海中鱼巣/装配.运行期业务.ixx:91</code> | direct_constructor | <code>需求任务方法数据操作_,需求服务_,存在服务_,状态服务_</code> | P22完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0902 | F0238 | F0424 | <code>海中鱼巣/装配.运行期业务.ixx:92</code> | direct_constructor | <code>需求任务方法数据操作_,存在服务_,状态服务_</code> | P23完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0903 | F0238 | F0425 | <code>海中鱼巣/装配.运行期业务.ixx:93</code> | direct_constructor | <code>需求服务_,任务服务_,方法服务_,特征服务_,状态服务_</code> | P24完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0904 | F0238 | F0426 | <code>海中鱼巣/装配.运行期业务.ixx:94</code> | direct_constructor | <code>存在场景数据操作_</code> | P25完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0905 | F0238 | F0427 | <code>海中鱼巣/装配.运行期业务.ixx:95</code> | direct_constructor | <code>状态服务_,状态动态数据操作_</code> | P26完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0906 | F0238 | F0428 | <code>海中鱼巣/装配.运行期业务.ixx:96</code> | direct_constructor | <code>特征服务_,特征体系数据操作_</code> | P27完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0907 | F0238 | F0429 | <code>海中鱼巣/装配.运行期业务.ixx:97</code> | direct_constructor | <code>语素服务_,语素基础数据操作_</code> | P28完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0908 | F0238 | F0430 | <code>海中鱼巣/装配.运行期业务.ixx:98</code> | direct_constructor | <code>概念图结构服务_</code> | P29完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0909 | F0238 | F0431 | <code>海中鱼巣/装配.运行期业务.ixx:99</code> | direct_constructor | <code>需求服务_,任务服务_,方法召回服务_</code> | P30完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0915 | F0239 | F0336 | <code>海中鱼巣/启动.运行期上下文.ixx:60</code> | direct_const_member+unique_name | <code>this=&接线_</code> | 仓库编号_非零；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0916 | F0239 | F0437 | <code>海中鱼巣/启动.运行期上下文.ixx:61</code> | direct_const_member+unique_name | <code>this=&协调器_</code> | 已接域；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0917 | F0239 | F0438 | <code>海中鱼巣/启动.运行期上下文.ixx:62</code> | direct_const_member | <code>this=&节点_</code> | 协调器域编号等于接线域编号 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0918 | F0239 | F0397 | <code>海中鱼巣/启动.运行期上下文.ixx:65</code> | callback_invoke_resolved_normal | <code>接线_.运行期状态</code> | 四项入口条件通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0919 | F0239 | F0338 | <code>海中鱼巣/启动.运行期上下文.ixx:66</code> | direct_const_member | <code>this=&许可</code> | 许可构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0920 | F0239 | F0339 | <code>海中鱼巣/启动.运行期上下文.ixx:67, 海中鱼巣/启动.运行期上下文.ixx:68, 海中鱼巣/启动.运行期上下文.ixx:69, 海中鱼巣/启动.运行期上下文.ixx:70, 海中鱼巣/启动.运行期上下文.ixx:71</code> | direct_const_member+unique_name | <code>this=&许可</code> | 许可有效；共享路径验证通过；主信息读取为空；节点读取为空；关系读取为空；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0921 | F0239 | F0399 | <code>海中鱼巣/启动.运行期上下文.ixx:67</code> | callback_invoke_resolved_normal | <code>接线_.运行期状态,令牌引用01</code> | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0923 | F0239 | F0439 | <code>海中鱼巣/启动.运行期上下文.ixx:68</code> | direct_const_member | <code>空主信息句柄,令牌引用02</code> | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0925 | F0239 | F0346 | <code>海中鱼巣/启动.运行期上下文.ixx:69</code> | direct_const_member | <code>空节点句柄,令牌引用03</code> | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0927 | F0239 | F0440 | <code>海中鱼巣/启动.运行期上下文.ixx:70</code> | direct_const_member | <code>空关系句柄,令牌引用04</code> | 令牌读取完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0930 | F0239 | F0345 | <code>海中鱼巣/启动.运行期上下文.ixx:71-72</code> | direct_destructor | <code>this=&许可</code> | 返回表达式求值完成或异常离开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0929 | F0239 | F0441 | <code>海中鱼巣/启动.运行期上下文.ixx:71</code> | direct_const_member+unique_name | <code>主键0,令牌引用05</code> | 令牌读取完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0931 | F0240 | F0442 | <code>海中鱼巣/装配.运行期业务.ixx:125</code> | direct_const_member | <code>this=&存在场景数据操作_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0932 | F0240 | F0443 | <code>海中鱼巣/装配.运行期业务.ixx:126</code> | direct_const_member | <code>this=&状态动态数据操作_</code> | E0931返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0933 | F0240 | F0444 | <code>海中鱼巣/装配.运行期业务.ixx:127</code> | direct_const_member | <code>this=&特征体系数据操作_</code> | E0932返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0934 | F0240 | F0445 | <code>海中鱼巣/装配.运行期业务.ixx:128</code> | direct_const_member | <code>this=&语素基础数据操作_</code> | E0933返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0935 | F0240 | F0446 | <code>海中鱼巣/装配.运行期业务.ixx:129</code> | direct_const_member | <code>this=&轻量因果数据操作_</code> | E0934返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0936 | F0240 | F0447 | <code>海中鱼巣/装配.运行期业务.ixx:130</code> | direct_const_member | <code>this=&概念图结构数据操作_</code> | E0935返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0937 | F0240 | F0448 | <code>海中鱼巣/装配.运行期业务.ixx:131</code> | direct_const_member | <code>this=&需求任务方法数据操作_</code> | E0936返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0938 | F0240 | F0449 | <code>海中鱼巣/装配.运行期业务.ixx:132</code> | direct_const_member | <code>this=&系统角色数据操作_</code> | E0937返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0939 | F0240 | F0450 | <code>海中鱼巣/装配.运行期业务.ixx:133</code> | direct_const_member | <code>this=&概念活动数据操作_</code> | E0938返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0940 | F0240 | F0451 | <code>海中鱼巣/装配.运行期业务.ixx:134</code> | direct_const_member | <code>this=&概念活动服务_</code> | E0939返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0941 | F0240 | F0452 | <code>海中鱼巣/装配.运行期业务.ixx:135</code> | direct_const_member | <code>this=&系统角色初始化器_</code> | E0940返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0942 | F0240 | F0453 | <code>海中鱼巣/装配.运行期业务.ixx:137</code> | direct_const_member | <code>this=&业务操作_</code> | E0941返回true且稳定动作键_非零 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0944 | F0241 | F0228 | <code>海中鱼巣/领域/系统角色清单.数据.h:235</code> | direct_const_member | <code>this=&参数</code> | E0943返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0943 | F0241 | F0245 | <code>海中鱼巣/领域/系统角色清单.数据.h:235</code> | direct_const_member | <code>this=&当前清单</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0945 | F0242 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:153</code> | direct_const_member | <code>this=&当前运行期业务装配</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0946 | F0242 | F0454 | <code>海中鱼巣/装配.运行期业务.ixx:153</code> | direct_const_member | <code>this=&系统角色初始化器_,清单=const只读借用</code> | E0945返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0947 | F0243 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:149</code> | direct_const_member | <code>this=&当前运行期业务装配</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0948 | F0243 | F0455 | <code>海中鱼巣/装配.运行期业务.ixx:149</code> | direct_member | <code>this=&系统角色初始化器_,参数=const只读借用</code> | E0947返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0949 | F0244 | F0456 | <code>海中鱼巣/领域/系统角色清单.数据.h:264</code> | implicit-defaulted-member+defaulted-member-comparison | <code>未单独冻结；读取源码调用点</code> | 默认比较依次比较主体字段；系统角色清单默认等值比较逐成员比较主体材料 | 专项源码静态类型与实际装配人工复核 |
| E0950 | F0244 | F0457 | <code>海中鱼巣/领域/系统角色清单.数据.h:264</code> | implicit-defaulted-member+defaulted-member-comparison | <code>未单独冻结；读取源码调用点</code> | 默认比较依次比较两项关系材料字段；系统角色清单默认等值比较逐成员比较两项系统角色关系材料 | 专项源码静态类型与实际装配人工复核 |
| E0953 | F0245 | F0458 | <code>海中鱼巣/领域/系统角色清单.数据.h:222</code> | direct_const_member | <code>this=&主体</code> | 版本相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0954 | F0245 | F0459 | <code>海中鱼巣/领域/系统角色清单.数据.h:223</code> | direct_const_member | <code>this=&世界根到场景关系</code> | E0953返回true；E0954返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0956 | F0245 | F0051 | <code>海中鱼巣/领域/系统角色清单.数据.h:225, 海中鱼巣/领域/系统角色清单.数据.h:226, 海中鱼巣/领域/系统角色清单.数据.h:229, 海中鱼巣/领域/系统角色清单.数据.h:230</code> | direct_free_operator | <code>世界根到场景关系.源节点,主体.世界根.节点</code> | E0955返回true且第一关系类型正确；E0956返回true；第一关系顺序号和第二关系类型正确；E0958返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0961 | F0247 | F0229 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | direct_const_member | <code>this=&参数</code> | E0960返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0960 | F0247 | F0251 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | direct_const_member | <code>this=&当前材料</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0962 | F0247 | F0460 | <code>海中鱼巣/领域/概念活动状态.数据.h:183</code> | defaulted_const_member | <code>this=&初始化参数,右=&参数</code> | E0961返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0963 | F0248 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:166</code> | direct_const_member | <code>this=&当前装配</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0964 | F0248 | F0461 | <code>海中鱼巣/装配.运行期业务.ixx:166</code> | direct_const_member | <code>this=&概念活动服务_,系统角色,预期材料=&材料</code> | E0963返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0965 | F0249 | F0240 | <code>海中鱼巣/装配.运行期业务.ixx:159</code> | direct_const_member | <code>this=&当前装配</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0966 | F0249 | F0462 | <code>海中鱼巣/装配.运行期业务.ixx:159</code> | direct_member | <code>this=&概念活动服务_,系统角色,参数</code> | E0965返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0967 | F0250 | F0460 | <code>海中鱼巣/领域/概念活动状态.数据.h:188</code> | defaulted_const_member | <code>this=&初始化参数,右=&右.初始化参数</code> | 材料版本和活动版本相等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0968 | F0250 | F0463 | <code>海中鱼巣/领域/概念活动状态.数据.h:188</code> | std_array_element_compare | <code>this=&状态角色组[元素],右=&右.状态角色组[元素]</code> | E0967返回true；由X0180按元素短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0969 | F0250 | F0464 | <code>海中鱼巣/领域/概念活动状态.数据.h:189</code> | direct_const_member | <code>this=&重建视图,右=&右.重建视图</code> | E0968聚合结果为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0970 | F0250 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:191</code> | direct_const_member | <code>this=&根组[索引],右=&右.根组[索引]</code> | E0969返回true；循环短路0..4次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0971 | F0251 | F0229 | <code>海中鱼巣/领域/概念活动状态.数据.h:162</code> | direct_const_member | <code>this=&初始化参数</code> | 材料版本正确且活动版本非零 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0972 | F0251 | F0466 | <code>海中鱼巣/领域/概念活动状态.数据.h:162</code> | direct_const_member | <code>this=&重建视图</code> | E0971返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0973 | F0251 | F0463 | <code>海中鱼巣/领域/概念活动状态.数据.h:163</code> | std_array_element_compare | <code>this=&状态角色组[元素],右=&重建视图.状态角色组[元素]</code> | E0972返回true；由X0182按元素短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0974 | F0251 | F0231 | <code>海中鱼巣/领域/概念活动状态.数据.h:164</code> | direct_const_member | <code>this=&初始化参数</code> | E0973聚合结果为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0975 | F0251 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:169</code> | direct_const_member | <code>this=&根组[索引],右=&重建视图.根组[索引]</code> | 三项稳定键一致；循环短路0..4次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0976 | F0251 | F0467 | <code>海中鱼巣/领域/概念活动状态.数据.h:173</code> | direct_const_member | <code>this=&当前状态角色</code> | 根投影均相等；范围遍历短路0..3次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0977 | F0253 | F0240 | <code>海中鱼巣/启动.运行期上下文.ixx:84</code> | direct_const_member | <code>this=&业务装配_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0999 | F0254 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:100</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0978 | F0254 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:75</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0979 | F0254 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:76</code> | direct_const_member | <code>this=&环境</code> | E0978返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0047 | F0254 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:82</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0048 | F0254 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:83</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0049 | F0254 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:84</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E0980 | F0254 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:87</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E0981 | F0254 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:92</code> | direct_const_member | <code>this=&结果</code> | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0982 | F0254 | F0476 | <code>海中鱼巣/自检.入口初始化.ixx:93, 海中鱼巣/自检.入口初始化.ixx:94, 海中鱼巣/自检.入口初始化.ixx:95, 海中鱼巣/自检.入口初始化.ixx:96</code> | local_lambda_direct | <code>存在根,节点类型::存在</code> | E0981返回true；E0982返回true；E0983返回true；E0984返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1000 | F0255 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:104</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1001 | F0255 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:105</code> | direct_const_member | <code>this=&环境</code> | E1000返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0050 | F0255 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:111</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0051 | F0255 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:112</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0052 | F0255 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:113</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1002 | F0255 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:116</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1003 | F0255 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:118</code> | direct_const_member | <code>this=&结果</code> | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1004 | F0255 | F0355 | <code>海中鱼巣/自检.入口初始化.ixx:118</code> | direct_const_member+unique_name | <code>上下文.概念图隐式this</code> | E1003返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1017 | F0255 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:128</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1018 | F0256 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:132</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1019 | F0256 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:133</code> | direct_const_member | <code>this=&环境</code> | E1018返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0053 | F0256 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:139</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0054 | F0256 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:140</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0055 | F0256 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:141</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1020 | F0256 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:144</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1021 | F0256 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:145, 海中鱼巣/自检.入口初始化.ixx:150</code> | direct_const_member | <code>this=&结果</code> | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1022 | F0256 | F0364 | <code>海中鱼巣/自检.入口初始化.ixx:146, 海中鱼巣/自检.入口初始化.ixx:146-147</code> | direct_member+unique_name | <code>L实体,存在根.根节点,存在根.根节点；第4形参主键采用默认值0</code> | E1021返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1023 | F0256 | F0352 | <code>海中鱼巣/自检.入口初始化.ixx:149</code> | direct_const_member+unique_name | <code>概念根类别::存在</code> | 环境成功；不依赖E1021/E1022结果；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1025 | F0256 | F0363 | <code>海中鱼巣/自检.入口初始化.ixx:150</code> | direct_const_member | <code>this=&别名</code> | E1024返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1026 | F0256 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:152</code> | direct_free_operator | <code>读回.根节点,结果.概念图.存在根.根节点</code> | 读回有值且前项均true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1039 | F0256 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:157</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1040 | F0257 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:161</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1041 | F0257 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:162</code> | direct_const_member | <code>this=&环境</code> | E1040返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0056 | F0257 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:168</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0057 | F0257 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:169</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0058 | F0257 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:170</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1042 | F0257 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:173</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0059 | F0257 | F0496 | <code>海中鱼巣/自检.入口初始化.ixx:175</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0060 | F0257 | F0497 | <code>海中鱼巣/自检.入口初始化.ixx:176</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1043 | F0257 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:180</code> | direct_const_member | <code>this=&结果</code> | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1044 | F0257 | F0495 | <code>海中鱼巣/自检.入口初始化.ixx:185, 海中鱼巣/自检.入口初始化.ixx:186</code> | local_lambda_direct | <code>存在根</code> | E1043及四个预置名称比较均为true；E1044返回true；E1045返回true；E1046返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1064 | F0257 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:190</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1065 | F0258 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:194</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1066 | F0258 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:195</code> | direct_const_member | <code>this=&环境</code> | E1065返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0061 | F0258 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:201</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0062 | F0258 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:202</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1071 | F0258 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:203, 海中鱼巣/自检.入口初始化.ixx:210</code> | direct_member+unique_name | <code>this=&上下文.概念图初始化</code> | 环境成功；不依赖首次成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1067 | F0258 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:206</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1068 | F0258 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:207, 海中鱼巣/自检.入口初始化.ixx:216</code> | direct_const_member | <code>this=&上下文.节点</code> | 环境成功；不依赖首次成功；E1077返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1069 | F0258 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:208, 海中鱼巣/自检.入口初始化.ixx:217</code> | direct_const_member | <code>this=&上下文.关系</code> | E1068返回；前节点等于后节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0063 | F0258 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:208, 海中鱼巣/自检.入口初始化.ixx:217</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1070 | F0258 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:209, 海中鱼巣/自检.入口初始化.ixx:218</code> | direct_const_member | <code>this=&上下文.索引</code> | E1069返回；前关系等于后关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1072 | F0258 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:211</code> | direct_const_member | <code>this=&首次</code> | E1071返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1073 | F0258 | F0049 | <code>海中鱼巣/自检.入口初始化.ixx:211</code> | direct_const_member | <code>this=&再次</code> | E1072返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1074 | F0258 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:212, 海中鱼巣/自检.入口初始化.ixx:213, 海中鱼巣/自检.入口初始化.ixx:214, 海中鱼巣/自检.入口初始化.ixx:215</code> | direct_operator | <code>再次.存在根.根节点,首次.概念图.存在根.根节点</code> | E1073返回true；E1074返回true；E1075返回true；E1076返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1093 | F0258 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:222</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1094 | F0259 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:226</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1095 | F0259 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:227</code> | direct_const_member | <code>this=&环境</code> | E1094返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0065 | F0259 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:233</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0066 | F0259 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:234</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0067 | F0259 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:235</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1096 | F0259 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:238</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1097 | F0259 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:239</code> | direct_const_member | <code>this=&结果</code> | 环境成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1098 | F0259 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:240</code> | std_optional_element_compare | <code>结果.自我存在根支持.value(),结果.概念图.存在根.根节点</code> | E1097返回true且两侧optional均有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1099 | F0259 | F0371 | <code>海中鱼巣/自检.入口初始化.ixx:241, 海中鱼巣/自检.入口初始化.ixx:241-242</code> | direct_const_member+unique_name | <code>this=&上下文.概念图,结果.自我初始化->世界树.自我存在节点</code> | E1098返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1112 | F0259 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:246</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1113 | F0260 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:250</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1114 | F0260 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:251</code> | direct_const_member | <code>this=&环境</code> | E1113返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0068 | F0260 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:257</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0069 | F0260 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:258</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0070 | F0260 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:259</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1115 | F0260 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:262</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1116 | F0260 | F0516 | <code>海中鱼巣/自检.入口初始化.ixx:263</code> | direct_member | <code>this=&上下文.存在</code> | 环境成功；不依赖E1115成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1117 | F0260 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:264</code> | direct_member | <code>this=&上下文.概念图,第二实例</code> | E1116返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1118 | F0260 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:265</code> | direct_const_member | <code>this=&结果</code> | E1117返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1119 | F0260 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:265</code> | std_optional_element_compare | <code>支持.value(),结果.概念图.存在根.根节点</code> | E1118返回true且支持有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1120 | F0260 | F0372 | <code>海中鱼巣/自检.入口初始化.ixx:266</code> | direct_const_member+unique_name | <code>this=&上下文.概念图,结果.概念图.存在根.根节点</code> | E1119返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1121 | F0260 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:270</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1134 | F0261 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:274</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1135 | F0261 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:275</code> | direct_const_member | <code>this=&环境</code> | E1134返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0071 | F0261 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:281</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0072 | F0261 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:282</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0073 | F0261 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:283</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1136 | F0261 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:286</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1137 | F0261 | F0516 | <code>海中鱼巣/自检.入口初始化.ixx:287</code> | direct_member | <code>this=&上下文.存在</code> | 环境成功；不依赖E1136成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1138 | F0261 | F0523 | <code>海中鱼巣/自检.入口初始化.ixx:288</code> | direct_member | <code>this=&上下文.节点,已删除</code> | E1137返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0074 | F0261 | R0147 | <code>海中鱼巣/自检.入口初始化.ixx:288</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1139 | F0261 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:289, 海中鱼巣/自检.入口初始化.ixx:297</code> | direct_const_member | <code>this=&上下文.节点</code> | E1138返回；E1145返回true且三个拒绝optional均为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1140 | F0261 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:290, 海中鱼巣/自检.入口初始化.ixx:298</code> | direct_const_member | <code>this=&上下文.关系</code> | E1139返回；E1146等于前节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0075 | F0261 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:290, 海中鱼巣/自检.入口初始化.ixx:298</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1141 | F0261 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:291, 海中鱼巣/自检.入口初始化.ixx:299</code> | direct_const_member | <code>this=&上下文.索引</code> | E1140返回；E1147等于前关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1142 | F0261 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:292, 海中鱼巣/自检.入口初始化.ixx:293, 海中鱼巣/自检.入口初始化.ixx:294</code> | direct_member | <code>this=&上下文.概念图,结果.概念图.存在根.根节点</code> | E1141返回；E1142返回；E1143返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1145 | F0261 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:295</code> | direct_const_member | <code>this=&结果</code> | 三次拒绝调用均已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1149 | F0261 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:303</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1162 | F0262 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:307</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1163 | F0262 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:308</code> | direct_const_member | <code>this=&环境</code> | E1162返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0077 | F0262 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:314</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0078 | F0262 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:315</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0079 | F0262 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:316</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1164 | F0262 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:319</code> | direct_free+unique_name | <code>端口,{方法登记根稳定键,根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1165 | F0262 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:320</code> | direct_const_member | <code>this=&结果</code> | E1164返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1166 | F0262 | F0530 | <code>海中鱼巣/自检.入口初始化.ixx:320</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | E1165返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1167 | F0262 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:321</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | E1166返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1168 | F0262 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:322</code> | direct_const_member | <code>this=&上下文.节点</code> | E1167返回运行中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1169 | F0262 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:323</code> | direct_const_member | <code>this=&上下文.关系</code> | E1168大于初始节点数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0080 | F0262 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:323</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1170 | F0262 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:324</code> | direct_const_member | <code>this=&上下文.索引</code> | E1169大于初始关系数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1171 | F0262 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:328</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1184 | F0263 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:332</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1185 | F0263 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:333</code> | direct_const_member | <code>this=&环境</code> | E1184返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0082 | F0263 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:339</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0083 | F0263 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:340</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0084 | F0263 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:341</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1186 | F0263 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:344</code> | direct_free+unique_name | <code>端口,{默认方法登记根稳定键,默认根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1187 | F0263 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:346, 海中鱼巣/自检.入口初始化.ixx:349</code> | direct_const_member | <code>this=&结果</code> | E1186返回且E1186结果已在345行无保护value访问；坐标optional已经形成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1188 | F0263 | F0059 | <code>海中鱼巣/自检.入口初始化.ixx:347</code> | direct_const_member+unique_name | <code>this=&上下文.世界树初始化,快照.世界树.自我存在节点</code> | E1187返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1190 | F0263 | F0173 | <code>海中鱼巣/自检.入口初始化.ixx:349</code> | direct_const_member | <code>this=&快照.语素</code> | E1189返回true且初始化代次等于1 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1191 | F0263 | F0174 | <code>海中鱼巣/自检.入口初始化.ixx:354</code> | direct_const_member | <code>this=&快照.世界树</code> | E1190及四个名称比较均为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1192 | F0263 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:359</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1205 | F0264 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:363</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0085 | F0264 | F0189 | <code>海中鱼巣/自检.入口初始化.ixx:364, 海中鱼巣/自检.入口初始化.ixx:380</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1206 | F0264 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:364</code> | direct_const_member | <code>this=&环境</code> | E1205返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0086 | F0264 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:371</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0087 | F0264 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:372</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0088 | F0264 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:373</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1207 | F0264 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:376</code> | direct_free+unique_name | <code>端口,{固定配置.方法登记根稳定键,固定配置.根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0089 | F0264 | F0441 | <code>海中鱼巣/自检.入口初始化.ixx:378, 海中鱼巣/自检.入口初始化.ixx:379</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1208 | F0264 | F0544 | <code>海中鱼巣/自检.入口初始化.ixx:378, 海中鱼巣/自检.入口初始化.ixx:379</code> | direct_const_member | <code>this=&上下文.索引,固定配置.根需求参数.安全值语素稳定键</code> | E1207返回且377行已无保护解引用自我初始化；E1208返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1210 | F0264 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:380</code> | direct_const_member | <code>this=&结果</code> | 两次索引读取已返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1211 | F0264 | F0175 | <code>海中鱼巣/自检.入口初始化.ixx:380</code> | direct_const_member | <code>this=&根</code> | E1210返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1212 | F0264 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:381, 海中鱼巣/自检.入口初始化.ixx:382</code> | std_optional_element_compare | <code>安全索引.value(),根.安全根需求.语素入口结果.语素入口</code> | E1211返回true且optional两侧有值；E1212返回true且optional两侧有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1214 | F0264 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:386</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1227 | F0265 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:390</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0090 | F0265 | F0350 | <code>海中鱼巣/自检.入口初始化.ixx:391, 海中鱼巣/自检.入口初始化.ixx:406, 海中鱼巣/自检.入口初始化.ixx:417</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1228 | F0265 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:391</code> | direct_const_member | <code>this=&环境</code> | E1227返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0091 | F0265 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:397</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0092 | F0265 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:398</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0093 | F0265 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:399</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1229 | F0265 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:402</code> | direct_free+unique_name | <code>端口,{默认方法登记根稳定键,默认根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0094 | F0265 | R0586 | <code>海中鱼巣/自检.入口初始化.ixx:405</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0095 | F0265 | F0553 | <code>海中鱼巣/自检.入口初始化.ixx:407</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0096 | F0265 | F0554 | <code>海中鱼巣/自检.入口初始化.ixx:409</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0097 | F0265 | F0555 | <code>海中鱼巣/自检.入口初始化.ixx:412</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1230 | F0265 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:417</code> | direct_const_member | <code>this=&结果</code> | 403行已无保护解引用自我初始化且F0551已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1231 | F0265 | F0551 | <code>海中鱼巣/自检.入口初始化.ixx:417, 海中鱼巣/自检.入口初始化.ixx:418</code> | local_lambda_direct | <code>根.安全根需求,1,10</code> | E1230返回true；E1231返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1233 | F0265 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:419</code> | rewritten_not_equal | <code>根.安全根需求.根需求,根.服务根需求.根需求</code> | E1232返回true；C++20把!=重写为==后取反 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1234 | F0265 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:423</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1255 | F0266 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:427</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1256 | F0266 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:428</code> | direct_const_member | <code>this=&环境</code> | E1255返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0098 | F0266 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:435</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0099 | F0266 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:436</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0100 | F0266 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:437</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1257 | F0266 | F0015 | <code>海中鱼巣/自检.入口初始化.ixx:440</code> | direct_free+unique_name | <code>端口,{固定配置.方法登记根稳定键,固定配置.根需求参数}</code> | 环境成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1258 | F0266 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:441, 海中鱼巣/自检.入口初始化.ixx:448</code> | direct_const_member | <code>this=&上下文.节点</code> | E1257返回，不要求成功；E1263等于运行中 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1259 | F0266 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:442, 海中鱼巣/自检.入口初始化.ixx:449</code> | direct_const_member | <code>this=&上下文.关系</code> | E1258返回；E1264等于前节点 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0101 | F0266 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:442, 海中鱼巣/自检.入口初始化.ixx:449</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1260 | F0266 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:443, 海中鱼巣/自检.入口初始化.ixx:450</code> | direct_const_member | <code>this=&上下文.索引</code> | E1259返回；E1265等于前关系 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1261 | F0266 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:444</code> | direct_member | <code>this=&上下文.自我线程实例,固定配置.根需求参数</code> | E1260返回，不要求首次成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1262 | F0266 | F0016 | <code>海中鱼巣/自检.入口初始化.ixx:445</code> | direct_const_member | <code>this=&首次</code> | E1261返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1263 | F0266 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:447</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | 首次成功且再次成功、复用并拒绝原因为重复启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1267 | F0266 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:454</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1280 | F0267 | F0468 | <code>海中鱼巣/自检.入口初始化.ixx:459</code> | direct_free | <code>配置,编号</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1281 | F0267 | F0469 | <code>海中鱼巣/自检.入口初始化.ixx:460</code> | direct_const_member | <code>this=&环境</code> | E1280返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1282 | F0267 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:464-465, 海中鱼巣/自检.入口初始化.ixx:466-468</code> | direct_member | <code>this=&上下文.自我线程实例,{0,服务值语素稳定键,1,10,1,10}</code> | 环境成功；E1282返回后无条件执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1284 | F0267 | F0530 | <code>海中鱼巣/自检.入口初始化.ixx:471</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | 两个拒绝结果均完全符合 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1285 | F0267 | F0531 | <code>海中鱼巣/自检.入口初始化.ixx:472</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | E1284返回false | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1286 | F0267 | F0562 | <code>海中鱼巣/自检.入口初始化.ixx:473</code> | direct_const_member | <code>this=&上下文.自我线程实例</code> | E1285等于未启动 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1287 | F0267 | F0197 | <code>海中鱼巣/自检.入口初始化.ixx:474</code> | direct_const_member | <code>this=&上下文.节点</code> | E1286返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1288 | F0267 | F0198 | <code>海中鱼巣/自检.入口初始化.ixx:475</code> | direct_const_member | <code>this=&上下文.关系</code> | E1287等于环境初始节点数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0103 | F0267 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:475</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1289 | F0267 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:476</code> | direct_const_member | <code>this=&上下文.索引</code> | E1288等于环境初始关系数量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1290 | F0267 | F0131 | <code>海中鱼巣/自检.入口初始化.ixx:480</code> | compiler_implicit_member_destructor | <code>this=&环境.上下文->自我线程实例</code> | 隔离环境作用域退出且上下文已构造 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1291 | F0268 | F0563 | <code>海中鱼巣/核心/日志系统.h:101, 海中鱼巣/核心/日志系统.h:104, 海中鱼巣/核心/日志系统.h:107, 海中鱼巣/核心/日志系统.h:109, 海中鱼巣/核心/日志系统.h:88, 海中鱼巣/核心/日志系统.h:90, 海中鱼巣/核心/日志系统.h:92, 海中鱼巣/核心/日志系统.h:98</code> | direct_free+lsp_direct | <code>未单独冻结；读取源码调用点</code> | 类别为逻辑错误；类别为运行；类别为事件；切片合法且等于SELF_TEST_ENTRY_INITIALIZATION；切片合法且等于SELF_TEST_DATABASE；切片合法且等于SELF_TEST_WAREHOUSE_PERFORMANCE；切片合法且等于SELF_TEST_D455；切片合法但不等于四个预置名；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E1294 | F0268 | F0564 | <code>海中鱼巣/核心/日志系统.h:94</code> | direct_free+lsp_direct | <code>调试切片名</code> | 类别为调试；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；LSP 直接解析并经源码调用词复核 |
| E1300 | F0272 | F0135 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:148</code> | member_constructor | <code>790000+有效关系规模,默认结构事务接线</code> | 规模和随机状态成员已初始化 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1301 | F0272 | F0136 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:149</code> | member_constructor | <code>主信息_,791000+有效关系规模,默认结构事务接线</code> | 主信息_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1302 | F0272 | F0137 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:150</code> | member_constructor | <code>节点_,792000+有效关系规模,默认结构事务接线</code> | 节点_构造完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1303 | F0273 | F0331 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:161</code> | direct_member_nonvirtual | <code>this=&主信息_</code> | 规模准入通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1312 | F0273 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:162, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:166, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:180</code> | direct_free+unique_name | <code>E1311返回关系</code> | 添加关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1304 | F0273 | F0565 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:162</code> | direct_free | <code>共用主信息</code> | 主信息创建返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1305 | F0273 | F0332 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:165</code> | direct_member_nonvirtual | <code>基础信息,共用主信息</code> | 节点循环每轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1306 | F0273 | F0163 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:166</code> | direct_free | <code>节点</code> | 节点创建返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1307 | F0273 | F0566 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:170</code> | direct_member_nonvirtual+unique_name | <code>this</code> | 1024个节点全部形成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1308 | F0273 | F0567 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:176, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:177, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:179</code> | direct_member_nonvirtual+unique_name | <code>this</code> | 随机填充循环每次尝试；源随机值形成；源目标索引不同；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1311 | F0273 | F0568 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:180</code> | direct_member_nonvirtual+unique_name | <code>引用,源索引,目标索引,顺序号,true</code> | 非自环随机候选；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1313 | F0273 | F0569 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:186</code> | direct_member_nonvirtual+unique_name | <code>this</code> | 参考表数量达到目标；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1314 | F0273 | F0198 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:188</code> | direct_const_member | <code>this=&关系_</code> | 固定变更成功并写摘要 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1315 | F0273 | F0277 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:189</code> | direct_const_member+unique_name | <code>this</code> | 有效关系数量等于规模；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1316 | F0273 | F0283 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:190</code> | direct_const_member+unique_name | <code>this</code> | 有效关系数量等于有效关系规模_且验证结构边界返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1317 | F0274 | F0570 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:208, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:209, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:210, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:211, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:212, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:213, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:214</code> | local_lambda_direct | <code>记录.关系编号</code> | 每条排序记录；E1317完成；E1318完成；E1319完成；E1320完成；E1321完成；E1322完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1324 | F0276 | F0278 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:220</code> | direct_const_member+unique_name | <code>this</code> | F0276被调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1325 | F0277 | F0571 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:235, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:236</code> | direct_const_member+unique_name | <code>节点组_[1002],关系类型::普通父子,false</code> | F0277被调用；E1325成功返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1327 | F0277 | F0572 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:237, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:238</code> | direct_const_member+unique_name | <code>节点组_[996],关系类型::概念上下位,false</code> | E1326成功返回；E1327结果非空；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1329 | F0277 | F0573 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:239, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:240, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:241</code> | direct_const_member+unique_name | <code>关系类型::概念上下位,节点组_[993],节点组_[994]</code> | 两棵根布尔值已形成；E1329返回true；E1329与E1330均返回true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1332 | F0278 | F0572 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:275, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:277, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:291, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:293</code> | direct_const_member+unique_name | <code>节点组_[1010],关系类型::引用,false</code> | 构造正向稀疏；0号构造完成；7号构造完成；8号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1334 | F0278 | F0571 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:279, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:281, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:285</code> | direct_const_member+unique_name | <code>节点组_[1013],关系类型::引用,false</code> | 1号构造完成；2号构造完成；4号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1336 | F0278 | F0573 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:283</code> | direct_const_member+unique_name | <code>关系类型::引用,节点组_[1010],节点组_[1011]</code> | 3号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1338 | F0278 | F0574 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:287, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:289</code> | direct_const_member+unique_name | <code>节点组_[1014]</code> | 5号构造完成；6号构造完成；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1342 | F0279 | F0575 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:299</code> | direct_const_member | <code>定义.源节点,定义.类型</code> | 正向记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1343 | F0279 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:301, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:306, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:311, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:316, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:321, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:325</code> | direct_free | <code>结束-开始</code> | 正向查询返回；反向查询返回；精确存在查询返回；目标存在查询返回；节点相关查询返回；审计查询返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1344 | F0279 | F0577 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:301, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:306, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:321, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:325</code> | direct_free | <code>实际,定义.预期记录组</code> | 正向查询返回；反向查询返回；节点相关查询返回；审计查询返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1345 | F0279 | F0578 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:304</code> | direct_const_member | <code>定义.目标节点,定义.类型</code> | 反向记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1348 | F0279 | F0579 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:309</code> | direct_const_member | <code>定义.类型,定义.源节点,定义.目标节点</code> | 精确存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1350 | F0279 | F0580 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:314</code> | direct_const_member | <code>定义.类型,定义.目标节点</code> | 目标存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1352 | F0279 | F0581 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:319</code> | direct_const_member | <code>定义.源节点</code> | 节点相关 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1355 | F0279 | F0582 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:323</code> | direct_const_member | <code>定义.源节点,定义.类型</code> | 当前审计或未声明值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1358 | F0281 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:107, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:108, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:109</code> | direct_free | <code>样本,0.50</code> | 名称、样本数量和候选上界字段已初始化；E1358正常返回；E1359正常返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1361 | F0282 | F0583 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:644</code> | direct_const_member+unique_name | <code>this=&夹具</code> | 进入并发基线；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1362 | F0282 | F0584 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:658-687</code> | thread_callback | <code>线程索引按值；写材料组、线程读取样本、开始、窗口结束、正确、夹具、密集正向、完成操作数量、最终关系组按引用捕获</code> | 每次std::thread构造成功后异步调度 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1363 | F0282 | F0585 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:695</code> | direct_member_nonvirtual+unique_name | <code>索引,最终关系组[索引]</code> | 全部工作线程已join；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1364 | F0282 | F0280 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:702</code> | direct_free | <code>合并样本,0.50</code> | 线程样本合并完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1368 | F0283 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:254, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:262</code> | direct_const_member | <code>当前句柄</code> | 参考记录状态有效；参考记录状态已删除 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1369 | F0283 | F0588 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:255, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:259</code> | direct_free | <code>*实际,记录</code> | 有效记录普通读取有值；失效记录审计读取有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1370 | F0283 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:258, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:263</code> | direct_const_member | <code>当前句柄</code> | 参考记录状态已失效；删除记录普通读取无值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1374 | F0284 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:347-348</code> | direct_member_nonvirtual | <code>关系类型::运行期临时,节点组_[940],节点组_[941],1</code> | 每轮开始时间已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1375 | F0284 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:350</code> | direct_free+unique_name | <code>新关系</code> | 累计正确仍为true；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1376 | F0284 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:350, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:351</code> | direct_const_member | <code>新关系</code> | 累计正确且句柄有效；累计正确且删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1377 | F0284 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:351</code> | direct_member_nonvirtual | <code>新关系</code> | 创建后累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1379 | F0284 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:352</code> | direct_free | <code>结束-开始</code> | 正式轮 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1380 | F0284 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:354</code> | direct_free | <code>"创建",样本,初始当前记录数量_+样本.size(),正确</code> | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1381 | F0285 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:382-383</code> | direct_member_nonvirtual | <code>关系类型::运行期临时,节点组_[940],节点组_[941],3</code> | 进入当前循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1382 | F0285 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:384</code> | direct_free+unique_name | <code>新关系</code> | 创建关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1383 | F0285 | F0591 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:389</code> | direct_member_nonvirtual | <code>新关系</code> | 新关系句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1384 | F0285 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:391</code> | direct_const_member | <code>失效结果.当前关系</code> | 失效入口返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1385 | F0285 | F0592 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:392</code> | direct_const_member | <code>this=&失效结果</code> | 累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1386 | F0285 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:394</code> | direct_const_member | <code>新关系</code> | 累计正确、结果完整且审计状态已失效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1387 | F0285 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:395</code> | direct_free | <code>结束-开始</code> | 正式轮且创建成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1388 | F0285 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:397</code> | direct_free | <code>"失效",样本,初始当前记录数量_+660,正确</code> | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1389 | F0286 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:404-405</code> | direct_member_nonvirtual | <code>关系类型::因果来源,节点组_[942],节点组_[943],4</code> | 进入重挂测量 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1390 | F0286 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:410</code> | direct_member_nonvirtual | <code>当前关系,节点组_[942],新目标</code> | 当前循环计时已开始 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1391 | F0286 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:416, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:417</code> | direct_const_member | <code>*新关系</code> | 重挂返回有值；重挂成功且累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1393 | F0286 | F0051 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:418</code> | direct_free | <code>记录->目标节点,新目标</code> | 旧记录为空且新记录存在 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1394 | F0286 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:421</code> | direct_free | <code>结束-开始</code> | 正式轮且重挂成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1395 | F0286 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:423</code> | direct_member_nonvirtual | <code>当前关系</code> | 全部循环结束且累计正确仍为true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1396 | F0286 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:424</code> | direct_free | <code>"重挂",样本,初始当前记录数量_+661,正确</code> | 最终删除短路完成 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1397 | F0287 | F0167 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:362-363</code> | direct_member_nonvirtual | <code>关系类型::运行期临时,节点组_[940],节点组_[941],2</code> | 进入当前循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1398 | F0287 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:364</code> | direct_free+unique_name | <code>新关系</code> | 创建关系返回；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1399 | F0287 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:369</code> | direct_member_nonvirtual | <code>新关系</code> | 新关系句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1400 | F0287 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:371</code> | direct_const_member | <code>新关系</code> | 累计正确且删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1401 | F0287 | F0576 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:372</code> | direct_free | <code>结束-开始</code> | 正式轮且创建成功；不要求删除成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1402 | F0287 | F0281 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:374</code> | direct_free | <code>"删除",样本,初始当前记录数量_+440,正确</code> | 全部循环结束 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1403 | F0291 | F0593 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:742</code> | direct_const_member | <code>this=&指标</code> | F0291回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1404 | F0292 | F0593 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:745</code> | direct_const_member | <code>this=&指标</code> | F0292回调执行 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1639 | F0321 | F0336 | <code>海中鱼巣/核心/结构事务接线.数据.h:83</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1638 | F0321 | F0618 | <code>海中鱼巣/核心/结构事务接线.数据.h:83</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1640 | F0322 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:17, 海中鱼巣/核心/节点仓库.cpp:18</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1644 | F0323 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:82, 海中鱼巣/核心/关系仓库.cpp:83</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1648 | F0324 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:13, 海中鱼巣/核心/索引仓库.cpp:14</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1652 | F0326 | F0163 | <code>海中鱼巣/领域/方法服务.h:139, 海中鱼巣/领域/方法服务.h:140, 海中鱼巣/领域/方法服务.h:141</code> | direct_free | <code>登记根</code> | 稳定非名称键非零；登记根句柄有效；活跃状态句柄有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0105 | F0326 | F0168 | <code>海中鱼巣/领域/方法服务.h:139, 海中鱼巣/领域/方法服务.h:140, 海中鱼巣/领域/方法服务.h:141</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1655 | F0326 | F0051 | <code>海中鱼巣/领域/方法服务.h:142, 海中鱼巣/领域/方法服务.h:143, 海中鱼巣/领域/方法服务.h:144</code> | direct_free_operator | <code>登记根,活跃状态</code> | 三个句柄均有效；登记根与活跃状态不等；登记根分别与两个状态不等 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1658 | F0327 | F0333 | <code>海中鱼巣/领域/方法服务.h:1748</code> | direct_const_member | <code>this=&方法服务,方法节点,节点类型::方法</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1659 | F0327 | F0330 | <code>海中鱼巣/领域/方法服务.h:1751</code> | direct_const_member | <code>this=&方法服务,方法节点,关系类型::模板,节点类型::状态,方法角色顺序号</code> | 方法节点类型匹配 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1660 | F0327 | F0329 | <code>海中鱼巣/领域/方法服务.h:1755</code> | direct_const_member | <code>this=&状态,角色状态节点.value()</code> | 角色状态目标存在且唯一 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1661 | F0327 | F0334 | <code>海中鱼巣/领域/方法服务.h:1760</code> | direct_const_member+unique_name | <code>this=&方法服务,角色</code> | 角色状态I64存在并完成枚举转换；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1662 | F0328 | F0190 | <code>海中鱼巣/领域/状态服务.h:206</code> | direct_const_member | <code>this=&节点_,状态节点</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1663 | F0328 | F0580 | <code>海中鱼巣/领域/状态服务.h:208</code> | direct_const_member | <code>this=&关系_,关系类型::运行期临时,状态节点</code> | 节点记录存在且类型为状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1665 | F0329 | F0190 | <code>海中鱼巣/领域/状态服务.h:186</code> | direct_const_member | <code>this=&节点_,状态节点</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1666 | F0329 | F0619 | <code>海中鱼巣/领域/状态服务.h:190</code> | direct_const_member | <code>this=&主信息_,记录->主信息</code> | 节点记录存在且类型为状态 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1668 | F0330 | F0333 | <code>海中鱼巣/领域/方法服务.h:1713, 海中鱼巣/领域/方法服务.h:1721</code> | direct_const_member | <code>1713:this=&方法服务,源节点,节点类型::方法；1721:this=&方法服务,目标,目标类型</code> | 1713函数进入时调用一次；1721仅在源节点类型匹配并取得目标组后，对每个目标调用0..N次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1669 | F0330 | F0620 | <code>海中鱼巣/领域/方法服务.h:1717</code> | direct_const_member | <code>this=&关系_,源节点,类型,顺序号.value()</code> | 源节点类型匹配且顺序号有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1670 | F0330 | F0621 | <code>海中鱼巣/领域/方法服务.h:1718</code> | direct_const_member | <code>this=&关系_,源节点,类型</code> | 源节点类型匹配且顺序号为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1672 | F0331 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:39</code> | direct_const_member | <code>this=&事务接线_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0106 | F0331 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:40</code> | resolved-function-pointer | <code>this=&事务接线_,事务接线_.运行期状态</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E1673 | F0331 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_const_member | <code>this=&许可</code> | U0008已形成许可对象 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1674 | F0331 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_const_member | <code>this=&许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1675 | F0331 | F0622 | <code>海中鱼巣/核心/主信息仓库.cpp:41</code> | direct_member | <code>this=&主信息仓库,许可.读取令牌()</code> | 许可有效且E1674已取得令牌借用；调用方可达且libclang直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1676 | F0331 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:40-42</code> | implicit_destructor | <code>this=&许可</code> | 仅事务接线已接域且许可对象已在40行形成；正常路径在41行返回对象形成后析构，异常路径在许可形成后的展开阶段析构；不覆盖43—50未接域分支 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1677 | F0332 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:54</code> | direct_const_member | <code>this=&事务接线_</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0107 | F0332 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:55</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 事务接线已接域；生产运行期唯一正式共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| E1681 | F0332 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:56-57</code> | implicit_destructor | <code>this=&许可</code> | 接域分支返回对象形成后或许可形成后的异常展开 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1678 | F0332 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_const_member | <code>this=&许可</code> | U0009已形成许可对象 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1679 | F0332 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_const_member | <code>this=&许可</code> | 许可有效 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1680 | F0332 | F0623 | <code>海中鱼巣/核心/节点仓库.cpp:56</code> | direct_member+direct-member | <code>this=&节点仓库,类型,主信息,许可.读取令牌()</code> | 许可有效且令牌借用已取得；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1682 | F0332 | F0624 | <code>海中鱼巣/核心/节点仓库.cpp:58</code> | direct_const_member | <code>this=&主信息_,主信息</code> | 未接域路径 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1683 | F0332 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:58</code> | direct_free_internal_linkage+direct-free | <code>类型</code> | 主信息有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1684 | F0333 | F0190 | <code>海中鱼巣/领域/方法服务.h:1835</code> | direct_const_member | <code>this=&节点_,节点句柄值</code> | 函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1685 | F0335 | F0626 | <code>海中鱼巣/领域/状态服务.h:297</code> | direct_const_member+suffix_match | <code>this=&状态服务,发生时间戳.value()</code> | 发生时间戳有值；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1686 | F0335 | F0331 | <code>海中鱼巣/领域/状态服务.h:300</code> | direct_member | <code>this=&主信息_</code> | 时间戳未提供或校验通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1688 | F0335 | F0184 | <code>海中鱼巣/领域/状态服务.h:301, 海中鱼巣/领域/状态服务.h:305, 海中鱼巣/领域/状态服务.h:305-306</code> | direct_free+unique_name | <code>状态值写入bool,L"创建状态节点时状态值写入不及预期。"</code> | F0627返回后；真假均调用；F0628返回后；真假均调用；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1687 | F0335 | F0627 | <code>海中鱼巣/领域/状态服务.h:301</code> | direct_member | <code>this=&主信息_,主信息句柄,状态值</code> | F0331返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1689 | F0335 | F0628 | <code>海中鱼巣/领域/状态服务.h:305</code> | direct_member | <code>this=&主信息_,主信息句柄,发生时间戳槽位,static_cast<I64>(发生时间戳.value())</code> | 状态值写入检查通过且发生时间戳有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1691 | F0335 | F0332 | <code>海中鱼巣/领域/状态服务.h:309</code> | direct_member | <code>this=&节点_,节点类型::状态,主信息句柄</code> | 状态值及可选时间戳写入检查通过 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1692 | F0343 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:180</code> | direct_free+direct-free | <code>仓库</code> | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1783 | F0343 | F0377 | <code>海中鱼巣/核心/关系仓库.cpp:181</code> | direct_const_member | <code>this=&节点_,节点</code> | 当前关系令牌为空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1782 | F0343 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:181</code> | direct_const_member | <code>this=&节点_,节点,*令牌</code> | 当前关系令牌非空 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1799 | F0345 | F0631 | <code>海中鱼巣/核心/结构事务接线.数据.h:40</code> | direct_member+unique_name | <code>this</code> | 析构函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1801 | F0346 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:251</code> | direct_free_internal_linkage+direct-free | <code>事务接线_,令牌</code> | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1833 | F0349 | F0633 | <code>海中鱼巣/线程/自我线程.ixx:303</code> | direct_member+unique_name | <code>this=&语素初始化_</code> | 首次停止请求为false；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1834 | F0349 | F0173 | <code>海中鱼巣/线程/自我线程.ixx:304</code> | direct_const_member | <code>this=&语素结果</code> | E1833返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1835 | F0349 | F0634 | <code>海中鱼巣/线程/自我线程.ixx:305, 海中鱼巣/线程/自我线程.ixx:310, 海中鱼巣/线程/自我线程.ixx:316</code> | direct_member+unique_name | <code>this=当前自我线程</code> | 语素结果不成功；世界树结果不成功；新快照不成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1836 | F0349 | F0635 | <code>海中鱼巣/线程/自我线程.ixx:308</code> | direct_member+unique_name | <code>this=&世界树初始化_</code> | 语素结果成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1837 | F0349 | F0174 | <code>海中鱼巣/线程/自我线程.ixx:309</code> | direct_const_member | <code>this=&世界树结果</code> | E1836返回后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1839 | F0349 | F0636 | <code>海中鱼巣/线程/自我线程.ixx:313</code> | direct_member+unique_name | <code>this=&需求初始化_,世界树结果,参数</code> | 世界树结果成功；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1840 | F0349 | F0047 | <code>海中鱼巣/线程/自我线程.ixx:315</code> | direct_const_member | <code>this=&新快照</code> | 根需求结果返回并构造新快照后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1843 | F0349 | F0638 | <code>海中鱼巣/线程/自我线程.ixx:343-345</code> | condition_variable_predicate | <code>捕获this</code> | condition_variable::wait初检或唤醒后，可重复 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1844 | F0350 | F0163 | <code>海中鱼巣/领域/初始化.需求.ixx:48, 海中鱼巣/领域/初始化.需求.ixx:50, 海中鱼巣/领域/初始化.需求.ixx:51, 海中鱼巣/领域/初始化.需求.ixx:52, 海中鱼巣/领域/初始化.需求.ixx:53</code> | direct_free | <code>this->特征定义</code> | 函数进入；E1845返回true；E1846返回true；E1847返回true；E1848返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0108 | F0350 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:48, 海中鱼巣/领域/初始化.需求.ixx:50, 海中鱼巣/领域/初始化.需求.ixx:51, 海中鱼巣/领域/初始化.需求.ixx:52, 海中鱼巣/领域/初始化.需求.ixx:53</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1845 | F0350 | F0363 | <code>海中鱼巣/领域/初始化.需求.ixx:49</code> | direct_const_member | <code>this=&语素入口结果</code> | E1844返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1850 | F0351 | F0496 | <code>海中鱼巣/领域/初始化.概念图.ixx:213</code> | direct_const_member+unique_name | <code>this=&语素_,项.名称语素入口.语素入口</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1851 | F0351 | F0497 | <code>海中鱼巣/领域/初始化.概念图.ixx:214</code> | direct_const_member+unique_name | <code>this=&语素_,项.名称语素入口.语素入口</code> | E1850返回后；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| E1852 | F0352 | F0639 | <code>海中鱼巣/领域/概念图服务.h:893</code> | direct_static+unique_name | <code>类别</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1853 | F0352 | F0640 | <code>海中鱼巣/领域/概念图服务.h:897</code> | direct_static+unique_name | <code>类别</code> | E1852返回true且已取得根登记共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1854 | F0352 | F0377 | <code>海中鱼巣/领域/概念图服务.h:898</code> | direct_const_member | <code>this=&节点_,材料->根节点</code> | 登记槽有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1855 | F0353 | F0369 | <code>海中鱼巣/领域/概念图服务.h:1863</code> | direct_const_member+unique_name | <code>this=当前概念图服务,概念</code> | 已取得活动图共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1856 | F0353 | F0360 | <code>海中鱼巣/领域/概念图服务.h:1866</code> | direct_const_member | <code>this=当前概念图服务,概念</code> | E1855返回有值且仍持活动图共享锁 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1857 | F0354 | F0641 | <code>海中鱼巣/领域/概念图服务.h:970</code> | direct_static+unique_name | <code>阶段</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1858 | F0354 | F0642 | <code>海中鱼巣/领域/概念图服务.h:974</code> | direct_static+unique_name | <code>阶段</code> | E1857返回true且已取得生命周期状态共享锁；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1859 | F0354 | F0377 | <code>海中鱼巣/领域/概念图服务.h:975</code> | direct_const_member | <code>this=&节点_,状态.value()</code> | 登记optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1860 | F0355 | F0377 | <code>海中鱼巣/领域/概念图服务.h:909</code> | direct_const_member | <code>this=&节点_,材料->根节点</code> | 每个有值登记槽；循环中至多四次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE1655 | F0356 | F0190 | <code>海中鱼巣/领域/概念图服务.h:850</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取根节点记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0110 | F0356 | R0521 | <code>海中鱼巣/领域/概念图服务.h:851</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 根记录存在时映射根类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0111 | F0356 | F0639 | <code>海中鱼巣/领域/概念图服务.h:852</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 入口短路首先复核根类别 | 专项源码静态类型与实际装配人工复核 |
| RCE1656 | F0356 | R0088 | <code>海中鱼巣/领域/概念图服务.h:860</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 入口通过并取得图写锁后读取上位来源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1657 | F0356 | F0051 | <code>海中鱼巣/领域/概念图服务.h:870, 海中鱼巣/领域/概念图服务.h:875, 海中鱼巣/领域/概念图服务.h:885</code> | operator | <code>未单独冻结；读取源码调用点</code> | 稳定键前置成立后比较根节点；发布读回前三项成立后比较根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0112 | F0356 | F0640 | <code>海中鱼巣/领域/概念图服务.h:880, 海中鱼巣/领域/概念图服务.h:881</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 唯一性遍历完成后写入并读回根登记槽 | 专项源码静态类型与实际装配人工复核 |
| RCE0113 | F0356 | F0184 | <code>海中鱼巣/领域/概念图服务.h:882</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 发布后读回完整条件形成 | 专项源码静态类型与实际装配人工复核 |
| E1707 | F0357 | F0190 | <code>海中鱼巣/领域/概念图服务.h:938</code> | direct_const_member | <code>this=&节点_,状态节点</code> | F0357函数进入；函数进入读取状态节点记录 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1667 | F0357 | F0329 | <code>海中鱼巣/领域/概念图服务.h:939</code> | direct_const_member | <code>this=&状态,状态节点</code> | E1707节点记录读取完成；不要求记录有值，且位于if短路判断之前；节点记录读取完成后无条件读取状态值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0114 | F0357 | F0641 | <code>海中鱼巣/领域/概念图服务.h:940</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 入口短路首先复核生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| E1664 | F0357 | F0328 | <code>海中鱼巣/领域/概念图服务.h:945</code> | direct_const_member | <code>this=&状态,状态节点</code> | 阶段有效、节点可读且类型为状态、状态值存在并匹配阶段；阶段、节点类型和状态值前置全部成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0115 | F0357 | F0642 | <code>海中鱼巣/领域/概念图服务.h:950</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 取得生命周期状态独占锁后计算槽索引 | 专项源码静态类型与实际装配人工复核 |
| RCE1658 | F0357 | F0051 | <code>海中鱼巣/领域/概念图服务.h:953, 海中鱼巣/领域/概念图服务.h:956, 海中鱼巣/领域/概念图服务.h:962</code> | operator | <code>未单独冻结；读取源码调用点</code> | 各 optional has_value 前置成立后比较状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0116 | F0357 | F0184 | <code>海中鱼巣/领域/概念图服务.h:962</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 发布后读回完整条件形成 | 专项源码静态类型与实际装配人工复核 |
| RCE0117 | F0358 | F0377 | <code>海中鱼巣/领域/概念图服务.h:3005</code> | direct_const_member+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个已登记状态复核节点有效；optional 状态有值后复核节点有效；单实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0118 | F0358 | R0518 | <code>海中鱼巣/领域/概念图服务.h:3010</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 按需调用具名节点句柄小于；std::sort 注册同类静态比较函数 | 专项源码静态类型与实际装配人工复核 |
| RCE1659 | F0358 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3011</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::adjacent_find 默认相等谓词按需比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0119 | F0359 | F0369 | <code>海中鱼巣/领域/概念图服务.h:3047</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取概念类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0120 | F0359 | F0641 | <code>海中鱼巣/领域/概念图服务.h:3048</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 预期阶段有值时复核阶段有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0121 | F0359 | F0354 | <code>海中鱼巣/领域/概念图服务.h:3054</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 预期阶段有值时读取预期状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1660 | F0359 | F0575 | <code>海中鱼巣/领域/概念图服务.h:3060</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 入口通过后读取概念生命周期关系组 | 专项源码静态类型与实际装配人工复核 |
| RCE0122 | F0359 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3062, 海中鱼巣/领域/概念图服务.h:3068, 海中鱼巣/领域/概念图服务.h:3072, 海中鱼巣/领域/概念图服务.h:3090, 海中鱼巣/领域/概念图服务.h:3114, 海中鱼巣/领域/概念图服务.h:3121, 海中鱼巣/领域/概念图服务.h:3128</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 各具名内部一致性与清理结果检查按源码短路成立 | 专项源码静态类型与实际装配人工复核 |
| RCE1661 | F0359 | R0605 | <code>海中鱼巣/领域/概念图服务.h:3067</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 已有唯一关系时读取非令牌生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| RCE1662 | F0359 | R0606 | <code>海中鱼巣/领域/概念图服务.h:3078</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::count_if 注册并调用非平凡完整匹配 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1663 | F0359 | R0607 | <code>海中鱼巣/领域/概念图服务.h:3095</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册并调用非平凡编号版本匹配 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1664 | F0359 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3111, 海中鱼巣/领域/概念图服务.h:3124, 海中鱼巣/领域/概念图服务.h:3125</code> | operator | <code>未单独冻结；读取源码调用点</code> | any_of 简单 lambda 或写后读回短路前置成立后比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1665 | F0359 | F0167 | <code>海中鱼巣/领域/概念图服务.h:3119</code> | direct_member | <code>未单独冻结；读取源码调用点</code> | 允许创建、预期状态存在且无残留登记 | 专项源码静态类型与实际装配人工复核 |
| RCE1666 | F0359 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3120, 海中鱼巣/领域/概念图服务.h:3129</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 创建后无条件读回；删除失败后再次读回 | 专项源码静态类型与实际装配人工复核 |
| RCE0124 | F0359 | F0168 | <code>海中鱼巣/领域/概念图服务.h:3121, 海中鱼巣/领域/概念图服务.h:3127</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 写后读回检查及失败清理前复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1667 | F0359 | F0590 | <code>海中鱼巣/领域/概念图服务.h:3129</code> | direct_member | <code>未单独冻结；读取源码调用点</code> | 新关系句柄有效且写后检查失败 | 专项源码静态类型与实际装配人工复核 |
| RCE1668 | F0360 | F0575 | <code>海中鱼巣/领域/概念图服务.h:3140</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入读取概念生命周期关系组 | 专项源码静态类型与实际装配人工复核 |
| RCE1669 | F0360 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3144, 海中鱼巣/领域/概念图服务.h:3149, 海中鱼巣/领域/概念图服务.h:3165, 海中鱼巣/领域/概念图服务.h:3170</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 唯一关系、阶段、登记一致性和最终命中检查 | 专项源码静态类型与实际装配人工复核 |
| RCE1670 | F0360 | R0605 | <code>海中鱼巣/领域/概念图服务.h:3148</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 唯一记录已借用后读取非令牌生命周期阶段 | 专项源码静态类型与实际装配人工复核 |
| RCE1671 | F0360 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3154, 海中鱼巣/领域/概念图服务.h:3162, 海中鱼巣/领域/概念图服务.h:3163</code> | operator | <code>未单独冻结；读取源码调用点</code> | 登记/记录节点比较按短路顺序执行 | 专项源码静态类型与实际装配人工复核 |
| RCE1672 | F0360 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3157</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 登记概念与状态匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1673 | F0360 | R0608 | <code>海中鱼巣/领域/概念图服务.h:3174</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 组合生命周期材料后复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1674 | F0361 | R0598 | <code>海中鱼巣/领域/概念图服务.h:3226</code> | operator | <code>未单独冻结；读取源码调用点</code> | std::find_if 简单 lambda 比较登记关系与待清理关系 | 专项源码静态类型与实际装配人工复核 |
| RCE0125 | F0361 | F0590 | <code>海中鱼巣/领域/概念图服务.h:3231</code> | direct_member+direct-member | <code>未单独冻结；读取源码调用点</code> | 生命周期登记命中后删除待清理关系；反向清理已登记生命周期关系；单实参写重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0126 | F0361 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3232</code> | direct_const_member+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 删除返回 false 后复核关系已不可读；删除失败后复核关系已不可读；单实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0127 | F0362 | R0531 | <code>海中鱼巣/领域/概念图服务.h:4202</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 逐根复核概念根登记 | 专项源码静态类型与实际装配人工复核 |
| RCE1675 | F0362 | F0051 | <code>海中鱼巣/领域/概念图服务.h:4203, 海中鱼巣/领域/概念图服务.h:4216, 海中鱼巣/领域/概念图服务.h:4235, 海中鱼巣/领域/概念图服务.h:4236, 海中鱼巣/领域/概念图服务.h:4267, 海中鱼巣/领域/概念图服务.h:4270, 海中鱼巣/领域/概念图服务.h:4281, 海中鱼巣/领域/概念图服务.h:4285, 海中鱼巣/领域/概念图服务.h:4290, 海中鱼巣/领域/概念图服务.h:4296</code> | operator+standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find/unique 默认比较、显式端点比较和简单 any_of lambda 比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0128 | F0362 | F0358 | <code>海中鱼巣/领域/概念图服务.h:4208</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 根与概念集合通过后复核生命周期状态登记完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1676 | F0362 | F0360 | <code>海中鱼巣/领域/概念图服务.h:4212</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 逐个快照概念读取非令牌生命周期材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0130 | F0362 | F0369 | <code>海中鱼巣/领域/概念图服务.h:4223, 海中鱼巣/领域/概念图服务.h:4249</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 读取签名概念与实例来源概念类别 | 专项源码静态类型与实际装配人工复核 |
| RCE0131 | F0362 | R0532 | <code>海中鱼巣/领域/概念图服务.h:4226</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 类别匹配前置后规范化签名 | 专项源码静态类型与实际装配人工复核 |
| RCE1677 | F0362 | F0587 | <code>海中鱼巣/领域/概念图服务.h:4231</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 逐个快照关系读取当前关系记录 | 专项源码静态类型与实际装配人工复核 |
| RCE1678 | F0362 | R0604 | <code>海中鱼巣/领域/概念图服务.h:4232</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 读取关系后复核概念活动关系材料完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0132 | F0362 | R0523 | <code>海中鱼巣/领域/概念图服务.h:4240, 海中鱼巣/领域/概念图服务.h:4241, 海中鱼巣/领域/概念图服务.h:4250, 海中鱼巣/领域/概念图服务.h:4259</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 按关系角色从快照读取源或目标概念签名 | 专项源码静态类型与实际装配人工复核 |
| RCE0133 | F0362 | R0533 | <code>海中鱼巣/领域/概念图服务.h:4244</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 上下位两份签名均存在后复核一般性 | 专项源码静态类型与实际装配人工复核 |
| RCE1679 | F0362 | F0190 | <code>海中鱼巣/领域/概念图服务.h:4258</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 其它关系角色读取目标材料节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0134 | F0362 | R0522 | <code>海中鱼巣/领域/概念图服务.h:4261</code> | direct_static | <code>未单独冻结；读取源码调用点</code> | 源签名和目标材料记录存在后复核类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1680 | F0362 | R0518 | <code>海中鱼巣/领域/概念图服务.h:4295</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 按需调用具名节点句柄小于 | 专项源码静态类型与实际装配人工复核 |
| RCE0135 | F0363 | F0168 | <code>海中鱼巣/领域/语素服务.h:37, 海中鱼巣/领域/语素服务.h:38</code> | direct_free | <code>对应关系；概念关系</code> | F0163@35 与 F0565@36 均返回 true 后调用对应关系；对应关系也返回 true 后调用概念关系 | 源码逐行与重载类型审计 PASS |
| RCE0136 | F0364 | R0571 | <code>海中鱼巣/领域/语素服务.h:101</code> | direct_const_member | <code>this=&语素服务,对应信息节点</code> | R0575@96 返回可作为最小词单元 | 源码逐行与静态接收者审计 PASS |
| RCE0137 | F0364 | R0572 | <code>海中鱼巣/领域/语素服务.h:106</code> | direct_const_member | <code>this=&语素服务,概念节点</code> | R0571@101 返回 true | 源码逐行、实参数量与重载类型审计 PASS |
| RCE0138 | F0364 | R0573 | <code>海中鱼巣/领域/语素服务.h:111</code> | direct_member | <code>this=&语素服务,对应信息节点,主键</code> | R0572@106 返回 true | 源码逐行与静态接收者审计 PASS |
| RCE0139 | F0364 | F0168 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_free | <code>概念关系</code> | F0167@115 已返回概念关系 | 源码逐行与重载类型审计 PASS |
| RCE0140 | F0364 | F0497 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_const_member | <code>this=&语素服务,结果.语素入口</code> | F0168@116 返回 true；逻辑与短路继续 | 源码逐行与短路顺序审计 PASS |
| RCE0141 | F0364 | R0570 | <code>海中鱼巣/领域/语素服务.h:116</code> | direct_const_member | <code>this=&语素服务,F0497返回的概念追溯目标组,概念节点</code> | F0168@116 返回 true 且 F0497 已返回目标组 | 源码逐行与短路顺序审计 PASS |
| RCE0142 | F0364 | R0574 | <code>海中鱼巣/领域/语素服务.h:119</code> | direct_member | <code>this=&语素服务,结果</code> | 概念关系无效，或概念追溯目标组不包含概念节点 | 源码逐行与失败分支审计 PASS |
| RCE0143 | F0364 | R0575 | <code>海中鱼巣/领域/语素服务.h:96</code> | direct_const_member | <code>this=&语素服务,文本</code> | F0364函数进入 | 源码逐行与静态接收者审计 PASS |
| RCE0144 | F0365 | F0184 | <code>海中鱼巣/领域/存在服务.h:107</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0145 | F0366 | F0184 | <code>海中鱼巣/领域/动态服务.h:290</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0146 | F0366 | R0610 | <code>海中鱼巣/领域/动态服务.h:294</code> | unique_name | <code>this, 根节点</code> | 记录存在、类型为动态、主信息有效且普通动态材料不存在 | 逐调用点源码复核：海中鱼巣/领域/动态服务.h:294 仅传入一个节点句柄，匹配 R0610；排除两参数事务重载 R0250 |
| RCE0147 | F0367 | R0206 | <code>海中鱼巣/领域/二次特征服务.h:50</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0148 | F0367 | F0184 | <code>海中鱼巣/领域/二次特征服务.h:52</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0149 | F0367 | R0204 | <code>海中鱼巣/领域/二次特征服务.h:55</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0151 | F0368 | F0184 | <code>海中鱼巣/领域/因果服务.h:59</code> | unique_name | <code>this=&因果服务, 记录.has_value() && 记录->类型==因果引用 && 主信息_.主信息是否有效(记录->主信息), 固定说明</code> | 记录读取返回后总是形成短路条件并调用 | 当前源码逐调用点与静态接收者复核 |
| RCE0152 | F0369 | R0521 | <code>海中鱼巣/领域/概念图服务.h:931</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0153 | F0370 | F0369 | <code>海中鱼巣/领域/概念图服务.h:1032, 海中鱼巣/领域/概念图服务.h:1033</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0154 | F0370 | R0531 | <code>海中鱼巣/领域/概念图服务.h:1038</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0155 | F0370 | R0526 | <code>海中鱼巣/领域/概念图服务.h:1041</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0156 | F0371 | F0621 | <code>海中鱼巣/领域/概念图服务.h:1045</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 实例到实例支持概念；二实参目标节点组重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0157 | F0372 | R0088 | <code>海中鱼巣/领域/概念图服务.h:1049</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 概念到实例支持概念；二实参来源节点组重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0158 | F0373 | F0192 | <code>海中鱼巣/领域/统计服务.h:959</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0159 | F0373 | F0193 | <code>海中鱼巣/领域/统计服务.h:960</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0160 | F0373 | F0194 | <code>海中鱼巣/领域/统计服务.h:963</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0161 | F0373 | F0195 | <code>海中鱼巣/领域/统计服务.h:964</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1802 | F0374 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:469</code> | direct_free_internal_linkage+direct-free | <code>事务接线_,令牌</code> | 函数进入；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE1681 | F0375 | R0600 | <code>海中鱼巣/核心/结构事务接线.数据.h:28</code> | direct-operator | <code>未单独冻结；读取源码调用点</code> | 移动构造函数体委托移动赋值 | 专项源码静态类型与实际装配人工复核 |
| RCE0162 | F0376 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:419</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0163 | F0377 | F0630 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | <code>this=F0377接收者, 节点=节点, 令牌=RCE1740返回引用</code> | RCE1739返回true且RCE1740完成后调用；RCE1739返回false时短路不调用 | 当前源码、接收者静态类型、二实参重载与&amp;&amp;左到右顺序复核 |
| RCE0164 | F0377 | F0190 | <code>海中鱼巣/核心/节点仓库.cpp:446</code> | direct_const_member+source_audited | <code>this=F0377接收者, 节点=节点</code> | RCE1737返回false；源码上同时覆盖完全未接域与接线不完整形态 | 当前源码、接收者静态类型与单实参重载复核 |
| RCE0165 | F0381 | F0207 | <code>海中鱼巣/适配/SQL数据库适配.cpp:251</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0166 | F0381 | F0208 | <code>海中鱼巣/适配/SQL数据库适配.cpp:252</code> | field_type_hint+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0167 | F0381 | F0209 | <code>海中鱼巣/适配/SQL数据库适配.cpp:257, 海中鱼巣/适配/SQL数据库适配.cpp:261</code> | field_type_hint+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0168 | F0381 | F0210 | <code>海中鱼巣/适配/SQL数据库适配.cpp:260</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0169 | F0381 | F0211 | <code>海中鱼巣/适配/SQL数据库适配.cpp:261</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0170 | F0383 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:447</code> | suffix_match+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0171 | F0384 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0172 | F0385 | R0247 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:52</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0173 | F0385 | R0149 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:49</code> | lsp_direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0174 | F0385 | R0248 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:49</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0175 | F0385 | R0240 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:50</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| E1790 | F0387 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:733-758</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1818 | F0387 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:734, 海中鱼巣/核心/关系仓库.cpp:733-758</code> | implicit_destructor | <code>this=&取得共享许可返回的已移空临时许可; this=&自动许可.value()</code> | 共享许可返回临时量经RCE1756移动构造后的完整表达式末；或正常/异常退出且自动许可已承载，后者严格发生在F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；当前宏展开与C++临时量/RAII生命周期复核 |
| E1765 | F0387 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:736, 海中鱼巣/核心/关系仓库.cpp:753</code> | direct_const_member+direct-member+direct-member | <code>this,父节点</code> | 函数入口许可范围建立；当前记录匹配父节点；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0176 | F0388 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:387</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0177 | F0388 | R0304 | <code>海中鱼巣/界面/控制面板窗口.cpp:387</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0178 | F0388 | R0306 | <code>海中鱼巣/界面/控制面板窗口.cpp:388</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0179 | F0388 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:406</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0180 | F0388 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:411</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0181 | F0389 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:428, 海中鱼巣/界面/控制面板窗口.cpp:445</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0182 | F0390 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:470</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0183 | F0391 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:480, 海中鱼巣/界面/控制面板窗口.cpp:488</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0184 | F0391 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:493</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0185 | F0392 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:509</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0186 | F0396 | R0151 | <code>海中鱼巣/界面/控制面板窗口.cpp:1609</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0187 | F0396 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1617</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0188 | F0396 | R0185 | <code>海中鱼巣/界面/控制面板窗口.cpp:1626</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0189 | F0396 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:1633</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0190 | F0396 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1634</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0191 | F0396 | R0160 | <code>海中鱼巣/界面/控制面板窗口.cpp:1644</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0192 | F0396 | R0201 | <code>海中鱼巣/界面/控制面板窗口.cpp:1652</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0193 | F0396 | R0166 | <code>海中鱼巣/界面/控制面板窗口.cpp:1660</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0194 | F0397 | R0101 | <code>海中鱼巣/核心/协调.结构事务.ixx:152</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 正式接线共享许可 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0195 | F0398 | R0101 | <code>海中鱼巣/核心/协调.结构事务.ixx:153</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 正式接线独占许可 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0196 | F0399 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:154</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 正式接线共享令牌验证 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0197 | F0400 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:155</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 正式接线独占令牌验证 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0198 | F0401 | R0099 | <code>海中鱼巣/核心/协调.结构事务.ixx:156</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 正式接线撤销失败隔离 lambda 被调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0199 | F0403 | R0114 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:321</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 状态动态数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0200 | F0404 | R0114 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:468</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 特征体系数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0201 | F0404 | R0265 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:468</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 特征体系数据操作构造原始材料侧表访问器 | 专项源码静态类型与实际装配人工复核 |
| RCE0202 | F0404 | R0272 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:469</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 特征体系数据操作构造批次发布记录只读访问器；只构造访问器，不使未接线批次写入路径可达 | 专项源码静态类型与实际装配人工复核 |
| RCE0203 | F0405 | R0114 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:215</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 语素基础数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0204 | F0407 | R0114 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:254</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 概念图结构数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0205 | F0408 | R0114 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:875</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 需求任务方法数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0206 | F0410 | R0114 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:34</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 概念活动数据操作构造其结构写入执行器 | 专项源码静态类型与实际装配人工复核 |
| RCE0207 | F0439 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:213</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| E1798 | F0440 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:1430-1433</code> | implicit_destructor | <code>this=&令牌范围</code> | 共享令牌验证通过且范围已构造，随后正常或异常退出 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0209 | F0440 | F0587 | <code>海中鱼巣/核心/关系仓库.cpp:1432</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0210 | F0441 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:189</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0211 | F0442 | F0336 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:154</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0212 | F0443 | F0336 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:325</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0213 | F0444 | F0336 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:472</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0214 | F0445 | F0336 | <code>海中鱼巣/领域/数据操作.语素基础.ixx:218</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0215 | F0446 | F0336 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:105</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0216 | F0447 | F0336 | <code>海中鱼巣/领域/数据操作.概念图结构.ixx:257</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0217 | F0448 | F0336 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:878</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0218 | F0449 | F0336 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:39</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0219 | F0450 | F0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:38</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0220 | F0454 | R0330 | <code>海中鱼巣/领域/初始化.系统角色.ixx:178</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0221 | F0454 | R0239 | <code>海中鱼巣/领域/初始化.系统角色.ixx:179</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0222 | F0454 | R0236 | <code>海中鱼巣/领域/初始化.系统角色.ixx:180</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0223 | F0455 | R0229 | <code>海中鱼巣/领域/初始化.系统角色.ixx:103, 海中鱼巣/领域/初始化.系统角色.ixx:111</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0224 | F0455 | R0499 | <code>海中鱼巣/领域/初始化.系统角色.ixx:146</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0225 | F0455 | R0231 | <code>海中鱼巣/领域/初始化.系统角色.ixx:154</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0226 | F0455 | R0236 | <code>海中鱼巣/领域/初始化.系统角色.ixx:156, 海中鱼巣/领域/初始化.系统角色.ixx:162</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0227 | F0455 | R0409 | <code>海中鱼巣/领域/初始化.系统角色.ixx:160</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0228 | F0455 | R0422 | <code>海中鱼巣/领域/初始化.系统角色.ixx:70, 海中鱼巣/领域/初始化.系统角色.ixx:152</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0229 | F0455 | R0232 | <code>海中鱼巣/领域/初始化.系统角色.ixx:72</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0230 | F0455 | R0496 | <code>海中鱼巣/领域/初始化.系统角色.ixx:77</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0231 | F0455 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:86, 海中鱼巣/领域/初始化.系统角色.ixx:87</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 系统角色初始化读取写前预检中的既有身份与稳定键 | 专项源码静态类型与实际装配人工复核 |
| RCE0232 | F0458 | R0456 | <code>海中鱼巣/领域/系统角色清单.数据.h:188, 海中鱼巣/领域/系统角色清单.数据.h:189, 海中鱼巣/领域/系统角色清单.数据.h:190, 海中鱼巣/领域/系统角色清单.数据.h:191, 海中鱼巣/领域/系统角色清单.数据.h:192, 海中鱼巣/领域/系统角色清单.数据.h:193, 海中鱼巣/领域/系统角色清单.数据.h:194, 海中鱼巣/领域/系统角色清单.数据.h:199</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0233 | F0458 | R0564 | <code>海中鱼巣/领域/系统角色清单.数据.h:196</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0234 | F0459 | F0168 | <code>海中鱼巣/领域/系统角色清单.数据.h:134</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0235 | F0461 | R0331 | <code>海中鱼巣/领域/服务.概念活动.ixx:73</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0236 | F0461 | R0500 | <code>海中鱼巣/领域/服务.概念活动.ixx:74</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0237 | F0461 | R0539 | <code>海中鱼巣/领域/服务.概念活动.ixx:75</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0238 | F0462 | R0500 | <code>海中鱼巣/领域/服务.概念活动.ixx:33</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0239 | F0462 | R0331 | <code>海中鱼巣/领域/服务.概念活动.ixx:38</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0240 | F0462 | R0340 | <code>海中鱼巣/领域/服务.概念活动.ixx:59</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0241 | F0464 | F0465 | <code>海中鱼巣/领域/概念活动状态.数据.h:146</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐项比较两侧概念活动根材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0242 | F0465 | R0542 | <code>海中鱼巣/领域/概念活动状态.数据.h:117, 海中鱼巣/领域/概念活动状态.数据.h:118</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE1683 | F0466 | R0538 | <code>海中鱼巣/领域/概念活动状态.数据.h:138</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 重建视图逐根复核给定活跃角色 | 专项源码静态类型与实际装配人工复核 |
| RCE0244 | F0467 | F0163 | <code>海中鱼巣/领域/概念活动状态.数据.h:65</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 状态节点静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0245 | F0467 | F0565 | <code>海中鱼巣/领域/概念活动状态.数据.h:65</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 主信息静态类型为主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0246 | F0468 | F0013 | <code>海中鱼巣/自检.入口初始化.ixx:55</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0247 | F0468 | F0014 | <code>海中鱼巣/自检.入口初始化.ixx:56</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0248 | F0468 | R0083 | <code>海中鱼巣/自检.入口初始化.ixx:61</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0249 | F0468 | F0199 | <code>海中鱼巣/自检.入口初始化.ixx:62</code> | direct_const_member+source_audited | <code>无实参；接收者静态类型与无参 const 重载逐调用点复核</code> | 调用方可达且初始化仓库自检路径进入数量读取 | 当前源码调用点、接收者类型与重载复核 |
| E0992 | F0470 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:80</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0993 | F0471 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:81</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0994 | F0472 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:82</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0995 | F0473 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:83</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0996 | F0474 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:84</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0997 | F0475 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:85</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E0998 | F0476 | F0190 | <code>海中鱼巣/自检.入口初始化.ixx:89</code> | captured_member_direct | <code>上下文.节点隐式this,根.根节点</code> | F0254调用类型匹配 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1011 | F0477 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:109</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1012 | F0478 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:110</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1013 | F0479 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:111</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1014 | F0480 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:112</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1015 | F0481 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:113</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1016 | F0482 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:114</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1033 | F0483 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:137</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1034 | F0484 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:138</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1035 | F0485 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:139</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1036 | F0486 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:140</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1037 | F0487 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:141</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1038 | F0488 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:142</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用对应端口 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1054 | F0489 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:166</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1055 | F0490 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:167</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1056 | F0491 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:168</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1057 | F0492 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:169</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1058 | F0493 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:170</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1059 | F0494 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:171</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1060 | F0495 | F0496 | <code>海中鱼巣/自检.入口初始化.ixx:175</code> | captured_member_direct | <code>上下文.语素隐式this,根.名称语素入口.语素入口</code> | F0257调用绑定可读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1061 | F0495 | F0497 | <code>海中鱼巣/自检.入口初始化.ixx:176</code> | captured_member_direct | <code>上下文.语素隐式this,根.名称语素入口.语素入口</code> | F0257调用绑定可读 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1062 | F0495 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:177, 海中鱼巣/自检.入口初始化.ixx:178</code> | std_find_element_compare | <code>绑定组元素,根.根节点</code> | 第一组std::find逐元素调用0..n次；E1062所在第一组命中后，第二组std::find逐元素调用0..n次 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0250 | F0496 | R0568 | <code>海中鱼巣/领域/语素服务.h:189</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0251 | F0497 | R0568 | <code>海中鱼巣/领域/语素服务.h:196</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1087 | F0498 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:199</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1088 | F0499 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:200</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1089 | F0500 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:201</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1090 | F0501 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:202</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1091 | F0502 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:203</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1092 | F0503 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:204</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1106 | F0504 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:231</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1107 | F0505 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:232</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1108 | F0506 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:233</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1109 | F0507 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:234</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1110 | F0508 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:235</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1111 | F0509 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:236</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1128 | F0510 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:255</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1129 | F0511 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:256</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1130 | F0512 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:257</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1131 | F0513 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:258</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1132 | F0514 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:259</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1133 | F0515 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:260</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1156 | F0517 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:279</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1157 | F0518 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:280</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1158 | F0519 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:281</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1159 | F0520 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:282</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1160 | F0521 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:283</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1161 | F0522 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:284</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0252 | F0523 | R0147 | <code>海中鱼巣/核心/节点仓库.cpp:334</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1178 | F0524 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:312</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1179 | F0525 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:313</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1180 | F0526 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:314</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1181 | F0527 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:315</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1182 | F0528 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:316</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1183 | F0529 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:317</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1199 | F0532 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:337</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1200 | F0533 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:338</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1201 | F0534 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:339</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1202 | F0535 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:340</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1203 | F0536 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:341</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1204 | F0537 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:342</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1221 | F0538 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:369</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1222 | F0539 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:370</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1223 | F0540 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:371</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1224 | F0541 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:372</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1225 | F0542 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:373</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1226 | F0543 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:374</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0253 | F0544 | F0441 | <code>海中鱼巣/核心/索引仓库.cpp:175</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1241 | F0545 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:395</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1242 | F0546 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:396</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1243 | F0547 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:397</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1244 | F0548 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:398</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1245 | F0549 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:399</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1246 | F0550 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:400</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1247 | F0551 | F0552 | <code>海中鱼巣/自检.入口初始化.ixx:405</code> | captured_member_direct | <code>this=&上下文.需求,单根.根需求</code> | F0551进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1248 | F0551 | F0350 | <code>海中鱼巣/自检.入口初始化.ixx:406</code> | direct_const_member | <code>this=&单根</code> | E1247返回 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1250 | F0551 | F0051 | <code>海中鱼巣/自检.入口初始化.ixx:407-408, 海中鱼巣/自检.入口初始化.ixx:409-411</code> | std_optional_element_compare | <code>目标特征候选.value(),单根.特征定义</code> | E1249返回optional有值；E1251返回optional有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1249 | F0551 | F0553 | <code>海中鱼巣/自检.入口初始化.ixx:407</code> | captured_member_direct | <code>this=&上下文.需求,单根.根需求</code> | E1248返回true且承接有值 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1251 | F0551 | F0554 | <code>海中鱼巣/自检.入口初始化.ixx:409-410</code> | captured_member_direct | <code>this=&上下文.特征,结果.自我初始化->世界树.自我存在节点,单根.特征定义</code> | E1250返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1253 | F0551 | F0555 | <code>海中鱼巣/自检.入口初始化.ixx:412</code> | captured_member_direct | <code>this=&上下文.特征,单根.实例特征槽位,单根.当前特征值</code> | E1252返回true | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1254 | F0551 | F0329 | <code>海中鱼巣/自检.入口初始化.ixx:414</code> | captured_member_direct | <code>this=&上下文.状态,单根.目标状态</code> | E1253等于当前参数 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0254 | F0553 | R0576 | <code>海中鱼巣/领域/需求服务.h:544, 海中鱼巣/领域/需求服务.h:548</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0255 | F0554 | R0559 | <code>海中鱼巣/领域/特征服务.h:263</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0256 | F0554 | R0561 | <code>海中鱼巣/领域/特征服务.h:263, 海中鱼巣/领域/特征服务.h:268</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0257 | F0555 | R0558 | <code>海中鱼巣/领域/特征服务.h:364</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| E1274 | F0556 | F0043 | <code>海中鱼巣/自检.入口初始化.ixx:433</code> | captured_member_direct | <code>上下文.方法隐式this,键,上下文.状态</code> | F0015调用方法登记根回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1275 | F0557 | F0044 | <code>海中鱼巣/自检.入口初始化.ixx:434</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,参数</code> | F0015调用自我线程启动回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1276 | F0558 | F0045 | <code>海中鱼巣/自检.入口初始化.ixx:435</code> | captured_member_direct | <code>上下文.自我线程实例隐式this,时限</code> | F0015调用等待初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1277 | F0559 | F0046 | <code>海中鱼巣/自检.入口初始化.ixx:436</code> | captured_member_direct | <code>上下文.自我线程实例隐式this</code> | F0015调用读取快照回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1278 | F0560 | F0048 | <code>海中鱼巣/自检.入口初始化.ixx:437</code> | captured_member_direct | <code>上下文.概念图初始化隐式this</code> | F0015调用概念图四根初始化回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1279 | F0561 | F0050 | <code>海中鱼巣/自检.入口初始化.ixx:438</code> | captured_member_direct | <code>上下文.概念图隐式this,实例</code> | F0015调用存在实例根支持回调 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0258 | F0566 | F0568 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:459, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:460, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:470, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:475, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:478, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:481, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:484, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:486, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:487, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:488, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:490, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:491, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:492, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:498</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0259 | F0566 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:461, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:462, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:470, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:475, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:478, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:481, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:484, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:486, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:487, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:488, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:493, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:494, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:495, 海中鱼巣/核心/自检.关系仓库性能基线.ixx:500</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0260 | F0568 | R0140 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:445</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0261 | F0568 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:448</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0262 | F0569 | F0591 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:511</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 固定失效变更 | 专项源码静态类型与实际装配人工复核 |
| RCE0263 | F0569 | F0592 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:512</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 读取失效返回材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0264 | F0569 | F0589 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:513</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 按失效后的当前关系读取审计 | 专项源码静态类型与实际装配人工复核 |
| RCE0265 | F0569 | F0590 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:518</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 固定删除变更；单实参写重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0266 | F0569 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:524</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 固定重挂并返回新句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0267 | F0569 | F0587 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:527</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 按重挂返回句柄读回 | 专项源码静态类型与实际装配人工复核 |
| E1793 | F0575 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:843-863</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1824 | F0575 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:843-863</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1755 | F0575 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:846</code> | direct_free+direct-free | <code>类型</code> | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1771 | F0575 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:846, 海中鱼巣/核心/关系仓库.cpp:855</code> | direct_const_member+direct-member+direct-member | <code>this,源节点</code> | 函数入口许可范围建立；记录有效且类型和源节点匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0268 | F0577 | F0588 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:61</code> | same_module | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1794 | F0578 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:919-947</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1826 | F0578 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:919-947</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1756 | F0578 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:922</code> | direct_free+direct-free | <code>类型</code> | 目标节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1773 | F0578 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:922, 海中鱼巣/核心/关系仓库.cpp:939</code> | direct_const_member+direct-member+direct-member | <code>this,目标节点</code> | 函数入口许可范围建立；当前记录类型和目标节点匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1796 | F0579 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:980-994</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1830 | F0579 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:980-994</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1757 | F0579 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:982</code> | direct_free+direct-free | <code>类型</code> | 源、目标节点均当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1778 | F0579 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:982</code> | direct_const_member+direct-member+direct-member | <code>this,源节点</code> | 函数入口许可范围建立；源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1780 | F0580 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:1005, 海中鱼巣/核心/关系仓库.cpp:998</code> | direct_const_member+direct-member+direct-member | <code>this,目标节点</code> | 函数入口许可范围建立；记录有效且类型和目标节点匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1797 | F0580 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:996-1010</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1832 | F0580 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:996-1010</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1758 | F0580 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:998</code> | direct_free+direct-free | <code>类型</code> | 目标节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1795 | F0581 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:949-978</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1828 | F0581 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:949-978</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1775 | F0581 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:952, 海中鱼巣/核心/关系仓库.cpp:970, 海中鱼巣/核心/关系仓库.cpp:971</code> | direct_const_member+lambda_direct_const_member+lambda_direct_const_member+direct-member+direct-member+direct-member | <code>this,节点</code> | 函数入口许可范围建立；lambda处理当前有效记录；记录源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1786 | F0582 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:314-337</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1810 | F0582 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:314-337</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1752 | F0582 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:319</code> | direct_free+direct-free | <code>类型</code> | 源节点句柄有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0269 | F0582 | R0085 | <code>海中鱼巣/核心/关系仓库.cpp:326</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1365 | F0584 | F0279 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:666</code> | lambda_direct_const_member | <code>this=&夹具,密集正向</code> | 窗口内且正确位为true的九读循环 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1367 | F0584 | F0586 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:676-677</code> | lambda_direct_member | <code>写材料.关系,写材料.源节点,新目标</code> | 关系仓库借用已取得 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1366 | F0584 | F0288 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:676</code> | lambda_direct_member | <code>this=&夹具</code> | 九次读取成功 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0270 | F0585 | F0168 | <code>海中鱼巣/核心/自检.关系仓库性能基线.ixx:333</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| E1789 | F0586 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:431-479</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1816 | F0586 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:431-479</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1763 | F0586 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:441</code> | direct_member+direct-member+direct-member | <code>this,新源节点</code> | 新端点句柄有效；新源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1784 | F0587 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:277-293</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1806 | F0587 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:277-293</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1759 | F0587 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:289</code> | direct_const_member+direct-member+direct-member | <code>this,记录.源节点</code> | 记录当前有效且前置形状通过；源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1785 | F0589 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:295-312</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1808 | F0589 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:295-312</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0271 | F0589 | R0085 | <code>海中鱼巣/核心/关系仓库.cpp:308</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1788 | F0590 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:409-424</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1814 | F0590 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:409-424</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1787 | F0591 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:339-407</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1812 | F0591 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:339-407</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1761 | F0591 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:366</code> | direct_member+direct-member+direct-member | <code>this,记录.源节点</code> | 记录存在且状态转换准入通过；源节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0272 | F0592 | F0168 | <code>海中鱼巣/核心/关系仓库.h:116, 海中鱼巣/核心/关系仓库.h:117, 海中鱼巣/核心/关系仓库.h:118, 海中鱼巣/核心/关系仓库.h:119</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE1684 | F0592 | R0598 | <code>海中鱼巣/核心/关系仓库.h:134</code> | operator | <code>未单独冻结；读取源码调用点</code> | 已在目标状态分支比较当前与原关系句柄；状态为已在目标状态、前一转换分支未短路且原状态为已失效时比较当前关系与原关系 | 专项源码静态类型与实际装配人工复核 |
| RCE0273 | F0619 | F0383 | <code>海中鱼巣/核心/主信息仓库.cpp:425</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0274 | F0619 | F0217 | <code>海中鱼巣/核心/主信息仓库.cpp:427</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1792 | F0620 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:823-841</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1822 | F0620 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:823-841</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1754 | F0620 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:826</code> | direct_free+direct-free | <code>类型</code> | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1769 | F0620 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:826, 海中鱼巣/核心/关系仓库.cpp:833</code> | direct_const_member+direct-member+direct-member | <code>this,源节点</code> | 函数入口许可范围建立；记录有效且类型、源节点和顺序号匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| E1791 | F0621 | F0344 | <code>海中鱼巣/核心/关系仓库.cpp:790-821</code> | implicit_destructor | <code>this=&自动令牌范围.value()</code> | 正常或异常退出且自动令牌范围已承载 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1820 | F0621 | F0345 | <code>海中鱼巣/核心/关系仓库.cpp:790-821</code> | implicit_destructor | <code>this=&自动许可.value()</code> | 正常或异常退出且自动许可已承载；F0344之后 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| E1753 | F0621 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:798</code> | direct_free+direct-free | <code>类型</code> | 源节点当前有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| E1767 | F0621 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:798, 海中鱼巣/核心/关系仓库.cpp:805</code> | direct_const_member+direct-member+direct-member | <code>this,源节点</code> | 函数入口许可范围建立；记录有效且类型和源节点匹配；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0275 | F0622 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:54</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0276 | F0623 | F0625 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| E1803 | F0623 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | direct_free_internal_linkage+direct-free | <code>事务接线_,令牌</code> | 函数进入；后继与主信息有效和节点类型门禁短路；调用方可达且 libclang 直接引用项目定义；源码位置复核 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；专项源码静态类型与实际装配人工复核 |
| RCE0277 | F0623 | R0009 | <code>海中鱼巣/核心/节点仓库.cpp:75</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0278 | F0624 | R0009 | <code>海中鱼巣/核心/主信息仓库.cpp:366</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0279 | F0624 | F0384 | <code>海中鱼巣/核心/主信息仓库.cpp:368</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0280 | F0627 | R0010 | <code>海中鱼巣/核心/主信息仓库.cpp:378</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0281 | F0627 | F0628 | <code>海中鱼巣/核心/主信息仓库.cpp:380</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0282 | F0628 | R0010 | <code>海中鱼巣/核心/主信息仓库.cpp:386</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| E1708 | F0629 | F0330 | <code>海中鱼巣/领域/方法服务.h:1708</code> | direct_const_member | <code>this=&方法服务,源节点,类型,目标类型,std::nullopt</code> | F0629函数进入 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核 |
| RCE0283 | F0630 | F0346 | <code>海中鱼巣/核心/节点仓库.cpp:450</code> | suffix_match+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0284 | F0631 | F0338 | <code>海中鱼巣/核心/结构事务接线.数据.h:59</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0285 | F0631 | R0100 | <code>海中鱼巣/核心/结构事务接线.数据.h:59</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 许可有效且保存的释放函数指针唯一 | 专项源码静态类型与实际装配人工复核 |
| E1804 | F0632 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:24</code> | direct_const_member+unique_name | <code>this=&接线</code> | 函数进入；调用方可达且源码分支条件成立 | HEAD 已发布图谱；源码自冻结提交至当前基线零差异；Debug\|x64 条件复核；全项目唯一函数名并经调用方源码范围复核 |
| RCE0286 | F0632 | F0399 | <code>海中鱼巣/核心/节点仓库.cpp:24</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 接线已接域；生产运行期唯一共享验证目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0287 | F0633 | R0241 | <code>海中鱼巣/领域/初始化.语素.ixx:146, 海中鱼巣/领域/初始化.语素.ixx:147, 海中鱼巣/领域/初始化.语素.ixx:148, 海中鱼巣/领域/初始化.语素.ixx:149, 海中鱼巣/领域/初始化.语素.ixx:150, 海中鱼巣/领域/初始化.语素.ixx:151, 海中鱼巣/领域/初始化.语素.ixx:152, 海中鱼巣/领域/初始化.语素.ixx:153, 海中鱼巣/领域/初始化.语素.ixx:154, 海中鱼巣/领域/初始化.语素.ixx:155, 海中鱼巣/领域/初始化.语素.ixx:156, 海中鱼巣/领域/初始化.语素.ixx:157, 海中鱼巣/领域/初始化.语素.ixx:158, 海中鱼巣/领域/初始化.语素.ixx:159, 海中鱼巣/领域/初始化.语素.ixx:160, 海中鱼巣/领域/初始化.语素.ixx:161, 海中鱼巣/领域/初始化.语素.ixx:162, 海中鱼巣/领域/初始化.语素.ixx:163</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0288 | F0633 | R0246 | <code>海中鱼巣/领域/初始化.语素.ixx:165</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0289 | F0635 | R0203 | <code>海中鱼巣/领域/初始化.世界树.ixx:54</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0290 | F0635 | R0275 | <code>海中鱼巣/领域/初始化.世界树.ixx:56</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0291 | F0635 | R0277 | <code>海中鱼巣/领域/初始化.世界树.ixx:57</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0292 | F0635 | R0225 | <code>海中鱼巣/领域/初始化.世界树.ixx:58, 海中鱼巣/领域/初始化.世界树.ixx:69, 海中鱼巣/领域/初始化.世界树.ixx:81</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0293 | F0635 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:65, 海中鱼巣/领域/初始化.世界树.ixx:66, 海中鱼巣/领域/初始化.世界树.ixx:67</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0294 | F0635 | R0223 | <code>海中鱼巣/领域/初始化.世界树.ixx:68</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0295 | F0635 | F0059 | <code>海中鱼巣/领域/初始化.世界树.ixx:73</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0296 | F0636 | F0184 | <code>海中鱼巣/领域/初始化.需求.ixx:104</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0297 | F0636 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:105</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0298 | F0636 | F0328 | <code>海中鱼巣/领域/初始化.需求.ixx:107</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0299 | F0636 | F0134 | <code>海中鱼巣/领域/初始化.需求.ixx:76, 海中鱼巣/领域/初始化.需求.ixx:105</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0300 | F0636 | R0249 | <code>海中鱼巣/领域/初始化.需求.ixx:81, 海中鱼巣/领域/初始化.需求.ixx:92</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0301 | R0001 | F0069 | <code>海中鱼巣/启动.生产运行期.ixx:126</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 生产运行期会话读取当前宿主持有的运行期上下文租约 | 专项源码静态类型与实际装配人工复核 |
| RCE0302 | R0002 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:12</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0303 | R0003 | R0013 | <code>海中鱼巣/核心/主信息仓库.cpp:111, 海中鱼巣/核心/主信息仓库.cpp:132</code> | suffix_match+direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；候选处于未发布阶段时执行确认；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0304 | R0003 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:114</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结构化确认先读取候选主信息 | 专项源码静态类型与实际装配人工复核 |
| RCE0305 | R0003 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:116</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化确认入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0306 | R0003 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:120</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0307 | R0004 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:144</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 撤销候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0308 | R0004 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:146</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0309 | R0005 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:16</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0310 | R0006 | R0004 | <code>海中鱼巣/核心/主信息仓库.cpp:169, 海中鱼巣/核心/主信息仓库.cpp:186</code> | suffix_match+direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；候选状态允许撤销；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0311 | R0006 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:172</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 形成撤销结果前读取候选句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0312 | R0006 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:174</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化撤销入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0313 | R0006 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:178</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0314 | R0006 | R0138 | <code>海中鱼巣/核心/主信息仓库.cpp:194</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 撤销结果成功后归零当前版本 | 专项源码静态类型与实际装配人工复核 |
| RCE0315 | R0008 | R0004 | <code>海中鱼巣/核心/主信息仓库.cpp:313</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 令牌删除重载委托撤销候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0316 | R0008 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:313</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 删除主信息令牌重载进入后验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0317 | R0009 | F0439 | <code>海中鱼巣/核心/主信息仓库.cpp:372</code> | suffix_match+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0318 | R0010 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:409</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 令牌写入 I64 入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0319 | R0011 | R0012 | <code>海中鱼巣/核心/主信息仓库.cpp:62, 海中鱼巣/核心/主信息仓库.cpp:69</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0320 | R0011 | R0005 | <code>海中鱼巣/核心/主信息仓库.cpp:65</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化创建主信息未发布候选进入后验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0321 | R0011 | R0014 | <code>海中鱼巣/核心/主信息仓库.cpp:74</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结构化候选创建后读取主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0322 | R0012 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:82</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 创建主信息未发布候选进入后验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0323 | R0013 | R0002 | <code>海中鱼巣/核心/主信息仓库.cpp:93</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 确认候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0324 | R0013 | R0007 | <code>海中鱼巣/核心/主信息仓库.cpp:95</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0325 | R0015 | F0168 | <code>海中鱼巣/核心/主信息仓库.h:44</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0326 | R0016 | R0047 | <code>海中鱼巣/核心/会话.结构写入.ixx:1033</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 只读视图已绑定会话时复核主信息候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0327 | R0017 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:1037</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 只读视图已绑定会话时复核节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0328 | R0018 | R0049 | <code>海中鱼巣/核心/会话.结构写入.ixx:1041</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 只读视图已绑定会话时读取候选节点类型 | 专项源码静态类型与实际装配人工复核 |
| RCE0329 | R0019 | R0050 | <code>海中鱼巣/核心/会话.结构写入.ixx:1045</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 只读视图已绑定会话时读取候选节点主信息 | 专项源码静态类型与实际装配人工复核 |
| RCE0330 | R0020 | R0051 | <code>海中鱼巣/核心/会话.结构写入.ixx:1051</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 只读视图已绑定会话时读取候选 I64 槽位 | 专项源码静态类型与实际装配人工复核 |
| RCE0331 | R0021 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:236</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0332 | R0021 | R0011 | <code>海中鱼巣/核心/会话.结构写入.ixx:237</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0333 | R0021 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:240, 海中鱼巣/核心/会话.结构写入.ixx:245, 海中鱼巣/核心/会话.结构写入.ixx:255</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0334 | R0021 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:249</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 创建未发布主信息成功后读取候选主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0335 | R0022 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:264</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0336 | R0022 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:268, 海中鱼巣/核心/会话.结构写入.ixx:273, 海中鱼巣/核心/会话.结构写入.ixx:283</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0337 | R0023 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:296</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0338 | R0023 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:298, 海中鱼巣/核心/会话.结构写入.ixx:304, 海中鱼巣/核心/会话.结构写入.ixx:313</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0339 | R0023 | R0066 | <code>海中鱼巣/核心/会话.结构写入.ixx:301</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0340 | R0023 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:311</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0341 | R0024 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:379</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0342 | R0024 | R0068 | <code>海中鱼巣/核心/会话.结构写入.ixx:380</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0343 | R0024 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:383, 海中鱼巣/核心/会话.结构写入.ixx:394, 海中鱼巣/核心/会话.结构写入.ixx:408</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0344 | R0024 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:392</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0345 | R0024 | R0054 | <code>海中鱼巣/核心/会话.结构写入.ixx:404</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0346 | R0025 | F0168 | <code>海中鱼巣/核心/会话.结构写入.ixx:417</code> | unique_name+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；关系审计入口复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0347 | R0025 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:417</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0348 | R0025 | R0073 | <code>海中鱼巣/核心/会话.结构写入.ixx:418</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 会话审计读取令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0349 | R0025 | R0053 | <code>海中鱼巣/核心/会话.结构写入.ixx:419</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0350 | R0026 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:426</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 关系审计记录组入口复核源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0351 | R0026 | R0074 | <code>海中鱼巣/核心/会话.结构写入.ixx:427</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0352 | R0026 | R0053 | <code>海中鱼巣/核心/会话.结构写入.ixx:428</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0353 | R0027 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:452</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0354 | R0027 | R0136 | <code>海中鱼巣/核心/会话.结构写入.ixx:452</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 绑定主键入口复核请求显式完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0355 | R0027 | R0119 | <code>海中鱼巣/核心/会话.结构写入.ixx:453</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0356 | R0027 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:455, 海中鱼巣/核心/会话.结构写入.ixx:464</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0357 | R0027 | R0125 | <code>海中鱼巣/核心/会话.结构写入.ixx:462</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0358 | R0028 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:474</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 候选 I64 写入入口复核主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0359 | R0028 | R0015 | <code>海中鱼巣/核心/会话.结构写入.ixx:478</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 遍历本会话主信息候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0360 | R0028 | R0010 | <code>海中鱼巣/核心/会话.结构写入.ixx:491</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 确认候选属于本会话后写入 I64 | 专项源码静态类型与实际装配人工复核 |
| RCE0361 | R0028 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:493, 海中鱼巣/核心/会话.结构写入.ixx:500</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0362 | R0029 | F0565 | <code>海中鱼巣/核心/会话.结构写入.ixx:510</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 候选 I64 读回入口复核主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0363 | R0029 | F0619 | <code>海中鱼巣/核心/会话.结构写入.ixx:514</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 候选写集命中后令牌读取 I64 | 专项源码静态类型与实际装配人工复核 |
| RCE0364 | R0029 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:515</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0365 | R0030 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:525</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 读取节点类型入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0366 | R0031 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:531</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 读取节点主信息入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0367 | R0032 | F0168 | <code>海中鱼巣/核心/会话.结构写入.ixx:537</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0368 | R0032 | R0044 | <code>海中鱼巣/核心/会话.结构写入.ixx:537</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0369 | R0033 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:542</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 读取节点主键组入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0370 | R0034 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:547</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 读取关系记录组入口复核源节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0371 | R0034 | R0078 | <code>海中鱼巣/核心/会话.结构写入.ixx:548</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0372 | R0035 | F0163 | <code>海中鱼巣/核心/会话.结构写入.ixx:554</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 读取来源关系记录组入口复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0373 | R0035 | R0079 | <code>海中鱼巣/核心/会话.结构写入.ixx:555</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0374 | R0036 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:567</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0375 | R0036 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:570</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0376 | R0036 | R0014 | <code>海中鱼巣/核心/会话.结构写入.ixx:573</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐候选读取主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0377 | R0037 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:578</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0378 | R0037 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:581</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0379 | R0038 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:589</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0380 | R0038 | F0440 | <code>海中鱼巣/核心/会话.结构写入.ixx:590</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 当前线程可访问后令牌读取关系 | 专项源码静态类型与实际装配人工复核 |
| RCE0381 | R0038 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:592</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0382 | R0038 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:597</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐变更能力读取写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0383 | R0040 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:633</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0384 | R0040 | R0136 | <code>海中鱼巣/核心/会话.结构写入.ixx:633</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 登记索引绑定入口首先验证请求显式完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0385 | R0040 | R0124 | <code>海中鱼巣/核心/会话.结构写入.ixx:634</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0386 | R0040 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:638</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0387 | R0041 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:648</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0388 | R0041 | R0055 | <code>海中鱼巣/核心/会话.结构写入.ixx:648</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0389 | R0044 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:733</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0390 | R0045 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:738</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0391 | R0046 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:743</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0392 | R0047 | F0168 | <code>海中鱼巣/核心/会话.结构写入.ixx:750</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0393 | R0047 | R0045 | <code>海中鱼巣/核心/会话.结构写入.ixx:750</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0394 | R0048 | F0168 | <code>海中鱼巣/核心/会话.结构写入.ixx:758</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0395 | R0048 | R0045 | <code>海中鱼巣/核心/会话.结构写入.ixx:758</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0396 | R0049 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:766</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0397 | R0050 | R0048 | <code>海中鱼巣/核心/会话.结构写入.ixx:772</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0398 | R0051 | R0047 | <code>海中鱼巣/核心/会话.结构写入.ixx:780</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0399 | R0053 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:807</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 确认阶段读取能力写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0400 | R0054 | R0094 | <code>海中鱼巣/核心/会话.结构写入.ixx:814</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 登记关系变更能力读取写前记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0401 | R0054 | R0095 | <code>海中鱼巣/核心/会话.结构写入.ixx:815</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 登记关系变更能力读取写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0402 | R0054 | R0072 | <code>海中鱼巣/核心/会话.结构写入.ixx:820</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0403 | R0054 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:821</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0404 | R0060 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；完成确认入口按短路顺序复核当前线程 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0405 | R0060 | R0056 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | suffix_match+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；线程可访问后复核提交决定 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0406 | R0060 | R0058 | <code>海中鱼巣/核心/会话.结构写入.ixx:846</code> | suffix_match+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；提交决定成立后复核无失败 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0407 | R0060 | R0142 | <code>海中鱼巣/核心/会话.结构写入.ixx:851</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个节点候选结构化确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0408 | R0060 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:852, 海中鱼巣/核心/会话.结构写入.ixx:859, 海中鱼巣/核心/会话.结构写入.ixx:866</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐项确认后复核结构写入结果 | 专项源码静态类型与实际装配人工复核 |
| RCE0409 | R0060 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:853, 海中鱼巣/核心/会话.结构写入.ixx:860, 海中鱼巣/核心/会话.结构写入.ixx:867</code> | suffix_match+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；任一确认失败时记录首次失败 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0410 | R0060 | R0003 | <code>海中鱼巣/核心/会话.结构写入.ixx:858</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个主信息候选结构化确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0411 | R0060 | R0071 | <code>海中鱼巣/核心/会话.结构写入.ixx:865</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；逐个关系变更结构化确认 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0412 | R0062 | R0043 | <code>海中鱼巣/核心/会话.结构写入.ixx:931</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；完成撤销入口复核当前线程 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0413 | R0062 | R0072 | <code>海中鱼巣/核心/会话.结构写入.ixx:938</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；逆序撤销关系变更 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0414 | R0062 | R0138 | <code>海中鱼巣/核心/会话.结构写入.ixx:938, 海中鱼巣/核心/会话.结构写入.ixx:942, 海中鱼巣/核心/会话.结构写入.ixx:945, 海中鱼巣/核心/会话.结构写入.ixx:948, 海中鱼巣/核心/会话.结构写入.ixx:951</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 每项撤销或严格删除后复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE0415 | R0062 | R0125 | <code>海中鱼巣/核心/会话.结构写入.ixx:941, 海中鱼巣/核心/会话.结构写入.ixx:942</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；逆序严格删除索引绑定 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0416 | R0062 | R0076 | <code>海中鱼巣/核心/会话.结构写入.ixx:945</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；逆序严格删除新关系 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0417 | R0062 | R0144 | <code>海中鱼巣/核心/会话.结构写入.ixx:948</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逆序撤销节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0418 | R0062 | R0006 | <code>海中鱼巣/核心/会话.结构写入.ixx:951</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逆序撤销主信息候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0419 | R0063 | R0046 | <code>海中鱼巣/核心/会话.结构写入.ixx:101</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0420 | R0063 | R0124 | <code>海中鱼巣/核心/会话.结构写入.ixx:102</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0421 | R0063 | R0052 | <code>海中鱼巣/核心/会话.结构写入.ixx:113</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0422 | R0064 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:105, 海中鱼巣/核心/关系仓库.cpp:106</code> | operator | <code>未单独冻结；读取源码调用点</code> | 逐字段比较源/目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0424 | R0065 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:1032</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 共享令牌有效后建立令牌范围 | 专项源码静态类型与实际装配人工复核 |
| RCE0423 | R0065 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1032</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 令牌薄包装入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0426 | R0065 | F0167 | <code>海中鱼巣/核心/关系仓库.cpp:1033</code> | direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 令牌范围建立后调用无令牌重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0427 | R0066 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1042</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化创建关系首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0428 | R0066 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:1046</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 独占令牌有效后复核关系类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0429 | R0066 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:1047, 海中鱼巣/核心/关系仓库.cpp:1048</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化创建复核源/目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0430 | R0066 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1049, 海中鱼巣/核心/关系仓库.cpp:1050</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 句柄前置通过后复核两个节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0431 | R0066 | R0065 | <code>海中鱼巣/核心/关系仓库.cpp:1054</code> | direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 入口材料通过后调用令牌创建重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0432 | R0066 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:1055</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 创建返回后复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0433 | R0068 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1227</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化挂载或重挂首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0434 | R0068 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1231, 海中鱼巣/核心/关系仓库.cpp:1232, 海中鱼巣/核心/关系仓库.cpp:1249, 海中鱼巣/核心/关系仓库.cpp:1254</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 挂载写前端点与父链逐节点复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0435 | R0068 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:1235, 海中鱼巣/核心/关系仓库.cpp:1274, 海中鱼巣/核心/关系仓库.cpp:1283, 海中鱼巣/核心/关系仓库.cpp:1284, 海中鱼巣/核心/关系仓库.cpp:1329, 海中鱼巣/核心/关系仓库.cpp:1331</code> | operator | <code>未单独冻结；读取源码调用点</code> | 节点/父节点/关系记录端点比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0436 | R0068 | R0086 | <code>海中鱼巣/核心/关系仓库.cpp:1245, 海中鱼巣/核心/关系仓库.cpp:1268</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0437 | R0068 | R0087 | <code>海中鱼巣/核心/关系仓库.cpp:1246, 海中鱼巣/核心/关系仓库.cpp:1269</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0438 | R0068 | R0069 | <code>海中鱼巣/核心/关系仓库.cpp:1250</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::all_of 注册父链端点验证回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0439 | R0068 | F0184 | <code>海中鱼巣/核心/关系仓库.cpp:1263, 海中鱼巣/核心/关系仓库.cpp:1360</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0440 | R0068 | R0067 | <code>海中鱼巣/核心/关系仓库.cpp:1288, 海中鱼巣/核心/关系仓库.cpp:1318, 海中鱼巣/核心/关系仓库.cpp:1347</code> | source-audited+direct-free+direct-free+direct-free | <code>未单独冻结；读取源码调用点</code> | 三个发布分支形成关系句柄；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0441 | R0068 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1314</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 重挂分支比较写前写后关系记录；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0442 | R0068 | R0096 | <code>海中鱼巣/核心/关系仓库.cpp:1340, 海中鱼巣/核心/关系仓库.cpp:1341</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 重挂分支形成变更能力 | 专项源码静态类型与实际装配人工复核 |
| RCE0443 | R0068 | R0080 | <code>海中鱼巣/核心/关系仓库.cpp:1342</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0444 | R0068 | R0093 | <code>海中鱼巣/核心/关系仓库.cpp:1355</code> | move-constructor | <code>未单独冻结；读取源码调用点</code> | optional emplace 移入能力 | 专项源码静态类型与实际装配人工复核 |
| RCE0445 | R0069 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1254</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个父链节点复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0446 | R0070 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:136</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 已发布关系变更能力复核写前关系类型；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0447 | R0070 | F0163 | <code>海中鱼巣/核心/关系仓库.cpp:138, 海中鱼巣/核心/关系仓库.cpp:139, 海中鱼巣/核心/关系仓库.cpp:140, 海中鱼巣/核心/关系仓库.cpp:141</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 能力完整性复核写前/写后源目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0448 | R0070 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:151, 海中鱼巣/核心/关系仓库.cpp:152, 海中鱼巣/核心/关系仓库.cpp:157, 海中鱼巣/核心/关系仓库.cpp:158</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 能力完整性复核写前/写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0449 | R0071 | R0095 | <code>海中鱼巣/核心/关系仓库.cpp:1368</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结构化确认先读取能力写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0450 | R0071 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1374</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化确认入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0451 | R0071 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:1375</code> | direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 复核关系变更能力完整；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0452 | R0071 | R0092 | <code>海中鱼巣/核心/关系仓库.cpp:1377</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 复核能力令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0453 | R0071 | R0080 | <code>海中鱼巣/核心/关系仓库.cpp:1381</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0454 | R0071 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1385</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 锁内复核写后关系记录；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0455 | R0072 | R0094 | <code>海中鱼巣/核心/关系仓库.cpp:1397</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 撤销已发布关系变更先读取写前记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0456 | R0072 | R0095 | <code>海中鱼巣/核心/关系仓库.cpp:1398</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 读取写前记录后读取写后记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0457 | R0072 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1404</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 撤销前验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0458 | R0072 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:1405</code> | direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 撤销前复核能力完整；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0459 | R0072 | R0092 | <code>海中鱼巣/核心/关系仓库.cpp:1407</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 能力完整后复核令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0460 | R0072 | R0084 | <code>海中鱼巣/核心/关系仓库.cpp:1411</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0461 | R0072 | R0064 | <code>海中鱼巣/核心/关系仓库.cpp:1415, 海中鱼巣/核心/关系仓库.cpp:1420</code> | source-audited+direct-free+direct-free | <code>未单独冻结；读取源码调用点</code> | 撤销前后两处关系记录一致性复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0462 | R0073 | R0089 | <code>海中鱼巣/核心/关系仓库.cpp:1435</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 令牌读取入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0464 | R0073 | F0589 | <code>海中鱼巣/核心/关系仓库.cpp:1436</code> | direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 令牌有效后调用无令牌审计重载；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0467 | R0074 | F0582 | <code>海中鱼巣/核心/关系仓库.cpp:1440</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0470 | R0075 | F0590 | <code>海中鱼巣/核心/关系仓库.cpp:1448</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0471 | R0076 | R0091 | <code>海中鱼巣/核心/关系仓库.cpp:1454</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 严格删除关系首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0472 | R0076 | F0168 | <code>海中鱼巣/核心/关系仓库.cpp:1458, 海中鱼巣/核心/关系仓库.cpp:1495, 海中鱼巣/核心/关系仓库.cpp:1496</code> | unique_name+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；严格删除入口和锁内端点比较 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0473 | R0076 | F0630 | <code>海中鱼巣/核心/关系仓库.cpp:1482, 海中鱼巣/核心/关系仓库.cpp:1483</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 严格删除前复核关系端点 | 专项源码静态类型与实际装配人工复核 |
| RCE0476 | R0077 | F0620 | <code>海中鱼巣/核心/关系仓库.cpp:1544</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0479 | R0078 | F0575 | <code>海中鱼巣/核心/关系仓库.cpp:1548</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0482 | R0079 | F0578 | <code>海中鱼巣/核心/关系仓库.cpp:1560</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0483 | R0080 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:162</code> | suffix_match+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0486 | R0081 | F0579 | <code>海中鱼巣/核心/关系仓库.cpp:1641</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0489 | R0082 | F0580 | <code>海中鱼巣/核心/关系仓库.cpp:1645</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0492 | R0083 | F0198 | <code>海中鱼巣/核心/关系仓库.cpp:1649</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0493 | R0084 | R0070 | <code>海中鱼巣/核心/关系仓库.cpp:166</code> | suffix_match+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0494 | R0086 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:689</code> | operator | <code>未单独冻结；读取源码调用点</code> | 普通父关系复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0495 | R0087 | R0086 | <code>海中鱼巣/核心/关系仓库.cpp:710</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0496 | R0087 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:721, 海中鱼巣/核心/关系仓库.cpp:724</code> | operator | <code>未单独冻结；读取源码调用点</code> | 父链目标比较和循环检测 | 专项源码静态类型与实际装配人工复核 |
| RCE0505 | R0088 | F0341 | <code>海中鱼巣/核心/关系仓库.cpp:873</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 目标节点有效；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0506 | R0088 | F0343 | <code>海中鱼巣/核心/关系仓库.cpp:873, 海中鱼巣/核心/关系仓库.cpp:880</code> | direct-const-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 入口；目标节点；读取有效来源节点；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0507 | R0088 | F0051 | <code>海中鱼巣/核心/关系仓库.cpp:879</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 记录目标等于目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0508 | R0088 | R0090 | <code>海中鱼巣/核心/关系仓库.cpp:884</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册的唯一比较回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0509 | R0089 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:88</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；短路复核接线已接域 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0510 | R0089 | F0399 | <code>海中鱼巣/核心/关系仓库.cpp:88</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 正式生产接线唯一共享令牌验证目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0511 | R0091 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:92</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0512 | R0091 | F0400 | <code>海中鱼巣/核心/关系仓库.cpp:92</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 正式生产接线唯一独占令牌验证目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0513 | R0098 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:45</code> | same_module+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0514 | R0099 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:59</code> | same_module+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0515 | R0099 | R0098 | <code>海中鱼巣/核心/协调.结构事务.ixx:60</code> | same_module+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0516 | R0100 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:67</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 结构事务内部直接调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0517 | R0101 | R0137 | <code>海中鱼巣/核心/协调.结构事务.ixx:130</code> | unique_name+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0518 | R0101 | R0097 | <code>海中鱼巣/核心/协调.结构事务.ixx:85</code> | same_module+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；结构事务内部直接调用 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0519 | R0102 | R0104 | <code>海中鱼巣/核心/执行器.结构写入.ixx:116</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0520 | R0102 | R0115 | <code>海中鱼巣/核心/执行器.结构写入.ixx:116</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 无对象限定调用当前结构写入执行器::有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0521 | R0102 | R0118 | <code>海中鱼巣/核心/执行器.结构写入.ixx:119</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0522 | R0102 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:120</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 取得独占许可后复核结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE0523 | R0102 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:122</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1685 | R0102 | R0391 | <code>海中鱼巣/核心/执行器.结构写入.ixx:124</code> | resolved-std-function-callback | <code>未单独冻结；读取源码调用点</code> | 本次执行调用语境由 海中鱼巣/领域/数据操作.特征体系.ixx:832 注册该回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0524 | R0102 | R0109 | <code>海中鱼巣/核心/执行器.结构写入.ixx:126, 海中鱼巣/核心/执行器.结构写入.ixx:135, 海中鱼巣/核心/执行器.结构写入.ixx:142, 海中鱼巣/核心/执行器.结构写入.ixx:155, 海中鱼巣/核心/执行器.结构写入.ixx:164, 海中鱼巣/核心/执行器.结构写入.ixx:173</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0525 | R0102 | R0062 | <code>海中鱼巣/核心/执行器.结构写入.ixx:127, 海中鱼巣/核心/执行器.结构写入.ixx:136, 海中鱼巣/核心/执行器.结构写入.ixx:143, 海中鱼巣/核心/执行器.结构写入.ixx:156, 海中鱼巣/核心/执行器.结构写入.ixx:165, 海中鱼巣/核心/执行器.结构写入.ixx:174</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 六个撤销收口点调用会话完成撤销 | 专项源码静态类型与实际装配人工复核 |
| RCE0526 | R0102 | R0108 | <code>海中鱼巣/核心/执行器.结构写入.ixx:128, 海中鱼巣/核心/执行器.结构写入.ixx:137, 海中鱼巣/核心/执行器.结构写入.ixx:157, 海中鱼巣/核心/执行器.结构写入.ixx:166, 海中鱼巣/核心/执行器.结构写入.ixx:175</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0527 | R0102 | R0058 | <code>海中鱼巣/核心/执行器.结构写入.ixx:133</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话静态类型复核失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0528 | R0102 | R0059 | <code>海中鱼巣/核心/执行器.结构写入.ixx:134</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话静态类型读取首次失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0529 | R0102 | R0057 | <code>海中鱼巣/核心/执行器.结构写入.ixx:141</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话静态类型复核撤销请求 | 专项源码静态类型与实际装配人工复核 |
| RCE0530 | R0102 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:144</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0531 | R0102 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:145</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0532 | R0102 | R0110 | <code>海中鱼巣/核心/执行器.结构写入.ixx:153</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 旧结构写入执行器按结构写入事务参与者静态层级准备提交 | 专项源码静态类型与实际装配人工复核 |
| RCE0533 | R0102 | R0060 | <code>海中鱼巣/核心/执行器.结构写入.ixx:162</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话静态类型完成确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0534 | R0102 | R0112 | <code>海中鱼巣/核心/执行器.结构写入.ixx:171</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 旧结构写入执行器按结构写入事务参与者静态层级确认待发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0535 | R0102 | R0061 | <code>海中鱼巣/核心/执行器.结构写入.ixx:180</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话静态类型完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0536 | R0102 | R0113 | <code>海中鱼巣/核心/执行器.结构写入.ixx:181</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 旧结构写入执行器按结构写入事务参与者静态层级完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0538 | R0103 | R0102 | <code>海中鱼巣/核心/执行器.结构写入.ixx:191</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 单个参与者提升为 span 后调用多参与者重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0539 | R0106 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:367</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 隔离入口复核许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0540 | R0106 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:368</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；许可有效且隔离函数字段存在 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0541 | R0106 | F0401 | <code>海中鱼巣/核心/执行器.结构写入.ixx:368</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 生产运行期唯一撤销失败隔离目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0542 | R0107 | R0106 | <code>海中鱼巣/核心/执行器.结构写入.ixx:372</code> | suffix_match+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；撤销失败进入隔离标记 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0543 | R0108 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:382</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0544 | R0108 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:383</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0545 | R0109 | R0111 | <code>海中鱼巣/核心/执行器.结构写入.ixx:390</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 逆序逐项完成旧结构写入参与者撤销 | 专项源码静态类型与实际装配人工复核 |
| RCE0546 | R0110 | R0039 | <code>海中鱼巣/核心/执行器.结构写入.ixx:402</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 安全准备参与者显式构造只读准备视图 | 专项源码静态类型与实际装配人工复核 |
| RCE0547 | R0110 | R0260 | <code>海中鱼巣/核心/执行器.结构写入.ixx:403</code> | resolved-virtual | <code>未单独冻结；读取源码调用点</code> | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0548 | R0111 | R0263 | <code>海中鱼巣/核心/执行器.结构写入.ixx:413</code> | resolved-virtual | <code>未单独冻结；读取源码调用点</code> | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0549 | R0112 | R0261 | <code>海中鱼巣/核心/执行器.结构写入.ixx:423</code> | resolved-virtual | <code>未单独冻结；读取源码调用点</code> | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0550 | R0113 | R0262 | <code>海中鱼巣/核心/执行器.结构写入.ixx:432</code> | resolved-virtual | <code>未单独冻结；读取源码调用点</code> | 可达特征值原始材料参与者上下文中的虚调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0551 | R0115 | F0336 | <code>海中鱼巣/核心/执行器.结构写入.ixx:70</code> | unique_name+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；执行器有效性首先复核事务接线已接域 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0552 | R0116 | R0061 | <code>海中鱼巣/核心/执行器.结构写入.ixx:103</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 确认成功后完成发布 | 专项源码静态类型与实际装配人工复核 |
| RCE0553 | R0116 | R0057 | <code>海中鱼巣/核心/执行器.结构写入.ixx:107</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 未提交分支读取显式撤销决定 | 专项源码静态类型与实际装配人工复核 |
| RCE0554 | R0116 | R0115 | <code>海中鱼巣/核心/执行器.结构写入.ixx:75</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 执行入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0556 | R0116 | F0398 | <code>海中鱼巣/核心/执行器.结构写入.ixx:76</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 执行器有效；生产运行期唯一正式独占许可目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0557 | R0116 | F0338 | <code>海中鱼巣/核心/执行器.结构写入.ixx:77</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 取得独占许可后复核许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0558 | R0116 | F0339 | <code>海中鱼巣/核心/执行器.结构写入.ixx:80</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 许可有效后读取事务令牌 | 专项源码静态类型与实际装配人工复核 |
| RCE0559 | R0116 | R0042 | <code>海中鱼巣/核心/执行器.结构写入.ixx:80</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 执行器有效且独占许可有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0560 | R0116 | R0341 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback | <code>未单独冻结；读取源码调用点</code> | 本次执行调用语境由 海中鱼巣/领域/数据操作.概念活动.ixx:42 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0561 | R0116 | R0410 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback | <code>未单独冻结；读取源码调用点</code> | 本次执行调用语境由 海中鱼巣/领域/数据操作.系统角色.ixx:124 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0562 | R0116 | R0433 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback | <code>未单独冻结；读取源码调用点</code> | 本次执行调用语境由 海中鱼巣/领域/数据操作.需求任务方法.ixx:1619 注册该唯一回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0563 | R0116 | R0455 | <code>海中鱼巣/核心/执行器.结构写入.ixx:82</code> | resolved-std-function-callback | <code>未单独冻结；读取源码调用点</code> | 本次执行调用语境由 海中鱼巣/领域/数据操作.需求任务方法.ixx:2876 注册该回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0564 | R0116 | R0062 | <code>海中鱼巣/核心/执行器.结构写入.ixx:84, 海中鱼巣/核心/执行器.结构写入.ixx:92, 海中鱼巣/核心/执行器.结构写入.ixx:100, 海中鱼巣/核心/执行器.结构写入.ixx:108</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 异常、失败、确认失败或未提交分支撤销会话 | 专项源码静态类型与实际装配人工复核 |
| RCE0565 | R0116 | R0105 | <code>海中鱼巣/核心/执行器.结构写入.ixx:85, 海中鱼巣/核心/执行器.结构写入.ixx:93, 海中鱼巣/核心/执行器.结构写入.ixx:101, 海中鱼巣/核心/执行器.结构写入.ixx:109</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 撤销后复核撤销结果 | 专项源码静态类型与实际装配人工复核 |
| RCE0566 | R0116 | R0107 | <code>海中鱼巣/核心/执行器.结构写入.ixx:87, 海中鱼巣/核心/执行器.结构写入.ixx:95, 海中鱼巣/核心/执行器.结构写入.ixx:101, 海中鱼巣/核心/执行器.结构写入.ixx:109</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 撤销不完整时隔离并返回内部不一致 | 专项源码静态类型与实际装配人工复核 |
| RCE0567 | R0116 | R0058 | <code>海中鱼巣/核心/执行器.结构写入.ixx:90</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 回调正常返回后复核会话失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0568 | R0116 | R0059 | <code>海中鱼巣/核心/执行器.结构写入.ixx:91</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 会话已有失败时读取首次失败 | 专项源码静态类型与实际装配人工复核 |
| RCE0569 | R0116 | R0056 | <code>海中鱼巣/核心/执行器.结构写入.ixx:97</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 无失败时复核是否请求提交 | 专项源码静态类型与实际装配人工复核 |
| RCE0570 | R0116 | R0060 | <code>海中鱼巣/核心/执行器.结构写入.ixx:98</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 已请求提交时完成确认 | 专项源码静态类型与实际装配人工复核 |
| RCE0571 | R0116 | R0138 | <code>海中鱼巣/核心/执行器.结构写入.ixx:99</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 确认返回后复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE0572 | R0118 | R0117 | <code>海中鱼巣/核心/执行器.节点直接身份结构写入.ixx:114</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0573 | R0119 | R0121 | <code>海中鱼巣/核心/索引仓库.cpp:132</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化绑定主键首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0574 | R0119 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:136</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结构化绑定主键复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0575 | R0119 | R0128 | <code>海中鱼巣/核心/索引仓库.cpp:136</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0576 | R0119 | R0122 | <code>海中鱼巣/核心/索引仓库.cpp:162</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 校验及加锁完成后绑定主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0577 | R0120 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:19</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0578 | R0120 | F0399 | <code>海中鱼巣/核心/索引仓库.cpp:19</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 运行期上下文把结构事务协调器::生成接线的共享验证 lambda 按值装入索引仓库接线 | 专项源码静态类型与实际装配人工复核 |
| RCE0579 | R0121 | F0336 | <code>海中鱼巣/核心/索引仓库.cpp:23</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0580 | R0121 | F0400 | <code>海中鱼巣/核心/索引仓库.cpp:23</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 运行期上下文把结构事务协调器::生成接线的独占验证 lambda 按值装入索引仓库接线 | 专项源码静态类型与实际装配人工复核 |
| RCE0581 | R0122 | R0127 | <code>海中鱼巣/核心/索引仓库.cpp:34, 海中鱼巣/核心/索引仓库.cpp:52</code> | operator | <code>未单独冻结；读取源码调用点</code> | 比较现有/插入主键绑定记录 | 专项源码静态类型与实际装配人工复核 |
| RCE0582 | R0123 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:299</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 共享令牌删除重载复核通过 | 专项源码静态类型与实际装配人工复核 |
| RCE0583 | R0123 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:299</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 删除主键令牌重载进入后验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0584 | R0123 | R0126 | <code>海中鱼巣/核心/索引仓库.cpp:301</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 共享令牌有效并取得锁后删除主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0585 | R0124 | R0120 | <code>海中鱼巣/核心/索引仓库.cpp:307</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 读取主键令牌重载进入后验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0586 | R0125 | R0121 | <code>海中鱼巣/核心/索引仓库.cpp:362</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 严格删除主键首先验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0587 | R0125 | F0163 | <code>海中鱼巣/核心/索引仓库.cpp:366</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 严格删除主键入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0588 | R0125 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:367</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 严格删除主键令牌复核目标节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0589 | R0125 | F0051 | <code>海中鱼巣/核心/索引仓库.cpp:376</code> | operator | <code>未单独冻结；读取源码调用点</code> | 索引绑定节点与预期节点比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0590 | R0125 | R0126 | <code>海中鱼巣/核心/索引仓库.cpp:393</code> | source-audited+direct-free | <code>未单独冻结；读取源码调用点</code> | 正反索引一致且加锁后删除主键；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0591 | R0127 | F0051 | <code>海中鱼巣/核心/索引仓库.h:52</code> | operator | <code>未单独冻结；读取源码调用点</code> | 比较绑定节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0592 | R0127 | R0132 | <code>海中鱼巣/核心/索引仓库.h:53</code> | operator | <code>未单独冻结；读取源码调用点</code> | 比较所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0593 | R0128 | F0163 | <code>海中鱼巣/核心/索引所有权.数据.h:103</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 兼容绑定请求复核目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0594 | R0128 | R0131 | <code>海中鱼巣/核心/索引所有权.数据.h:104</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0595 | R0128 | R0135 | <code>海中鱼巣/核心/索引所有权.数据.h:104</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 兼容完整性复核所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0596 | R0129 | R0134 | <code>海中鱼巣/核心/索引所有权.数据.h:119</code> | lsp_direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0597 | R0134 | R0133 | <code>海中鱼巣/核心/索引所有权.数据.h:83</code> | lsp_direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0598 | R0135 | R0130 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 所有者声明符合规范首先复核显式完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0599 | R0135 | R0132 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 显式完整后比较声明与规范声明 | 专项源码静态类型与实际装配人工复核 |
| RCE0600 | R0135 | R0134 | <code>海中鱼巣/核心/索引所有权.数据.h:88</code> | lsp_direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | LSP 直接解析并经源码调用词复核 |
| RCE0601 | R0136 | F0163 | <code>海中鱼巣/核心/索引所有权.数据.h:98</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 绑定请求复核目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0602 | R0136 | R0135 | <code>海中鱼巣/核心/索引所有权.数据.h:98</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 物理键和目标句柄有效后复核所有者声明 | 专项源码静态类型与实际装配人工复核 |
| RCE1686 | R0137 | R0599 | <code>海中鱼巣/核心/结构事务接线.数据.h:47</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 创建工厂先默认构造结果对象 | 专项源码静态类型与实际装配人工复核 |
| RCE0603 | R0141 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:128</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 确认候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0604 | R0141 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:130</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0605 | R0142 | R0141 | <code>海中鱼巣/核心/节点仓库.cpp:146, 海中鱼巣/核心/节点仓库.cpp:167</code> | suffix_match+direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；候选处于未发布阶段时执行确认；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0606 | R0142 | R0148 | <code>海中鱼巣/核心/节点仓库.cpp:149</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结构化确认先读取候选节点 | 专项源码静态类型与实际装配人工复核 |
| RCE0607 | R0142 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:151</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化确认入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0608 | R0142 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:155</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0609 | R0143 | F0632 | <code>海中鱼巣/核心/节点仓库.cpp:179</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 撤销候选入口验证共享令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0610 | R0143 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:181</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0611 | R0144 | R0143 | <code>海中鱼巣/核心/节点仓库.cpp:204, 海中鱼巣/核心/节点仓库.cpp:221</code> | suffix_match+direct-const-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；候选状态允许撤销；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0612 | R0144 | R0148 | <code>海中鱼巣/核心/节点仓库.cpp:207</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 形成撤销结果前读取候选句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0613 | R0144 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:209</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 结构化撤销入口验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0614 | R0144 | R0146 | <code>海中鱼巣/核心/节点仓库.cpp:213</code> | direct-const-member+direct-free | <code>未单独冻结；读取源码调用点</code> | 复核候选令牌一致；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0615 | R0144 | R0138 | <code>海中鱼巣/核心/节点仓库.cpp:229</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 撤销结果成功后归零当前版本 | 专项源码静态类型与实际装配人工复核 |
| RCE0616 | R0145 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:28</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0617 | R0147 | R0143 | <code>海中鱼巣/核心/节点仓库.cpp:351</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 令牌删除重载委托撤销候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0618 | R0147 | R0145 | <code>海中鱼巣/核心/节点仓库.cpp:351</code> | direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 删除节点令牌重载进入后验证独占令牌；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0619 | R0150 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:105</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0620 | R0151 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1017, 海中鱼巣/界面/控制面板窗口.cpp:1086, 海中鱼巣/界面/控制面板窗口.cpp:1121</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0621 | R0151 | R0183 | <code>海中鱼巣/界面/控制面板窗口.cpp:1097</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0622 | R0151 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1100</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0623 | R0151 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1100</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0624 | R0151 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1103</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0625 | R0151 | R0177 | <code>海中鱼巣/界面/控制面板窗口.cpp:1126</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0626 | R0151 | R0152 | <code>海中鱼巣/界面/控制面板窗口.cpp:1127</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0627 | R0151 | R0155 | <code>海中鱼巣/界面/控制面板窗口.cpp:1128</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0628 | R0151 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:1129</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0629 | R0151 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:1130</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0630 | R0151 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1131</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0631 | R0151 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1132</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0632 | R0153 | R0186 | <code>海中鱼巣/界面/控制面板窗口.cpp:1146</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0633 | R0153 | R0154 | <code>海中鱼巣/界面/控制面板窗口.cpp:1155, 海中鱼巣/界面/控制面板窗口.cpp:1188</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0634 | R0153 | R0327 | <code>海中鱼巣/界面/控制面板窗口.cpp:1167</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0635 | R0156 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1251</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0636 | R0156 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:1323, 海中鱼巣/界面/控制面板窗口.cpp:1331</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0637 | R0156 | R0157 | <code>海中鱼巣/界面/控制面板窗口.cpp:1338</code> | direct-local-lambda | <code>未单独冻结；读取源码调用点</code> | 当前分页为系统信息且当前系统信息导航为数据库审计 | 专项源码静态类型与实际装配人工复核 |
| RCE0638 | R0158 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:1362</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0639 | R0158 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1367, 海中鱼巣/界面/控制面板窗口.cpp:1370, 海中鱼巣/界面/控制面板窗口.cpp:1377</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0640 | R0158 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1370, 海中鱼巣/界面/控制面板窗口.cpp:1377</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0641 | R0158 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1378</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0642 | R0158 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1382</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0643 | R0158 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1383</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0644 | R0158 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:1384</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0645 | R0158 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1385</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0646 | R0158 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1386</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0647 | R0160 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1393</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0648 | R0160 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1400</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0649 | R0161 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1411</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0650 | R0163 | R0158 | <code>海中鱼巣/界面/控制面板窗口.cpp:1424</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0651 | R0164 | F0388 | <code>海中鱼巣/界面/控制面板窗口.cpp:1434</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0652 | R0164 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:1440</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0653 | R0164 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:1451, 海中鱼巣/界面/控制面板窗口.cpp:1458</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0654 | R0164 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:1451, 海中鱼巣/界面/控制面板窗口.cpp:1458</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0655 | R0164 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1459</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0656 | R0164 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:1463</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0657 | R0164 | R0177 | <code>海中鱼巣/界面/控制面板窗口.cpp:1464</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0658 | R0164 | R0152 | <code>海中鱼巣/界面/控制面板窗口.cpp:1465</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0659 | R0164 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:1466</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0660 | R0164 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:1467</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0661 | R0164 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:1468</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0662 | R0164 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1469</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0663 | R0165 | R0202 | <code>海中鱼巣/界面/控制面板窗口.cpp:1476</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0664 | R0165 | R0308 | <code>海中鱼巣/界面/控制面板窗口.cpp:1476</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0665 | R0165 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1480</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0666 | R0165 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1484</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0667 | R0166 | R0199 | <code>海中鱼巣/界面/控制面板窗口.cpp:1492, 海中鱼巣/界面/控制面板窗口.cpp:1494, 海中鱼巣/界面/控制面板窗口.cpp:1496</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0668 | R0166 | R0200 | <code>海中鱼巣/界面/控制面板窗口.cpp:1498, 海中鱼巣/界面/控制面板窗口.cpp:1500</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0669 | R0166 | R0164 | <code>海中鱼巣/界面/控制面板窗口.cpp:1502</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0670 | R0166 | R0163 | <code>海中鱼巣/界面/控制面板窗口.cpp:1504</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0671 | R0166 | R0161 | <code>海中鱼巣/界面/控制面板窗口.cpp:1506</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0672 | R0166 | R0165 | <code>海中鱼巣/界面/控制面板窗口.cpp:1508</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0673 | R0170 | F0268 | <code>海中鱼巣/界面/控制面板窗口.cpp:173</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0674 | R0171 | F0268 | <code>海中鱼巣/界面/控制面板窗口.cpp:193</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0675 | R0171 | F0270 | <code>海中鱼巣/界面/控制面板窗口.cpp:194</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0676 | R0171 | R0170 | <code>海中鱼巣/界面/控制面板窗口.cpp:198</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | LSP 直接解析并经源码调用词复核；专项源码静态类型与实际装配人工复核 |
| RCE0677 | R0172 | R0172 | <code>海中鱼巣/界面/控制面板窗口.cpp:205</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0678 | R0173 | R0172 | <code>海中鱼巣/界面/控制面板窗口.cpp:213</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0679 | R0174 | F0168 | <code>海中鱼巣/界面/控制面板窗口.cpp:219</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0680 | R0174 | R0174 | <code>海中鱼巣/界面/控制面板窗口.cpp:229</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0681 | R0175 | R0173 | <code>海中鱼巣/界面/控制面板窗口.cpp:240</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0682 | R0175 | R0174 | <code>海中鱼巣/界面/控制面板窗口.cpp:246</code> | lsp_direct+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0683 | R0176 | R0307 | <code>海中鱼巣/界面/控制面板窗口.cpp:322</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0684 | R0176 | R0150 | <code>海中鱼巣/界面/控制面板窗口.cpp:323</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0685 | R0176 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:328, 海中鱼巣/界面/控制面板窗口.cpp:372</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0686 | R0176 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:332</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0687 | R0176 | R0175 | <code>海中鱼巣/界面/控制面板窗口.cpp:342</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0688 | R0176 | R0303 | <code>海中鱼巣/界面/控制面板窗口.cpp:350</code> | field_type_hint | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0689 | R0177 | F0056 | <code>海中鱼巣/界面/控制面板窗口.cpp:419</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 控制面板读取最近五十条结构统计审计材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0690 | R0183 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:561, 海中鱼巣/界面/控制面板窗口.cpp:568, 海中鱼巣/界面/控制面板窗口.cpp:574</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0691 | R0183 | R0178 | <code>海中鱼巣/界面/控制面板窗口.cpp:565</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0692 | R0183 | R0180 | <code>海中鱼巣/界面/控制面板窗口.cpp:567</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0693 | R0183 | R0179 | <code>海中鱼巣/界面/控制面板窗口.cpp:572</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0694 | R0186 | R0185 | <code>海中鱼巣/界面/控制面板窗口.cpp:598</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0695 | R0187 | R0184 | <code>海中鱼巣/界面/控制面板窗口.cpp:607</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0696 | R0187 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:610</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0697 | R0187 | R0187 | <code>海中鱼巣/界面/控制面板窗口.cpp:614</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0698 | R0188 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:624, 海中鱼巣/界面/控制面板窗口.cpp:635</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0699 | R0188 | R0184 | <code>海中鱼巣/界面/控制面板窗口.cpp:632</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0700 | R0189 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:643, 海中鱼巣/界面/控制面板窗口.cpp:666, 海中鱼巣/界面/控制面板窗口.cpp:676, 海中鱼巣/界面/控制面板窗口.cpp:685</code> | unique_name+direct-member+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0701 | R0189 | R0188 | <code>海中鱼巣/界面/控制面板窗口.cpp:659</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0702 | R0189 | R0190 | <code>海中鱼巣/界面/控制面板窗口.cpp:661, 海中鱼巣/界面/控制面板窗口.cpp:667, 海中鱼巣/界面/控制面板窗口.cpp:671, 海中鱼巣/界面/控制面板窗口.cpp:677, 海中鱼巣/界面/控制面板窗口.cpp:686, 海中鱼巣/界面/控制面板窗口.cpp:689</code> | implicit-local-destructor-of-explicit-project-definition | <code>未单独冻结；读取源码调用点</code> | line 653 局部收口器已构造；后续任一 return 或正常作用域退出均调用源码显式定义析构 | 专项源码静态类型与实际装配人工复核 |
| RCE0703 | R0189 | R0187 | <code>海中鱼巣/界面/控制面板窗口.cpp:670</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0704 | R0191 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:696</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0705 | R0192 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:707, 海中鱼巣/界面/控制面板窗口.cpp:716, 海中鱼巣/界面/控制面板窗口.cpp:728, 海中鱼巣/界面/控制面板窗口.cpp:736</code> | unique_name+direct-member+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0706 | R0192 | R0303 | <code>海中鱼巣/界面/控制面板窗口.cpp:722</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐项发布概念根下拉选项前复核选项完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0707 | R0194 | R0168 | <code>海中鱼巣/界面/控制面板窗口.cpp:760</code> | same_module+direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 逐边源码/静态类型审计 PASS；专项源码静态类型与实际装配人工复核 |
| RCE0708 | R0195 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:769, 海中鱼巣/界面/控制面板窗口.cpp:770</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0709 | R0195 | R0169 | <code>海中鱼巣/界面/控制面板窗口.cpp:772, 海中鱼巣/界面/控制面板窗口.cpp:773, 海中鱼巣/界面/控制面板窗口.cpp:774, 海中鱼巣/界面/控制面板窗口.cpp:775, 海中鱼巣/界面/控制面板窗口.cpp:776, 海中鱼巣/界面/控制面板窗口.cpp:777, 海中鱼巣/界面/控制面板窗口.cpp:778, 海中鱼巣/界面/控制面板窗口.cpp:779, 海中鱼巣/界面/控制面板窗口.cpp:780, 海中鱼巣/界面/控制面板窗口.cpp:781, 海中鱼巣/界面/控制面板窗口.cpp:782</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0710 | R0196 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:787, 海中鱼巣/界面/控制面板窗口.cpp:803, 海中鱼巣/界面/控制面板窗口.cpp:822</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0711 | R0197 | F0563 | <code>海中鱼巣/界面/控制面板窗口.cpp:866, 海中鱼巣/界面/控制面板窗口.cpp:880</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0712 | R0197 | R0182 | <code>海中鱼巣/界面/控制面板窗口.cpp:872, 海中鱼巣/界面/控制面板窗口.cpp:874, 海中鱼巣/界面/控制面板窗口.cpp:876</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0713 | R0197 | R0171 | <code>海中鱼巣/界面/控制面板窗口.cpp:873, 海中鱼巣/界面/控制面板窗口.cpp:875, 海中鱼巣/界面/控制面板窗口.cpp:877</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0714 | R0198 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:892</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0715 | R0198 | R0196 | <code>海中鱼巣/界面/控制面板窗口.cpp:895</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0716 | R0198 | R0181 | <code>海中鱼巣/界面/控制面板窗口.cpp:896</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0717 | R0198 | R0155 | <code>海中鱼巣/界面/控制面板窗口.cpp:897</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0718 | R0198 | R0197 | <code>海中鱼巣/界面/控制面板窗口.cpp:901</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0719 | R0199 | R0159 | <code>海中鱼巣/界面/控制面板窗口.cpp:907, 海中鱼巣/界面/控制面板窗口.cpp:923</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0720 | R0199 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:908</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0721 | R0199 | R0183 | <code>海中鱼巣/界面/控制面板窗口.cpp:912</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0722 | R0199 | R0191 | <code>海中鱼巣/界面/控制面板窗口.cpp:915</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0723 | R0199 | R0194 | <code>海中鱼巣/界面/控制面板窗口.cpp:918</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0724 | R0199 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:919</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0725 | R0199 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:920</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0726 | R0199 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:921</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0727 | R0199 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:925</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0728 | R0200 | R0159 | <code>海中鱼巣/界面/控制面板窗口.cpp:931</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0729 | R0200 | R0162 | <code>海中鱼巣/界面/控制面板窗口.cpp:933</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且 libclang 直接引用项目定义；源码位置复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0730 | R0200 | R0199 | <code>海中鱼巣/界面/控制面板窗口.cpp:933</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0731 | R0201 | R0167 | <code>海中鱼巣/界面/控制面板窗口.cpp:1000, 海中鱼巣/界面/控制面板窗口.cpp:980, 海中鱼巣/界面/控制面板窗口.cpp:989</code> | unique_name+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0732 | R0201 | F0393 | <code>海中鱼巣/界面/控制面板窗口.cpp:1003, 海中鱼巣/界面/控制面板窗口.cpp:940, 海中鱼巣/界面/控制面板窗口.cpp:968, 海中鱼巣/界面/控制面板窗口.cpp:984, 海中鱼巣/界面/控制面板窗口.cpp:993</code> | unique_name+direct-member+direct-member+direct-member+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0733 | R0201 | R0191 | <code>海中鱼巣/界面/控制面板窗口.cpp:944, 海中鱼巣/界面/控制面板窗口.cpp:973</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0734 | R0201 | R0153 | <code>海中鱼巣/界面/控制面板窗口.cpp:947, 海中鱼巣/界面/控制面板窗口.cpp:979</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0735 | R0201 | R0176 | <code>海中鱼巣/界面/控制面板窗口.cpp:954</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0736 | R0201 | R0189 | <code>海中鱼巣/界面/控制面板窗口.cpp:963, 海中鱼巣/界面/控制面板窗口.cpp:967</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0737 | R0201 | R0192 | <code>海中鱼巣/界面/控制面板窗口.cpp:963, 海中鱼巣/界面/控制面板窗口.cpp:967</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0738 | R0201 | R0193 | <code>海中鱼巣/界面/控制面板窗口.cpp:976</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0739 | R0201 | R0195 | <code>海中鱼巣/界面/控制面板窗口.cpp:977, 海中鱼巣/界面/控制面板窗口.cpp:997</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0740 | R0201 | R0156 | <code>海中鱼巣/界面/控制面板窗口.cpp:978, 海中鱼巣/界面/控制面板窗口.cpp:999</code> | unique_name+direct-member+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核；专项源码静态类型与实际装配人工复核 |
| RCE0741 | R0201 | R0197 | <code>海中鱼巣/界面/控制面板窗口.cpp:988</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0742 | R0201 | R0198 | <code>海中鱼巣/界面/控制面板窗口.cpp:998</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用方可达且 libclang 直接引用项目定义；源码位置复核 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE0743 | R0204 | R0077 | <code>海中鱼巣/领域/二次特征服务.h:111</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0744 | R0204 | R0205 | <code>海中鱼巣/领域/二次特征服务.h:113</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0745 | R0206 | F0622 | <code>海中鱼巣/领域/二次特征服务.h:39</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0746 | R0206 | F0623 | <code>海中鱼巣/领域/二次特征服务.h:40</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0747 | R0206 | R0009 | <code>海中鱼巣/领域/二次特征服务.h:43</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0748 | R0207 | R0218 | <code>海中鱼巣/领域/任务服务.h:414</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0749 | R0207 | R0219 | <code>海中鱼巣/领域/任务服务.h:417</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0750 | R0208 | R0218 | <code>海中鱼巣/领域/任务服务.h:421</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0751 | R0208 | R0219 | <code>海中鱼巣/领域/任务服务.h:424</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0752 | R0209 | R0218 | <code>海中鱼巣/领域/任务服务.h:428</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0753 | R0209 | R0219 | <code>海中鱼巣/领域/任务服务.h:431</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0754 | R0210 | R0218 | <code>海中鱼巣/领域/任务服务.h:435</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0755 | R0210 | R0219 | <code>海中鱼巣/领域/任务服务.h:438</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0756 | R0211 | R0222 | <code>海中鱼巣/领域/任务服务.h:442</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0757 | R0211 | R0221 | <code>海中鱼巣/领域/任务服务.h:445</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0758 | R0212 | R0222 | <code>海中鱼巣/领域/任务服务.h:449</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0759 | R0212 | R0221 | <code>海中鱼巣/领域/任务服务.h:452</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0760 | R0213 | R0218 | <code>海中鱼巣/领域/任务服务.h:456</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0761 | R0213 | R0219 | <code>海中鱼巣/领域/任务服务.h:459, 海中鱼巣/领域/任务服务.h:460, 海中鱼巣/领域/任务服务.h:461, 海中鱼巣/领域/任务服务.h:462</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整；四类承接目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0762 | R0213 | R0221 | <code>海中鱼巣/领域/任务服务.h:463</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整；读取已创建状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0763 | R0214 | R0222 | <code>海中鱼巣/领域/任务服务.h:482, 海中鱼巣/领域/任务服务.h:486</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 需求入口及每个来源节点类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0764 | R0214 | R0088 | <code>海中鱼巣/领域/任务服务.h:485</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 需求类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0765 | R0214 | R0213 | <code>海中鱼巣/领域/任务服务.h:489</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 来源节点为任务 | 专项源码静态类型与实际装配人工复核 |
| RCE0766 | R0214 | F0051 | <code>海中鱼巣/领域/任务服务.h:490, 海中鱼巣/领域/任务服务.h:503</code> | direct_free+standard_library_callback+source_audited | <code>line490: 承接材料-&gt;来源需求, 需求节点；line503: std::unique相邻节点句柄对</code> | line490在承接材料存在时调用；line503在任务组排序完成后由std::unique零到多次调用 | 当前源码直接比较、标准算法默认相等合同、节点句柄静态类型与RCB0003复核 |
| RCE0767 | R0214 | R0215 | <code>海中鱼巣/领域/任务服务.h:494</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册的唯一比较回调 | 专项源码静态类型与实际装配人工复核 |
| RCE0768 | R0216 | R0218 | <code>海中鱼巣/领域/任务服务.h:517</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0769 | R0216 | R0220 | <code>海中鱼巣/领域/任务服务.h:520</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整；四参数顺序重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0770 | R0217 | R0218 | <code>海中鱼巣/领域/任务服务.h:544</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0771 | R0217 | R0220 | <code>海中鱼巣/领域/任务服务.h:547</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务承接壳完整；四参数顺序重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0772 | R0218 | R0219 | <code>海中鱼巣/领域/任务服务.h:727, 海中鱼巣/领域/任务服务.h:728, 海中鱼巣/领域/任务服务.h:729, 海中鱼巣/领域/任务服务.h:730</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 短路读取四类承接目标 | 专项源码静态类型与实际装配人工复核 |
| RCE0773 | R0218 | R0221 | <code>海中鱼巣/领域/任务服务.h:731</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 四类承接目标均存在 | 专项源码静态类型与实际装配人工复核 |
| RCE0774 | R0219 | R0220 | <code>海中鱼巣/领域/任务服务.h:735</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 显式第四参数 std::nullopt | 专项源码静态类型与实际装配人工复核 |
| RCE0775 | R0220 | R0222 | <code>海中鱼巣/领域/任务服务.h:740, 海中鱼巣/领域/任务服务.h:748</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 源任务及每个关系目标类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0776 | R0220 | F0620 | <code>海中鱼巣/领域/任务服务.h:744</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 顺序号有值；三实参仓库重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0777 | R0220 | F0621 | <code>海中鱼巣/领域/任务服务.h:745</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 顺序号无值；两实参仓库重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0778 | R0221 | R0222 | <code>海中鱼巣/领域/任务服务.h:761, 海中鱼巣/领域/任务服务.h:767</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务及每个关系目标类型复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0779 | R0221 | F0621 | <code>海中鱼巣/领域/任务服务.h:764</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 任务类型匹配 | 专项源码静态类型与实际装配人工复核 |
| RCE0780 | R0221 | F0329 | <code>海中鱼巣/领域/任务服务.h:770</code> | source-audited | <code>this=&状态,目标</code> | 遍历任务引用目标且目标类型为状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0781 | R0222 | F0190 | <code>海中鱼巣/领域/任务服务.h:784</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 入口 | 专项源码静态类型与实际装配人工复核 |
| RCE0782 | R0224 | R0147 | <code>海中鱼巣/领域/初始化.世界树.ixx:132</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0783 | R0224 | R0008 | <code>海中鱼巣/领域/初始化.世界树.ixx:133</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0784 | R0225 | F0168 | <code>海中鱼巣/领域/初始化.世界树.ixx:137, 海中鱼巣/领域/初始化.世界树.ixx:140, 海中鱼巣/领域/初始化.世界树.ixx:143</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0785 | R0225 | R0075 | <code>海中鱼巣/领域/初始化.世界树.ixx:138, 海中鱼巣/领域/初始化.世界树.ixx:141, 海中鱼巣/领域/初始化.世界树.ixx:144</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0786 | R0225 | R0224 | <code>海中鱼巣/领域/初始化.世界树.ixx:146, 海中鱼巣/领域/初始化.世界树.ixx:147, 海中鱼巣/领域/初始化.世界树.ixx:148</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0787 | R0229 | R0501 | <code>海中鱼巣/领域/初始化.系统角色.ixx:220</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 特征服务字段静态类型为特征业务服务& | 专项源码静态类型与实际装配人工复核 |
| RCE0789 | R0229 | R0369 | <code>海中鱼巣/领域/初始化.系统角色.ixx:221, 海中鱼巣/领域/初始化.系统角色.ixx:228</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 两个返回值静态类型均为特征体系业务结果 | 专项源码静态类型与实际装配人工复核 |
| RCE1687 | R0229 | R0601 | <code>海中鱼巣/领域/初始化.系统角色.ixx:221, 海中鱼巣/领域/初始化.系统角色.ixx:228</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 特征结果状态静态类型为特征体系业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0790 | R0229 | R0370 | <code>海中鱼巣/领域/初始化.系统角色.ixx:222, 海中鱼巣/领域/初始化.系统角色.ixx:229</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 两个返回值静态类型均为特征体系业务结果 | 专项源码静态类型与实际装配人工复核 |
| RCE0791 | R0229 | R0566 | <code>海中鱼巣/领域/初始化.系统角色.ixx:224</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 特征状态组合器字段静态类型为特征状态组合器& | 专项源码静态类型与实际装配人工复核 |
| RCE0792 | R0229 | R0514 | <code>海中鱼巣/领域/初始化.系统角色.ixx:231</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 需求服务字段静态类型为需求业务服务& | 专项源码静态类型与实际装配人工复核 |
| RCE0794 | R0229 | R0465 | <code>海中鱼巣/领域/初始化.系统角色.ixx:234</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 需求返回值静态类型为需求提交结果 | 专项源码静态类型与实际装配人工复核 |
| RCE1688 | R0229 | R0602 | <code>海中鱼巣/领域/初始化.系统角色.ixx:234</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 需求结果状态静态类型为需求任务方法业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1689 | R0229 | R0603 | <code>海中鱼巣/领域/初始化.系统角色.ixx:235</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 需求结果状态静态类型为需求任务方法业务状态 | 专项源码静态类型与实际装配人工复核 |
| RCE0796 | R0230 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:242</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 形成身份进入后查找具名系统角色键占用材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0797 | R0231 | R0230 | <code>海中鱼巣/领域/初始化.系统角色.ixx:253, 海中鱼巣/领域/初始化.系统角色.ixx:254, 海中鱼巣/领域/初始化.系统角色.ixx:255, 海中鱼巣/领域/初始化.系统角色.ixx:260, 海中鱼巣/领域/初始化.系统角色.ixx:261, 海中鱼巣/领域/初始化.系统角色.ixx:262, 海中鱼巣/领域/初始化.系统角色.ixx:263, 海中鱼巣/领域/初始化.系统角色.ixx:264, 海中鱼巣/领域/初始化.系统角色.ixx:267, 海中鱼巣/领域/初始化.系统角色.ixx:268, 海中鱼巣/领域/初始化.系统角色.ixx:269, 海中鱼巣/领域/初始化.系统角色.ixx:270, 海中鱼巣/领域/初始化.系统角色.ixx:271, 海中鱼巣/领域/初始化.系统角色.ixx:273, 海中鱼巣/领域/初始化.系统角色.ixx:274, 海中鱼巣/领域/初始化.系统角色.ixx:275, 海中鱼巣/领域/初始化.系统角色.ixx:276, 海中鱼巣/领域/初始化.系统角色.ixx:278, 海中鱼巣/领域/初始化.系统角色.ixx:279, 海中鱼巣/领域/初始化.系统角色.ixx:280, 海中鱼巣/领域/初始化.系统角色.ixx:281</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0798 | R0232 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:288, 海中鱼巣/领域/初始化.系统角色.ixx:299, 海中鱼巣/领域/初始化.系统角色.ixx:306, 海中鱼巣/领域/初始化.系统角色.ixx:312, 海中鱼巣/领域/初始化.系统角色.ixx:313</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 按短路顺序查找世界根、动态根、因果根、关系根和存在根用途 | 专项源码静态类型与实际装配人工复核 |
| RCE0799 | R0232 | R0233 | <code>海中鱼巣/领域/初始化.系统角色.ixx:294, 海中鱼巣/领域/初始化.系统角色.ixx:296</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0800 | R0232 | R0398 | <code>海中鱼巣/领域/初始化.系统角色.ixx:302</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0801 | R0232 | R0512 | <code>海中鱼巣/领域/初始化.系统角色.ixx:308</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0802 | R0232 | R0234 | <code>海中鱼巣/领域/初始化.系统角色.ixx:334</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0803 | R0233 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:355, 海中鱼巣/领域/初始化.系统角色.ixx:356, 海中鱼巣/领域/初始化.系统角色.ixx:357, 海中鱼巣/领域/初始化.系统角色.ixx:358, 海中鱼巣/领域/初始化.系统角色.ixx:359, 海中鱼巣/领域/初始化.系统角色.ixx:360, 海中鱼巣/领域/初始化.系统角色.ixx:361</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 完成安全组和服务组用途选择后查找七类占用材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0804 | R0233 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:385</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 目标占用材料存在且已占用，并已读取主键状态材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0805 | R0233 | R0434 | <code>海中鱼巣/领域/初始化.系统角色.ixx:393</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0806 | R0234 | R0565 | <code>海中鱼巣/领域/初始化.系统角色.ixx:406, 海中鱼巣/领域/初始化.系统角色.ixx:407, 海中鱼巣/领域/初始化.系统角色.ixx:408, 海中鱼巣/领域/初始化.系统角色.ixx:409</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 已有方法根语义匹配后查找登记根和三状态角色用途 | 专项源码静态类型与实际装配人工复核 |
| RCE0807 | R0234 | R0235 | <code>海中鱼巣/领域/初始化.系统角色.ixx:410, 海中鱼巣/领域/初始化.系统角色.ixx:411, 海中鱼巣/领域/初始化.系统角色.ixx:412</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0808 | R0234 | R0497 | <code>海中鱼巣/领域/初始化.系统角色.ixx:418</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0809 | R0235 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:430</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 占用材料非空且已占用，并已读取主键状态材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0810 | R0236 | R0237 | <code>海中鱼巣/领域/初始化.系统角色.ixx:441, 海中鱼巣/领域/初始化.系统角色.ixx:442</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0811 | R0236 | R0512 | <code>海中鱼巣/领域/初始化.系统角色.ixx:445</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0812 | R0236 | R0398 | <code>海中鱼巣/领域/初始化.系统角色.ixx:447</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0813 | R0236 | R0497 | <code>海中鱼巣/领域/初始化.系统角色.ixx:462</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0814 | R0236 | R0238 | <code>海中鱼巣/领域/初始化.系统角色.ixx:468, 海中鱼巣/领域/初始化.系统角色.ixx:471, 海中鱼巣/领域/初始化.系统角色.ixx:474</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0815 | R0237 | R0390 | <code>海中鱼巣/领域/初始化.系统角色.ixx:492</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 复核根需求组完成五类材料读取后进入合取链第一项 | 专项源码静态类型与实际装配人工复核 |
| RCE0816 | R0237 | R0343 | <code>海中鱼巣/领域/初始化.系统角色.ixx:494</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 定义材料完整且身份匹配后复核槽位材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0817 | R0237 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:502</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 根需求组特征材料复核通过后复核目标抽象状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE0818 | R0237 | R0434 | <code>海中鱼巣/领域/初始化.系统角色.ixx:504</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0819 | R0238 | R0407 | <code>海中鱼巣/领域/初始化.系统角色.ixx:518</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 复核抽象状态进入并读取身份稳定主键材料 | 专项源码静态类型与实际装配人工复核 |
| RCE0820 | R0241 | F0168 | <code>海中鱼巣/领域/初始化.语素.ixx:178</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0821 | R0241 | R0242 | <code>海中鱼巣/领域/初始化.语素.ixx:178</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0822 | R0241 | R0244 | <code>海中鱼巣/领域/初始化.语素.ixx:179</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0823 | R0241 | F0364 | <code>海中鱼巣/领域/初始化.语素.ixx:182</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0824 | R0241 | R0243 | <code>海中鱼巣/领域/初始化.语素.ixx:184</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0825 | R0241 | R0245 | <code>海中鱼巣/领域/初始化.语素.ixx:185</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0826 | R0242 | R0009 | <code>海中鱼巣/领域/初始化.语素.ixx:195</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0827 | R0243 | F0496 | <code>海中鱼巣/领域/初始化.语素.ixx:199</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0828 | R0243 | F0497 | <code>海中鱼巣/领域/初始化.语素.ixx:200</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0829 | R0244 | R0147 | <code>海中鱼巣/领域/初始化.语素.ixx:220</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0830 | R0244 | R0008 | <code>海中鱼巣/领域/初始化.语素.ixx:221</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0831 | R0245 | F0168 | <code>海中鱼巣/领域/初始化.语素.ixx:225, 海中鱼巣/领域/初始化.语素.ixx:228, 海中鱼巣/领域/初始化.语素.ixx:231, 海中鱼巣/领域/初始化.语素.ixx:234</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0832 | R0245 | R0075 | <code>海中鱼巣/领域/初始化.语素.ixx:226, 海中鱼巣/领域/初始化.语素.ixx:229</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0833 | R0245 | R0147 | <code>海中鱼巣/领域/初始化.语素.ixx:232</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0834 | R0245 | R0008 | <code>海中鱼巣/领域/初始化.语素.ixx:235</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0835 | R0245 | R0244 | <code>海中鱼巣/领域/初始化.语素.ixx:237</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0836 | R0246 | R0245 | <code>海中鱼巣/领域/初始化.语素.ixx:264</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0837 | R0247 | F0220 | <code>海中鱼巣/领域/初始化.语素.ixx:36</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 名称和类型两字段依次短路调用 | 专项源码静态类型与实际装配人工复核 |
| RCE0838 | R0249 | R0560 | <code>海中鱼巣/领域/初始化.需求.ixx:123</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0839 | R0249 | F0168 | <code>海中鱼巣/领域/初始化.需求.ixx:124, 海中鱼巣/领域/初始化.需求.ixx:138, 海中鱼巣/领域/初始化.需求.ixx:142, 海中鱼巣/领域/初始化.需求.ixx:156</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0840 | R0249 | F0184 | <code>海中鱼巣/领域/初始化.需求.ixx:124, 海中鱼巣/领域/初始化.需求.ixx:130, 海中鱼巣/领域/初始化.需求.ixx:138, 海中鱼巣/领域/初始化.需求.ixx:142, 海中鱼巣/领域/初始化.需求.ixx:156, 海中鱼巣/领域/初始化.需求.ixx:178</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0841 | R0249 | F0364 | <code>海中鱼巣/领域/初始化.需求.ixx:128</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0842 | R0249 | R0562 | <code>海中鱼巣/领域/初始化.需求.ixx:143</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0843 | R0249 | R0579 | <code>海中鱼巣/领域/初始化.需求.ixx:148</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0844 | R0249 | R0581 | <code>海中鱼巣/领域/初始化.需求.ixx:159</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0845 | R0249 | F0554 | <code>海中鱼巣/领域/初始化.需求.ixx:161</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0846 | R0249 | F0555 | <code>海中鱼巣/领域/初始化.需求.ixx:162</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0847 | R0249 | R0586 | <code>海中鱼巣/领域/初始化.需求.ixx:164</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0848 | R0249 | F0496 | <code>海中鱼巣/领域/初始化.需求.ixx:165</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0849 | R0249 | F0497 | <code>海中鱼巣/领域/初始化.需求.ixx:166</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0866 | R0260 | R0017 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:111</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 准备视图形参复核值节点属于本会话候选 | 专项源码静态类型与实际装配人工复核 |
| RCE0867 | R0260 | R0016 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:112</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0868 | R0260 | R0018 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:113</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 准备视图形参读取值节点候选类型 | 专项源码静态类型与实际装配人工复核 |
| RCE0869 | R0260 | R0019 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:115</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0870 | R0260 | R0020 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:118</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0871 | R0260 | R0554 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:129</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0872 | R0260 | R0556 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:130</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0873 | R0260 | R0264 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:166</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0874 | R0263 | R0264 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:197</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0875 | R0264 | R0546 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:207</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 非 const 服务引用选择 Vec 记录非 const 查找重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0876 | R0264 | R0550 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:215</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 非 const 服务引用选择 I64 记录非 const 查找重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0877 | R0266 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:247</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 侧表读取入口复核节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0878 | R0266 | R0554 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:254</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；const 服务读取 Vec 记录数量 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0879 | R0266 | R0556 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:255</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；const 服务读取 I64 版本记录数量 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0880 | R0266 | R0552 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:261</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；const 服务选择 I64 记录 const 查找重载 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0881 | R0266 | R0545 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:262</code> | unique_name+direct-static-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；复核 I64 版本记录内部一致 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0882 | R0266 | R0548 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:272</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；const 服务选择 Vec 记录 const 查找重载 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0883 | R0266 | R0544 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:273</code> | unique_name+direct-static-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；复核 Vec 记录内部一致 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0884 | R0267 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:31</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 特征值节点形参静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0885 | R0267 | F0565 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:31</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 主信息形参静态类型为主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0886 | R0268 | F0163 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:63</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 快照中的特征值节点静态类型为节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0887 | R0270 | R0267 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:83</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 材料形参静态类型复核字段自洽 | 专项源码静态类型与实际装配人工复核 |
| RCE0888 | R0270 | R0271 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:84</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册并调用本地唯一重复身份谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE0889 | R0271 | R0259 | <code>海中鱼巣/领域/参与者.特征值原始材料.ixx:85</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | find_if 谓词调用同类静态节点身份比较 | 专项源码静态类型与实际装配人工复核 |
| RCE0892 | R0275 | R0276 | <code>海中鱼巣/领域/场景服务.h:53</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0893 | R0278 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1019</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0894 | R0278 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:1048</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0895 | R0278 | R0328 | <code>海中鱼巣/领域/控制面板服务.h:1050</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0896 | R0279 | R0280 | <code>海中鱼巣/领域/控制面板服务.h:1064</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0897 | R0279 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1077, 海中鱼巣/领域/控制面板服务.h:1127</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0898 | R0279 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1083, 海中鱼巣/领域/控制面板服务.h:1133</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0899 | R0279 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:1083, 海中鱼巣/领域/控制面板服务.h:1133</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0900 | R0279 | R0527 | <code>海中鱼巣/领域/控制面板服务.h:1090</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0901 | R0280 | R0310 | <code>海中鱼巣/领域/控制面板服务.h:1068</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 投影深度相等 | 专项源码静态类型与实际装配人工复核 |
| RCE0902 | R0281 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1192</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 目标存在 | 专项源码静态类型与实际装配人工复核 |
| RCE0903 | R0281 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1193</code> | source-static-type+direct-free | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核；子项已生成 | 专项源码静态类型与实际装配人工复核 |
| RCE0904 | R0282 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1204</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 把具体句柄提升为 optional 重载 | 专项源码静态类型与实际装配人工复核 |
| RCE0905 | R0283 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1211, 海中鱼巣/领域/控制面板服务.h:1232</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0906 | R0283 | R0580 | <code>海中鱼巣/领域/控制面板服务.h:1218</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0907 | R0283 | R0214 | <code>海中鱼巣/领域/控制面板服务.h:1222</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0908 | R0283 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1224</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0909 | R0284 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1248, 海中鱼巣/领域/控制面板服务.h:1257</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0910 | R0284 | R0217 | <code>海中鱼巣/领域/控制面板服务.h:1253</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0911 | R0284 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1265</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0912 | R0284 | R0285 | <code>海中鱼巣/领域/控制面板服务.h:1266</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0913 | R0285 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1267, 海中鱼巣/领域/控制面板服务.h:1270, 海中鱼巣/领域/控制面板服务.h:1273</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 方法来源任务组稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0914 | R0286 | R0580 | <code>海中鱼巣/领域/控制面板服务.h:1289</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0915 | R0286 | R0593 | <code>海中鱼巣/领域/控制面板服务.h:1293</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0916 | R0286 | F0552 | <code>海中鱼巣/领域/控制面板服务.h:1294</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 需求有效 | 专项源码静态类型与实际装配人工复核 |
| RCE0917 | R0286 | R0586 | <code>海中鱼巣/领域/控制面板服务.h:1294</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0918 | R0286 | R0581 | <code>海中鱼巣/领域/控制面板服务.h:1295</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0919 | R0286 | R0582 | <code>海中鱼巣/领域/控制面板服务.h:1296</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0920 | R0286 | R0583 | <code>海中鱼巣/领域/控制面板服务.h:1297</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0921 | R0286 | R0584 | <code>海中鱼巣/领域/控制面板服务.h:1298</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0922 | R0286 | R0585 | <code>海中鱼巣/领域/控制面板服务.h:1299</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0923 | R0286 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1316, 海中鱼巣/领域/控制面板服务.h:1329</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0924 | R0286 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1317, 海中鱼巣/领域/控制面板服务.h:1336</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0925 | R0286 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:1317, 海中鱼巣/领域/控制面板服务.h:1336</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0926 | R0286 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1318, 海中鱼巣/领域/控制面板服务.h:1319, 海中鱼巣/领域/控制面板服务.h:1320, 海中鱼巣/领域/控制面板服务.h:1321, 海中鱼巣/领域/控制面板服务.h:1322, 海中鱼巣/领域/控制面板服务.h:1337, 海中鱼巣/领域/控制面板服务.h:1338, 海中鱼巣/领域/控制面板服务.h:1339, 海中鱼巣/领域/控制面板服务.h:1351</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0927 | R0286 | R0587 | <code>海中鱼巣/领域/控制面板服务.h:1327</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；需求承接材料完整 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0928 | R0286 | R0588 | <code>海中鱼巣/领域/控制面板服务.h:1330</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0929 | R0286 | R0589 | <code>海中鱼巣/领域/控制面板服务.h:1331</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0930 | R0286 | R0590 | <code>海中鱼巣/领域/控制面板服务.h:1332</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0931 | R0286 | R0591 | <code>海中鱼巣/领域/控制面板服务.h:1333</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0932 | R0286 | R0592 | <code>海中鱼巣/领域/控制面板服务.h:1334</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；存在最近结算 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0933 | R0286 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1335, 海中鱼巣/领域/控制面板服务.h:1359</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0934 | R0286 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1345, 海中鱼巣/领域/控制面板服务.h:1361</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 具体句柄目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0935 | R0286 | R0214 | <code>海中鱼巣/领域/控制面板服务.h:1358</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0936 | R0287 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1373</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0937 | R0287 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1374</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0938 | R0287 | R0286 | <code>海中鱼巣/领域/控制面板服务.h:1386</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0939 | R0288 | R0213 | <code>海中鱼巣/领域/控制面板服务.h:1399</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0940 | R0288 | R0207 | <code>海中鱼巣/领域/控制面板服务.h:1400</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0941 | R0288 | R0208 | <code>海中鱼巣/领域/控制面板服务.h:1401</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0942 | R0288 | R0209 | <code>海中鱼巣/领域/控制面板服务.h:1402</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0943 | R0288 | R0210 | <code>海中鱼巣/领域/控制面板服务.h:1403</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0944 | R0288 | R0211 | <code>海中鱼巣/领域/控制面板服务.h:1404</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0945 | R0288 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1419</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0946 | R0288 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1420</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0947 | R0288 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:1420</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0948 | R0288 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1421, 海中鱼巣/领域/控制面板服务.h:1422, 海中鱼巣/领域/控制面板服务.h:1423, 海中鱼巣/领域/控制面板服务.h:1424, 海中鱼巣/领域/控制面板服务.h:1425, 海中鱼巣/领域/控制面板服务.h:1433, 海中鱼巣/领域/控制面板服务.h:1438, 海中鱼巣/领域/控制面板服务.h:1443</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 任务 optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0949 | R0288 | R0212 | <code>海中鱼巣/领域/控制面板服务.h:1429</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0950 | R0288 | R0216 | <code>海中鱼巣/领域/控制面板服务.h:1430</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0951 | R0288 | R0217 | <code>海中鱼巣/领域/控制面板服务.h:1431</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0952 | R0289 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1455</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0953 | R0289 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1456</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0954 | R0289 | R0288 | <code>海中鱼巣/领域/控制面板服务.h:1468</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0955 | R0290 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:1489, 海中鱼巣/领域/控制面板服务.h:1501, 海中鱼巣/领域/控制面板服务.h:1514, 海中鱼巣/领域/控制面板服务.h:1525, 海中鱼巣/领域/控制面板服务.h:1536</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0956 | R0290 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:1490, 海中鱼巣/领域/控制面板服务.h:1502, 海中鱼巣/领域/控制面板服务.h:1516, 海中鱼巣/领域/控制面板服务.h:1527, 海中鱼巣/领域/控制面板服务.h:1537</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0957 | R0290 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:1490, 海中鱼巣/领域/控制面板服务.h:1502, 海中鱼巣/领域/控制面板服务.h:1516, 海中鱼巣/领域/控制面板服务.h:1527, 海中鱼巣/领域/控制面板服务.h:1537</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0958 | R0290 | R0468 | <code>海中鱼巣/领域/控制面板服务.h:1496</code> | unique_name | <code>(方法节点, 状态_)</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0959 | R0290 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1503</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法虚拟存在具体句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0960 | R0290 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1511, 海中鱼巣/领域/控制面板服务.h:1512, 海中鱼巣/领域/控制面板服务.h:1579, 海中鱼巣/领域/控制面板服务.h:1580</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0961 | R0290 | R0492 | <code>海中鱼巣/领域/控制面板服务.h:1515</code> | unique_name+source-audited | <code>(方法节点, 条件, 状态_)</code> | 调用方可达且源码分支条件成立；方法树节点包含方法条件 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE0962 | R0290 | R0281 | <code>海中鱼巣/领域/控制面板服务.h:1517, 海中鱼巣/领域/控制面板服务.h:1528, 海中鱼巣/领域/控制面板服务.h:1545, 海中鱼巣/领域/控制面板服务.h:1551, 海中鱼巣/领域/控制面板服务.h:1560</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法 optional 目标叶项 | 专项源码静态类型与实际装配人工复核 |
| RCE0963 | R0290 | R0493 | <code>海中鱼巣/领域/控制面板服务.h:1526</code> | unique_name | <code>(方法节点, 结果节点, 状态_)</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0964 | R0290 | R0474 | <code>海中鱼巣/领域/控制面板服务.h:1542</code> | unique_name | <code>(候选材料->动作入口)</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0965 | R0290 | R0475 | <code>海中鱼巣/领域/控制面板服务.h:1543</code> | unique_name | <code>(候选材料->动作入口)</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0966 | R0290 | R0473 | <code>海中鱼巣/领域/控制面板服务.h:1558</code> | unique_name | <code>(方法节点, 状态_)</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0967 | R0290 | R0291 | <code>海中鱼巣/领域/控制面板服务.h:1575, 海中鱼巣/领域/控制面板服务.h:1582, 海中鱼巣/领域/控制面板服务.h:1588</code> | direct-lambda | <code>未单独冻结；读取源码调用点</code> | 父方法/前置/后续递归投影 | 专项源码静态类型与实际装配人工复核 |
| RCE0968 | R0291 | R0290 | <code>海中鱼巣/领域/控制面板服务.h:1567</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 关联方法递归展开 | 专项源码静态类型与实际装配人工复核 |
| RCE0969 | R0292 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:1603</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0970 | R0292 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:1604</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0971 | R0292 | R0293 | <code>海中鱼巣/领域/控制面板服务.h:1615</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0972 | R0292 | R0290 | <code>海中鱼巣/领域/控制面板服务.h:1631</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0973 | R0292 | R0282 | <code>海中鱼巣/领域/控制面板服务.h:1647</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法来源任务具体句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE0974 | R0293 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1616, 海中鱼巣/领域/控制面板服务.h:1619, 海中鱼巣/领域/控制面板服务.h:1622</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 方法来源任务组稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0975 | R0294 | R0294 | <code>海中鱼巣/领域/控制面板服务.h:1661</code> | direct-recursive | <code>未单独冻结；读取源码调用点</code> | 对子节点递归排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0976 | R0294 | R0295 | <code>海中鱼巣/领域/控制面板服务.h:1663</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0977 | R0295 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1668, 海中鱼巣/领域/控制面板服务.h:1671</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 关系角色相同 | 专项源码静态类型与实际装配人工复核 |
| RCE0978 | R0296 | R0294 | <code>海中鱼巣/领域/控制面板服务.h:1680</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0979 | R0296 | R0297 | <code>海中鱼巣/领域/控制面板服务.h:1682</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE0980 | R0297 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:1683</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 根节点稳定排序 | 专项源码静态类型与实际装配人工复核 |
| RCE0981 | R0298 | R0298 | <code>海中鱼巣/领域/控制面板服务.h:1696</code> | direct-recursive | <code>未单独冻结；读取源码调用点</code> | 对子节点递归标记 | 专项源码静态类型与实际装配人工复核 |
| RCE0982 | R0299 | R0298 | <code>海中鱼巣/领域/控制面板服务.h:1703</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0983 | R0300 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:1718, 海中鱼巣/领域/控制面板服务.h:1727</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0984 | R0300 | R0300 | <code>海中鱼巣/领域/控制面板服务.h:1734</code> | direct-recursive | <code>未单独冻结；读取源码调用点</code> | 对子节点递归复核 | 专项源码静态类型与实际装配人工复核 |
| RCE0985 | R0301 | R0300 | <code>海中鱼巣/领域/控制面板服务.h:1757</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0986 | R0303 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:203</code> | source-static-type+direct-free | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核；根类别在范围内 | 专项源码静态类型与实际装配人工复核 |
| RCE0987 | R0303 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:203</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE0988 | R0304 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:280</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0989 | R0304 | R0495 | <code>海中鱼巣/领域/控制面板服务.h:285</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 逐边源码/静态类型审计 PASS |
| RCE0990 | R0306 | R0305 | <code>海中鱼巣/领域/控制面板服务.h:302, 海中鱼巣/领域/控制面板服务.h:303, 海中鱼巣/领域/控制面板服务.h:304, 海中鱼巣/领域/控制面板服务.h:305, 海中鱼巣/领域/控制面板服务.h:306, 海中鱼巣/领域/控制面板服务.h:307</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0991 | R0307 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:313</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0992 | R0307 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:315, 海中鱼巣/领域/控制面板服务.h:319, 海中鱼巣/领域/控制面板服务.h:322, 海中鱼巣/领域/控制面板服务.h:325, 海中鱼巣/领域/控制面板服务.h:331, 海中鱼巣/领域/控制面板服务.h:372, 海中鱼巣/领域/控制面板服务.h:375, 海中鱼巣/领域/控制面板服务.h:378, 海中鱼巣/领域/控制面板服务.h:391, 海中鱼巣/领域/控制面板服务.h:397, 海中鱼巣/领域/控制面板服务.h:427</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0993 | R0307 | R0321 | <code>海中鱼巣/领域/控制面板服务.h:328</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0994 | R0307 | R0318 | <code>海中鱼巣/领域/控制面板服务.h:339, 海中鱼巣/领域/控制面板服务.h:342</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0995 | R0307 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:346, 海中鱼巣/领域/控制面板服务.h:380</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0996 | R0307 | R0283 | <code>海中鱼巣/领域/控制面板服务.h:352, 海中鱼巣/领域/控制面板服务.h:359</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0997 | R0307 | R0284 | <code>海中鱼巣/领域/控制面板服务.h:361</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0998 | R0307 | R0317 | <code>海中鱼巣/领域/控制面板服务.h:381</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE0999 | R0307 | R0326 | <code>海中鱼巣/领域/控制面板服务.h:406</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1000 | R0307 | R0278 | <code>海中鱼巣/领域/控制面板服务.h:409</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1001 | R0307 | R0287 | <code>海中鱼巣/领域/控制面板服务.h:412</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1002 | R0307 | R0289 | <code>海中鱼巣/领域/控制面板服务.h:415</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1003 | R0307 | R0292 | <code>海中鱼巣/领域/控制面板服务.h:418</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1004 | R0307 | R0320 | <code>海中鱼巣/领域/控制面板服务.h:434</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1005 | R0308 | R0302 | <code>海中鱼巣/领域/控制面板服务.h:555</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1006 | R0312 | R0311 | <code>海中鱼巣/领域/控制面板服务.h:614</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1007 | R0313 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:626</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册具名比较函数 | 专项源码静态类型与实际装配人工复核 |
| RCE1008 | R0317 | R0316 | <code>海中鱼巣/领域/控制面板服务.h:665, 海中鱼巣/领域/控制面板服务.h:666, 海中鱼巣/领域/控制面板服务.h:667, 海中鱼巣/领域/控制面板服务.h:669, 海中鱼巣/领域/控制面板服务.h:670, 海中鱼巣/领域/控制面板服务.h:671, 海中鱼巣/领域/控制面板服务.h:674, 海中鱼巣/领域/控制面板服务.h:675, 海中鱼巣/领域/控制面板服务.h:676</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1009 | R0318 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:696</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1010 | R0318 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:696</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1011 | R0318 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:703</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1012 | R0320 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:732, 海中鱼巣/领域/控制面板服务.h:740</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1013 | R0320 | R0296 | <code>海中鱼巣/领域/控制面板服务.h:734</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1014 | R0320 | R0299 | <code>海中鱼巣/领域/控制面板服务.h:735</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1015 | R0320 | R0301 | <code>海中鱼巣/领域/控制面板服务.h:738</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1016 | R0321 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:763</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1017 | R0321 | R0319 | <code>海中鱼巣/领域/控制面板服务.h:765, 海中鱼巣/领域/控制面板服务.h:771, 海中鱼巣/领域/控制面板服务.h:792, 海中鱼巣/领域/控制面板服务.h:811, 海中鱼巣/领域/控制面板服务.h:822, 海中鱼巣/领域/控制面板服务.h:827, 海中鱼巣/领域/控制面板服务.h:834, 海中鱼巣/领域/控制面板服务.h:836, 海中鱼巣/领域/控制面板服务.h:839, 海中鱼巣/领域/控制面板服务.h:844, 海中鱼巣/领域/控制面板服务.h:850, 海中鱼巣/领域/控制面板服务.h:854, 海中鱼巣/领域/控制面板服务.h:859</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1018 | R0321 | R0515 | <code>海中鱼巣/领域/控制面板服务.h:767</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1019 | R0321 | F0355 | <code>海中鱼巣/领域/控制面板服务.h:768</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1020 | R0321 | R0516 | <code>海中鱼巣/领域/控制面板服务.h:769</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1021 | R0321 | R0322 | <code>海中鱼巣/领域/控制面板服务.h:773</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::sort 注册局部比较 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1022 | R0321 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:780, 海中鱼巣/领域/控制面板服务.h:806</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1023 | R0321 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:788</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1024 | R0321 | R0315 | <code>海中鱼巣/领域/控制面板服务.h:795</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1025 | R0321 | R0323 | <code>海中鱼巣/领域/控制面板服务.h:808</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::all_of 注册局部判定 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1026 | R0321 | R0317 | <code>海中鱼巣/领域/控制面板服务.h:824</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1027 | R0321 | R0517 | <code>海中鱼巣/领域/控制面板服务.h:829</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1028 | R0321 | R0530 | <code>海中鱼巣/领域/控制面板服务.h:831</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 抽象树读回已返回 | 专项源码静态类型与实际装配人工复核 |
| RCE1029 | R0321 | R0279 | <code>海中鱼巣/领域/控制面板服务.h:857</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1030 | R0321 | R0320 | <code>海中鱼巣/领域/控制面板服务.h:868</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1031 | R0322 | R0309 | <code>海中鱼巣/领域/控制面板服务.h:778</code> | direct-static-member | <code>未单独冻结；读取源码调用点</code> | 根类别相等 | 专项源码静态类型与实际装配人工复核 |
| RCE1032 | R0323 | R0303 | <code>海中鱼巣/领域/控制面板服务.h:809</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个概念根选项 | 专项源码静态类型与实际装配人工复核 |
| RCE1033 | R0324 | F0190 | <code>海中鱼巣/领域/控制面板服务.h:879</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1034 | R0324 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:883</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1035 | R0324 | R0312 | <code>海中鱼巣/领域/控制面板服务.h:891</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1036 | R0324 | R0327 | <code>海中鱼巣/领域/控制面板服务.h:896</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 形成关系角色人读文本 | 专项源码静态类型与实际装配人工复核 |
| RCE1037 | R0325 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:908, 海中鱼巣/领域/控制面板服务.h:911, 海中鱼巣/领域/控制面板服务.h:916</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1038 | R0325 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:908, 海中鱼巣/领域/控制面板服务.h:911, 海中鱼巣/领域/控制面板服务.h:916</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1039 | R0325 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:914</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1040 | R0325 | R0312 | <code>海中鱼巣/领域/控制面板服务.h:917</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1041 | R0325 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:920</code> | direct-recursive | <code>未单独冻结；读取源码调用点</code> | 对子节点递归规范化 | 专项源码静态类型与实际装配人工复核 |
| RCE1042 | R0326 | R0314 | <code>海中鱼巣/领域/控制面板服务.h:938</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1043 | R0326 | R0325 | <code>海中鱼巣/领域/控制面板服务.h:967</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1044 | R0328 | F0496 | <code>海中鱼巣/领域/控制面板服务.h:986</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1045 | R0328 | F0497 | <code>海中鱼巣/领域/控制面板服务.h:987</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1046 | R0328 | R0313 | <code>海中鱼巣/领域/控制面板服务.h:988, 海中鱼巣/领域/控制面板服务.h:989</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1047 | R0328 | R0324 | <code>海中鱼巣/领域/控制面板服务.h:991, 海中鱼巣/领域/控制面板服务.h:998</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1048 | R0328 | F0163 | <code>海中鱼巣/领域/控制面板服务.h:992, 海中鱼巣/领域/控制面板服务.h:999</code> | source-static-type | <code>未单独冻结；读取源码调用点</code> | 控制面板按需树路径；字段静态类型/重载专项复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1049 | R0328 | F0168 | <code>海中鱼巣/领域/控制面板服务.h:992, 海中鱼巣/领域/控制面板服务.h:999</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1050 | R0331 | R0333 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:135</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1051 | R0331 | F0231 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:138</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 四项入口前置通过后读取参数稳定键组 | 专项源码静态类型与实际装配人工复核 |
| RCE1052 | R0331 | R0407 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:142</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 逐个主键状态材料复核完整抽象状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1053 | R0331 | R0334 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:146</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1054 | R0331 | F0339 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:151</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1055 | R0331 | R0332 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:152</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1056 | R0331 | R0338 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:155</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1057 | R0331 | R0078 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:158</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1058 | R0331 | R0073 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:169</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 概念活动审计读取令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1059 | R0333 | R0542 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:198</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1060 | R0335 | F0231 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:216</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 函数进入后读取参数稳定键组并逐主键复核 | 专项源码静态类型与实际装配人工复核 |
| RCE1061 | R0335 | R0063 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:217, 海中鱼巣/领域/数据操作.概念活动.ixx:227</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1062 | R0335 | R0030 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:228</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1063 | R0335 | R0031 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:229</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1064 | R0335 | R0540 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:230</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1065 | R0335 | R0541 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:231</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1066 | R0335 | R0036 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:236</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1067 | R0335 | R0037 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:236</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1068 | R0335 | R0026 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:239</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1069 | R0336 | R0025 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:254</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 读取关系材料会话入口读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1070 | R0336 | R0038 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:259</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1071 | R0337 | R0034 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:270</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1072 | R0337 | R0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:280</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1073 | R0338 | R0540 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:293</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1074 | R0338 | R0541 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:294</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1075 | R0338 | F0441 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:298</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1076 | R0340 | R0339 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:126</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1077 | R0340 | R0333 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:57</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1078 | R0340 | R0332 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:59</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1079 | R0340 | R0116 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:63</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 外层入口前置通过并形成 std::function<void(结构写入会话&)> | 专项源码静态类型与实际装配人工复核 |
| RCE1080 | R0341 | R0337 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:110</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1081 | R0341 | R0041 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:119</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 候选材料完整后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1082 | R0341 | R0335 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:64</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1083 | R0341 | R0403 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:69, 海中鱼巣/领域/数据操作.概念活动.ixx:70, 海中鱼巣/领域/数据操作.概念活动.ixx:71</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1084 | R0341 | R0334 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:81, 海中鱼巣/领域/数据操作.概念活动.ixx:82, 海中鱼巣/领域/数据操作.概念活动.ixx:83</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1085 | R0341 | R0023 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:87</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 全新前置及三项状态写入成功后创建根组关系 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1086 | R0341 | R0336 | <code>海中鱼巣/领域/数据操作.概念活动.ixx:96</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1087 | R0343 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:104, 海中鱼巣/领域/数据操作.特征体系.ixx:105</code> | direct | <code>未单独冻结；读取源码调用点</code> | 复核宿主、特征定义和实例槽位节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1088 | R0343 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:105</code> | direct | <code>未单独冻结；读取源码调用点</code> | 复核槽位主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1089 | R0343 | R0387 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:106</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 复核宿主关系和模板关系证据 | 专项源码静态类型与实际装配人工复核 |
| RCE1090 | R0344 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1028</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1091 | R0344 | R0345 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1035</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1092 | R0345 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1044</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1093 | R0346 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1054, 海中鱼巣/领域/数据操作.特征体系.ixx:1055</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1094 | R0347 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1073</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1095 | R0348 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1085</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1096 | R0348 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1086</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 身份静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1097 | R0348 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1094</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1098 | R0348 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1098</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1099 | R0348 | R0354 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1099</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1100 | R0349 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1112, 海中鱼巣/领域/数据操作.特征体系.ixx:1139</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1101 | R0349 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1113, 海中鱼巣/领域/数据操作.特征体系.ixx:1140</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 身份与宿主静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1102 | R0349 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1121</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1103 | R0349 | R0355 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1122, 海中鱼巣/领域/数据操作.特征体系.ixx:1131, 海中鱼巣/领域/数据操作.特征体系.ixx:1133, 海中鱼巣/领域/数据操作.特征体系.ixx:1136, 海中鱼巣/领域/数据操作.特征体系.ixx:1138, 海中鱼巣/领域/数据操作.特征体系.ixx:1141</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1104 | R0349 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1124</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1105 | R0349 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1130, 海中鱼巣/领域/数据操作.特征体系.ixx:1137</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1106 | R0349 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1132</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1107 | R0349 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1133</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1108 | R0349 | R0079 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1135</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1109 | R0349 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1140</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1110 | R0350 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1161, 海中鱼巣/领域/数据操作.特征体系.ixx:1177</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1111 | R0350 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1166</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1112 | R0350 | R0348 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1167</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1113 | R0350 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1168</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1114 | R0350 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1174</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1115 | R0350 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1175</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1116 | R0350 | R0355 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1176, 海中鱼巣/领域/数据操作.特征体系.ixx:1178, 海中鱼巣/领域/数据操作.特征体系.ixx:1184</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1117 | R0350 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1180</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1118 | R0350 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1182</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1119 | R0351 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1199</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位形参静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1120 | R0351 | R0078 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1203</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1121 | R0351 | R0356 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1209, 海中鱼巣/领域/数据操作.特征体系.ixx:1211, 海中鱼巣/领域/数据操作.特征体系.ixx:1214, 海中鱼巣/领域/数据操作.特征体系.ixx:1217, 海中鱼巣/领域/数据操作.特征体系.ixx:1224, 海中鱼巣/领域/数据操作.特征体系.ixx:1235, 海中鱼巣/领域/数据操作.特征体系.ixx:1238</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1122 | R0351 | R0346 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1210</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1123 | R0351 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1212</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1124 | R0351 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1213</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 读取身份结果静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1125 | R0351 | R0347 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1216</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1126 | R0351 | R0266 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1219</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 读取当前值时读取原始材料侧表 | 专项源码静态类型与实际装配人工复核 |
| RCE1127 | R0351 | R0268 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1224</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 侧表快照静态类型复核可读性 | 专项源码静态类型与实际装配人工复核 |
| RCE1128 | R0352 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:125, 海中鱼巣/领域/数据操作.特征体系.ixx:126</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1129 | R0353 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1311, 海中鱼巣/领域/数据操作.特征体系.ixx:1313</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1130 | R0353 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1313</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1131 | R0353 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1314, 海中鱼巣/领域/数据操作.特征体系.ixx:1316, 海中鱼巣/领域/数据操作.特征体系.ixx:1319, 海中鱼巣/领域/数据操作.特征体系.ixx:1321, 海中鱼巣/领域/数据操作.特征体系.ixx:1323</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1132 | R0353 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1314</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1133 | R0353 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1316</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1134 | R0353 | R0388 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1317</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 读取身份结果静态类型为特征节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1135 | R0353 | R0079 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1319</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1136 | R0353 | R0356 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1320, 海中鱼巣/领域/数据操作.特征体系.ixx:1325</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1137 | R0353 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1321</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1138 | R0353 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1322</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 读取槽位结果静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1139 | R0353 | R0357 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1322</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1140 | R0353 | R0351 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1323</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1141 | R0353 | R0352 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1324</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 输出静态类型为特征原始值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1142 | R0354 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1387</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1143 | R0355 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1395</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1144 | R0356 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1403</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1145 | R0358 | R0021 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1446</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1146 | R0358 | R0022 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1449</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话形参创建节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE1147 | R0358 | R0027 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1452</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话形参绑定索引请求 | 专项源码静态类型与实际装配人工复核 |
| RCE1148 | R0358 | R0129 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1452</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1149 | R0358 | R0036 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1454</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1150 | R0358 | R0037 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1454</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1151 | R0358 | R0040 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1455</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1152 | R0359 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1462</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 定义形参静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1153 | R0359 | R0030 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1463, 海中鱼巣/领域/数据操作.特征体系.ixx:1465</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1154 | R0359 | R0031 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1464, 海中鱼巣/领域/数据操作.特征体系.ixx:1466</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1155 | R0359 | R0040 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1467</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1156 | R0359 | R0034 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1468</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1157 | R0360 | R0034 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1475, 海中鱼巣/领域/数据操作.特征体系.ixx:1478</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1158 | R0360 | R0030 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1476</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1159 | R0361 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1505</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1160 | R0361 | R0371 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1506, 海中鱼巣/领域/数据操作.特征体系.ixx:1515</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取幂等主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1161 | R0361 | R0023 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1507, 海中鱼巣/领域/数据操作.特征体系.ixx:1510</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话形参创建槽位关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1162 | R0361 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1508, 海中鱼巣/领域/数据操作.特征体系.ixx:1516</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取宿主 | 专项源码静态类型与实际装配人工复核 |
| RCE1163 | R0361 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1511, 海中鱼巣/领域/数据操作.特征体系.ixx:1517</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取特征定义 | 专项源码静态类型与实际装配人工复核 |
| RCE1164 | R0361 | R0038 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1513</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1165 | R0362 | R0358 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1533</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1166 | R0362 | R0375 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1534, 海中鱼巣/领域/数据操作.特征体系.ixx:1549</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取幂等主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1167 | R0362 | R0023 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1535</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话形参创建当前值关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1168 | R0362 | R0376 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1537, 海中鱼巣/领域/数据操作.特征体系.ixx:1540, 海中鱼巣/领域/数据操作.特征体系.ixx:1544, 海中鱼巣/领域/数据操作.特征体系.ixx:1554</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取原始类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1169 | R0362 | R0028 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1538</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1170 | R0362 | R0377 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1538, 海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1545, 海中鱼巣/领域/数据操作.特征体系.ixx:1556</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 I64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1171 | R0362 | R0378 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1557</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 VecI64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1172 | R0362 | R0379 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1541, 海中鱼巣/领域/数据操作.特征体系.ixx:1558</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 VecU64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1173 | R0362 | R0270 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1542</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1174 | R0362 | R0038 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1543</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1175 | R0362 | R0029 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1545</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1176 | R0363 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1565</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 材料形参静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1177 | R0363 | R0371 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1565</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取幂等主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1178 | R0363 | R0372 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1566</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取宿主 | 专项源码静态类型与实际装配人工复核 |
| RCE1179 | R0363 | R0373 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1567</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型读取特征定义 | 专项源码静态类型与实际装配人工复核 |
| RCE1180 | R0364 | R0352 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1573</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 材料形参静态类型为特征原始值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1181 | R0364 | R0375 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1573</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取幂等主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1182 | R0364 | R0376 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1574</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取原始类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1183 | R0364 | R0377 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1575</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 I64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1184 | R0364 | R0378 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1576</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 VecI64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1185 | R0364 | R0379 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1577</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型读取 VecU64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1186 | R0368 | F0184 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:1650, 海中鱼巣/领域/数据操作.特征体系.ixx:1653, 海中鱼巣/领域/数据操作.特征体系.ixx:1656</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1187 | R0374 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:273</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1188 | R0381 | R0329 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:480</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 虚拟存在写入规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1189 | R0382 | R0329 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:493</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 虚拟存在写入规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1190 | R0383 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:564</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1191 | R0383 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:564, 海中鱼巣/领域/数据操作.特征体系.ixx:566</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1192 | R0383 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:566</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1193 | R0383 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:567</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1194 | R0383 | R0344 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:567</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1195 | R0384 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:594, 海中鱼巣/领域/数据操作.特征体系.ixx:596</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1196 | R0384 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:596</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1197 | R0384 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:597, 海中鱼巣/领域/数据操作.特征体系.ixx:599</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1198 | R0384 | F0441 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:597</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1199 | R0384 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:599</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1200 | R0385 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:603</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1201 | R0385 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:603, 海中鱼巣/领域/数据操作.特征体系.ixx:605</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1202 | R0385 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:605</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1203 | R0385 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:606</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1204 | R0385 | R0350 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:606</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1205 | R0386 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:610</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1206 | R0386 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:610, 海中鱼巣/领域/数据操作.特征体系.ixx:612</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1207 | R0386 | F0338 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:612</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1208 | R0386 | F0339 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:613, 海中鱼巣/领域/数据操作.特征体系.ixx:615</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1209 | R0386 | R0349 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:613</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1210 | R0386 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:614</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1211 | R0386 | R0357 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:614</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1212 | R0386 | R0351 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:615</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1213 | R0387 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:64</code> | direct | <code>未单独冻结；读取源码调用点</code> | 特征关系证据复核源和目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1214 | R0387 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:64</code> | unique_name+direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；特征关系证据复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1215 | R0388 | F0168 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:76</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1216 | R0389 | F0444 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1217 | R0389 | R0374 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1218 | R0389 | R0380 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:835</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1219 | R0389 | R0383 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:837</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1220 | R0389 | R0390 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:839</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 定义静态类型为特征定义值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1221 | R0389 | R0366 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:840</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1222 | R0389 | R0342 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:842</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1223 | R0389 | R0385 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:844, 海中鱼巣/领域/数据操作.特征体系.ixx:860, 海中鱼巣/领域/数据操作.特征体系.ixx:900</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1224 | R0389 | R0343 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:846, 海中鱼巣/领域/数据操作.特征体系.ixx:862, 海中鱼巣/领域/数据操作.特征体系.ixx:902</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 写前与写后槽位静态类型为特征槽位值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1225 | R0389 | R0386 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:847, 海中鱼巣/领域/数据操作.特征体系.ixx:863, 海中鱼巣/领域/数据操作.特征体系.ixx:903</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1226 | R0389 | R0363 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:848, 海中鱼巣/领域/数据操作.特征体系.ixx:865, 海中鱼巣/领域/数据操作.特征体系.ixx:905</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1227 | R0389 | R0364 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:848, 海中鱼巣/领域/数据操作.特征体系.ixx:865, 海中鱼巣/领域/数据操作.特征体系.ixx:905</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1228 | R0389 | R0365 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:849, 海中鱼巣/领域/数据操作.特征体系.ixx:866, 海中鱼巣/领域/数据操作.特征体系.ixx:906</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1229 | R0389 | R0367 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:854</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1230 | R0389 | R0384 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:856</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1231 | R0389 | R0353 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:857</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1232 | R0389 | R0269 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:884</code> | constructor | <code>未单独冻结；读取源码调用点</code> | 创建槽位并发布初始状态时构造本次原始材料事务参与者 | 专项源码静态类型与实际装配人工复核 |
| RCE1233 | R0389 | R0103 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:885</code> | direct-template-member | <code>未单独冻结；读取源码调用点</code> | 外层入口前置通过并形成结构写入会话回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1234 | R0389 | R0368 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:915</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1235 | R0390 | F0163 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:88</code> | direct | <code>未单独冻结；读取源码调用点</code> | 复核特征定义节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1236 | R0390 | F0565 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:88</code> | direct | <code>未单独冻结；读取源码调用点</code> | 复核特征定义主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1237 | R0391 | R0359 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:886</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1238 | R0391 | R0360 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:887</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1239 | R0391 | R0361 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:893</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1240 | R0391 | R0362 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:895</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1241 | R0391 | R0041 | <code>海中鱼巣/领域/数据操作.特征体系.ixx:897</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | lambda 形参静态类型为结构写入会话 | 专项源码静态类型与实际装配人工复核 |
| RCE1242 | R0392 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1014</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1243 | R0392 | R0394 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1021</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1244 | R0393 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:103</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1245 | R0394 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1030</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1246 | R0395 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1040, 海中鱼巣/领域/数据操作.状态动态.ixx:1041</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1247 | R0396 | R0392 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1058, 海中鱼巣/领域/数据操作.状态动态.ixx:1110</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1248 | R0396 | R0397 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1069, 海中鱼巣/领域/数据操作.状态动态.ixx:1074, 海中鱼巣/领域/数据操作.状态动态.ixx:1088, 海中鱼巣/领域/数据操作.状态动态.ixx:1098, 海中鱼巣/领域/数据操作.状态动态.ixx:1102, 海中鱼巣/领域/数据操作.状态动态.ixx:1109, 海中鱼巣/领域/数据操作.状态动态.ixx:1111, 海中鱼巣/领域/数据操作.状态动态.ixx:1113, 海中鱼巣/领域/数据操作.状态动态.ixx:1116, 海中鱼巣/领域/数据操作.状态动态.ixx:1121, 海中鱼巣/领域/数据操作.状态动态.ixx:1129, 海中鱼巣/领域/数据操作.状态动态.ixx:1130, 海中鱼巣/领域/数据操作.状态动态.ixx:1133</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1249 | R0396 | R0082 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1087</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1250 | R0396 | R0408 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1100</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1251 | R0396 | R0078 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1107, 海中鱼巣/领域/数据操作.状态动态.ixx:1125</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1252 | R0396 | R0395 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1108, 海中鱼巣/领域/数据操作.状态动态.ixx:1128</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1253 | R0397 | F0184 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:1326</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1254 | R0398 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:159</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1255 | R0401 | R0393 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:306</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 状态写入参与结果材料静态类型复核实例状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1256 | R0401 | R0407 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:306</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 状态写入参与结果材料静态类型复核抽象状态完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1257 | R0402 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:377</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 入口复核状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1258 | R0402 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:377</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1259 | R0402 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:377, 海中鱼巣/领域/数据操作.状态动态.ixx:379</code> | suffix_match+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；读取状态材料入口复核数据操作有效 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1260 | R0402 | F0397 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:378</code> | resolved-function-pointer | <code>未单独冻结；读取源码调用点</code> | 生产运行期唯一共享许可目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1261 | R0402 | F0338 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:379</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 取得共享许可后复核有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1262 | R0402 | F0339 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:380</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；许可有效后读取令牌 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1263 | R0402 | R0396 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:380</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；调用已许可状态材料读取 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1264 | R0403 | F0443 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:409</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1265 | R0403 | R0405 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:411</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1266 | R0403 | R0400 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:412</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 规格完整后读取状态值作为结构写入实参 | 专项源码静态类型与实际装配人工复核 |
| RCE1267 | R0404 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:60</code> | direct | <code>未单独冻结；读取源码调用点</code> | 关系值式证据复核源和目标节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1268 | R0404 | F0168 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:60</code> | unique_name+direct | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；关系值式证据复核关系句柄 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1269 | R0405 | R0021 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:902</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1270 | R0405 | R0028 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:905</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1271 | R0405 | R0022 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:906</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 主键和主信息候选写入成功后创建节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE1272 | R0405 | R0027 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:909</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 节点候选成功后绑定主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1273 | R0405 | R0129 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:909</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1274 | R0405 | R0036 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:911</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1275 | R0405 | R0029 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:912</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1276 | R0405 | R0037 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:913</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1277 | R0405 | R0040 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:914</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1278 | R0405 | R0407 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:923</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 抽象状态结构候选读回完成后复核输出完整抽象状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1279 | R0406 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:92</code> | direct | <code>未单独冻结；读取源码调用点</code> | 当前可读复核状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1280 | R0406 | F0565 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:92</code> | direct | <code>未单独冻结；读取源码调用点</code> | 当前可读复核主信息句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1281 | R0407 | R0406 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:96</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 抽象状态完整性首先复核当前可读 | 专项源码静态类型与实际装配人工复核 |
| RCE1282 | R0407 | F0163 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:97</code> | direct | <code>未单独冻结；读取源码调用点</code> | 抽象状态要求场景和主体节点句柄均无效 | 专项源码静态类型与实际装配人工复核 |
| RCE1283 | R0407 | R0404 | <code>海中鱼巣/领域/数据操作.状态动态.ixx:98</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 抽象状态要求三份关系值式证据均不完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1284 | R0409 | R0116 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:132</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 外层入口前置通过并形成 std::function<void(结构写入会话&)> | 专项源码静态类型与实际装配人工复核 |
| RCE1285 | R0409 | R0421 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:172</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1286 | R0410 | R0415 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:133</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1287 | R0410 | R0417 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:134</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1288 | R0410 | R0418 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:139, 海中鱼巣/领域/数据操作.系统角色.ixx:145</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1289 | R0410 | R0041 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:152</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 世界拓扑结构发生变化后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1290 | R0413 | F0441 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:237</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1291 | R0414 | R0030 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:247</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1292 | R0414 | R0031 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:248</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1293 | R0414 | R0036 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:249</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1294 | R0414 | R0033 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:250</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 身份、类型、主信息和可读校验成立后读取节点主键组 | 专项源码静态类型与实际装配人工复核 |
| RCE1295 | R0414 | R0040 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:252</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1296 | R0415 | R0564 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:258</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1297 | R0415 | R0414 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:259</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1298 | R0417 | R0025 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:285</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 系统角色关系材料完整后读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1299 | R0417 | R0038 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:289</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1300 | R0418 | R0035 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:298</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1301 | R0418 | R0038 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:311, 海中鱼巣/领域/数据操作.系统角色.ixx:329</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1302 | R0418 | R0024 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:319</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 来源普通父子关系组为空时挂载或重挂节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1303 | R0418 | R0025 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:325</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 挂载结果已创建且父关系存在后读取关系审计 | 专项源码静态类型与实际装配人工复核 |
| RCE1304 | R0419 | R0079 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:342</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1305 | R0419 | R0416 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:351</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1306 | R0420 | R0078 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:360</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1307 | R0421 | F0339 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:378</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1308 | R0421 | R0564 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:379</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1309 | R0421 | R0413 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:380</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1310 | R0421 | R0420 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:384, 海中鱼巣/领域/数据操作.系统角色.ixx:388</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1311 | R0421 | R0416 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:392</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1312 | R0421 | R0419 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:398, 海中鱼巣/领域/数据操作.系统角色.ixx:400</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1313 | R0422 | R0411 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:45</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1314 | R0422 | F0339 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:51</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1315 | R0422 | F0441 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:53</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1316 | R0422 | R0412 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:76, 海中鱼巣/领域/数据操作.系统角色.ixx:77, 海中鱼巣/领域/数据操作.系统角色.ixx:78, 海中鱼巣/领域/数据操作.系统角色.ixx:79, 海中鱼巣/领域/数据操作.系统角色.ixx:97</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1317 | R0422 | R0420 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:88, 海中鱼巣/领域/数据操作.系统角色.ixx:91</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1318 | R0422 | R0079 | <code>海中鱼巣/领域/数据操作.系统角色.ixx:98</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1319 | R0423 | F0446 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:126, 海中鱼巣/领域/数据操作.轻量因果.ixx:128</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1320 | R0423 | F0339 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:129, 海中鱼巣/领域/数据操作.轻量因果.ixx:131</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1321 | R0423 | F0441 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:129</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1322 | R0423 | R0425 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:131</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1323 | R0424 | F0441 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:208</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1324 | R0425 | R0426 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:230, 海中鱼巣/领域/数据操作.轻量因果.ixx:234</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1325 | R0425 | R0424 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:232</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1326 | R0426 | F0184 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:243</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1327 | R0428 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:112</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1328 | R0429 | F0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1111, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1113</code> | suffix_match+source-audited | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；无对象限定调用当前数据操作::有效 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1329 | R0429 | F0338 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1113</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 共享许可静态类型为结构事务许可 | 专项源码静态类型与实际装配人工复核 |
| RCE1330 | R0429 | F0339 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1114, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1116</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1331 | R0429 | F0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1114</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1332 | R0429 | R0442 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1116</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1333 | R0430 | F0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1160, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1162</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1334 | R0430 | F0339 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1163, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1165</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1335 | R0430 | F0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1163</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1336 | R0430 | R0448 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1165</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1337 | R0431 | R0454 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1214</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1338 | R0432 | R0430 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1621, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1649</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1339 | R0432 | R0460 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1623, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1650</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1340 | R0432 | R0461 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1628</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1341 | R0432 | R0116 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1632</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 外层入口前置通过并形成 std::function<void(结构写入会话&)> | 专项源码静态类型与实际装配人工复核 |
| RCE1342 | R0432 | R0462 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1658</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1343 | R0433 | R0451 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1634</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1344 | R0433 | R0403 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1635, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1636, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1637</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1345 | R0433 | R0452 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1643, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1644, 海中鱼巣/领域/数据操作.需求任务方法.ixx:1645</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项复核改挂到非平凡业务 lambda |
| RCE1346 | R0433 | R0036 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1646</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1347 | R0433 | R0037 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1646</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1348 | R0433 | R0040 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1647</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；专项复核改挂到非平凡业务 lambda |
| RCE1349 | R0433 | R0041 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1647</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 主信息、节点和主键绑定读回匹配后请求提交 | 专项源码静态类型与实际装配人工复核；专项复核改挂到非平凡业务 lambda |
| RCE1350 | R0434 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:191, 海中鱼巣/领域/数据操作.需求任务方法.ixx:192, 海中鱼巣/领域/数据操作.需求任务方法.ixx:193, 海中鱼巣/领域/数据操作.需求任务方法.ixx:194</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1351 | R0435 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:1940</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1352 | R0440 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2005, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2029</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1353 | R0440 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2006, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2030</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 身份与目标静态类型均为高级节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1354 | R0440 | R0438 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2019</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1355 | R0440 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2026, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2046</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1356 | R0440 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2027, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2049</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1357 | R0440 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2032, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2036</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1358 | R0440 | R0439 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2066, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2067, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2068</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1359 | R0441 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2080, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2097, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2125, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2139</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1360 | R0441 | R0440 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2085</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1361 | R0441 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2095, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2123, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2137</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1362 | R0441 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2101, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2126, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2140</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1363 | R0441 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2104, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2109, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2114, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2131, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2145</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1364 | R0441 | R0444 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2171</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1365 | R0442 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2188, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2206, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2223, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2235, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2271, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2282</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1366 | R0442 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2203, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2233, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2259, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2281</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1367 | R0442 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2204, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2224, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2236, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2262, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2272</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1368 | R0442 | R0443 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2205, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2207, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2209, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2211, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2213, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2215, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2217, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2227, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2239, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2244, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2260, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2263, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2269, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2274, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2283, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2285, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2298</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1369 | R0442 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2209, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2211, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2213, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2215, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2217, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2226, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2238</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1370 | R0442 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2221, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2268</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1371 | R0442 | R0441 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2290</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1372 | R0444 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2317, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2334, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2351</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1373 | R0444 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2333, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2349, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2358, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2387</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1374 | R0444 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2335, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2352, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2367, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2392</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1375 | R0444 | R0445 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2336, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2338, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2340, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2342, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2344, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2346, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2355, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2360, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2362, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2368, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2370, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2389, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2393, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2396</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1376 | R0444 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2338, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2340, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2342, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2344, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2346, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2354, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2368</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1377 | R0444 | R0446 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2361</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1378 | R0444 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2365</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1379 | R0444 | R0447 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2394</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1380 | R0446 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2412</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1381 | R0446 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2413</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1382 | R0447 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2456</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1383 | R0447 | R0428 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2457</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 读取身份结果静态类型为高级节点身份 | 专项源码静态类型与实际装配人工复核 |
| RCE1384 | R0448 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2517</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1385 | R0448 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2529</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1386 | R0448 | R0436 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2530</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1387 | R0448 | R0450 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2531</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1388 | R0448 | R0449 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2532, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2534, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2536, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2538, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2541</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1389 | R0448 | R0437 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2534, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2536, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2538</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1390 | R0450 | R0435 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2759</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1391 | R0450 | R0078 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2761</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1392 | R0450 | R0079 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2762</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1393 | R0451 | R0021 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2778</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1394 | R0451 | R0022 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2784</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 主信息候选成功且未命中故障后创建节点候选 | 专项源码静态类型与实际装配人工复核 |
| RCE1395 | R0451 | R0027 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2790</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 节点候选成功后绑定主键 | 专项源码静态类型与实际装配人工复核 |
| RCE1396 | R0451 | R0129 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2790</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1397 | R0452 | R0023 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2809</code> | direct-member | <code>未单独冻结；读取源码调用点</code> | 追加关系会话入口创建关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1398 | R0452 | R0038 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2817</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1399 | R0453 | R0452 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2834, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2840, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2846, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2853, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2858, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2863, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2869</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1400 | R0454 | R0466 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2882</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 规格形参静态类型为完整需求写入规格 | 专项源码静态类型与实际装配人工复核 |
| RCE1401 | R0454 | R0429 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2883, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2917</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1402 | R0454 | R0459 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2885, 海中鱼巣/领域/数据操作.需求任务方法.ixx:2918</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1403 | R0454 | R0463 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2889</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1404 | R0454 | R0116 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2892</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 外层入口前置通过并形成结构写入会话回调 | 专项源码静态类型与实际装配人工复核 |
| RCE1405 | R0454 | R0464 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2926</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1406 | R0455 | R0458 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2893</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1407 | R0455 | R0403 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2895</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1408 | R0455 | R0401 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2896</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | lambda 内状态写入结果静态类型为状态写入参与结果 | 专项源码静态类型与实际装配人工复核 |
| RCE1409 | R0455 | R0451 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2901</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1410 | R0455 | R0453 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2907</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1411 | R0455 | R0036 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2914</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1412 | R0455 | R0037 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2914</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1413 | R0455 | R0040 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2915</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核；机械发现后经源码范围改挂到非平凡结构写入 lambda |
| RCE1414 | R0455 | R0041 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:2915</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | lambda 形参静态类型为结构写入会话 | 专项源码静态类型与实际装配人工复核 |
| RCE1415 | R0456 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:293, 海中鱼巣/领域/数据操作.需求任务方法.ixx:294</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1416 | R0457 | R0030 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3148</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1417 | R0457 | R0034 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3149</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1418 | R0457 | R0035 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3150</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1419 | R0457 | R0031 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3151</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1420 | R0457 | R0032 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3152</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 会话形参读取 I64 值 | 专项源码静态类型与实际装配人工复核 |
| RCE1421 | R0458 | R0030 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3306, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3307, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3308, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3309, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3310</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1422 | R0458 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3311</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1423 | R0458 | R0457 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3312</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1424 | R0459 | R0434 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3382</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1425 | R0460 | R0456 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3468</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 当前静态类型为方法登记根材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1426 | R0460 | R0402 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3469, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3470, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3471</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 状态数据操作字段静态类型为状态动态数据操作& | 专项源码静态类型与实际装配人工复核 |
| RCE1427 | R0460 | R0407 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3472</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 角色/活跃/失效返回类型均为状态值式材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1428 | R0460 | R0399 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3473, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3474, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3475</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 三字段静态类型均为抽象状态写入规格 | 专项源码静态类型与实际装配人工复核 |
| RCE1429 | R0460 | R0400 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3476, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3477, 海中鱼巣/领域/数据操作.需求任务方法.ixx:3478</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 三份规格字段静态类型为抽象状态写入规格 | 专项源码静态类型与实际装配人工复核 |
| RCE1430 | R0463 | R0461 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3591</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1431 | R0464 | R0462 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:3599</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1432 | R0466 | F0168 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:449, 海中鱼巣/领域/数据操作.需求任务方法.ixx:450, 海中鱼巣/领域/数据操作.需求任务方法.ixx:451</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1433 | R0467 | R0329 | <code>海中鱼巣/领域/数据操作.需求任务方法.ixx:893</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 虚拟存在写入规格静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1434 | R0468 | R0479 | <code>海中鱼巣/领域/方法服务.h:1009</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1435 | R0468 | R0489 | <code>海中鱼巣/领域/方法服务.h:1012</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1436 | R0468 | R0494 | <code>海中鱼巣/领域/方法服务.h:1013</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1437 | R0468 | R0490 | <code>海中鱼巣/领域/方法服务.h:1020</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1438 | R0468 | R0491 | <code>海中鱼巣/领域/方法服务.h:1021</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1439 | R0469 | R0479 | <code>海中鱼巣/领域/方法服务.h:1046</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1440 | R0469 | F0330 | <code>海中鱼巣/领域/方法服务.h:1049</code> | direct-const-member | <code>this=&方法服务,方法首节点,关系类型::引用,节点类型::任务,来源任务顺序号</code> | 四参数读取来源任务；顺序号20 | 专项源码静态类型与实际装配人工复核 |
| RCE1441 | R0470 | R0479 | <code>海中鱼巣/领域/方法服务.h:1063</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1442 | R0470 | F0330 | <code>海中鱼巣/领域/方法服务.h:1066</code> | direct-const-member | <code>this=&方法服务,方法首节点,关系类型::引用,节点类型::方法,父方法顺序号</code> | 四参数读取父方法；顺序号21 | 专项源码静态类型与实际装配人工复核 |
| RCE1443 | R0471 | R0483 | <code>海中鱼巣/领域/方法服务.h:1079</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1444 | R0472 | R0483 | <code>海中鱼巣/领域/方法服务.h:1092</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1445 | R0473 | R0479 | <code>海中鱼巣/领域/方法服务.h:1097</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1446 | R0473 | R0469 | <code>海中鱼巣/领域/方法服务.h:1100</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1447 | R0473 | R0470 | <code>海中鱼巣/领域/方法服务.h:1101</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1448 | R0473 | R0471 | <code>海中鱼巣/领域/方法服务.h:1102</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1449 | R0473 | R0472 | <code>海中鱼巣/领域/方法服务.h:1103</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1450 | R0474 | F0330 | <code>海中鱼巣/领域/方法服务.h:1210</code> | direct-const-member | <code>this=&方法服务,动作入口节点,关系类型::引用,节点类型::场景,动作输入规格顺序号</code> | 四参数读取动作输入规格；顺序号1 | 专项源码静态类型与实际装配人工复核 |
| RCE1451 | R0475 | F0330 | <code>海中鱼巣/领域/方法服务.h:1214</code> | direct-const-member | <code>this=&方法服务,动作入口节点,关系类型::引用,节点类型::场景,动作输出规格顺序号</code> | 四参数读取动作输出规格；顺序号2 | 专项源码静态类型与实际装配人工复核 |
| RCE1452 | R0476 | F0327 | <code>海中鱼巣/领域/方法服务.h:1358</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1453 | R0476 | F0629 | <code>海中鱼巣/领域/方法服务.h:1359</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法首基础结构读取虚拟存在 | 专项源码静态类型与实际装配人工复核 |
| RCE1454 | R0477 | F0160 | <code>海中鱼巣/领域/方法服务.h:1365</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1455 | R0477 | R0476 | <code>海中鱼巣/领域/方法服务.h:1365</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1456 | R0477 | F0575 | <code>海中鱼巣/领域/方法服务.h:1377, 海中鱼巣/领域/方法服务.h:1394</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 读取登记关系组；无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1457 | R0477 | R0478 | <code>海中鱼巣/领域/方法服务.h:1423</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法登记项材料已形成 | 专项源码静态类型与实际装配人工复核 |
| RCE1458 | R0478 | F0163 | <code>海中鱼巣/领域/方法服务.h:157, 海中鱼巣/领域/方法服务.h:158</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 登记项节点句柄字段完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1459 | R0478 | F0168 | <code>海中鱼巣/领域/方法服务.h:157, 海中鱼巣/领域/方法服务.h:158, 海中鱼巣/领域/方法服务.h:159, 海中鱼巣/领域/方法服务.h:160</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1460 | R0478 | F0565 | <code>海中鱼巣/领域/方法服务.h:159, 海中鱼巣/领域/方法服务.h:160</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 登记项关系句柄字段完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1461 | R0479 | R0488 | <code>海中鱼巣/领域/方法服务.h:1669</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1462 | R0480 | R0479 | <code>海中鱼巣/领域/方法服务.h:1674</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1463 | R0480 | F0327 | <code>海中鱼巣/领域/方法服务.h:1675</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1464 | R0480 | F0579 | <code>海中鱼巣/领域/方法服务.h:1676</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 条件节点准入；无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1465 | R0480 | F0629 | <code>海中鱼巣/领域/方法服务.h:1677</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 条件节点关系存在；三参数读取唯一目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1466 | R0481 | R0479 | <code>海中鱼巣/领域/方法服务.h:1681</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1467 | R0481 | F0327 | <code>海中鱼巣/领域/方法服务.h:1682</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1468 | R0481 | F0579 | <code>海中鱼巣/领域/方法服务.h:1683</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结果节点准入；无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1469 | R0481 | F0629 | <code>海中鱼巣/领域/方法服务.h:1684</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结果节点关系存在；三参数读取唯一目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1470 | R0482 | F0334 | <code>海中鱼巣/领域/方法服务.h:1690</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1471 | R0482 | R0479 | <code>海中鱼巣/领域/方法服务.h:1690</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1472 | R0482 | F0621 | <code>海中鱼巣/领域/方法服务.h:1693</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 读取方法角色节点；二实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1473 | R0482 | F0327 | <code>海中鱼巣/领域/方法服务.h:1695</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1474 | R0482 | F0333 | <code>海中鱼巣/领域/方法服务.h:1695</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 逐个目标验证节点类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1475 | R0482 | R0480 | <code>海中鱼巣/领域/方法服务.h:1698</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1476 | R0482 | R0481 | <code>海中鱼巣/领域/方法服务.h:1699</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1477 | R0483 | R0479 | <code>海中鱼巣/领域/方法服务.h:1735, 海中鱼巣/领域/方法服务.h:1740</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1478 | R0483 | F0620 | <code>海中鱼巣/领域/方法服务.h:1738</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 按顺序读取方法关系目标；三实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1479 | R0485 | F0327 | <code>海中鱼巣/领域/方法服务.h:1807</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1480 | R0485 | R0487 | <code>海中鱼巣/领域/方法服务.h:1808</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1481 | R0486 | R0479 | <code>海中鱼巣/领域/方法服务.h:1812</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1482 | R0486 | R0485 | <code>海中鱼巣/领域/方法服务.h:1813</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1483 | R0486 | F0579 | <code>海中鱼巣/领域/方法服务.h:1815</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 动作入口关系准入；无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1484 | R0487 | F0327 | <code>海中鱼巣/领域/方法服务.h:1819</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1485 | R0487 | F0330 | <code>海中鱼巣/领域/方法服务.h:1822</code> | direct-const-member | <code>this=&方法服务,动作入口节点,关系类型::模板,节点类型::状态,动作入口状态顺序号</code> | 四参数读取动作入口状态；顺序号1 | 专项源码静态类型与实际装配人工复核 |
| RCE1486 | R0487 | F0329 | <code>海中鱼巣/领域/方法服务.h:1826</code> | direct-const-member | <code>this=&状态,状态节点.value()</code> | 四参数唯一状态目标存在 | 专项源码静态类型与实际装配人工复核 |
| RCE1487 | R0487 | R0484 | <code>海中鱼巣/领域/方法服务.h:1831</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1488 | R0488 | R0477 | <code>海中鱼巣/领域/方法服务.h:455</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1489 | R0489 | R0479 | <code>海中鱼巣/领域/方法服务.h:871</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1490 | R0489 | F0629 | <code>海中鱼巣/领域/方法服务.h:874</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 方法基础结构完整；三参数读取唯一目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1491 | R0490 | R0482 | <code>海中鱼巣/领域/方法服务.h:878</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1492 | R0491 | R0482 | <code>海中鱼巣/领域/方法服务.h:882</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1493 | R0492 | R0480 | <code>海中鱼巣/领域/方法服务.h:903</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1494 | R0492 | F0629 | <code>海中鱼巣/领域/方法服务.h:906</code> | source-audited | <code>未单独冻结；读取源码调用点</code> | 方法条件节点前置校验通过 | 专项源码静态类型与实际装配人工复核 |
| RCE1495 | R0493 | R0481 | <code>海中鱼巣/领域/方法服务.h:911</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1496 | R0493 | F0629 | <code>海中鱼巣/领域/方法服务.h:914</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 结果节点有效；三参数读取唯一目标 | 专项源码静态类型与实际装配人工复核 |
| RCE1497 | R0494 | R0479 | <code>海中鱼巣/领域/方法服务.h:947</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1498 | R0494 | F0621 | <code>海中鱼巣/领域/方法服务.h:950</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 读取动作入口；二实参无令牌重载 | 专项源码静态类型与实际装配人工复核 |
| RCE1499 | R0494 | R0486 | <code>海中鱼巣/领域/方法服务.h:953</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1500 | R0494 | R0487 | <code>海中鱼巣/领域/方法服务.h:956</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1501 | R0497 | R0430 | <code>海中鱼巣/领域/服务.方法.ixx:214</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1502 | R0499 | R0427 | <code>海中鱼巣/领域/服务.方法.ixx:100</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1503 | R0499 | R0432 | <code>海中鱼巣/领域/服务.方法.ixx:104</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1504 | R0499 | R0498 | <code>海中鱼巣/领域/服务.方法.ixx:98</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1505 | R0502 | F0168 | <code>海中鱼巣/领域/服务.特征.ixx:118, 海中鱼巣/领域/服务.特征.ixx:119</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1506 | R0502 | R0504 | <code>海中鱼巣/领域/服务.特征.ixx:121</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1507 | R0502 | R0390 | <code>海中鱼巣/领域/服务.特征.ixx:122</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 定义材料静态类型复核完整 | 专项源码静态类型与实际装配人工复核 |
| RCE1508 | R0502 | R0507 | <code>海中鱼巣/领域/服务.特征.ixx:123</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1509 | R0502 | R0505 | <code>海中鱼巣/领域/服务.特征.ixx:125</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1510 | R0502 | R0381 | <code>海中鱼巣/领域/服务.特征.ixx:126</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1511 | R0503 | R0506 | <code>海中鱼巣/领域/服务.特征.ixx:138</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1512 | R0503 | R0382 | <code>海中鱼巣/领域/服务.特征.ixx:139</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1513 | R0504 | F0168 | <code>海中鱼巣/领域/服务.特征.ixx:233</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1514 | R0512 | R0423 | <code>海中鱼巣/领域/服务.轻量因果.ixx:52</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1515 | R0514 | R0431 | <code>海中鱼巣/领域/服务.需求.ixx:104</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 写入规格有值后创建完整目标状态需求 | 专项源码静态类型与实际装配人工复核 |
| RCE1516 | R0514 | F0168 | <code>海中鱼巣/领域/服务.需求.ixx:91, 海中鱼巣/领域/服务.需求.ixx:92, 海中鱼巣/领域/服务.需求.ixx:93</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1517 | R0514 | R0511 | <code>海中鱼巣/领域/服务.需求.ixx:97</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 状态服务字段形成目标抽象状态规格 | 专项源码静态类型与实际装配人工复核 |
| RCE1518 | R0514 | R0510 | <code>海中鱼巣/领域/服务.需求.ixx:98</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 抽象状态规格结果静态类型复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE1519 | R0514 | R0513 | <code>海中鱼巣/领域/服务.需求.ixx:98</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1520 | R0514 | R0467 | <code>海中鱼巣/领域/服务.需求.ixx:99</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；状态规格成功后形成完整需求写入规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1521 | R0517 | R0525 | <code>海中鱼巣/领域/概念图服务.h:2438</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1690 | R0523 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3775</code> | operator | <code>未单独冻结；读取源码调用点</code> | 逐份签名材料比较概念节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1525 | R0524 | R0081 | <code>海中鱼巣/领域/概念图服务.h:3941</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1526 | R0524 | F0184 | <code>海中鱼巣/领域/概念图服务.h:3943, 海中鱼巣/领域/概念图服务.h:3947, 海中鱼巣/领域/概念图服务.h:3958</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1527 | R0524 | F0168 | <code>海中鱼巣/领域/概念图服务.h:3958</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1528 | R0525 | R0523 | <code>海中鱼巣/领域/概念图服务.h:4342</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1529 | R0525 | F0184 | <code>海中鱼巣/领域/概念图服务.h:4344, 海中鱼巣/领域/概念图服务.h:4357, 海中鱼巣/领域/概念图服务.h:4387, 海中鱼巣/领域/概念图服务.h:4395</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1530 | R0525 | R0528 | <code>海中鱼巣/领域/概念图服务.h:4395</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 抽象树遍历正常完成后最终追根因检查；视图静态类型为抽象树视图材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1531 | R0526 | R0524 | <code>海中鱼巣/领域/概念图服务.h:4402</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1532 | R0527 | F0163 | <code>海中鱼巣/领域/概念图服务.h:666</code> | direct | <code>未单独冻结；读取源码调用点</code> | 抽象树投影项复核概念节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1533 | R0527 | F0051 | <code>海中鱼巣/领域/概念图服务.h:670</code> | operator | <code>未单独冻结；读取源码调用点</code> | 比较投影路径末尾节点与概念节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1534 | R0528 | F0163 | <code>海中鱼巣/领域/概念图服务.h:685</code> | direct | <code>未单独冻结；读取源码调用点</code> | 复核根概念节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1535 | R0528 | F0051 | <code>海中鱼巣/领域/概念图服务.h:687</code> | operator | <code>未单独冻结；读取源码调用点</code> | 比较首个投影项概念与根概念 | 专项源码静态类型与实际装配人工复核 |
| RCE1536 | R0528 | R0529 | <code>海中鱼巣/领域/概念图服务.h:688</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::all_of 注册局部投影项完整性 lambda | 专项源码静态类型与实际装配人工复核 |
| RCE1537 | R0529 | R0527 | <code>海中鱼巣/领域/概念图服务.h:689</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 逐个抽象树投影项复核完整性 | 专项源码静态类型与实际装配人工复核 |
| RCE1691 | R0531 | F0051 | <code>海中鱼巣/领域/概念图服务.h:922</code> | operator | <code>未单独冻结；读取源码调用点</code> | 概念根登记槽有值后比较登记根节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1538 | R0532 | R0534 | <code>海中鱼巣/领域/概念图算法.h:233</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1539 | R0532 | R0535 | <code>海中鱼巣/领域/概念图算法.h:234</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1540 | R0533 | R0532 | <code>海中鱼巣/领域/概念图算法.h:254, 海中鱼巣/领域/概念图算法.h:255</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1541 | R0533 | R0537 | <code>海中鱼巣/领域/概念图算法.h:258</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1542 | R0535 | R0534 | <code>海中鱼巣/领域/概念图算法.h:632</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1543 | R0536 | R0535 | <code>海中鱼巣/领域/概念图算法.h:677</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1544 | R0537 | R0536 | <code>海中鱼巣/领域/概念图算法.h:689</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1545 | R0538 | F0467 | <code>海中鱼巣/领域/概念活动状态.数据.h:105</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 活跃角色静态类型为概念活动状态角色材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1546 | R0538 | R0563 | <code>海中鱼巣/领域/概念活动状态.数据.h:105</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 根身份静态类型为系统角色身份材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1547 | R0538 | R0540 | <code>海中鱼巣/领域/概念活动状态.数据.h:106</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 根身份完整后映射系统角色用途 | 专项源码静态类型与实际装配人工复核 |
| RCE1548 | R0538 | R0541 | <code>海中鱼巣/领域/概念活动状态.数据.h:107</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 用途匹配后映射节点类型 | 专项源码静态类型与实际装配人工复核 |
| RCE1549 | R0538 | R0542 | <code>海中鱼巣/领域/概念活动状态.数据.h:108</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | 类型匹配后复核概念根签名 | 专项源码静态类型与实际装配人工复核 |
| RCE1550 | R0538 | F0459 | <code>海中鱼巣/领域/概念活动状态.数据.h:109</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 签名成立后复核活跃关系材料 | 专项源码静态类型与实际装配人工复核 |
| RCE1551 | R0544 | F0168 | <code>海中鱼巣/领域/特征值服务.h:581</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1552 | R0544 | R0543 | <code>海中鱼巣/领域/特征值服务.h:585, 海中鱼巣/领域/特征值服务.h:588</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1553 | R0545 | F0168 | <code>海中鱼巣/领域/特征值服务.h:594</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1554 | R0546 | R0547 | <code>海中鱼巣/领域/特征值服务.h:674</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册非 const Vec 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1555 | R0547 | F0051 | <code>海中鱼巣/领域/特征值服务.h:675</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | Vec 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1556 | R0548 | R0549 | <code>海中鱼巣/领域/特征值服务.h:680</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册 const Vec 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1557 | R0549 | F0051 | <code>海中鱼巣/领域/特征值服务.h:681</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | const Vec 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1558 | R0550 | R0551 | <code>海中鱼巣/领域/特征值服务.h:686</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册非 const I64 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1559 | R0551 | F0051 | <code>海中鱼巣/领域/特征值服务.h:687</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | I64 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1560 | R0552 | R0553 | <code>海中鱼巣/领域/特征值服务.h:692</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::find_if 注册 const I64 记录匹配谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1561 | R0553 | F0051 | <code>海中鱼巣/领域/特征值服务.h:693</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | const I64 记录节点句柄与目标节点句柄比较 | 专项源码静态类型与实际装配人工复核 |
| RCE1562 | R0554 | R0555 | <code>海中鱼巣/领域/特征值服务.h:699</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::count_if 注册 Vec 记录计数谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1563 | R0555 | F0051 | <code>海中鱼巣/领域/特征值服务.h:700</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | Vec 计数谓词比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1564 | R0556 | R0557 | <code>海中鱼巣/领域/特征值服务.h:706</code> | standard-library-callback | <code>未单独冻结；读取源码调用点</code> | std::count_if 注册 I64 记录计数谓词 | 专项源码静态类型与实际装配人工复核 |
| RCE1565 | R0557 | F0051 | <code>海中鱼巣/领域/特征值服务.h:707</code> | direct-free | <code>未单独冻结；读取源码调用点</code> | I64 计数谓词比较节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1566 | R0560 | F0622 | <code>海中鱼巣/领域/特征服务.h:217</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1567 | R0560 | F0623 | <code>海中鱼巣/领域/特征服务.h:217</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1568 | R0562 | R0558 | <code>海中鱼巣/领域/特征服务.h:357</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1569 | R0566 | F0168 | <code>海中鱼巣/领域/组合.特征状态.ixx:33</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1570 | R0566 | R0502 | <code>海中鱼巣/领域/组合.特征状态.ixx:34</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；形成实例特征槽位规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1571 | R0566 | R0508 | <code>海中鱼巣/领域/组合.特征状态.ixx:36, 海中鱼巣/领域/组合.特征状态.ixx:39</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 槽位规格静态类型复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE1572 | R0566 | R0509 | <code>海中鱼巣/领域/组合.特征状态.ixx:36, 海中鱼巣/领域/组合.特征状态.ixx:39</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 初始值规格静态类型复核成功 | 专项源码静态类型与实际装配人工复核 |
| RCE1573 | R0566 | R0567 | <code>海中鱼巣/领域/组合.特征状态.ixx:36, 海中鱼巣/领域/组合.特征状态.ixx:39</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1574 | R0566 | R0503 | <code>海中鱼巣/领域/组合.特征状态.ixx:37</code> | unique_name+direct-const-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；形成初始特征值规格 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1575 | R0566 | R0389 | <code>海中鱼巣/领域/组合.特征状态.ixx:40</code> | direct-const-member | <code>未单独冻结；读取源码调用点</code> | 两份规格成功后创建槽位并发布初始状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1576 | R0571 | F0580 | <code>海中鱼巣/领域/语素服务.h:287, 海中鱼巣/领域/语素服务.h:288</code> | direct_const_member | <code>this=&关系_,关系类型::运行期临时,节点句柄值</code> | 节点记录存在，且类型为状态或动态时按逻辑或短路调用 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1577 | R0572 | F0580 | <code>海中鱼巣/领域/语素服务.h:304, 海中鱼巣/领域/语素服务.h:305</code> | direct_const_member | <code>this=&关系_,关系类型::运行期临时,节点句柄值</code> | 节点记录存在，且类型为状态或动态时按逻辑或短路调用 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1578 | R0573 | F0441 | <code>海中鱼巣/领域/语素服务.h:331, 海中鱼巣/领域/语素服务.h:362</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1579 | R0573 | F0622 | <code>海中鱼巣/领域/语素服务.h:335</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1580 | R0573 | F0168 | <code>海中鱼巣/领域/语素服务.h:336, 海中鱼巣/领域/语素服务.h:341, 海中鱼巣/领域/语素服务.h:347</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1581 | R0573 | F0623 | <code>海中鱼巣/领域/语素服务.h:340</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1582 | R0573 | R0574 | <code>海中鱼巣/领域/语素服务.h:342, 海中鱼巣/领域/语素服务.h:348, 海中鱼巣/领域/语素服务.h:355, 海中鱼巣/领域/语素服务.h:366</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1583 | R0573 | R0568 | <code>海中鱼巣/领域/语素服务.h:360</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1584 | R0573 | F0496 | <code>海中鱼巣/领域/语素服务.h:361</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1585 | R0573 | R0570 | <code>海中鱼巣/领域/语素服务.h:361</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1586 | R0574 | F0441 | <code>海中鱼巣/领域/语素服务.h:374</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1587 | R0574 | R0123 | <code>海中鱼巣/领域/语素服务.h:376</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1588 | R0574 | F0168 | <code>海中鱼巣/领域/语素服务.h:379, 海中鱼巣/领域/语素服务.h:382, 海中鱼巣/领域/语素服务.h:385</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1589 | R0574 | R0075 | <code>海中鱼巣/领域/语素服务.h:380</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1590 | R0574 | R0147 | <code>海中鱼巣/领域/语素服务.h:383</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1591 | R0574 | R0008 | <code>海中鱼巣/领域/语素服务.h:386</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1592 | R0575 | R0569 | <code>海中鱼巣/领域/语素服务.h:66</code> | suffix_match | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1593 | R0576 | R0077 | <code>海中鱼巣/领域/需求服务.h:1010</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1594 | R0577 | R0077 | <code>海中鱼巣/领域/需求服务.h:1030</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1595 | R0578 | F0622 | <code>海中鱼巣/领域/需求服务.h:132</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1596 | R0578 | F0623 | <code>海中鱼巣/领域/需求服务.h:132</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1597 | R0579 | F0554 | <code>海中鱼巣/领域/需求服务.h:300</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1598 | R0579 | F0555 | <code>海中鱼巣/领域/需求服务.h:302</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1599 | R0579 | F0168 | <code>海中鱼巣/领域/需求服务.h:307, 海中鱼巣/领域/需求服务.h:311, 海中鱼巣/领域/需求服务.h:321, 海中鱼巣/领域/需求服务.h:322, 海中鱼巣/领域/需求服务.h:323, 海中鱼巣/领域/需求服务.h:324, 海中鱼巣/领域/需求服务.h:325, 海中鱼巣/领域/需求服务.h:326</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1600 | R0579 | F0184 | <code>海中鱼巣/领域/需求服务.h:307, 海中鱼巣/领域/需求服务.h:311, 海中鱼巣/领域/需求服务.h:321, 海中鱼巣/领域/需求服务.h:339</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1601 | R0579 | R0578 | <code>海中鱼巣/领域/需求服务.h:310</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1602 | R0579 | R0586 | <code>海中鱼巣/领域/需求服务.h:331</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1603 | R0579 | F0553 | <code>海中鱼巣/领域/需求服务.h:337</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1604 | R0579 | R0585 | <code>海中鱼巣/领域/需求服务.h:338</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1605 | R0580 | F0336 | <code>海中鱼巣/领域/需求服务.h:349</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1606 | R0580 | R0594 | <code>海中鱼巣/领域/需求服务.h:350</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1607 | R0581 | R0077 | <code>海中鱼巣/领域/需求服务.h:357</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1608 | R0582 | R0576 | <code>海中鱼巣/领域/需求服务.h:514</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1609 | R0583 | R0576 | <code>海中鱼巣/领域/需求服务.h:519</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1610 | R0583 | R0582 | <code>海中鱼巣/领域/需求服务.h:523</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1611 | R0583 | R0078 | <code>海中鱼巣/领域/需求服务.h:527</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1612 | R0584 | R0576 | <code>海中鱼巣/领域/需求服务.h:540</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1613 | R0585 | R0576 | <code>海中鱼巣/领域/需求服务.h:553, 海中鱼巣/领域/需求服务.h:557</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1614 | R0586 | R0580 | <code>海中鱼巣/领域/需求服务.h:577</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1615 | R0586 | R0595 | <code>海中鱼巣/领域/需求服务.h:578, 海中鱼巣/领域/需求服务.h:579, 海中鱼巣/领域/需求服务.h:580, 海中鱼巣/领域/需求服务.h:581</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1616 | R0587 | R0078 | <code>海中鱼巣/领域/需求服务.h:708</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1617 | R0587 | R0596 | <code>海中鱼巣/领域/需求服务.h:713</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1618 | R0588 | R0597 | <code>海中鱼巣/领域/需求服务.h:749</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1619 | R0589 | R0597 | <code>海中鱼巣/领域/需求服务.h:753</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1620 | R0590 | R0597 | <code>海中鱼巣/领域/需求服务.h:757</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1621 | R0591 | R0577 | <code>海中鱼巣/领域/需求服务.h:761</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1622 | R0592 | R0597 | <code>海中鱼巣/领域/需求服务.h:765</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1623 | R0593 | F0168 | <code>海中鱼巣/领域/需求服务.h:773</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1624 | R0593 | R0581 | <code>海中鱼巣/领域/需求服务.h:776</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1625 | R0593 | R0586 | <code>海中鱼巣/领域/需求服务.h:780</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1626 | R0595 | R0594 | <code>海中鱼巣/领域/需求服务.h:826, 海中鱼巣/领域/需求服务.h:831</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1627 | R0595 | R0077 | <code>海中鱼巣/领域/需求服务.h:830</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1628 | R0596 | R0588 | <code>海中鱼巣/领域/需求服务.h:980</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1629 | R0596 | R0589 | <code>海中鱼巣/领域/需求服务.h:981</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1630 | R0596 | R0590 | <code>海中鱼巣/领域/需求服务.h:982</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1631 | R0596 | R0591 | <code>海中鱼巣/领域/需求服务.h:983</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 同一具名类型内直接调用；源码调用词与定义范围闭合 |
| RCE1632 | R0597 | R0077 | <code>海中鱼巣/领域/需求服务.h:991</code> | unique_name | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立 | 全项目唯一函数名并经调用方源码范围复核 |
| RCE1692 | R0600 | F0631 | <code>海中鱼巣/核心/结构事务接线.数据.h:31</code> | unique_name+direct-member | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；非自赋值分支先释放当前许可 | 同一具名类型内直接调用；源码调用词与定义范围闭合；专项源码静态类型与实际装配人工复核 |
| RCE1693 | R0604 | F0168 | <code>海中鱼巣/领域/概念图服务.h:244, 海中鱼巣/领域/概念图服务.h:245, 海中鱼巣/领域/概念图服务.h:246</code> | unique_name+direct_free | <code>未单独冻结；读取源码调用点</code> | 调用方可达且源码分支条件成立；复核关系句柄有效 | 全项目唯一函数名并经调用方源码范围复核；专项源码静态类型与实际装配人工复核 |
| RCE1694 | R0604 | F0163 | <code>海中鱼巣/领域/概念图服务.h:245, 海中鱼巣/领域/概念图服务.h:246</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 依次复核源节点和目标节点句柄有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1695 | R0605 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3019</code> | operator | <code>未单独冻结；读取源码调用点</code> | 登记槽有值后比较状态节点 | 专项源码静态类型与实际装配人工复核 |
| RCE1696 | R0605 | F0377 | <code>海中鱼巣/领域/概念图服务.h:3020</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 登记状态等于输入后复核节点仍有效 | 专项源码静态类型与实际装配人工复核 |
| RCE1697 | R0606 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3079, 海中鱼巣/领域/概念图服务.h:3087, 海中鱼巣/领域/概念图服务.h:3088</code> | operator | <code>未单独冻结；读取源码调用点</code> | lambda 先比较登记概念和状态，再比较读回端点 | 专项源码静态类型与实际装配人工复核 |
| RCE1698 | R0606 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3082</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 登记概念和状态均匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1699 | R0607 | F0051 | <code>海中鱼巣/领域/概念图服务.h:3096</code> | operator | <code>未单独冻结；读取源码调用点</code> | lambda 比较登记概念和状态 | 专项源码静态类型与实际装配人工复核 |
| RCE1700 | R0607 | F0587 | <code>海中鱼巣/领域/概念图服务.h:3099</code> | direct_const_member | <code>未单独冻结；读取源码调用点</code> | 登记概念和状态均匹配后读取登记关系 | 专项源码静态类型与实际装配人工复核 |
| RCE1701 | R0608 | F0163 | <code>海中鱼巣/领域/概念图服务.h:56, 海中鱼巣/领域/概念图服务.h:59</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 复核概念和状态节点句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1702 | R0608 | F0168 | <code>海中鱼巣/领域/概念图服务.h:60</code> | direct_free | <code>未单独冻结；读取源码调用点</code> | 阶段和两个节点句柄前置成立后复核关系句柄 | 专项源码静态类型与实际装配人工复核 |
| RCE1703 | R0571 | F0190 | <code>海中鱼巣/领域/语素服务.h:279</code> | direct_const_member | <code>this=&节点_,节点句柄值</code> | R0571函数进入 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1704 | R0572 | F0190 | <code>海中鱼巣/领域/语素服务.h:297</code> | direct_const_member | <code>this=&节点_,节点句柄值</code> | R0572函数进入 | 源码逐行、实参数量与重载类型审计 PASS |
| RCE1705 | F0363 | F0163 | <code>海中鱼巣/领域/语素服务.h:35</code> | direct_free | <code>语素入口</code> | F0363函数进入 | 源码逐行与重载类型审计 PASS |
| RCE1706 | F0363 | F0565 | <code>海中鱼巣/领域/语素服务.h:36</code> | direct_free | <code>主信息</code> | F0163@35 返回 true；逻辑与短路继续 | 源码逐行与重载类型审计 PASS |
| RCE1707 | F0364 | F0163 | <code>海中鱼巣/领域/语素服务.h:112</code> | direct_free | <code>结果.语素入口</code> | R0573@111 已返回语素入口创建结果 | 源码逐行与重载类型审计 PASS |
| RCE1708 | F0364 | F0167 | <code>海中鱼巣/领域/语素服务.h:115</code> | direct_member | <code>this=&关系_,关系类型::语素概念追溯,结果.语素入口,概念节点,默认顺序号0</code> | F0163@112 返回 true | 源码逐行、默认实参与重载类型审计 PASS |
| RCE1709 | F0365 | F0331 | <code>海中鱼巣/领域/存在服务.h:104</code> | direct_member_nonvirtual | <code>this=&主信息_</code> | F0365进入 | 源码静态接收者与零参数重载复核 |
| RCE1710 | F0365 | F0332 | <code>海中鱼巣/领域/存在服务.h:105</code> | direct_member_nonvirtual | <code>this=&节点_, 节点类型::存在, 主信息句柄</code> | 主信息句柄已创建 | 源码静态接收者与二参数重载复核 |
| RCE1711 | F0365 | R0609 | <code>海中鱼巣/领域/存在服务.h:106</code> | direct_const_member | <code>this, 根节点</code> | 根节点已创建 | 同一具名类型一参数重载与定义范围复核 |
| RCE1712 | F0365 | F0624 | <code>海中鱼巣/领域/存在服务.h:107</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在 | 源码静态接收者与一参数重载复核 |
| RCE1713 | F0366 | F0331 | <code>海中鱼巣/领域/动态服务.h:286</code> | direct_member_nonvirtual | <code>this=&主信息_</code> | F0366进入 | 源码静态接收者与零参数重载复核 |
| RCE1714 | F0366 | F0332 | <code>海中鱼巣/领域/动态服务.h:287</code> | direct_member_nonvirtual | <code>this=&节点_, 节点类型::动态, 主信息句柄</code> | 主信息句柄已创建 | 源码静态接收者与二参数重载复核 |
| RCE1715 | F0366 | F0190 | <code>海中鱼巣/领域/动态服务.h:288</code> | direct_const_member | <code>this=&节点_, 根节点</code> | 根节点已创建 | 源码静态接收者与一参数重载复核 |
| RCE1716 | F0366 | F0619 | <code>海中鱼巣/领域/动态服务.h:289</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在 | 源码静态接收者与一参数重载复核 |
| RCE1717 | F0366 | F0624 | <code>海中鱼巣/领域/动态服务.h:292</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在且类型为动态 | 源码静态接收者与一参数重载复核 |
| RCE1718 | F0367 | F0190 | <code>海中鱼巣/领域/二次特征服务.h:51</code> | direct_const_member | <code>this=&节点_, 根节点</code> | 根节点已创建 | 源码静态接收者与一参数重载复核 |
| RCE1719 | F0367 | F0624 | <code>海中鱼巣/领域/二次特征服务.h:54</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在且类型为二次特征 | 源码静态接收者与一参数重载复核 |
| RCE1720 | R0609 | F0190 | <code>海中鱼巣/领域/存在服务.h:132</code> | direct_const_member | <code>this=&节点_, 存在节点</code> | R0609进入 | 源码静态接收者与一参数重载复核 |
| RCE1721 | R0610 | F0190 | <code>海中鱼巣/领域/动态服务.h:316</code> | direct_const_member | <code>this=&节点_, 动态节点</code> | R0610进入 | 源码静态接收者与一参数重载复核 |
| RCE1722 | R0610 | F0580 | <code>海中鱼巣/领域/动态服务.h:318</code> | direct_const_member | <code>this=&关系_, 关系类型::运行期临时, 动态节点</code> | 记录存在且类型为动态 | 源码静态接收者与二参数重载复核 |
| RCE1723 | F0516 | R0609 | <code>海中鱼巣/领域/存在服务.h:96</code> | direct_const_member | <code>this, 存在</code> | 存在节点已创建 | 同一具名类型一参数重载与定义范围复核 |
| RCE1724 | R0277 | R0609 | <code>海中鱼巣/领域/存在服务.h:140</code> | direct_const_member | <code>this, 存在节点</code> | R0277进入 | 同一具名类型一参数重载与定义范围复核 |
| RCE1726 | F0368 | R0611 | <code>海中鱼巣/领域/因果服务.h:57</code> | direct_member_nonvirtual | <code>this=&因果服务, 无显式参数</code> | F0368进入 | 当前源码重载与调用点复核 |
| RCE1727 | F0368 | F0190 | <code>海中鱼巣/领域/因果服务.h:58</code> | direct_const_member | <code>this=&节点_, 根节点</code> | R0611返回后 | 当前源码逐调用点复核 |
| RCE1728 | F0368 | F0624 | <code>海中鱼巣/领域/因果服务.h:61</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在且类型为因果引用 | 当前源码逐调用点复核 |
| RCE1729 | R0611 | F0331 | <code>海中鱼巣/领域/因果服务.h:45</code> | direct_member_nonvirtual | <code>this=&主信息_</code> | R0611进入 | 当前源码逐调用点复核 |
| RCE1730 | R0611 | F0332 | <code>海中鱼巣/领域/因果服务.h:46</code> | direct_member_nonvirtual | <code>this=&节点_, 节点类型::因果引用, 主信息句柄</code> | 主信息句柄已创建 | 当前源码逐调用点复核 |
| RCE1731 | R0611 | F0190 | <code>海中鱼巣/领域/因果服务.h:47</code> | direct_const_member | <code>this=&节点_, 因果引用</code> | 因果引用节点已创建 | 当前源码逐调用点复核 |
| RCE1732 | R0611 | F0624 | <code>海中鱼巣/领域/因果服务.h:49</code> | direct_const_member | <code>this=&主信息_, 记录->主信息</code> | 记录存在且类型为因果引用 | 当前源码逐调用点复核 |
| RCE1733 | R0611 | F0184 | <code>海中鱼巣/领域/因果服务.h:48-50</code> | direct_free_function | <code>记录.has_value() && 记录->类型==因果引用 && 主信息_.主信息是否有效(记录->主信息), 固定说明</code> | 记录读取返回后总是形成条件并调用 | 当前源码逐调用点复核 |
| RCE1734 | F0369 | F0190 | <code>海中鱼巣/领域/概念图服务.h:927</code> | direct_const_member | <code>this=&节点_, 节点</code> | F0369进入 | 当前源码逐调用点复核 |
| RCE1735 | F0370 | F0051 | <code>海中鱼巣/领域/概念图服务.h:1034</code> | direct_free_operator | <code>实例, 概念</code> | 实例与概念粗类别读取完成后进入写前短路判断 | 当前源码逐调用点复核 |
| RCE1736 | F0376 | F0630 | <code>海中鱼巣/核心/索引仓库.cpp:427</code> | direct_const_member+source_audited | <code>this=&节点_, 候选, 令牌</code> | 共享令牌有效；候选快照已形成；逐候选循环 | 当前源码逐调用点、接收者类型与重载复核 |
| RCE1737 | F0377 | F0336 | <code>海中鱼巣/核心/节点仓库.cpp:442</code> | direct_const_member+source_audited | <code>this=&事务接线_</code> | F0377进入 | 当前源码逐调用点、Clang AST候选与同仓接域调用模式复核 |
| RCE1738 | F0377 | F0397 | <code>海中鱼巣/核心/节点仓库.cpp:443</code> | resolved_function_pointer+source_audited | <code>状态=事务接线_.运行期状态</code> | RCE1737返回true | 当前源码逐调用点、现行接线装配与既有RCE0042同域绑定复核 |
| RCE1739 | F0377 | F0338 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | <code>this=&许可</code> | RCE1738已形成局部许可 | 当前源码逐调用点与Clang AST短路表达式复核 |
| RCE1740 | F0377 | F0339 | <code>海中鱼巣/核心/节点仓库.cpp:444</code> | direct_const_member+source_audited | <code>this=&许可</code> | RCE1739返回true；逻辑与右侧开始求值 | 当前源码逐调用点与Clang AST左到右短路顺序复核 |
| RCE1741 | F0377 | F0345 | <code>海中鱼巣/核心/节点仓库.cpp:443-445</code> | implicit_destructor+source_audited | <code>this=&许可</code> | RCE1738已形成许可；返回值求值完成或异常展开 | 当前源码作用域、C++ RAII与既有E0790同域生命周期复核 |
| RCE1742 | F0381 | F0214 | <code>海中鱼巣/适配/SQL数据库适配.cpp:251-265</code> | implicit_destructor+source_audited | <code>this=&语句</code> | F0207已完成局部语句构造；随后正常返回或异常展开 | 当前源码作用域、C++ RAII与F0214已定义析构逐行复核 |
| RCE1743 | F0384 | F0336 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | <code>this=&事务接线_</code> | F0384进入 | 当前源码单行控制流、静态接收者与同仓调用模式复核 |
| RCE1744 | F0384 | F0397 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | resolved_function_pointer+source_audited | <code>状态=事务接线_.运行期状态</code> | RCE1743返回true | 当前源码、现行接线装配与既有同域函数指针绑定复核 |
| RCE1745 | F0384 | F0338 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | <code>this=&许可</code> | RCE1744已形成局部许可 | 当前源码三元表达式左到右求值与静态接收者复核 |
| RCE1746 | F0384 | F0339 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | direct_const_member+source_audited | <code>this=&许可</code> | RCE1745返回true；三元表达式选择读取分支 | 当前源码三元表达式条件分支与静态接收者复核 |
| RCE1747 | F0384 | F0345 | <code>海中鱼巣/核心/主信息仓库.cpp:199</code> | implicit_destructor+source_audited | <code>this=&许可</code> | RCE1744已形成许可；返回值求值完成或异常展开 | 当前源码单行作用域、C++ RAII与同仓许可生命周期复核 |
| RCE1748 | F0385 | F0173 | <code>海中鱼巣/界面/投影.控制面板启动.ixx:45</code> | direct_const_member+source_audited | <code>this=&语素初始化读数</code> | F0385进入；false时第46行返回std::nullopt | 当前源码逐调用点、静态接收者类型与F0173完整签名复核 |
| RCE1749 | F0388 | R0612 | <code>海中鱼巣/界面/控制面板窗口.cpp:394-401</code> | standard_algorithm_callback+source_audited | <code>材料=std::all_of当前遍历元素const引用</code> | F0388执行X01021；std::all_of对尚未短路的当前元素调用谓词 | 当前源码397-401行lambda正文、标准算法回调语义与R0612身份复核 |
| RCE1750 | F0387 | F0336 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | <code>this=&事务接线_</code> | F0387进入并展开关系共享许可范围宏 | 当前源码宏定义116-123行、调用点734行与同宏调用模式复核 |
| RCE1751 | F0387 | F0337 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_free+macro_expanded+source_audited | <code>仓库=*this</code> | RCE1750返回true；逻辑与右侧开始求值 | 当前源码宏定义116-123行、调用点734行与F0337完整签名复核 |
| RCE1752 | F0387 | F0397 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | resolved_function_pointer+macro_expanded+source_audited | <code>状态=事务接线_.运行期状态</code> | RCE1750返回true且RCE1751返回nullptr | 当前源码宏展开、现行生产装配与既有同域函数指针绑定复核 |
| RCE1753 | F0387 | F0338 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | <code>this=&自动许可.value()</code> | RCE1752返回许可并经RCE1756移动构造完成 | 当前源码宏定义120-121行、调用点734行与F0338签名复核 |
| RCE1754 | F0387 | F0339 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_const_member+macro_expanded+source_audited | <code>this=&自动许可.value()</code> | RCE1753返回true | 当前源码宏定义121-122行、调用点734行与F0339签名复核 |
| RCE1755 | F0387 | F0340 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | direct_constructor+macro_expanded+source_audited | <code>仓库=*this,令牌=RCE1754返回const引用</code> | RCE1753返回true且RCE1754已取得令牌引用 | 当前源码宏定义122行、调用点734行与F0340完整构造签名复核 |
| RCE1756 | F0387 | F0375 | <code>海中鱼巣/核心/关系仓库.cpp:734</code> | move-constructor+macro-expanded-move-constructor | <code>其它=RCE1752返回的临时结构事务许可</code> | RCE1752返回临时许可；进入optional.emplace承载值构造 | 当前源码宏定义120行、调用点734行、F0375签名与既有E0793同宏先例复核 |
| RCE1757 | F0402 | R0114 | <code>海中鱼巣/领域/数据操作.存在场景.ixx:150</code> | constructor+source-audited | <code>接线=接线_,主信息=&主信息_,节点=&节点_,关系=&关系_,索引=&索引_</code> | F0402成员接线、四仓库引用和关系仓库编号完成初始化 | 当前源码第150行成员初始化器、成员声明与R0114构造函数签名专项复核 |
| RCE1758 | F0406 | R0114 | <code>海中鱼巣/领域/数据操作.轻量因果.ixx:102</code> | constructor+source-audited | <code>接线=接线_,主信息=&主信息_,节点=&节点_,关系=&关系_,索引=&索引_</code> | F0406成员接线和四仓库引用完成初始化 | 当前源码第102行成员初始化器、成员声明与R0114构造函数签名专项复核 |
