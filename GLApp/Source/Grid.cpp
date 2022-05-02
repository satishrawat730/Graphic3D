#include "Grid.h"
#include "Common.h"
#include "ShaderManager.h"

GridSystem::GridSystem(int rows, int cols, float scale)
{
	m_Row = rows;
	m_Cols = cols;
	m_Scale = scale;
}

GridSystem::~GridSystem()
{
	m_Row = -1;
	m_Cols = -1;
}

int GridSystem::GetRows()
{
	return m_Row;
}

void GridSystem::SetRows(int rows)
{
	m_Row = rows;
}

int GridSystem::GetCols()
{
	return m_Cols;
}

void GridSystem::SetCols(int cols)
{
	m_Cols = cols;
}

void GridSystem::DrawGrid(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	auto activeShader = ShaderManager::GetInstance()->GetShader(m_Material->GetShaderName());
	activeShader->UseShader();
	activeShader->SetValMat4("model", model);
    activeShader->SetValMat4("projection", projection);
	activeShader->SetValMat4("view", view);
	auto baseColor = m_Material->GetBaseColor();
	activeShader->SetValVec4("inputColor", glm::vec4(baseColor.R, baseColor.G, baseColor.B, 1.0));	
	glBindVertexArray(m_Mesh->GetVAO());		
	glDrawElements(GL_LINES, m_Mesh->GetTriangleCount(), GL_UNSIGNED_INT, NULL);
	
	glDisable(GL_DEPTH_TEST);
	activeShader->SetValVec4("inputColor", glm::vec4(1.0, 0.0, 0.0, 1.0));
	glBindVertexArray(m_XAxis->GetVAO());
	glDrawElements(GL_LINES, m_XAxis->GetTriangleCount(), GL_UNSIGNED_INT, NULL);

	activeShader->SetValVec4("inputColor", glm::vec4(0.0, 1.0, 0.0, 1.0));
	glBindVertexArray(m_YAxis->GetVAO());
	glDrawElements(GL_LINES, m_YAxis->GetTriangleCount(), GL_UNSIGNED_INT, NULL);

	activeShader->SetValVec4("inputColor", glm::vec4(0.0, 0.0, 1.0, 1.0));
	glBindVertexArray(m_ZAxis->GetVAO());
	glDrawElements(GL_LINES, m_ZAxis->GetTriangleCount(), GL_UNSIGNED_INT, NULL);
	glEnable(GL_DEPTH_TEST);
}

std::shared_ptr<Mesh> GridSystem::CreateMesh(std::vector<GLfloat> vertices, std::vector<int> indices)
{
	unsigned int VAO, EBO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.at(0)) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.at(0)) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.at(0)) * 3, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	auto mesh = ElementFactory::CreateMesh(VAO, VBO, EBO, (unsigned int)indices.size());
	mesh->SetTriangleCount((int)indices.size());

	return mesh;
}

void GridSystem::CreateGrid()
{
	std::vector<GLfloat> vertices;
	std::vector<int> indices;

	double x = - (GetCols() / 2);
	double z = - (GetRows() / 2);
	double y = 0.0;

	for (int i = 0; i < GetRows(); ++i)
	{
		for (int j = 0; j < GetCols(); ++j)
		{
			vertices.push_back((GLfloat)(x * m_Scale));
			vertices.push_back((GLfloat)(y * m_Scale));
			vertices.push_back((GLfloat)(z * m_Scale));

			vertices.push_back((GLfloat)(x * m_Scale));
			vertices.push_back((GLfloat)(y * m_Scale));
			vertices.push_back((GLfloat)((z + 1) * m_Scale));

			vertices.push_back((GLfloat)((x + 1) * m_Scale));
			vertices.push_back((GLfloat)(y * m_Scale));
			vertices.push_back((GLfloat)((z + 1) * m_Scale));

			vertices.push_back((GLfloat)((x + 1) * m_Scale));
			vertices.push_back((GLfloat)(y * m_Scale));
			vertices.push_back((GLfloat)(z * m_Scale));

			++x;
			int indx = (int)indices.size() / 2;

			indices.push_back(indx);
			indices.push_back(indx + 1);

			indices.push_back(indx + 1);
			indices.push_back(indx + 2);

			indices.push_back(indx + 2);
			indices.push_back(indx + 3);

			indices.push_back(indx + 3);
			indices.push_back(indx);
		}

		++z;
		x = -(GetCols() / 2);
	}		
	
	m_Mesh = CreateMesh(vertices, indices);
	BaseColor color;
	color.R = 0.1f;
	color.G = 0.1f;
	color.B = 0.1f;
	m_Material = ElementFactory::CreateMaterial(color);
	m_Material->SetShaderName(ShaderManager::GetInstance()->GetShaderName(ShaderType::PBR));

	std::vector<GLfloat> axisVertices = {
											0.0, 0.0, 0.0,
											(float)m_Scale, 0.0, 0.0,
										};

	std::vector<int> axisIndices = { 0, 1};	
	
	m_XAxis = CreateMesh(axisVertices, axisIndices);

	axisVertices =	{ 
						0.0, 0.0, 0.0,
						0.0, (float)m_Scale, 0.0,
					};

	m_YAxis = CreateMesh(axisVertices, axisIndices);

	axisVertices = {
						0.0, 0.0, 0.0,
						0.0, 0.0, (float)m_Scale,
	};
	m_ZAxis = CreateMesh(axisVertices, axisIndices);
}

double GridSystem::GetScale()
{
	return m_Scale;
}

void GridSystem::SetScale(double scale)
{
	m_Scale = scale;
}