#include "TestTexture.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"


namespace test {
	TestTexture::TestTexture(): 
		m_va(), 
		m_vb(nullptr),
		m_ib(nullptr),
		m_shader(nullptr)
	{
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

		m_vb = new VertexBuffer(position, sizeof(float) * 4 * 4);
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_va = new VertexArray();
		m_va->AddBuffer(*m_vb, layout);
		m_ib = new IndexBuffer(indices, 6);

		m_shader = new Shader("res/shaders/Basic.shader");
		m_shader->Bind();
	
		m_texture = new Texture("res/textures/log_icon.png");
		m_texture->Bind();
		m_shader->SetUniform1i("u_Texture", 0);

		m_va->Unbind();
		m_vb->Unbind();
		m_ib->Unbind();
		m_texture->Unbind();
		m_shader->Unbind();
	}

	TestTexture::~TestTexture() {

	}

	void TestTexture::onRender() {
		//GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;
		m_texture->Bind();
		renderer.Draw(*m_va, *m_ib, *m_shader);
	}

	void TestTexture::onUpdate() {

	}

	void TestTexture::OnImguiRender() {

	}
}