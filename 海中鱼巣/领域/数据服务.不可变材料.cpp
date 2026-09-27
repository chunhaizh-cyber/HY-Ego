#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "数据服务.不可变材料.h"
#include <Windows.h>
#include <algorithm>
#include <bcrypt.h>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <string>
#pragma comment(lib, "bcrypt.lib")

namespace 海中鱼巣 {
namespace {
using Key = L1所有者范围写集本地键;
bool budget_ok(const 世界结构预算_B1 &b) noexcept {
  return 世界结构预算有效(b);
}
bool within(const 世界结构用量_B1 &u, const 世界结构预算_B1 &b) noexcept {
  return u.最大节点数 <= b.最大节点数 && u.最大关系数 <= b.最大关系数 &&
         u.最大值数 <= b.最大值数 && u.最大祖先数 <= b.最大祖先数 &&
         u.最大后代数 <= b.最大后代数 && u.最大候选数 <= b.最大候选数 &&
         u.最大值元素数 <= b.最大值元素数 &&
         u.最大材料字节数 <= b.最大材料字节数 &&
         u.最大域原子数 <= b.最大域原子数;
}
材料状态_B1 map_read(L1所有者范围读取状态 s) noexcept {
  if (s == L1所有者范围读取状态::未找到)
    return 材料状态_B1::未找到;
  if (s == L1所有者范围读取状态::资源失败)
    return 材料状态_B1::资源失败;
  return 材料状态_B1::内部不一致;
}
材料状态_B1 map_consistent(L1所有者范围一致当前读取状态 s) noexcept {
  return s == L1所有者范围一致当前读取状态::资源失败 ? 材料状态_B1::资源失败
                                                     : 材料状态_B1::内部不一致;
}
材料状态_B1 map_write(L1所有者范围写入状态 s) noexcept {
  if (s == L1所有者范围写入状态::幂等冲突)
    return 材料状态_B1::幂等冲突;
  if (s == L1所有者范围写入状态::引用冲突)
    return 材料状态_B1::引用保护;
  if (s == L1所有者范围写入状态::资源失败)
    return 材料状态_B1::资源失败;
  return 材料状态_B1::内部不一致;
}

std::array<std::uint8_t, 32> sha256(const std::vector<std::uint8_t> &v) {
  BCRYPT_ALG_HANDLE a{};
  BCRYPT_HASH_HANDLE h{};
  DWORD n{}, got{};
  std::vector<std::uint8_t> obj;
  std::array<std::uint8_t, 32> out{};
  if (BCryptOpenAlgorithmProvider(&a, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0)
    throw std::runtime_error("sha");
  try {
    if (BCryptGetProperty(a, BCRYPT_OBJECT_LENGTH, reinterpret_cast<PUCHAR>(&n),
                          sizeof(n), &got, 0) < 0)
      throw std::runtime_error("sha");
    obj.resize(n);
    if (BCryptCreateHash(a, &h, obj.data(), n, nullptr, 0, 0) < 0)
      throw std::runtime_error("sha");
    if (!v.empty() && BCryptHashData(h, const_cast<PUCHAR>(v.data()),
                                     static_cast<ULONG>(v.size()), 0) < 0)
      throw std::runtime_error("sha");
    if (BCryptFinishHash(h, out.data(), static_cast<ULONG>(out.size()), 0) < 0)
      throw std::runtime_error("sha");
    BCryptDestroyHash(h);
    BCryptCloseAlgorithmProvider(a, 0);
    return out;
  } catch (...) {
    if (h)
      BCryptDestroyHash(h);
    BCryptCloseAlgorithmProvider(a, 0);
    throw;
  }
}
std::string hex(const std::array<std::uint8_t, 32> &d) {
  static constexpr char h[] = "0123456789abcdef";
  std::string s;
  s.reserve(64);
  for (auto x : d) {
    s.push_back(h[x >> 4]);
    s.push_back(h[x & 15]);
  }
  return s;
}
std::filesystem::path material_path(const std::filesystem::path &d,
                                    不可变材料格式身份_B1 f,
                                    const std::array<std::uint8_t, 32> &h) {
  return d / (std::to_string(f.值.值) + "-" + hex(h) + ".hyb");
}
std::filesystem::path handle_path(HANDLE h) {
  const auto n = GetFinalPathNameByHandleW(
      h, nullptr, 0, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
  if (!n)
    throw std::runtime_error("path");
  std::wstring s(n, L'\0');
  const auto w = GetFinalPathNameByHandleW(
      h, s.data(), n, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
  if (!w || w >= n)
    throw std::runtime_error("path");
  s.resize(w);
  if (s.rfind(L"\\\\?\\", 0) == 0)
    s.erase(0, 4);
  return std::filesystem::path(s).lexically_normal();
}
bool same_path(const std::filesystem::path &a,
               const std::filesystem::path &b) noexcept {
  return _wcsicmp(a.c_str(), b.c_str()) == 0;
}
HANDLE secure_root(const std::filesystem::path &input,
                   std::filesystem::path &resolved) {
  const auto root = std::filesystem::absolute(input).lexically_normal();
  if (!root.is_absolute())
    throw std::invalid_argument("root");
  auto cur = root.root_path();
  for (const auto &p : root.relative_path()) {
    cur /= p;
    auto x = GetFileAttributesW(cur.c_str());
    if (x == INVALID_FILE_ATTRIBUTES &&
        !CreateDirectoryW(cur.c_str(), nullptr) &&
        GetLastError() != ERROR_ALREADY_EXISTS)
      throw std::runtime_error("root");
    x = GetFileAttributesW(cur.c_str());
    if (x == INVALID_FILE_ATTRIBUTES || (x & FILE_ATTRIBUTE_DIRECTORY) == 0 ||
        (x & FILE_ATTRIBUTE_REPARSE_POINT))
      throw std::runtime_error("root");
  }
  HANDLE h = CreateFileW(
      root.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
      nullptr, OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
  if (h == INVALID_HANDLE_VALUE)
    throw std::runtime_error("root");
  try {
    FILE_ATTRIBUTE_TAG_INFO t{};
    if (!GetFileInformationByHandleEx(h, FileAttributeTagInfo, &t, sizeof(t)) ||
        (t.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
      throw std::runtime_error("root");
    resolved = handle_path(h);
    if (!same_path(resolved, root))
      throw std::runtime_error("root");
    return h;
  } catch (...) {
    CloseHandle(h);
    throw;
  }
}
void require_file(HANDLE h, const std::filesystem::path &root) {
  FILE_ATTRIBUTE_TAG_INFO t{};
  if (!GetFileInformationByHandleEx(h, FileAttributeTagInfo, &t, sizeof(t)) ||
      (t.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) ||
      !same_path(handle_path(h).parent_path(), root))
    throw std::runtime_error("path");
}
std::vector<std::uint8_t> read_file(const std::filesystem::path &root,
                                    const std::filesystem::path &p,
                                    std::uint64_t max) {
  HANDLE h = CreateFileW(
      p.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
  if (h == INVALID_HANDLE_VALUE)
    throw std::runtime_error("file");
  try {
    require_file(h, root);
    LARGE_INTEGER z{};
    if (!GetFileSizeEx(h, &z) || z.QuadPart <= 0 ||
        static_cast<std::uint64_t>(z.QuadPart) > max || z.QuadPart > 16'777'216)
      throw std::length_error("file");
    std::vector<std::uint8_t> v(static_cast<std::size_t>(z.QuadPart));
    std::size_t at = 0;
    while (at < v.size()) {
      DWORD got{};
      auto n = static_cast<DWORD>(
          (std::min)(v.size() - at, static_cast<std::size_t>(
                                        (std::numeric_limits<DWORD>::max)())));
      if (!ReadFile(h, v.data() + at, n, &got, nullptr) || got == 0)
        throw std::runtime_error("file");
      at += got;
    }
    CloseHandle(h);
    return v;
  } catch (...) {
    CloseHandle(h);
    throw;
  }
}
bool valid_wire(const std::vector<std::uint8_t> &v, 不可变材料格式种类_B1 k) {
  if (v.size() < 32 || v[0] != 'H' || v[1] != 'Y' || v[2] != 'B' || v[3] != '1')
    return false;
  auto le = [&](std::size_t at, unsigned n) {
    std::uint64_t x = 0;
    for (unsigned i = 0; i < n; ++i)
      x |= std::uint64_t(v[at + i]) << (8 * i);
    return x;
  };
  if (le(4, 4) != 1 || le(8, 8) != static_cast<std::uint8_t>(k))
    return false;
  auto n = le(16, 8);
  return n && n <= ((std::numeric_limits<std::size_t>::max)() - 24) / 8 &&
         24 + 8 * n == v.size();
}

L1所有者范围写集请求 original_a_set() {
  L1所有者范围写集请求 w;
  w.写入幂等身份 = {1};
  for (std::uint32_t i = 1; i <= 16; ++i) {
    std::optional<L1所有者范围值表示种类> r;
    if (i == 3 || i == 10 || i == 14)
      r = L1所有者范围值表示种类::U64组;
    else if (i == 4 || i == 9)
      r = L1所有者范围值表示种类::I64;
    else if (i == 15)
      r = L1所有者范围值表示种类::I64组;
    w.节点.push_back({{i}, r ? 节点种类::属性类型 : 节点种类::普通, r});
  }
  return w;
}
L1所有者范围写集请求 registration_set(稳定编码 a) {
  L1所有者范围写集请求 w;
  w.写入幂等身份 = {0x424D41544C415931ULL};
  for (std::uint32_t i = 1; i <= 13; ++i) {
    std::optional<L1所有者范围值表示种类> r;
    if (i == 4 || i == 8 || i == 11)
      r = L1所有者范围值表示种类::I64;
    else if (i == 9)
      r = L1所有者范围值表示种类::U64组;
    w.节点.push_back({{i}, r ? 节点种类::属性类型 : 节点种类::普通, r});
  }
  for (std::uint32_t i = 3; i <= 11; ++i)
    w.关系.push_back(
        {{11 + i}, Key{1}, Key{i}, Key{3}, static_cast<std::int64_t>(i)});
  w.关系.push_back({{23}, Key{1}, a, Key{10}, 1});
  w.值.push_back({{24}, Key{1}, Key{4}, std::int64_t{1}, Key{1}});
  w.属性槽变更.push_back({Key{1}, Key{4}, Key{24}});
  w.值.push_back({{25}, Key{12}, Key{11}, std::int64_t{1}, Key{12}});
  w.属性槽变更.push_back({Key{12}, Key{11}, Key{25}});
  w.值.push_back({{26}, Key{13}, Key{11}, std::int64_t{2}, Key{13}});
  w.属性槽变更.push_back({Key{13}, Key{11}, Key{26}});
  return w;
}
L1所有者范围写集请求 publish_set(const 材料发布请求_B1 &r,
                                 const 不可变材料布局_B1 &l,
                                 const std::array<std::uint8_t, 32> &d) {
  L1所有者范围写集请求 w;
  w.写入幂等身份 = r.幂等身份;
  w.节点.push_back({{1}, 节点种类::普通, std::nullopt});
  w.关系 = {{Key{2}, l.族锚点, Key{1}, l.族成员, 1},
            {Key{3}, Key{1}, r.格式.值, l.材料格式, 1},
            {Key{4}, Key{1}, r.来源, l.来源, 1}};
  std::vector<std::uint64_t> x;
  for (auto b : d)
    x.push_back(b);
  w.值 = {{Key{5}, Key{1}, l.字节长度, std::int64_t(r.完整载荷.size()), Key{1}},
          {Key{6}, Key{1}, l.摘要, std::move(x), Key{1}}};
  w.属性槽变更 = {{Key{1}, l.字节长度, Key{5}}, {Key{1}, l.摘要, Key{6}}};
  return w;
}
bool mapping(const L1所有者范围写入结果 &r, std::size_t n,
             std::vector<稳定编码> &out) {
  out.assign(n, {});
  std::vector<bool> seen(n);
  if (r.新编码映射.size() != n)
    return false;
  for (const auto &[k, v] : r.新编码映射) {
    if (!k.值 || k.值 > n || seen[k.值 - 1] || !有效(v))
      return false;
    seen[k.值 - 1] = true;
    out[k.值 - 1] = v;
  }
  return true;
}
bool nodes_complete(const 原A特征定义承接事实_B1 &f, 稳定编码 anchor) noexcept {
  if (f.元节点[0].编码 != anchor)
    return false;
  for (std::size_t i = 0; i < f.元节点.size(); ++i) {
    const auto &n = f.元节点[i];
    if (!有效(n.编码))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (n.编码 == f.元节点[j].编码)
        return false;
    std::optional<L1所有者范围值表示种类> r;
    auto k = i + 1;
    if (k == 3 || k == 10 || k == 14)
      r = L1所有者范围值表示种类::U64组;
    else if (k == 4 || k == 9)
      r = L1所有者范围值表示种类::I64;
    else if (k == 15)
      r = L1所有者范围值表示种类::I64组;
    if (n.种类 != (r ? 节点种类::属性类型 : 节点种类::普通) || n.属性表示 != r)
      return false;
  }
  return true;
}
bool fact_complete(const 材料事实_B1 &m) {
  return 有效(m.材料) && 有效(m.格式) && 有效(m.族关系) && 有效(m.格式关系) &&
         有效(m.来源关系) && 有效(m.来源) && 有效(m.长度值) && 有效(m.摘要值) &&
         m.字节长度 == m.完整载荷.size() && sha256(m.完整载荷) == m.SHA256 &&
         m.格式见证.格式 == m.格式 && valid_wire(m.完整载荷, m.格式见证.种类);
}
bool read_ok(const 材料读取结果_B1 &o, const 材料读取请求_B1 &r) {
  return r.版本 == 1 && 有效(r.材料) && budget_ok(r.预算) &&
         o.状态 == 材料状态_B1::已读取 && o.材料 && o.材料->材料 == r.材料 &&
         fact_complete(*o.材料) && within(o.用量, r.预算);
}
} // namespace

原A特征定义承接读取结果_B1 不可变材料数据服务::读取原A特征定义承接_B1(
    const 原A特征定义承接读取请求_B1 &r, const L1事实基座服务 &l,
    const L1所有者范围写端口 &p) noexcept {
  原A特征定义承接读取结果_B1 o;
  try {
    if (r.版本 != 1 || !有效(r.预期锚点) || !budget_ok(r.预算) || !p.有效() ||
        !p.绑定于(l))
      return o;
    const auto first = p.读取首次写入材料({{1}});
    if (first.状态 == L1所有者范围读取状态::未找到) {
      o.状态 = 材料状态_B1::未找到;
      return o;
    }
    if (first.状态 != L1所有者范围读取状态::成功) {
      o.状态 = map_read(first.状态);
      return o;
    }
    if (!first.首次规范化写集 || !first.首次写入结果 ||
        *first.首次规范化写集 != original_a_set() ||
        first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
        !first.首次写入结果->是否形成内存权威发布) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    std::vector<稳定编码> ids;
    if (!mapping(*first.首次写入结果, 16, ids) || ids[0] != r.预期锚点) {
      o.状态 = 材料状态_B1::幂等冲突;
      return o;
    }
    L1所有者范围一致当前读取请求 q;
    q.所有者 = {p.所有者身份()};
    q.节点 = ids;
    const auto x = l.尝试读取所有者范围一致当前投影(q);
    if (x.状态 != L1所有者范围一致当前读取状态::成功) {
      o.状态 = map_consistent(x.状态);
      return o;
    }
    if (x.节点.size() != 16) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    原A特征定义承接事实_B1 f;
    for (std::size_t i = 0; i < 16; ++i) {
      const auto &n = x.节点[i];
      if (n.状态 != L1所有者范围一致当前读取项目状态::成功 || !n.事实 ||
          n.事实->写入所有者 != p.所有者身份()) {
        o.状态 = 材料状态_B1::未找到;
        return o;
      }
      f.元节点[i] = {n.事实->编码, n.事实->种类, n.事实->属性类型表示};
    }
    if (!nodes_complete(f, r.预期锚点)) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    o.用量.最大节点数 = 16;
    if (!within(o.用量, r.预算)) {
      o.状态 = 材料状态_B1::数量预算不足;
      return o;
    }
    o.承接 = std::move(f);
    o.状态 = 材料状态_B1::已读取;
    return o;
  } catch (const std::bad_alloc &) {
    o.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    o.状态 = 材料状态_B1::内部不一致;
    return o;
  }
}

不可变材料登记结果_B1 不可变材料数据服务::登记不可变材料结构_B1(
    const 不可变材料登记请求_B1 &r, L1事实基座服务 &l, L1所有者范围写端口 &p,
    const L1所有者范围写端口 &a) noexcept {
  不可变材料登记结果_B1 o;
  try {
    if (r.版本 != 1 || r.幂等身份.值 != 0x424D41544C415931ULL ||
        !有效(r.A承接锚点) || !budget_ok(r.预算) || !p.有效() || !p.绑定于(l) ||
        !a.有效() || !a.绑定于(l) || p.所有者身份() == a.所有者身份())
      return o;
    const auto expected = registration_set(r.A承接锚点);
    const auto first = p.读取首次写入材料({r.幂等身份});
    L1所有者范围写入结果 saved;
    bool replay = false;
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !first.首次写入结果 ||
          *first.首次规范化写集 != expected) {
        o.状态 = 材料状态_B1::幂等冲突;
        return o;
      }
      saved = *first.首次写入结果;
      replay = true;
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
      const auto empty = l.读取所有者范围完整空域({p.所有者身份()});
      if (empty.状态 != L1所有者范围空域完整读取状态::成功 || !empty.空域 ||
          !*empty.空域) {
        o.状态 = empty.状态 == L1所有者范围空域完整读取状态::资源失败
                     ? 材料状态_B1::资源失败
                     : 材料状态_B1::旧格式不支持;
        return o;
      }
      const auto proof = 读取原A特征定义承接_B1({1, r.A承接锚点, r.预算}, l, a);
      o.A承接 = proof.承接;
      o.用量 = proof.用量;
      if (proof.状态 != 材料状态_B1::已读取 || !proof.承接) {
        o.状态 = proof.状态;
        return o;
      }
      o.发布 = 材料发布状态_B1::可能发布;
      saved = p.提交所有者范围中性写集(expected);
    } else {
      o.状态 = map_read(first.状态);
      return o;
    }
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      o.状态 = map_write(saved.状态);
      return o;
    }
    std::vector<稳定编码> id;
    if (!mapping(saved, 26, id)) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    o.布局 = 不可变材料布局_B1{id[0],  id[1],    id[2],    id[3],      id[4],
                               id[5],  id[6],    id[7],    id[8],      id[9],
                               id[10], {id[11]}, {id[12]}, r.A承接锚点};
    if (!o.A承接) {
      const auto proof = 读取原A特征定义承接_B1({1, r.A承接锚点, r.预算}, l, a);
      o.A承接 = proof.承接;
      if (proof.状态 != 材料状态_B1::已读取) {
        o.状态 = proof.状态;
        return o;
      }
    }
    o.发布 = 材料发布状态_B1::确认发布;
    o.状态 = replay ? 材料状态_B1::精确重复 : 材料状态_B1::已创建;
    return o;
  } catch (const std::bad_alloc &) {
    o.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    o.状态 = o.发布 == 材料发布状态_B1::可能发布 ? 材料状态_B1::可能已发布
                                                 : 材料状态_B1::内部不一致;
    return o;
  }
}

不可变材料数据服务::不可变材料数据服务(const L1事实基座服务 &l,
                                       L1所有者范围写端口 &&p,
                                       const 不可变材料布局_B1 &layout,
                                       const std::filesystem::path &dir,
                                       const L1所有者范围写端口 &a)
    : l1_(l), port_(std::move(p)), layout_(layout) {
  try {
    if (!port_.有效() || !port_.绑定于(l) || !a.有效() || !a.绑定于(l) ||
        port_.所有者身份() == a.所有者身份() || !有效(layout_.A承接锚点))
      throw 材料构造失败_B1(材料状态_B1::内部不一致,
                            材料发布状态_B1::确认未发布, {});
    const auto first = port_.读取首次写入材料({{0x424D41544C415931ULL}});
    if (first.状态 != L1所有者范围读取状态::成功 || !first.首次规范化写集 ||
        !first.首次写入结果 ||
        *first.首次规范化写集 != registration_set(layout_.A承接锚点))
      throw 材料构造失败_B1(材料状态_B1::未找到, 材料发布状态_B1::确认未发布,
                            {});
    std::vector<稳定编码> id;
    if (!mapping(*first.首次写入结果, 26, id))
      throw 材料构造失败_B1(材料状态_B1::内部不一致, 材料发布状态_B1::确认发布,
                            {});
    const 不可变材料布局_B1 actual{
        id[0], id[1], id[2], id[3],  id[4],    id[5],    id[6],
        id[7], id[8], id[9], id[10], {id[11]}, {id[12]}, layout_.A承接锚点};
    if (actual != layout_)
      throw 材料构造失败_B1(材料状态_B1::幂等冲突, 材料发布状态_B1::确认发布,
                            {});
    const auto proof = 读取原A特征定义承接_B1(
        {1, layout_.A承接锚点, {16, 0, 0, 0, 0, 0, 0, 0, 0}}, l, a);
    if (proof.状态 != 材料状态_B1::已读取)
      throw 材料构造失败_B1(proof.状态, 材料发布状态_B1::确认发布, proof.用量);
    directory_handle_ = secure_root(dir, directory_);
  } catch (const 材料构造失败_B1 &) {
    throw;
  } catch (...) {
    throw 材料构造失败_B1(材料状态_B1::资源失败, 材料发布状态_B1::确认发布, {});
  }
}
不可变材料数据服务::~不可变材料数据服务() noexcept {
  if (directory_handle_)
    CloseHandle(static_cast<HANDLE>(directory_handle_));
}
bool 不可变材料数据服务::绑定于(const L1事实基座服务 &l) const noexcept {
  return &l1_ == &l && port_.绑定于(l);
}

材料格式读取结果_B1
不可变材料数据服务::读取材料格式(const 材料格式读取请求_B1 &r) const noexcept {
  材料格式读取结果_B1 o;
  try {
    if (r.版本 != 1 || !budget_ok(r.预算))
      return o;
    const auto f = r.种类 == 不可变材料格式种类_B1::有序I64载荷
                       ? layout_.有序I64格式
                   : r.种类 == 不可变材料格式种类_B1::有序U64载荷
                       ? layout_.有序U64格式
                       : 不可变材料格式身份_B1{};
    if (!有效(f)) {
      o.状态 = 材料状态_B1::规则未提供;
      return o;
    }
    L1所有者范围一致当前读取请求 q;
    q.所有者 = {port_.所有者身份()};
    q.节点 = {f.值};
    q.属性值 = {{f.值, layout_.载荷格式代码}};
    const auto x = l1_.尝试读取所有者范围一致当前投影(q);
    if (x.状态 != L1所有者范围一致当前读取状态::成功) {
      o.状态 = map_consistent(x.状态);
      return o;
    }
    if (x.节点.size() != 1 || x.属性值.size() != 1 ||
        x.节点[0].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !x.节点[0].事实 ||
        x.属性值[0].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !x.属性值[0].投影) {
      o.状态 = 材料状态_B1::未找到;
      return o;
    }
    const auto &n = *x.节点[0].事实;
    const auto &v = x.属性值[0].投影->当前值事实;
    const auto expected = std::int64_t(static_cast<std::uint8_t>(r.种类));
    if (n.种类 != 节点种类::普通 || n.属性类型表示 || v.所属节点 != f.值 ||
        v.属性类型节点 != layout_.载荷格式代码 || v.来源节点 != f.值 ||
        !std::holds_alternative<std::int64_t>(v.材料) ||
        std::get<std::int64_t>(v.材料) != expected) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    o.用量 = {1, 0, 1, 0, 0, 0, 1, 0, 0};
    if (!within(o.用量, r.预算)) {
      o.状态 = 材料状态_B1::数量预算不足;
      return o;
    }
    o.格式 = 材料格式事实_B1{
        f,
        r.种类,
        {n.编码, n.种类, n.属性类型表示},
        {v.编码, v.所属节点, v.属性类型节点, v.来源节点, expected}};
    o.状态 = 材料状态_B1::已读取;
    return o;
  } catch (const std::bad_alloc &) {
    o.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    o.状态 = 材料状态_B1::内部不一致;
    return o;
  }
}

不可变材料数据服务::材料完整读取内部结果_B1
不可变材料数据服务::读取材料完整内部(const 材料读取请求_B1 &r) const noexcept {
  材料完整读取内部结果_B1 o;
  auto &pub = o.公开结果;
  try {
    if (r.版本 != 1 || !有效(r.材料) || !budget_ok(r.预算))
      return o;
    L1所有者范围一致当前读取请求 q;
    q.所有者 = {port_.所有者身份()};
    q.节点 = {r.材料.值, layout_.有序I64格式.值, layout_.有序U64格式.值};
    q.属性值 = {{r.材料.值, layout_.字节长度},
                {r.材料.值, layout_.摘要},
                {layout_.有序I64格式.值, layout_.载荷格式代码},
                {layout_.有序U64格式.值, layout_.载荷格式代码}};
    q.目标关系组 = {{r.材料.值, layout_.族成员}};
    q.源关系组 = {{r.材料.值, layout_.材料格式}, {r.材料.值, layout_.来源}};
    const auto x = l1_.尝试读取所有者范围一致当前投影(q);
    if (x.状态 != L1所有者范围一致当前读取状态::成功) {
      pub.状态 = map_consistent(x.状态);
      return o;
    }
    if (x.节点.size() != 3 || x.属性值.size() != 4 ||
        x.目标关系组.size() != 1 || x.源关系组.size() != 2 ||
        x.节点[0].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !x.节点[0].事实) {
      pub.状态 = 材料状态_B1::未找到;
      return o;
    }
    if (x.目标关系组[0].成员.size() != 1 || x.源关系组[0].成员.size() != 1 ||
        x.源关系组[1].成员.size() != 1 ||
        x.属性值[0].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        x.属性值[1].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        x.属性值[2].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        x.属性值[3].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !x.属性值[0].投影 || !x.属性值[1].投影 || !x.属性值[2].投影 ||
        !x.属性值[3].投影 ||
        x.节点[1].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        x.节点[2].状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !x.节点[1].事实 || !x.节点[2].事实) {
      pub.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    const auto &n = *x.节点[0].事实;
    const auto &e1 = x.目标关系组[0].成员[0].关系;
    const auto &e2 = x.源关系组[0].成员[0].关系;
    const auto &e3 = x.源关系组[1].成员[0].关系;
    const auto &len = x.属性值[0].投影->当前值事实;
    const auto &dig = x.属性值[1].投影->当前值事实;
    if (n.种类 != 节点种类::普通 || n.属性类型表示 ||
        e1.源节点 != layout_.族锚点 || e1.目标节点 != r.材料.值 ||
        e2.源节点 != r.材料.值 || e3.源节点 != r.材料.值 ||
        !std::holds_alternative<std::int64_t>(len.材料) ||
        !std::holds_alternative<std::vector<std::uint64_t>>(dig.材料)) {
      pub.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    const auto size = std::get<std::int64_t>(len.材料);
    const auto &dv = std::get<std::vector<std::uint64_t>>(dig.材料);
    if (size <= 0 || dv.size() != 32) {
      pub.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    std::array<std::uint8_t, 32> d{};
    for (std::size_t i = 0; i < 32; ++i) {
      if (dv[i] > 255) {
        pub.状态 = 材料状态_B1::内部不一致;
        return o;
      }
      d[i] = static_cast<std::uint8_t>(dv[i]);
    }
    const 不可变材料格式身份_B1 f{e2.目标节点};
    const auto kind =
        f == layout_.有序I64格式   ? 不可变材料格式种类_B1::有序I64载荷
        : f == layout_.有序U64格式 ? 不可变材料格式种类_B1::有序U64载荷
                                   : static_cast<不可变材料格式种类_B1>(0);
    if (static_cast<std::uint8_t>(kind) == 0) {
      pub.状态 = 材料状态_B1::规则未提供;
      return o;
    }
    const auto formatIndex =
        f == layout_.有序I64格式 ? std::size_t{1} : std::size_t{2};
    const auto attributeIndex =
        f == layout_.有序I64格式 ? std::size_t{2} : std::size_t{3};
    const auto &formatNode = *x.节点[formatIndex].事实;
    const auto &formatValue = x.属性值[attributeIndex].投影->当前值事实;
    const auto expectedKind = std::int64_t(static_cast<std::uint8_t>(kind));
    if (formatNode.编码 != f.值 || formatNode.种类 != 节点种类::普通 ||
        formatNode.属性类型表示 || formatValue.所属节点 != f.值 ||
        formatValue.属性类型节点 != layout_.载荷格式代码 ||
        formatValue.来源节点 != f.值 ||
        !std::holds_alternative<std::int64_t>(formatValue.材料) ||
        std::get<std::int64_t>(formatValue.材料) != expectedKind) {
      pub.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    const 材料格式事实_B1 formatFact{
        f,
        kind,
        {formatNode.编码, formatNode.种类, formatNode.属性类型表示},
        {formatValue.编码, formatValue.所属节点, formatValue.属性类型节点,
         formatValue.来源节点, expectedKind}};
    auto payload = read_file(directory_, material_path(directory_, f, d),
                             r.预算.最大材料字节数);
    if (payload.size() != static_cast<std::size_t>(size) ||
        sha256(payload) != d) {
      pub.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    pub.用量 = {3, 3, 4, 0, 0, 0, 34, payload.size(), 0};
    if (!within(pub.用量, r.预算)) {
      pub.状态 = 材料状态_B1::数量预算不足;
      return o;
    }
    pub.材料 = 材料事实_B1{r.材料,
                           f,
                           e1.编码,
                           e2.编码,
                           e3.编码,
                           e3.目标节点,
                           len.编码,
                           dig.编码,
                           static_cast<std::uint64_t>(size),
                           d,
                           std::move(payload),
                           formatFact};
    o.关系 = {
        {e1.编码, e1.源节点, e1.目标节点, e1.关系类型节点, e1.角色或顺序},
        {e2.编码, e2.源节点, e2.目标节点, e2.关系类型节点, e2.角色或顺序},
        {e3.编码, e3.源节点, e3.目标节点, e3.关系类型节点, e3.角色或顺序}};
    o.属性 = {{len.编码, len.所属节点, len.属性类型节点, len.来源节点,
               std::get<std::int64_t>(len.材料)},
              {dig.编码, dig.所属节点, dig.属性类型节点, dig.来源节点, dv}};
    pub.状态 = 材料状态_B1::已读取;
    return o;
  } catch (const std::length_error &) {
    pub.状态 = 材料状态_B1::数量预算不足;
    return o;
  } catch (const std::bad_alloc &) {
    pub.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    pub.状态 = 材料状态_B1::内部不一致;
    return o;
  }
}
材料读取结果_B1
不可变材料数据服务::读取材料(const 材料读取请求_B1 &r) const noexcept {
  return 读取材料完整内部(r).公开结果;
}

材料发布结果_B1
不可变材料数据服务::发布材料(const 材料发布请求_B1 &r) noexcept {
  材料发布结果_B1 o;
  o.原请求 = r;
  try {
    std::scoped_lock lock(mutex_);
    if (r.版本 != 1 || !有效(r.幂等身份) || !有效(r.格式) || !有效(r.来源) ||
        !budget_ok(r.预算) || r.完整载荷.empty() ||
        r.完整载荷.size() > r.预算.最大材料字节数)
      return o;
    const auto expected = publish_set(r, layout_, sha256(r.完整载荷));
    const auto first = port_.读取首次写入材料({r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::未找到) {
      o.状态 = 材料状态_B1::规则未提供;
      o.发布 = 材料发布状态_B1::确认未发布;
      return o;
    }
    if (first.状态 != L1所有者范围读取状态::成功) {
      o.状态 = map_read(first.状态);
      return o;
    }
    if (!first.首次规范化写集 || !first.首次写入结果 ||
        *first.首次规范化写集 != expected) {
      o.状态 = 材料状态_B1::幂等冲突;
      return o;
    }
    std::vector<稳定编码> id;
    if (!mapping(*first.首次写入结果, 6, id)) {
      o.状态 = 材料状态_B1::可能已发布;
      o.发布 = 材料发布状态_B1::可能发布;
      return o;
    }
    const 材料读取请求_B1 qr{1, {id[0]}, r.预算};
    const auto current = 读取材料(qr);
    o.用量 = current.用量;
    if (read_ok(current, qr)) {
      if (current.材料->格式 != r.格式 || current.材料->来源 != r.来源 ||
          current.材料->完整载荷 != r.完整载荷) {
        o.状态 = 材料状态_B1::幂等冲突;
        return o;
      }
      o.首次材料 = o.当前材料 = current.材料;
      o.当前终态 = 当前终态_B1::原后态仍成立;
    } else if (current.状态 == 材料状态_B1::未找到)
      o.当前终态 = 当前终态_B1::已删除;
    else {
      o.状态 = current.状态;
      o.发布 = 材料发布状态_B1::可能发布;
      return o;
    }
    o.状态 = 材料状态_B1::精确重复;
    o.发布 = 材料发布状态_B1::确认发布;
    o.本次来源已记录 = true;
    return o;
  } catch (const std::bad_alloc &) {
    o.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    o.状态 = 材料状态_B1::内部不一致;
    return o;
  }
}

材料退出结果_B1
不可变材料数据服务::退出材料(const 材料退出请求_B1 &r) noexcept {
  材料退出结果_B1 o;
  o.原请求 = r;
  try {
    std::scoped_lock lock(mutex_);
    if (r.版本 != 1 || !有效(r.幂等身份) || !有效(r.材料) || !budget_ok(r.预算))
      return o;
    const auto first = port_.读取首次写入材料({r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::未找到) {
      o.状态 = 材料状态_B1::规则未提供;
      o.发布 = 材料发布状态_B1::确认未发布;
      return o;
    }
    if (first.状态 != L1所有者范围读取状态::成功) {
      o.状态 = map_read(first.状态);
      return o;
    }
    if (!first.首次规范化写集 || !first.首次写入结果) {
      o.状态 = 材料状态_B1::可能已发布;
      o.发布 = 材料发布状态_B1::可能发布;
      return o;
    }
    const auto &w = *first.首次规范化写集;
    const auto &s = *first.首次写入结果;
    if (w.写入幂等身份 != r.幂等身份 || !w.节点.empty() || !w.关系.empty() ||
        !w.值.empty() || !w.属性槽变更.empty() || w.退出事实.size() != 6 ||
        std::find(w.退出事实.begin(), w.退出事实.end(), r.材料.值) ==
            w.退出事实.end()) {
      o.状态 = 材料状态_B1::幂等冲突;
      return o;
    }
    if (s.状态 != L1所有者范围写入状态::成功 &&
        s.状态 != L1所有者范围写入状态::精确重复) {
      o.状态 = map_write(s.状态);
      return o;
    }
    const auto current = 读取材料({1, r.材料, r.预算});
    o.用量 = current.用量;
    if (current.状态 != 材料状态_B1::未找到 || current.材料) {
      o.状态 = 材料状态_B1::内部不一致;
      return o;
    }
    o.当前终态 = 当前终态_B1::已删除;
    o.发布 = 材料发布状态_B1::确认发布;
    o.状态 = 材料状态_B1::精确重复;
    return o;
  } catch (const std::bad_alloc &) {
    o.状态 = 材料状态_B1::资源失败;
    return o;
  } catch (...) {
    o.状态 = 材料状态_B1::内部不一致;
    return o;
  }
}
} // namespace 海中鱼巣
