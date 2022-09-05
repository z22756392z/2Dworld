#pragma once
#include<GL/glew.h>

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x/*# turn x into string*/,__FILE__,__LINE__));


void GLClearError();


bool GLLogCall(const char* function, const char* file, int line);


class Renderer {
private:

public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const;
    void DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,const void* index_data)const;
    void DrawPoint(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const;
    void Clear() const;
};


