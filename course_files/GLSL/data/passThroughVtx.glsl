attribute vec3 vertex;
varying vec2 windowCoord;
void main()
{
    gl_Position=vec4(vertex,1.0);
	windowCoord=vertex.yx;
}
