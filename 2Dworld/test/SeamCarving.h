#pragma once
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <glm/vec3.hpp>
#include <vector>
namespace test {
	class SeamCarving : public Test
	{
	public:
		SeamCarving();
		~SeamCarving();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;

		void CalculateEnergyMap();
		float CalculatePixelEnergy(const int x, const int y);
		void RemoveSeams();
		std::vector<int> FindLowestEnergySeam();
		void RemoveSeamFromImage(const std::vector<int>& seam);
		void UpdateEnergyMap(const std::vector<int>& seam);
	
		void DrawTexture();
		void DrawNextSeam(const std::vector<int>& seam);
		void DrawEnergyMap();

		glm::vec4 GetBufferColor(const int x, const int y) const;
		void SetBufferColor(const int x, const int y, const glm::vec4& color);
		bool CheckBound(const int x, const int y) const;
		
	private:
		VertexArray* m_va;
		VertexBuffer* m_vb;
		IndexBuffer* m_ib;
		Shader* m_shader;
		Texture* m_texture;
		float m_threshold;
		int m_seam_carve_number;
		unsigned char* m_buffer;
		
		int m_width;
		int m_height;
		std::vector<std::vector<float>> m_energyMap;

		bool m_isEnergyMap;
	};
}

