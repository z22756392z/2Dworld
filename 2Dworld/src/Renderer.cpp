#include "Renderer.h"
#include <iostream>


void GLClearError()
{
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << " )" << function <<
            " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr););
}

void Renderer::DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const void* index_data)const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_LINES,ib.getCount(), GL_UNSIGNED_INT,index_data));
}

void Renderer::DrawPoint(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const {
    
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_POINTS, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}


void Renderer::Clear() const{
    GLCall(glClear(GL_COLOR_BUFFER_BIT););
}
