#include <GL\glew.h>
#include "GL\glus.h"
#include "..\vsgl3\glHelper.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <windows.h>

#include "scene.h"

scene g_scene;

void set_vsync(bool sync) {
  typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

  if(wglSwapIntervalEXT) {
    wglSwapIntervalEXT(sync);
	}
}

void print_gl_info() {
  CHECK_GL_ERRORS;
  std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GPU Name  : " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL_VER    : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL_VER  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

GLUSboolean init(GLUSvoid) {
  try {
    print_gl_info();
    set_vsync(1);
		g_scene.init();
		return GLUS_TRUE;
  }
  catch(std::runtime_error e) {
    std::cerr << e.what() << std::endl;
    exit(-1);
  }
  catch(...) {
    std::cerr << "Unexpected Exception (init)!" << std::endl;
    exit(-1);
  }
}

GLUSvoid reshape(GLUSuint width, GLUSuint height) {
	g_scene.reshape(width, height);
}

GLUSvoid mouse(GLUSboolean pressed, GLUSuint button, GLUSuint x, GLUSuint y) {
  g_scene.mouse_press(pressed, button, x, y);
}

GLUSvoid mouse_move(GLUSuint button, GLUSint x, GLUSint y) {
  g_scene.mouse_move(button, x, y);
}

GLUSvoid keyboard(GLUSboolean pressed, GLUSuint key) {
	g_scene.keyboard(pressed, key);
}

GLUSboolean update(GLUSfloat time)
{
  try {
		g_scene.update(time);
    return GLUS_TRUE;
  }
  catch(std::runtime_error e) {
    std::cerr << e.what() << std::endl;
    exit(-1);
  }
  catch(...) {
    std::cerr << "Unexpected Exception(render)!" << std::endl;
    exit(-1);
  }
}

GLUSvoid shutdown(GLUSvoid) {
}

int main(int argc, char* argv[]) {
	glusInitFunc(init);
	glusReshapeFunc(reshape);
	glusUpdateFunc(update);
	glusTerminateFunc(shutdown);
  glusMouseFunc(mouse);
  glusMouseMoveFunc(mouse_move);
  glusKeyFunc(keyboard);

	glusPrepareContext(3, 0, GLUS_FORWARD_COMPATIBLE_BIT);

	if (!glusCreateWindow("Shallow Water Template Sample", 640, 480, GLUS_FALSE)) {
		printf("Could not create window!");
		return -1;

	}

	glewExperimental = GL_TRUE;
  GLenum err=glewInit();
  if(err!=GLEW_OK) {
    sprintf("glewInitError", "Error: %s\n", glewGetErrorString(err));
    return -1;
  }
  glGetError();
  
	if (!glewIsSupported("GL_VERSION_3_0")) {
		printf("OpenGL 3.0 not supported.");

		glusDestroyWindow();
		return -1;
	}

	glusRun();

	return 0;
}
