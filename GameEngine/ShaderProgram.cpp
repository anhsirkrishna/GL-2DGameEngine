#include <fstream>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include "ShaderProgram.h"

/*
* Reads a file which contains a GLSL program
* and returns it as a c-style string
* Returns: const c-style string
*/
const char* ReadProgram(const char* filename) {
    std::ifstream f;
    f.open(filename, std::ios_base::binary);

    //Get size of the file
    f.seekg(0, std::ios_base::end);
    int length = f.tellg();

    //Create buffer with same size as file
    char* content = new char[length + 1];

    //Read the entire file into the buffer
    f.seekg(0, std::ios_base::beg);
    f.read(content, length);
    f.close();

    //End the string with null char
    content[length] = char(0);
    return content;
}

/*
* Log the info for a shader or program
* Returns: void
*/
void LogInfo(GLuint id) {
    //id can be shader_id or program_id
    GLint infoLogLength = 0;

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

    //Allocate a buffer for the info log string
    GLchar* infoLog = new GLchar[infoLogLength];
    glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);

    if (infoLogLength > 0) {
        SDL_Log("%s\n", infoLog);
    }

    delete[] infoLog;
}

ShaderProgram::ShaderProgram() {
    program_id = glCreateProgram();
}

void ShaderProgram::Use() {
    glUseProgram(program_id);
}

void ShaderProgram::Unuse() {
    glUseProgram(0);
}

/*
* Call this once for each shader that needs to be added.
* Usually called twice, once for vertex and once for fragment shader.
* Returns: void
*/
void ShaderProgram::AddShader(const char* filename, const unsigned int shader_type) {
    //Read the shader program
    const char* temp_buffer = ReadProgram(filename);
    const GLchar* shaderSource[] = { temp_buffer };

    //Create and attach the shader to the program
    GLuint shaderId = glCreateShader(shader_type);
    glAttachShader(program_id, shaderId);

    //Read the shader from the source and compile it.
    glShaderSource(shaderId, 1, shaderSource, NULL);
    glCompileShader(shaderId);

    //Delete the char buffer allocated for the shader program since it's already been loaded into H/W
    delete temp_buffer;

    //Check if compilation was succesfull
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != GL_TRUE) {
        SDL_Log("Shader %d compilation failed.\n", shaderId);
        LogInfo(shaderId);
    }
}

/*
* Link the shader program after all shaders have been attached 
*/
void ShaderProgram::LinkProgram() {
    glLinkProgram(program_id);

    //Check if program was linked correctly
    GLint programLinked = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &programLinked);

    //If linking failed, log the reason.
    if (programLinked != GL_TRUE) {
        SDL_Log("Program %d linking failed.\n", program_id);
        LogInfo(program_id);
    }
}