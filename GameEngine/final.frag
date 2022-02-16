//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;
uniform int mode;
uniform sampler2D texture_map;
uniform vec2 tex_offset;
out vec4 out_Color;
uniform int particle;

void main() {
	//Mode specified if we are filling with colors or textures.
	if (mode == 0)
		out_Color = ex_Color;
	else{
		vec4 tex_color = texture2D(texture_map, ex_TextCoord + tex_offset);
		//If the alpha value is transparent then discard the pixel to achieve transparency.
		if (tex_color.a < 0.1)
			discard;
		//If we are renderign a particle add the color
		if (particle == 1)
			tex_color += ex_Color;
		out_Color = tex_color; 
	}
}