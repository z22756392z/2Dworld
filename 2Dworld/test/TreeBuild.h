#pragma once
#include"Test.h"
#include"Renderer.h"
#include"Texture.h"
#include"VertexBufferLayout.h"
namespace test {
	
	class TreeBuild :public Test{
	public:
		TreeBuild();
		~TreeBuild();
		void onUpdate();
		void onRender();
		void OnImguiRender();
	
	private:
		void Free();
		VertexArray* vao;
		VertexBuffer* vbo;
		IndexBuffer* ibo;
		Shader* shader;
		Texture* texture;
	};

}
