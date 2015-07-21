#include <GL/glew.h>
#include "../vsgl3/glHelper.h"
#include "light.h"

point_light::point_light() 
	: position_(0.0f, 7.0f, 0.0f, 1.0f)
	, ambient_(0.7f, 0.7f, 0.7f, 1.0f)
	, diffuse_(1.0f, 1.0f, 1.0f, 1.0f)
	, specular_(0.7f, 0.7f, 0.7f, 1.0f)
	{}

void point_light::setup_shader(GLuint program) const {
	setUniform(program, "light.position", position_);
	setUniform(program, "light.ambient",  ambient_);
	setUniform(program, "light.diffuse",  diffuse_);
	setUniform(program, "light.specular", specular_);
}


material::material()
	: ambient_(0.8f, 0.8f, 0.8f, 1.0f)
	, diffuse_(1.0f, 1.0f, 1.0f, 1.0f)
	, specular_(1.0f, 1.0f, 1.0f, 1.0f)
	, shininess_(0.6f)
	{}

material::material(float4 a, float4 d, float4 s, float sh)
	: ambient_(a)
	, diffuse_(d)
	, specular_(s)
	, shininess_(sh)
	{}

void material::setup_shader(GLuint program) const {
	setUniform(program, "material.ambient",  ambient_);
	setUniform(program, "material.diffuse",  diffuse_);
	setUniform(program, "material.specular", specular_);
	float3 shininess;
	shininess.x = shininess_;
	setUniform(program, "material.shininess", shininess);
}