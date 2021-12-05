#version 330


layout (location = 0) in vec3 in_Position;	//위치변수
layout (location = 1) in vec3 in_Normal;	//노말변수
layout (location = 2) in vec2 in_vTexCoord; //텍스처변수

uniform mat4 transform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	gl_Position = projectionTransform * viewTransform * transform * vec4(in_Position, 1.0);
	
	FragPos = vec3(transform * vec4(in_Position, 1.0));

	Normal = mat3(transpose(inverse(transform))) * in_Normal;

	TexCoord = in_vTexCoord;
}