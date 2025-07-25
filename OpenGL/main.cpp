
// GLEW is used to go into drivers and get function declarations/definitions for OpenGL > 1.1 which is what Windows goes up to
#include <GL/glew.h> // this must be included before other OpenGL things
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


static ShaderProgramSource ParseShader(const std::string& file)
{
	std::ifstream stream(file);
	std::string line;

	std::stringstream stringStreams[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			stringStreams[(int)type] << line << '\n';
		}
	}

	return { stringStreams[0].str(), stringStreams[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0]
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// Query shader that is compiled to see if anything is wrong
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // i means int, v means vector
	if (result == GL_FALSE)
	{
		// Get the length of the error message
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//alloca is a C function that lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

/* Takes in actual source code for these */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Documentation is available on these
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// delete intermediate (obj files) since they've been linked
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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
			-0.5f, -0.5f,
			 0.5f,  -0.5f,
			 0.5f, 0.5f,
			 -0.5f, 0.5f,
		};

		// These are the indices from positionsBuffer that we want (instead of repeating the duplicate vertices). Must be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// when you specify the vertex attrib pointer, it binds the vertex array to the data (in fewer steps)
		unsigned int vertexArray;
		GLCall(glGenVertexArrays(1, &vertexArray));
		GLCall(glBindVertexArray(vertexArray)); // no target here

		VertexBuffer vertexBuffer(positionsBuffer, 4 * 2 * sizeof(float));

		// Tell OpenGL what kind/how much data we're giving it
		GLCall(glEnableVertexAttribArray(0)); //order of enabling and having the pointer doesn't matter because it's a state machine (as long as buffer bound)
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));// only specifying one attribute so only need to call this once


		IndexBuffer indexBuffer(indices, 6);

		ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		//binds the shader so it knows which one to send data to
		GLCall(glUseProgram(shader));

		//note that uniforms are set per color
		//retrieve location of u_Color and then define it to write to color
		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); // allows us to move code from shader to C++

		// Unbind what we just bound so other things can be bound
		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			//Draw using immediate mode with OpenGL 1.1 (no GLEW needed)
			/*glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glEnd();*/

			//Draws the buffer that's currently bound
			//glDrawArrays(GL_TRIANGLES, 0, 6); // only when no index buffer

			// Bind shader and set up uniform
			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // allows us to move code from shader to C++

			// Bind vertex buffer
			//GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferSlot));
			//GLCall(glEnableVertexAttribArray(0));
			//// Specify layout for vertex buffer. Vertex arrays would take care of this step and the above step of binding the vertex buffer
			//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

			GLCall(glBindVertexArray(vertexArray)); // when doing it like this, the vertex buffer isn't bound

			// Bind index buffer
			indexBuffer.Bind();

			// Call glDrawElements
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			// Vertex buffer, index buffer, GL_DRAW_ELEMENTS is the key

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		GLCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}