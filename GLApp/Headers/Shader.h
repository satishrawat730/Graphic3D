#pragma once

#include <string>
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

class Shader
{
public:

	Shader(std::string vertexShader, std::string fragmentShader);

	bool GetCompilationErrors(int shaderId);

	bool GetLinkingErrors(int shaderId);

	void UseShader();

	void SetValMat4(std::string variableName, glm::mat4 value);

	void SetValVec4(std::string variableName, glm::vec4 value);

	void SetValVec3(std::string variableName, glm::vec3 value);

	void SetValInt(std::string variableName, int value);

	void SetValFloat(std::string variableName, float value);

	unsigned int GetID();

	void BuildShader();

private:

	unsigned int ID;
	std::string m_VertexShader;
	std::string m_FragmentShader;
};
