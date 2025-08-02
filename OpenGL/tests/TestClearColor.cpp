#include "TestClearColor.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {

	test::TestClearColor::TestClearColor()
		: clearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
		
	}

	test::TestClearColor::~TestClearColor()
	{

	}

	void test::TestClearColor::OnUpdate(float deltaTime)
	{

	}

	void test::TestClearColor::OnRender()
	{
		GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void test::TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear color", clearColor);
	}
}