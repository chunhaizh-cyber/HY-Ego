# main当前可达项目调用边表

代码基线：`720f4afd78e63d95d244867aa13d72970a419654`；项目边：20。

| 边ID | 调用方 | 被调方 | 调用点 | 类别 | 条件 |
| --- | --- | --- | --- | --- | --- |
| E0001 | F0001 | F0002 | 海中鱼巣/入口.cpp:9 | direct_free_function | always |
| E0002 | F0001 | F0003 | 海中鱼巣/入口.cpp:10 | direct_method | 解析完成 |
| E0003 | F0001 | F0004 | 海中鱼巣/入口.cpp:11；海中鱼巣/入口.cpp:14 | direct_free_function | 解析失败或运行完成 |
| E0004 | F0001 | F0005 | 海中鱼巣/入口.cpp:13 | direct_free_function | 解析成功 |
| E0006 | F0004 | F0003 | 海中鱼巣/启动.程序入口.ixx:54 | direct_method | always |
| E0007 | F0004 | F0006 | 海中鱼巣/启动.程序入口.ixx:60 | direct_method | 运行指针非空 |
| E0008 | F0005 | F0007 | 海中鱼巣/启动.应用程序.ixx:40 | direct_free_function | 普通控制面板或无窗口常驻 |
| E0014 | F0007 | F0013 | 海中鱼巣/启动.应用程序.ixx:18 | direct_free_function | 普通模式有效 |
| E0015 | F0007 | F0014 | 海中鱼巣/启动.应用程序.ixx:19 | direct_method | 装配返回 |
| E0020 | F0007 | F0019 | 海中鱼巣/启动.应用程序.ixx:25 | direct_free_function | 无窗口模式且装配成功 |
| E0021 | F0007 | F0020 | 海中鱼巣/启动.应用程序.ixx:26 | direct_method | 信号安装返回 |
| RCE4565 | F0007 | R1503 | 海中鱼巣/启动.应用程序.ixx:29 | direct_free_function | 信号安装成功 |
| E0026 | F0007 | F0025 | 海中鱼巣/启动.应用程序.ixx:29-30 | implicit_destructor | 无窗口宿主返回后离开作用域 |
| E0054 | F0013 | F0039 | 海中鱼巣/装配.普通应用.ixx:45 | direct_method | always |
| E0055 | F0013 | F0040 | 海中鱼巣/装配.普通应用.ixx:49 | constructor_via_make_unique | 配置有效 |
| RCE4566 | F0040 | R1504 | 海中鱼巣/装配.普通应用.ixx:20,26 | member_default_constructor | 上下文构造 |
| RCE4567 | R1504 | R1505 | 海中鱼巣/核心/服务.L1事实基座.ixx:103,397 | member_default_constructor | 服务构造 |
| E0074 | F0019 | F0057 | 海中鱼巣/启动.程序运行宿主.ixx:105 | constructor_via_make_unique | 安装前置成立 |
| E0075 | F0019 | F0058 | 海中鱼巣/启动.程序运行宿主.ixx:107；海中鱼巣/启动.程序运行宿主.ixx:112；海中鱼巣/启动.程序运行宿主.ixx:118 | callback_register | 依次安装；SIGBREAK仅Windows |
| E0076 | F0019 | F0025 | 海中鱼巣/启动.程序运行宿主.ixx:109；海中鱼巣/启动.程序运行宿主.ixx:114；海中鱼巣/启动.程序运行宿主.ixx:120；海中鱼巣/启动.程序运行宿主.ixx:125-127 | implicit_destructor | 失败返回、异常或所有权清理 |

项目边端点全部属于当前身份表。
