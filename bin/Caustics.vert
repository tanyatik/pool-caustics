#version 330

in vec2 vertex;

uniform sampler2D waterPositions;
uniform sampler2D waterNormals;
uniform vec3 wallCoord;
uniform mat4 waterObjectMatrix;
uniform mat4 poolInversedObjectMatrix;

uniform vec4 lightPosition;

out vec4 cLightColor;


void main() {
	float t, tmp;
	vec4 worldQ, Q, P, position;
	vec3 worldP, waterN, lightDirection, Refracted;
	vec2 tex_coord = vec2(vertex.x * 0.5 + 0.5, vertex.y * 0.5 + 0.5);

	cLightColor = vec4(0.3, 0.3, 0.3, 0.6);
		
	if(wallCoord.y != 0.0) { // floor
		P = vec4(vertex.x, textureLod(waterPositions, tex_coord, 0).x, vertex.y, 1.0);
	}
	else { // walls
		P = vec4(vertex.y, textureLod(waterPositions, tex_coord, 0).x, vertex.x, 1.0);
	}

	worldP = vec3(waterObjectMatrix * P);

	waterN = vec3(textureLod(waterNormals, tex_coord, 0));
	
	lightDirection = normalize(worldP - vec3(lightPosition)); // direcion to light from P in world coordinates
	Refracted = refract(lightDirection, waterN, 1.0/1.44); // refracted vector

	if(wallCoord.y != 0.0) {
		t = (wallCoord.y - worldP.y) / (Refracted.y); 
		worldQ = vec4(worldP.x + t*Refracted.x, wallCoord.y, worldP.z + t*Refracted.z, 1.0);
	}
	else if(wallCoord.x != 0.0) { // left or right wall
		t = (wallCoord.x - worldP.x) / (Refracted.x); 
		worldQ = vec4(wallCoord.x, worldP.y + t*Refracted.y, worldP.z + t*Refracted.z, 1.0);
	}
	else if(wallCoord.z != 0.0) { // front or back wall 
		t = (wallCoord.z - worldP.z) / (Refracted.z); 
		worldQ = vec4(worldP.x + t*Refracted.x, worldP.y + t*Refracted.y, wallCoord.z, 1.0);
	}
	
	Q = poolInversedObjectMatrix*worldQ;
	position = vec4(Q.z, Q.x, 1.0, 1.0);

	if(wallCoord.y != 0.0) {
		tmp = position.x;
		position.x = position.y;
		position.y = tmp;
	}

	gl_Position = position;
}