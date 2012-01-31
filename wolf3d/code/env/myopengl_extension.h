/*

	Copyright (C) 2004 Michael Liebscher

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
 *	myopengl_extension.h:   Interface to OpenGL extensions.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 */

/*

	Notes:

	This module is implemented by opengl_extenstion.c

*/


#ifndef __MYOPENGL_EXTENSION_H__
#define __MYOPENGL_EXTENSION_H__


#ifdef IPHONE
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
#include <GL/gl.h>
#include "GL/glext.h"
#endif							


#ifndef APIENTRY

	#define APIENTRY
	
#endif

#ifndef APIENTRYP

	#define APIENTRYP APIENTRY *
	
#endif



///////////////////////////////////////////////////////////////////////////////
//
// OpenGL Extensions
//
///////////////////////////////////////////////////////////////////////////////


#define GL_POINT_SIZE_MIN_EXT				0x8126
#define GL_POINT_SIZE_MAX_EXT				0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT	0x8128
#define GL_DISTANCE_ATTENUATION_EXT			0x8129

#ifdef __sgi
#define GL_SHARED_TEXTURE_PALETTE_EXT		GL_TEXTURE_COLOR_TABLE_SGI
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif


#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3

#ifndef WGL_ARB_pixel_format
	#define WGL_ARB_pixel_format				1
	#define WGL_NUMBER_PIXEL_FORMATS_ARB		0x2000
	#define WGL_DRAW_TO_WINDOW_ARB				0x2001
	#define WGL_DRAW_TO_BITMAP_ARB				0x2002
	#define WGL_ACCELERATION_ARB				0x2003
	#define WGL_NEED_PALETTE_ARB				0x2004
	#define WGL_NEED_SYSTEM_PALETTE_ARB			0x2005
	#define WGL_SWAP_LAYER_BUFFERS_ARB			0x2006
	#define WGL_SWAP_METHOD_ARB					0x2007
	#define WGL_NUMBER_OVERLAYS_ARB				0x2008
	#define WGL_NUMBER_UNDERLAYS_ARB			0x2009
	#define WGL_TRANSPARENT_ARB					0x200A
	#define WGL_TRANSPARENT_RED_VALUE_ARB		0x2037
	#define WGL_TRANSPARENT_GREEN_VALUE_ARB		0x2038
	#define WGL_TRANSPARENT_BLUE_VALUE_ARB		0x2039
	#define WGL_TRANSPARENT_ALPHA_VALUE_ARB		0x203A
	#define WGL_TRANSPARENT_INDEX_VALUE_ARB		0x203B
	#define WGL_SHARE_DEPTH_ARB					0x200C
	#define WGL_SHARE_STENCIL_ARB				0x200D
	#define WGL_SHARE_ACCUM_ARB					0x200E
	#define WGL_SUPPORT_GDI_ARB					0x200F
	#define WGL_SUPPORT_OPENGL_ARB				0x2010
	#define WGL_DOUBLE_BUFFER_ARB				0x2011
	#define WGL_STEREO_ARB						0x2012
	#define WGL_PIXEL_TYPE_ARB					0x2013
	#define WGL_COLOR_BITS_ARB					0x2014
	#define WGL_RED_BITS_ARB					0x2015
	#define WGL_RED_SHIFT_ARB					0x2016
	#define WGL_GREEN_BITS_ARB					0x2017
	#define WGL_GREEN_SHIFT_ARB					0x2018
	#define WGL_BLUE_BITS_ARB					0x2019
	#define WGL_BLUE_SHIFT_ARB					0x201A
	#define WGL_ALPHA_BITS_ARB					0x201B
	#define WGL_ALPHA_SHIFT_ARB					0x201C
	#define WGL_ACCUM_BITS_ARB					0x201D
	#define WGL_ACCUM_RED_BITS_ARB				0x201E
	#define WGL_ACCUM_GREEN_BITS_ARB			0x201F
	#define WGL_ACCUM_BLUE_BITS_ARB				0x2020
	#define WGL_ACCUM_ALPHA_BITS_ARB			0x2021
	#define WGL_DEPTH_BITS_ARB					0x2022
	#define WGL_STENCIL_BITS_ARB				0x2023
	#define WGL_AUX_BUFFERS_ARB					0x2024

	#define WGL_NO_ACCELERATION_ARB				0x2025
	#define WGL_GENERIC_ACCELERATION_ARB		0x2026
	#define WGL_FULL_ACCELERATION_ARB			0x2027

	#define WGL_SWAP_EXCHANGE_ARB				0x2028
	#define WGL_SWAP_COPY_ARB					0x2029
	#define WGL_SWAP_UNDEFINED_ARB				0x202A

	#define WGL_TYPE_RGBA_ARB					0x202B
	#define WGL_TYPE_COLORINDEX_ARB				0x202C
