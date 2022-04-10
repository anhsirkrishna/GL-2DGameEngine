//Gbuffer rendering vertex shader

#version 430

uniform float transparency;

in vec4 v_color;

out vec4 out_color;

void main() {
	out_color = vec4(v_color.xyz, transparency);
}