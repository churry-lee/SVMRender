#include "svmShader.hpp"


GLboolean Shader::LoadFile(const std::string & filename, GLenum shaderType)
{
    auto result = LoadTextFile(filename);
    if (!result.has_value())
        return false;

    auto & code = result.value();
    const char * codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar * const *)&codePtr, &codeLength);
    glCompileShader(m_shader);

    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        return false;
    }
    return true;
}

std::optional<std::string> Shader::LoadTextFile(const std::string & filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		return {};
	}
	std::stringstream text;
	text << fin.rdbuf();
	return text.str();
}

Shader::~Shader() { if (m_shader) glDeleteShader(m_shader); }
