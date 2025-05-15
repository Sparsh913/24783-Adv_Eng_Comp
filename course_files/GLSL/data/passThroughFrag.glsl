varying vec2 windowCoord;
void main()
{
	float R=(windowCoord.x+1.0)/2.0;
	float G=(windowCoord.y+1.0)/2.0;
    gl_FragColor=vec4(R,G,0,1);
}
