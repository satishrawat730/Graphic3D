#include "Scene.h"

std::vector<std::shared_ptr<Model>>& GLScene::GetAllModelsFromScene()
{
	return m_Models;
}

void GLScene::AddModelToScene(std::shared_ptr<Model> model)
{
	m_Models.push_back(model);
}

void GLScene::RemoveModelFromScene(std::shared_ptr<Model> model)
{
	auto itr = std::find(m_Models.begin(), m_Models.end(), model);

	if (itr != m_Models.end())
	{
		m_Models.erase(itr);
	}
}

void GLScene::AddLightToScene(std::shared_ptr<Light> light)
{
	m_Lights.push_back(light);
}

std::vector<std::shared_ptr<Light>> GLScene::GetAllLightsFromScene()
{
	return m_Lights;
}