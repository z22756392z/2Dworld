#pragma once
#include "Test.h"

namespace test {
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;

	private:
		float m_Color[4];
	};
}
