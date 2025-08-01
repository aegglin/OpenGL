
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
		// this is where the vertices' coordinates are
		float vertexPositionsBuffer[] = {
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			200.0f, 200.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 1.0f
		};

		// These are the indices from positionsBuffer that we want (instead of repeating the duplicate vertices). Must be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Add blending -- when we render something that is partially/fully transparent, how is that done? 
		// blending determines how we combine output color with what is already in target buffer
		GLCall(glEnable(GL_BLEND));
		// specifies how we blend two colors together
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(vertexPositionsBuffer, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer indexBuffer(indices, 6);

		// order in ortho is left, right, top, bottom, near, far (near and far are the Z-axis)
		// these are the bounds on our window (if vertex position was > the max they wouldn't appear)
		// projection matrix converts into (-1, 1) range
		glm::mat4 projectionMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);// othrographic matrix that is basically 4 x 3
		// view matrix is/handles the camera -> basically moving the scene 100 units to the left to simulate moving the camera to the right
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		

		Shader shader("../res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("../res/textures/beetle.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		// Unbind what we just bound so other things can be bound
		va.Unbind();
		vb.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		Renderer renderer;
		
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			// Need this before any ImGui calls
			ImGui_ImplGlfwGL3_NewFrame();

			// the model matrix is the object itself -- move up and to the right
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation);
			// this is the MVP (reverese order b/c it's column major) -> camera * transform of object * projection to normalize to -1 to 1
			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

			//Draw using immediate mode with OpenGL 1.1 (no GLEW needed)
			/*glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();*/

			//Draws the buffer that's currently bound
			//glDrawArrays(GL_TRIANGLES, 0, 6); // only when no index buffer

			// Bind shader and set up uniform, done each frame
			shader.Bind();
			shader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);

			renderer.Draw(va, indexBuffer,shader);

			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			
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