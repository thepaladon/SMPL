#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Counter.h"

#ifdef RASPBERRY
#include <GLES3/gl3.h>
#elif WIN32
#include <GL/glew.h>
#endif


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::string vertFilepath = filepath + ".vert";
    std::string fragFilepath = filepath + ".frag";

	// ifstream opens for read, ofstream for write, fstream for both
    std::ifstream VERTstream(vertFilepath.c_str());
    std::ifstream FRAGstream(fragFilepath.c_str());

    if (!VERTstream.is_open()) 
    { printf("%s : Could not be opened \n", vertFilepath.c_str()); };

	if (!FRAGstream.is_open()) 
    { printf("%s : Could not be opened \n", fragFilepath.c_str()); };

    enum class ShaderType
    {
        None = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;

    while (getline(VERTstream, line))
    {
        type = ShaderType::VERTEX;
        ss[static_cast<int>(type)] << line << '\n';
    }

    while (getline(FRAGstream, line))
    {
        type = ShaderType::FRAGMENT;
        ss[static_cast<int>(type)] << line << '\n';
    }

   // std::string vertShaderString(std::istreambuf_iterator(VERTstream), std::istreambuf_iterator<char>());
  //  std::string fragShaderString(std::istreambuf_iterator(FRAGstream), std::istreambuf_iterator<char>());

    return { ss[0].str(), ss[1].str() };
}


Shader::Shader(const std::string& filepath)
	:m_Filepath(filepath)
{
    ShaderProgramSource source = ParseShader(filepath);

    //For printing out the shaders in the console
	/*std::cout << source.VertexSource << std::endl;
    std::cout << source.FragmentSource << std::endl;*/
     
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(m_RendererID) ;
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID); 
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); //check out what this does in OpenGL
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    Counter::GetCounter().addShaderID(program);
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program); //Read up on these in https://docs.gl

    glDeleteShader(vs); //There is also a "Detach Shader" which deletes the source code
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    glUniform3f(GetUniformLocation(name), f0, f1, f2);
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void Shader::SetUniform1f(const std::string& name, float f0)
{
    glUniform1f(GetUniformLocation(name), f0);
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
    glUniform1i(GetUniformLocation(name), i0);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    //we use the location of the first element through the index and by passing it through index
}


int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) return m_UniformLocationCache[name];
      
    const int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) 
    { std::cout << "Warning: " << name << " doesn't exist in" << m_Filepath << "\n"; }

	m_UniformLocationCache[name] = location;
    return location;
}
