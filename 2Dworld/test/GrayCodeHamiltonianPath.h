#pragma once
#include "Test.h"
#include<string>
#include "imgui-master/imgui.h"
using namespace std;
namespace test {
	class GrayCodeHamiltonianPath: public Test
	{
	public:
		GrayCodeHamiltonianPath();
		~GrayCodeHamiltonianPath();
		void onUpdate();
		void onRender();
		void OnImguiRender();
		void Calculate();

		int Q;
		string ans;
		vector<string> v;
	};
}


