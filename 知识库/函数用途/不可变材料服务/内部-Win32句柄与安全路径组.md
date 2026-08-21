# 内部-Win32句柄与安全路径组

- 稳定身份：16个完整函数身份
- 边界类别：内部服务函数
- 模块：`海中鱼巣.领域.服务.不可变材料`
- 位置：`海中鱼巣/领域/服务.不可变材料.ixx:56-178`
- 绑定提交：`c003a6f4547b6c2bbdaee73676f03d19c7e6af59`
- 源码 blob：`5382f87a57943fac39e8e69823fb9c0b7bf1daf9`
- 知识状态：当前
- 验证状态：16/16身份与Win32路径边界静态互证；根、候选、重解析和异卷黑盒已覆盖

## 完整身份与用途

| 完整身份 | 只负责 |
| --- | --- |
| `不可变材料内部::唯一句柄::唯一句柄() noexcept` | 形成无效句柄RAII壳 |
| `explicit 不可变材料内部::唯一句柄::唯一句柄(HANDLE) noexcept` | 接管一个Win32句柄 |
| `不可变材料内部::唯一句柄::唯一句柄(const 唯一句柄&) = delete` | 禁止复制句柄所有权 |
| `不可变材料内部::唯一句柄& 不可变材料内部::唯一句柄::operator=(const 唯一句柄&) = delete` | 禁止复制赋值 |
| `不可变材料内部::唯一句柄::唯一句柄(唯一句柄&&) noexcept` | 移动接管并释放来源 |
| `不可变材料内部::唯一句柄& 不可变材料内部::唯一句柄::operator=(唯一句柄&&) noexcept` | 关闭旧值后移动接管 |
| `不可变材料内部::唯一句柄::~唯一句柄()` | 关闭仍持有的有效句柄 |
| `HANDLE 不可变材料内部::唯一句柄::取得() const noexcept` | 借出原句柄值，不转移所有权 |
| `explicit 不可变材料内部::唯一句柄::operator bool() const noexcept` | 判断非空且非`INVALID_HANDLE_VALUE` |
| `HANDLE 不可变材料内部::唯一句柄::释放() noexcept` | 移出原句柄并把自身置无效 |
| `void 不可变材料内部::唯一句柄::重置(HANDLE = INVALID_HANDLE_VALUE) noexcept` | 关闭旧句柄并接管新值 |
| `bool 不可变材料内部::路径字面量有效(const std::filesystem::path&) noexcept` | 纯值拒绝空、非绝对和`.`/`..`分量 |
| `std::optional<std::filesystem::path> 不可变材料内部::读取句柄规范路径(HANDLE)` | 从已打开句柄取得最终规范DOS路径 |
| `std::optional<不可变材料内部::路径句柄事实> 不可变材料内部::打开路径事实(const std::filesystem::path&, bool, DWORD = GENERIC_READ, DWORD = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)` | 以不跟随重解析点的句柄打开文件/目录并读回规范路径、卷序列和元数据 |
| `bool 不可变材料内部::规范路径位于目录内(const std::filesystem::path&, const std::filesystem::path&) noexcept` | 在句柄证据之后按Windows大小写不敏感规则比较严格后代关系 |
| `std::optional<不可变材料内部::路径句柄事实> 不可变材料内部::确保普通子目录(const 路径句柄事实&, const std::filesystem::path&)` | 建立或打开普通子目录并互证同卷、根内和无重解析点 |

## 边界与非职责

这些函数只支撑材料服务的受控文件系统边界。字符串路径检查不是安全证明；只有句柄规范路径、卷序列和重解析点元数据组合后才形成准入证据。外部模块不得直接复用这些内部名字建立通用文件服务，也不得把`取得()`借出的句柄保存到RAII对象生命周期之外。

它们不创建材料身份、不读取L1、不决定格式或来源、不解释文件内容。目录建立失败由构造/公开边界映射，内部函数本身不记录业务状态。

## 依据与证据

- `计划/已完成计划/20260814_CONCEPT-SPACE-STRUCTURE-CRUD-REBUILD-M1_不可变材料结构代码实施切片_v0.3.md:264-268`
- `验证记录/20260814_CONCEPT-M1_不可变材料验证记录.md:30-32,40`
