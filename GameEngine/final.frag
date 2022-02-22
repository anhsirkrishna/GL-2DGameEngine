//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;
uniform int mode;
uniform sampler2D texture_map;
uniform vec2 tex_offset;
out vec4 out_Color;
uniform int particle;
uniform float particle_lifetime;

void main() {
	//Mode specified if we are filling with colors or textures.
	if (mode == 0)
		out_Color = ex_Color;
	else{
		vec4 tex_color = texture(texture_map, ex_TextCoord + tex_offset);
		//If we are renderign a particle add the color with particle lifetime
		if (particle == 1) {
			tex_color *= (ex_Color * particle_lifetime);
		}
		out_Color = tex_color; 
	}
}