#endif /* WGL_ARB_pixel_format */

#ifndef WGL_ARB_pbuffer
	#define WGL_ARB_pbuffer					1
	#define WGL_DRAW_TO_PBUFFER_ARB			0x202D
	#define WGL_MAX_PBUFFER_PIXELS_ARB		0x202E
	#define WGL_MAX_PBUFFER_WIDTH_ARB		0x202F
	#define WGL_MAX_PBUFFER_HEIGHT_ARB		0x2030
	#define WGL_PBUFFER_LARGEST_ARB			0x2033
	#define WGL_PBUFFER_WIDTH_ARB			0x2034
	#define WGL_PBUFFER_HEIGHT_ARB			0x2035
	#define WGL_PBUFFER_LOST_ARB			0x2036
#endif










/* 
	-----------------------
	ARB_MultiTexture
	-----------------------
*/
void (APIENTRYP pfglActiveTextureARB) (GLenum);
void (APIENTRYP pfglClientActiveTextureARB) (GLenum);
void (APIENTRYP pfglMultiTexCoord1dARB) (GLenum, GLdouble);
void (APIENTRYP pfglMultiTexCoord1dvARB) (GLenum, const GLdouble *);
void (APIENTRYP pfglMultiTexCoord1fARB) (GLenum, GLfloat);
void (APIENTRYP pfglMultiTexCoord1fvARB) (GLenum, const GLfloat *);
void (APIENTRYP pfglMultiTexCoord1iARB) (GLenum, GLint);
void (APIENTRYP pfglMultiTexCoord1ivARB) (GLenum, const GLint *);
void (APIENTRYP pfglMultiTexCoord1sARB) (GLenum, GLshort);
void (APIENTRYP pfglMultiTexCoord1svARB) (GLenum, const GLshort *);
void (APIENTRYP pfglMultiTexCoord2dARB) (GLenum, GLdouble, GLdouble);
void (APIENTRYP pfglMultiTexCoord2dvARB) (GLenum, const GLdouble *);
void (APIENTRYP pfglMultiTexCoord2fARB) (GLenum, GLfloat, GLfloat);
void (APIENTRYP pfglMultiTexCoord2fvARB) (GLenum, const GLfloat *);
void (APIENTRYP pfglMultiTexCoord2iARB) (GLenum, GLint, GLint);
void (APIENTRYP pfglMultiTexCoord2ivARB) (GLenum, const GLint *);
void (APIENTRYP pfglMultiTexCoord2sARB) (GLenum, GLshort, GLshort);
void (APIENTRYP pfglMultiTexCoord2svARB) (GLenum, const GLshort *);
void (APIENTRYP pfglMultiTexCoord3dARB) (GLenum, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglMultiTexCoord3dvARB) (GLenum, const GLdouble *);
void (APIENTRYP pfglMultiTexCoord3fARB) (GLenum, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglMultiTexCoord3fvARB) (GLenum, const GLfloat *);
void (APIENTRYP pfglMultiTexCoord3iARB) (GLenum, GLint, GLint, GLint);
void (APIENTRYP pfglMultiTexCoord3ivARB) (GLenum, const GLint *);
void (APIENTRYP pfglMultiTexCoord3sARB) (GLenum, GLshort, GLshort, GLshort);
void (APIENTRYP pfglMultiTexCoord3svARB) (GLenum, const GLshort *);
void (APIENTRYP pfglMultiTexCoord4dARB) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglMultiTexCoord4dvARB) (GLenum, const GLdouble *);
void (APIENTRYP pfglMultiTexCoord4fARB) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglMultiTexCoord4fvARB) (GLenum, const GLfloat *);
void (APIENTRYP pfglMultiTexCoord4iARB) (GLenum, GLint, GLint, GLint, GLint);
void (APIENTRYP pfglMultiTexCoord4ivARB) (GLenum, const GLint *);
void (APIENTRYP pfglMultiTexCoord4sARB) (GLenum, GLshort, GLshort, GLshort, GLshort);
void (APIENTRYP pfglMultiTexCoord4svARB) (GLenum, const GLshort *);


/* 
	-----------------------
	ARB_TransposeMatrix
	-----------------------
*/
void (APIENTRYP pfglLoadTransposeMatrixfARB) (const GLfloat *);
void (APIENTRYP pfglLoadTransposeMatrixdARB) (const GLdouble *);
void (APIENTRYP pfglMultTransposeMatrixfARB) (const GLfloat *);
void (APIENTRYP pfglMultTransposeMatrixdARB) (const GLdouble *);


