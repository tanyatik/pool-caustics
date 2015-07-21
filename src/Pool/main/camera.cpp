#include "camera.h"

#define _USE_MATH_DEFINES

#include <math.h>

camera::camera() {
	camera::camera(0, 7.0, -20.0, 0.0, 0.0, 20.0);
}

camera::camera(float x, float y, float z, float dx, float dy, float dz)
	: pos(x,y,z)
	, dir(dx - x, dy - y, dz - z)
	, step(0.3f) {
		dir.normalize();	  

		zenith = acos(dir.y);
		azimuth = atan(-dir.z / dir.x);
		if(dir.z > 0) azimuth += M_PI;
		
		old_pos.x = old_pos.y = 0;
		float temp[16];
		glusLookAtf(temp, pos.x, pos.y, pos.z, 
			                pos.x + dir.x, pos.y + dir.y, pos.z + dir.z, 
			                0.0f, 1.0f, 0.0f);

}

void camera::step_forward() {
	pos.x += dir.x * step;
	pos.z += dir.z * step;
}

void camera::step_backward() {
	pos.x -= dir.x * step;
	pos.z -= dir.z * step;
}

void camera::step_up() {
	pos.y += step;
}

void camera::step_down() {
	pos.y -= step;
}

void camera::step_left() {
	pos.x += dir.z * step;
	pos.z -= dir.x * step;
}

void camera::step_right() {
	pos.x -= dir.z * step;
	pos.z += dir.x * step;
}

void camera::rotate(int x, int y) {
	float delta_az = static_cast<float>(x - old_pos.x)*0.01f;
	float delta_zen = static_cast<float>(y - old_pos.y)*0.01f;

	azimuth += delta_az;
	dir.x = sin(azimuth);
	dir.z = -cos(azimuth); 

	float tmp = zenith + delta_zen;
	if (tmp >= 0 && tmp <=  M_PI) {
		zenith = tmp;
		dir.y = cos(zenith);
	}
	dir.normalize();
	old_pos.x = float(x);
	old_pos.y = float(y);
}

void camera::update(GLUSfloat* model_view) {
	glusLookAtf(model_view, pos.x, pos.y, pos.z, 
			                    pos.x + dir.x, pos.y + dir.y, pos.z + dir.z, 
			                    0.0f, 1.0f, 0.0f);
}

void camera::set_old_pos(int x, int y) {
	old_pos.x = float(x);
	old_pos.y = float(y);
}
 
float3 camera::get_pos() const {
	return pos;
}

void camera::debug_print() const {
	printf("pos: %f %f %f dir: %f %f %f az: %f zen: %f \n",
				pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, azimuth, zenith);
}