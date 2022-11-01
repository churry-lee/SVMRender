#include "svmVertexLayout.hpp"

void VertexLayout::Bind() const { glBindVertexArray(m_vertexArrayObject); }

void VertexLayout::SetAttrib(GLuint attribIndex, GLint count, GLuint type, GLboolean normalized, GLint stride, GLuint64 offset)
{
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, count, type, normalized, stride, (const void *)offset);
}

void VertexLayout::Init()
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    Bind();
}

VertexLayout::~VertexLayout() { if (m_vertexArrayObject) glDeleteVertexArrays(1, &m_vertexArrayObject); }
