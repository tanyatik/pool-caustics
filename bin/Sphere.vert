#version 330

uniform struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;

uniform struct Material
{
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;

    vec3 shininess;
} material;

uniform mat4 objectMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;

in vec4 vertex;
in vec2 texCoord;
in vec4 normal;

out vec2 TexCoord;
out vec3 Normal;

out vec3  lightDir;
out vec3  viewDir;


void main()
{
	TexCoord = vec2(texCoord);

	vec4 fragmentWorldPos4 = objectMatrix * vertex;
	vec4 fragmentWorldNorm4 = objectMatrix * normal;
	Normal = normalize(vec3(fragmentWorldNorm4));
		

	lightDir = vec3(light.position) - vec3(fragmentWorldPos4);
	viewDir  = cameraPosition - vec3(fragmentWorldPos4);

	gl_Position = viewProjectionMatrix * fragmentWorldPos4;
}