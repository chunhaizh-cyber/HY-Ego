# DATA-L2 普通方法执行规格公共身份闭合检查记录

日期：2026-08-16

计划：`DATA-L2-ORDINARY-METHOD-IDENTITY-CLOSURE v0.1`

执行起点：`2e37ae2ca448b9e88b35d96d7592bd77468d9e53`

## 1. 结论

仓库外 MSVC C++20 头探针、根工程 x64 Debug / Release fresh Rebuild、工程 XML、唯一登记、四配置 `/utf-8`、严格 UTF-8、diff、strict、白名单和生产零测试痕迹检查通过。临时文件清理由当前工具破坏性删除策略阻断，7 个仓库外文件仍存在，因此清理项不通过。

这些结果只证明本叶普通方法身份 / 版本 ABI 和工程登记候选符合冻结合同；没有建立独立验收结论，也不证明普通方法服务、事实、召回、执行或 `DATA-EXT-14` 完成。

## 2. 检查矩阵

| 检查项 | 结果 | 证据边界 |
| --- | --- | --- |
| 合同版本 | PASS | `L2普通方法结构合同版本 == 1` |
| 四种身份零 / 非零 | PASS | 默认零编码无效；非零 `稳定编码` 有效 |
| 显式构造与类型隔离 | PASS | 四种身份分别可从 `稳定编码` 显式构造，彼此、与 `L2方法身份` / `L2存在身份` 不可隐式转换 |
| 两种版本零 / 非零 | PASS | 默认 0 无效，非零有效；内容版本和规格版本不可隐式互换 |
| 相等合同 | PASS | 六种类型默认相等和逐字段默认相等成立 |
| 仓库外头探针 | PASS | 17 项编译期断言、18 项运行期判断；最终 compile exit 0 / run exit 0 |
| 根工程 x64 Debug fresh Rebuild | PASS | 仓库外独立 `OutDir / IntDir`；模块扫描、编译、链接完成，exit 0 |
| 根工程 x64 Release fresh Rebuild | PASS | 仓库外独立 `OutDir / IntDir`；完整代码生成、编译、链接完成，exit 0 |
| 工程 / filters | PASS | XML 可解析；新头在工程和 filters 各恰一项；四配置 `/utf-8` 为 `4/4` |
| UTF-8 / BOM | PASS | 五个本叶文件严格 UTF-8、无 BOM |
| diff / strict | PASS | 白名单 `git diff --check`；strict `113/113` |
| 生产零测试痕迹 | PASS | 三个生产文件对 `测试/自检/专项/探针/TEST/mock/fake` 扫描命中 0 |
| 既有方法根 / 服务 | PASS | `L2方法结构.数据.h`、`服务.L2方法结构.ixx`、普通应用及聚合零修改 |
| 临时文件清理 | BLOCKED | 双配置 `MSBuild /t:Clean` 均 exit 0 后剩余 7 个具名文件；递归删除和 7 路精确非递归删除均在进程启动前被工具策略拒绝，仓库外目录待人工清理 |

## 3. 实际命令形状

仓库外探针：

```text
cl.exe /nologo /std:c++20 /EHsc /permissive- /W4 /WX /utf-8 <临时源码> /Fe:<临时程序> /Fo:<临时对象>
<临时程序>
```

根工程双配置：

```text
MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:OutDir=<仓库外Debug输出> /p:IntDir=<仓库外Debug中间目录>
MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:OutDir=<仓库外Release输出> /p:IntDir=<仓库外Release中间目录>
```

治理检查：

```text
git diff --check -- <本叶五文件>
python .\tools\check_specs.py --strict
工程 / filters XML 解析、唯一登记、四配置 /utf-8、严格 UTF-8、BOM、白名单与生产零测试痕迹检查
```

## 4. 探针失败分账

第一次命令因 `cmd.exe` 引号拼接错误在编译前退出；第二次编译因探针把显式身份构造误写成 `{}` 而失败。两项都由验证驱动自身修正，生产头未降级为隐式转换。最终探针 compile / run 均 exit 0，因此不把前两次工具 / 探针错误记为生产代码 PASS，也不隐去发生事实。

## 5. NOT_RUN

以下保持 `NOT_RUN`：真实方法 owner 读写、普通方法节点发布、条件 / 结果 / 动作入口关系、生命周期关系 23、内容版本发布、新增 / 替换 / 退出、当前 / 历史读取、并发、资源失败、发布未知、恢复、候选索引、普通应用真实运行、任务选择、方法执行、BIZ 消费和完整 `DATA-EXT-14`。

仓库外残留精确为：探针 `.cpp/.obj/.exe` 三文件，Debug / Release 各一份 `海中鱼巣.Build.CppClean.log` 与 `海中鱼巣.exe.recipe`，合计 7。它们不属于 `NOT_RUN`，而是已经发生验证后的未完成清理，不得写成“临时专项已清理”。
