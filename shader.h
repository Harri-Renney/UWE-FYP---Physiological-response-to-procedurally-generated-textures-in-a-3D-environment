#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
	unsigned int ID;

	//Constructor reads and builds shaders.
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexSource;
		std::string fragmentSource;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		//Read file's buffer content into stream.
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Close files.
		vShaderFile.close();
		fShaderFile.close();
		//Convert stream to string.
		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();

		//Set source code in char* for opengl c use.
		const char* vShaderCode = vertexSource.c_str();
		const char* fShaderCode = fragmentSource.c_str();

		//Compile shaders.
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		glLinkProgram(this->ID);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	bool use()
	{
		int status;
		glGetProgramiv(ID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			return false;
		glUseProgram(ID);
		return true;
	}

	void updateMatrices(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
	{
		use();
		setMatrix4X4("modelMatrix", modelMatrix);
		setMatrix4X4("viewMatrix", viewMatrix);
		setMatrix4X4("projectionMatrix", projectionMatrix);
	}

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMatrix4X4(const std::string &name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
};

#endif