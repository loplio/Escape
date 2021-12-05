#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform mat4 LightTransform;

uniform sampler2D texture0;

void main()
{
	float ambientLight = 0.3;

	vec3 normalVector = normalize(Normal);
	vec3 Pos = lightPos;
	Pos = vec3(LightTransform * vec4(Pos, 1.0));
	vec3 lightDir = normalize(Pos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);

	int shininess = 128;
	vec3 vPos = viewPos;
	vec3 viewDir = normalize(vPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0);
	specularLight = pow(specularLight, shininess);

	vec3 result = (diffuseLight + specularLight + ambientLight) * lightColor * objectColor;

	FragColor = texture(texture0, TexCoord) * vec4(result, 1.0f);
	//FragColor = vec4(result, 1.0);
}