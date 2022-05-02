#include "fbo.h"

Fbo::Fbo()
{
    m_NativeWitdh = 0;
    m_NativeHeight = 0;
    m_fb = 0;
}

Fbo::Fbo(int w, int h)
{
    m_NativeWitdh = w; 
    m_NativeHeight = h;

    glGenFramebuffers(1, &m_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fb);

    // create a color attachment texture
    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_NativeWitdh, m_NativeHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_NativeWitdh, m_NativeHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo); // now actually attach it
    
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
     //   printf("unable to create framebuffer, disabling post processing\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // create the quad onto which the fbo will be rendered.
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Fbo::enableFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
    glEnable(GL_DEPTH_TEST); 
}


void Fbo::disableFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); 
}

void Fbo::drawFramebuffer()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    auto activeShader = ShaderManager::GetInstance()->GetShader(ShaderType::SCREEN_SPACE);
    activeShader->UseShader();
    glBindVertexArray(m_quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    activeShader->SetValInt("screenTexture", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Fbo::drawFramebuffer(ShaderType shaderType)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    auto activeShader = ShaderManager::GetInstance()->GetShader(shaderType);
    activeShader->UseShader();
    glBindVertexArray(m_quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    activeShader->SetValInt("screenTexture", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Fbo::~Fbo()
{}
