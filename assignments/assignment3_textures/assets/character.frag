#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _CatTexture;

void main(){
	// scales the size of the cat so that it doesnt fill the screen
	FragColor = texture(_CatTexture,UV*2);
}