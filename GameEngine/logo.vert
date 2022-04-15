//Fade In/Out rendering

#version 430

in vec3 in_vertices;
in vec4 in_color;
in vec2 in_tex_coords;

out vec4 v_color;
out vec2 v_tex_coords;
void main() {
	gl_Position = vec4(in_vertices, 1.0f);
	v_color = in_color;
	v_tex_coords = in_tex_coords;
}