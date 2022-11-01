#ifndef _SVMVERTEXLAYOUT_HPP_
#define _SVMVERTEXLAYOUT_HPP_

#include "svmCore.hpp"

class VertexLayout
{
public:
	VertexLayout() {}
	~VertexLayout();

    void Init();
	void SetAttrib(GLuint attribIndex, GLint count, GLuint type, GLboolean normalized, GLint stride, GLuint64 offset);

	GLuint Get() const { return m_vertexArrayObject; }
	void DisableAttrib(int attribIndex) const;

    void Bind() const;
private:
	GLuint m_vertexArrayObject { 0 };
};

#endif //_SVMVERTEXLAYOUT_HPP_
