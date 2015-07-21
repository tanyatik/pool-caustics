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

#include <math.h>

//

GLUSboolean glusIsRowZerof(const GLUSfloat matrix[16], GLUSuint row)
{
	GLUSuint column;

	for (column = 0; column < 4; column++)
	{
		if (matrix[column*4+row] != 0.0f)
			return GLUS_FALSE;
	}
	
	return GLUS_TRUE;
}
	
GLUSboolean glusIsColumnZerof(const GLUSfloat matrix[16], GLUSuint column)
{
	GLUSuint row; 

	for (row = 0; row < 4; row++)
	{
		if (matrix[column*4+row] != 0.0f)
			return GLUS_FALSE;
	}
	
	return GLUS_TRUE;
}
	
GLUSvoid glusDevideRowByf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint row, GLUSfloat value)
{
	GLUSuint column;

	for (column = 0; column < 4; column++)
	{
		matrix[column*4+row] /= value;
		result[column*4+row] /= value;
	}
}
	
GLUSvoid glusSwapRowf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint rowOne, GLUSuint rowTwo)
{
	GLUSuint column;

	GLUSfloat temp;
	
	for (column = 0; column < 4; column++)
	{
		temp = matrix[column*4 + rowOne];
		matrix[column*4 + rowOne] = matrix[column*4 + rowTwo];
		matrix[column*4 + rowTwo] = temp;

		temp = result[column*4 + rowOne];
		result[column*4 + rowOne] = result[column*4 + rowTwo];
		result[column*4 + rowTwo] = temp;
	}	
}
	
GLUSvoid glusAddRowf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint rowOne, GLUSuint rowTwo, GLUSfloat factor)
{
	GLUSuint column;

	for (column = 0; column < 4; column++)
	{
		matrix[column*4+rowOne] += matrix[column*4+rowTwo]*factor;
		result[column*4+rowOne] += result[column*4+rowTwo]*factor;
	}
}

//

GLUSvoid GLUSAPIENTRY glusLoadIdentityf(GLUSfloat matrix[16])
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

GLUSvoid GLUSAPIENTRY glusCopyMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16])
{
	GLUSuint i;

	for (i = 0; i < 16; i++)
	{
		matrix[i] = source[i];
	}
}

GLUSvoid GLUSAPIENTRY glusMultMatrixf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16])
{
	GLUSuint i;
	GLUSuint k; 

	GLUSfloat temp[16];

	for (i = 0; i < 16; i++)
	{
		temp[i] = 0.0f;
		
		for(k = 0; k < 4; k++)
		{
			//			  		row   column   		   row column
			temp[i] += matrix0[(i%4)+(k*4)] * matrix1[(k)+((i/4)*4)];
		}
	}
		
	for (i = 0; i < 16; i++)
	{
		matrix[i] = temp[i];
	}
}

GLUSboolean GLUSAPIENTRY glusInverseMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16])
{
	GLUSuint i;

	GLUSint column;
	GLUSint row;

	GLUSfloat copy[16];

	glusLoadIdentityf(matrix);
	
	//
	// Copy the original matrix as we want to manipulate it
	//
	for (i = 0; i < 16; i++)
	{
		copy[i] = source[i];
	}
	
	//
	// Make triangle form
	//
	for (column = 0; column < 4; column++)
	{
		GLUSuint row;

		for (row = column; row < 4; row++)
		{
			//
			// Is row all zero, then return false			
			//
			if (glusIsRowZerof(copy, row))
				return GLUS_FALSE;	

			//
			// Divide, if not zero, by copy[column*4+row]
			//
			if (copy[column*4+row] != 0.0f)
			{
				glusDevideRowByf(matrix, copy, row, copy[column*4+row]);
			}
		}

		//
		// Is column all zero, then return false
		//		
		if (glusIsColumnZerof(copy, column))
			return GLUS_FALSE;
		
		//
		// Is pivot [column*4+column] = 0.0f
		//
		for (row = column+1; row < 4; row++)
		{
			if (copy[column*4+row] == 1.0f)
			{
				//
				// Swap with pivot row = column
				//
				glusSwapRowf(matrix, copy, column, row);
				
				break;
			}
		}
		
		for (row = column+1; row < 4; row++)
		{
			//
			// Subtract, [column*4+row] not zero, current row minus pivot row = column
			//
			if (copy[column*4+row] != 0.0f)
			{
				glusAddRowf(matrix, copy, row, column, -1.0f);
			}
		}
	}
	
	//
	// Make diagonal form
	//
	for (column = 3; column >= 0; column--)
	{
		for (row = column-1; row >= 0; row--)
		{
			//
			// Subtract, if [column*4+row] not zero, current row minus pivot row = column with factor [column*4+row]
			//
			if (copy[column*4+row] != 0.0f)
			{
				glusAddRowf(matrix, copy, row, column, -copy[column*4+row]);
			}
		}
	}

	return GLUS_TRUE;
}


