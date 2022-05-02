#pragma once

#include "Mesh.h"

#include "ElementFactory.h"
#include "Material.h"
#include "Shader.h"

#include "glm/mat4x4.hpp"

class GridSystem
{
public:
	GridSystem(int rows, int cols, float scale = 0.01);
	~GridSystem();

	int GetRows();
	void SetRows(int rows);

	int GetCols();
	void SetCols(int cols);

	void DrawGrid(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void CreateGrid();

	double GetScale();
	void SetScale(double scale);

	std::shared_ptr<Mesh> CreateMesh(std::vector<GLfloat> vertices, std::vector<int> indices);

private:

	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;

	std::shared_ptr<Mesh> m_XAxis;	
	std::shared_ptr<Mesh> m_YAxis;
	std::shared_ptr<Mesh> m_ZAxis;

	int m_Row;
	int m_Cols;
	
	double m_Scale;

protected:
};