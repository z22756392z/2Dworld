#include "EdgeDetection.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <imgui-master/imgui.h>

namespace test {
	EdgeDetection::EdgeDetection() :
		m_va(),
		m_vb(nullptr),
		m_ib(nullptr),
		m_shader(nullptr),
		threshold(1),
		mode(0)
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

		m_shader = new Shader("res/shaders/EdgeDetection.shader");
		m_shader->Bind();

		m_texture = new Texture("res/textures/squirrel.jpg");
		m_texture->Bind();
		m_shader->SetUniform1i("u_Texture", 0);

		m_va->Unbind();
		m_vb->Unbind();
		m_ib->Unbind();
		m_texture->Unbind();
		m_shader->Unbind();
	}

	EdgeDetection::~EdgeDetection() {

	}

	void EdgeDetection::onUpdate() {
		m_shader->Bind();
		m_shader->SetUniform1f("threshold", threshold);
		m_shader->SetUniform1i("mode", mode);
	}

	void EdgeDetection::onRender() {
		//GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;
		m_texture->Bind();
		renderer.Draw(*m_va, *m_ib, *m_shader);
	}

	void EdgeDetection::OnImguiRender() {
		ImGui::SliderFloat("Scale Magnitude", &threshold,0,10,"%f");

		std::string mode_name = "Mode";
		if (mode == 0) {
			mode_name += " origin image";
		}
		else if (mode == 1) {
			mode_name += " dx";
		}
		else if (mode == 2) {
			mode_name += " dy";
		}
		else if (mode == 3) {
			mode_name += " gardient magnitude";
		}
		ImGui::SliderInt(mode_name.c_str(), &mode, 0, 3, "%d");
	}
}
