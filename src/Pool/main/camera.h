#pragma once

#include <GL/glew.h>

#include "GL/glus.h"
#include "../vsgl3/glHelper.h"

struct camera
{
  camera();
	camera(float x, float y, float z, float dx, float dy, float dz);

	public:
		void step_forward();
		void step_backward();
		void step_up();
		void step_down();
		void step_left();
		void step_right();
		void rotate(int x, int y);
		void update(GLUSfloat* model_view);
		void set_old_pos(int x, int y);
		float3 get_pos() const;
		void debug_print() const;
	private:
  float2 old_pos;
	float3 pos;
	float3 dir;
	float azimuth;
	float zenith;
	float step;
};
