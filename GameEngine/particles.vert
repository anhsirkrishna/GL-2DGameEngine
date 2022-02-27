//Vertex shader

#version 430
layout(location=0) in vec3 in_position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_TexCoords;
layout(location=3) in float in_particle_brightness;

uniform mat4 orthoGraphProj;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 translateMatrix;
uniform mat4 scaleMatrix;
uniform mat4 rotateMatrix, preRotateMatrix, postRotateMatrix;

out vec4 ex_Color;
out vec2 ex_TextCoord;
out float ex_particle_brightness;

void main() {

	gl_Position = projection*view*translateMatrix*
	/*The preRotateMatrix is required to move the 
	 *centre of the quad being drawn, to origin.
	 *All scales and rotates have to be done at origin.
	 *The postRotateMatrix is required to move the quad 
	 *being drawn back to it's position
	*/
	postRotateMatrix*scaleMatrix*rotateMatrix*preRotateMatrix*vec4(in_position, 1.0);
	
	ex_Color = in_Color;
	ex_TextCoord = in_TexCoords;
	ex_particle_brightness = in_particle_brightness;
}