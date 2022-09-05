#pragma once
#include "Test.h"

#include "VertexArray.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace test {
	class TestTexture :  public Test {
	private:
		VertexArray* m_va;
		VertexBuffer* m_vb;
		IndexBuffer* m_ib;
		Shader* m_shader;
		Texture* m_texture;
	public:
		TestTexture();
		~TestTexture();

		void onUpdate() override;
		void onRender() override;
		void OnImguiRender() override;
	};
}