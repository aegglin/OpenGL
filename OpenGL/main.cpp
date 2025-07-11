
// GLEW is used to go into drivers and get function declarations/definitions for OpenGL > 1.1 which is what Windows goes up to
#include <GL/glew.h> // this must be included before other OpenGL things
#include <GLFW/glfw3.h>

#include <iostream>

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


	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	glfwTerminate();
	return 0;

}