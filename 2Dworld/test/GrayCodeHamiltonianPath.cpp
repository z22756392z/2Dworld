#include "GrayCodeHamiltonianPath.h"
using namespace std;
namespace test {
	GrayCodeHamiltonianPath::GrayCodeHamiltonianPath():Q(4) ,v(vector<string>(1025,"")),ans(""){

	}

	GrayCodeHamiltonianPath::~GrayCodeHamiltonianPath() {

	}

	void GrayCodeHamiltonianPath::onUpdate() {

	}

	void GrayCodeHamiltonianPath::onRender() {

	}

	void GrayCodeHamiltonianPath::OnImguiRender() {
		string content = "Hamiltonian path Q";
		ImGui::SliderInt(content.c_str(), &Q, 0, 10, to_string(Q).c_str());
		if (ImGui::Button("Apply")) {
			Calculate();
			ans = "";
			int counter = 1;
			for (int i = 0; i < Q; i++) {
				counter *= 2;
			}

			for (int i = 0; i < counter; i++) {
				ans += v[i] + "\n";
			}
		}
		ImGui::Text(ans.c_str());
	}

	void GrayCodeHamiltonianPath::Calculate() {
		int a = 1;
		v = vector<string>(1025, "");
		for (int i = 0; i < Q; i++) {
			vector<string> temp = v;
			for (int j = 0; j < a; j++) {
				v[j] = "0" + temp[j];
			}
			for (int j = a; j < 2 * a; j++) {
				v[j] = "1" + temp[j-a];
			}
			a *= 2;
		}
	}
}