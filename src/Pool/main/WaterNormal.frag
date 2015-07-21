#version 330

in  vec2  fragmentTexCoord;
out vec3 fragmentNormal;

uniform mat4 objectMatrix;
uniform mat4 inversedObjectMatrix;
uniform sampler2D inPositions;
uniform int gridSizeX;
uniform int gridSizeZ;

vec3 get_triangle_normal(float x1, float z1, float x2, float z2, float x3, float z3, vec2 invSize) 
{
  float c1 = textureLod(inPositions, vec2(x1,z1)*invSize, 0).x; // get height from the texture	
  vec3 p1 = vec3(x1/gridSizeX, c1, z1/gridSizeZ);
  float c2 = textureLod(inPositions, vec2(x2,z2)*invSize, 0).x; // get height from the texture	
  vec3 p2 = vec3(x2/gridSizeX, c2, z2/gridSizeZ);
  float c3 = textureLod(inPositions, vec2(x3,z3)*invSize, 0).x; // get height from the texture	
  vec3 p3 = vec3(x3/gridSizeX, c3, z3/gridSizeZ);

  vec3 u = p2 - p1;
  vec3 v = p3 - p1;
  return cross(v, u);
}


void main(void) 
{
  float x = fragmentTexCoord.x*float(gridSizeX);
  float z = fragmentTexCoord.y*float(gridSizeZ);

  vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeZ));
  

  vec3 normal = -normalize(get_triangle_normal(x, z, x-1, z  , x-1, z+1, invSize) +
						  get_triangle_normal(x, z, x-1, z+1, x  , z+1, invSize) + 
						  get_triangle_normal(x, z, x  , z+1, x+1, z  , invSize) +
						  get_triangle_normal(x, z, x+1, z  , x+1, z-1, invSize) + 
						  get_triangle_normal(x, z, x+1, z-1, x  , z-1, invSize) + 
						  get_triangle_normal(x, z, x  , z-1, x-1, z  , invSize));

	mat3 normalM = transpose(mat3(inversedObjectMatrix));

	
	normal = normalM * normal;

	fragmentNormal  = normalize(normal);
}