/* 
	-----------------------
	ARB_MultiSample
	-----------------------
*/
void (APIENTRYP pfglSampleCoverageARB) (GLclampf, GLboolean);


/* 
	-----------------------
	ARB_TextureCompression
	-----------------------
*/
void (APIENTRYP pfglCompressedTexImage3DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void (APIENTRYP pfglCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
void (APIENTRYP pfglCompressedTexImage1DARB) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
void (APIENTRYP pfglCompressedTexSubImage3DARB) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglCompressedTexSubImage2DARB) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglCompressedTexSubImage1DARB) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglGetCompressedTexImageARB) (GLenum, GLint, GLvoid *);


/* 
	-----------------------
	ARB_PointParameters
	-----------------------
*/
void (APIENTRYP pfglPointParameterfARB) (GLenum, GLfloat);
void (APIENTRYP pfglPointParameterfvARB) (GLenum, const GLfloat *);


/* 
	-----------------------
	ARB_VertexBlend
	-----------------------
*/
void (APIENTRYP pfglWeightbvARB) (GLint, const GLbyte *);
void (APIENTRYP pfglWeightsvARB) (GLint, const GLshort *);
void (APIENTRYP pfglWeightivARB) (GLint, const GLint *);
void (APIENTRYP pfglWeightfvARB) (GLint, const GLfloat *);
void (APIENTRYP pfglWeightdvARB) (GLint, const GLdouble *);
void (APIENTRYP pfglWeightubvARB) (GLint, const GLubyte *);
void (APIENTRYP pfglWeightusvARB) (GLint, const GLushort *);
void (APIENTRYP pfglWeightuivARB) (GLint, const GLuint *);
void (APIENTRYP pfglWeightPointerARB) (GLint, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglVertexBlendARB) (GLint);


/* 
	-----------------------
	ARB_MatrixPalette
	-----------------------
*/
void (APIENTRYP pfglCurrentPaletteMatrixARB) (GLint);
void (APIENTRYP pfglMatrixIndexubvARB) (GLint, const GLubyte *);
void (APIENTRYP pfglMatrixIndexusvARB) (GLint, const GLushort *);
void (APIENTRYP pfglMatrixIndexuivARB) (GLint, const GLuint *);
void (APIENTRYP pfglMatrixIndexPointerARB) (GLint, GLenum, GLsizei, const GLvoid *);


/* 
	------------------------------
	ARB_VertexProgram
	------------------------------
*/
void (APIENTRYP pfglVertexAttrib1dARB) (GLuint, GLdouble);
void (APIENTRYP pfglVertexAttrib1dvARB) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib1fARB) (GLuint, GLfloat);
void (APIENTRYP pfglVertexAttrib1fvARB) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib1sARB) (GLuint, GLshort);
void (APIENTRYP pfglVertexAttrib1svARB) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib2dARB) (GLuint, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib2dvARB) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib2fARB) (GLuint, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib2fvARB) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib2sARB) (GLuint, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib2svARB) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib3dARB) (GLuint, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib3dvARB) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib3fARB) (GLuint, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib3fvARB) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib3sARB) (GLuint, GLshort, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib3svARB) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib4NbvARB) (GLuint, const GLbyte *);
void (APIENTRYP pfglVertexAttrib4NivARB) (GLuint, const GLint *);
void (APIENTRYP pfglVertexAttrib4NsvARB) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib4NubARB) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
void (APIENTRYP pfglVertexAttrib4NubvARB) (GLuint, const GLubyte *);
void (APIENTRYP pfglVertexAttrib4NuivARB) (GLuint, const GLuint *);
void (APIENTRYP pfglVertexAttrib4NusvARB) (GLuint, const GLushort *);
void (APIENTRYP pfglVertexAttrib4bvARB) (GLuint, const GLbyte *);
void (APIENTRYP pfglVertexAttrib4dARB) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib4dvARB) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib4fARB) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib4fvARB) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib4ivARB) (GLuint, const GLint *);
void (APIENTRYP pfglVertexAttrib4sARB) (GLuint, GLshort, GLshort, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib4svARB) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib4ubvARB) (GLuint, const GLubyte *);
void (APIENTRYP pfglVertexAttrib4uivARB) (GLuint, const GLuint *);
void (APIENTRYP pfglVertexAttrib4usvARB) (GLuint, const GLushort *);
void (APIENTRYP pfglVertexAttribPointerARB) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
void (APIENTRYP pfglEnableVertexAttribArrayARB) (GLuint);
void (APIENTRYP pfglDisableVertexAttribArrayARB) (GLuint);
void (APIENTRYP pfglProgramStringARB) (GLenum, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglBindProgramARB) (GLenum, GLuint);
void (APIENTRYP pfglDeleteProgramsARB) (GLsizei, const GLuint *);
void (APIENTRYP pfglGenProgramsARB) (GLsizei, GLuint *);
void (APIENTRYP pfglProgramEnvParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglProgramEnvParameter4dvARB) (GLenum, GLuint, const GLdouble *);
void (APIENTRYP pfglProgramEnvParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglProgramEnvParameter4fvARB) (GLenum, GLuint, const GLfloat *);
void (APIENTRYP pfglProgramLocalParameter4dARB) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglProgramLocalParameter4dvARB) (GLenum, GLuint, const GLdouble *);
void (APIENTRYP pfglProgramLocalParameter4fARB) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglProgramLocalParameter4fvARB) (GLenum, GLuint, const GLfloat *);
void (APIENTRYP pfglGetProgramEnvParameterdvARB) (GLenum, GLuint, GLdouble *);
void (APIENTRYP pfglGetProgramEnvParameterfvARB) (GLenum, GLuint, GLfloat *);
void (APIENTRYP pfglGetProgramLocalParameterdvARB) (GLenum, GLuint, GLdouble *);
void (APIENTRYP pfglGetProgramLocalParameterfvARB) (GLenum, GLuint, GLfloat *);
void (APIENTRYP pfglGetProgramivARB) (GLenum, GLenum, GLint *);
void (APIENTRYP pfglGetProgramStringARB) (GLenum, GLenum, GLvoid *);
void (APIENTRYP pfglGetVertexAttribdvARB) (GLuint, GLenum, GLdouble *);
void (APIENTRYP pfglGetVertexAttribfvARB) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetVertexAttribivARB) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetVertexAttribPointervARB) (GLuint, GLenum, GLvoid* *);
GLboolean (APIENTRYP pfglIsProgramARB) (GLuint);


