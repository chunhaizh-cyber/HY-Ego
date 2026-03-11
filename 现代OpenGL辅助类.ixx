module;

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>

export module 现代OpenGL辅助模块;

static inline std::string 读取文本文件(const std::string& path)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f.is_open()) return {};

    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static inline GLuint 编译Shader(GLenum type, const char* src, std::string* errOut)
{
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);

    GLint ok = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (ok == GL_TRUE) return sh;

    GLint len = 0;
    glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
    if (errOut) {
        std::string log;
        if (len > 0) {
            log.resize((size_t)len);
            glGetShaderInfoLog(sh, len, nullptr, log.data());
        }
        *errOut = std::move(log);
    }

    glDeleteShader(sh);
    return 0;
}

static inline GLuint 链接Program(GLuint vs, GLuint fs, std::string* errOut)
{
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    GLint ok = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (ok == GL_TRUE) return prog;

    GLint len = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (errOut) {
        std::string log;
        if (len > 0) {
            log.resize((size_t)len);
            glGetProgramInfoLog(prog, len, nullptr, log.data());
        }
        *errOut = std::move(log);
    }

    glDeleteProgram(prog);
    return 0;
}

export GLuint 创建ShaderProgram_源码(const char* vertexSrc, const char* fragmentSrc, std::string* errOut = nullptr)
{
    if (!vertexSrc || !fragmentSrc) return 0;

    std::string log;
    GLuint vs = 编译Shader(GL_VERTEX_SHADER, vertexSrc, &log);
    if (!vs) { if (errOut) *errOut = std::move(log); return 0; }

    GLuint fs = 编译Shader(GL_FRAGMENT_SHADER, fragmentSrc, &log);
    if (!fs) {
        glDeleteShader(vs);
        if (errOut) *errOut = std::move(log);
        return 0;
    }

    GLuint prog = 链接Program(vs, fs, &log);
    glDeleteShader(vs);
    glDeleteShader(fs);

    if (!prog) { if (errOut) *errOut = std::move(log); return 0; }
    return prog;
}

// 兼容旧接口：从文件读取
export GLuint 创建ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vs = 读取文本文件(vertexPath);
    std::string fs = 读取文本文件(fragmentPath);
    if (vs.empty() || fs.empty()) return 0;
    return 创建ShaderProgram_源码(vs.c_str(), fs.c_str(), nullptr);
}
