# main当前可达外部边界表

代码基线：`720f4afd78e63d95d244867aa13d72970a419654`；外部边界：24；具名回调表项：0。

| 边界ID | 调用方 | 外部签名 | 调用点 | 次数 | 类别 |
| --- | --- | --- | --- | --- | --- |
| X00001 | F0002 | `std::basic_string_view<char>::basic_string_view(const char*)` | 海中鱼巣/启动.程序入口.ixx:29 | 1 | Standard library |
| X00002 | F0002 | `std::operator==(std::basic_string_view<char>, std::basic_string_view<char>) noexcept` | 海中鱼巣/启动.程序入口.ixx:31；海中鱼巣/启动.程序入口.ixx:38 | 2 | Standard library |
| X00003 | F0002 | `std::basic_string_view<char>::operator=(const basic_string_view&) noexcept` | 海中鱼巣/启动.程序入口.ixx:44 | 1 | Standard library |
| X02269 | F0007 | `std::unique_ptr<停止信号租约>::operator*() const` | 海中鱼巣/启动.应用程序.ixx:29 | 1 | Standard library |
| X06246 | F0007 | `std::unique_ptr<普通应用上下文>::~unique_ptr()` | 海中鱼巣/启动.应用程序.ixx:20；海中鱼巣/启动.应用程序.ixx:23；海中鱼巣/启动.应用程序.ixx:27；海中鱼巣/启动.应用程序.ixx:29-30 | 4 | Standard library lifecycle |
| X06247 | F0007 | `std::unique_ptr<停止信号租约>::~unique_ptr()` | 海中鱼巣/启动.应用程序.ixx:29-30 | 1 | Standard library lifecycle |
| X00010 | F0013 | `std::make_unique<普通应用上下文>()` | 海中鱼巣/装配.普通应用.ixx:49 | 1 | Standard library |
| X06248 | F0014 | `std::operator!=(const std::unique_ptr<普通应用上下文>&, std::nullptr_t) noexcept` | 海中鱼巣/装配.普通应用.ixx:40 | 1 | Standard library |
| X06249 | F0019 | `std::function<程序信号处理函数(int, 程序信号处理函数)>::operator bool() const noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:101 | 1 | Standard library |
| X00013 | F0019 | `std::make_unique<停止信号租约>()` | 海中鱼巣/启动.程序运行宿主.ixx:105 | 1 | Standard library |
| X00014 | F0019 | `std::move(程序信号安装函数&)` | 海中鱼巣/启动.程序运行宿主.ixx:106 | 1 | Standard library |
| X06250 | F0019 | `std::unique_ptr<停止信号租约>::operator->() const noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:106-124 | 13 | Standard library |
| X06251 | F0019 | `std::function::operator=(std::function&&) noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:106 | 1 | Standard library |
| X00015 | F0019 | `std::function::operator()(int, 程序信号处理函数) / std::signal` | 海中鱼巣/启动.程序运行宿主.ixx:107；海中鱼巣/启动.程序运行宿主.ixx:112；海中鱼巣/启动.程序运行宿主.ixx:118 | 3 | C runtime / standard-library function wrapper |
| X06252 | F0019 | `std::unique_ptr<停止信号租约>::get() const noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:124 | 1 | Standard library |
| X00016 | F0019 | `std::move(std::unique_ptr<停止信号租约>&)` | 海中鱼巣/启动.程序运行宿主.ixx:125 | 1 | Standard library |
| X06253 | F0019 | `std::unique_ptr<停止信号租约>::~unique_ptr()` | 海中鱼巣/启动.程序运行宿主.ixx:109；海中鱼巣/启动.程序运行宿主.ixx:114；海中鱼巣/启动.程序运行宿主.ixx:120；海中鱼巣/启动.程序运行宿主.ixx:125-127 | 5 | Standard library lifecycle |
| X00017 | F0020 | `std::operator!=(const std::unique_ptr<停止信号租约>&, std::nullptr_t) noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:81 | 1 | Standard library |
| X06254 | R1503 | `std::chrono::milliseconds::milliseconds(std::int64_t)` | 海中鱼巣/启动.程序运行宿主.ixx:133 | 1 | Standard library |
| X06255 | R1503 | `std::this_thread::sleep_for(const std::chrono::milliseconds&)` | 海中鱼巣/启动.程序运行宿主.ixx:133 | 1 | Standard library / OS scheduling |
| X06256 | F0025 | `std::function::operator bool() const noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:35 | 1 | Standard library |
| X06257 | F0025 | `std::function::operator()(int, 程序信号处理函数)` | 海中鱼巣/启动.程序运行宿主.ixx:38；海中鱼巣/启动.程序运行宿主.ixx:42；海中鱼巣/启动.程序运行宿主.ixx:45 | 3 | C runtime / standard-library function wrapper |
| X06258 | F0057 | `std::function<程序信号处理函数(int, 程序信号处理函数)>::function() noexcept` | 海中鱼巣/启动.程序运行宿主.ixx:33；海中鱼巣/启动.程序运行宿主.ixx:65 | 1 | Standard library lifecycle |
| X06259 | R1505 | `编译器成员初始化：std::shared_mutex + 7个std::unordered_map + 1个std::unordered_set` | 海中鱼巣/核心/仓库.L1事实基座.ixx:84；海中鱼巣/核心/仓库.L1事实基座.ixx:613-629；海中鱼巣/核心/仓库.L1事实基座.ixx:1319-1320 | 9 | Standard library lifecycle |

F0019到F0058的信号回调按可唯一解析项目边E0075登记，不重复计入外部回调表。
