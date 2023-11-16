#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; // per fragment interpolated UV
	vec3 WorldPosition; // per fragment interpolated world position
	vec3 WorldNormal; // per fragment interpolated world normal
}fs_in;


struct Light
{
	vec3 position;
	vec3 color;
};
uniform float _LightsAmount;
uniform Light _Lights[4];

uniform vec3 _CameraPosition;

uniform float _Shininess;
uniform float _Diffuse;
uniform float _Specular;
uniform float _Ambient;
uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 viewAngle = normalize(_CameraPosition - fs_in.WorldPosition);
	vec3 light = vec3(0,0,0);
	Light _Light;
	for (int i = 0; i < _LightsAmount; i++){
		_Light = _Lights[i];
		vec3 lightAngle = normalize(_Light.position - fs_in.WorldPosition );
		vec3 halfVector = normalize(viewAngle + lightAngle);
		vec3 reflection = reflect(-lightAngle,normal);
		light = light + (_Light.color * _Ambient + _Light.color * _Diffuse * max(dot(lightAngle,normal),0) + _Light.color * _Specular * pow(max(dot(halfVector,normal),0),_Shininess));
	}
	FragColor = texture(_Texture,fs_in.UV) * vec4(light,1);

}