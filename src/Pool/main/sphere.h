#pragma once

#include "camera.h"
#include "light.h"


class sphere {
public:
	sphere();
	~sphere();
	void update(const float4x4& view_projection, const camera& camera, const point_light& light);

private:
	SimpleMesh* sphere_mesh_;
	ShaderProgram render_prog_;
	Texture2D* sphere_tex_;
	float4x4 object_matrix_;
	material material_;
};