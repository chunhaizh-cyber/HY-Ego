#define main 直接归属联合旧专项_main
#include "直接归属联合预算自由完整读取专项验证.cpp"
#undef main

namespace {
using namespace 海中鱼巣;

class 漏报根子组提供者 final : public 直接归属联合只读提供者 {
public:
  漏报根子组提供者(const 直接归属联合只读提供者 &base, 稳定编码 root)
      : base_(base), root_(root) {}
  bool 绑定于(const L1事实基座服务 &l1) const noexcept override {
    return base_.绑定于(l1);
  }
  直接归属联合读取结果
  读取当前联合父(const 直接归属联合父读取请求 &r) const override {
    return base_.读取当前联合父(r);
  }
  直接归属联合读取结果
  读取当前联合子组(const 直接归属联合子组读取请求 &r) const override {
    return base_.读取当前联合子组(r);
  }
  直接归属联合当前完整读取结果_v2
  读取当前联合父_v2(
      const 直接归属联合父当前完整读取请求_v2 &r) const override {
    return base_.读取当前联合父_v2(r);
  }
  直接归属联合当前完整读取结果_v2
  读取当前联合子组_v2(
      const 直接归属联合子组当前完整读取请求_v2 &r) const override {
    if (r.父 != root_)
      return base_.读取当前联合子组_v2(r);
    return {直接归属联合当前完整读取状态_v2::已读取,
            直接归属联合当前完整读取合同版本_v2, r.G0, std::nullopt, {}};
  }
  直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &r) const override {
    return base_.读取当前场景角色位置(r);
  }
  直接归属场景角色当前完整读取结果_v2
  读取当前场景角色位置_v2(
      const 直接归属场景角色当前完整读取请求_v2 &r) const override {
    return base_.读取当前场景角色位置_v2(r);
  }

private:
  const 直接归属联合只读提供者 &base_;
  稳定编码 root_{};
};

