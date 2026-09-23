#include "../海中鱼巣/领域/数据服务.方法登记根.h"
#include "../海中鱼巣/业务/应用服务.方法登记根生产初始化.h"

#include <charconv>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace {
using namespace 海中鱼巣;

static_assert(
    方法登记根生产初始化请求{1}.最大空域核验事实数 ==
    方法登记根生产默认最大空域核验事实数);
static_assert(
    L2方法登记根初始化请求{
        1, 1, 方法登记根首次写入幂等身份}
        .最大空域核验事实数 == 0);

int 通过数 = 0;

void 要求(bool 成立, std::string_view 名称) {
  if (!成立) {
    std::cerr << "FAIL " << 名称 << '\n';
    throw std::runtime_error("method registry root budget validation failed");
  }
  ++通过数;
  std::cout << "PASS " << 名称 << '\n';
}

[[noreturn]] void 夹具失败(const char *消息) {
  throw std::runtime_error(消息);
}

std::uint64_t 当前代次(const L1事实基座服务 &服务) {
  const auto 结果 = 服务.读取中性当前事实代次({L1中性CRUD合同版本});
  if (结果.状态 != L1中性读取状态::成功 || !结果.事实代次)
    夹具失败("fixture generation read failed");
  return 结果.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器 &签发器,
                              const L1事实基座服务 &服务,
                              std::uint64_t 建立键) {
  auto 交付 = 签发器.建立所有者范围(
      {L1所有者范围CRUD合同版本, {建立键},
       L1所有者范围种类::独占结构范围});
  if ((交付.建立结果.状态 != L1所有者范围管理状态::成功 &&
       交付.建立结果.状态 != L1所有者范围管理状态::精确重复) ||
      !交付.建立结果.所有者事实 || !交付.写入端口 ||
      !交付.写入端口->有效() || !交付.写入端口->绑定于(服务))
    夹具失败("fixture owner creation failed");
  return 交付;
}

struct 权威表计数 final {
  std::uint64_t 当前节点 = 0;
  std::uint64_t 当前关系 = 0;
  std::uint64_t 当前值 = 0;
  std::uint64_t 历史 = 0;
  std::uint64_t 墓碑 = 0;

  std::uint64_t 合计() const {
    std::uint64_t 总数 = 0;
    for (const auto 数量 : {当前节点, 当前关系, 当前值, 历史, 墓碑}) {
      if (数量 > std::numeric_limits<std::uint64_t>::max() - 总数)
        夹具失败("fixture fact count overflow");
      总数 += 数量;
    }
    return 总数;
  }
};

权威表计数 发布普通节点(L1所有者范围写端口 &端口,
                           const L1事实基座服务 &服务,
                           std::uint64_t 幂等键,
                           std::uint64_t 节点数) {
  L1所有者范围写集请求 请求{
      L1所有者范围CRUD合同版本, 当前代次(服务), {幂等键}};
  if (节点数 > std::numeric_limits<std::uint32_t>::max())
    夹具失败("fixture local key range exceeded");
  for (std::uint32_t i = 0; i < 节点数; ++i)
    请求.节点.push_back(
        {{i + 1}, 节点种类::普通, std::nullopt});
  const auto 结果 = 端口.提交所有者范围中性写集(请求);
  if ((结果.状态 != L1所有者范围写入状态::成功 &&
       结果.状态 != L1所有者范围写入状态::精确重复) ||
      结果.新编码映射.size() != 节点数)
    夹具失败("fixture node publication failed");
  return {节点数, 0, 0, 0, 0};
}

struct 内存夹具 final {
  std::unique_ptr<L1事实基座运行包> 运行包;
  L1所有者范围交付 方法所有者;
  L1所有者范围交付 填充所有者;
  权威表计数 计数;
  std::unique_ptr<L2方法结构聚合服务> 聚合;
  std::unique_ptr<方法登记根生产初始化提供者> 提供者;

