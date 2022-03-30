//Fragment shader code for UI elements

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;

uniform int mode;
uniform sampler2D texture_map;
uniform vec2 tex_offset;
uniform float gamma;
uniform int text;

layout(location = 0) out vec4 out_Color;

void main() {
	//Mode specified if we are filling with colors or textures.
	if (mode == 0){
		out_Color = ex_Color;
	}
	else {
		if (text == 1) {
			out_Color = vec4( ex_Color.xyz, texture(texture_map, ex_TextCoord + tex_offset).r);
			if (out_Color.a < 0.01)
				discard;
		}
		else {
			vec4 tex_color = texture(texture_map, ex_TextCoord + tex_offset);
			if (tex_color.a < 0.01)
				discard;
			out_Color = tex_color;
		}
	}
	out_Color.rgb = pow(out_Color.rgb, vec3(1.0f/gamma));
}