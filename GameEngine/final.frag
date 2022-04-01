//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;

uniform int mode;
uniform sampler2D texture_map;
uniform vec2 tex_offset;
uniform vec3 brightness;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 post_Buffer;

void main() {
	//Mode specified if we are filling with colors or textures.
	if (mode == 0){
		out_Color = ex_Color;
	}
	else {
		vec4 tex_color = texture(texture_map, ex_TextCoord + tex_offset);
		if (tex_color.a < 0.01)
			discard;
		out_Color = tex_color;
	}
	out_Color.rgb *= brightness;

	float luminance = dot(out_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (luminance > 1.5f)
		post_Buffer = vec4(vec3(out_Color.rgb), 1.0f);
	else
		post_Buffer = vec4(vec3(0.0f), 1.0f);
}