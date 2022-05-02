#pragma once

#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "glm/vec3.hpp"

class Model
{
public:
	Model();
	Model(std::string name);
	~Model();

	void AddMeshToScene(std::shared_ptr<Mesh> mesh);

	bool RemoveMeshFromScene(std::shared_ptr<Mesh> mesh);

	std::vector<std::shared_ptr<Mesh>>& GetAllMeshesFromScene();

	void AddMtlToScene(std::shared_ptr<Material> mtl);

	bool RemoveMtlFromScene(std::shared_ptr<Material> mtl);

	std::vector<std::shared_ptr<Material>>& GetAllMtlsFromScene();

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);

	glm::vec3 GetScale();
	void SetScale(glm::vec3 scale);

	std::string GetName() { return m_Name; }
	void SetName(std::string n) { m_Name = n; }

private:
	std::string m_Name;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	std::vector<std::shared_ptr<Material>> m_Materials;
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
};