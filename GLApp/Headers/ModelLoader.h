#pragma once

#include <memory>
#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Scene.h"
#include "ElementFactory.h"
#include "ShaderManager.h"

class ModelLoader
{
public:

	static void ProcessMesh(unsigned int meshIdx, const aiScene* scene, std::shared_ptr<Model> model);

	static void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Model> model);

	static void LoadMeshes(const aiScene* scene, std::shared_ptr<Model> model);

	static void CopyMaterials(const aiScene* scene, std::shared_ptr<Model> model, std::string filePath);

	static std::shared_ptr<Model> LoadModel(std::string filePath);

};