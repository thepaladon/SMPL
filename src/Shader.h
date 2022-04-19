#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniform3f(const std::string& name, float f0, float f1, float f2);

	void SetUniform4f(const std::string& name, float f1, float v2, float v3, float v4);
	void SetUniform1f(const std::string& name, float i0);
	void SetUniform1i(const std::string& name, int i0);
	void SetUniformMat4f(const std::string& name, glm::mat4& mat);

	unsigned int getID() { return m_RendererID; }

private:
	unsigned int m_RendererID;
	std::string m_Filepath;	
	std::unordered_map<std::string, int> m_UniformLocationCache;

private:
	unsigned CompileShader(unsigned type, const std::string& source);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};