/* All ARB_fragment_program entry points are shared with ARB_vertex_program. */


/* 
	------------------------------
	ARB_VertexBufferObject
	------------------------------
*/
void (APIENTRYP pfglBindBufferARB) (GLenum, GLuint);
void (APIENTRYP pfglDeleteBuffersARB) (GLsizei, const GLuint *);
void (APIENTRYP pfglGenBuffersARB) (GLsizei, GLuint *);
GLboolean (APIENTRYP pfglIsBufferARB) (GLuint);
void (APIENTRYP pfglBufferDataARB) (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
void (APIENTRYP pfglBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *);
void (APIENTRYP pfglGetBufferSubDataARB) (GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *);
GLvoid* (APIENTRYP pfglMapBufferARB) (GLenum, GLenum);
GLboolean (APIENTRYP pfglUnmapBufferARB) (GLenum);
void (APIENTRYP pfglGetBufferParameterivARB) (GLenum, GLenum, GLint *);
void (APIENTRYP pfglGetBufferPointervARB) (GLenum, GLenum, GLvoid* *);


/* 
	------------------------------
	ARB_OcclusionQuery
	------------------------------
*/
void (APIENTRYP pfglGenQueriesARB) (GLsizei, GLuint *);
void (APIENTRYP pfglDeleteQueriesARB) (GLsizei, const GLuint *);
GLboolean (APIENTRYP pfglIsQueryARB) (GLuint);
void (APIENTRYP pfglBeginQueryARB) (GLenum, GLuint);
void (APIENTRYP pfglEndQueryARB) (GLenum);
void (APIENTRYP pfglGetQueryivARB) (GLenum, GLenum, GLint *);
void (APIENTRYP pfglGetQueryObjectivARB) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetQueryObjectuivARB) (GLuint, GLenum, GLuint *);


