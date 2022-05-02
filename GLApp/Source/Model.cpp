#include "Model.h"

Model::Model()
{
	m_Position = glm::vec3(0.0, 0.0, 0.0);
	m_Scale = glm::vec3(1.0, 1.0, 1.0);
}

Model::Model(std::string name)
{
	m_Name = name;
	m_Position = glm::vec3(0.0, 0.0, 0.0);
	m_Scale = glm::vec3(1.0, 1.0, 1.0);
}

Model::~Model()
{

}

void Model::AddMeshToScene(std::shared_ptr<Mesh> mesh)
{
	m_Meshes.push_back(mesh);
}

bool Model::RemoveMeshFromScene(std::shared_ptr<Mesh> mesh)
{
	if (m_Meshes.empty() || mesh == nullptr)
		return false;

	std::vector<std::shared_ptr<Mesh>>::iterator itr = m_Meshes.begin();

	while (itr != m_Meshes.end())
	{
		if (*itr == mesh)
		{
			m_Meshes.erase(itr);
			return true;
		}
		++itr;
	}

	return true;
}

std::vector<std::shared_ptr<Mesh>>& Model::GetAllMeshesFromScene()
{
	return m_Meshes;
}

void Model::AddMtlToScene(std::shared_ptr<Material> mtl)
{
	m_Materials.push_back(mtl);
}

bool Model::RemoveMtlFromScene(std::shared_ptr<Material> mtl)
{
	if (m_Materials.empty() || mtl == nullptr)
		return false;

	std::vector<std::shared_ptr<Material>>::iterator itr = m_Materials.begin();

	while (itr != m_Materials.end())
	{
		if (*itr == mtl)
		{
			m_Materials.erase(itr);
			return true;
		}
		++itr;
	}

	return true;
}

std::vector<std::shared_ptr<Material>>& Model::GetAllMtlsFromScene()
{
	return m_Materials;
}

glm::vec3 Model::GetPosition()
{
	return m_Position;
}

void Model::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

glm::vec3 Model::GetScale()
{
	return m_Scale;
}

void Model::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}