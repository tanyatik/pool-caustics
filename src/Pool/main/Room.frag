#version 330

in vec3  lightDir;
in vec3  viewDir;
in float  distance;

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 objectMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;

uniform sampler2D caustics;
uniform sampler2D floorTilt;

uniform int gridSizeX;
uniform int gridSizeZ;

uniform int sizeX;
uniform int sizeZ;

uniform struct PointLight
{
        vec4 position;
        vec4 ambient;
        vec4 diffuse;
        vec4  specular;
} light;

uniform struct Material
{
        vec4  ambient;
        vec4  diffuse;
        vec4  specular;
        vec3 shininess;
} material;

out vec4 color;



void main(void)
{
	float x = fragmentTexCoord.x*float(gridSizeX);
	float z = fragmentTexCoord.y*float(gridSizeZ);

	vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeZ));

	
	vec4 causticsColors[9];
	causticsColors[0] = textureLod(caustics, vec2(x - 1, z - 1)*invSize, 0);
	causticsColors[1] = textureLod(caustics, vec2(x - 1, z    )*invSize, 0);
	causticsColors[2] = textureLod(caustics, vec2(x - 1, z + 1)*invSize, 0);
	causticsColors[3] = textureLod(caustics, vec2(x    , z - 1)*invSize, 0);
	causticsColors[4] = textureLod(caustics, vec2(x    , z    )*invSize, 0);
	causticsColors[5] = textureLod(caustics, vec2(x    , z + 1)*invSize, 0);
	causticsColors[6] = textureLod(caustics, vec2(x + 1, z - 1)*invSize, 0);
	causticsColors[7] = textureLod(caustics, vec2(x + 1, z    )*invSize, 0);
	causticsColors[8] = textureLod(caustics, vec2(x + 1, z + 1)*invSize, 0);
	
	/*
	Median filter

	float tmp;

	for (int n = 8; n > 0; --n) {
	  for (int i = 0; i < n; ++i) {
		if(causticsColors[i].x > causticsColors[i+1].x) {
		  tmp = causticsColors[i].x;
		  causticsColors[i].x = causticsColors[i+1].x;
		  causticsColors[i+1].x = tmp;
		}

		if(causticsColors[i].y > causticsColors[i+1].y) {
		  tmp = causticsColors[i].y;
		  causticsColors[i].y = causticsColors[i+1].y;
		  causticsColors[i+1].y = tmp;
		}

		if(causticsColors[i].z > causticsColors[i+1].z) {
		  tmp = causticsColors[i].z;
		  causticsColors[i].z = causticsColors[i+1].z;
		  causticsColors[i+1].z = tmp;
		}
	  }
	}
	*/	
	
	vec4 causticsColor;
	causticsColor.x = 0.5 * causticsColors[4].x + 0.128 * (causticsColors[0].x + causticsColors[1].x + causticsColors[2].x +
														   causticsColors[3].x + causticsColors[5].x + 
														   causticsColors[6].x + causticsColors[7].x + causticsColors[8].x);
	causticsColor.y = 0.5 * causticsColors[4].y + 0.128 * (causticsColors[0].y + causticsColors[1].y + causticsColors[2].y +
														   causticsColors[3].y + causticsColors[5].y + 
														   causticsColors[6].y + causticsColors[7].y + causticsColors[8].y);
	causticsColor.z = 0.5 * causticsColors[4].z + 0.128 * (causticsColors[0].z + causticsColors[1].z + causticsColors[2].z +
														   causticsColors[3].z + causticsColors[5].z + 
														   causticsColors[6].z + causticsColors[7].z + causticsColors[8].z);

	causticsColor.w = 0.5 * causticsColors[4].w + 0.128 * (causticsColors[0].w + causticsColors[1].w + causticsColors[2].w +
														   causticsColors[3].w + causticsColors[5].w + 
														   causticsColors[6].w + causticsColors[7].w + causticsColors[8].w);
	
	vec2 tmp_tex_coord = vec2(fragmentTexCoord.x * sizeZ, fragmentTexCoord.y * sizeX);
	tmp_tex_coord = vec2(tmp_tex_coord.x - floor(tmp_tex_coord.x), tmp_tex_coord.y - floor(tmp_tex_coord.y));

	vec4 sceneColor = causticsColor + texture(floorTilt, tmp_tex_coord);

	vec4 final_color = 
		(sceneColor * material.ambient) + 
		(light.ambient * material.ambient);
							
	vec3 N = normalize(fragmentNormal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm >= 0.0)
	{
		final_color += light.diffuse * 
		               material.diffuse * 
					   lambertTerm;	
		
		vec3 E = normalize(viewDir);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 material.shininess.x );
		final_color += light.specular * 
		               material.specular * 	
					   specular;	
	}
	final_color.w = 1.0;
	
	color = final_color;			
}