/* 
	------------------------------
	ARB_ShaderObjects
	------------------------------
*/
void (APIENTRYP pfglDeleteObjectARB) (GLhandleARB);
GLhandleARB (APIENTRYP pfglGetHandleARB) (GLenum);
void (APIENTRYP pfglDetachObjectARB) (GLhandleARB, GLhandleARB);
GLhandleARB (APIENTRYP pfglCreateShaderObjectARB) (GLenum);
void (APIENTRYP pfglShaderSourceARB) (GLhandleARB, GLsizei, const GLcharARB* *, const GLint *);
void (APIENTRYP pfglCompileShaderARB) (GLhandleARB);
GLhandleARB (APIENTRYP pfglCreateProgramObjectARB) (void);
void (APIENTRYP pfglAttachObjectARB) (GLhandleARB, GLhandleARB);
void (APIENTRYP pfglLinkProgramARB) (GLhandleARB);
void (APIENTRYP pfglUseProgramObjectARB) (GLhandleARB);
void (APIENTRYP pfglValidateProgramARB) (GLhandleARB);
void (APIENTRYP pfglUniform1fARB) (GLint, GLfloat);
void (APIENTRYP pfglUniform2fARB) (GLint, GLfloat, GLfloat);
void (APIENTRYP pfglUniform3fARB) (GLint, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglUniform4fARB) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglUniform1iARB) (GLint, GLint);
void (APIENTRYP pfglUniform2iARB) (GLint, GLint, GLint);
void (APIENTRYP pfglUniform3iARB) (GLint, GLint, GLint, GLint);
void (APIENTRYP pfglUniform4iARB) (GLint, GLint, GLint, GLint, GLint);
void (APIENTRYP pfglUniform1fvARB) (GLint, GLsizei, const GLfloat *);
void (APIENTRYP pfglUniform2fvARB) (GLint, GLsizei, const GLfloat *);
void (APIENTRYP pfglUniform3fvARB) (GLint, GLsizei, const GLfloat *);
void (APIENTRYP pfglUniform4fvARB) (GLint, GLsizei, const GLfloat *);
void (APIENTRYP pfglUniform1ivARB) (GLint, GLsizei, const GLint *);
void (APIENTRYP pfglUniform2ivARB) (GLint, GLsizei, const GLint *);
void (APIENTRYP pfglUniform3ivARB) (GLint, GLsizei, const GLint *);
void (APIENTRYP pfglUniform4ivARB) (GLint, GLsizei, const GLint *);
void (APIENTRYP pfglUniformMatrix2fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void (APIENTRYP pfglUniformMatrix3fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void (APIENTRYP pfglUniformMatrix4fvARB) (GLint, GLsizei, GLboolean, const GLfloat *);
void (APIENTRYP pfglGetObjectParameterfvARB) (GLhandleARB, GLenum, GLfloat *);
void (APIENTRYP pfglGetObjectParameterivARB) (GLhandleARB, GLenum, GLint *);
void (APIENTRYP pfglGetInfoLogARB) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);
void (APIENTRYP pfglGetAttachedObjectsARB) (GLhandleARB, GLsizei, GLsizei *, GLhandleARB *);
GLint (APIENTRYP pfglGetUniformLocationARB) (GLhandleARB, const GLcharARB *);
void (APIENTRYP pfglGetActiveUniformARB) (GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
void (APIENTRYP pfglGetUniformfvARB) (GLhandleARB, GLint, GLfloat *);
void (APIENTRYP pfglGetUniformivARB) (GLhandleARB, GLint, GLint *);
void (APIENTRYP pfglGetShaderSourceARB) (GLhandleARB, GLsizei, GLsizei *, GLcharARB *);


/* 
	------------------------------
	ARB_VertexShader
	------------------------------
*/
void (APIENTRYP pfglBindAttribLocationARB) (GLhandleARB, GLuint, const GLcharARB *);
void (APIENTRYP pfglGetActiveAttribARB) (GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *);
GLint (APIENTRYP pfglGetAttribLocationARB) (GLhandleARB, const GLcharARB *);


/* 
	-----------------------
	EXT_BlendColor
	-----------------------
*/
void (APIENTRYP pfglBlendColorEXT) (GLclampf, GLclampf, GLclampf, GLclampf);


/* 
	-------------------------
	EXT_CompiledVertexArray
	-------------------------
*/
void ( APIENTRYP pfglLockArraysEXT) ( GLint, GLsizei );
void ( APIENTRYP pfglUnlockArraysEXT) ( void );


/* 
	-----------------------
	EXT_SecondaryColor
	-----------------------
*/
void (APIENTRYP pfglSecondaryColor3bEXT) (GLbyte, GLbyte, GLbyte);
void (APIENTRYP pfglSecondaryColor3bvEXT) (const GLbyte *);
void (APIENTRYP pfglSecondaryColor3dEXT) (GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglSecondaryColor3dvEXT) (const GLdouble *);
void (APIENTRYP pfglSecondaryColor3fEXT) (GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglSecondaryColor3fvEXT) (const GLfloat *);
void (APIENTRYP pfglSecondaryColor3iEXT) (GLint, GLint, GLint);
void (APIENTRYP pfglSecondaryColor3ivEXT) (const GLint *);
void (APIENTRYP pfglSecondaryColor3sEXT) (GLshort, GLshort, GLshort);
void (APIENTRYP pfglSecondaryColor3svEXT) (const GLshort *);
void (APIENTRYP pfglSecondaryColor3ubEXT) (GLubyte, GLubyte, GLubyte);
void (APIENTRYP pfglSecondaryColor3ubvEXT) (const GLubyte *);
void (APIENTRYP pfglSecondaryColor3uiEXT) (GLuint, GLuint, GLuint);
void (APIENTRYP pfglSecondaryColor3uivEXT) (const GLuint *);
void (APIENTRYP pfglSecondaryColor3usEXT) (GLushort, GLushort, GLushort);
void (APIENTRYP pfglSecondaryColor3usvEXT) (const GLushort *);
void (APIENTRYP pfglSecondaryColorPointerEXT) (GLint, GLenum, GLsizei, const GLvoid *);


/* 
	-----------------------
	EXT_VertexShader
	-----------------------
*/
void (APIENTRYP pfglBeginVertexShaderEXT) (void);
void (APIENTRYP pfglEndVertexShaderEXT) (void);
void (APIENTRYP pfglBindVertexShaderEXT) (GLuint);
GLuint (APIENTRYP pfglGenVertexShadersEXT) (GLuint);
void (APIENTRYP pfglDeleteVertexShaderEXT) (GLuint);
void (APIENTRYP pfglShaderOp1EXT) (GLenum, GLuint, GLuint);
void (APIENTRYP pfglShaderOp2EXT) (GLenum, GLuint, GLuint, GLuint);
void (APIENTRYP pfglShaderOp3EXT) (GLenum, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglSwizzleEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
void (APIENTRYP pfglWriteMaskEXT) (GLuint, GLuint, GLenum, GLenum, GLenum, GLenum);
void (APIENTRYP pfglInsertComponentEXT) (GLuint, GLuint, GLuint);
void (APIENTRYP pfglExtractComponentEXT) (GLuint, GLuint, GLuint);
GLuint (APIENTRYP pfglGenSymbolsEXT) (GLenum, GLenum, GLenum, GLuint);
void (APIENTRYP pfglSetInvariantEXT) (GLuint, GLenum, const GLvoid *);
void (APIENTRYP pfglSetLocalConstantEXT) (GLuint, GLenum, const GLvoid *);
void (APIENTRYP pfglVariantbvEXT) (GLuint, const GLbyte *);
void (APIENTRYP pfglVariantsvEXT) (GLuint, const GLshort *);
void (APIENTRYP pfglVariantivEXT) (GLuint, const GLint *);
void (APIENTRYP pfglVariantfvEXT) (GLuint, const GLfloat *);
void (APIENTRYP pfglVariantdvEXT) (GLuint, const GLdouble *);
void (APIENTRYP pfglVariantubvEXT) (GLuint, const GLubyte *);
void (APIENTRYP pfglVariantusvEXT) (GLuint, const GLushort *);
void (APIENTRYP pfglVariantuivEXT) (GLuint, const GLuint *);
void (APIENTRYP pfglVariantPointerEXT) (GLuint, GLenum, GLuint, const GLvoid *);
void (APIENTRYP pfglEnableVariantClientStateEXT) (GLuint);
void (APIENTRYP pfglDisableVariantClientStateEXT) (GLuint);
GLuint (APIENTRYP pfglBindLightParameterEXT) (GLenum, GLenum);
GLuint (APIENTRYP pfglBindMaterialParameterEXT) (GLenum, GLenum);
GLuint (APIENTRYP pfglBindTexGenParameterEXT) (GLenum, GLenum, GLenum);
GLuint (APIENTRYP pfglBindTextureUnitParameterEXT) (GLenum, GLenum);
GLuint (APIENTRYP pfglBindParameterEXT) (GLenum);
GLboolean (APIENTRYP pfglIsVariantEnabledEXT) (GLuint, GLenum);
void (APIENTRYP pfglGetVariantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void (APIENTRYP pfglGetVariantIntegervEXT) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetVariantFloatvEXT) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetVariantPointervEXT) (GLuint, GLenum, GLvoid* *);
void (APIENTRYP pfglGetInvariantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void (APIENTRYP pfglGetInvariantIntegervEXT) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetInvariantFloatvEXT) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetLocalConstantBooleanvEXT) (GLuint, GLenum, GLboolean *);
void (APIENTRYP pfglGetLocalConstantIntegervEXT) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetLocalConstantFloatvEXT) (GLuint, GLenum, GLfloat *);


