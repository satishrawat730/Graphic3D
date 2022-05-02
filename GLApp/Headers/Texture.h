#pragma once

#include "glad/glad.h"

#include "stb_image.h"

class Texture
{
public:

	Texture();
	Texture(char const * fileLoc);
	~Texture();

	bool LoadTexture();
	void UseTexture(GLenum texIdx);
	void ClearTexture();

private:

	GLuint m_textureId;
	int m_width, m_height, m_bitDepth;

	char const* m_fileLocation;
};