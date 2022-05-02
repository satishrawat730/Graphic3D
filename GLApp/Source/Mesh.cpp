#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	
}

Mesh::Mesh(unsigned int VAO, unsigned int VBO, unsigned int EBO, int triangleCount)
{
	m_VAO = VAO;
	m_VBO = VBO;
	m_EBO = EBO;
	m_TriangleCount = triangleCount;
}

unsigned int Mesh::GetVAO()
{
	return m_VAO;
}

void Mesh::SetVAO(unsigned int VAO)
{
	m_VAO = VAO;
}

unsigned int Mesh::GetVBO()
{
	return m_VBO;
}

void Mesh::SetVBO(unsigned int VBO)
{
	m_VBO = VBO;
}

unsigned int Mesh::GetEBO()
{
	return m_EBO;
}

void Mesh::SetEBO(unsigned int EBO)
{
	m_EBO = EBO;
}

int Mesh::GetTriangleCount()
{
	return m_TriangleCount;
}

void Mesh::SetTriangleCount(int triangleCount)
{
	m_TriangleCount = triangleCount;
}

int Mesh::GetMaterialIdx()
{
	return m_MaterialIndex;
}

void Mesh::SetMaterialIdx(int idx)
{
	m_MaterialIndex = idx;
}
