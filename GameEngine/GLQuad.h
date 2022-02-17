#pragma once


#include <vector>
#include <GL\glew.h>
#include <glm.hpp>
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
private:
	Texture* p_texture;
	std::vector<Texture*> p_texure_list;
	GLuint vao_id = 0;
	Transform* p_owner_transform;
	GLfloat tex_offset[2];
	unsigned int texture_mode;
	unsigned int vertex_count;
	glm::vec2 dimensions;

public:

	//Default ctor to create a GLQuad
	GLQuad();
	
	//Returns a pointer to the associated texture.
	Texture* GetTexture();

	//Sets a texture as the currently associated texture.
	void SetTexture(unsigned int index);

	//Adds a texture
	void AddTexture(Texture* _p_texture);

	//Clear all textures
	void ClearTextures();
	
	//Draw the quad by using the OpenGL Draw call
	void Draw(ShaderProgram* program);

	//Sets the texture offset to draw a different section from the same texture.
	void SetTexOffset(GLfloat tex_offset_x, GLfloat tex_offset_y);
	
	//Function to Link with other dependant components like the Transform component
	virtual void Link();

	//Sets the render mode to color (int=0) or texture (int=1)
	void SetTextureMode(int mode_);

	/*Serialize the GLquad object from a json input
	* Expects a json dict with the following keys:
	* texture_names : list of texture file names present in the resources folder
	* vertex_list: A 1D list of 3*4 floats. Usually 4 Vertices.
				   Each group of 3 floats represents 1 vertexs x, y,and z values.
	* color_list: A 1D list of 4*4 floats. Each group of 4 floats
				  represents 1 vertexs r, g, b, and a color values.
	* texture_list: A 1D list of 2*4 floats. Each group of 2 floats represents
					the texture coordinates for each vertex.
	* Returns : void
	*/
	void Serialize(json json_object);

	/*Change the state of the GLQuad component
	* Involves changing the texture that this quad is pointing to.
	* Returns : void
	*/
	virtual void ChangeState(json json_object);

	//Get the quad width and height
	glm::vec2 GetDimensions();
};