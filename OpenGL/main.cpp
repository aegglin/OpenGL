
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

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core means that you have to explicitly use vertex array objects. compatibility has a default one


	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		float positionsBuffer[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f,  -0.5f, 1.0f, 0.0f,
			 0.5f, 0.5f, 1.0f, 1.0f,
			 -0.5f, 0.5f, 0.0f, 1.0f
		};

		// These are the indices from positionsBuffer that we want (instead of repeating the duplicate vertices). Must be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positionsBuffer, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		VertexBuffer vertexBuffer(positionsBuffer, 4 * 2 * sizeof(float));

		IndexBuffer indexBuffer(indices, 6);

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

		float r = 0.0f;
		float increment = 0.05f;

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

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
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f); // allows us to move code from shader to C++
			renderer.Draw(va, indexBuffer,shader);

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}