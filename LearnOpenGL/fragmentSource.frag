#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 cameraPos;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){				
	//FragColor = mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),texture(ourFace,TexCoord).a*0.2);
	//�۲췽��
	vec3 cameraVec = normalize(cameraPos-FragPos);
	//���߷���
	vec3 r=lightPos-FragPos;
	vec3 lightDir = normalize(r);
	//����ϵ��
	float coefficient=180/(pow(r.x,2)+pow(r.y,2)+pow(r.z,2));
	//����
	vec3 normal = normalize(Normal);
	//������
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = texture(material.diffuse, TexCoord).rgb * coefficient * diff * lightColor;
	//����
	//vec4 objColor= mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),texture(ourFace,TexCoord).a*0.2);
	//Phong���淴��
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularAmount = pow(max(dot(reflectDir,cameraVec),0),material.shininess);
	vec3 specular = material.specular * coefficient * specularAmount * lightColor;

	//������
	vec3 ambient = material.ambient * ambientColor * texture(material.diffuse, TexCoord).rgb;

	//���ջ��
	FragColor=vec4(diffuse + ambient + specular,1.0);//* objColor;
}
