#ifndef _SVMBUFFER_HPP_
#define _SVMBUFFER_HPP_

#include "svmCore.hpp"

class Buffer
{
public:
	Buffer() {}
    ~Buffer();
	void Init(GLuint bufferType, GLuint usage, const void * data, GLuint stride, GLuint count);
    GLuint GetStride() const { return m_stride; }
    GLuint GetCount() const { return m_count; }

	GLuint Get() const { return m_buffer; }
private:
	void Bind() const;
    GLuint m_buffer { 0 };
    GLuint m_bufferType { 0 };
    GLuint m_usage { 0 };
    GLuint m_stride { 0 };
    GLuint m_count { 0 };
};

#endif //_SVMBUFFER_HPP_
