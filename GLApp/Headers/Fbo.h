#pragma once
#include "glad/glad.h"
#include "Common.h"
#include "ShaderManager.h"

class Fbo
{
	GLuint m_fb;
	
	GLuint m_colorTexture;
	GLuint m_rbo;
	// the default height set by the application
	GLuint m_NativeWitdh, m_NativeHeight;
	GLuint m_quadVAO, m_quadVBO;
	
	float quadVertices[24] =
	{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

public:
	Fbo();
	Fbo(int w, int h);
	void enableFramebuffer();
	void disableFramebuffer();
	void drawFramebuffer();
	void drawFramebuffer(ShaderType shaderType);
	~Fbo();
};