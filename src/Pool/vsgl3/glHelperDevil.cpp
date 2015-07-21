#include "glHelper.h"
#include <sstream>

#include <il/il.h>
#include <il/ilu.h>

#pragma comment(lib, "devil.lib")
#pragma comment(lib, "ilu.lib")

static bool openILInitialised = false;


GLuint CreateGLTextureFromFile(const std::string& a_fileName)
{
  if(!openILInitialised)
  {
    ilInit();
    iluInit();
    openILInitialised = true;
  }

  ILuint imageName = ilGenImage();
  ilBindImage(imageName);

  if(!ilLoadImage(a_fileName.c_str()))
  {
    fprintf(stderr, "image %s don't exists or it has unknown format\n", a_fileName.c_str());
    ilDeleteImages(1, &imageName); 
    return -1;
  }

  ILinfo ImageInfo;
  iluGetImageInfo(&ImageInfo);
  if( ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT )
    iluFlipImage();

  int w   = ilGetInteger(IL_IMAGE_WIDTH);
  int h   = ilGetInteger(IL_IMAGE_HEIGHT);
  int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  int fmt = ilGetInteger(IL_IMAGE_FORMAT);

  int numChannels = 4; 

  // conversions to 4 component texture
  //
  if(bpp < 4)
  {
    if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
    {
      fprintf(stderr, "can't convert image, input bpp = %d\n", bpp);
      ilDeleteImages(1, &imageName); 
      return -1;
    }

    bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  }
  else if (bpp > 4 && bpp != 8 && bpp <= 16)
  {
    if(!ilConvertImage(IL_RGBA, IL_FLOAT))
    {
      fprintf(stderr, "can't convert image, input bpp = %d\n", bpp);
      ilDeleteImages(1, &imageName); 
      return -1;
    }

    bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  }

  GLuint internalFormat = GL_RGBA;
  GLuint inputDataType  = GL_UNSIGNED_BYTE;
  //
  //
  if(bpp == 8)
  {
    internalFormat = GL_RGBA16F;
    inputDataType  = GL_HALF_FLOAT;
  }
  else if(bpp == 16)
  {
    internalFormat = GL_RGBA32F;
    inputDataType  = GL_FLOAT;
  }

  char* data = new char [w*h*bpp*numChannels];

  ilCopyPixels(0,0,0,w,h,bpp,IL_RGBA,inputDataType,data);
  ilDeleteImages(1, &imageName);

  GLuint textureId;
  glGenTextures(1, &textureId); CHECK_GL_ERRORS;
  glBindTexture(GL_TEXTURE_2D, textureId); CHECK_GL_ERRORS;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  CHECK_GL_ERRORS;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  CHECK_GL_ERRORS;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  CHECK_GL_ERRORS;

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, internalFormat, inputDataType, data); CHECK_GL_ERRORS;
  glGenerateMipmap(GL_TEXTURE_2D);

  delete [] data;

  return textureId;
}


// WARNING! Values below zero will be clamped by DevIL to 0!
//
void SaveGLTextureToFile(const std::string& a_fileName, GLuint a_texId, GLenum a_format, int mipLevel)
{
 
  if(!openILInitialised)
  {
    ilInit();
    iluInit();
    openILInitialised = true;
  }

  unsigned int devilFormat = IL_BYTE;
  unsigned int sizeOfFormat = 1;

  switch(a_format)
  {
  case GL_BYTE: devilFormat = IL_BYTE; sizeOfFormat=1; break;
  case GL_UNSIGNED_BYTE:  devilFormat = IL_UNSIGNED_BYTE; sizeOfFormat=1; break;
  case GL_INT:   devilFormat = IL_INT; sizeOfFormat=4; break;
  case GL_UNSIGNED_INT:   devilFormat = IL_UNSIGNED_INT; sizeOfFormat=4; break;
  case GL_UNSIGNED_SHORT: devilFormat = IL_UNSIGNED_SHORT; sizeOfFormat=2; break;
  case GL_SHORT: devilFormat = IL_SHORT; sizeOfFormat=2; break;
  case GL_FLOAT: devilFormat = IL_FLOAT; sizeOfFormat=4; break;
  case GL_HALF_FLOAT: devilFormat = IL_HALF; sizeOfFormat=2; break;
  
  // actually you may use this only, because devIL's formats have same the same defines than OpenGL formats ()
  // i.e. GL_BYTE = IL_BYTE, GL_UNSIGNED_BYTE = IL_UNSIGNED_BYTE and e.t.c
  //
  default : 
    devilFormat = a_format; 
    break;
  };

  sizeOfFormat *= 4; // we have 4 channels

  // copy data to CPU
  GLint w = 0, h = 0;
  glBindTexture(GL_TEXTURE_2D, a_texId); CHECK_GL_ERRORS;         
  glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, &h);


  void* data = new char [sizeOfFormat*w*h];
  glGetTexImage(GL_TEXTURE_2D, mipLevel, GL_RGBA, a_format, data); CHECK_GL_ERRORS;


  // save CPU data to file
  ILenum Error;
  while ((Error = ilGetError()) != IL_NO_ERROR);

  ILuint imageName = ilGenImage();
  ilBindImage(imageName);

  ilTexImage(w,h,1,4,IL_RGBA,devilFormat,data);
  delete [] data;

  ilEnable(IL_FILE_OVERWRITE);
  ilSaveImage(a_fileName.c_str());

  while ((Error = ilGetError()) != IL_NO_ERROR)
    fprintf(stderr,"SaveImageToFile; \n OpenIL error, can save image: %d: %s\n", Error, iluErrorString(Error));

  ilDeleteImages(1, &imageName); 
  ilDisable(IL_FILE_OVERWRITE);
}

