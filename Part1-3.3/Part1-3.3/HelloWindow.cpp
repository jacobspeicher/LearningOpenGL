#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// callback to modify the OpenGL rendering area when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

	// render loop will continue runing until the user tells it to stop
	// checks if GLFW has been instructed to close
	while (!glfwWindowShouldClose(window)) {
		// swaps the color buffer to show output to the screen
		glfwSwapBuffers(window);
		// checks if any events are triggered (keyboard/mouse) and updates window state, calls appropriate callback functions
		glfwPollEvents();
	}

	// clean/delete all of GLFW's resources that were allocated
	glfwTerminate();

	return 0;
}