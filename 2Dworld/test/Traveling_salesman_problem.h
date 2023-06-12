#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <string>
#include <vector>
namespace test {
	class Traveling_salesman_problem : public Test
	{
	public:
		Traveling_salesman_problem();
		~Traveling_salesman_problem();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;

		float distance(const std::pair<int, int> a, const std::pair<int, int> b);
		std::vector<std::pair<int, int>> ParseData(const std::string filePath);

	private:
		std::vector<std::pair<int, int>> _city_coordiates;
		VertexArray* _vao;
		VertexBuffer* _vbo;
		IndexBuffer* _ibo;
		Shader* _shader;
		Texture* _texture;
	};
}


