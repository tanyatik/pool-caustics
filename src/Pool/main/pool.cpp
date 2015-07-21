#include "../vsgl3/glHelper.h"
#include "pool.h"
#include "camera.h"
#include "light.h"
#include "common.h"

pool::pool(GLfloat floor_height, GLfloat left_wall_coord, GLfloat right_wall_coord, GLfloat front_wall_coord, GLfloat back_wall_coord)
	: material_(float4(0.45f, 0.45f, 0.45f, 1.0f),
							float4(0.1f, 0.1f, 0.1f, 1.0f),
							float4(0.01f, 0.01f, 0.01f, 1.0f),
	 						0.01f) {		
	render_prog_ = ShaderProgram("Room.vert", "Room.frag");
	pool_mesh_ = new SimpleMesh(render_prog_ .program, 256, SimpleMesh::PLANE, 1.0f);

	floor_tex_ = new Texture2D("pool_texture.bmp");

	wall_coord_[FLOOR]      = float3(0, floor_height, 0);
	wall_coord_[LEFT_WALL]  = float3(left_wall_coord, 0, 0);
	wall_coord_[RIGHT_WALL] = float3(right_wall_coord, 0, 0);
	wall_coord_[FRONT_WALL] = float3(0, 0, front_wall_coord);
	wall_coord_[BACK_WALL]  = float3(0, 0, back_wall_coord);

	init_object_matrix(FLOOR,                                  10, 10,                    10,                  0,       floor_height,                0,   0,   0);
	init_object_matrix(LEFT_WALL,  GLfloat(abs(floor_height / 2)), 10,                    10,    left_wall_coord, GLfloat(floor_height / 2.0),                0,   0,  90);
	init_object_matrix(RIGHT_WALL, GLfloat(abs(floor_height / 2)), 10,                    10,   right_wall_coord, GLfloat(floor_height / 2.0),                0,   0, -90);
	init_object_matrix(FRONT_WALL,                             10, 10, abs(floor_height / 2),                  0, GLfloat(floor_height / 2.0), front_wall_coord,  90,   0);
	init_object_matrix(BACK_WALL,                              10, 10, abs(floor_height / 2),                  0, GLfloat(floor_height / 2.0),  back_wall_coord, -90,   0);
}

void pool::init_object_matrix(int num_obj, GLfloat scale_coeff_x, GLfloat scale_coeff_y, GLfloat scale_coeff_z,
															GLfloat translate_coeff_x, GLfloat translate_coeff_y, GLfloat translate_coeff_z,
															GLfloat rotate_coeff_x, GLfloat rotate_coeff_z) {
	float4x4 rotationMatrix, scaleMatrix, translateMatrix;
	float4x4 transformMatrix1, transformMatrix2;

  glusRotateRzRyRxf(rotationMatrix.L(), rotate_coeff_x, 0.0f, rotate_coeff_z);
	glusScalef(scaleMatrix.L(), scale_coeff_x, scale_coeff_y, scale_coeff_z);
	glusTranslatef(translateMatrix.L(), translate_coeff_x, translate_coeff_y, translate_coeff_z);
	glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
	glusMultMatrixf(object_matrix_[num_obj].L(), translateMatrix.L(), transformMatrix1.L());
	glusInverseMatrixf(inversed_object_matrix_[num_obj].L(), object_matrix_[num_obj].L());

	size_x_[num_obj] = (int) scale_coeff_x;
	size_z_[num_obj] = (int) scale_coeff_z;
}

pool::~pool() {
	delete pool_mesh_; pool_mesh_ = NULL;
	delete floor_tex_; floor_tex_ = NULL;
}

float3 pool::get_wall_coord(int num_wall) const {
	 return wall_coord_[num_wall]; 	 
}

float4x4 pool::get_inversed_object_matrix(int num_obj) const {
	return inversed_object_matrix_[num_obj];
}

void pool::update(const float4x4& view_projection, const camera& camera, const point_light& light, GLuint caustics_tex_id, int num_wall) {
	glUseProgram(render_prog_.program);

	setUniform(render_prog_.program, "cameraPosition", camera.get_pos());
	setUniform(render_prog_.program, "viewProjectionMatrix", view_projection);


	setUniform(render_prog_.program, "objectMatrix", object_matrix_[num_wall]);
	setUniform(render_prog_.program, "inversedObjectMatrix", inversed_object_matrix_[num_wall]);

	setUniform(render_prog_.program, "gridSizeX", 256);
	setUniform(render_prog_.program, "gridSizeZ", 256);
	setUniform(render_prog_.program, "sizeX", size_x_[num_wall]);
	setUniform(render_prog_.program, "sizeZ", size_z_[num_wall]);
	light.setup_shader(render_prog_.program);
	material_.setup_shader(render_prog_.program);

	bindTexture(render_prog_.program, CAUSTICS + num_wall, "caustics", caustics_tex_id);
  bindTexture(render_prog_.program, FLOOR_TILT, "floorTilt", floor_tex_->GetColorTexId());

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	pool_mesh_->Draw(GL_TRIANGLES);

	glDisable(GL_BLEND); 
	glDisable(GL_CULL_FACE);
}