#include "glad/glad.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
    public:
        Texture(const std::string &path);

        void Bind(unsigned int slot = 0);


    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char *m_LocalBuffer;
        int m_Width, m_Height, m_BPP;
};

Texture::Texture(const std::string &path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(NULL), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    // glBindTexture(GL_TEXTURE_2D, 0);

    if(m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}



void Texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
