
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

	if(u_AddR < 0.)
		src.r *= u_AddR+1.;
	else
		src.r += u_AddR;
		
	if(u_AddG < 0.)
		src.g *= u_AddG+1.;
	else
		src.g += u_AddG;
		
	if(u_AddB < 0.)
		src.b *= u_AddB+1.;
	else
		src.b += u_AddB;


	if(u_GrPer != 0.){
		float valGr = (src.r + src.g + src.b) / 3.;
		src.r = ((src.r * (1. + u_GrPer)) - (valGr * u_GrPer));// * src.a;
		src.g = ((src.g * (1. + u_GrPer)) - (valGr * u_GrPer));// * src.a;
		src.b = ((src.b * (1. + u_GrPer)) - (valGr * u_GrPer));// * src.a;
	}
	src.a -= u_Alpha;
	
	gl_FragColor = vec4(src.r, src.g, src.b, src.a);
}
