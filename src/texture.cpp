#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

Texture::Texture(const std::string& path)
{
	create();
	load(path);
}

void Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::create()
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::load(const std::string& path) const
{
	int width{};
	int height{};
	int nrOfChannels{};
	unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrOfChannels,
		STBI_rgb_alpha);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Error loading texture: \n" << path << '\n';
	}
	stbi_image_free(textureData);
}
