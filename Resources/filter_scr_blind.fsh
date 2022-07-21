
#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_Value;

int mod(int a, int b) { return a - (int(a / b) * b); }

void main(void)
{
	float id;
	
	vec4 src = texture2D(u_texture, v_texCoord).rgba;

	id = float(mod(int(v_texCoord.y*480.), 8)) / 8.;

	if(u_Value > id)
		gl_FragColor = src;
	else
		gl_FragColor.a = 0.;
}
