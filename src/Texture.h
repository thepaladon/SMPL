#pragma once
#include <string>

enum TextureType
{
	DIFFUSE = 0,
	SPECULAR = 1,
	NORMAL = 2,
};

enum TextureInterpolation
{
	NEAREST = 0x2600,
	LINEAR = 0x2601
	
};

class Texture
{
public:
	Texture(TextureType type, int red, int green, int blue, int fourth);
	Texture(std::string filepath);
	Texture(TextureType type, TextureInterpolation texIntp, int sizex, int sizey);
	Texture(TextureType type, TextureInterpolation texIntp, int sizex, int sizey, int red, int green, int blue );
	Texture(std::string filepath, TextureType type);
	~Texture() = default;

	void Bind(unsigned int slot = 0)const; //in OpenGL we have various slots we can bind textures to. (we can bind more than 1 dogTex at once, depends on the platform.
	void Unbind() const;
	void UpdateTexture(float red, float green, float blue, float fourth) const;

	int LoadTexture(const std::string filepath);

	unsigned getID() const { return m_RendererID; }
	std::string getFilepath() const { return m_Filepath; }
	int getTypeInt() const { return typeOfTexture; }
	int getWidth() const { return m_Width;  }
	int getHeight() const { return m_Height; }
	std::pair<std::string, TextureType> getTypePair() { return m_Type; }

	void setFilepath(std::string path) { m_Filepath = path; }
	void setType(TextureType type)
	{
		switch (type) {
		case DIFFUSE:
			m_Type = { "texture_diffuse", DIFFUSE };
			typeOfTexture = DIFFUSE;
			break;
		case SPECULAR:
			m_Type = { "texture_specular", SPECULAR };
			typeOfTexture = SPECULAR;
			break;
		case NORMAL:
			m_Type = { "texture_normal", NORMAL };
			typeOfTexture = NORMAL;
			break;
		}

		//check for enum validity
		if(type < DIFFUSE || type > NORMAL)
			printf("WARNING: Texture format not recognized \n");
	}

private:
	unsigned int m_RendererID = 0;
	std::string m_Filepath;
	std::pair<std::string, TextureType> m_Type;
	int typeOfTexture = 0;
	int m_Width, m_Height, m_BPP;
};