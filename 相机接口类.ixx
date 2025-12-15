export module 相机接口模块;

import <vector>;
import <cstdint>;
import <chrono>;
import 基础数据类型模块;


// ===== 抽象深度相机接口 =====
export class 抽象深度相机接口 {
public:
    virtual ~抽象深度相机接口() = default;

    virtual bool 打开() = 0;
    virtual void 关闭() = 0;

    virtual bool 采集一帧(结构体_原始场景帧& 输出) = 0;
};
