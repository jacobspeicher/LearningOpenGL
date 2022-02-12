#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// Shader ID
	unsigned int ID;

	// constructor - will read and build the shader program
	Shader(const char* vertexPath, const char* fragmentPath);
	// activate the Shader
	void use();

	// uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, int count, glm::mat4 value) const;
};

#endif