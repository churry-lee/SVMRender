#include "svmBuffer.hpp"

void Buffer::Bind() const { glBindBuffer(m_bufferType, m_buffer); }

void Buffer::Init(GLuint bufferType, GLuint usage, const void *data, GLuint stride, GLuint count)
{
    m_bufferType = bufferType;
    m_usage = usage;
    m_stride = stride;              // size of vertex
    m_count = count;                // number of vertices

    glGenBuffers(1, &m_buffer);
    Bind();

    glBufferData(m_bufferType, m_stride*m_count, data, m_usage);
}

Buffer::~Buffer() { if (m_buffer) glDeleteBuffers(1, &m_buffer); }
