#include "Chaos_Equations.h"
#include <math.h>
#include <stdlib.h>
#include <string>
#include "Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui-master/imgui.h"
#include <random>

static const int num_params = 18;
static std::mt19937 rand_gen;

static void RandParams(double* params) {
	std::uniform_int_distribution<int> rand_int(0, 3);
	for (int i = 0; i < num_params; ++i) {
		const int r = rand_int(rand_gen);
		if (r == 0) {
			params[i] = 1.0f;
		}
		else if (r == 1) {
			params[i] = -1.0f;
		}
		else {
			params[i] = 0.0f;
		}
	}
}

static std::string ParamsToString(const double* params) {
	const char base27[] = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static_assert(num_params % 3 == 0, "Params must be a multiple of 3");
	int a = 0;
	int n = 0;
	std::string result;
	for (int i = 0; i < num_params; ++i) {
		a = a * 3 + int(params[i]) + 1;
		n += 1;
		if (n == 3) {
			result += base27[a];
			a = 0;
			n = 0;
		}
	}
	return result;
}

static void StringToParams(const std::string& str, double* params) {
	for (int i = 0; i < num_params / 3; ++i) {
		int a = 0;
		const char c = (i < str.length() ? str[i] : '_');
		if (c >= 'A' && c <= 'Z') {
			a = int(c - 'A') + 1;
		}
		else if (c >= 'a' && c <= 'z') {
			a = int(c - 'a') + 1;
		}
		params[i * 3 + 2] = double(a % 3) - 1.0;
		a /= 3;
		params[i * 3 + 1] = double(a % 3) - 1.0;
		a /= 3;
		params[i * 3 + 0] = double(a % 3) - 1.0;
	}
}
namespace test {
	Chaos_Equations::Chaos_Equations() :
		t(-0.3f),
		x(0.0f),
		y(0.0f),
		scale(1.0f),
		num_point(1000),
		speed(0.00001f),
		vao(nullptr),
		vb(nullptr),
		ib(nullptr)
	{
		srand(time(NULL));
		rand_gen.seed((unsigned int)time(0));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_POINT_SMOOTH));
		GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
		static const float dot_sizes[] = { 1.0f, 3.0f, 10.0f };
		GLCall(glPointSize(dot_sizes[1]));

		vao = new VertexArray();
		vb = new VertexBuffer();
		ib = new IndexBuffer();

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		vao->AddBuffer(*vb, layout);

		proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
		
		shader = new Shader("res/shaders/BigO.shader");
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", proj);

		vb->Unbind();
		ib->Unbind();
		vao->Unbind();
		shader->Unbind();
		StringToParams("CODEPA", params);
		for (size_t i = 0; i < num_point; ++i) {
			color[i] = GenRandColor();
		}
	}

	Chaos_Equations::~Chaos_Equations() {
		delete vao;
		delete vb;
		delete ib;
		vao = nullptr;
		vb = nullptr;
		ib = nullptr;
	}

	void Chaos_Equations::onUpdate() {
		Vertex vertex;
		float alpha = 1.0f;
		for (int i = 0; i < 10; i++) {
			vertex.position = glm::vec2(t - speed * i, t - speed * i);
			alpha -= 0.1f;
			for (int j = 0; j < num_point; j++) {
				vertex = Equation(vertex);
				Point(vertex.position, glm::vec4(color[j].r, color[j].g, color[j].b, alpha));
			}
		}
		
		if (t > 3 || t < -3) {
			speed = -speed;
		}
		t += speed;
		
		vb->Allocate(vertexs.size() * sizeof(Vertex));//allocate memory
		ib->Allocate(indices.size());
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertexs.size() * sizeof(Vertex), vertexs.data()));//upload data
		GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data()));
		vb->Unbind();
		ib->Unbind();
		vertexs.clear();
		indices.clear();
	}

	void Chaos_Equations::onRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;
		renderer.DrawPoint(*vao, *ib, *shader);
	}

	void Chaos_Equations::OnImguiRender() {
		ImGui::SliderFloat("T", &t, -3.0f, 3.0f, std::to_string(t).c_str());
		ImGui::SliderFloat("Speed", &speed, 0.0f, 0.0001f, std::to_string(speed).c_str());
		ImGui::SliderFloat("scale", &scale, 0.0001f, 100.0f, std::to_string(scale).c_str());
		if (ImGui::Button("Apply")) {
			glm::vec3 scale(scale, scale, 0.0f);
			proj = glm::scale(glm::mat4(1.0f), scale) * proj;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", proj);
		}
		if(ImGui::Button("Random Generate")) {
			RandParams(params);
			t = 0.01f;
		}
		std::string str = "Now Code: " + ParamsToString(params);
		ImGui::Text(str.c_str());
		if (ImGui::InputText("Code", &code[0], 7 * sizeof(char))) {
			printf("%s", code.c_str());
			StringToParams(code.c_str(), params);
			t = 0.01f;
		}
		
		ImGui::Text("Some Code :\n  DPPREG\n  RMCQDI\n  LDNMGQ\n  JJPVDN\n  HELPME\n  I_CNJJ\n  VJQZJG\n  MMJKMA\n  CODEPA\n  EPJWMY\n");
	}

	void Chaos_Equations::Point(const glm::vec2& a,const glm::vec4& color) {
		int i = vertexs.size();
		vertexs.resize(vertexs.size() + 1);

		vertexs[i].position = a;
		vertexs[i].color = color;

		indices.push_back(i);
	}

	const Vertex& Chaos_Equations::Equation(const Vertex& old_vertex) {
		Vertex vertex = {};
		const float X = old_vertex.position.x;
		const float Y = old_vertex.position.y;
		const double nx = (double)X*X * params[0] + (double)Y*Y * params[1] + (double)t*t * params[2] + (double)X*Y * params[3] + (double)X*t * params[4] + (double)Y*t * params[5] + X * params[6] + Y * params[7] + t * params[8];
		const double ny = (double)X*X * params[9] + (double)Y*Y * params[10] + (double)t*t * params[11] + (double)X*Y * params[12] + (double)X*t * params[13] + (double)Y*t * params[14] + X * params[15] + Y * params[16] + t * params[17];
		vertex.position = glm::vec2(nx,ny);
		vertex.color = old_vertex.color;
		return vertex;
	}

	const glm::vec4 Chaos_Equations::GenRandColor() const {
		
		return glm::vec4(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f, 1.0f);
		
	}

	void Chaos_Equations::Line(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color) {
		int i = vertexs.size();
		vertexs.resize(vertexs.size() + 2);

		vertexs[i].position = a;
		vertexs[i].color = color;
		vertexs[i+1].position = b;
		vertexs[i+1].color = color;

		indices.push_back(i);
		indices.push_back(i+1);
	}
}
