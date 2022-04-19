#version 310 es

precision mediump float;
#define MAX_NUM_LIGHTS 10 //has to be the same as the frag

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords; 
layout (location = 3) in vec3 aTangent; 
layout (location = 4) in vec3 aBitangent; 

uniform mat4 u_MVP;
uniform mat4 model;

uniform int numPointLights;
uniform vec3 pospointLights[MAX_NUM_LIGHTS]; //position
uniform vec3 viewPos;
uniform vec3 playerPos;

out vec2 v_TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;
out vec3 TangentLightPos[MAX_NUM_LIGHTS];
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out flat int pointLightsAmount;

void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	T = normalize(T - dot(T, N) * N);

	FragPos = vec3(model * vec4(aPos.xyz, 1.0));

	Normal = mat3(transpose(inverse(model))) * aNormal; //expensive, calculate it on the CPU (transpose + inverse)
	v_TexCoord = aTexCoords;
	
	TBN = mat3(T, B, N);   

	for(int i = 0; i < numPointLights; i++){
		TangentLightPos[i] = TBN * pospointLights[i];
	}
	
	pointLightsAmount = numPointLights;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));

	gl_Position = u_MVP * vec4(aPos.xyz, 1.0);

};