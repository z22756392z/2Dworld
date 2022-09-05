#include "Plot_of_Big_O_effeciency_measures.h"
#include "Renderer.h"


#include "glm/gtc/matrix_transform.hpp"
#include "imgui-master/imgui.h"
#include <string.h>
#include <iostream>

namespace test {
	Plot_of_Big_O_effeciency_measures::Plot_of_Big_O_effeciency_measures() :
		scale(50.0f),
		stride(0.2f),
		m_va(),
		vbo(nullptr),
		m_ib(nullptr),
		shader(nullptr),
		m_vertices(0),
		m_indices(0)
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		vbo = new VertexBuffer();
		m_ib = new IndexBuffer();
		m_va = new VertexArray();
		m_va->AddBuffer(*vbo, layout);
		
		glm::mat4 proj = glm::ortho(0.0f, 1000.0f, 0.0f, 600.0f,-1.0f,1.0f);

		shader = new Shader("res/shaders/BigO.shader");
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", proj);

		vbo->Unbind();
		m_ib->Unbind();
		m_va->Unbind();
		shader->Unbind();
	}
	void Plot_of_Big_O_effeciency_measures::onUpdate() {
		int b = 3;
		int& a = b;
		stride = 15 / scale;
		Linear(glm::vec2(200.0f, 500.0f),glm::vec2(200.0f, 100.0f),glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Linear(glm::vec2(600.0f, 100.0f), glm::vec2(200.0f, 100.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Linear(glm::vec2(200.0f, 100.0f), glm::vec2(600.0f, 500.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		Linear(glm::vec2(200.0f + scale, 120.0f), glm::vec2(200.0f + scale, 80.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		Linear_logarithmic(glm::vec2(200.0f, 100.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),stride,scale);
		Logarithmic(glm::vec2(200.0f, 100.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), stride, scale);

		Quadratic(glm::vec2(200.0f, 100.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), stride, scale, 3.0f);
		Quadratic(glm::vec2(200.0f, 100.0f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), stride, scale, 2.0f);
		
		vbo->Allocate(m_vertices.size() * sizeof(vertices));//allocate memory
		m_ib->Allocate(m_indices.size());
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(vertices), m_vertices.data()));//upload data
		GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(unsigned int), m_indices.data()));
		vbo->Unbind();
		m_ib->Unbind();
		m_vertices.clear();
		m_indices.clear();
	}

	void Plot_of_Big_O_effeciency_measures::onRender() {
		GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;
		renderer.DrawLine(*m_va, *m_ib, *shader,NULL);
	}

	void Plot_of_Big_O_effeciency_measures::OnImguiRender() {
		ImGui::Text("X axis is n\nY axis is O(n)\nBule line is n\nGreen line is nlogn\nIndigo line is logn\nRed line is n^3\nOrange line is n^2");

		ImGui::SliderFloat("scale", &scale, 30, 200, &(std::to_string(scale)+ " pixel")[0]);
	}
	
	void Plot_of_Big_O_effeciency_measures::Logarithmic(const glm::vec2(position),const glm::vec4(color),const float stride, const float scale,float increment /*= 1*/) {
		int j = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 150);
		float count = 0;
		for (int i = j; i < j+150; i += 2) {
			if (position.x + (count * scale) > 600.0f || position.y + log10(count) * scale * increment > 500.0f) break;
			m_vertices[i].position = glm::vec2(position.x+(count * scale),position.y + log10(count) * scale * increment);
			m_vertices[i].color = glm::vec4(color);
			m_vertices[i+1].position = glm::vec2(position.x+((count + stride) * scale), position.y + log10((count + stride)) * scale * increment);
			m_vertices[i+1].color = glm::vec4(color);
			m_indices.push_back(i);
			m_indices.push_back(i + 1);
			count += stride;
		}
	}

	void Plot_of_Big_O_effeciency_measures::Linear(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color) {
		int i = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 2);

		m_vertices[i].position = a;
		m_vertices[i].color = color;
		m_vertices[i + 1].position = b;
		m_vertices[i + 1].color = color;

		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	void Plot_of_Big_O_effeciency_measures::Linear_logarithmic(const glm::vec2(position), const glm::vec4(color), const float stride, const float scale) {
		int j = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 150);
		float count = 0;
		for (int i = j; i < j + 150; i += 2) {
			if (position.x + (count * scale) > 600.0f || position.y + log10(count) * count * scale > 500.0f) break;
			m_vertices[i].position = glm::vec2(position.x + (count * scale), position.y + log10(count) * count * scale);
			m_vertices[i].color = glm::vec4(color);
			m_vertices[i + 1].position = glm::vec2(position.x + ((count + stride) * scale), position.y + log10((count + stride)) * (count + stride) * scale);
			m_vertices[i + 1].color = glm::vec4(color);
			m_indices.push_back(i);
			m_indices.push_back(i + 1);
			count+= stride;
		}
	}
	
	void Plot_of_Big_O_effeciency_measures::Quadratic(const glm::vec2(position), const glm::vec4(color), const float stride, const float scale, float increment /*= 1*/) {
		int j = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 50);
		float count = 0;
		for (int i = j; i < j + 50; i += 2) {
			if (position.x + (count * scale) > 600.0f || position.y + pow(count, increment) * scale > 500.0f) break;
			m_vertices[i].position = glm::vec2(position.x + (count * scale), position.y + pow(count, increment) * scale);
			m_vertices[i].color = glm::vec4(color);
			m_vertices[i + 1].position = glm::vec2(position.x + ((count + stride) * scale), position.y + pow(count+ stride, increment) * scale) ;
			m_vertices[i + 1].color = glm::vec4(color);
			m_indices.push_back(i);
			m_indices.push_back(i + 1);
			count+= stride;
		}
	}

	void Plot_of_Big_O_effeciency_measures::Polynomial() {

	}
}