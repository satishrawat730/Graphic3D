#include "Terrain.h"
#include "stb_image.h"

Terrain::~Terrain()
{
	m_Points.clear();
	m_Indices.clear();
	baseTex.ClearTexture();
	heightTex.ClearTexture();
}

Terrain::Terrain(uint32_t size, bool randomHeight)
{
	CreateTerrain(size, "", "");
	bRandomHeight = randomHeight;
}

Terrain::Terrain(std::string heightmapImage, std::string baseTexImage, shared_ptr<Shader> shader)
{
	CreateTerrain(0, heightmapImage, baseTexImage);
	SetTerrainShader(shader);
}

void Terrain::CreateTerrain(uint32_t size, std::string heightmapImage, std::string baseTexImage)
{
	// set properties here
	heightScale = 15.f;	
	randomHeightsMax = 10.f;
	gridScale = .3f;
	terrainPosition = glm::vec3(0.0, 0.0, 0.0);
	detailTexScale = 50.f;
	//terrainColor = glm::vec3(1.0f, 0.0f, 0.0f);
	//bWireframe = true;

	bTextureAvailable = LoadTerrainTexture(heightmapImage, baseTexImage);
	if (GetHeightMapInfo(heightmapImage) && bTextureAvailable)
	{
		texScale = (float) m_width;
		size = m_width - 1;
		sizeX = sizeY = size;
		bRandomHeight = false;
		totalVertex = sizeX * sizeX * vertexSize;
	}
	else
	{
		size = size < 5 ? 5 : size; // minimum terrain size
		sizeX = sizeY = size + 1; // current terrain shape is square
		totalVertex = sizeX * sizeX * vertexSize;

		bWireframe = true;
		gridScale = 1.0f;
		heightScale = .1f;
		//bRandomHeight = false;
	}
	// restrict not below .1
	texScale = texScale < .1f ? .1f : texScale;	
	m_Points.reserve(totalVertex);

	uint32_t i = 0;
	for (uint32_t x = 0; x < sizeX; x++)
	{
		for (uint32_t y = 0; y < sizeY; y++)
		{
			m_Points.push_back(float (x) );
			if (bRandomHeight)
			{
				float randomHeight = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / randomHeightsMax));
				m_Points.push_back(float(randomHeight));// height
			}
			else
			{
				m_Points.push_back( 0. ); // height from image
			}
			m_Points.push_back(float (y) );

			m_Points.push_back(float (x) / texScale); // basetex coordinate U
			m_Points.push_back(float (y) / texScale); // basetex coordinate V

			m_Points.push_back(float(x) / detailTexScale); // detail tex coordinate U
			m_Points.push_back(float(y) / detailTexScale); // detail tex coordinate V

			m_Points.push_back(0.); // normal
			m_Points.push_back(1.);
			m_Points.push_back(0.);
		}
	}

	// indicies
	GenerateTerrainIndices();
	// intialize graphic
	InitGraphics();
}

bool Terrain::GetHeightMapInfo(std::string heightmapImage)
{
	unsigned char* imgData = stbi_load(heightmapImage.c_str(), &m_width, &m_height, &m_bitDepth, 0);

	if (!imgData)// file open check
	{
		std::cout << "Failed to find the image: " << heightmapImage << std::endl;
		return false;
	}

	auto sz = m_height * m_width;
	heightData.reserve(sz);
	// fill height data
	size_t m = 0;
	for (size_t i = 0; i < m_height; i++)
	{
		for (size_t j = 0; j < m_width; j++)
		{
			heightData.push_back((float)imgData[m++] / 255.0f);
		}
	}

	stbi_image_free(imgData);

	return true;
}

void Terrain::GenerateTerrainIndices()
{
	float slope{ 0. }, height{ 0. };
	unsigned int ix{ 0 }, i1{ 0 }, ix1{ 0 };
	auto sz = (sizeX - 1);
	totalIndex = sz * sz * 6;

	m_Indices.reserve(totalIndex);
	uint32_t m = 0;
	for (uint32_t j = 0; j < sizeY-1; j++)
	{
		uint32_t end = sizeX * (j + 1);
		for (uint32_t i = 0 + (j * sizeX); i < end-1; i++)
		{
			ix = i + sizeX;
			i1 = i + 1;
			ix1 = ix + 1;
			//even face
			{
				m_Indices.push_back(i);
				m_Indices.push_back(ix);
				m_Indices.push_back(i1);
				
				/*height = heightData[ix] + heightData[i1];
				slope = (( height / 2.f) - heightData[i]) * gridScale;
				slopePoints.push_back(slope);*/
			}
			// odd face
			{
				m_Indices.push_back(i1);
				m_Indices.push_back(ix);
				m_Indices.push_back(ix1);

				/*height = heightData[ix] + heightData[ix1];
				slope = ((height / 2.f) - heightData[i1]) * gridScale;
				slopePoints.push_back(slope);*/
			}
		}
	}
}

