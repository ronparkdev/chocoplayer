
#ifdef GLES2
precision lowp vec2;
precision lowp float; 
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform int u_Value[150];

int mod(int a, int b) { return a - (int(a / b) * b); }

void main(void)
{
	int i, id, ida, idb;
	
	id = int(v_texCoord.x*40.) + int(v_texCoord.y*30.) * 40;
	
	ida = id / 8;
	idb = mod(id,8);
	
	id = u_Value[ida];
	for(i=0; i<idb; i++)
		id /= 2;
		
	if(mod(id, 2) == 1)
		gl_FragColor = texture2D(u_texture, v_texCoord).rgba;
	else
		gl_FragColor.a = 0.;
}