#pragma once

#include <GL/glew.h>
#include "../vsgl3/LiteMath.h"

struct point_light
{
	point_light();
	void setup_shader(GLuint program) const;

	float4 get_position() const { return position_; }

	private:
		float4  position_;
		float4  ambient_;
		float4  diffuse_;
		float4  specular_;
};

struct material
{
	material();
	material(float4 a, float4 d, float4 s, float sh);
	void setup_shader(GLuint program) const;

	private:
		float4  ambient_;
		float4  diffuse_;
		float4  specular_;
		float shininess_;
};
