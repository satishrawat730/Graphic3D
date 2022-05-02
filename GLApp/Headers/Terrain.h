#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "glad/glad.h"

#include "Common.h"
#include "Texture.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"

using namespace std;
class Terrain
{
	enum class TextureId { HeightID, BaseTextureID, DetailTextureID, GrassTextureID, SlopeTextureID, RockTextureID};

public:
	Terrain() = delete;
	~Terrain();

	Terrain(std::string heightmapImage, std::string baseTexImage, shared_ptr<Shader> shader);
	Terrain(uint32_t terrainSize, bool randomHeight = true);

	void CreateTerrain(uint32_t size, std::string heightmapImage, std::string baseTexImage);
	bool GetHeightMapInfo(std::string heightmapImage);

	void Render(glm::mat4 vw, glm::mat4 proj, glm::vec3 pos);
	void SetHeightScale(float scale);
	void SetGridScale(float scale);
	void SetTerrainShader(std::shared_ptr<Shader> shader);
	void SetTranslation();
	void SetShaderValues(glm::mat4 vw, glm::mat4 proj, glm::vec3 pos);
	void SetTerrainPosition(glm::vec3 pos);
	void SetLight(std::shared_ptr<DirectionalLight> dirLight, std::shared_ptr<PointLight> pointLight);
	void SetWireframeMode(bool on_off);

private:

	void InitGraphics();
	void GenerateTerrainIndices();
	bool LoadTerrainTexture(string heightImagePath, string baseImagePath );

	Texture baseTex;
	Texture heightTex;
	Texture detailTex;
	//for slope
	Texture grassTex;
	Texture slopeTex;
	Texture rockTex;
	Texture grassNormalTex;
	Texture slopeNormalTex;
	Texture rockNormalTex;

	std::vector< float > m_Points; // terrain vertices 
	std::vector< uint32_t > m_Indices;// terrain indices
	std::vector< float > m_SlopePoints;

	uint32_t offset[4] { 0, 3, 5, 7 };
	uint32_t attributeIndex[4] { 3, 2, 2, 3 };
	uint32_t vertexSize{ 10 };

	uint32_t VAO{ 0 };
	uint32_t VBO{ 0 };
	uint32_t EBO{ 0 };
	uint32_t sizeX{ 1 };
	uint32_t sizeY{ 1 };
	uint32_t totalVertex{ 0 };
	uint32_t totalIndex{ 0 };

	std::vector<float> heightData;

	bool bWireframe{ false };
	bool bTextureAvailable{ false };
	bool bRandomHeight{ true };
	// height map properties
	int m_width{ 0 },
		m_height{ 0 },
		m_bitDepth{ 0 };

	float heightScale{ 1. }; // scale between height points
	float randomHeightsMax{ 1. }; // max limit for random height generated
	float texScale{ 1. }; // base texture scaling
	float detailTexScale{ 1. }; // base texture scaling
	float gridScale{ 1. }; // XZ scaling for terrain grid 

	glm::vec3 terrainPosition{ 1. }; // terrain position in 3D World
	glm::vec3 terrainColor{ 1. };  // terrain color if no texture // Not used
	glm::mat4 terrainModel{ 1.0 }; // for scaling

	shared_ptr<Shader> terrainShader{ nullptr }; // shader using
	shared_ptr<DirectionalLight> directionalLight { nullptr };
	shared_ptr<PointLight> pointLight{ nullptr };
};
