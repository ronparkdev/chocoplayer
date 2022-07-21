
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

	gl_FragColor.r = src.r * u_Value;
	gl_FragColor.g = src.g * u_Value;
	gl_FragColor.b = src.b * u_Value;
	gl_FragColor.a = src.a;
}
