#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _NoiseTexture;
uniform sampler2D _BrickTexture;

uniform float time;

uniform float distortSpeed;
uniform float zoomSpeed;
uniform float maxDistortion;

void main(){
	float noise = texture(_NoiseTexture,UV).r;
	//slowly distorts the background more or less over time
	vec2 uv = UV + noise * abs(sin(time*distortSpeed))*maxDistortion;
	//zooms in and out of the background
	FragColor = texture(_BrickTexture,uv*((sin(time*zoomSpeed)/2)+1));
}
