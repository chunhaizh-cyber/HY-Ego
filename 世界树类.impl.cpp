module 世界树模块;

// 在某个场景下新建一个“存在实例”
存在节点类* 世界树类::新建存在(场景节点类* 所在场景, 存在节点主信息类* 主信息) {
#ifdef DEBUG
    if (!根场景) {
        //   抛出错误
    }
#endif // DEBUG

   
    if (!所在场景) {
        所在场景 = 现实世界;
    }
    if (!主信息) {
        主信息 = new 存在节点主信息类();
    }

    基础信息基类* 基类指针 = 主信息;
    auto* 节点 = static_cast<存在节点类*>(
        宇宙链.添加子节点(所在场景, 基类指针)
        );
    return 节点;
}

// 给某个存在实例挂一个特征（颜色、形状、位置等）
特征节点类* 世界树类::为存在添加特征(存在节点类* 存在, 特征节点主信息类* 特征主信息) {
    if (!存在 || !特征主信息) return nullptr;
     
	auto* 特征节点=查找特征节点_类型加值(存在, 特征主信息->类型, 特征主信息->值);
    if (特征节点 == nullptr) {
        auto* 
        特征节点 = 宇宙链.添加子节点(存在, 特征主信息);
    }
    return 特征节点;
}


// 1. 查找某个存在下，指定类型 + 指定值的特征节点（精确匹配）
特征节点类* 世界树类::查找特征节点_类型加值(存在节点类* 存在,词性节点类* 特征类型,特征值节点类* 目标特征值) const
{
    if (!存在 || !特征类型 || !目标特征值) return nullptr;

    基础信息节点类* child = 存在->子;
    if (!child) return nullptr;

    基础信息节点类* cur = child;
    do {
        if (auto* feat = dynamic_cast<特征节点类*>(cur)) {
            auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
            if (info && info->类型 == 特征类型) {
                // 遍历该特征节点下的所有值环
                if (info->值) {
                    特征值节点类* val = info->值;
                    do {
                        if (val == 目标特征值) {  // 指针相等：全局特征值集保证相同值同一节点
                            return feat;
                        }
                        val = val->下;
                    } while (val != info->值);
                }
            }
        }
        cur = cur->下;
    } while (cur != child);

    return nullptr;
}

// 2. 检查某个存在是否已经拥有某个具体特征值（常用于查重）
bool 世界树类::是否存在该特征值(
    存在节点类* 存在,
    词性节点类* 特征类型,
    特征值节点类* 目标特征值) const
{
    return 查找特征节点_类型加值(存在, 特征类型, 目标特征值) != nullptr;
}

// 3. 只按类型查找（返回第一个匹配的特征节点）—— 仍保留，用于读取“当前最新”或“唯一”特征的情况
// 在 世界树类.ixx 或其实现文件中添加

特征节点类* 世界树类::查找特征节点(存在节点类* 存在, 词性节点类* 特征类型) const
{
    if (!存在 || !特征类型) {
        return nullptr;
    }

    // 存在节点的子节点链（同层环）
    基础信息节点类* child = 存在->子;
    if (!child) {
        return nullptr;  // 没有子节点
    }

    // 遍历同层子节点环（双向链表）
    基础信息节点类* cur = child;
    do {
        // 动态转换为特征节点
        if (auto* feat = dynamic_cast<特征节点类*>(cur)) {
            // 获取特征主信息
            if (auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息)) {
                // 判断类型是否匹配（通常 info->类型 指向词性节点）
                if (info->类型 == 特征类型) {
                    return feat;
                }
            }
        }

        cur = cur->下;  // 下一个同层兄弟节点
    } while (cur != child);  // 回到起点即遍历完成

    return nullptr;  // 未找到
}

std::vector<特征节点类*> 世界树类::查找所有特征节点(存在节点类* 存在, 词性节点类* 特征类型) const
{
    return std::vector<特征节点类*>();
}
