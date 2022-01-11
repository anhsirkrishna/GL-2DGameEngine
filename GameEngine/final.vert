//Vertex shader

#version 430
in vec3 in_position;
in vec4 in_Color;
in vec2 in_TexCoords;
uniform mat4 orthoGraphProj;
uniform mat4 translateMatrix;
uniform mat4 scaleMatrix;
uniform mat4 rotateMatrix, preRotateMatrix, postRotateMatrix;
out vec4 ex_Color;
out vec2 ex_TextCoord;

void main() {
	gl_Position = orthoGraphProj*translateMatrix*postRotateMatrix*scaleMatrix*rotateMatrix*preRotateMatrix*vec4(in_position, 1.0);
	ex_Color = in_Color;
	ex_TextCoord = in_TexCoords;
}