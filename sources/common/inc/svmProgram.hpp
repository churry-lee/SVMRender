#ifndef _SVMPROGRAM_HPP_
#define _SVMPROGRAM_HPP_

#include "svmShader.hpp"

class Program
{
public:
	Program(const std::string & vertShaderFileName, const std::string & fragShaderFileName, const std::string & geomShaderFileName = "\0");
	~Program();

    GLboolean Init();

	GLuint Get() const { return m_program; }
	void Use() const;

    void SetUniform(const std::string& name, GLint value) const;
    void SetUniform(const std::string& name, GLfloat value) const;
	void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

private:
	GLboolean Link(const std::vector<Shader * > & shaders);
    GLuint m_program { 0 };

    Shader * m_vertShader;
    Shader * m_fragShader;
	Shader * m_geomShader;
	std::string m_vertFileName;
	std::string m_fragFileName;
	std::string m_geomFileName;

};

#endif //_SVMPROGRAM_HPP_
