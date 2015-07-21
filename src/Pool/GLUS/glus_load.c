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

#include "GL/glus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLUSboolean GLUSAPIENTRY glusLoadTextFile(const GLUSchar* filename, GLUStextfile* textfile)
{
	FILE* f;

	if (!filename || !textfile)
	{
		return GLUS_FALSE;
	}

	textfile->text = 0;

	textfile->length = -1;

	f = fopen(filename, "r");

	if (!f)
	{
		return GLUS_FALSE;
	}

	fseek(f, 0, SEEK_END);

	textfile->length = ftell(f);

	textfile->text = (GLUSchar*)malloc(textfile->length+10);

	if (!textfile->text)
	{
		textfile->length = -1;

		return GLUS_FALSE;
	}

	memset(textfile->text, 0, textfile->length+10);

	rewind(f);

	fread(textfile->text, 1, textfile->length, f);

	fclose(f);

	return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusDestroyTextFile(GLUStextfile* textfile)
{
	if (!textfile)
	{
		return;
	}

	if (textfile->text)
	{
		free(textfile->text);

		textfile->text = 0;
	}

	textfile->length = -1;
}

GLUSvoid glusSwapColorChannel(GLUSuint width, GLUSuint height, GLUSenum format, GLUSubyte* data)
{
	GLUSuint i;
	GLUSubyte temp;
	GLUSubyte bytesPerPixel = 3;
	
	if (format == GLUS_RGBA)
	{
		bytesPerPixel = 4;
	}
	
	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (i = 0; i < width*height*bytesPerPixel; i+=bytesPerPixel)
	{
		temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}
}

GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage)
{
	FILE*				file;
	GLUSubyte				type;
	GLUSubyte				bitsPerPixel;				

	// check, if we have a valid pointer
	if (!filename || !tgaimage)
	{
		return GLUS_FALSE;
	}

	// open filename in "read binary" mode
	file = fopen(filename, "rb");

	if (!file)
	{
		return GLUS_FALSE;
	}

 	// seek through the tga header, up to the type:
 	fseek(file, 2, SEEK_CUR);

 	// read the type
 	fread(&type, 1, 1, file);

	// check the type	
	if (type != 2 && type != 10 && type != 11)
	{
		fclose(file);
		return GLUS_FALSE;
	}

 	// seek through the tga header, up to the width/height:
 	fseek(file, 9, SEEK_CUR);
	
 	// read the width
 	fread(&tgaimage->width, 2, 1, file);
 
 	// read the height 
 	fread(&tgaimage->height, 2, 1, file);

	// read the bits per pixel
	fread(&bitsPerPixel, 1, 1, file);

	// check the pixel depth
	if (bitsPerPixel != 8 && bitsPerPixel != 24 && bitsPerPixel != 32)
	{
		fclose(file);
		return GLUS_FALSE;
	}
	else
	{	
		tgaimage->format = GLUS_ALPHA;	
		if (bitsPerPixel == 24)
		{
			tgaimage->format = GLUS_RGB;
		}
		else if (bitsPerPixel == 32)
		{
			tgaimage->format = GLUS_RGBA;
		}
	}

	// move file pointer to beginning of targa data
	fseek(file, 1, SEEK_CUR);

	// allocate enough memory for the targa  data
	tgaimage->data = (GLUSubyte*)malloc(tgaimage->width*tgaimage->height*bitsPerPixel/8);

	// verify memory allocation
	if (!tgaimage->data)
	{
		fclose(file);
		return GLUS_FALSE;
	}

	if (type == 2)
	{
		// read in the raw data
		fread(tgaimage->data, 1, tgaimage->width*tgaimage->height*bitsPerPixel/8, file);
	}
	else
	{
		// RLE encoded
		GLUSuint pixelsRead = 0;

		while (pixelsRead < (GLUSuint)tgaimage->width*tgaimage->height)
		{
			GLUSubyte amount;

			fread(&amount, 1, 1, file);

			if (amount & 0x80)
			{
				GLUSuint			i;
				GLUSuint			k;

				amount&=0x7F;

				amount++;

				// read in the rle data
				fread(&tgaimage->data[pixelsRead*bitsPerPixel/8], 1, bitsPerPixel/8, file);

				for (i = 1; i < amount; i++)
				{
					for (k = 0; k < (GLUSuint)bitsPerPixel/8; k++)
					{
						tgaimage->data[(pixelsRead+i)*bitsPerPixel/8+k] = tgaimage->data[pixelsRead*bitsPerPixel/8+k];
					}
				}
			}
			else
			{
				amount&=0x7F;

				amount++;
				
				// read in the raw data
				fread(&tgaimage->data[pixelsRead*bitsPerPixel/8], 1, (GLUSuint)amount*bitsPerPixel/8, file);				
			}

			pixelsRead += amount;
		}
	}

	// swap the color if necessary
	if (bitsPerPixel == 24 || bitsPerPixel == 32)
	{
		glusSwapColorChannel(tgaimage->width, tgaimage->height, tgaimage->format, tgaimage->data);
	}

	// close the file
	fclose(file);
	
	return GLUS_TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* размер заголовка BMP-файла */
#define BMP_SIZE_FILEHEADER 14 

/* размер информационного заголовка BMP-файла */
#define BMP_SIZE_INFOHEADER 40  
#define BMP_COLOR_BITS_24 24 

unsigned int uInt16Number(unsigned char buf[2])
{
  return (buf[1] << 8) | buf[0];
}

unsigned int uInt32Number(unsigned char buf[4])
{
  unsigned numb = buf[3];
  numb = (numb << 8) | buf[2];
  numb = (numb << 8) | buf[1];
  numb = (numb << 8) | buf[0];
  return numb;
}


int ReadFileHeader(FILE* f, int* bitmap_pos) 
{
  unsigned char header[BMP_SIZE_FILEHEADER];
  size_t numb = 0;  
  int offset = 0;

  if (fseek(f, 0, SEEK_SET))
    return 0;

  numb = fread(header, BMP_SIZE_FILEHEADER, 1, f);
  if (numb != 1)
    return 0;

  if (header[0] != 'B' || header[1] != 'M') 
    return 0;

  offset = uInt32Number(header + 10);

  numb = fread(header, 4, 1, f);
  if (numb != 1)
    return 0;

  if (uInt32Number(header) != 40)
    return 0;

  *bitmap_pos  = offset;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
GLUSboolean GLUSAPIENTRY glusLoadBmpImage(const GLUSchar* filename, GLUStgaimage* image)
{
  FILE* f; 
  int bitmap_pos;
  unsigned char buf[40]; 
  size_t numb;
  int x_res;
  int y_res;
  int n_bits;
  int compression;
  int size_image;
  int n_used_colors;

  /* открываем файл */
  f = fopen(filename, "rb");
  if (!f) return 0;

  /* читаем заголовок */
  if (!ReadFileHeader(f, &bitmap_pos))
  {
    fclose(f);
    return 0;
  }

  if (fseek(f, BMP_SIZE_FILEHEADER, SEEK_SET))
  {
    fclose(f);
    return 0;
  }

  numb = fread(buf, 40, 1, f);
  if (numb != 1)
  {
    fclose(f);
    return 0;
  }

  x_res = (int)uInt32Number(buf + 4);
  y_res = (int)uInt32Number(buf + 8);
  n_bits          = (int)uInt16Number(buf + 14);
  compression     = (int)uInt32Number(buf + 16);
  size_image      = (int)uInt32Number(buf + 20);
  n_used_colors   = (int)uInt32Number(buf + 32);

  /* читаем только полноцветные файлы */
  if (n_bits == BMP_COLOR_BITS_24)
  {
    int rgb_size;
    unsigned char* rgb;
    int y;
    unsigned char* line;
    int rest_4;

    if (bitmap_pos != BMP_SIZE_FILEHEADER + BMP_SIZE_INFOHEADER)
    {
      fclose(f);
      return 0;
    }

    if (fseek(f, bitmap_pos, SEEK_SET))
    {
      fclose(f);
      return 0;
    }

    rgb_size = 3 * x_res;
    rest_4 = rgb_size % 4;
    if (rest_4 > 0)
      rgb_size += 4 - rest_4;

    image->width  = x_res;
    image->height = y_res;
    image->format = GLUS_RGB;
    
    image->data   = (char*)malloc(x_res * y_res * 3);
    if (image->data == NULL)
      return 0;

    rgb = (unsigned char*)malloc(rgb_size);

    /* заполняем данные из файла */
    for (y = 0; y < y_res; y++)
    {
      size_t numb = 0;
      int x = 0;
      numb = fread(rgb, rgb_size, 1, f);
      if (numb != 1)
      {
        fclose(f);
        free(rgb);
        return 0;
      }
      numb = 0;
      line = image->data + x_res * 3 * y;
      for (x = 0; x < x_res; x++)
      {
        line[2] = rgb[numb++];
        line[1] = rgb[numb++];
        line[0]= rgb[numb++];
        line += 3;
      }
    }
    fclose(f);
    free(rgb);
  }
  else
    return 0;

  return 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage)
{
	if (!tgaimage)
	{
		return;
	}

	if (tgaimage->data)
	{
		free(tgaimage->data);

		tgaimage->data = 0;
	}

	tgaimage->width = 0;

	tgaimage->height = 0;

	tgaimage->format = 0;
}
