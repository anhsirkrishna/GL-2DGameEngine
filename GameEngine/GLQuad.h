#pragma once


#include <vector>

#include <GL\glew.h>
#include "Component.h"

//Forward Declaration
class Texture;
class Transform;

/*
* A GLQuad Class.
* The smallest unit for graphics operations of the Engine.
* Represents a quad which will be drawn to the OpenGL window
* The coordinates are represented in pixel space
* The top left corner being (0,0)
* Can have a color and a texture. 
* What is being drawn will depend on the mode.
*/

class GLQuad : public Component {
public:
	//Default ctor to create a GLQuad
	GLQuad();
	
	//Returns a pointer to the associated texture.
	Texture* GetTexture();

	//Sets a texture as the currently associated texture.
	void SetTexture(unsigned int index);

	//Adds a texture
	void AddTexture(Texture* _p_texture);
	
	//Draw the quad by using the OpenGL Draw call
	void Draw(ShaderProgram* program);

	//Sets the texture offset to draw a different section from the same texture.
	void SetTexOffset(GLfloat tex_offset_x, GLfloat tex_offset_y);
	
	//Function to Link with other dependant components like the Transform component
	virtual void Link();

	//This method should ONLY be used for first commit in the minimum viable engine
	//Replace it with Serialize() ASAP
	void CreateDemo();
	//void Serialize(json json_object);
private:
	Texture* p_texture;
	Texture* p_texure_list[5];
	unsigned int texure_list_size;
	GLuint vao_id = 0;
	Transform* p_owner_transform;
	GLfloat tex_offset[2];
};