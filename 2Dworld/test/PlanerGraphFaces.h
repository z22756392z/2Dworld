#pragma once
#include<Test.h>

namespace test {
	class PlanerGraphFaces:public Test
	{
	public:
		PlanerGraphFaces();
		~PlanerGraphFaces();
		void onUpdate();
		void onRender();
		void OnImguiRender();
		void Calculate();

		int info[2];
		int faces;
	};

}

