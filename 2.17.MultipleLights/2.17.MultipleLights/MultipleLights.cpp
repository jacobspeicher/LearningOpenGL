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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2;
float lastY = screenHeight / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.0f, 2.0f, 3.0f);

glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};

glm::vec3 pointLightAmbients[] =
{
	glm::vec3(0.2f, 0.2f, 0.0f),
	glm::vec3(0.11f, 0.05f, 0.01f),
	glm::vec3(0.2f, 0.0f, 0.0f),
	glm::vec3(0.2f, 0.2f, 0.0f),
};

glm::vec3 pointLightDiffuses[] =
{
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.33f, 0.14f, 0.04f),
	glm::vec3(0.5f, 0.0f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
};

glm::vec3 pointLightSpeculars[] =
{
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(0.66f, 0.27f, 0.07f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
};

int main()
{
#pragma region Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Multiple Lights", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::FAILED_TO_LOAD" << std::endl;
		glfwTerminate();
		return -1;
	}
#pragma endregion Init

#pragma region VertexSetup
	unsigned int lightVAO, objectVAO, VBO;
	glGenVertexArrays(1, &lightVAO);
	glGenVertexArrays(1, &objectVAO);
	glGenBuffers(1, &VBO);
#pragma endregion VertexSetup

#pragma region VertexData
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
#pragma endregion VertexData

#pragma region Textures
	stbi_set_flip_vertically_on_load(true);

	unsigned int diffuseMap, specularMap;
	int width, height, numChannels;
	unsigned char* data;

	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);

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

	stbi_image_free(data);
#pragma endregion Textures

#pragma region Shaders
	Shader lightShader("vertex.sh", "lightFragment.sh");
	Shader objectShader("vertex.sh", "objectFragment.sh");

	objectShader.use();

	objectShader.setInt("material.diffuse", 0);
	objectShader.setInt("material.specular", 1);
	objectShader.setFloat("material.shininess", 32.0f);

	objectShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	objectShader.setVec3("dirLight.ambient", glm::vec3(0.1f));
	objectShader.setVec3("dirLight.diffuse", glm::vec3(0.2f));
	objectShader.setVec3("dirLight.specular", glm::vec3(0.9f));

	for (int i = 0; i < 4; ++i)
	{
		std::string pointLight = "pointLights[" + std::to_string(i) + "]";

		objectShader.setVec3(pointLight + ".position", pointLightPositions[i]);
		objectShader.setFloat(pointLight + ".constant", 1.0f);
		objectShader.setFloat(pointLight + ".linear", 0.09f);
		objectShader.setFloat(pointLight + ".quadratic", 0.032f);
	}
	
	
	objectShader.setVec3("pointLights[0].ambient", pointLightAmbients[0]);
	objectShader.setVec3("pointLights[0].diffuse", pointLightDiffuses[0]);
	objectShader.setVec3("pointLights[0].specular", pointLightSpeculars[0]);


	objectShader.setVec3("pointLights[1].ambient", pointLightAmbients[0]);
	objectShader.setVec3("pointLights[1].diffuse", pointLightDiffuses[0]);
	objectShader.setVec3("pointLights[1].specular", pointLightSpeculars[0]);


	objectShader.setVec3("pointLights[2].ambient", pointLightAmbients[2]);
	objectShader.setVec3("pointLights[2].diffuse", pointLightDiffuses[2]);
	objectShader.setVec3("pointLights[2].specular", pointLightSpeculars[2]);


	objectShader.setVec3("pointLights[3].ambient", pointLightAmbients[2]);
	objectShader.setVec3("pointLights[3].diffuse", pointLightDiffuses[2]);
	objectShader.setVec3("pointLights[3].specular", pointLightSpeculars[2]);

	objectShader.setVec3("spotLight.ambient", glm::vec3(0.4f));
	objectShader.setVec3("spotLight.diffuse", glm::vec3(0.75f));
	objectShader.setVec3("spotLight.specular", glm::vec3(1.0f));
	objectShader.setFloat("spotLight.constant", 1.0f);
	objectShader.setFloat("spotLight.linear", 0.09f);
	objectShader.setFloat("spotLight.quadratic", 0.032f);
#pragma endregion Shaders

#pragma region Render
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//glClearColor(1.0f, 0.65f, 0.0f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(objectVAO);

		objectShader.use();
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		objectShader.setVec3("viewPos", camera.Position);

		objectShader.setVec3("spotLight.position", camera.Position);
		objectShader.setVec3("spotLight.direction", camera.Front);
		objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		for (unsigned int i = 0; i < 10; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			objectShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(lightVAO);

		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);

		for (int i = 0; i < 4; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));

			lightShader.setMat4("model", model);

			if (i < 2)
				lightShader.setVec3("lightColor", pointLightSpeculars[0]);
			else
				lightShader.setVec3("lightColor", pointLightSpeculars[2]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
#pragma endregion Render

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
	screenWidth = static_cast<float>(width);
	screenHeight = static_cast<float>(height);

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

	float yOffset = lastY - yPos;
	float xOffset = xPos - lastX;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}