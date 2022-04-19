#version 310 es

precision mediump float;
#define MAX_NUM_LIGHTS 10 //has to be the same as the vert

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct TexMaterial {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

/*struct SpotLight{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;

//spotlight
float innerCutOff;
float outerCutOff; 
};*/

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_NUM_LIGHTS];
uniform Material material;
uniform TexMaterial texMatt;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform int typeOfRend;

in flat int pointLightsAmount;
in vec2 v_TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;
in vec3 TangentLightPos[MAX_NUM_LIGHTS];
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 PlayerPos;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	// combine results
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, v_TexCoord));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1, v_TexCoord)));
	vec3 specular = light.specular * (spec * vec3(texture(texture_specular1, v_TexCoord)));

	return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int i)
{
	vec3 lightDir = normalize(TangentLightPos[i] - fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// attenuation
	float dist = length(TangentLightPos[i] - fragPos);

	float attenuation = 1.0 / (light.constant + light.linear * dist +
	light.quadratic * (dist * dist));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, v_TexCoord));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1, v_TexCoord)));
	vec3 specular = light.specular * (spec * vec3(texture(texture_specular1, v_TexCoord)));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
};

void main()
{
	vec3 normal = texture(texture_normal1, v_TexCoord).rgb;
	
    normal = (normal * 2.0) - 1.0;   
	normal = normalize(TBN * normal); 

	vec3 viewDir =  normalize(TangentViewPos - TangentFragPos);
	
	// phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, normal, viewDir);

	// phase 2: Point lights
	for(int i = 0; i < pointLightsAmount; i++){
		result += CalcPointLight(pointLights[i], normal, TangentFragPos, viewDir, i);
	}

	// phase 3: Spot light
	//	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	if(typeOfRend == 0)
	FragColor = vec4(result, 1.0);  

	else if(typeOfRend == 1)
	FragColor = vec4(normal, 1.0);

	else if(typeOfRend == 2)
	FragColor = vec4(TBN * vec3(0.0, 0.0, 1.0), 1.0);

	else if(typeOfRend == 3)
	FragColor = texture(texture_diffuse1, v_TexCoord);

	else if(typeOfRend == 4)
	FragColor = texture(texture_normal1, v_TexCoord);

	else if(typeOfRend == 5)
	FragColor = texture(texture_specular1, v_TexCoord);
}



