#include "ElementFactory.h"

std::shared_ptr<Mesh> ElementFactory::CreateMesh(unsigned int VAO, unsigned int VBO, unsigned int EBO, int triangleCount)
{
	return std::shared_ptr<Mesh>(new Mesh(VAO, VBO, EBO, triangleCount));
}

std::shared_ptr<GLScene> ElementFactory::CreateScene()
{
	return std::shared_ptr<GLScene>(new GLScene());
}

std::shared_ptr<Material> ElementFactory::CreateMaterial(BaseColor baseColor)
{
	return std::shared_ptr<Material>(new Material(baseColor, 1.0f, 32.0));
}

std::shared_ptr<Texture> ElementFactory::CreateTexture(const char* path)
{
	auto texture = std::shared_ptr<Texture>(new Texture(path));
	texture->LoadTexture();
	return texture;
}

std::shared_ptr<Material> ElementFactory::CreateMaterial(std::shared_ptr<Texture> texture)
{
	return std::shared_ptr<Material>(new Material(texture));
}

std::shared_ptr<DirectionalLight> ElementFactory::CreateDirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity,
	glm::vec3 direction, GLfloat diffuseIntensity)
{
	return std::shared_ptr<DirectionalLight>(new DirectionalLight(lightColor, ambientIntensity, direction, diffuseIntensity));
}

std::shared_ptr<PointLight> ElementFactory::CreatePointLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 position, GLfloat diffuseIntensity,
											float quadratic, float linear, float constant)
{
	auto pointLight = std::shared_ptr<PointLight>(new PointLight(lightColor, ambientIntensity, position, diffuseIntensity, quadratic, linear, constant));
	pointLight->CreateLightCube(position);
	return pointLight;
}

std::shared_ptr<Model> ElementFactory::CreateModel()
{
	return std::shared_ptr<Model>(new Model());
}

std::shared_ptr<Shader> ElementFactory::CreateShader(std::string vertexShader, std::string fragmentShader)
{
	auto shader = std::shared_ptr<Shader>(new Shader(vertexShader, fragmentShader));
	shader->BuildShader();
	return shader;
}

std::shared_ptr<GridSystem> ElementFactory::CreateGrid(int rows, int cols, double scale)
{
	auto grid = std::shared_ptr<GridSystem>(new GridSystem(rows, cols, (float)scale));
	grid->CreateGrid();
	return grid;
}

std::shared_ptr<Terrain> ElementFactory::CreateTerrain(std::string heightMapPath, std::string baseTexPath, shared_ptr<Shader> shader)
{
	return  std::shared_ptr<Terrain>(new Terrain(heightMapPath, baseTexPath, shader));
}