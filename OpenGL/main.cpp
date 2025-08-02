
// GLEW is used to go into drivers and get function declarations/definitions for OpenGL > 1.1 which is what Windows goes up to
#include <GL/glew.h> // this must be included before other OpenGL things
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "tests/TestClearColor.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core means that you have to explicitly use vertex array objects. compatibility has a default one


	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Change the frame rate 
	glfwSwapInterval(1);

	// This must be called after the GLFW context is created
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		// Add blending -- when we render something that is partially/fully transparent, how is that done? 
		// blending determines how we combine output color with what is already in target buffer
		GLCall(glEnable(GL_BLEND));
		// specifies how we blend two colors together
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;
		
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::TestClearColor test;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			test.OnUpdate(0.0f);
			test.OnRender();

			// Need this before any ImGui calls
			ImGui_ImplGlfwGL3_NewFrame();
			test.OnImGuiRender();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}