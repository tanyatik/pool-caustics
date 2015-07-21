#version 330

in vec3  lightDir;
in vec3  viewDir;

in vec3 fragmentWorldPos;
//in vec3 fragmentNormalFast;
in vec3 fragmentNormal;
in vec3 fragmentTexCoord;

uniform mat4 objectMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;

uniform int gridSizeX;
uniform int gridSizeY;

uniform sampler2D inPositions;

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

out vec4 color;

void main(void)
{
	vec4 sceneColor = vec4(0.3,0.3,1.0,1.0);

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
	final_color.w = 0.3;
	
	color = final_color;			
}
