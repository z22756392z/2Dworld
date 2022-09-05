#include "TestClearColor.h"

#include "Renderer.h"
#include "GL/glew.h"

#include "imgui-master/imgui.h"

namespace test {
	TestClearColor::TestClearColor()
		: m_Color{ 0.5f, 0.5f, 0.5f, 1.0f }
	{

	}

	TestClearColor::~TestClearColor() {

	}

	void TestClearColor::onUpdate(){

	}

	void TestClearColor::onRender() {
		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImguiRender() {
		ImGui::ColorEdit4("Clear Color", m_Color);
	}
}
