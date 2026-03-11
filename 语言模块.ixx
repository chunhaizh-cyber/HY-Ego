export module 语言模块;

import 主信息定义模块;
import 语素模块;

export class 语言类 {
public:
    class 词到世界树本能转换器 {
    public:
        static 基础信息节点类* 转换(词节点类* 词节点, 场景节点类* 当前场景 = nullptr)
        {
            auto* 词信息 = dynamic_cast<词主信息类*>(词节点->主信息);
            auto* 词性信息 = 获取主词性(词节点);

            if (!词信息 || !词性信息) return nullptr;

            const std::wstring& 词 = 词信息->词;
            枚举_词性 词性 = 词性信息->词性;

            if (是名词(词性) || 词性 == 枚举_词性::n || 词性 == 枚举_词性::nr || 词性 == 枚举_词性::ns) {
                return 创建或查找存在(词, 当前场景);
            }

            if (是动词(词性)) {
                return 创建动态模板(词);
            }

            if (是形容词(词性) || 是副词(词性)) {
                return 创建特征模板(词);
            }

            if (词性 == 枚举_词性::r) {
                return 处理代词(词, 当前场景);
            }

            if (词性 == 枚举_词性::m || 词性 == 枚举_词性::q) {
                return 创建数量特征(词);
            }

            if (是介词(词性) || 是连词(词性)) {
                return 创建关系模板(词);
            }

            return 创建存在(词 + L"(未知)", 当前场景);
        }

    private:
        static 存在节点类* 创建或查找存在(const std::wstring& 名, 场景节点类* 场景)
        {
            auto 结果 = 世界树.查找节点_全链();
            if (结果) return 结果;

            auto* 信息 = new 存在节点主信息类();
            信息->名称 = new 词主信息类(名);
            auto* 节点 = 世界树.添加节点(信息);
            if (场景) 世界树.添加子节点(场景, 节点);
            return 节点;
        }

        static 指代节点主信息类* 处理代词(const std::wstring& 代词, 场景节点类* 场景)
        {
            if (代词 == L"我" || 代词 == L"俺" || 代词 == L"咱") {
                return 获取自我指代();
            }
            auto* 指代 = new 指代节点主信息类();
            指代->代词 = new 词主信息类(代词);
            return 指代;
        }
    };
};
