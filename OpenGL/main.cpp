
// GLEW is used to go into drivers and get function declarations/definitions for OpenGL > 1.1 which is what Windows goes up to
#include <GL/glew.h> // this must be included before other OpenGL things
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0]
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Query shader that is compiled to see if anything is wrong
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // i means int, v means vector
	if (result == GL_FALSE)
	{
		// Get the length of the error message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//alloca is a C function that lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		type == GL_VERTEX_SHADER ? "vertex" : "fragment";
		std::cout << "Failed to compile " << type << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

/* Takes in actual source code for these */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Documentation is available on these
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// delete intermediate (obj files) since they've been linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// This must be called after the GLFW context is created
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positionsBuffer[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};
	// the buffer writes into this unsigned int's memory
	unsigned int buffer;
	glGenBuffers(1, &buffer); // only want one buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //select which buffer and what kind (array buffer)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positionsBuffer, GL_STATIC_DRAW); // STATIC_DRAW is a hint as to how many times this will be used
	// OpenGL is a state machine so it's gradually learning more about our data

	// Tell OpenGL what kind/how much data we're giving it
	glEnableVertexAttribArray(0); //order of enabling and having the pointer doesn't matter because it's a state machine (as long as buffer bound)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);// only specifying one attribute so only need to call this once
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location=0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location=0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color=vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw using immediate mode with OpenGL 1.1 (no GLEW needed)
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();*/

		//Draws the buffer that's currently bound
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;

}