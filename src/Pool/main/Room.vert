#version 330

uniform mat4 projectionMatrix;
uniform mat4 objectMatrix;
uniform mat4 inversedObjectMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;
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

in vec4 vertex;
in vec2 texCoord;
in vec4 normal;

out vec3  lightDir;
out vec3  viewDir;
out float  distance;

out vec3 fragmentWorldPos;
out vec3 fragmentNormal;
out vec2 fragmentTexCoord;

uniform sampler2D caustics;
uniform sampler2D floorTilt;

uniform int sizeX;
uniform int sizeZ;

void main(void)
{
  vec4 fragmentWorldPos4 = objectMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);
  fragmentWorldPos = vec3(fragmentWorldPos4);
  fragmentTexCoord = texCoord;
  
  mat3 normalM = transpose(mat3(inversedObjectMatrix));
  lightDir = vec3(light.position) - fragmentWorldPos;

  vec3 normal3 = vec3(normal.x, normal.y, normal.z);

  fragmentNormal   = normalM * normal3;
  viewDir  = cameraPosition - vec3(fragmentWorldPos);
  distance = length(lightDir);

  gl_Position = viewProjectionMatrix * fragmentWorldPos4;
}
