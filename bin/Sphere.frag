#version 330

uniform	sampler2D sphereTex;

in vec3 Normal;
in vec2 TexCoord;

in vec3  lightDir;
in vec3  viewDir;

out vec4 color;

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



void main()
{	
	vec4 sceneColor;

	sceneColor = texture(sphereTex, TexCoord);
	
	vec4 final_color = 
		(sceneColor * material.ambient) + 
		(light.ambient * material.ambient);
							
	vec3 N = normalize(Normal);
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
	
	color = sceneColor;			
}
