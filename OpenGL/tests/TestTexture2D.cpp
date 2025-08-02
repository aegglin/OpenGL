#include "TestTexture2D.h"

#include <GL/glew.h>
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

	test::TestTexture2D::TestTexture2D()
		: translationA(200, 200, 0), translationB(400, 200, 0),
		proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		view(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0)))
	{
		float positions[] = {
		-50.0f, -50.0f, 0.0f, 0.0f,
		 50.0f, -50.0f, 1.0f, 0.0f,
		 50.0f,  50.0f, 1.0f, 1.0f,
		-50.0f,  50.0f, 0.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;

		vertexArray = std::make_unique<VertexArray>();

		layout.Push<float>(2);
		layout.Push<float>(2);
		vertexArray->AddBuffer(*vb, layout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		shader->Bind();

		texture = std::make_unique<Texture>("res/textures/Beetle.png");
		shader->SetUniform1i("u_Texture", 0);
	}

	test::TestTexture2D::~TestTexture2D()
	{

	}

	void test::TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void test::TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		texture->Bind();
	
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj * view * model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proj * view * model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}

	}

	void test::TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}