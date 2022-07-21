#ifdef GLES2
precision lowp vec2;
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform sampler2D u_background;
uniform float u_bg_x; //real pos x / 160
uniform float u_bg_y; //real pos y / 80

/*
vec2 bgpos = vec2(
		u_bg_x + mod(v_texCoord.x, 0.076923) * 13. / 10. , 
		u_bg_y + mod(v_texCoord.y, 0.25) * 4. / 5.
	) ;
*/

void main(void)
{
	vec4 mask = texture2D(u_texture, v_texCoord).rgba;
	
	vec2 bgpos = vec2(
			u_bg_x + mod(v_texCoord.x, 0.076923) *  1.3 , 
			u_bg_y + mod(v_texCoord.y, 0.25) * 0.8
		) ;
	vec4 bg = texture2D(u_background, bgpos).rgba;
	
	bg.r *= mask.a;
	bg.g *= mask.a;
	bg.b *= mask.a;
	bg.a = mask.a;
	
	gl_FragColor = bg;
}