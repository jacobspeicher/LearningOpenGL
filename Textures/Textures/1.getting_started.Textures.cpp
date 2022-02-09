#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"

float vertices[] =
{
	// pos			   // colors		 // texture coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,		// top right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// bottom left
	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// top left
};

//float vertices[] =
//{
//	// pos			   // colors		 // texture coords
//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.51f, 0.51f,		// top right
//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.51f, 0.5f,	// bottom right
//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f,	// bottom left
//	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.51f,	// top left
//};

unsigned int indices[] =
{
	0, 1, 3,
	1, 2, 3,
};

float mixFactor = 0.2f;

// callback to modify rendering area when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
	{
		if (mixFactor < 1.0f)
		{
			mixFactor += 0.01f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		if (mixFactor > 0.0f)
		{
			mixFactor -= 0.01f;
		}
	}
}

int main()
{
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Textures", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialize GLAD for function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// create vertex objects
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the objects
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy previously declared vertex data into buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// copy the indices into the Element Buffer Object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// interpret and enable the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// interpret and enable the color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// interpret and enable the texture coords data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// flip loaded images upside down so they appear upside right
	stbi_set_flip_vertically_on_load(true);

	// generate a texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options
	// TEXTURE WRAPPING
	//		- GL_REPEAT: the default behavior for textures, repeats the texture image
	// 		- GL_MIRRORED_REPEAT: same as GL_REPEAT but mirrors the image with each repeat
	// 		- GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. Result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern
	//		- GL_CLAMP_TO_BORDER: coordinates outside the range are now given a user specified border color
	// 	TEXTURE FILTERING (which texture pixel [texel] to map the texture coordinate to)
	//		- GL_NEAREST: default texture filtering method of OpenGL. Selects the texel with a center closest to the texture coordinate
	//		- GL_LINEAR: (bi)linear filtering, takes an interpolated value from the texture coordinates neighboring texels, approximating a color between the texels
	// 	TEXTURE FILTERING FOR MIPMAPS
	//		- GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel saize and uses nearest neighbor interpolation for texture sampling
	//		- GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation
	//		- GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation
	//		- GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load image data
	int width, height, nrChannels;
	// (loc of image file, width, height, number of color channels, <ignore>)
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		// load the texture
		// arguments:
		//	- texture target: GL_TEXTURE_2D means this will generate a texture on the currently bound texture object at the same target
		//	- mipmap level to create a texture for (if you want to do this manually, 0 is base level)
		//	- what format to store the texture
		//	- width, height
		//	- always 0 (legacy stuff)
		//	- format of image
		//	- datatype of image
		//	- actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// free the image memory
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load second texture" << std::endl;
	}

	stbi_image_free(data);

	// create the shader
	Shader shaderProgram("vertex.vs", "fragment.fs");

	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); // active texture unit first
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shaderProgram.use();

		shaderProgram.setFloat("mixFactor", mixFactor);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}