#version 330 core									  
layout(location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
//layout(location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
layout(location = 3) in vec2 aTexCoord; // uv变量的属性位置值为 2
layout(location = 2) in vec3 aNormal;

//out vec4 vertexColor;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

//uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(){										   
   gl_Position = projMat * viewMat * modelMat * vec4(aPos.xyz, 1.0);   
   Normal =mat3(transpose(inverse(modelMat)))*aNormal;
   FragPos=(modelMat * vec4(aPos.xyz,1.0)).xyz;
   //vertexColor = vec4(aColor,1.0);	
   TexCoord = aTexCoord;
}
