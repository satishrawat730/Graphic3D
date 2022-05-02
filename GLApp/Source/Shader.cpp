#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
	m_VertexShader = vertexShader;
	m_FragmentShader = fragmentShader;
}

void Shader::BuildShader()
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(m_VertexShader);
		fragmentShaderFile.open(m_FragmentShader);

		std::stringstream vsStream, fsStream;

		vsStream << vertexShaderFile.rdbuf();
		fsStream << fragmentShaderFile.rdbuf();

		vertexShaderCode = vsStream.str();
		fragmentShaderCode = fsStream.str();
	}
	catch (const std::exception& ex)
	{
		std::cout << "ERROR: Exception Occured while reading Shader code" << ex.what();
	}

	const char* vShaderCode = vertexShaderCode.c_str();

	const char* fShaderCode = fragmentShaderCode.c_str();

	unsigned int vShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShaderId, 1, &vShaderCode, nullptr);
	glCompileShader(vShaderId);
	GetCompilationErrors(vShaderId);

	unsigned int fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderId, 1, &fShaderCode, nullptr);
	glCompileShader(fShaderId);
	GetCompilationErrors(fShaderId);

	ID = glCreateProgram();
	glAttachShader(ID, vShaderId);
	glAttachShader(ID, fShaderId);
	glLinkProgram(ID);
	GetLinkingErrors(ID);
}

bool Shader::GetCompilationErrors(int shaderID)
{
	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cout << "ERROR: Compilation of Shader Failed with Error: " << infoLog;
		return false;
	}

	return true;
}

bool Shader::GetLinkingErrors(int shaderID)
{
	int success;
	glGetShaderiv(shaderID, GL_LINK_STATUS, &success);
	
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cout << "ERROR: Linking of Shader failed with Error: " << infoLog;
		return false;
	}

	return true;
}

void Shader::UseShader()
{
	glUseProgram(ID);
}

void Shader::SetValMat4(std::string variableName, glm::mat4 value)
{
	UseShader();
	int locn = glGetUniformLocation(ID, variableName.c_str());
	glUniformMatrix4fv(locn, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetValVec4(std::string variableName, glm::vec4 value)
{
	UseShader();
	int locn = glGetUniformLocation(ID, variableName.c_str());
	glUniform4fv(locn, 1, glm::value_ptr(value));
}

void Shader::SetValVec3(std::string variableName, glm::vec3 value)
{
	UseShader();
	int locn = glGetUniformLocation(ID, variableName.c_str());
	glUniform3fv(locn, 1, glm::value_ptr(value));
	//glUniform3f(locn, value.x, value.y, value.z);
}

void Shader::SetValInt(std::string variableName, int value)
{
	UseShader();
	int locn = glGetUniformLocation(ID, variableName.c_str());
	glUniform1i(locn, value);
}

void Shader::SetValFloat(std::string variableName, float value)
{
	UseShader();
	int locn = glGetUniformLocation(ID, variableName.c_str());
	glUniform1f(locn, value);
}

unsigned int Shader::GetID()
{
	return ID;
}