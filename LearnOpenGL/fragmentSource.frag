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
	float constant;
	float linear;
	float quadratic;
};
uniform LightPoint LightP;

struct LightSpot{
	float cosPhy;
};
uniform LightSpot lightS;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 cameraPos;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDirUniform;
uniform float time;

void main(){				
	//FragColor = mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),texture(ourFace,TexCoord).a*0.2);
	//光线方向
	vec3 r=lightPos-FragPos;
	vec3 lightDir = normalize(r);

	//距离系数
	float coefficient=1;
	float d  = length(lightDir);
	//float coefficient= 1.0 / (LightP.constant + LightP.linear * d + LightP.quadratic * (d * d));
	
	//法向
	vec3 normal = normalize(Normal);

	//漫反射
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = texture(material.diffuse, TexCoord).rgb  * diff * lightColor;

	//观察方向
	vec3 cameraVec = normalize(cameraPos-FragPos);

	//材质
	//vec4 objColor= mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),texture(ourFace,TexCoord).a*0.2);
	
	//Phong镜面反射
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularAmount = pow(max(dot(reflectDir,cameraVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).rgb  * specularAmount * lightColor;

	//环境光
	vec3 ambient = material.ambient * ambientColor * texture(material.diffuse, TexCoord).rgb;
	
	//发光
	vec3 emission;
	if( texture(material.specular,TexCoord).rgb== vec3(0,0,0) ){
		emission=  coefficient * texture(material.emission,TexCoord).rgb;
		//fun
		emission = texture(material.emission,TexCoord + vec2(0.0,time/2)).rgb;//moving
		emission =  emission * (sin(time) * 0.5 + 0.5) * 2.0;//fading
	}

//	float cosTheta = dot(normalize(FragPos-lightPos), lightDirUniform * -1);
//	if( cosTheta > lightS.cosPhy) {
//		//inside
//		FragColor=vec4(ambient+(diffuse+specular)  +emission ,1.0f);
//	}
//	else{
//		//outside
//		FragColor=vec4(ambient,1.0f);
//	}
	//最终混合
	//FragColor=vec4(ambient+(diffuse+specular) * coefficient +emission ,1.0f);
	FragColor=vec4(diffuse+specular ,1.0f);
}
