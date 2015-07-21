#include "../vsgl3/glHelper.h"
#include "../vsgl3/LiteMath.h"
#include "caustics.h"
#include "camera.h"
#include "common.h"

photon_mesh::photon_mesh(GLuint a_programId, int photon_map_size)
	: photon_map_size_(photon_map_size)
	, num_photons_(photon_map_size*photon_map_size)
	, num_indices_(photon_map_size*photon_map_size)
{
  // заполнить данные

	GLfloat* photons = new GLfloat[num_photons_*2];
	int k = 0;//, m = 0;
	int photon_map_size_half = photon_map_size >> 1;

	for (int i = -photon_map_size_half; i < photon_map_size_half; i += 1)
		for (int j = -photon_map_size_half; j < photon_map_size_half; j += 1)
		{
			photons[k] = GLfloat(j) / GLfloat(photon_map_size_half);
			photons[k + 1] = GLfloat(i) / GLfloat(photon_map_size_half);
			k += 2;
		}
	m_vertexPosBufferObject = -1;
  m_vertexArrayObject     = -1;
  m_vertexPosLocation = 0; // simple layout, assume have only positions at location = 0

  glGenBuffers(1, &m_vertexPosBufferObject);                                                CHECK_GL_ERRORS;
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexPosBufferObject);                                   CHECK_GL_ERRORS;
  glBufferData(GL_ARRAY_BUFFER, num_photons_ * 2 * sizeof(GLfloat), (GLfloat*) photons, GL_STATIC_DRAW);    CHECK_GL_ERRORS;

  glGenVertexArrays(1, &m_vertexArrayObject);                                               CHECK_GL_ERRORS;
  glBindVertexArray(m_vertexArrayObject);                                                   CHECK_GL_ERRORS;

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexPosBufferObject);                    CHECK_GL_ERRORS;                   
  glEnableVertexAttribArray(m_vertexPosLocation);                            CHECK_GL_ERRORS;
  glVertexAttribPointer(m_vertexPosLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);   CHECK_GL_ERRORS;

  glBindVertexArray(0);

	delete [] photons;
}


void photon_mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject); CHECK_GL_ERRORS;
  glDrawArrays(GL_POINTS, 0, num_photons_ );  CHECK_GL_ERRORS; 
}


photon_mesh::~photon_mesh()
{

  if(m_vertexPosBufferObject)
  {
    glDeleteBuffers(1, &m_vertexPosBufferObject);
    m_vertexPosBufferObject = 0;
  }

  if(m_vertexArrayObject)
  {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    m_vertexArrayObject = 0;
  }
}


caustics::caustics() {		
	caustics_prog_ = ShaderProgram("Caustics.vert", "Caustics.frag");
	photon_mesh_ = new photon_mesh(caustics_prog_.program, 256);

	for(int i = 0; i < 5; ++i) {
		caustics_tex_[i] = new RenderableTexture2D(GL_RGBA8, 256, 256);
		caustics_tex_[i]->BeginRenderingToThisTexture();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		caustics_tex_[i]->EndRenderingToThisTexture();
	}
}

caustics::~caustics() {
	delete photon_mesh_; photon_mesh_ = NULL;

	for(int i = 0; i < 5; ++i) {
		delete caustics_tex_[i]; caustics_tex_[i] = NULL;
	}
}

void caustics::update(const float4x4 water_object_matrix, 
		const float4x4 pool_inversed_object_matrix, float4 light_position, 
		GLuint water_positions_tex_id, GLuint water_normals_tex_id, float3 wall_coord, int num_wall) {
	glUseProgram(caustics_prog_.program);
	
	GLint old_viewport[4];
  glGetIntegerv(GL_VIEWPORT, old_viewport);

  glViewport(0, 0, photon_mesh_->get_photon_map_size(), photon_mesh_->get_photon_map_size());
  glDisable(GL_DEPTH_TEST);
	

	caustics_tex_[num_wall]->BeginRenderingToThisTexture();

	setUniform(caustics_prog_.program, "lightPosition", light_position);
	setUniform(caustics_prog_.program, "waterObjectMatrix", water_object_matrix);
	setUniform(caustics_prog_.program, "poolInversedObjectMatrix", pool_inversed_object_matrix);
	setUniform(caustics_prog_.program, "wallCoord", wall_coord);

	bindTexture(caustics_prog_.program, WATER_POSITIONS, "waterPositions", water_positions_tex_id);
	bindTexture(caustics_prog_.program, WATER_NORMALS, "waterNormals", water_normals_tex_id);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	photon_mesh_->draw();

	glDisable(GL_BLEND); 
  
	caustics_tex_[num_wall]->EndRenderingToThisTexture();
	
	glEnable(GL_DEPTH_TEST);
  glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
}