  explicit 内存夹具(std::uint64_t 填充节点数) {
    运行包 =
        std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &服务 = 运行包->读取服务();
    auto &签发器 = 运行包->所有者范围签发器();
    方法所有者 = 建立所有者(
        签发器, 服务, 方法登记根初始化所有者建立键);
    填充所有者 = 建立所有者(签发器, 服务, 0x4255'4447'4554'4631ULL);
    计数 = 发布普通节点(
        *填充所有者.写入端口, 服务, 0x4255'4447'4554'5731ULL,
        填充节点数);
    聚合 = std::make_unique<L2方法结构聚合服务>(
        服务, std::move(*方法所有者.写入端口));
    提供者 =
        std::make_unique<方法登记根生产初始化提供者>(*聚合);
  }

  L1事实基座服务 &服务() { return 运行包->读取服务(); }
  L2方法结构服务 &方法服务() {
    return 聚合->取得L2方法结构服务();
  }
};

void 验证入口拒绝() {
  内存夹具 夹具{4};
  const auto 初始G = 当前代次(夹具.服务());
  const auto 全组 = 夹具.方法服务().读取全部当前方法登记根({1, 0});
  要求(全组.状态 == 方法登记根结构状态::尚未建立,
       "entry-fixture-root-absent");

  const auto 错版本 = 夹具.方法服务().建立或读取方法登记根(
      {2, 全组.Gread, 方法登记根首次写入幂等身份, 4});
  const auto 零预算 = 夹具.方法服务().建立或读取方法登记根(
      {1, 全组.Gread, 方法登记根首次写入幂等身份});
  const auto 生产错版本 = 夹具.提供者->初始化({2, 4});
  const auto 生产零预算 = 夹具.提供者->初始化({1, 0});
  要求(错版本.状态 == 方法登记根结构状态::入口拒绝 &&
           零预算.状态 == 方法登记根结构状态::入口拒绝 &&
           生产错版本.状态 == 方法登记根生产初始化状态::入口拒绝 &&
           生产零预算.状态 == 方法登记根生产初始化状态::入口拒绝 &&
           当前代次(夹具.服务()) == 初始G,
       "entry-bad-version-and-zero-budget-zero-write");
}

void 验证预算不足建立与重复() {
  内存夹具 夹具{4};
  const auto 足够预算 = 夹具.计数.合计();
  要求(足够预算 == 4, "budget-derived-from-fixture-write-set");
  const auto 不足预算 = 足够预算 - 1;
  const auto 初始G = 当前代次(夹具.服务());
  const auto 全组 = 夹具.方法服务().读取全部当前方法登记根({1, 0});

  const auto L2不足 = 夹具.方法服务().建立或读取方法登记根(
      {1, 全组.Gread, 方法登记根首次写入幂等身份, 不足预算});
  要求(L2不足.状态 == 方法登记根结构状态::数量预算不足 &&
           L2不足.Gread == 初始G && L2不足.H == 0 && !L2不足.根 &&
           L2不足.请求回显.最大空域核验事实数 == 不足预算 &&
           当前代次(夹具.服务()) == 初始G,
       "l2-budget-short-by-one-zero-write");

  const auto 生产不足 = 夹具.提供者->初始化({1, 不足预算});
  要求(生产不足.状态 == 方法登记根生产初始化状态::数量预算不足 &&
           生产不足.最终Gread == 初始G && !生产不足.根 &&
           生产不足.请求回显.最大空域核验事实数 == 不足预算 &&
           当前代次(夹具.服务()) == 初始G,
       "provider-budget-short-by-one-zero-write");

  const auto 已建立 = 夹具.提供者->初始化({1, 足够预算});
  要求(已建立.成功() &&
           已建立.状态 == 方法登记根生产初始化状态::已初始化 &&
           已建立.根 && 当前代次(夹具.服务()) == 初始G + 1,
       "provider-exact-fixture-budget-created");

  const auto 建立后G = 当前代次(夹具.服务());
  const auto 根 = *已建立.根;
  const auto 重复 = 夹具.提供者->初始化({1, 1});
  要求(重复.成功() &&
           重复.状态 == 方法登记根生产初始化状态::精确重复 &&
           重复.根 && *重复.根 == 根 && 重复.最终Gread == 建立后G &&
           当前代次(夹具.服务()) == 建立后G,
       "different-nonzero-budget-exact-repeat-zero-write");
}

void 验证所有者未知结构冲突() {
  auto 运行包 = 建立L1事实基座运行包();
  auto &服务 = 运行包.读取服务();
  auto 方法所有者 = 建立所有者(
      运行包.所有者范围签发器(), 服务,
      方法登记根初始化所有者建立键);
  const auto 计数 = 发布普通节点(
      *方法所有者.写入端口, 服务, 0x554E'4B4E'4F57'4E31ULL, 1);
  const auto 写后G = 当前代次(服务);
  L2方法结构聚合服务 聚合{服务, std::move(*方法所有者.写入端口)};
  auto &方法服务 = 聚合.取得L2方法结构服务();
  const auto 全组 = 方法服务.读取全部当前方法登记根({1, 0});
  const auto 结果 = 方法服务.建立或读取方法登记根(
      {1, 全组.Gread, 方法登记根首次写入幂等身份, 计数.合计()});
  要求(结果.状态 == 方法登记根结构状态::所有者非空冲突 &&
           结果.Gread == 写后G && 结果.H == 0 && !结果.根 &&
           当前代次(服务) == 写后G,
       "unknown-method-owner-structure-conflict-zero-write");
}

void 验证事实代次漂移() {
  auto 运行包 = 建立L1事实基座运行包();
  auto &服务 = 运行包.读取服务();
  auto &签发器 = 运行包.所有者范围签发器();
  auto 方法所有者 = 建立所有者(
      签发器, 服务, 方法登记根初始化所有者建立键);
  auto 填充所有者 = 建立所有者(签发器, 服务, 0x4452'4946'544F'4631ULL);
  L2方法结构聚合服务 聚合{服务, std::move(*方法所有者.写入端口)};
  auto &方法服务 = 聚合.取得L2方法结构服务();
  const auto 旧全组 = 方法服务.读取全部当前方法登记根({1, 0});
  const auto 计数 = 发布普通节点(
      *填充所有者.写入端口, 服务, 0x4452'4946'5457'4631ULL, 1);
  const auto 写后G = 当前代次(服务);
  const auto 结果 = 方法服务.建立或读取方法登记根(
      {1, 旧全组.Gread, 方法登记根首次写入幂等身份, 计数.合计()});
  要求(结果.状态 == 方法登记根结构状态::事实代次漂移 &&
           结果.Gread == 写后G && 结果.H == 0 && !结果.根 &&
           当前代次(服务) == 写后G,
       "stale-generation-rejected-zero-write");
}

std::uint64_t 解析根编码(std::string_view 文本) {
  std::uint64_t 结果 = 0;
  const auto [末尾, 错误] =
      std::from_chars(文本.data(), 文本.data() + 文本.size(), 结果);
  if (错误 != std::errc{} || 末尾 != 文本.data() + 文本.size() ||
      结果 == 0)
    throw std::runtime_error("invalid expected root id");
  return 结果;
}

std::uint64_t 验证持久栈(const std::filesystem::path &根目录,
                         bool 首次建立,
                         std::optional<std::uint64_t> 期望根) {
  auto 持久 = 建立L1事实基座持久运行包_v1(
      {L1事实基座持久恢复合同版本_v1,
       std::filesystem::absolute(根目录)});
  要求(持久.成功() && 持久.运行包,
       首次建立 ? "persistent-seed-runtime" : "persistent-recover-runtime");
  auto &运行包 = *持久.运行包;
  auto &服务 = 运行包.读取服务();
  const auto 入口G = 首次建立 ? 0 : 当前代次(服务);
  auto &签发器 = 运行包.所有者范围签发器();
  auto 方法所有者 = 建立所有者(
      签发器, 服务, 方法登记根初始化所有者建立键);
  auto 填充所有者 = 建立所有者(签发器, 服务, 0x5052'5354'4F57'4E31ULL);
  const auto 计数 = 首次建立
      ? 发布普通节点(*填充所有者.写入端口, 服务,
            0x5052'5354'4641'4354ULL, 4)
      : 权威表计数{4, 0, 0, 0, 0};
  L2方法结构聚合服务 聚合{服务, std::move(*方法所有者.写入端口)};
  方法登记根生产初始化提供者 提供者{聚合};
  const auto 结果 = 提供者.初始化({1, 首次建立 ? 计数.合计() : 1});
  要求(结果.成功() && 结果.根 &&
           结果.状态 == (首次建立
               ? 方法登记根生产初始化状态::已初始化
               : 方法登记根生产初始化状态::精确重复),
       首次建立 ? "persistent-seed-root-created"
                : "persistent-recover-root-read");
  if (!首次建立) {
    要求(期望根 && 结果.根->身份.结点.值 == *期望根,
         "persistent-recover-same-root");
    要求(当前代次(服务) == 入口G,
         "persistent-recover-zero-write");
  }
  return 结果.根->身份.结点.值;
}

}  // namespace

int main(int argc, char **argv) {
  try {
    if (argc == 3 && std::string_view{argv[1]} == "seed") {
      const auto 根 = 验证持久栈(argv[2], true, std::nullopt);
      std::cout << "ROOT=" << 根 << '\n';
    } else if (argc == 4 && std::string_view{argv[1]} == "recover") {
      const auto 期望根 = 解析根编码(argv[3]);
      const auto 根 = 验证持久栈(argv[2], false, 期望根);
      std::cout << "ROOT=" << 根 << '\n';
    } else if (argc == 1) {
      验证入口拒绝();
      验证预算不足建立与重复();
      验证所有者未知结构冲突();
      验证事实代次漂移();
    } else {
      throw std::runtime_error("invalid validation arguments");
    }
    std::cout << "PASS total=" << 通过数 << '\n';
    return 0;
  } catch (const std::exception &异常) {
    std::cerr << 异常.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "unknown validation failure\n";
    return 2;
  }
}