/* 
	-----------------------
	EXT_VertexWeighting
	-----------------------
*/
void (APIENTRYP pfglVertexWeightfEXT) (GLfloat);
void (APIENTRYP pfglVertexWeightfvEXT) (const GLfloat *);
void (APIENTRYP pfglVertexWeightPointerEXT) (GLsizei, GLenum, GLsizei, const GLvoid *);



//////////////////////////////////
//
//	nVidia Extensions
//
//////////////////////////////////



/* 
	-----------------------
	NV_Fence
	-----------------------
*/
void (APIENTRYP pfglDeleteFencesNV) (GLsizei, const GLuint *);
void (APIENTRYP pfglGenFencesNV) (GLsizei, GLuint *);
GLboolean (APIENTRYP pfglIsFenceNV) (GLuint);
GLboolean (APIENTRYP pfglTestFenceNV) (GLuint);
void (APIENTRYP pfglGetFenceivNV) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglFinishFenceNV) (GLuint);
void (APIENTRYP pfglSetFenceNV) (GLuint, GLenum);


/* 
	-----------------------
	NV_VertexArrayRange
	-----------------------
*/
void (APIENTRYP pfglFlushVertexArrayRangeNV) (void);
void (APIENTRYP pfglVertexArrayRangeNV) (GLsizei, const GLvoid *);


