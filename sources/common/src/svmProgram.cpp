#include "svmProgram.hpp"

Program::Program(const std::string & vertShaderFileName, const std::string & fragShaderFileName, const std::string & geomShaderFileName)
{
	m_vertShader = new Shader();
	m_fragShader = new Shader();
	m_geomShader = new Shader();

	m_vertFileName = vertShaderFileName;
	m_fragFileName = fragShaderFileName;
	m_geomFileName = geomShaderFileName;
}


GLboolean Program::Init()
{
    m_vertShader->LoadFile(m_vertFileName, GL_VERTEX_SHADER);
    m_fragShader->LoadFile(m_fragFileName, GL_FRAGMENT_SHADER);

    if (!m_vertShader || !m_fragShader)
        return false;

    if (!Link({m_vertShader, m_fragShader}))
        return false;

	if (std::strcmp(m_geomFileName.c_str(), "\0") != 0)
	{
		m_geomShader->LoadFile(m_geomFileName, GL_GEOMETRY_SHADER);
		if (!m_geomShader)
			return false;
		if(!Link({m_geomShader}))
			return false;
	}

    return true;
}

GLboolean Program::Link(const std::vector<Shader * > & shaders)
{
    m_program = glCreateProgram();

    for (auto & shader: shaders)
        glAttachShader(m_program, shader->Get());
    glLinkProgram(m_program);

    GLint success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
//        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }

    return true;
}

void Program::Use() const { glUseProgram(m_program); }

void Program::SetUniform(const std::string& name, GLint value) const
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
}

void Program::SetUniform(const std::string& name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
}

void Program::SetUniform(const std::string& name, const glm::vec2 & value) const
{
	glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::vec3 & value) const
{
    glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

Program::~Program()
{
//	if (m_program) glDeleteProgram(m_program);
//
//	delete m_vertShader;
//	delete m_fragShader;
}