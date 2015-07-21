#pragma once 

#include <GL/glew.h>
#include <GL/glus.h>

#include "camera.h"

struct photon_mesh
{
  enum MESH_TYPE{SPHERE,CUBE,CUBE_OPEN,PLANE,TORUS};
 
  photon_mesh(GLuint a_programId, int photonMapSize);
  ~photon_mesh();

  void draw();
	int get_photon_map_size() const { return photon_map_size_; }

protected:
  photon_mesh(const photon_mesh& ths) {}
  photon_mesh& operator=(const photon_mesh& rhs) {return *this;}

  GLuint m_vertexPosBufferObject;
  GLuint m_vertexPosLocation;

  GLuint m_indexBufferObject;

  GLuint m_vertexArrayObject;
	int photon_map_size_;
	int num_photons_;
	int num_indices_;
};


class caustics
{
public:
  caustics();
  ~caustics();

	void update(const float4x4 water_object_matrix, const float4x4 water_inversed_object_matrix,
			float4 light_position, GLuint water_positions_tex_id, 
			GLuint water_normals_tex_id, float3 wall_coord, int num_wall);

  GLuint get_render_program() { return caustics_prog_.program; }
	float4x4 get_object_matrix() const;
	float4x4 get_inversed_object_matrix() const;
	GLuint get_caustics_tex(int num) const { return caustics_tex_[num]->GetColorTexId(); }
protected:
  caustics(const caustics& rhs) {}
  caustics& operator=(const caustics& rhs) {return *this;}

	void draw();

	ShaderProgram caustics_prog_;
	RenderableTexture2D* caustics_tex_ [5];

	photon_mesh* photon_mesh_;
};

