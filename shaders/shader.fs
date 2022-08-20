#version 120

varying vec2 fragPosition;
varying vec4 fragColour;
varying vec2 fragUV;

varying vec4 u_colour;

uniform float time;
uniform sampler2D mySampler;


void main(){

	vec4 textureColour = texture2D(mySampler, fragUV); 

	//u_colour = vec4(fragColour.r * (cos(fragPosition.x + time) + 1.0) * 0.5,
	//		fragColour.g * (cos(fragPosition.y + time) + 1.0) * 0.5,
	//		fragColour.b * (cos(fragPosition.x * 0.5 + time) + 1.0) * 0.5,
	//		fragColour.a) * textureColour;
	//u_colour = fragColour * textureColour;
	gl_FragColor = fragColour * textureColour;
}
