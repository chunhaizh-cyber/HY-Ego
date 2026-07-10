// 文件规则：显示服务只组装人读只读展示材料；不得写业务事实或接入控制面板、WebView、外设显示通道。
#pragma once

#include "../核心/句柄.h"

#include <cstdint>

namespace 海中鱼巣 {

enum class 显示请求状态 : std::uint32_t {
    可展示 = 0,
    拒绝越权读取 = 1,
    拒绝写业务事实 = 2,
    拒绝文本裁决 = 3,
    后续专项门禁 = 4,
    拒绝材料不完整 = 5
};

struct 显示请求 {
    bool 来自服务只读入口 = false;
    bool 试图写业务事实 = false;
    bool 试图用文本裁决事实 = false;
    bool 请求外部显示通道 = false;
};

struct 显示材料 {
    显示请求状态 状态 = 显示请求状态::拒绝越权读取;
    bool 展示材料人读 = true;
    bool 允许写业务事实 = false;
};

struct 需求结算显示请求 {
    显示请求 边界;
    节点句柄 需求;
    节点句柄 任务;
    节点句柄 结算记录;
    节点句柄 动态证据;
};

struct 需求结算只读显示材料 {
    显示请求状态 状态 = 显示请求状态::拒绝越权读取;
    节点句柄 需求;
    节点句柄 任务;
    节点句柄 结算记录;
    节点句柄 动态证据;
    bool 展示材料人读 = true;
    bool 只读材料 = true;
    bool 允许写业务事实 = false;
};

class 显示服务 {
public:
    显示材料 生成只读展示材料(const 显示请求& 请求) const {
        if (!请求.来自服务只读入口) {
            return {显示请求状态::拒绝越权读取, true, false};
        }
        if (请求.试图写业务事实) {
            return {显示请求状态::拒绝写业务事实, true, false};
        }
        if (请求.试图用文本裁决事实) {
            return {显示请求状态::拒绝文本裁决, true, false};
        }
        if (请求.请求外部显示通道) {
            return {显示请求状态::后续专项门禁, true, false};
        }
        return {显示请求状态::可展示, true, false};
    }

    需求结算只读显示材料 生成需求结算只读展示材料(const 需求结算显示请求& 请求) const {
        const auto 边界材料 = 生成只读展示材料(请求.边界);
        if (边界材料.状态 != 显示请求状态::可展示) {
            return {边界材料.状态, {}, {}, {}, {}, true, true, false};
        }
        if (!句柄有效(请求.需求)
            || !句柄有效(请求.任务)
            || !句柄有效(请求.结算记录)
            || !句柄有效(请求.动态证据)) {
            return {显示请求状态::拒绝材料不完整, {}, {}, {}, {}, true, true, false};
        }
        return {
            显示请求状态::可展示,
            请求.需求,
            请求.任务,
            请求.结算记录,
            请求.动态证据,
            true,
            true,
            false
        };
    }
};

}
