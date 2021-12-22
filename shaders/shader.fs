#version 130

in vec2 fragPosition;
in vec4 fragColour;
in vec2 fragUV;

out vec4 u_colour;

uniform float time;
uniform sampler2D mySampler;


void main(){

	vec4 textureColour = texture(mySampler, fragUV); 

	//u_colour = vec4(fragColour.r * (cos(fragPosition.x + time) + 1.0) * 0.5,
	//		fragColour.g * (cos(fragPosition.y + time) + 1.0) * 0.5,
	//		fragColour.b * (cos(fragPosition.x * 0.5 + time) + 1.0) * 0.5,
	//		fragColour.a) * textureColour;
	u_colour = fragColour * textureColour;

}
