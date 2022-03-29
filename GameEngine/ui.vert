//Vertex shader for UI elements

#version 430
layout(location=0) in vec3 in_position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_TexCoords;

uniform mat4 ortho_projection;
uniform mat4 translateMatrix;
uniform mat4 scaleMatrix;
uniform mat4 rotateMatrix, preRotateMatrix, postRotateMatrix;

out vec4 ex_Color;
out vec2 ex_TextCoord;

void main() {
	gl_Position = ortho_projection*translateMatrix*
	/*The preRotateMatrix is required to move the 
	 *centre of the quad being drawn, to origin.
	 *All scales and rotates have to be done at origin.
	 *The postRotateMatrix is required to move the quad 
	 *being drawn back to it's position
	*/
	postRotateMatrix*scaleMatrix*rotateMatrix*preRotateMatrix*vec4(in_position, 1.0);
	ex_Color = in_Color;
	ex_TextCoord = in_TexCoords;
}
