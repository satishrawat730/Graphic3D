#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();

	Mesh(unsigned int VAO, unsigned int VBO, unsigned int EBO, int triangleCount);

	unsigned int GetVAO();
	void SetVAO(unsigned int VAO);

	unsigned int GetVBO();
	void SetVBO(unsigned int VBO);

	unsigned int GetEBO();
	void SetEBO(unsigned int EBO);

	int GetTriangleCount();
	void SetTriangleCount(int triangleCount);

	int GetMaterialIdx();
	void SetMaterialIdx(int idx);

private:

	unsigned int m_VAO, m_VBO, m_EBO, m_ShaderID;
	int m_TriangleCount, m_MaterialIndex;	

protected:
};