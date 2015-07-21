#pragma once

//////////////////////////////////////////////////////////////////
// glHelper.h Author: Vladimir Frolov, 2011, Graphics & Media Lab.
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <string>

#include <GL/glew.h>
#include "LiteMath.h"

#pragma warning( disable : 4996)

#include "GL/glus.h"


struct GlusHelperRequireExt
{
  GlusHelperRequireExt() 
  {
    int n=0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);  
    for(int i=0;i<n;i++)
    {
      const char*  extensions = (const char*)glGetStringi(GL_EXTENSIONS,i);
      supportedExtensions[extensions] = true;
    }
  }

  void require(const std::string& extName)
  {
    if (supportedExtensions.find(extName) == supportedExtensions.end())
      throw std::runtime_error("unsupported extension: " + extName);
  }

  std::map<std::string, bool> supportedExtensions;
};

static std::string DecodeGLError(GLenum gl_error, int line, const char *file)
{
  std::ostringstream errMsg;

  //GLenum gl_error = glGetError();

  switch(gl_error)
  {
  case GL_INVALID_ENUM:
    errMsg << "GL_INVALID_ENUM in " << file << ", line " << line;
    break;

  case GL_INVALID_VALUE:
    errMsg << "GL_INVALID_VALUE in " << file << ", line " << line;
    break;

  case GL_INVALID_OPERATION:
    errMsg << "GL_INVALID_OPERATION in " << file << ", line " << line;
    break;

  case GL_STACK_OVERFLOW:
    errMsg << "GL_STACK_OVERFLOW in " << file << ", line " << line;
    break;

  case GL_STACK_UNDERFLOW:
    errMsg << "GL_STACK_UNDERFLOW in " << file << ", line " << line;
    break;

  case GL_OUT_OF_MEMORY:
    errMsg << "GL_OUT_OF_MEMORY in " << file << ", line " << line;
    break;

  case GL_TABLE_TOO_LARGE:
    errMsg << "GL_TABLE_TOO_LARGE in " << file << ", line " << line;
    break;

  case GL_NO_ERROR:
    errMsg << "GL_NO_ERROR in " << file << ", line " << line;
    break;

  default:
    errMsg << "UNKNOWN GL ERROR in " << file << ", line " << line;
    break;
  }

  return errMsg.str();
}

#pragma warning(disable:4996)
static void ThrowExceptionOnGLError(int line, const char *file)
{

  static char errMsg[512];

  GLenum gl_error = glGetError();

  if(gl_error == GL_NO_ERROR)
    return;

  switch(gl_error)
  {
  case GL_INVALID_ENUM:
    sprintf(errMsg, "GL_INVALID_ENUM file %s line %d\n", file, line);
    break;

  case GL_INVALID_VALUE:
    sprintf(errMsg, "GL_INVALID_VALUE file %s line %d\n",  file, line);
    break;

  case GL_INVALID_OPERATION:
    sprintf(errMsg, "GL_INVALID_OPERATION file %s line %d\n",  file, line);
    break;

  case GL_STACK_OVERFLOW:
    sprintf(errMsg, "GL_STACK_OVERFLOW file %s line %d\n",  file, line);
    break;

  case GL_STACK_UNDERFLOW:
    sprintf(errMsg, "GL_STACK_UNDERFLOW file %s line %d\n",  file, line);
    break;

  case GL_OUT_OF_MEMORY:
    sprintf(errMsg, "GL_OUT_OF_MEMORY file %s line %d\n",  file, line);
    break;

  case GL_TABLE_TOO_LARGE:
    sprintf(errMsg, "GL_TABLE_TOO_LARGE file %s line %d\n",  file, line);
    break;

  case GL_NO_ERROR:
    break;

  default:
    sprintf(errMsg, "Unknown error @ file %s line %d\n",  file, line);
    break;
  }

  if(gl_error != GL_NO_ERROR)
    throw std::runtime_error(errMsg);
}




#undef CHECK_GL_ERRORS
#define CHECK_GL_ERRORS ThrowExceptionOnGLError(__LINE__,__FILE__)


struct ShaderProgram //: public GLUSshaderprogram
{
  ShaderProgram();
  ShaderProgram(const std::string& vs_path);
  ShaderProgram(const std::string& vs_path, const std::string& ps_path);
  ShaderProgram(const std::string& vs_path, const std::string& gs_path, const std::string& ps_path);

  ~ShaderProgram();

  ShaderProgram& operator=(const ShaderProgram& a_prog);
  //ShaderProgram& operator=(const GLUSshaderprogram& a_glusProg);

  bool Link();

  GLuint	program;
  GLuint	vertex;
  GLuint	control;
  GLint		evaluation;
  GLuint	geometry;
  GLuint	fragment;

protected:

  bool Compile(const char** vert, const char** control, const char** evaluation, const char** geometry, const char** fragment);

  mutable int m_refCounter;
};



