#pragma once
#ifndef H_TEXTURE
#define H_TEXTURE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "context.h"

class Texture
{
public:
	Texture(const char* name): fstream_(name), src_(nullptr), ID_(0u), width_(0u), height_(0u)
	{
		stbi_set_flip_vertically_on_load(true);
		glGenTextures(1, &ID_);
		glBindTexture(GL_TEXTURE_2D, ID_);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// load and generate the texture
		int nrChannels;
		unsigned char *data = stbi_load(name, &width_, &height_, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);
	}

	~Texture() {glDeleteTextures(1, &ID_);};
	
	void use() const
	{
		glBindTexture(GL_TEXTURE_2D, ID_);
	}

	const GLuint GetTextureID() const { return ID_; };
	
private:
	FStream fstream_; //TODO throw away stb_image.h
	char* src_;
	GLuint ID_;
	int width_;
	int height_;
};

#endif