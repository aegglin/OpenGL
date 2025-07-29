#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	int rendererID;
	std::string filePath;
	//caching for uniforms
	std::unordered_map<std::string, int> uniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	bool CompileShader();
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource ParseShader(const std::string& file);

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};