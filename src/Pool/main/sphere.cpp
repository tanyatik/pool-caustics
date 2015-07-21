#include "sphere.h"
#include "common.h"

sphere::sphere() 
: material_(float4(0.45f, 0.45f, 0.45f, 1.0f),
							float4(0.1f, 0.1f, 0.1f, 1.0f),
							float4(0.01f, 0.01f, 0.01f, 1.0f),
	 						0.01f) {
	render_prog_ = ShaderProgram("Sphere.vert", "Sphere.frag");
	sphere_mesh_ = new SimpleMesh(render_prog_ .program, 256, SimpleMesh::SPHERE, 1.0f);

	sphere_tex_ = new Texture2D("sphere_texture.bmp");

	float4x4 rotationMatrix, scaleMatrix, translateMatrix;
	float4x4 transformMatrix1, transformMatrix2;

  glusRotateRzRyRxf(rotationMatrix.L(), 0.0f, 0.0f, -90.0f);
	glusScalef(scaleMatrix.L(), 1.0, 1.0, 1.0);
	glusTranslatef(translateMatrix.L(), 0.0, -5.0, 0.0);
	glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
	glusMultMatrixf(object_matrix_.L(), translateMatrix.L(), transformMatrix1.L());
}

sphere::~sphere() {
	delete sphere_mesh_; sphere_mesh_ = NULL;
	delete sphere_tex_; sphere_tex_ = NULL;
}

void sphere::update(const float4x4& view_projection, const camera& camera, const point_light& light) {
	glUseProgram(render_prog_.program);

	setUniform(render_prog_.program, "cameraPosition", camera.get_pos());
	setUniform(render_prog_.program, "viewProjectionMatrix", view_projection);

	setUniform(render_prog_.program, "objectMatrix", object_matrix_);

	light.setup_shader(render_prog_.program);
	material_.setup_shader(render_prog_.program);

	bindTexture(render_prog_.program, SPHERE, "sphereTex", sphere_tex_->GetColorTexId());

	sphere_mesh_->Draw(GL_TRIANGLES);
}