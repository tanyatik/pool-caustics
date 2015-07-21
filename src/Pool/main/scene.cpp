#include "common.h"
#include "scene.h"
#include "sphere.h"

scene::scene()
	: width_(0)
	, height_(0)
	, water_(NULL)
	, pool_(NULL)
	, caustics_(NULL) 
	, camera1_(-20.0, 7.0, -20.0, 0.0, 0.0, 0.0)
	, camera2_(0.0, -4.0, 0.0, 0.0, -4.0, 10.0)
	, camera3_(-10.0, 0.5, 10.0, 0.0, 0.5, 0.0)
	, current_camera_(&camera1_) {	
}

scene::~scene() {
	delete water_; water_ = NULL;
	delete pool_; pool_ = NULL;
	delete caustics_; caustics_ = NULL;
	delete sphere_; sphere_ = NULL;
}

void scene::init() {
	water_ = new water();
	pool_ = new pool(-6.0, 10, -10, -10, 10);
	caustics_ = new caustics();
	sphere_ = new sphere();
}

void scene::reshape(GLUSuint width, GLUSuint height) {
	glViewport(0, 0, width, height);
  width_ = width;
  height_ = height;

  glusPerspectivef(projection_matrix_.L(), 45.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);  // Calculate the projection matrix
}

void scene::mouse_press(GLUSboolean pressed, GLUSuint button, GLUSuint x, GLUSuint y) {
  if(!pressed)
    return;

  if (button & 1) {
    current_camera_->set_old_pos(x, y);
  }

  if (button & 4)	{
	  add_wave(float(x), float(y));
  }
}

void scene::mouse_move(GLUSuint button, GLUSint x, GLUSint y) {
  if(button & 1) {
	  current_camera_->rotate(x, y);
  }

  if(button & 4) {
	  add_wave(float(x), float(y));
  }
}

void scene::keyboard(GLUSboolean pressed, GLUSuint key) {
	if(!pressed)
		return;
  switch(key) {
		case 'W':
			current_camera_->step_forward();
			break;
		case 'S':
			current_camera_->step_backward();
			break;
		case 'A':
			current_camera_->step_left();
			break;
		case 'D':
			current_camera_->step_right();
			break;
		case 'I': 
			current_camera_->step_up();
			break;
		case 'K':
			current_camera_->step_down();
			break;
		case 112: //F1
			current_camera_ = &camera1_;
			break;
		case 113: //F2
			current_camera_ = &camera2_;
			break;
		case 114: //F3
			current_camera_ = &camera3_;
			break;
		case 'C':
			water_->calm();
			break;
		default:
			break;
	}
}

void scene::update(GLUSfloat time)
{
  static float elaspedTimeFromStart = 0;
  elaspedTimeFromStart += 10*time;

  float4x4 model;
	float4x4 view;
	float4x4 view_projection;
	float4x4 normal;

  glusLoadIdentityf(model.L()); 
	current_camera_->update(view.L());

  glusMultMatrixf(model_view_.L(), view.L(), model.L());
	glusMultMatrixf(view_projection.L(), projection_matrix_.L(), model_view_.L());
  
  glViewport(0, 0, width_, height_);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
	
	water_->sim_step();
	for(int num_wall = 0; num_wall < 5; ++num_wall) {
		caustics_->update(water_->get_object_matrix(), pool_->get_inversed_object_matrix(num_wall),
			light_.get_position(), water_->get_positions_tex(), water_->get_normals_tex(), pool_->get_wall_coord(num_wall), num_wall);
		pool_->update(view_projection, *current_camera_, light_, caustics_->get_caustics_tex(num_wall), num_wall);
	}
	sphere_->update(view_projection, *current_camera_, light_);
	water_->update(view_projection, *current_camera_, light_);
}

void scene::get_mouse_to_world_pos(float x, float y, GLUSfloat* pos_x, GLUSfloat* pos_y, GLUSfloat* pos_z)
{
    GLint viewport[4];
    GLdouble model_view[16];
    GLdouble projection[16];
    GLfloat win_x, win_y, win_z;

    glGetIntegerv( GL_VIEWPORT, viewport);
 
    win_x = x;
    win_y = (float)viewport[3] - y;
    glReadPixels( int(x), int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
	  for(int i = 0; i < 16; ++i) {
			model_view[i] = (GLdouble) model_view_.L() [i];
			projection[i] = (GLdouble) projection_matrix_.L() [i];
		}

		GLdouble tempX = 0.0, tempY = 0.0, tempZ = 0.0;
    gluUnProject( win_x, win_y, win_z, model_view, projection, viewport, &tempX, &tempY, &tempZ);

    *pos_x = GLfloat(tempX);
		*pos_y = GLfloat(tempY);
		*pos_z = GLfloat(tempZ);
}

void scene::add_wave(float x, float z) {
	GLUSfloat vec[3];
	get_mouse_to_world_pos(x, z, &vec[0], &vec[1], &vec[2]);
	water_->add_wave(vec);
}