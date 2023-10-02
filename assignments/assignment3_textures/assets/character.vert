#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float time;
uniform float catSpeed;

vec3 offset = vec3(0,0,0);
void main(){
	UV = vUV;
	//moves cat around
	offset = vec3(-abs(sin(time*2*catSpeed)),-abs(sin(time*4*catSpeed)/2),0);
	gl_Position = vec4(vPos-offset,1.0);
}