#pragma once
#include"Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <math.h>
#include <vector>

namespace test {
	class Plot_of_Big_O_effeciency_measures : public Test
	{
	public:
		Plot_of_Big_O_effeciency_measures();
		~Plot_of_Big_O_effeciency_measures() {};

		void onUpdate();
		void onRender();
		void OnImguiRender();

		struct vertices {
			glm::vec2 position;
			glm::vec4 color;
		};
	private:
		float scale;
		float stride;
		VertexArray* m_va;
		VertexBuffer* vbo;
		IndexBuffer* m_ib;
		Shader* shader;
		std::vector<vertices> m_vertices;
		std::vector<unsigned int> m_indices;

		void Logarithmic(const glm::vec2(position), const glm::vec4(color) ,const float stride,const float scale,const float increment = 1.0f);
		void Linear(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color);
		void Linear_logarithmic(const glm::vec2(position), const glm::vec4(color), const float stride, const float scale);
		void Quadratic(const glm::vec2(position), const glm::vec4(color), const float stride, const float scale, const float increment = 1) ;
		void Polynomial(); 
	};
}


