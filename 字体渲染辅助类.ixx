module;

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

export module 字体渲染辅助模块;

// 说明：本模块当前提供“空实现”的字体渲染器，用于让显示链路先跑通。
// 你后续若要上 FreeType，把实现替换即可（保持接口不变）。

export struct 字形信息 {
    GLuint 纹理ID = 0;
    glm::ivec2 尺寸{ 0,0 };
    glm::ivec2 轴心{ 0,0 };
    GLuint 进位 = 0;
};

export class 字体渲染器 {
public:
    // face 预留：FreeType 的 FT_Face 本质是指针，这里用 void* 避免依赖。
    explicit 字体渲染器(void* face = nullptr) : _face(face) {}

    bool 初始化() { return true; }

    // 2D/3D 文本渲染：当前为空实现
    void 渲染文本2D(const std::string& text, float x, float y, float scale, const glm::vec3& color)
    {
        (void)text; (void)x; (void)y; (void)scale; (void)color;
    }

    void 渲染文本3D(const std::string& text, const glm::vec3& pos, float scale, const glm::vec3& color)
    {
        (void)text; (void)pos; (void)scale; (void)color;
    }

private:
    void* _face = nullptr;
};
