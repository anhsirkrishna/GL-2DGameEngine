#pragma once
//Encapsulates the shader program to a single object with easy to use methods

class ShaderProgram {
public:
	unsigned int program_id;

	ShaderProgram();

	//Basic methods
	void AddShader(const char* filename, const unsigned int shader_type);
	void LinkProgram();
	void Use();
	void Unuse();
};