class 回边注入提供者 final : public 直接归属联合只读提供者 {
public:
  回边注入提供者(const 直接归属联合只读提供者 &base, 稳定编码 child,
                 稳定编码 root)
      : base_(base), child_(child), root_(root) {}
  bool 绑定于(const L1事实基座服务 &l1) const noexcept override {
    return base_.绑定于(l1);
  }
  直接归属联合读取结果
  读取当前联合父(const 直接归属联合父读取请求 &r) const override {
    return base_.读取当前联合父(r);
  }
  直接归属联合读取结果
  读取当前联合子组(const 直接归属联合子组读取请求 &r) const override {
    return base_.读取当前联合子组(r);
  }
  直接归属联合当前完整读取结果_v2
  读取当前联合父_v2(
      const 直接归属联合父当前完整读取请求_v2 &r) const override {
    return base_.读取当前联合父_v2(r);
  }
  直接归属联合当前完整读取结果_v2
  读取当前联合子组_v2(
      const 直接归属联合子组当前完整读取请求_v2 &r) const override {
    if (r.父 != child_)
      return base_.读取当前联合子组_v2(r);
    return {直接归属联合当前完整读取状态_v2::已读取,
            直接归属联合当前完整读取合同版本_v2,
            r.G0,
            std::nullopt,
            {{r.G0, 直接归属来源::存在组成,
              {0x7FFF'0000'0000'0001ULL}, child_, root_, 1}}};
  }
  直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &r) const override {
    return base_.读取当前场景角色位置(r);
  }
  直接归属场景角色当前完整读取结果_v2
  读取当前场景角色位置_v2(
      const 直接归属场景角色当前完整读取请求_v2 &r) const override {
    return base_.读取当前场景角色位置_v2(r);
  }

private:
  const 直接归属联合只读提供者 &base_;
  稳定编码 child_{}, root_{};
};

void 验证公开谓词() {
  直接归属场景角色当前完整读取结果_v2 role;
  role.状态 = 直接归属场景角色当前完整读取状态_v2::资源失败;
  role.Gread = 7;
  role.角色 = 直接归属场景角色事实{};
  检查(!role.成功({2, 7, {1}}), "scene-role-v2-non-success-payload-rejected");
  场景树当前完整读取结果_v2 tree;
  tree.状态 = 场景树当前完整读取状态_v2::资源失败;
  tree.Gread = 7;
  tree.树 = 场景树当前事实{};
  检查(!tree.成功({2, 7, {1}}), "scene-tree-v2-non-success-payload-rejected");
}

struct 真实大规模夹具 final {
  std::unique_ptr<L1事实基座运行包> 运行包;
  std::unique_ptr<特征值类数据服务> 特征值;
  std::unique_ptr<特征类数据服务> 特征;
  std::unique_ptr<存在类数据服务> 存在;
  std::unique_ptr<状态类数据服务> 状态;
  std::unique_ptr<场景类数据服务> 场景;
  std::unique_ptr<直接归属联合只读组合器> 联合;
  稳定编码 根{};

  真实大规模夹具() {
    运行包 = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &服务 = 运行包->读取服务();
    auto &签发器 = 运行包->所有者范围签发器();
    auto 特征定义所有者 = 建立所有者(签发器, 0xF101);
    auto 特征信息所有者 = 建立所有者(签发器, 0xF102);
    auto 存在所有者 = 建立所有者(签发器, 0xF103);
    auto 场景所有者 = 建立所有者(签发器, 0xF104);
    auto 状态所有者 = 建立所有者(签发器, 0xF105);

    const auto 生产者 = 建立元数据(
        *特征定义所有者.写入端口, 服务, 0xF111,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    std::vector<节点规格> 定义结构(16, {节点种类::普通, std::nullopt});
    for (const auto i : {2, 9, 13})
      定义结构[i] = {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (const auto i : {3, 8})
      定义结构[i] = {节点种类::属性类型, L1所有者范围值表示种类::I64};
    定义结构[14] = {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    (void)建立元数据(*特征定义所有者.写入端口, 服务, 1, 定义结构);
    (void)建立元数据(
        *特征信息所有者.写入端口, 服务, 1,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::普通, std::nullopt}});
    特征值 = std::make_unique<特征值类数据服务>(服务);
    特征 = std::make_unique<特征类数据服务>(
        服务, std::move(*特征定义所有者.写入端口),
        std::move(*特征信息所有者.写入端口), *特征值, 生产者.front());

    const auto 存在布局 = 建立元数据(
        *存在所有者.写入端口, 服务, 0xF121,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    const auto 存在族 = 建立元数据(
        *存在所有者.写入端口, 服务, 0x455849535446414DULL,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    const 存在单例角色结构登记请求 角色请求{1, 当前代次(服务), {0x1202}};
    const auto 角色 = 存在类数据服务::登记单例角色结构(
        服务, *存在所有者.写入端口, 角色请求);
    const 实例特征结构登记请求 实例特征请求{
        2, 当前代次(服务), {0x4946525354525632ULL}};
    const auto 实例特征 = 存在类数据服务::登记实例特征结构(
        服务, *存在所有者.写入端口, 实例特征请求);
    检查(角色.成功(角色请求) && 角色.交付 &&
             实例特征.成功(实例特征请求) && 实例特征.交付,
         "large-existence-layouts");

    constexpr std::uint32_t 节点数量 = 4098;
    constexpr std::uint32_t 每批节点数 = 256;
    std::map<std::uint64_t, 稳定编码> existenceMap;
    std::uint64_t 身份批次 = 0;
    for (std::uint32_t 首项 = 1; 首项 <= 节点数量; 首项 += 每批节点数) {
      const auto 数量 = std::min(每批节点数, 节点数量 - 首项 + 1);
      L1所有者范围写集请求 身份写集;
      身份写集.节点.reserve(数量);
      身份写集.关系.reserve(数量);
      for (std::uint32_t j = 1; j <= 数量; ++j) {
        身份写集.节点.push_back({{j}, 节点种类::普通, std::nullopt});
        身份写集.关系.push_back(
            {{数量 + j}, L1所有者范围写集本地键{j}, 存在族[0], 存在族[1], 1});
      }
      const auto 映射 = 提交写集(*存在所有者.写入端口, 服务,
                                 0xF200 + 身份批次++, std::move(身份写集));
      for (std::uint32_t j = 1; j <= 数量; ++j)
        existenceMap.emplace(首项 + j - 1, 映射.at(j));
    }
    根 = existenceMap.at(1);

    constexpr std::size_t 每批关系数 = 256;
    std::uint64_t 关系批次 = 0;
    L1所有者范围写集请求 关系写集;
    关系写集.关系.reserve(每批关系数);
    auto 提交关系批次 = [&] {
      if (关系写集.关系.empty())
        return;
      (void)提交写集(*存在所有者.写入端口, 服务, 0xF300 + 关系批次++,
                     std::move(关系写集));
      关系写集 = {};
      关系写集.关系.reserve(每批关系数);
    };
    auto 加关系 = [&](稳定编码 源, 稳定编码 目标, 稳定编码 类型) {
      const auto 本地键 = static_cast<std::uint32_t>(关系写集.关系.size() + 1);
      关系写集.关系.push_back({{本地键}, 源, 目标, 类型, 1});
      if (关系写集.关系.size() == 每批关系数)
        提交关系批次();
    };
    for (std::uint32_t i = 1; i < 节点数量; ++i)
      加关系(根, existenceMap.at(i + 1), 存在布局[0]);
    提交关系批次();

    const auto 状态布局 = 建立元数据(
        *状态所有者.写入端口, 服务, 0xF131,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::U64组},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64}});
    状态 = std::make_unique<状态类数据服务>(
        服务, *特征, std::move(*状态所有者.写入端口),
        状态类结构交付{状态布局[0], 状态布局[1], 状态布局[2], 状态布局[3],
                       状态布局[4], 状态布局[5], 状态布局[6], 状态布局[7]});

    场景结构登记请求 场景请求{2, 当前代次(服务), {1}};
    const auto 场景结构 = 场景类数据服务::登记结构类型(
        服务, *场景所有者.写入端口, 场景请求);
    场景特征组织扩展登记请求 特征组织请求{1, 当前代次(服务)};
    const auto 特征组织 = 场景类数据服务::登记特征组织扩展(
        服务, *场景所有者.写入端口, 特征组织请求);
    场景直接包含扩展登记请求 包含请求{1, 当前代次(服务)};
    const auto 包含 = 场景类数据服务::登记直接包含扩展(
        服务, *场景所有者.写入端口, 包含请求);
    检查(场景结构.成功(场景请求) && 场景结构.交付 &&
             特征组织.成功(特征组织请求) && 特征组织.交付 &&
             包含.成功(包含请求) && 包含.交付,
         "large-scene-layouts");

    存在 = std::make_unique<存在类数据服务>(
        服务, *特征, std::move(*存在所有者.写入端口), 存在布局[0], 存在布局[1],
        存在当前采用结构交付{存在布局[2]}, *实例特征.交付, *角色.交付);
    场景 = std::make_unique<场景类数据服务>(
        服务, std::move(*场景所有者.写入端口), *场景结构.交付, *存在, *状态,
        *特征组织.交付, *包含.交付);
    联合 = std::make_unique<直接归属联合只读组合器>(*存在, *场景);
    const 场景树根启用请求 enable{1, 当前代次(服务), {0xF141}, 根, 4096,
                                      4096};
    const auto rootIdentity = 存在->确认当前存在结构身份(enable.G0, 根);
    std::cout << "INFO large-scene-root-input version=" << enable.版本
              << " G0=" << enable.G0 << " root=" << enable.对象存在.值
              << " relation-budget=" << enable.最大关系数量
              << " ancestor-budget=" << enable.最大祖先数量
              << " bound=" << 联合->绑定于(服务)
              << " identity-status=" << static_cast<unsigned>(rootIdentity.状态)
              << " identity-G=" << rootIdentity.Gread << '\n';
    const auto enabled = 场景->启用并建立场景树根(enable, *联合);
    std::cout << "INFO large-scene-root status="
              << static_cast<unsigned>(enabled.结果头.状态)
              << " Gread=" << enabled.结果头.Gread << '\n';
    检查(enabled.建根成功(enable), "large-scene-root-enabled");
  }

  std::uint64_t G() const { return 当前代次(运行包->读取服务()); }
};

void 验证真实根与大规模遍历(夹具 &x) {
  const auto root = x.E[3];
  检查(x.场景->读取当前场景树_v2({1, x.G(), root}, *x.联合).状态 ==
             场景树当前完整读取状态_v2::入口拒绝,
         "scene-tree-v2-version-rejected");
  检查(x.场景->读取当前场景树_v2({2, x.G(), {0xFFFF'FFFF'FFFF'FF11ULL}},
                                    *x.联合)
             .状态 == 场景树当前完整读取状态_v2::根未找到,
         "scene-tree-v2-root-missing");
  检查(x.场景->读取当前场景树_v2({2, x.G(), root}, *x.联合).状态 ==
             场景树当前完整读取状态_v2::根角色未启用,
         "scene-tree-v2-root-role-not-enabled");
  检查(x.场景->读取当前场景树_v2({2, x.G() - 1, root}, *x.联合).状态 ==
             场景树当前完整读取状态_v2::事实代次漂移,
         "scene-tree-v2-generation-drift");
  const 场景树根启用请求 enable{1, x.G(), {0xE201}, root, 4096, 4096};
  const auto enabled = x.场景->启用并建立场景树根(enable, *x.联合);
  检查(enabled.建根成功(enable), "scene-root-enabled");

  const 场景树当前完整读取请求_v2 request{2, x.G(), root};
  const auto direct = x.场景->读取当前场景树_v2(request, *x.联合);
  检查(direct.成功(request) && direct.树 && direct.树->场景组.size() == 1,
       "scene-tree-v2-real-root-read");

  const auto child = x.E[6];
  const 场景直接子场景启用请求 enableChild{
      1, x.G(), {0xE202}, child, root, 4096, 4096};
  const auto childEnabled =
      x.场景->启用并接纳直接子场景(enableChild, *x.联合);
  检查(childEnabled.子场景纳入成功(enableChild), "scene-child-enabled");
  const 场景树当前完整读取请求_v2 childTreeRequest{2, x.G(), root};
  const auto childTree =
      x.场景->读取当前场景树_v2(childTreeRequest, *x.联合);
  检查(childTree.成功(childTreeRequest) && childTree.树 &&
             childTree.树->场景组.size() == 2,
         "scene-tree-v2-real-child-context-read");
  检查(x.场景->读取当前场景树_v2({2, x.G(), child}, *x.联合).状态 ==
             场景树当前完整读取状态_v2::根角色未启用,
         "scene-tree-v2-non-root-rejected-as-root");
  漏报根子组提供者 omitted{*x.联合, root};
  const auto incomplete = x.场景->读取当前场景树_v2(childTreeRequest, omitted);
  检查(incomplete.状态 == 场景树当前完整读取状态_v2::引用冲突 &&
             !incomplete.树,
         "scene-tree-v2-rejects-omitted-authoritative-child");
  回边注入提供者 cycle{*x.联合, child, root};
  const auto cyclic = x.场景->读取当前场景树_v2(childTreeRequest, cycle);
  检查(cyclic.状态 == 场景树当前完整读取状态_v2::形成场景环 && !cyclic.树,
         "scene-tree-v2-classifies-ancestor-back-edge-as-cycle");
}

void 验证真实4097结构边() {
  真实大规模夹具 x;
  const 场景树当前完整读取请求_v2 request{2, x.G(), x.根};
  const auto result = x.场景->读取当前场景树_v2(request, *x.联合);
  检查(result.成功(request) && result.树 && result.树->场景组.size() == 1,
       "scene-tree-v2-real-4097-structure-edges");
}
} // namespace

int main() {
  try {
    std::cout << std::unitbuf;
    验证公开谓词();
    夹具 data;
    验证真实根与大规模遍历(data);
    验证真实4097结构边();
    std::cout << "PASS total=" << 通过数量 << '\n';
    return 0;
  } catch (const std::exception &error) {
    std::cerr << "FAIL " << error.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "FAIL unexpected exception\n";
    return 2;
  }
}
