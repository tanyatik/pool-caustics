/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __glus_h_
#define __glus_h_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif


#ifndef GLUSAPIENTRY
#define GLUSAPIENTRY
#endif
#ifndef GLUSAPIENTRYP
#define GLUSAPIENTRYP GLUSAPIENTRY *
#endif
#ifndef GLUSAPI
#define GLUSAPI extern
#endif

typedef unsigned int GLUSenum;
typedef unsigned char GLUSboolean;
typedef unsigned int GLUSbitfield;
typedef signed char GLUSbyte;
typedef short GLUSshort;
typedef int GLUSint;
typedef int GLUSsizei;
typedef unsigned char GLUSubyte;
typedef unsigned short GLUSushort;
typedef unsigned int GLUSuint;
typedef float GLUSfloat;
typedef float GLUSclampf;
typedef double GLUSdouble;
typedef double GLUSclampd;
typedef char GLUSchar;

#ifdef __cplusplus
#define GLUSvoid void
#else
typedef void GLUSvoid;
#endif

#define GLUS_ALPHA 0x1906
#define GLUS_RGB 0x1907
#define GLUS_RGBA 0x1908

#define PIf		3.1415926535897932384626433832795f
				
typedef struct _GLUStextfile
{
	GLUSchar* text;

	GLUSint	length;

} GLUStextfile;

typedef struct _GLUStgaimage
{
	GLUSushort	width;

	GLUSushort	height;

	GLUSubyte*	data;

	GLUSenum 	format;

} GLUStgaimage;

typedef struct _GLUSshaderprogram
{
	GLUSuint	program;

	GLUSuint	vertex;

	GLUSuint	control;

	GLUSint		evaluation;

	GLUSuint	geometry;

	GLUSuint	fragment;

} GLUSshaderprogram;

typedef struct _GLUSshape
{
	GLUSfloat*	vertices;
	
	GLUSfloat*	normals;
	
	GLUSfloat*	tangents;
	
	GLUSfloat*  texCoords;
	
	GLUSuint*	indices;

	GLUSuint	numberVertices;

	GLUSuint	numberIndices;

} GLUSshape;

#define GLUS_TRUE	1
#define GLUS_FALSE	0

#define GLUS_BACKWARD_COMPATIBLE_BIT	0x0000
#define GLUS_FORWARD_COMPATIBLE_BIT		0x0002

GLUSAPI GLUSvoid GLUSAPIENTRY glusInitFunc( GLUSboolean (*glusNewInit)(GLUSvoid) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusReshapeFunc( GLUSvoid (*glusNewReshape)(GLUSuint width, GLUSuint height) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusUpdateFunc( GLUSboolean (*glusNewUpdate)(GLUSfloat time) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusTerminateFunc( GLUSvoid (*glusNewTerminate)(GLUSvoid) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusPrepareContext(GLUSuint major, GLUSuint minor, GLUSint flags);

GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateWindow(const GLUSchar* title, GLUSuint width, GLUSuint height, GLUSboolean fullscreen);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid);

GLUSAPI GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusKeyFunc( GLUSvoid (*glusNewKey)(GLUSboolean pressed, GLUSuint key) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseFunc( GLUSvoid (*glusNewMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseWheelFunc( GLUSvoid (*glusNewMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseMoveFunc( GLUSvoid (*glusNewMouseMove)(GLUSuint buttons, GLUSint xPos, GLUSint yPos) );

//

GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTextFile(const GLUSchar* filename, GLUStextfile* textfile);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTextFile(GLUStextfile* textfile);

GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage);
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadBmpImage(const GLUSchar* filename, GLUStgaimage* tgaimage);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage);

//

GLUSAPI GLUSfloat GLUSAPIENTRY glusAmountf(const GLUSfloat vector[3]);
		
GLUSAPI GLUSvoid GLUSAPIENTRY glusNormalizef(GLUSfloat vector[3]);

GLUSAPI GLUSfloat GLUSAPIENTRY glusDotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCrossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusOrthof(GLUSfloat result[16], GLUSfloat left, GLUSfloat right, GLUSfloat bottom, GLUSfloat top, GLUSfloat nearVal, GLUSfloat farVal);

GLUSAPI GLUSvoid GLUSAPIENTRY glusFrustumf(GLUSfloat result[16], GLUSfloat left, GLUSfloat right, GLUSfloat bottom, GLUSfloat top, GLUSfloat nearVal, GLUSfloat farVal);

GLUSAPI GLUSvoid GLUSAPIENTRY glusPerspectivef(GLUSfloat result[16], GLUSfloat fovy, GLUSfloat aspect, GLUSfloat zNear, GLUSfloat zFar);

GLUSAPI GLUSvoid GLUSAPIENTRY glusLookAtf(GLUSfloat result[16], GLUSfloat eyeX, GLUSfloat eyeY, GLUSfloat eyeZ, GLUSfloat centerX, GLUSfloat centerY, GLUSfloat centerZ, GLUSfloat upX, GLUSfloat upY, GLUSfloat upZ);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusLoadIdentityf(GLUSfloat matrix[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCopyMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultMatrixf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16]);

GLUSAPI GLUSboolean GLUSAPIENTRY glusInverseMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusTransposef(GLUSfloat matrix[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusTranslatef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusRotatef(GLUSfloat matrix[16], GLUSfloat angle, GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRzRyRxf(GLUSfloat matrix[16], GLUSfloat anglex, GLUSfloat angley, GLUSfloat anglez);

GLUSAPI GLUSvoid GLUSAPIENTRY glusScalef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat vector[4]);

//

GLUSAPI GLUSboolean GLUSAPIENTRY glusBuildProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);

GLUSAPI GLUSboolean GLUSAPIENTRY glusCompileProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);

GLUSAPI GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreatePlanef(GLUSshape* shape, GLUSfloat halfExtend);
GLUSAPI GLUSvoid GLUSAPIENTRY glusCreatePlaneSlicedf(GLUSshape* shape, GLUSfloat halfExtend, GLUSuint numberSlices);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateRectangularPlanef(GLUSshape* shape, GLUSfloat horizontalExtend, GLUSfloat verticalExtend);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateCubef(GLUSshape* shape, GLUSfloat halfExtend);
GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateCubeOpenf(GLUSshape* shape, GLUSfloat halfExtend);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateSpheref(GLUSshape* shape, GLUSfloat radius, GLUSuint numberSlices);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateTorusf(GLUSshape* shape, GLUSfloat innerRadius, GLUSfloat outerRadius, GLUSuint numSides, GLUSuint numFaces);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyShapef(GLUSshape* shape);

GLUSAPI GLUSvoid GLUSAPIENTRY glusPrintErrorMsg(GLenum);

#ifdef __cplusplus
}
#endif


// проверка на ошибки OpenGL
//#define OGL_CHECK_FOR_ERRORS \
//{ \
//  GLenum openGLError; \
//  if ((openGLError = glGetError()) != GL_NO_ERROR) \
//  { \
//    glusPrintErrorMsg(openGLError); \
//    fprintf(stderr, "file %s, \nline %d\n", __FILE__, __LINE__); \
//  } \
//} \

// безопасный вызов функции OpenGL
//#define OGL_SAFE_CALL(expression) \
//{ \
//  expression; \
//  GLenum openGLError; \
//  if ((openGLError = glGetError()) != GL_NO_ERROR) \
//  fprintf(stderr, "OpenGL call failed \"" #expression "\" error %d\n", (int)openGLError); \
//}





#endif /*__glus_h_*/
