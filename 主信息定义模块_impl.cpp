
module;

#include <ostream>
#include <istream>
module 主信息定义模块;
/*
// ======================= 主信息序列化辅助函数实现 =======================
namespace 主信息定义模块_存档_detail
{
    template<class T>
    inline void 写POD(std::ostream& os, const T& v)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        os.write(reinterpret_cast<const char*>(&v), sizeof(T));
    }

    template<class T>
    inline bool 读POD(std::istream& is, T& v)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        is.read(reinterpret_cast<char*>(&v), sizeof(T));
        return (bool)is;
    }

    inline void 写字符串(std::ostream& os, const std::string& s)
    {
        const std::uint32_t n = (std::uint32_t)s.size();
        写POD(os, n);
        if (n) os.write(s.data(), n);
    }

    inline bool 读字符串(std::istream& is, std::string& out, std::uint32_t 上限)
    {
        std::uint32_t n = 0;
        if (!读POD(is, n)) return false;
        if (n > 上限) return false;
        out.assign(n, '\0');
        if (n) is.read(out.data(), n);
        return (bool)is;
    }

    template<class NodePtr>
    inline std::string 节点主键(NodePtr p)
    {
        if (!p) return {};
        return p->获取主键();
    }

    template<class Enum>
    inline void 写枚举(std::ostream& os, Enum e)
    {
        using U = std::underlying_type_t<Enum>;
        写POD(os, (U)e);
    }

    template<class Enum>
    inline bool 读枚举(std::istream& is, Enum& e)
    {
        using U = std::underlying_type_t<Enum>;
        U u{};
        if (!读POD(is, u)) return false;
        e = (Enum)u;
        return true;
    }

 
   */