//Gbuffer rendering vertex shader

#version 430

in vec4 v_color;
in vec2 v_tex_coords;

uniform float transparency;
uniform int texture_enabled;
uniform sampler2D texture_map;

out vec4 out_color;

void main() {
	vec4 color;
	if (texture_enabled == 0)
		color = v_color;
	else if(texture_enabled == 1)
		color = texture(texture_map, v_tex_coords);
	if (color.a < 1)
		discard;
	out_color = vec4(color.xyz, transparency);
}