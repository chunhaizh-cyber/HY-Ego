
module;
#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <iostream>


export module 全局函数模块;
import <string>;
import <vector>;
import <mutex>;
import <cassert>;
import <type_traits>;
import <stdexcept>;


namespace 全局函数 {
    int 统计位数(std::uint64_t x)
    {
#if defined(__cpp_lib_bitops)
        return static_cast<int>(std::popcount(x));
#else
        int c = 0;
        while (x)
        {
            x &= (x - 1);
            ++c;
        }
        return c;
#endif
    }
	export std::string wstring_to_utf8(const std::wstring& wstr) {
		int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), nullptr, 0, nullptr, nullptr);
		std::string utf8(ulen, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), utf8.data(), ulen, nullptr, nullptr);
		return utf8;
	}
	export std::wstring utf8_to_wstring(const std::string& utf8) {
		int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.size(), nullptr, 0);
		std::wstring wstr(wlen, 0);
		MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.size(), wstr.data(), wlen);
		return wstr;
	}

    // 从你的 vector<int64_t> 原始数据构造 点阵图
    export 点阵图 构造点阵图(std::int32_t 宽, std::int32_t 高, const std::vector<std::int64_t>& 原始数据)
    {
        点阵图 图;
        图.宽 = 宽;
        图.高 = 高;

        if (宽 <= 0 || 高 <= 0)
        {
            // 空图
            图.数据.clear();
            return 图;
        }

        const std::size_t N = static_cast<std::size_t>(宽) * static_cast<std::size_t>(高);
        const std::size_t 期望块数 = (N + 63u) / 64u;

        图.数据.assign(期望块数, 0);

        const std::size_t 实际块数 = 原始数据.size();
        const std::size_t 拷贝块数 = 实际块数 < 期望块数 ? 实际块数 : 期望块数;

        for (std::size_t i = 0; i < 拷贝块数; ++i)
        {
            图.数据[i] = static_cast<std::uint64_t>(原始数据[i]);
        }

        return 图;
    }

    // 汉明相似度：1 = 完全相同，0 = 完全相反
    export double 计算_轮廓_汉明相似度(const 点阵图& A, const 点阵图& B)
    {
        if (A.宽 != B.宽 || A.高 != B.高)
        {
            throw std::invalid_argument("计算_轮廓_汉明相似度: 分辨率不一致");
        }
        if (A.数据.size() != B.数据.size())
        {
            throw std::invalid_argument("计算_轮廓_汉明相似度: 数据块数量不一致");
        }

        const std::size_t N = static_cast<std::size_t>(A.宽) * static_cast<std::size_t>(A.高);
        if (N == 0)
        {
            return 1.0; // 约定：空图视为完全相同
        }

        int diff = 0;
        for (std::size_t i = 0; i < A.数据.size(); ++i)
        {
            diff += 统计位数(A.数据[i] ^ B.数据[i]);
        }

        const double ratio = static_cast<double>(diff) / static_cast<double>(N);
        return 1.0 - ratio;
    }

    // IoU: 交并比 = |A∩B| / |A∪B|
    export double 计算_轮廓_IoU(const 点阵图& A, const 点阵图& B)
    {
        if (A.宽 != B.宽 || A.高 != B.高)
        {
            throw std::invalid_argument("计算_轮廓_IoU: 分辨率不一致");
        }
        if (A.数据.size() != B.数据.size())
        {
            throw std::invalid_argument("计算_轮廓_IoU: 数据块数量不一致");
        }

        int 交数 = 0;
        int 并数 = 0;

        for (std::size_t i = 0; i < A.数据.size(); ++i)
        {
            const std::uint64_t 交 = A.数据[i] & B.数据[i];
            const std::uint64_t 并 = A.数据[i] | B.数据[i];

            交数 += 统计位数(交);
            并数 += 统计位数(并);
        }

        if (并数 == 0)
        {
            // 两个都是全空图，视为完全相同
            return 1.0;
        }

        return static_cast<double>(交数) / static_cast<double>(并数);
    }
}
