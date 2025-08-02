#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {
	class TestTexture2D : public Test {
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Texture> texture;

		glm::vec3 translationA, translationB;
		glm::mat4 proj, view;
	};
}