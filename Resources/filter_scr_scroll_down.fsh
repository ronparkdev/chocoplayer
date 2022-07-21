
#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_Value;

void main(void)
{
	vec2 coord = v_texCoord;
	if(coord.y > u_Value){
		gl_FragColor.a = 0.;
	}else{
		coord.y -= -1. + u_Value;
		gl_FragColor = texture2D(u_texture, coord).rgba;
	}
}
