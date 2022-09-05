#include "PlanerGraphFaces.h"
#include "imgui-master/imgui.h"
#include<string>
using namespace std;
namespace test {
	PlanerGraphFaces::PlanerGraphFaces(){
		info[0] = 25;
		info[1] = 75;
		faces = 0;
	}
	PlanerGraphFaces::~PlanerGraphFaces(){}
	void PlanerGraphFaces::onUpdate() {

	}

	void PlanerGraphFaces::onRender() {

	}

	void PlanerGraphFaces::OnImguiRender() {
		ImGui::Text("It is a planer graph.");

		ImGui::SliderInt("###vertices", &info[0], 1, 100, "It have %d vertices");
		ImGui::SliderInt("###edges", &info[1], 1, 100, "It have %d number of edges");
		ImGui::Text("And all degree of vertex are same");
		if (ImGui::Button("Apply")) {
			faces = 0;
			Calculate();
		}

		if (faces != 0 && faces > 0) {
			string content = "It have " + to_string(faces) + " faces";
			ImGui::Text(content.c_str());
		}
		else if (faces != 0 && faces < 0 ) {
			ImGui::Text("Wrong input");
		}
	}

	void PlanerGraphFaces::Calculate() {
		faces = 2 - info[0] + info[1];
	}
}