struct FullScreenQuad
{
  FullScreenQuad();
  virtual ~FullScreenQuad();
  
  void Draw();
  inline GLuint GetVBO() const { return m_vertexPosBufferObject;}

protected:

  FullScreenQuad(const FullScreenQuad& ths) {}
  FullScreenQuad& operator=(const FullScreenQuad& rhs) {return *this;}

  GLuint m_vertexPosBufferObject;
  GLuint m_vertexPosLocation;
  GLuint m_vertexArrayObject;
};



struct SimpleMesh
{
  enum MESH_TYPE{SPHERE,CUBE,CUBE_OPEN,PLANE,TORUS};
 
  SimpleMesh(GLuint a_programId, int a_slicesNum, int a_meshType = SPHERE, float a_halfSize = 1.0f);
  ~SimpleMesh();

  void Draw(GLuint a_primType = GL_TRIANGLES);

protected:
  
  SimpleMesh(const SimpleMesh& ths) {}
  SimpleMesh& operator=(const SimpleMesh& rhs) {return *this;}

  GLuint m_vertexPosBufferObject;
  GLuint m_vertexPosLocation;

  GLuint m_vertexNormBufferObject;
  GLuint m_vertexNormLocation;

  GLuint m_vertexTexCoordsBufferObject;
  GLuint m_vertexTexCoordsLocation;

  GLuint m_indexBufferObject;

  GLuint m_vertexArrayObject;

  GLUSshape m_glusShape;

};



struct Texture2D {
  Texture2D(GLenum format, GLsizei width, GLsizei height, const void* data);
  Texture2D(const std::string& file_name);

  ~Texture2D();

  GLuint GetColorTexId() const {return m_colorTexture;}

protected:

  GLuint m_colorTexture;
};


struct RenderableTexture2D {
  RenderableTexture2D(GLenum format, GLsizei width, GLsizei height);
  ~RenderableTexture2D();

  void BeginRenderingToThisTexture();
  void EndRenderingToThisTexture();

  GLuint GetColorTexId() const {return m_colorTexture;}
  GLuint GetDepthTexId() const {return m_depthTexture;}

  void BuildMipMapChain();

protected:

  static GLuint TextureCreateEmpty(GLint internalFormat, GLenum format, GLsizei width, GLsizei height);

  GLuint m_colorTexture;
  GLuint m_depthTexture;
  GLuint m_fbo;
};




void bindTexture(GLuint program, GLint unit, const GLchar *name, GLuint texture);
void bindTextureBuffer(GLuint program, GLint unit, const GLchar *name, GLuint texture);


#define setUniform(prog, name, value) setUniformF(prog, name, value, __LINE__, __FILE__)
#define setUniformArray(prog, name, value, count) setUniformArrayF(prog, name, value, count, __LINE__, __FILE__)

template<class T> 
static void setUniformF(GLuint a_program, const std::string& a_name, T value, int line, char* file) { bindUniform_UNDEFINED_FOR_THIS_TYPE_T; }

template<class T> 
static void setUniformArrayF(GLuint a_program, const std::string& a_name, T* p_array, int count, int line, char* file) { bindUniform_Array_UNDEFINED_FOR_THIS_TYPE; }


template<> 
static void setUniformArrayF<float>(GLuint a_program, const std::string& a_name, float* p_array, int count, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform1fv(location, count, p_array); 

  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformArrayF<float2>(GLuint a_program, const std::string& a_name, float2* p_array, int count, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform2fv(location, count, (float*)p_array); 

  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformArrayF<float3>(GLuint a_program, const std::string& a_name, float3* p_array, int count, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform3fv(location, count, (float*)p_array); 

  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformF<int>(GLuint a_program, const std::string& a_name, int value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform1i(location, value);
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformF<float>(GLuint a_program, const std::string& a_name, float value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform1f(location, value); 
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformF<float2>(GLuint a_program, const std::string& a_name, float2 value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform2f(location, value.x, value.y); 
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}

template<> 
static void setUniformF<float3>(GLuint a_program, const std::string& a_name, float3 value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform3f(location, value.x, value.y, value.z); 
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}

template<> 
static void setUniformF<float4>(GLuint a_program, const std::string& a_name, float4 value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniform4f(location, value.x, value.y, value.z, value.w);
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}


template<> 
static void setUniformF<float4x4>(GLuint a_program, const std::string& a_name, float4x4 value, int line, char* file)      
{ 
  GLint location = glGetUniformLocation(a_program, a_name.c_str());
  if(location >= 0)
    glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&value);
  
  GLenum error = glGetError();
  if(error!=GL_NO_ERROR)
    throw std::runtime_error(DecodeGLError(error, line, file) + " around shader variable " + a_name);
}




void   SaveGLTextureToFile(const std::string& a_fileName, GLuint a_texId, GLenum a_format, int mipLevel = 0);
GLuint CreateGLTextureFromFile(const std::string& a_fileName);

