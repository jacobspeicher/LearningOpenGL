#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

float vertices[] =
{
	// pos
	-0.5f,	-0.5f,	-0.5f,
	 0.5f,	-0.5f,	-0.5f,
	 0.5f,	 0.5f,	-0.5f,
	 0.5f,	 0.5f,	-0.5f,
	-0.5f,	 0.5f,	-0.5f,
	-0.5f,	-0.5f,	-0.5f,

	-0.5f,	-0.5f,	 0.5f,
	 0.5f,	-0.5f,	 0.5f,
	 0.5f,	 0.5f,	 0.5f,
	 0.5f,	 0.5f,	 0.5f,
	-0.5f,	 0.5f,	 0.5f,
	-0.5f,	-0.5f,	 0.5f,

	-0.5f,	 0.5f,	 0.5f,
	-0.5f,	 0.5f,	-0.5f,
	-0.5f,	-0.5f,	-0.5f,
	-0.5f,	-0.5f,	-0.5f,
	-0.5f,	-0.5f,	 0.5f,
	-0.5f,	 0.5f,	 0.5f,

	 0.5f,	 0.5f,	 0.5f,
	 0.5f,	 0.5f,	-0.5f,
	 0.5f,	-0.5f,	-0.5f,
	 0.5f,	-0.5f,	-0.5f,
	 0.5f,	-0.5f,	 0.5f,
	 0.5f,	 0.5f,	 0.5f,

	-0.5f,	-0.5f,	-0.5f,
	 0.5f,	-0.5f,	-0.5f,
	 0.5f,	-0.5f,	 0.5f,
	 0.5f,	-0.5f,	 0.5f,
	-0.5f,	-0.5f,	 0.5f,
	-0.5f,	-0.5f,	-0.5f,

	-0.5f,	 0.5f,	-0.5f,
	 0.5f,	 0.5f,	-0.5f,
	 0.5f,	 0.5f,	 0.5f,
	 0.5f,	 0.5f,	 0.5f,
	-0.5f,	 0.5f,	 0.5f,
	-0.5f,	 0.5f,	-0.5f,
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

	// create a window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Colors", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::FAILED_TO_CREATE" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window the current context
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// initialize GLAD for function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::FAILED_TO_LOAD" << std::endl;
		glfwTerminate();
		return -1;
	}
#pragma endregion Init

#pragma region VertexBuffers
	unsigned int VAO, lightVAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
#pragma endregion VertexBuffers

#pragma region InterpretArray
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO data is already here
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#pragma endregion InterpretArray

#pragma region ShaderProgram
	Shader objectShader("vertex.vs", "objectFragment.fs");
	Shader lightShader("vertex.vs", "lightFragment.fs");

	objectShader.use();
	objectShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	objectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
#pragma endregion ShaderProgram

#pragma region RenderLoop
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.f);

		glBindVertexArray(VAO);

		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		objectShader.use();

		objectShader.setMat4("model", 1, model);
		objectShader.setMat4("view", 1, view);
		objectShader.setMat4("projection", 1, projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(lightVAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader.use();

		lightShader.setMat4("model", 1, model);
		lightShader.setMat4("view", 1, view);
		lightShader.setMat4("projection", 1, projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
#pragma endregion RenderLoop

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	screenWidth = (float)width;
	screenHeight = (float)height;
	glViewport(0, 0, width, height);
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
	float yOffset = lastY - yPos; // reversed, y ranges bottom to top

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
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