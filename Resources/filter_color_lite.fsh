
#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_AddR;
uniform float u_AddG;
uniform float u_AddB;
uniform float u_GrPer;
uniform float u_Alpha;

void main(void)
{
    vec4 src = texture2D(u_texture, v_texCoord).rgba;

	gl_FragColor.r = src.r + u_AddR;
	gl_FragColor.g = src.g + u_AddG;
    gl_FragColor.b = src.b + u_AddB;
    gl_FragColor.a = src.a - u_Alpha;
}