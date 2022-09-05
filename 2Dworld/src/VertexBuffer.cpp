#include"VertexBuffer.h"

#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glCreateBuffers(1, &m_RendererID););
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID););
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW););
}

VertexBuffer::VertexBuffer() {
    GLCall(glCreateBuffers(1, &m_RendererID););
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID););
    
}

void VertexBuffer::Allocate(unsigned int size) const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID););
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID););
}

void VertexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID););
}

void VertexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0););
}