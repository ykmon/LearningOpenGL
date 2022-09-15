#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;

void main(){
	FragColor = mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),
				texture(ourFace,TexCoord).a*0.2);}	