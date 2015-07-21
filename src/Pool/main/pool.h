#pragma once

#include <GL/glew.h>
#include "GL/glus.h"
#include "../vsgl3/glHelper.h"
#include "light.h"
#include "camera.h"
#include "caustics.h"

class pool
{
public:
  pool(GLfloat floor_height, GLfloat left_wall_coord, GLfloat right_wall_coord, GLfloat front_wall_coord, GLfloat back_wall_coord);
  ~pool();

	void update(const float4x4& view_projection, const camera& camera, const point_light& light, GLuint caustics_tex_id, int num_wall);

  GLuint get_render_program() { return render_prog_.program; }
	float4x4 get_object_matrix(int num_obj) const;
	float4x4 get_inversed_object_matrix(int num_obj) const;
	float3 get_wall_coord(int num_wall) const;
	/*
	float get_floor_height() const { return floor_height_; }
	float get_left_wall_coordinate() const { return left_wall_coordinate_; }
	float get_right_wall_coordinate() const { return right_wall_coordinate_; }
	float get_front_wall_coordinate() const { return front_wall_coordinate_; }
	float get_back_wall_coordinate() const { return back_wall_coordinate_; }
	*/
protected:
  pool(const pool& rhs) {}
  pool& operator=(const pool& rhs) {return *this;}

	void init_object_matrix(int num_obj, GLfloat scale_coeff_x, GLfloat scale_coeff_y, GLfloat scale_coeff_z,
		GLfloat translate_coeff_x, GLfloat translate_coeff_y, GLfloat translate_coeff_z, 
		GLfloat rotate_coeff_x, GLfloat rotate_coeff_z);

	void draw();

  ShaderProgram render_prog_;
	ShaderProgram caustics_prog_;

	Texture2D* floor_tex_;

	material material_;
  SimpleMesh*   pool_mesh_;  
	float4x4 object_matrix_[5];
	float4x4 inversed_object_matrix_[5];

	int grid_size_x_;
	int grid_size_z_;

	int size_x_[5];
	int size_z_[5];

	float3 wall_coord_[5];
	/*
	float floor_height_;
	float left_wall_coordinate_;
	float right_wall_coordinate_;
	float front_wall_coordinate_;
	float back_wall_coordinate_;
	*/
};