void Terrain::InitGraphics()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Points.size(), &m_Points[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
	
	for (uint16_t i = 0; i < 4; i++)
	{
		glVertexAttribPointer(i, attributeIndex[i], GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset[i] * sizeof(float)));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition)
{
	SetTranslation();
	SetShaderValues(view, projection, cameraPosition);

	heightTex.UseTexture(GL_TEXTURE0);
	baseTex.UseTexture(GL_TEXTURE1);
	detailTex.UseTexture(GL_TEXTURE2);

	grassTex.UseTexture(GL_TEXTURE3);
	slopeTex.UseTexture(GL_TEXTURE4);
	rockTex.UseTexture(GL_TEXTURE5);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	if(bWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::SetHeightScale(float scale)
{
	heightScale = scale;
}

void Terrain::SetGridScale(float scale)
{
	gridScale = scale;
}

void Terrain::SetTerrainShader(std::shared_ptr<Shader> shader)
{
	terrainShader = shader;
}

void Terrain::SetTranslation()
{
	terrainModel = glm::scale(terrainModel, glm::vec3(1.f));
	terrainModel = glm::translate(terrainModel, terrainPosition);
}

void Terrain::SetShaderValues(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition)
{
	terrainShader->UseShader();
	terrainShader->SetValMat4("model", terrainModel);
	terrainShader->SetValMat4("projection", projection);
	terrainShader->SetValMat4("view", view);
	terrainShader->SetValFloat("heightScale", heightScale);
	terrainShader->SetValFloat("gridScale", gridScale);
	
	terrainShader->SetValInt("applyTexture", int(bTextureAvailable));

	terrainShader->SetValInt("heightTexture", (int)TextureId::HeightID);
	terrainShader->SetValInt("baseTexture", (int)TextureId::BaseTextureID);
	terrainShader->SetValInt("detailTexture", (int)TextureId::DetailTextureID);

	terrainShader->SetValInt("grassTexture", (int)TextureId::GrassTextureID );
	terrainShader->SetValInt("slopeTexture", (int)TextureId::SlopeTextureID );
	terrainShader->SetValInt("rockTexture", (int)TextureId::RockTextureID );

	terrainShader->SetValVec3("eyePosition", cameraPosition);
	terrainShader->SetValVec3("pointLight.position", pointLight->GetPosition());
	terrainShader->SetValVec3("directionalLight.direction", directionalLight->GetDirection());
	directionalLight->UseLight(terrainShader.get());
	pointLight->UseLight(terrainShader.get());
}

void Terrain::SetTerrainPosition(glm::vec3 pos)
{
	terrainPosition = pos;
}

void Terrain::SetLight(std::shared_ptr<DirectionalLight> dirLight, std::shared_ptr<PointLight> pLight)
{
	directionalLight = dirLight;
	pointLight = pLight;
}

void Terrain::SetWireframeMode(bool on_off)
{
	bWireframe = on_off;
}

bool Terrain::LoadTerrainTexture(std::string heightImagePath, std::string baseImagePath)
{
	heightTex = Texture(heightImagePath.c_str());
	if (!heightTex.LoadTexture())
		return false;

	baseTex = Texture(baseImagePath.c_str());
	if (!baseTex.LoadTexture())
		return false;
	
	detailTex = Texture(Constants::TerrainDetailTexFilePath.c_str());
	if (!detailTex.LoadTexture())
		return false;

	grassTex = Texture(Constants::TerrainGrassTexFilePath.c_str());
	if (!grassTex.LoadTexture())
		return false;

	slopeTex = Texture(Constants::TerrainSlopeTexFilePath.c_str());
	if (!slopeTex.LoadTexture())
		return false;

	rockTex = Texture(Constants::TerrainRockTexFilePath.c_str());
	if (!rockTex.LoadTexture())
		return false;

	grassNormalTex = Texture(Constants::TerrainGrassNormalTexFilePath.c_str());
	if (!grassNormalTex.LoadTexture())
		return false;

	slopeNormalTex = Texture(Constants::TerrainSlopeNormalTexFilePath.c_str());
	if (!slopeNormalTex.LoadTexture())
		return false;

	rockNormalTex = Texture(Constants::TerrainRockNormalTexFilePath.c_str());
	if (!rockNormalTex.LoadTexture())
		return false;

	return true;
}
