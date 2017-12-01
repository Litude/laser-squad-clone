uniform sampler2D texture;

void main()
{	
	vec2 texOffset = vec2(0.01, 0.01);
	vec2 vertTexCoord = gl_TexCoord[0].xy;
	vec2 tc0 = vertTexCoord.st + vec2(-texOffset.s, -texOffset.t);
	vec2 tc1 = vertTexCoord.st + vec2(         0.0, -texOffset.t);
	vec2 tc2 = vertTexCoord.st + vec2(+texOffset.s, -texOffset.t);
	vec2 tc3 = vertTexCoord.st + vec2(-texOffset.s,          0.0);
	vec2 tc4 = vertTexCoord.st + vec2(         0.0,          0.0);
	vec2 tc5 = vertTexCoord.st + vec2(+texOffset.s,          0.0);
	vec2 tc6 = vertTexCoord.st + vec2(-texOffset.s, +texOffset.t);
	vec2 tc7 = vertTexCoord.st + vec2(         0.0, +texOffset.t);
	vec2 tc8 = vertTexCoord.st + vec2(+texOffset.s, +texOffset.t);
	 
	vec4 col0 = texture2D(texture, tc0);
	vec4 col1 = texture2D(texture, tc1);
	vec4 col2 = texture2D(texture, tc2);
	vec4 col3 = texture2D(texture, tc3);
	vec4 col4 = texture2D(texture, tc4);
	vec4 col5 = texture2D(texture, tc5);
	vec4 col6 = texture2D(texture, tc6);
	vec4 col7 = texture2D(texture, tc7);
	vec4 col8 = texture2D(texture, tc8);
	 
	vec4 sum = (1.0 * col0 + 2.0 * col1 + 1.0 * col2 + 
				2.0 * col3 + 4.0 * col4 + 2.0 * col5 +
				1.0 * col6 + 2.0 * col7 + 1.0 * col8) / 16.0;
	vec4 final = gl_Color * sum;
	gl_FragColor = vec4(final.x, final.y, final.z, min(1.0-final.w, 0.5));
}
