/******************************************************************************/
/*!
/*File   Texture.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Texture class encapsulation implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <SDL_image.h>

#include "Texture.h"
#include "MemoryManager.h"

#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Texture.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }

/*Constructor to create a texture object from a file path.
* Loads an image into an SDL surface, assigned a texture id to it
* and sends it to the graphics card.
* Frees the SDL surface after sending it to the graphics card.
*/
Texture::Texture(const std::string& filename) {
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());

	name = filename;
	width = loadedSurface->w;
	height = loadedSurface->h;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	GLuint Mode = GL_SRGB;
	GLuint format = GL_RGB;

	if (loadedSurface->format->BytesPerPixel == 4) {
		Mode = GL_SRGB_ALPHA;
		format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, format, 
				 GL_UNSIGNED_BYTE, loadedSurface->pixels);
	CHECKERROR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECKERROR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECKERROR;

	SDL_FreeSurface(loadedSurface);
}

// Makes a texture accessible to a program specified by the program_id
// The name of the sampler2D variable in the shader program is specified by name
void Texture::Bind(const int unit, const int program_id, const std::string& name)
{
	glActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
	CHECKERROR;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	CHECKERROR;
	int loc = glGetUniformLocation(program_id, name.c_str());
	CHECKERROR;
	glUniform1i(loc, unit);
	CHECKERROR;
}

//Unbind a texture after use
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}