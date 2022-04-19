#include "Texture.h"

#include <istream>
#include <vector>

#include "Counter.h"
#include "Random.h"

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#include "external/stb_image/stb_image.h"
#elif WIN32
#include <GL/glew.h>
#include "stb_image/stb_image.h"
#endif

Texture::Texture(TextureType type, int red, int green, int blue, int fourth)
	:m_RendererID(0),  m_Width(1), m_Height(1), m_BPP(3)
{
	setType(type);
	m_Filepath = "Manual: " + m_Type.first;

	unsigned char m_LocalBuffer[4] = { static_cast<unsigned char>(red), static_cast<unsigned char>(green),
										static_cast<unsigned char>(blue), static_cast<unsigned char>(fourth) };

	if (!m_LocalBuffer) { printf("Warning: unable to manually create texture\n"); }

	glGenTextures(1, &m_RendererID);
	Counter::GetCounter().addTextureID(m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Setting up Texture settings. If we don't we're gonna get a black texture. There are NO sensible defaults.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//sends the texture to the GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	//note we could also leave the last parameter blank,
	//that would allocate the data in the GPU but NOT fill it. so that's still good

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string filepath)
	:m_RendererID(0), m_Filepath(filepath), m_Width(0), m_Height(0), m_BPP(0)
{
	LoadTexture(filepath);
}

Texture::Texture(TextureType type, TextureInterpolation texIntp, int sizex, int sizey)
	: m_RendererID(0), m_Width(sizex), m_Height(sizey), m_BPP(3)
{
	stbi_set_flip_vertically_on_load(true); //it flips our texture vertically. 
	setType(type);

	m_Filepath = "Perlin: " + m_Type.first;

	std::vector<unsigned char> m_LocalBuffer;

	for (int x = 0; x < sizex; x++) {
		for (int y = 0; y < sizey; y++) {
			auto r_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * 255);
			auto g_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * 255);
			auto b_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * 255);
			m_LocalBuffer.push_back(r_bit);
			m_LocalBuffer.push_back(g_bit);
			m_LocalBuffer.push_back(b_bit);
		}
	}

	glGenTextures(1, &m_RendererID);
	Counter::GetCounter().addTextureID(m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Setting up Texture settings. If we don't we're gonna get a black texture. There are NO sensible defaults.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texIntp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texIntp);

	//sends the texture to the GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizex, sizey, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	//note we could also leave the last parameter blank,
	//that would allocate the data in the GPU but NOT fill it. so that's still good

	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::Texture(TextureType type, TextureInterpolation texIntp, int sizex, int sizey, int red, int green, int blue)
{
	setType(type);
	m_Filepath = "Perlin: " + m_Type.first;

	std::vector<unsigned char> m_LocalBuffer;

	for (int x = 0; x < sizex; x++) {
		for (int y = 0; y < sizey; y++) {
			auto r_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * red);
			auto g_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * green);
			auto b_bit = static_cast<unsigned char>(rng::InterpolatedNoise(2, (float)x, (float)y) * blue);
			m_LocalBuffer.push_back(r_bit);
			m_LocalBuffer.push_back(g_bit);
			m_LocalBuffer.push_back(b_bit);
		}
	}

	glGenTextures(1, &m_RendererID);
	Counter::GetCounter().addTextureID(m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Setting up Texture settings. If we don't we're gonna get a black texture. There are NO sensible defaults.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texIntp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texIntp);

	//sends the texture to the GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizex, sizey, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	//note we could also leave the last parameter blank,
	//that would allocate the data in the GPU but NOT fill it. so that's still good

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string filepath, TextureType type)
	: m_RendererID(0), m_Filepath(filepath), m_Width(0), m_Height(0), m_BPP(0)
{
	setType(type);
	m_RendererID = LoadTexture(filepath);
}



void Texture::Bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE1 + slot);     //slot to which we bind the texture.
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	 glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UpdateTexture(float red, float green, float blue, float fourth) const
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	unsigned char m_LocalBuffer[4] = { static_cast<unsigned char>(red), static_cast<unsigned char>(green),
									static_cast<unsigned char>(blue), static_cast<unsigned char>(fourth) };

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::LoadTexture(const std::string filepath)
{
	stbi_set_flip_vertically_on_load(true); //it flips our texture vertically. 
	//WE need to to this because OpenGL expects our pixels to start at the BOTTOM LEFT, not the TOP LEFT as usual.
	unsigned char* m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	if (!m_LocalBuffer) { printf("Warning: %s FAILED TO FIND \n", filepath.c_str()); }

	GLenum format = GL_RGBA;

	if (m_BPP == 1)
		format = GL_RED;
	else if (m_BPP == 3)
		format = GL_RGB;
	else if (m_BPP == 4)
		format = GL_RGBA;

	glGenTextures(1, &m_RendererID);
	Counter::GetCounter().addTextureID(m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//Setting up Texture settings. If we don't we're gonna get a black texture. There are NO sensible defaults.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//sends the texture to the GPU.
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	//note we could also leave the last parameter blank,
	//that would allocate the data in the GPU but NOT fill it. so that's still good

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer) { stbi_image_free(m_LocalBuffer); }
	return m_RendererID;
}
