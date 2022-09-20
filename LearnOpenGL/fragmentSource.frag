#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};
uniform Material material;

struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
};
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;

struct LightSpot{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
	float cosPhyInner;
	float cosPhyOutter;
};
uniform LightSpot lightS;

struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};
uniform LightDirectional lightD;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 cameraPos;
uniform vec3 ambientColor;
uniform float time;

vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){
	
	// diffuse max(dot(L,N),0)
	float diffIntensity = max(dot(light.dirToLight, uNormal),0);
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;
	// seprcular pow(max(dot(R , Cam),0),shininess)
	vec3 R = normalize(reflect(-light.dirToLight,uNormal));
	float sepcIntensity = pow(max(dot(R ,dirToCamera),0),material.shininess);
	vec3 specColor = sepcIntensity * light.color * texture(material.specular, TexCoord).rgb;
	
	vec3 result = diffColor  + specColor;
	return result;
};

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
	// Ë¥¼õ
	float dist = length(light.pos - FragPos);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	// diffuse
	float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal),0) * attenuation;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;
	// sepcular
	vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
	float sepcIntensity = pow(max(dot(R ,dirToCamera),0),material.shininess) * attenuation;
	vec3 specColor = sepcIntensity * light.color * texture(material.specular, TexCoord).rgb;
	
	vec3 result = diffColor;
	return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
	// Ë¥¼õ
	float dist = length(light.pos - FragPos);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	float spotRatio;
	float cosTheta = dot(normalize(FragPos - light.pos),-light.dirToLight);

	if(cosTheta > light.cosPhyInner){
		spotRatio = 1.0;
	}
	else if(cosTheta > light.cosPhyOutter){
		//spotRatio = 0.5;
		spotRatio = (cosTheta - lightS.cosPhyOutter)/(light.cosPhyInner - lightS.cosPhyOutter);
	}
	else{
		spotRatio = 0.0;
	}
	// diffuse
	float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal),0) * attenuation * spotRatio;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;
	// sepcular
	vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
	float sepcIntensity = pow(max(dot(R ,dirToCamera),0),material.shininess) * attenuation * spotRatio;
	vec3 specColor = sepcIntensity * light.color * texture(material.specular, TexCoord).rgb;
	
	vec3 result = diffColor  + specColor;
	return result;
}

void main(){				
	
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);

	finalResult += CalcLightDirectional( lightD, uNormal, dirToCamera);
	finalResult += CalcLightPoint( lightP0, uNormal, dirToCamera);
	finalResult += CalcLightPoint( lightP1, uNormal, dirToCamera);
	finalResult += CalcLightPoint( lightP2, uNormal, dirToCamera);
	finalResult += CalcLightPoint( lightP3, uNormal, dirToCamera);
	finalResult += CalcLightSpot( lightS, uNormal, dirToCamera);
	FragColor=vec4(finalResult ,1.0f);
}
