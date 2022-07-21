
#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_Value;

void main(void)
{
	vec4 src = texture2D(u_texture, v_texCoord).rgba;

	if(	v_texCoord.x < u_Value*0.5 ||
		v_texCoord.x > 1. - u_Value*0.5 ||
		v_texCoord.y < u_Value*0.5 ||
		v_texCoord.y > 1. - u_Value*0.5)
		gl_FragColor = src;
	else
		gl_FragColor.a = 0.;
}
