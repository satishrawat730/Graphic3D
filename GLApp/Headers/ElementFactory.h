#pragma once

#include <memory>
#include <string>


#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Shader.h"
#include "Terrain.h"
#include "Grid.h"

class GridSystem;

class ElementFactory
{
public:

	static std::shared_ptr<Mesh> CreateMesh(unsigned int VAO, unsigned int VBO, unsigned int EBO, int triangleCount);

	static std::shared_ptr<GLScene> CreateScene();	
	
	static std::shared_ptr<Material> CreateMaterial(BaseColor baseColor);

	static std::shared_ptr<Material> CreateMaterial(std::shared_ptr<Texture> texture);

	static std::shared_ptr<Texture> CreateTexture(const char* path);

	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity, 
		glm::vec3 direction, GLfloat diffuseIntensity);

	static std::shared_ptr<PointLight> CreatePointLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 position, GLfloat diffuseIntensity,
														float quadratic, float linear, float constant);

	static std::shared_ptr<Model> CreateModel();

	static std::shared_ptr<Shader> CreateShader(std::string vertexShader, std::string fragmentShader);

	static std::shared_ptr<GridSystem> CreateGrid(int rows, int cols, double scale = 1.0);
	
	static std::shared_ptr<Terrain> CreateTerrain(std::string heightMapPath, std::string baseTexPath, shared_ptr<Shader> shader);
};
