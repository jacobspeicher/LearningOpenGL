#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

float vertices[] =
{
	// pos					// cube face normals	 // texture coords
	-0.5f,	-0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 0.0f,	 0.0f,	// bottom left
	 0.5f,	-0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 1.0f,	 0.0f,	// bottom right
	 0.5f,	 0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	 0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	 0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 0.0f,	 1.0f,	// top left
	-0.5f,	-0.5f,	-0.5f,   0.0f,   0.0f,  -1.0f,	 0.0f,	 0.0f,	// bottom left

	-0.5f,	-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 0.0f,	 0.0f,	// bottom left
	 0.5f,	-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 1.0f,	 0.0f,	// bottom right
	 0.5f,	 0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	 0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	 0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 0.0f,	 1.0f,	// top left
	-0.5f,	-0.5f,	 0.5f,   0.0f,   0.0f,   1.0f,	 0.0f,	 0.0f,	// bottom left

	-0.5f,	 0.5f,	 0.5f,  -1.0f,   0.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left
	-0.5f,	 0.5f,	-0.5f,  -1.0f,   0.0f,   0.0f,	 1.0f,	 0.0f,	// bottom right
	-0.5f,	-0.5f,	-0.5f,  -1.0f,   0.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	-0.5f,	-0.5f,  -1.0f,   0.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	-0.5f,	 0.5f,  -1.0f,   0.0f,   0.0f,	 0.0f,	 1.0f,	// top left
	-0.5f,	 0.5f,	 0.5f,  -1.0f,   0.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left

	 0.5f,	 0.5f,	 0.5f,   1.0f,   0.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left
	 0.5f,	 0.5f,	-0.5f,   1.0f,   0.0f,   0.0f,	 1.0f,	 0.0f,	// bottom right
	 0.5f,	-0.5f,	-0.5f,   1.0f,   0.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	-0.5f,	-0.5f,   1.0f,   0.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	-0.5f,	 0.5f,   1.0f,   0.0f,   0.0f,	 0.0f,	 1.0f,	// top left
	 0.5f,	 0.5f,	 0.5f,   1.0f,   0.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left

	-0.5f,	-0.5f,	-0.5f,   0.0f,   -1.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left
	 0.5f,	-0.5f,	-0.5f,   0.0f,   -1.0f,   0.0f,	 1.0f,	 0.0f,	// bottom right
	 0.5f,	-0.5f,	 0.5f,   0.0f,   -1.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	-0.5f,	 0.5f,   0.0f,   -1.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	-0.5f,	 0.5f,   0.0f,   -1.0f,   0.0f,	 0.0f,	 1.0f,	// top left
	-0.5f,	-0.5f,	-0.5f,   0.0f,   -1.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left

	-0.5f,	 0.5f,	-0.5f,   0.0f,    1.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left
	 0.5f,	 0.5f,	-0.5f,   0.0f,    1.0f,   0.0f,	 1.0f,	 0.0f,	// bottom right
	 0.5f,	 0.5f,	 0.5f,   0.0f,    1.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	 0.5f,	 0.5f,	 0.5f,   0.0f,    1.0f,   0.0f,	 1.0f,	 1.0f,	// top right
	-0.5f,	 0.5f,	 0.5f,   0.0f,    1.0f,   0.0f,	 0.0f,	 1.0f,	// top left
	-0.5f,	 0.5f,	-0.5f,   0.0f,    1.0f,   0.0f,	 0.0f,	 0.0f,	// bottom left
};

float screenWidth = 800.0f;
float screenHeight = 600.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2;
float lastY = screenHeight / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
#pragma region Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Light Casters", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::FAILED_TO_CREATE" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::FAILED_TO_INIT" << std::endl;
		glfwTerminate();
		return -1;
	}
#pragma endregion init

#pragma region VertexArrays
	unsigned int objectVAO, lightVAO, VBO;

	glGenVertexArrays(1, &objectVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
#pragma endregion VertexArrays

#pragma region InterpretArrays
	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#pragma endregion InterpretArrays

#pragma region Textures
	unsigned int diffuseMap, specularMap;
	int width, height, numChannels;
	unsigned char* data;

	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);

	stbi_set_flip_vertically_on_load(true);

	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("container2.png", &width, &height, &numChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "ERROR::TEXTURE::DIFFUSE_MAP_FAILED" << std::endl;

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, specularMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("container2_specular.png", &width, &height, &numChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "ERROR::TEXTURE::SPECULAR_MAP_FAILED" << std::endl;

	stbi_image_free(data);
#pragma endregion Textures

#pragma region Shaders
	Shader objectShader("vertex.sh", "objectFragment.sh");
	Shader lightShader("vertex.sh", "lightFragment.sh");

	objectShader.use();
	objectShader.setInt("material.diffuse", 0);
	objectShader.setInt("material.specular", 1);
	objectShader.setFloat("material.shininess", 32.0f);

	//objectShader.setVec3("light.position", lightPos);
	//objectShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	objectShader.setVec3("light.ambient", glm::vec3(0.2f));
	objectShader.setVec3("light.diffuse", glm::vec3(0.5f));
	objectShader.setVec3("light.specular", glm::vec3(1.0f));
	objectShader.setFloat("light.constant", 1.0f);
	objectShader.setFloat("light.linear", 0.09f);
	objectShader.setFloat("light.quadratic", 0.032f);
#pragma endregion Shaders

#pragma region RenderLoop
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] =
	{
		glm::vec3(-1.0f, 2.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, -1.2f, 0.0f),
		glm::vec3(5.0f, -3.0f, -3.0f),
		glm::vec3(3.5f, -4.5f, -6.5f),
		glm::vec3(7.0f, -0.0f, -3.5f),
		glm::vec3(-3.5f, 3.25f, -1.0f),
		glm::vec3(2.75f, 2.75f, -2.75f),
		glm::vec3(6.0f, -1.75f, -5.0f),
		glm::vec3(1.0f, -2.5f, -3.2f),
	};

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(objectVAO);

		objectShader.use();
		objectShader.setMat4("view", 1, view);
		objectShader.setMat4("projection", 1, projection);
		objectShader.setVec3("viewPos", camera.Position);
		objectShader.setVec3("light.position", camera.Position);
		objectShader.setVec3("light.direction", camera.Front);
		objectShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		objectShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		for (unsigned int i = 0; i < 10; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			objectShader.setMat4("model", 1, model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(lightVAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader.use();
		lightShader.setMat4("model", 1, model);
		lightShader.setMat4("view", 1, view);
		lightShader.setMat4("projection", 1, projection);
		lightShader.setVec3("lightColor", glm::vec3(1.0f));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
#pragma endregion RenderLoop

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	screenWidth = (float)width;
	screenHeight = (float)height;
	glViewport(0, 0, screenWidth, screenHeight);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}