
#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"



Shader::Shader(const std::string& filePath)
	: filePath(filePath), rendererID(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(rendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader(const std::string& file)
{
	std::ifstream stream(file);
	std::string line;

	std::stringstream stringStreams[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			stringStreams[(int)type] << line << '\n';
		}
	}

	return { stringStreams[0].str(), stringStreams[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0]
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// Query shader that is compiled to see if anything is wrong
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // i means int, v means vector
	if (result == GL_FALSE)
	{
		// Get the length of the error message
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//alloca is a C function that lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

/* Takes in actual source code for these */
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Documentation is available on these
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// delete intermediate (obj files) since they've been linked
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{

	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
	else
		uniformLocationCache[name] = location;
	return location;
}