#include "Test.h"
#include "imgui-master/imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& currentTestPointer)
		: m_CurrnetTest(currentTestPointer)
	{
		
	}

	void TestMenu::OnImguiRender() {
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str())) {
				m_CurrnetTest = test.second();
			}
		}
	}

}
