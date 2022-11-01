#ifndef _SVMSHADER_HPP_
#define _SVMSHADER_HPP_

#include "svmCore.hpp"

class Shader
{
public:
	Shader() = default;
	~Shader();
	GLboolean LoadFile(const std::string & filename, GLenum shaderType);

	unsigned int Get() const { return m_shader; }
private:
    unsigned int m_shader { 0 };
	std::optional<std::string> LoadTextFile(const std::string & filename);
};

#endif //_SVMSHADER_HPP_
