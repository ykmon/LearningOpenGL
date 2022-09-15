#version 330 core									  
layout(location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0
layout(location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1
layout(location = 2) in vec2 aTexCoord; // uv����������λ��ֵΪ 2

out vec4 vertexColor;
out vec2 TexCoord;

//uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(){										   
   gl_Position = projMat * viewMat * modelMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);   
   vertexColor = vec4(aColor,1.0);	
   TexCoord = aTexCoord;
}
