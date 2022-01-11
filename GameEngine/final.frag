//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;
uniform int mode;
uniform sampler2D texture_map;
uniform vec2 tex_offset;
out vec4 out_Color;

void main() {
	if (mode == 0)
		out_Color = ex_Color;
	else{
		vec4 tex_color = texture2D(texture_map, ex_TextCoord + tex_offset);
		if (tex_color.a < 0.1)
			discard;
		out_Color = tex_color; 
	}
}