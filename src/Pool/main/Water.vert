#version 330

in vec4 vertex;
in vec2 texCoord;

uniform mat4 objectMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;

uniform int gridSizeX;
uniform int gridSizeZ;

uniform sampler2D inPositions;
uniform sampler2D inNormals;

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

out vec3  lightDir;
out vec3  viewDir;

out vec3 fragmentWorldPos;
out vec3 fragmentNormal;
out vec3 fragmentTexCoord;

void main(void) 
{
  fragmentTexCoord = vec3(vertex);

  float x = texCoord.x*float(gridSizeX);
  float z = texCoord.y*float(gridSizeZ);

  //vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeZ));
  
  float height = textureLod(inPositions, texCoord, 0).x; // get height from the texture	

  vec4 norm_tex = textureLod(inNormals, texCoord, 0);
  fragmentNormal = vec3(norm_tex.x, norm_tex.y, norm_tex.z);


  vec4 new_vertex = vertex;
  new_vertex.y = height;
  //fragmentTexCoord = vec3(vertex);
  new_vertex.a = 1.0;

  vec4 fragmentWorldPos4 = objectMatrix * new_vertex;
  fragmentWorldPos = vec3(fragmentWorldPos4);

  lightDir = vec3(light.position) - fragmentWorldPos;
  viewDir  = cameraPosition - vec3(fragmentWorldPos);

  gl_Position = viewProjectionMatrix * fragmentWorldPos4;
}
