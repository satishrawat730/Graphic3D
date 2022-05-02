#include <iostream>
#include "Texture.h"

Texture::Texture()
{
	m_textureId = 0;
	m_width = 0;
	m_height = 0;
	m_bitDepth = 0;
	m_fileLocation = nullptr;
}

Texture::~Texture()
{
	ClearTexture();
}

Texture::Texture(char const * fileLoc)
{
	m_textureId = 0;
	m_width = 0;
	m_height = 0;
	m_bitDepth = 0;
	m_fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);

	if (!texData)
	{
		std::cout << "Failed to find the texture: " << m_fileLocation << std::endl;
		return false;
	}
	GLenum format;
	if (m_bitDepth == 1)
		format = GL_RED;
	else if (m_bitDepth == 3)
		format = GL_RGB;
	else if (m_bitDepth == 4)
		format = GL_RGBA;

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, texData);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture(GLenum texIdx)
{
	glActiveTexture(texIdx);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &m_textureId);

	m_textureId = 0;
	m_width = 0;
	m_height = 0;
	m_bitDepth = 0;
	m_fileLocation = nullptr;
}