GLUSvoid GLUSAPIENTRY glusTransposef(GLUSfloat matrix[16])
{
	GLUSuint column; 
	GLUSuint row; 

	GLUSfloat temp[16];

	glusCopyMatrixf(temp, matrix);

	for (column = 0; column < 4; column++)
	{
		for (row = 0; row < 4; row++)
		{
			matrix[row*4+column] = temp[column*4+row];
		}
	}
}


GLUSvoid GLUSAPIENTRY glusTranslatef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z)
{
	GLUSfloat temp[16];

	glusLoadIdentityf(temp);

	temp[12] = x;
	temp[13] = y;
	temp[14] = z;
	
	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusRotatef(GLUSfloat matrix[16], GLUSfloat angle, GLUSfloat x, GLUSfloat y, GLUSfloat z)
{
	GLUSfloat temp[16];

	GLUSfloat s = sinf(2.0f*PIf*angle/360.0f);
	GLUSfloat c = cosf(2.0f*PIf*angle/360.0f);

	GLUSfloat vector[3] = {x, y, z};

	glusNormalizef(vector);

	x = vector[0];
	y = vector[1];
	z = vector[2];

	glusLoadIdentityf(temp);

	temp[0] = x*x*(1 - c) + c;
	temp[1] = x*y*(1 - c) + z*s;
	temp[2] = x*z*(1 - c) - y*s;

	temp[4] = x*y*(1 - c) - z*s;
	temp[5] = y*y*(1 - c) + c;
	temp[6] = y*z*(1 - c) + x*s;

	temp[8] = x*z*(1 - c) + y*s;
	temp[9] = y*z*(1 - c) - x*s;
	temp[10] = z*z*(1 - c) + c;

	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusRotateRzRyRxf(GLUSfloat matrix[16], GLUSfloat anglex, GLUSfloat angley, GLUSfloat anglez)
{
	GLUSfloat temp[16];

	GLUSfloat rx = 2.0f*PIf*anglex/360.0f;
	GLUSfloat ry = 2.0f*PIf*angley/360.0f;
	GLUSfloat rz = 2.0f*PIf*anglez/360.0f;
	GLUSfloat sx = sinf(rx);
	GLUSfloat cx = cosf(rx);
	GLUSfloat sy = sinf(ry);
	GLUSfloat cy = cosf(ry);
	GLUSfloat sz = sinf(rz);
	GLUSfloat cz = cosf(rz);
	
	glusLoadIdentityf(temp);

	temp[0] = cy*cz;
	temp[1] = cy*sz;
	temp[2] = -sy;

	temp[4] = cz*sx*sy-cx*sz;
	temp[5] = cx*cz+sx*sy*sz;
	temp[6] = cy*sx;

	temp[8] = cx*cz*sy+sx*sz;
	temp[9] = -cz*sx+cx*sy*sz;
	temp[10] = cx*cy;

	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusScalef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z)
{
	GLUSfloat temp[16];

	glusLoadIdentityf(temp);

	temp[0] = x;
	temp[5] = y;
	temp[10] = z;
	
	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMultVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3])
{
	GLUSuint i;

	GLUSfloat temp[3];

	for (i = 0; i < 3; i++)
		temp[i] = matrix[i]*vector[0] + matrix[4+i]*vector[1] + matrix[8+i]*vector[2];
	
	for (i = 0; i < 3; i++)	
    	result[i] = temp[i];
}

GLUSvoid GLUSAPIENTRY glusMultPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat vector[4])
{
	GLUSuint i;

	GLUSfloat temp[4];

	for (i = 0; i < 4; i++)
		temp[i] = matrix[i]*vector[0] + matrix[4+i]*vector[1] + matrix[8+i]*vector[2] + matrix[12+i]*vector[3];
	
	for (i = 0; i < 3; i++)	
    	result[i] = temp[i];
        
    if (temp[3] != 0.0f)
    {
		for (i = 0; i < 3; i++)	
    		result[i] /= temp[3];
    }
}
