#version 120

attribute vec2 vertPosition;

attribute vec4 vertColour;
attribute vec2 vertUV;

varying vec2 fragPosition;
varying vec4 fragColour;
varying vec2 fragUV;

uniform mat4 P;

void main(){
	gl_Position.xy = (P * vec4(vertPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	fragColour = vertColour;
	fragPosition = vertPosition;
	fragUV = vec2(vertUV.x, 1.0 - vertUV.y);
}
