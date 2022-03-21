//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;
in float ex_particle_brightness;

uniform int mode;
uniform sampler2D texture_map;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 post_Buffer;

void main() {
	//Mode specified if we are filling with colors or textures.
	if (mode == 0)
		out_Color = ex_Color;
	else{
		vec4 tex_color = texture(texture_map, ex_TextCoord);
		if (tex_color.a < 0.01)
			discard;
		//If we are renderign a particle add the color with particle lifetime
		tex_color *= ex_Color;
		out_Color = tex_color;
	}

	post_Buffer = vec4(vec3(out_Color.rgb), 1.0f);
}