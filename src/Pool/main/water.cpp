#include "water.h"
#include "camera.h"
#include "common.h"

const int WAVE_SIZE = 3;

water::water()
  : material_(float4(0.4f, 0.4f, 0.4f, 1.0f),
							float4(0.1f, 0.1f, 0.1f, 1.0f),
							float4(0.6f, 0.6f, 0.6f, 1.0f),
							0.9f) {
  render_prog_ = ShaderProgram("Water.vert", "Water.frag");
  sim_prog_ = ShaderProgram("Quad.vert", "WaterSimStep.frag");
  normal_prog_ = ShaderProgram("Quad.vert", "WaterNormal.frag");

  water_mesh_ = new SimpleMesh(render_prog_.program, 256, SimpleMesh::PLANE, 0.999999f);
  
  full_screen_quad_ = new FullScreenQuad();

  grid_size_x_ = 256;
  grid_size_z_ = 256;

  for(int i = 0; i < 3; i++)
    pos_tex_[i] = new RenderableTexture2D(GL_R32F, grid_size_x_, grid_size_z_);

	norm_tex_ = new RenderableTexture2D(GL_RGB16F, grid_size_x_, grid_size_z_);
  a_ = 0; 
  b_ = 1; 
  c_ = 2;

  GLint old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);
  glViewport(0, 0, grid_size_x_, grid_size_z_);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  for(int i = 0; i < 3; i++) {
    pos_tex_[i]->BeginRenderingToThisTexture();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pos_tex_[i]->EndRenderingToThisTexture();
  }
	norm_tex_->BeginRenderingToThisTexture();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  norm_tex_->EndRenderingToThisTexture();


  glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);

	float4x4 rotation_matrix, scale_matrix, translate_matrix;
  float4x4 transform_matrix1;

  glusScalef(scale_matrix.L(), 10, 10, 10);
  glusTranslatef(translate_matrix.L(), 0,0,0);
  glusMultMatrixf(object_matrix_.L(), translate_matrix.L(), scale_matrix.L());
	//glusMultMatrixf(object_matrix_.L(), scale_matrix.L(), translate_matrix.L());

	glusInverseMatrixf(inversed_object_matrix_.L(), object_matrix_.L());
}

void water::calm() {
	GLint old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);
  glViewport(0, 0, grid_size_x_, grid_size_z_);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  for(int i = 0; i < 3; i++) {
    pos_tex_[i]->BeginRenderingToThisTexture();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    pos_tex_[i]->EndRenderingToThisTexture();
  }
	glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
}

water::~water() {
  delete water_mesh_; water_mesh_ = NULL;
  delete full_screen_quad_; full_screen_quad_ = NULL;

  for(int i=0; i < 3; i++) {
    delete pos_tex_[i];
    pos_tex_[i] = NULL;
  }
}

void water::add_wave(GLUSfloat vec[3]) {
	GLUSfloat res[3];
	glusMultVectorf(res, inversed_object_matrix_.L(), vec);
	float x = res[2]*0.5f + 0.5f; //?
	float z = res[0]*0.5f + 0.5f;

  if (x < 0.0 || z < 0.0 || x > 1.0 || z > 1.0 || res[1] < -0.3) return;

  int pixelPosX = int( x*grid_size_x_ - 0.5f*float(WAVE_SIZE) + 0.5f);
  int pixelPosZ = int( z*grid_size_z_ - 0.5f*float(WAVE_SIZE) + 0.5f);

  if(pixelPosX < 0) pixelPosX = 0;
  if(pixelPosZ < 0) pixelPosZ = 0;

  if(pixelPosX >= grid_size_x_ - WAVE_SIZE) pixelPosX = grid_size_x_ - WAVE_SIZE;
  if(pixelPosZ >= grid_size_z_ - WAVE_SIZE) pixelPosZ = grid_size_z_ - WAVE_SIZE;

  float cpuData [WAVE_SIZE*WAVE_SIZE];

  for(int i = 0; i < WAVE_SIZE*WAVE_SIZE; i++)
    cpuData[i] = -0.05f;

  glBindTexture(GL_TEXTURE_2D, pos_tex_[0]->GetColorTexId());
  glTexSubImage2D(GL_TEXTURE_2D, 0, pixelPosX, pixelPosZ, WAVE_SIZE, WAVE_SIZE, GL_RED, GL_FLOAT, cpuData);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, pos_tex_[1]->GetColorTexId());
  glTexSubImage2D(GL_TEXTURE_2D, 0, pixelPosX, pixelPosZ, WAVE_SIZE, WAVE_SIZE, GL_RED, GL_FLOAT, cpuData);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void water::sim_step() {
  GLint old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);

  glViewport(0, 0, grid_size_x_, grid_size_z_);
  glDisable(GL_DEPTH_TEST);

  int temp = a_;
  a_ = b_;
  b_ = c_;
  c_ = temp;
 
  pos_tex_[c_]->BeginRenderingToThisTexture();
  
  glUseProgram(sim_prog_.program);

  setUniform(sim_prog_.program, "gridSizeX", grid_size_x_);
  setUniform(sim_prog_.program, "gridSizeZ", grid_size_z_);

  bindTexture(sim_prog_.program, WATER_POSITIONS_PREV, "inPositionsPrev",  pos_tex_[a_]->GetColorTexId());
  bindTexture(sim_prog_.program, WATER_POSITIONS, "inPositions",      pos_tex_[b_]->GetColorTexId());

  full_screen_quad_->Draw();

  pos_tex_[c_]->EndRenderingToThisTexture();

  glEnable(GL_DEPTH_TEST);
  glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
}

void water::update(const float4x4& view_projection, const camera& camera, const point_light& light) {
	// count normals
	GLint old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);

  glViewport(0, 0, grid_size_x_, grid_size_z_);
  glDisable(GL_DEPTH_TEST);

	norm_tex_->BeginRenderingToThisTexture();
	glUseProgram(normal_prog_.program);
	setUniform(normal_prog_.program, "objectMatrix", object_matrix_);
	setUniform(normal_prog_.program, "inversedObjectMatrix", inversed_object_matrix_);
	setUniform(normal_prog_.program, "gridSizeX",    grid_size_x_);
  setUniform(normal_prog_.program, "gridSizeZ",   grid_size_z_);
  bindTexture(normal_prog_.program, WATER_POSITIONS, "inPositions", pos_tex_[c_]->GetColorTexId());
	full_screen_quad_->Draw();
	norm_tex_->EndRenderingToThisTexture();
	glEnable(GL_DEPTH_TEST);
  glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);


	// draw water
	glUseProgram(render_prog_.program);
  setUniform(render_prog_.program, "cameraPosition", camera.get_pos());
	setUniform(render_prog_.program, "viewProjectionMatrix", view_projection);
  setUniform(render_prog_.program, "objectMatrix", object_matrix_);

	light.setup_shader(render_prog_.program);
	material_.setup_shader(render_prog_.program); 
	setUniform(render_prog_.program, "gridSizeX", grid_size_x_);
  setUniform(render_prog_.program, "gridSizeZ", grid_size_z_);
  bindTexture(render_prog_.program, WATER_POSITIONS, "inPositions", pos_tex_[c_]->GetColorTexId());
	bindTexture(render_prog_.program, WATER_NORMALS, "inNormals", norm_tex_->GetColorTexId());

	//glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	water_mesh_->Draw(GL_TRIANGLES);

	glDisable(GL_BLEND); 
}
