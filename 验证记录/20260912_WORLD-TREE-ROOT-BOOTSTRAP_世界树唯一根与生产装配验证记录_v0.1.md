# WORLD-TREE-ROOT-BOOTSTRAP 世界树唯一根与生产装配验证记录

日期：2026-09-12
计划起点 HEAD：`290b0e3ab8bd278efa77d6344ce95112377060e2`
隔离根：`D:/TEMP/海中鱼巣/WORLD-TREE-ROOT-BOOTSTRAP/01a06f6b-20260912`

## 自动验证结果

| 项目 | Debug | Release | 证据 |
| --- | --- | --- | --- |
| 世界树根启动专项 Rebuild | PASS | PASS | `special/<配置>/build.log` |
| 专项核心运行 | PASS，183 项 | PASS，183 项 | `special/<配置>/core-run.log` |
| 普通入口独立进程 | PASS，阶段 17 | PASS，阶段 17 | `special/<配置>/ordinary-run.log` |
| 接口/实现拆分与公开符号扫描 | PASS | PASS | `special/<配置>/result.json` |
| 根工程 v145 全新目录 Rebuild | PASS | PASS | `root-Debug-final/rebuild.log`、`root-Release-final/rebuild.log` |

专项覆盖空 L1 原子建根、原键零写重复、不同键第二根拒绝、同 G 并发最终唯一根、七节点结构、六节点旧材料拒绝、统一位置、预算不足、双边出生、已有 E 场景视角启退、v3 启退拒绝、存在成员移动、直接子场景四边迁移及原键重放、普通入口阶段 17。

## 分账与未证明边界

- PASS：并发探针证明同 G 竞争后权威根闭集恰一且至少一方成功。竞争方每一种允许状态及再次收敛没有确定性故障调度，因此更强分支记为 `NOT_RUN`。
- `NOT_RUN`：没有可注入的生产根参与者用于稳定制造提交前资源失败或提交后读回失败；未宣称覆盖这两个故障注入分支。
- `NOT_RUN`：持久恢复、多进程恢复、概念迁移、特征三类父、因果服务、自我形成和具名集成验收不属于本计划完成声明。
