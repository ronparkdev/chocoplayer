
#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform int u_Value;

int mod(int a, int b) { return a - (int(a / b) * b); }

void main(void)
{
	int id, idm;

	vec4 src = texture2D(u_texture, v_texCoord).rgba;

	id = int(v_texCoord.y*60.);

	idm = mod(id, 2);
	
	if(((idm == 0) && (u_Value > id)) || 
	   ((idm == 1) && (u_Value > 60 - id)))
		gl_FragColor = src;
	else
		gl_FragColor.a = 0.;
}
