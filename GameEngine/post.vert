//Gbuffer rendering vertex shader

#version 430

in vec3 in_vertices;


void main() {
	gl_Position = vec4(in_vertices, 0.0f);
}