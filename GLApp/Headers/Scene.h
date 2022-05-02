#pragma once

#include <vector>
#include <memory>
#include <map>

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"

class GLScene
{
public:

	GLScene() = default;
	~GLScene() = default;

	std::vector<std::shared_ptr<Model>>& GetAllModelsFromScene();

	void AddModelToScene(std::shared_ptr<Model> model);

	void RemoveModelFromScene(std::shared_ptr<Model> model);

	void AddLightToScene(std::shared_ptr<Light> light);

	std::vector<std::shared_ptr<Light>> GetAllLightsFromScene();

private:
	
	std::vector<std::shared_ptr<Model>> m_Models;
	std::vector<std::shared_ptr<Light>> m_Lights;
};