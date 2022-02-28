//Gbuffer rendering vertex shader

#version 430

const float exposure = 1;

uniform sampler2D gBuffer;
uniform sampler2D gBuffer_d;
uniform sampler2D bloomBuffer;

uniform int width, height;
uniform float gamma;

uniform int debug;

out vec4 out_color;

void main() {
	vec2 uv = gl_FragCoord.xy / vec2(width, height);
	vec4 fragColor = texture(gBuffer, uv);
	vec4 bloomColor;
	bloomColor = texture(bloomBuffer, uv);
	if (debug == 1)
		bloomColor = vec4(0.0f);
	if (debug == 2){
		out_color = texture(gBuffer_d, uv);
		return;
	}
	fragColor += bloomColor;

	//Convert color back into sRGB space
    fragColor.rgb = (exposure*fragColor.rgb)/(exposure*fragColor.rgb + 1);
	//fragColor.rgb = vec3(1.0) - exp(-fragColor.rgb*exposure);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0f/gamma));
	out_color = vec4(fragColor.rgb, 1.0);
}