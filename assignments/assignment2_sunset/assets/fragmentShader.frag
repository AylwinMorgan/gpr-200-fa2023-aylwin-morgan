#version 450
out vec4 fragColor;
uniform float iTime;
uniform float sunSpeed;
uniform vec3 skyColor1;
uniform vec3 skyColor2;
uniform vec3 skyColor3;
uniform vec3 skyColor4;
uniform vec3 sunColor1;
uniform vec3 sunColor2;
uniform vec3 mountainColor;
in vec2 uv;
void main(){
    vec2 center = vec2(0,sin(iTime*sunSpeed)-0.5);
    float d = distance(center,uv);
    float radius = 0.5;
    float t = smoothstep(radius,radius+0.1,d);
    
    float mountains = -0.5-abs(mod(uv.x,0.4)-0.2);
    mountains = -0.75+abs(sin(5.0*uv.x)/5.0);
    float drawMountains = step(mountains,uv.y);

    vec3 sunColor = mix(sunColor2,sunColor1,-sin(iTime*sunSpeed)*0.5);
    vec3 daySkyColor = mix(skyColor2,skyColor1,uv.y);
    vec3 nightSkyColor;
    
    vec3 skyColor = mix(daySkyColor,nightSkyColor,-sin(iTime*sunSpeed)*0.5);
    
    vec3 col = mix(sunColor,skyColor,t);
    col = mix(mountainColor, col, drawMountains);
    // Output to screen
    fragColor = vec4(col,1.0);

	//FragColor = vec4(UV,0.0,1.0);
}