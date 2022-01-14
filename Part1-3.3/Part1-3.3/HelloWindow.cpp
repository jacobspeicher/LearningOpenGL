#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// the source code for a very simple vertex shader
// each vertex position is passed into the gl_Position variable, which sets the output
// of this shader
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// the source code for a very simple fragment shader
// requires one output variable, FragColor, that is a vec4 of a color for the vertex
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

// a triangle in 3d space (flat z plane, because it's really 2d)
float vertices[] =
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
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
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); <- add this for MacOS X for the initialization code to work (unsure if still works for newer versions, probably though)

	// create a window (w, h, name, <ignore>, <ignore>)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
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

	// tell OpenGL the size of the rendering window so it can display data and coordinates with respect to the window
	// ((location of lower left of window), w, h)
	glViewport(0, 0, 800, 600);

	// register the callback for every time the window resizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

#pragma region VertexArrayObject
	// create a Vertex Array Object (VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind the VAO
	glBindVertexArray(VAO);
#pragma endregion VertexArrayObject

#pragma region VertexBufferObject
	// create a vertex buffer object (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// bind the new buffer to the vertex buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy the previously declared vertex data into the buffer's memory
	// this copies into the buffer currently bound to type GL_ARRAY_BUFFER (VBO)
	// tells the GPU how to handle the data:
	//		- GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times
	//		- GL_STATIC_DRAW: the data is set only once and used many times
	//		- GL_DYNAMIC_DRAW: the data is changed a lot and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion VertexBufferObject

#pragma region InterpretVertexData
	// vertex data is stored as 3 4-byte floating point values
	// values are tightly packed
	// the first value is at the beginning of the buffer

	// tell OpenGL how to interpret the vertex data (per vertex attribute)
	// parameters:
	//		- which vertex attribute we want to configure (`layout (location = 0)` from the vertex shader, set this data at 0)
	//		- the size of the vertex attrtibute (vec3)
	//		- the type of data (float)
	//		- should the data be normalized?
	//		- the STRIDE, the space between consecutive vertex attribs (since values are tightly packed, could have passed 0 to let OpenGL determine the stride)
	//		- the OFFSET, where the position data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertext attribute with the vertex attribute location (`layout (location = 0)`)
	glEnableVertexAttribArray(0);
#pragma endregion InterpretVertexData

#pragma region VertexShader
	// create a vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach the shader source code to the shader object and compile
	// (shader object to compile, number of strings passed as source, actual source code string, <ignore>)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for success of compiling the shader
	int vertexSuccess;
	char vertexInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	if (!vertexSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
	}
#pragma endregion VertexShader

#pragma region FragmentShader
	// create a fragment shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// attach the shader source code to the shader object and compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for success of compiling the shader
	int fragmentSuccess;
	char fragmentInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
	if (!fragmentSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
	}
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

	// check for the success of the shader program linking
	int programSuccess;
	char programInfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, programInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << programInfoLog << std::endl;
	}

	// delete the shader objects
	// now that they're linked, we don't need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion ShaderProgram

#pragma region RenderLoop
	// render loop will continue runing until the user tells it to stop
	// checks if GLFW has been instructed to close
	// this loop will be called every frame
	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) 
	{
		// check all inputs
		processInput(window);

		// rendering commands
		// clear the color buffer to clear all the color values
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// (the OpenGL primitive to draw, the starting index of the vertex array, how many vertices to draw)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swaps the color buffer to show output to the screen
		glfwSwapBuffers(window);
		// checks if any events are triggered (keyboard/mouse) and updates window state, calls appropriate callback functions
		glfwPollEvents();
	}
#pragma endregion RenderLoop

	// clean/delete all of GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}