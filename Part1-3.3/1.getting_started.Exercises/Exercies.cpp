#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// yellow fragment shader
const char* yellowFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

float doubleVertices[] =
{
	-1.0f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	1.0f, -0.5f, 0.0f,
};

// a triangle in 3d space (flat z plane)
float leftVertices[] =
{
	-1.0f, 0.5f, 0.0f, // left
	-0.5f, 1.0f, 0.0f,	// top
	0.0f, 0.5f, 0.0f,	// right
};

float rightVertices[] =
{
	0.0f, 0.5f, 0.0f,	// left
	0.5f, 1.0f, 0.0f,	// top
	1.0f, 0.5f, 0.0f,	// right
};

// callback to modify the OpenGL rendering area when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// keep input processing organized
void processInput(GLFWwindow* window)
{
	// get the escape key in context of window, check if pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// close the window
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	// initialize GLFW to know to use OpenGL 3.3 core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercises", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window the context on the current thread
	glfwMakeContextCurrent(window);

	// initialize GLAD for OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell OpenGL the size of the rendering window so it can display data and coordinates with respect to the window
	glViewport(0, 0, 800, 600);

	// register the callback for every time the window resizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma region SingleArrayMoreVertices
	// create vertex objects
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the objects
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy previously declared vertex data into the buffers memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(doubleVertices), doubleVertices, GL_STATIC_DRAW);

	// interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertex attrib with the vertex attrib location
	glEnableVertexAttribArray(0);
#pragma endregion SingleArrayMoreVertices

#pragma region TwoArraysMoreObjects
	// create vertex objects
	unsigned int leftVAO;
	unsigned int leftVBO;
	
	unsigned int rightVAO;
	unsigned int rightVBO;

	glGenVertexArrays(1, &leftVAO);
	glGenBuffers(1, &leftVBO);

	glGenVertexArrays(1, &rightVAO);
	glGenBuffers(1, &rightVBO);

	// bind left objects
	glBindVertexArray(leftVAO);
	glBindBuffer(GL_ARRAY_BUFFER, leftVBO);

	// copy vertex data into the left VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftVertices), leftVertices, GL_STATIC_DRAW);

	// interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertex attrib with the vertex attrib location
	glEnableVertexAttribArray(0);

	// bind right objects
	glBindVertexArray(rightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rightVBO);

	// copy vertex data into the right VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightVertices), rightVertices, GL_STATIC_DRAW);

	// interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertex attrib with the vertex attrib location
	glEnableVertexAttribArray(0);
#pragma endregion TwoArraysMoreObjects

	// create the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach vertex shader source and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

#pragma region OrangeShaderProgram
	// create shader objects
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();

	// attach fragment shader source and compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// attach compiled shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link the shaders together
	glLinkProgram(shaderProgram);

	// delete compiled shaders
	glDeleteShader(fragmentShader);
#pragma endregion OrangeShaderProgram

#pragma region YellowShaderProgram
	unsigned int yellowFragmentShader;
	unsigned int yellowShaderProgram;
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	yellowShaderProgram = glCreateProgram();

	// attach fragment shader source and compile
	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);

	// attach compiled shaders to the program
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);

	// link the shaders
	glLinkProgram(yellowShaderProgram);

	// delete fragment shader
	glDeleteShader(yellowFragmentShader);
#pragma endregion YellowShaderProgram

	// delete the vertex shader
	glDeleteShader(vertexShader);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// check inputs
		processInput(window);

		// render commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader program
		glUseProgram(shaderProgram);

#pragma region DrawSingleArrayMoreVertices
		glBindVertexArray(VAO);

		// draw the arrays
		glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion DrawSingleArrayMoreVertices
#pragma region DrawTwoArraysMoreObjects
		// draw left triangle
		glBindVertexArray(leftVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// switch to the yellow shader program
		glUseProgram(yellowShaderProgram);

		// draw right triangle
		glBindVertexArray(rightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
#pragma endregion DrawTwoArraysMoreObjects

		// swaps the color buffer to show output
		glfwSwapBuffers(window);
		// checks if any events are triggered
		glfwPollEvents();
	}

	// clean/delete all of GLFW's resources
	glfwTerminate();
	
	return 0;
}