# DATA-L2 普通方法执行规格引用身份闭合检查记录

日期：2026-08-16

计划：`DATA-L2-ORDINARY-METHOD-SPEC-REFERENCE-CLOSURE v0.1`

执行起点：`8a6c14289af5bcec2727ad9d97308d88642cdd73`

## 1. 结论

仓库外 MSVC C++20 头探针、根工程 x64 Debug / Release fresh Rebuild、工程 XML、唯一登记、四配置 `/utf-8`、严格 UTF-8、diff、strict 和生产零测试痕迹检查通过。仓库外临时目录清理由工具策略阻断，`363` 个文件和 `14` 个子目录仍待人工清理，因此清理项不通过。

这些检查只证明本叶五类引用 ABI 在已覆盖分支符合冻结合同；不证明真实方法结构、服务、执行或 `DATA-EXT-14` 已完成。

## 2. 检查矩阵

| 检查项 | 结果 | 证据边界 |
| --- | --- | --- |
| 合同版本 | PASS | 既有普通方法结构合同版本和新增规格引用合同版本均为 `1` |
| 五种类型零 / 非零 | PASS | 四种身份默认零编码无效、非零有效；动作键默认 `0` 无效、非零有效 |
| 显式构造与类型隔离 | PASS | 四种身份可从 `稳定编码` 显式构造，彼此及与既有方法身份不可隐式转换 |
| 动作键隔离 | PASS | 动作键与裸整数、方法身份、动作入口身份不可隐式互换 |
| 相等合同 | PASS | 五种类型默认逐字段相等成立 |
| 仓库外头探针 | PASS | 最终 compile exit `0`、run exit `0`；探针未登记生产工程 |
| 根工程 x64 Debug fresh Rebuild | PASS | 仓库外独立 `OutDir / IntDir`，完整扫描、编译、链接，exit `0` |
| 根工程 x64 Release fresh Rebuild | PASS | 仓库外独立 `OutDir / IntDir`，完整扫描、编译、链接，exit `0` |
| 工程 / filters | PASS | XML 可解析；目标头登记 `1/1`；四配置 `/utf-8` 为 `4/4` |
| UTF-8 / BOM | PASS | 生产头严格 UTF-8、无 BOM |
| diff / strict | PASS | 生产头 `git diff --check`；strict `113/113` |
| 生产零测试痕迹 | PASS | 生产头对测试、自检、专项、探针、TEST、mock、fake 扫描命中 `0` |
| 临时目录清理 | BLOCKED | 精确目录递归删除在进程启动前被工具策略拒绝；`363` 文件、`14` 子目录仍存在 |

## 3. 实际命令形状与失败分账

头探针使用 MSVC C++20 `/W4 /WX /utf-8 /permissive-` 编译并运行。首轮因探针用 `{}` 触发显式身份转换拒绝而 compile exit `2`；只修探针后 compile / run 均 exit `0`，生产 ABI 未降级。

根工程使用 `海中鱼巣.slnx`、`/t:Rebuild /m:1` 和仓库外 `OutDir / IntDir` 分别构建 Debug / Release。Debug 第一次因驱动引号拼接错误在编译前返回 `MSB4184`；改用参数数组后双配置均 exit `0`。

治理检查包括：

```text
git diff --check -- 海中鱼巣/领域/L2普通方法结构.数据.h
python .\tools\check_specs.py --strict
vcxproj / filters XML、唯一登记、四配置 /utf-8、严格 UTF-8、BOM、白名单与生产零测试痕迹检查
```

## 4. NOT_RUN

真实绑定 / 参数 / 禁止项 / 适用范围节点与关系、完整普通方法 DTO、owner、服务、CRUD、并发、资源失败、发布未知、恢复、候选索引、动作映射、普通应用、任务冻结、方法执行、BIZ 消费和完整 `DATA-EXT-14` 均为 `NOT_RUN`。本记录不是独立验收结论；最终适用性只在合法消费者实际使用对应公开服务时判断。
