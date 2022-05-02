#include "ModelLoader.h"
#include "Common.h"

void ModelLoader::ProcessMesh(unsigned int meshIdx, const aiScene* scene, std::shared_ptr<Model> model)
{
	aiMesh* mesh = scene->mMeshes[meshIdx];
	std::vector<GLfloat> vertices;
	std::vector<int> indices;
	unsigned int VBO, VAO, EBO, instanceVBO;

	if (mesh->mNumVertices > 0)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			if (mesh->HasTextureCoords(0))
			{
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertices.push_back(0.0);
				vertices.push_back(0.0);
			}

			if (mesh->HasNormals() == true)
			{
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);				
			}
			else
			{
				vertices.push_back(0.0);
				vertices.push_back(0.0);
				vertices.push_back(0.0);				
			}
			if (mesh->HasTangentsAndBitangents())
			{
				vertices.push_back(mesh->mTangents[i].x);
				vertices.push_back(mesh->mTangents[i].y);
				vertices.push_back(mesh->mTangents[i].z);
				vertices.push_back(mesh->mBitangents[i].x);
				vertices.push_back(mesh->mBitangents[i].y);
				vertices.push_back(mesh->mBitangents[i].z);				
			}
			else
			{
				vertices.push_back(0.0);
				vertices.push_back(0.0);
				vertices.push_back(0.0);

				vertices.push_back(0.0);
				vertices.push_back(0.0);
				vertices.push_back(0.0);
			}

		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			int numIndices = face.mNumIndices;
			for (int j = 0; j < numIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.at(0)) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.at(0)) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 14, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 14, (void*)(sizeof(vertices.at(0)) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 14, (void*)(sizeof(vertices.at(0)) * 5));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 14, (void*)(sizeof(vertices.at(0)) * 8));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 14, (void*)(sizeof(vertices.at(0)) * 11));
	glEnableVertexAttribArray(4);

	glm::vec3 translations[5];
	translations[0] = glm::vec3(0.0, 0.0, 0.0);
	translations[1] = glm::vec3(-2.0, -2.0, 0.0);
	translations[2] = glm::vec3(-4.0, -4.0, 0.0);
	translations[3] = glm::vec3(4.0, 4.0, 0.0);
	translations[4] = glm::vec3(2.0, 2.0, 0.0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 5, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(5, 1); // tell OpenGL this is an instanced vertex attribute.


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	auto oMesh = ElementFactory::CreateMesh(VAO, VBO, EBO, (unsigned int)indices.size());
	oMesh->SetMaterialIdx(mesh->mMaterialIndex);
	model->AddMeshToScene(oMesh);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<Model> model)
{
	if (node->mNumChildren > 0)
	{
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	if (node->mNumMeshes > 0)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			ProcessMesh(node->mMeshes[i], scene, model);
		}
	}
}

void ModelLoader::LoadMeshes(const aiScene* scene, std::shared_ptr<Model> model)
{
	ProcessNode(scene->mRootNode, scene, model);
}

void ModelLoader::CopyMaterials(const aiScene* scene, std::shared_ptr<Model> model, std::string filePath)
{
	if (!scene->HasMaterials())
		return;

	model->GetAllMtlsFromScene().resize(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* material = scene->mMaterials[i];
		BaseColor baseColor;
		baseColor.R = baseColor.G = baseColor.B = 1.0;
		aiColor3D color(0.f, 0.f, 0.f);
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			baseColor.R = color.r;
			baseColor.G = color.g;
			baseColor.B = color.b;
		}

		auto mtl = ElementFactory::CreateMaterial(baseColor);
		model->GetAllMtlsFromScene()[i] = mtl;
		mtl->SetShaderName(ShaderManager::GetInstance()->GetShaderName(ShaderType::ADS));

#ifdef _DEBUG
		//  print the textures names that you're loading
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_DIFFUSE, %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_SPECULAR))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_SPECULAR %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_AMBIENT))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_AMBIENT %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_EMISSIVE))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_EMISSIVE %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_HEIGHT))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_HEIGHT %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_NORMALS))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_NORMALS %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_SHININESS))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_SHININESS %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_OPACITY))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_OPACITY %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_DISPLACEMENT))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_DISPLACEMENT %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_LIGHTMAP))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_LIGHTMAP %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_REFLECTION))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_REFLECTION, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_REFLECTION %s\n", sPath.c_str());
			}
		}
		if (material->GetTextureCount(aiTextureType_UNKNOWN))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_UNKNOWN, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				printf("aiTextureType_UNKNOWN %s\n", sPath.c_str());
			}
		}
#endif

		aiString path;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				auto indx = sPath.rfind("\\");
				std::string sFileName = sPath.substr(indx + 1);
				indx = filePath.rfind("\\");
				std::string sDir = filePath.substr(0, indx + 1);
				sDir.append(sFileName);
				const char* chPath = sDir.c_str();
				auto diffTexture = ElementFactory::CreateTexture(chPath);
				mtl->SetDiffuseMap(diffTexture);
			}
		}
		if (material->GetTextureCount(aiTextureType_NORMALS))
		{
			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				auto indx = sPath.rfind("\\");
				std::string sFileName = sPath.substr(indx + 1);

				indx = filePath.rfind("\\");
				std::string sDir = filePath.substr(0, indx + 1);
				sDir.append(sFileName);
				const char* chPath = sDir.c_str();
				auto normalMap = ElementFactory::CreateTexture(chPath);

				mtl->SetNormalMap(normalMap);
			}
		}
		if (material->GetTextureCount(aiTextureType_SPECULAR))
		{
			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				auto indx = sPath.rfind("\\");
				std::string sFileName = sPath.substr(indx + 1);

				indx = filePath.rfind("\\");
				std::string sDir = filePath.substr(0, indx + 1);
				sDir.append(sFileName);
				const char* chPath = sDir.c_str();
				auto metallicMap = ElementFactory::CreateTexture(chPath);

				mtl->SetMetallicMap(metallicMap);
			}
		}
		if (material->GetTextureCount(aiTextureType_AMBIENT))
		{
			if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				auto indx = sPath.rfind("\\");
				std::string sFileName = sPath.substr(indx + 1);

				indx = filePath.rfind("\\");
				std::string sDir = filePath.substr(0, indx + 1);
				sDir.append(sFileName);
				const char* chPath = sDir.c_str();
				auto aoMap = ElementFactory::CreateTexture(chPath);

				mtl->SetAmbientMap(aoMap);
			}
		}
		if (material->GetTextureCount(aiTextureType_SHININESS))
		{
			if (material->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS)
			{
				std::string sPath(path.C_Str());
				auto indx = sPath.rfind("\\");
				std::string sFileName = sPath.substr(indx + 1);

				indx = filePath.rfind("\\");
				std::string sDir = filePath.substr(0, indx + 1);
				sDir.append(sFileName);
				const char* chPath = sDir.c_str();
				auto roughnessMap = ElementFactory::CreateTexture(chPath);

				mtl->SetRoughnessMap(roughnessMap);
			}
		}
		else
		{
			const char* chPath = Constants::PlainTexture.c_str();
			auto diffTexture = ElementFactory::CreateTexture(chPath);
			diffTexture->LoadTexture();
			mtl->SetDiffuseMap(diffTexture);
		}
	}
}

std::shared_ptr<Model> ModelLoader::LoadModel(std::string filePath)
{
	std::shared_ptr<Model> model = ElementFactory::CreateModel();
	//pScene->AddModelToScene(model);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	CopyMaterials(scene, model, filePath);
	LoadMeshes(scene, model);
	return model;
}