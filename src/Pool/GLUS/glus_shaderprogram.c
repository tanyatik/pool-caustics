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

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

GLUSboolean GLUSAPIENTRY glusCompileProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
{
	GLint compiled;

	GLint logLength, charsWritten;

	char* log;

	if (!shaderProgram || !vertexSource)
		return GLUS_FALSE;

	shaderProgram->program = 0;
	shaderProgram->vertex = 0;
	shaderProgram->control = 0;
	shaderProgram->evaluation = 0;
	shaderProgram->geometry = 0;
	shaderProgram->fragment = 0;

	shaderProgram->vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(shaderProgram->vertex, 1, (const char**)vertexSource, 0);
	glCompileShader(shaderProgram->vertex);
  glGetShaderiv(shaderProgram->vertex, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		glGetShaderiv(shaderProgram->vertex, GL_INFO_LOG_LENGTH, &logLength);
		log = (char*)malloc(logLength);
    glGetShaderInfoLog(shaderProgram->vertex, logLength, &charsWritten, log);

		printf("Vertex shader compile error:\n");
		printf("%s\n", log);

		free(log);

		shaderProgram->vertex = 0;

		return GLUS_FALSE;
	}

	if (controlSource)
	{
		shaderProgram->control = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(shaderProgram->control, 1, (const char**)controlSource, 0);
		glCompileShader(shaderProgram->control);
		glGetShaderiv(shaderProgram->control, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			glGetShaderiv(shaderProgram->control, GL_INFO_LOG_LENGTH, &logLength);
			log = (char*)malloc(logLength);
			glGetShaderInfoLog(shaderProgram->control, logLength, &charsWritten, log);

			printf("Control shader compile error:\n");
			printf("%s\n", log);

			free(log);

			shaderProgram->control = 0;

			return GLUS_FALSE;
		}
	}

	if (evaluationSource)
	{
		shaderProgram->evaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(shaderProgram->evaluation, 1, (const char**)evaluationSource, 0);
		glCompileShader(shaderProgram->evaluation);
		glGetShaderiv(shaderProgram->evaluation, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			glGetShaderiv(shaderProgram->evaluation, GL_INFO_LOG_LENGTH, &logLength);
			log = (char*)malloc(logLength);
			glGetShaderInfoLog(shaderProgram->evaluation, logLength, &charsWritten, log);

			printf("Evaluation shader compile error:\n");
			printf("%s\n", log);

			free(log);

			shaderProgram->evaluation = 0;

			return GLUS_FALSE;
		}
	}


	if (geometrySource)
	{
		shaderProgram->geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(shaderProgram->geometry, 1, (const char**)geometrySource, 0);
		glCompileShader(shaderProgram->geometry);
		glGetShaderiv(shaderProgram->geometry, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			glGetShaderiv(shaderProgram->geometry, GL_INFO_LOG_LENGTH, &logLength);

			log = (char*)malloc(logLength);
			glGetShaderInfoLog(shaderProgram->geometry, logLength, &charsWritten, log);

			printf("Geometry shader compile error:\n");
			printf("%s\n", log);

			free(log);

			shaderProgram->geometry = 0;

			return GLUS_FALSE;
		}
	}


  if(fragmentSource)
  {
    shaderProgram->fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shaderProgram->fragment, 1, (const char**)fragmentSource, 0);
    glCompileShader(shaderProgram->fragment);
    glGetShaderiv(shaderProgram->fragment, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
      glGetShaderiv(shaderProgram->fragment, GL_INFO_LOG_LENGTH, &logLength);

      log = (char*)malloc(logLength);

      glGetShaderInfoLog(shaderProgram->fragment, logLength, &charsWritten, log);

      printf("Fragment shader compile error:\n");
      printf("%s\n", log);

      free(log);

      shaderProgram->fragment = 0;

      return GLUS_FALSE;
    }
  }


	shaderProgram->program = glCreateProgram();

  glAttachShader(shaderProgram->program, shaderProgram->vertex);

	if (shaderProgram->control)
		glAttachShader(shaderProgram->program, shaderProgram->control);

	if (shaderProgram->evaluation)
		glAttachShader(shaderProgram->program, shaderProgram->evaluation);

	if (shaderProgram->geometry)
		glAttachShader(shaderProgram->program, shaderProgram->geometry);

  if(shaderProgram->fragment)
    glAttachShader(shaderProgram->program, shaderProgram->fragment);

	return GL_TRUE;
}

GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram)
{
    GLint linked;

	GLint logLength, charsWritten;

	char* log;

	if (!shaderProgram)
	{
		return GLUS_FALSE;
	}

	// http://www.opengl.org/sdk/docs/man/xhtml/glLinkProgram.xml
  glLinkProgram(shaderProgram->program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
	glGetProgramiv(shaderProgram->program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
		glGetProgramiv(shaderProgram->program, GL_INFO_LOG_LENGTH, &logLength);

		log = (char*)malloc(logLength);

		// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgramInfoLog.xml
		glGetProgramInfoLog(shaderProgram->program, logLength, &charsWritten, log);

		printf("Shader program link error:\n");
		printf("%s\n", log);

		free(log);

		shaderProgram->program = 0;

		return GLUS_FALSE;
	}

	return GL_TRUE;
}

GLUSboolean GLUSAPIENTRY glusBuildProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
{
	if (!glusCompileProgram(shaderProgram, vertexSource, controlSource, evaluationSource, geometrySource, fragmentSource))
	{
		return GLUS_FALSE;
	}

	return glusLinkProgram(shaderProgram);
}

GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram)
{
	if (!shaderprogram)
	{
		return;
	}

	if (shaderprogram->program)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteProgram.xml
		glDeleteProgram(shaderprogram->program);

		shaderprogram->program = 0;
	}

	if (shaderprogram->fragment)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->fragment);

		shaderprogram->fragment = 0;
	}

	if (shaderprogram->geometry)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->geometry);

		shaderprogram->geometry = 0;
	}

	if (shaderprogram->evaluation)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->evaluation);

		shaderprogram->evaluation = 0;
	}

	if (shaderprogram->control)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->control);

		shaderprogram->control = 0;
	}

	if (shaderprogram->vertex)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->vertex);

		shaderprogram->vertex = 0;
	}
}
