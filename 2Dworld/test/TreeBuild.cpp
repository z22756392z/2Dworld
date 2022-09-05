#include"TreeBuild.h"

namespace test {

	TreeBuild::TreeBuild():vao(nullptr),vbo(nullptr),ibo(nullptr) {
		
		float position[] = {
			0.0f,0.0f,	0.0f,0.0f,
			1.0f,0.0f,	1.0f,0.0f,
			1.0f,1.0f,	1.0f,1.0f,
			0.0f,1.0f,	0.0f,1.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		vbo = new VertexBuffer(position, sizeof(float) * 4 * 4);
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vao = new VertexArray();
		vao->AddBuffer(*vbo, layout);
		ibo = new IndexBuffer(indices, 6);

		shader = new Shader("res/shaders/Basic.shader");
		shader->Bind();

		texture = new Texture("res/textures/circle.png");
		texture->Bind();
		shader->SetUniform1i("u_Texture", 0);

		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
		texture->Unbind();
		shader->Unbind();
	}

	TreeBuild::~TreeBuild() {
		Free();
	}

	void TreeBuild::onUpdate() {

	}

	void TreeBuild::onRender() {
		vao->Bind();
		vbo->Bind();
		shader->Bind();
		texture->Bind();
		Renderer renderer;
		renderer.Draw(*vao, *ibo, *shader);
	}

	void TreeBuild::OnImguiRender() {
		
	}

	void TreeBuild::Free() {
		delete vao;
		delete ibo;
		delete vbo;
		vao = nullptr;
		ibo = nullptr;
		vbo = nullptr;
	}
}