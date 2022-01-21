#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// the source code for a very simple vertex shader
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// the source code for a very simple fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

// a rectangle in 3d space (flat z plane)
float vertices[] =
{
	0.5f, 0.5f, 0.0f,	// top right
	0.5f, -0.5f, 0.0f,	// bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f,	// top left
};

// indices for drawing the rectangle using two triangles
unsigned int indices[] =
{
	0, 1, 3,	// first triangle
	1, 2, 3,	// second triangle
};

// callback to modify the OpenGL rendering area when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// keeps input code organized
void processInput(GLFWwindow* window)
{
	// get the escape key in context of the window, check if pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// close the window
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	// initialize GLFW to know that OpenGL 3.3 core_profile is the version to use
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window (w, h, name, <ig>, <ig>)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Element Buffer Objects", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
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

	// tell OpenGL the size of the rendering window so it can display data and coordinates wrt the window
	glViewport(0, 0, 800, 600);

	// register the callback for every time the window resizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma region VertexArrayObject
	// create a VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind the VAO
	glBindVertexArray(VAO);
#pragma endregion VertexArrayObject

#pragma region VertexBufferObject
	// create a VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// bind the new buffer to the vertex buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy the vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion VertexBufferObject

#pragma region ElementBufferObject
	// create an Element Buffer Object (EBO)
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// copy the indices into the buffer currently bound to GL_ELEMENT_ARRAY_BUFFER (EBO)
	// GL_STATIC_DRAW: the data is set only once and used many times
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#pragma endregion ElementBufferObject

#pragma region VertextAttributes
	// tell OpenGL how to interpret the vertex data (per vertex attribute)
	// parameters:
	//		- which vertext attribute we want to configure (0)
	//		- the size of the vertext attribute (vec3)
	//		- the type of data (float)
	//		- should the data be normalized?
	//		- the STRIDE, the space between consecutive vertex attribs
	//		- the OFFSET, where the position data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertex attribute with the vertex attribute location
	glEnableVertexAttribArray(0);
#pragma endregion VertextAttributes

#pragma region VertexShader
	// create a vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach the shader source code to the shader object and compile
	// (shader object to compile, number of strings passed as source, actual source code string, <ig>)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
#pragma endregion VertexShader

#pragma region FragmentShader
	// create a fragment shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// attach the shader source code to the shader object and compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
#pragma endregion FragmentShader

#pragma region ShaderProgram
	// create a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// attach the previously compiled shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link these shaders together
	glLinkProgram(shaderProgram);

	// delete the shader objects now that they're linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion ShaderProgram

#pragma region SuccessCheck
	/*int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR\n" << infoLog << std::endl;
	}*/
#pragma endregion SuccessCheck

	// render in wirefram
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region RenderLoop
	while (!glfwWindowShouldClose(window))
	{
		// check inputs
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// (mode to draw in, number of elements to draw, type of indices, offset in the EBO)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// unbind the VAO
		glBindVertexArray(0);

		// swaps the color buffer to show output to the screen
		glfwSwapBuffers(window);
		// checks if any events are triggered (keyboard/mouse) and updates window state, calls appropriate callback function
		glfwPollEvents();
	}
#pragma endregion RenderLoop

	// clean/delete all of GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}