#pragma once

#include "camera.h"
#include "light.h"
#include "water.h"
#include "pool.h"
#include "caustics.h"
#include "sphere.h"

struct scene {
	scene();
	~scene();

	void mouse_press(GLUSboolean pressed, GLUSuint button, GLUSuint x, GLUSuint y);
	void mouse_move(GLUSuint button, GLUSint x, GLUSint y);
	void keyboard(GLUSboolean pressed, GLUSuint key);
	void update(GLUSfloat time);
	void init();
	void reshape(GLUSuint width, GLUSuint height);
	void add_wave(float x, float z);
private:
	int width_;
	int height_;

	float4x4 projection_matrix_;
	float4x4 model_view_;

	water* water_;
	pool* pool_;
	caustics* caustics_;
	sphere* sphere_;
	camera camera1_;
	camera camera2_;
	camera camera3_;

	camera* current_camera_;

	point_light light_;

	void get_mouse_to_world_pos(float x, float y, GLUSfloat* pos_x, GLUSfloat* pos_y, GLUSfloat* pos_z);
};