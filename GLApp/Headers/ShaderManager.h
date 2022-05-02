#pragma once

#include <memory>
#include <map>
#include <string>

#include "Shader.h"
#include "Common.h"
#include "ElementFactory.h"

//namespace RE::Shader{
//
//
//}
// make sure you follow the order inside the 
// ShaderNames::Shaders vector
enum class ShaderType
{
	ADS, EXPERIMENTAL, NORMAL_MAP, PASS_THROUGH,
	PBR, TERRAIN, SCREEN_SPACE, COLOR_FILTER
};

class ShaderManager
{
	const vector<string> ShadersName = 
	{ "ADS", "Experimental",  "NormalMap", "passthrough", "pbr", "terrainShader", "ScreenSpace",  "colorFilter"
	};

	const vector<string> ShaderFileNames = {
		"ADS.vs", "ADS.fs",
		"Experimental.vs", "Experimental.fs",
		"NormalMap.vs",	"NormalMap.fs",
		"passthrough.vs", "passthrough.fs",
		"pbr.vs", "pbr.fs",
		"TerrainVShader.vs", "TerrainFShader.fs",
		"ScreenSpace.vs", "ScreenSpace.fs",
		"colorFilter.vs", "colorFilter.fs",
	};

public:	
	static shared_ptr<ShaderManager> GetInstance();

	~ShaderManager() = default;
	string GetShaderName(ShaderType shaderType);
	shared_ptr<Shader> GetShader(string shaderName);
	shared_ptr<Shader> GetShader(ShaderType shaderType);
	void Initialize();

private:
	map<string, shared_ptr<Shader>> m_Shaders;
	bool AddShader(string shaderName, shared_ptr<Shader> shader);

protected:
	static shared_ptr<ShaderManager> m_Instance;

	ShaderManager() = default;
};
