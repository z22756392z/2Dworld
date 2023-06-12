#pragma once
#include "Test.h"
namespace test {
	class Spline : public Test
	{
	public:
		Spline();
		~Spline();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;
	};
}
