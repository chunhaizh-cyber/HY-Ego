module;

#include <cstdint>

export module 海中鱼巣.装配.普通应用;

export namespace 海中鱼巣 {

enum class 普通应用装配状态 : std::uint8_t {
  已装配 = 1,
  入口拒绝 = 2,
  所有者建立失败 = 3,
  元结构建立失败 = 4,
  服务建立失败 = 5,
  世界树根失败 = 6,
  世界树应用失败 = 7,
  资源失败 = 8,
  内部不一致 = 9
};

struct 普通应用装配结果 final {
  普通应用装配状态 状态 = 普通应用装配状态::入口拒绝;
  std::uint64_t 根事实代次 = 0;
  bool 成功() const noexcept {
    return 状态 == 普通应用装配状态::已装配 && 根事实代次;
  }
};

普通应用装配结果 构造普通应用上下文() noexcept;

} // namespace 海中鱼巣