/* 
	-----------------------
	NV_VertexProgram
	-----------------------
*/
GLboolean (APIENTRYP pfglAreProgramsResidentNV) (GLsizei, const GLuint *, GLboolean *);
void (APIENTRYP pfglBindProgramNV) (GLenum, GLuint);
void (APIENTRYP pfglDeleteProgramsNV) (GLsizei, const GLuint *);
void (APIENTRYP pfglExecuteProgramNV) (GLenum, GLuint, const GLfloat *);
void (APIENTRYP pfglGenProgramsNV) (GLsizei, GLuint *);
void (APIENTRYP pfglGetProgramParameterdvNV) (GLenum, GLuint, GLenum, GLdouble *);
void (APIENTRYP pfglGetProgramParameterfvNV) (GLenum, GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetProgramivNV) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetProgramStringNV) (GLuint, GLenum, GLubyte *);
void (APIENTRYP pfglGetTrackMatrixivNV) (GLenum, GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetVertexAttribdvNV) (GLuint, GLenum, GLdouble *);
void (APIENTRYP pfglGetVertexAttribfvNV) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetVertexAttribivNV) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglGetVertexAttribPointervNV) (GLuint, GLenum, GLvoid* *);
GLboolean (APIENTRYP pfglIsProgramNV) (GLuint);
void (APIENTRYP pfglLoadProgramNV) (GLenum, GLuint, GLsizei, const GLubyte *);
void (APIENTRYP pfglProgramParameter4dNV) (GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglProgramParameter4dvNV) (GLenum, GLuint, const GLdouble *);
void (APIENTRYP pfglProgramParameter4fNV) (GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglProgramParameter4fvNV) (GLenum, GLuint, const GLfloat *);
void (APIENTRYP pfglProgramParameters4dvNV) (GLenum, GLuint, GLuint, const GLdouble *);
void (APIENTRYP pfglProgramParameters4fvNV) (GLenum, GLuint, GLuint, const GLfloat *);
void (APIENTRYP pfglRequestResidentProgramsNV) (GLsizei, const GLuint *);
void (APIENTRYP pfglTrackMatrixNV) (GLenum, GLuint, GLenum, GLenum);
void (APIENTRYP pfglVertexAttribPointerNV) (GLuint, GLint, GLenum, GLsizei, const GLvoid *);
void (APIENTRYP pfglVertexAttrib1dNV) (GLuint, GLdouble);
void (APIENTRYP pfglVertexAttrib1dvNV) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib1fNV) (GLuint, GLfloat);
void (APIENTRYP pfglVertexAttrib1fvNV) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib1sNV) (GLuint, GLshort);
void (APIENTRYP pfglVertexAttrib1svNV) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib2dNV) (GLuint, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib2dvNV) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib2fNV) (GLuint, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib2fvNV) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib2sNV) (GLuint, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib2svNV) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib3dNV) (GLuint, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib3dvNV) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib3fNV) (GLuint, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib3fvNV) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib3sNV) (GLuint, GLshort, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib3svNV) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib4dNV) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
void (APIENTRYP pfglVertexAttrib4dvNV) (GLuint, const GLdouble *);
void (APIENTRYP pfglVertexAttrib4fNV) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
void (APIENTRYP pfglVertexAttrib4fvNV) (GLuint, const GLfloat *);
void (APIENTRYP pfglVertexAttrib4sNV) (GLuint, GLshort, GLshort, GLshort, GLshort);
void (APIENTRYP pfglVertexAttrib4svNV) (GLuint, const GLshort *);
void (APIENTRYP pfglVertexAttrib4ubNV) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
void (APIENTRYP pfglVertexAttrib4ubvNV) (GLuint, const GLubyte *);
void (APIENTRYP pfglVertexAttribs1dvNV) (GLuint, GLsizei, const GLdouble *);
void (APIENTRYP pfglVertexAttribs1fvNV) (GLuint, GLsizei, const GLfloat *);
void (APIENTRYP pfglVertexAttribs1svNV) (GLuint, GLsizei, const GLshort *);
void (APIENTRYP pfglVertexAttribs2dvNV) (GLuint, GLsizei, const GLdouble *);
void (APIENTRYP pfglVertexAttribs2fvNV) (GLuint, GLsizei, const GLfloat *);
void (APIENTRYP pfglVertexAttribs2svNV) (GLuint, GLsizei, const GLshort *);
void (APIENTRYP pfglVertexAttribs3dvNV) (GLuint, GLsizei, const GLdouble *);
void (APIENTRYP pfglVertexAttribs3fvNV) (GLuint, GLsizei, const GLfloat *);
void (APIENTRYP pfglVertexAttribs3svNV) (GLuint, GLsizei, const GLshort *);
void (APIENTRYP pfglVertexAttribs4dvNV) (GLuint, GLsizei, const GLdouble *);
void (APIENTRYP pfglVertexAttribs4fvNV) (GLuint, GLsizei, const GLfloat *);
void (APIENTRYP pfglVertexAttribs4svNV) (GLuint, GLsizei, const GLshort *);
void (APIENTRYP pfglVertexAttribs4ubvNV) (GLuint, GLsizei, const GLubyte *);


