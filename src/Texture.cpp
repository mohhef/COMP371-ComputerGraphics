#include "Texture.h"
#include "stb_image.h"
#include "Renderer.h"

Texture::Texture(const std::string& path)
	:filePath(path), localBuffer(nullptr), width(0), height(0), nrChannels(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	GLCall(glGenTextures(1, &id));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;


	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (localBuffer) {
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &id));
}
void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}