#pragma once

#include <GL/glew.h>

#include "GL/glus.h"
#include "../vsgl3/glHelper.h"
#include "light.h"
#include "camera.h"
#include "light.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class water
{
public:
  water();
  virtual ~water();

	void update(const float4x4& view_projection, const camera& camera, const point_light& light);
  virtual void sim_step();
  virtual void add_wave(GLUSfloat vec[3]);

	const float4x4& get_object_matrix() const { return object_matrix_; } 
	const float4x4& get_inversed_object_matrix() const { return inversed_object_matrix_; } 
	GLuint get_positions_tex() const { return pos_tex_[c_]->GetColorTexId(); }
	GLuint get_normals_tex() const { return norm_tex_->GetColorTexId(); }

	void calm();
protected:
  water(const water& rhs) {}
  water& operator=(const water& rhs) {return *this;}

  ShaderProgram render_prog_;
  ShaderProgram sim_prog_;
	ShaderProgram normal_prog_;

  int grid_size_x_;
  int grid_size_z_;

  RenderableTexture2D* pos_tex_[3];
	RenderableTexture2D* norm_tex_;
  int a_, b_, c_;

  FullScreenQuad* full_screen_quad_;

  // water mesh
	material material_;
  SimpleMesh*   water_mesh_;  
	float4x4 object_matrix_;
	float4x4 inversed_object_matrix_;
};



