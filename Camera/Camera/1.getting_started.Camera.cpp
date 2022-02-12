#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"

float vertices[] =
{
	// pos					 // texture coords
	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 0.0f,	 // bottom left
	 0.5f,	-0.5f,	-0.5f,	 1.0f,	 0.0f,	 // bottom right
	 0.5f,	 0.5f,	-0.5f,	 1.0f,	 1.0f,	 // top right
	 0.5f,	 0.5f,	-0.5f,	 1.0f,	 1.0f,	 // top right
	-0.5f,	 0.5f,	-0.5f,	 0.0f,	 1.0f,	 // top left
	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 0.0f,	 // bottom left

	-0.5f,	-0.5f,	 0.5f,	 0.0f,	 0.0f,	 // bottom left
	 0.5f,	-0.5f,	 0.5f,	 1.0f,	 0.0f,	 // bottom right
	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 1.0f,	 // top right
	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 1.0f,	 // top right
	-0.5f,	 0.5f,	 0.5f,	 0.0f,	 1.0f,	 // top left
	-0.5f,	-0.5f,	 0.5f,	 0.0f,	 0.0f,	 // bottom left

	-0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // bottom left
	-0.5f,	 0.5f,	-0.5f,	 1.0f,	 1.0f,	 // bottom right
	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // top right
	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // top right
	-0.5f,	-0.5f,	 0.5f,	 0.0f,	 0.0f,	 // top left
	-0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // bottom left

	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // bottom left
	 0.5f,	 0.5f,	-0.5f,	 1.0f,	 1.0f,	 // bottom right
	 0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // top right
	 0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // top right
	 0.5f,	-0.5f,	 0.5f,	 0.0f,	 0.0f,	 // top left
	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // bottom left

	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // bottom left
	 0.5f,	-0.5f,	-0.5f,	 1.0f,	 1.0f,	 // bottom right
	 0.5f,	-0.5f,	 0.5f,	 1.0f,	 0.0f,	 // top right
	 0.5f,	-0.5f,	 0.5f,	 1.0f,	 0.0f,	 // top right
	-0.5f,	-0.5f,	 0.5f,	 0.0f,	 0.0f,	 // top left
	-0.5f,	-0.5f,	-0.5f,	 0.0f,	 1.0f,	 // bottom left

	-0.5f,	 0.5f,	-0.5f,	 0.0f,	 1.0f,	 // bottom left
	 0.5f,	 0.5f,	-0.5f,	 1.0f,	 1.0f,	 // bottom right
	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // top right
	 0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 // top right
	-0.5f,	 0.5f,	 0.5f,	 0.0f,	 0.0f,	 // top left
	-0.5f,	 0.5f,	-0.5f,	 0.0f,	 1.0f, 	 // bottom left
};

float mixFactor = 0.2f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;	// time of last frame

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;
float fov = 45.0f;

// callback to modify rendering area when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;	// reversed, y ranges bottom to top

	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
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
			mixFactor += 0.1f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		if (mixFactor > 0.0f)
		{
			mixFactor -= 0.1f;
		}
	}

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	// cameraPos.y = 0.0f; - FPS camera, keeps you stuck to the "ground"
}

int main()
{
#pragma region Init
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Camera", NULL, NULL);
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
		std::cout << "ERROR:GLAD::FAILED_TO_LOAD_PROCESS" << std::endl;
		glfwTerminate();
		return -1;
	}
#pragma endregion Init

#pragma region CreateObjects
	// create vertex objects
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the objects
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy previously declared vertex data into buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion CreateObjects
	
#pragma region InterpretArray
	// enable the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// enable the texture coords data
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion InterpretArray

#pragma region LoadTextures
	// flip loaded images upside down so they appear the right way up
	stbi_set_flip_vertically_on_load(true);

	// generate textures
	int width, height, numChannels;
	unsigned char* data;
	unsigned int crateTexture, faceTexture;
	glGenTextures(1, &crateTexture);
	glGenTextures(1, &faceTexture);

	// create crate texture
	glBindTexture(GL_TEXTURE_2D, crateTexture);

	// set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load image data
	data = stbi_load("container.jpg", &width, &height, &numChannels, 0);

	if (data)
	{
		// load image data into bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::CRATE_TEXTURE_FAILED_TO_LOAD" << std::endl;
	}

	// free texture data
	stbi_image_free(data);

	// create face texture
	glBindTexture(GL_TEXTURE_2D, faceTexture);

	// set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load image data
	data = stbi_load("awesomeface.png", &width, &height, &numChannels, 0);

	if (data)
	{
		// load image data into bound texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FACE_TEXTURE_FAILED_TO_LOAD" << std::endl;
	}

	// free texture data
	stbi_image_free(data);
#pragma endregion LoadTextures

#pragma region ShaderProgram
	// create the shader
	Shader shaderProgram("vertex.vs", "fragment.fs");

	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);
#pragma endregion ShaderProgram

#pragma region Camera
	// Gram-Schmidt Process to get all vectors for the camera position
	
	// position the camera (backwards is along the positive z-axis)
	// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	// calculate the camera's direction (direction doesn't change with vector length)
	// subtract the target from the camera pos so that we get the negated direction (camera's pos z-axis is our negative z-axis)
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	// get the camera's positive x-axis vector
	// cross product between up vector and camera direction gives us a perpendicular vector facing in the positive x-axis direction relative to the camera
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	// get the camera's position y-axis vector
	// cross product between the camera's z-axis and x-axis will give us a perpendicular vector facing in the positive y-axis direction relative to the camera
	// glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

#pragma endregion Camera

#pragma region 3DMatrices
	/*
	// create the view matrix (camera/viewers POV)
	glm::mat4 view = glm::mat4(1.0f);
	// create a LookAt matrix using the vectors determined by the camera above. the LookAt matrix will be used to transform
	// world coordinates to the defined view space
	// [ Rx, Ry, Rz, 0 ] x [ 1, 0, 0, -Px ]
	// [ Ux, Uy, Uz, 0 ]   [ 0, 1, 0, -Py ]
	// [ Dx, Dy, Dz, 0 ]   [ 0, 0, 1, -Pz ]
	// [  0,  0,  0, 1 ]   [ 0, 0, 0,   1 ]
	// the matrices are transposed and negated respectively since the world needs to be rotated and translated in the opposite direction of where the camera is moving
	// lookAt(cameraPos, cameraTarget, cameraUpVector)
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 1.0f, 0.0f));
	*/

	/*
	// create the projection matrix (gives a lil perspective)
	glm::mat4 projection;
	// (FOV, aspect ratio, near plane, far plane);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.5f, 100.f);
	*/
#pragma endregion 3DMatrices

#pragma region CubePositions
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,	2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),
	};
#pragma endregion CubePositions

#pragma region Rendering
	// enable depth testing to draw the cubes correctly
	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crateTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

		glBindVertexArray(VAO);

		glm::mat4 trans = glm::mat4(1.0f);

		// rotate camera around the scene over time
		/*
		const float radius = 5.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		*/

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		shaderProgram.use();

		shaderProgram.setFloat("mixFactor", mixFactor);
		shaderProgram.setMat4("transform", 1, trans);
		shaderProgram.setMat4("view", 1, view);
		shaderProgram.setMat4("projection", 1, projection);

		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;

			if (i % 3 == 0)
			{
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}

			shaderProgram.setMat4("model", 1, model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
#pragma endregion Rendering

	glfwTerminate();
	return 0;
}