//////////////////////////////////
//
//	ATI Extensions
//
//////////////////////////////////

/* 
	------------------------------
	ATI_EnvmapBumpmap
	------------------------------
*/
void (APIENTRYP pfglTexBumpParameterivATI) (GLenum, const GLint *);
void (APIENTRYP pfglTexBumpParameterfvATI) (GLenum, const GLfloat *);
void (APIENTRYP pfglGetTexBumpParameterivATI) (GLenum, GLint *);
void (APIENTRYP pfglGetTexBumpParameterfvATI) (GLenum, GLfloat *);


/* 
	------------------------------
	ATI_FragmentShader
	------------------------------
*/
GLuint (APIENTRYP pfglGenFragmentShadersATI) (GLuint);
void (APIENTRYP pfglBindFragmentShaderATI) (GLuint);
void (APIENTRYP pfglDeleteFragmentShaderATI) (GLuint);
void (APIENTRYP pfglBeginFragmentShaderATI) (void);
void (APIENTRYP pfglEndFragmentShaderATI) (void);
void (APIENTRYP pfglPassTexCoordATI) (GLuint, GLuint, GLenum);
void (APIENTRYP pfglSampleMapATI) (GLuint, GLuint, GLenum);
void (APIENTRYP pfglColorFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglColorFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglColorFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglAlphaFragmentOp1ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglAlphaFragmentOp2ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglAlphaFragmentOp3ATI) (GLenum, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void (APIENTRYP pfglSetFragmentShaderConstantATI) (GLuint, const GLfloat *);

/* 
	------------------------------
	ATI_VertexArrayObject
	------------------------------
*/
GLuint (APIENTRYP pfglNewObjectBufferATI) (GLsizei, const GLvoid *, GLenum);
GLboolean (APIENTRYP pfglIsObjectBufferATI) (GLuint);
void (APIENTRYP pfglUpdateObjectBufferATI) (GLuint, GLuint, GLsizei, const GLvoid *, GLenum);
void (APIENTRYP pfglGetObjectBufferfvATI) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetObjectBufferivATI) (GLuint, GLenum, GLint *);
void (APIENTRYP pfglFreeObjectBufferATI) (GLuint);
void (APIENTRYP pfglArrayObjectATI) (GLenum, GLint, GLenum, GLsizei, GLuint, GLuint);
void (APIENTRYP pfglGetArrayObjectfvATI) (GLenum, GLenum, GLfloat *);
void (APIENTRYP pfglGetArrayObjectivATI) (GLenum, GLenum, GLint *);
void (APIENTRYP pfglVariantArrayObjectATI) (GLuint, GLenum, GLsizei, GLuint, GLuint);
void (APIENTRYP pfglGetVariantArrayObjectfvATI) (GLuint, GLenum, GLfloat *);
void (APIENTRYP pfglGetVariantArrayObjectivATI) (GLuint, GLenum, GLint *);




/* 
	------------------------------
	ATI_MapObjectBuffer
	------------------------------
*/
GLvoid* (APIENTRYP pfglMapObjectBufferATI) (GLuint);
void (APIENTRYP pfglUnmapObjectBufferATI) (GLuint);









#endif /* __MYOPENGL_EXTENSION_H__ */
