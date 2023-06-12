#pragma once
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
namespace test {
	class EdgeDetection : public Test
	{
	public:
		EdgeDetection();
		~EdgeDetection();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;

	private:
		VertexArray* m_va;
		VertexBuffer* m_vb;
		IndexBuffer* m_ib;
		Shader* m_shader;
		Texture* m_texture;
		float threshold;
		int mode;
	};
}
