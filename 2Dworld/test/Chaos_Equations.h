#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include <vector>

namespace test {
	class Chaos_Equations : public Test
	{
	public:
		Chaos_Equations();
		~Chaos_Equations();

		void onUpdate();
		void onRender();
		void OnImguiRender();
	
	private:
		float x,y,scale;
		glm::mat4 proj;
		int num_point;
		float speed,t;
		unsigned int index_data;
		double params[18];
		std::string code;
		std::vector<Vertex> vertexs;
		VertexArray* vao;
		VertexBuffer* vb;
		IndexBuffer* ib;
		Shader* shader;
		std::vector<unsigned int> indices;
		glm::vec4 color[1000];
		
		const Vertex& Equation(const Vertex& old_vertex);
		void Point(const glm::vec2& a, const glm::vec4& color);
		void Line(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color);
		const glm::vec4 GenRandColor() const;
	};
}


