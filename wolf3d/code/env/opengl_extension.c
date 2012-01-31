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
 *	myopengl_extension.c:   Interface to OpenGL extensions.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 */

/*

	This module implements the binding of OpenGL extensions 
	to our function pointers.

	To use this module call the function GL_ConfigExtensions().

*/

#include <string.h>

#include "myopengl.h"
#include "opengl_local.h"
#include "myopengl_extension.h"
#include "../../common/common_utils.h"
#include "../../common/common.h"



							/* Implemented here */


// Define what you require here.
// If hardware does not meet these defines, then extension load will fail.
#define MY_MAX_ARB_TEXTURESTAGES	4

#define MY_GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTR				128
#define MY_GL_MAX_VERTEX_SHADER_LOCALS_EXT					68
#define MY_GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT			2
#define MY_GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT	97
#define MY_GL_MAX_VERTEX_SHADER_VARIANTS_EXT				4




GL_Extensions gl_ext;



//////////////////////////////
//
//	ARB Functions
//
//////////////////////////////

PRIVATE W8 ConfigARBMultiTexture( const char *glext )
{
	if( strstr( glext, "GL_ARB_multitexture" ) == NULL )
	{
		Com_Printf( "...GL_ARB_multitexture not found\n" );
		return 0;
	}	

	if( ! (pfglActiveTextureARB = ( void * )pfwglGetProcAddress( "glActiveTextureARB" )) ) return 0;
	if( ! (pfglClientActiveTextureARB = ( void * )pfwglGetProcAddress("glClientActiveTextureARB")))return 0;

	if( ! (pfglMultiTexCoord1sARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1sARB" ))) return 0;
	if( ! (pfglMultiTexCoord1iARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1iARB" ))) return 0;
	if( ! (pfglMultiTexCoord1fARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1fARB" ))) return 0;
	if( ! (pfglMultiTexCoord1dARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1dARB" ))) return 0;
	if( ! (pfglMultiTexCoord2sARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2sARB" ))) return 0;
	if( ! (pfglMultiTexCoord2iARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2iARB" ))) return 0;
	if( ! (pfglMultiTexCoord2fARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2fARB" ))) return 0;
	if( ! (pfglMultiTexCoord2dARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2dARB" ))) return 0;
	if( ! (pfglMultiTexCoord3sARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3sARB" ))) return 0;
	if( ! (pfglMultiTexCoord3iARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3iARB" ))) return 0;
	if( ! (pfglMultiTexCoord3fARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3fARB" ))) return 0;
	if( ! (pfglMultiTexCoord3dARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3dARB" ))) return 0;
	if( ! (pfglMultiTexCoord4sARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4sARB" ))) return 0;
	if( ! (pfglMultiTexCoord4iARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4iARB" ))) return 0;
	if( ! (pfglMultiTexCoord4fARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4fARB" ))) return 0;
	if( ! (pfglMultiTexCoord4dARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4dARB" ))) return 0;

	if( ! (pfglMultiTexCoord1svARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1svARB" ))) return 0;
	if( ! (pfglMultiTexCoord1ivARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1ivARB" ))) return 0;
	if( ! (pfglMultiTexCoord1fvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1fvARB" ))) return 0;
	if( ! (pfglMultiTexCoord1dvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord1dvARB" ))) return 0;
	if( ! (pfglMultiTexCoord2svARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2svARB" ))) return 0;
	if( ! (pfglMultiTexCoord2ivARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2ivARB" ))) return 0;
	if( ! (pfglMultiTexCoord2fvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2fvARB" ))) return 0;
	if( ! (pfglMultiTexCoord2dvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord2dvARB" ))) return 0;
	if( ! (pfglMultiTexCoord3svARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3svARB" ))) return 0;
	if( ! (pfglMultiTexCoord3ivARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3ivARB" ))) return 0;
	if( ! (pfglMultiTexCoord3fvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3fvARB" ))) return 0;
	if( ! (pfglMultiTexCoord3dvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord3dvARB" ))) return 0;
	if( ! (pfglMultiTexCoord4svARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4svARB" ))) return 0;
	if( ! (pfglMultiTexCoord4ivARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4ivARB" ))) return 0;
	if( ! (pfglMultiTexCoord4fvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4fvARB" ))) return 0;
	if( ! (pfglMultiTexCoord4dvARB = ( void * )pfwglGetProcAddress( "glMultiTexCoord4dvARB" ))) return 0;

	
	Com_Printf( "...enabling GL_ARB_multitexture\n" );

	return 1;
}


PRIVATE W8 ConfigARBTextureCompression( const char *glext )
{
	if( strstr( glext, "GL_ARB_texture_compression" ) == NULL )
	{
		Com_Printf( "...GL_ARB_texture_compression not found\n" );
		return 0;
	}	  

	if( ! (pfglCompressedTexImage3DARB = ( void * )pfwglGetProcAddress( "glCompressedTexImage3DARB" ))) return 0;
	if( ! (pfglCompressedTexImage2DARB = ( void * )pfwglGetProcAddress( "glCompressedTexImage2DARB" ))) return 0;
	if( ! (pfglCompressedTexImage1DARB = ( void * )pfwglGetProcAddress( "glCompressedTexImage1DARB" ))) return 0;
	if( ! (pfglCompressedTexSubImage3DARB = ( void * )pfwglGetProcAddress( "glCompressedTexSubImage3DARB" ))) return 0;
	if( ! (pfglCompressedTexSubImage2DARB = ( void * )pfwglGetProcAddress( "glCompressedTexSubImage2DARB" ))) return 0;
	if( ! (pfglCompressedTexSubImage1DARB = ( void * )pfwglGetProcAddress( "glCompressedTexSubImage1DARB" ))) return 0;
	if( ! (pfglGetCompressedTexImageARB = ( void * )pfwglGetProcAddress( "glGetCompressedTexImageARB" ))) return 0;

	Com_Printf( "...enabling GL_ARB_texture_compression\n" );

	return 1;
}


PRIVATE W8 CheckARBTextureCubeMap( const char *glext )
{
	if( strstr( glext, "GL_ARB_texture_cube_map" ) == NULL )
	{
		Com_Printf( "...GL_ARB_texture_cube_map not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_ARB_texture_cube_map\n" );

	return 1;
}

PRIVATE W8 ConfigARBVertexProgram( const char *glext )
{
	if( strstr( glext, "GL_ARB_vertex_program" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglVertexAttrib1sARB = ( void * )pfwglGetProcAddress("glVertexAttrib1sARB"))) return 0;
	if( ! (pfglVertexAttrib1fARB = ( void * )pfwglGetProcAddress("glVertexAttrib1fARB"))) return 0;
	if( ! (pfglVertexAttrib1dARB = ( void * )pfwglGetProcAddress("glVertexAttrib1dARB"))) return 0;
	if( ! (pfglVertexAttrib2sARB = ( void * )pfwglGetProcAddress("glVertexAttrib2sARB"))) return 0;
	if( ! (pfglVertexAttrib2fARB = ( void * )pfwglGetProcAddress("glVertexAttrib2fARB"))) return 0;
	if( ! (pfglVertexAttrib2dARB = ( void * )pfwglGetProcAddress("glVertexAttrib2dARB"))) return 0;
	if( ! (pfglVertexAttrib3sARB = ( void * )pfwglGetProcAddress("glVertexAttrib3sARB"))) return 0;
	if( ! (pfglVertexAttrib3fARB = ( void * )pfwglGetProcAddress("glVertexAttrib3fARB"))) return 0;
	if( ! (pfglVertexAttrib3dARB = ( void * )pfwglGetProcAddress("glVertexAttrib3dARB"))) return 0;
	if( ! (pfglVertexAttrib4sARB = ( void * )pfwglGetProcAddress("glVertexAttrib4sARB"))) return 0;
	if( ! (pfglVertexAttrib4fARB = ( void * )pfwglGetProcAddress("glVertexAttrib4fARB"))) return 0;
	if( ! (pfglVertexAttrib4dARB = ( void * )pfwglGetProcAddress("glVertexAttrib4dARB"))) return 0;
	if( ! (pfglVertexAttrib4NubARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NubARB"))) return 0;
	if( ! (pfglVertexAttrib1svARB = ( void * )pfwglGetProcAddress("glVertexAttrib1svARB"))) return 0;
	if( ! (pfglVertexAttrib1fvARB = ( void * )pfwglGetProcAddress("glVertexAttrib1fvARB"))) return 0;
	if( ! (pfglVertexAttrib1dvARB = ( void * )pfwglGetProcAddress("glVertexAttrib1dvARB"))) return 0;
	if( ! (pfglVertexAttrib2svARB = ( void * )pfwglGetProcAddress("glVertexAttrib2svARB"))) return 0;
	if( ! (pfglVertexAttrib2fvARB = ( void * )pfwglGetProcAddress("glVertexAttrib2fvARB"))) return 0;
	if( ! (pfglVertexAttrib2dvARB = ( void * )pfwglGetProcAddress("glVertexAttrib2dvARB"))) return 0;
	if( ! (pfglVertexAttrib3svARB = ( void * )pfwglGetProcAddress("glVertexAttrib3svARB"))) return 0;
	if( ! (pfglVertexAttrib3fvARB = ( void * )pfwglGetProcAddress("glVertexAttrib3fvARB"))) return 0;
	if( ! (pfglVertexAttrib3dvARB = ( void * )pfwglGetProcAddress("glVertexAttrib3dvARB"))) return 0;
	if( ! (pfglVertexAttrib4bvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4bvARB"))) return 0;
	if( ! (pfglVertexAttrib4svARB = ( void * )pfwglGetProcAddress("glVertexAttrib4svARB"))) return 0;
	if( ! (pfglVertexAttrib4ivARB = ( void * )pfwglGetProcAddress("glVertexAttrib4ivARB"))) return 0;
	if( ! (pfglVertexAttrib4ubvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4ubvARB"))) return 0;
	if( ! (pfglVertexAttrib4usvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4usvARB"))) return 0;
	if( ! (pfglVertexAttrib4uivARB = ( void * )pfwglGetProcAddress("glVertexAttrib4uivARB"))) return 0;
	if( ! (pfglVertexAttrib4fvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4fvARB"))) return 0;
	if( ! (pfglVertexAttrib4dvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4dvARB"))) return 0;
	if( ! (pfglVertexAttrib4NbvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NbvARB"))) return 0;
	if( ! (pfglVertexAttrib4NsvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NsvARB"))) return 0;
	if( ! (pfglVertexAttrib4NivARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NivARB"))) return 0;
	if( ! (pfglVertexAttrib4NubvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NubvARB"))) return 0;
	if( ! (pfglVertexAttrib4NusvARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NusvARB"))) return 0;
	if( ! (pfglVertexAttrib4NuivARB = ( void * )pfwglGetProcAddress("glVertexAttrib4NuivARB"))) return 0;
	if( ! (pfglVertexAttribPointerARB = ( void * )pfwglGetProcAddress("glVertexAttribPointerARB"))) return 0;
	if( ! (pfglEnableVertexAttribArrayARB = ( void * )pfwglGetProcAddress("glEnableVertexAttribArrayARB"))) return 0;
	if( ! (pfglDisableVertexAttribArrayARB = ( void * )pfwglGetProcAddress("glDisableVertexAttribArrayARB"))) return 0;
	if( ! (pfglProgramStringARB = ( void * )pfwglGetProcAddress("glProgramStringARB"))) return 0;
	if( ! (pfglBindProgramARB = ( void * )pfwglGetProcAddress("glBindProgramARB"))) return 0;
	if( ! (pfglDeleteProgramsARB = ( void * )pfwglGetProcAddress("glDeleteProgramsARB"))) return 0;
	if( ! (pfglGenProgramsARB = ( void * )pfwglGetProcAddress("glGenProgramsARB"))) return 0;
	if( ! (pfglProgramEnvParameter4fARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4fARB"))) return 0;
	if( ! (pfglProgramEnvParameter4dARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4dARB"))) return 0;
	if( ! (pfglProgramEnvParameter4fvARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4fvARB"))) return 0;
	if( ! (pfglProgramEnvParameter4dvARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4dvARB"))) return 0;
	if( ! (pfglProgramLocalParameter4fARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4fARB"))) return 0;
	if( ! (pfglProgramLocalParameter4dARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4dARB"))) return 0;
	if( ! (pfglProgramLocalParameter4fvARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4fvARB"))) return 0;
	if( ! (pfglProgramLocalParameter4dvARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4dvARB"))) return 0;
	if( ! (pfglGetProgramEnvParameterfvARB = ( void * )pfwglGetProcAddress("glGetProgramEnvParameterfvARB"))) return 0;
	if( ! (pfglGetProgramEnvParameterdvARB = ( void * )pfwglGetProcAddress("glGetProgramEnvParameterdvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameterfvARB = ( void * )pfwglGetProcAddress("glGetProgramLocalParameterfvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameterdvARB = ( void * )pfwglGetProcAddress("glGetProgramLocalParameterdvARB"))) return 0;
	if( ! (pfglGetProgramivARB = ( void * )pfwglGetProcAddress("glGetProgramivARB"))) return 0;
	if( ! (pfglGetProgramStringARB = ( void * )pfwglGetProcAddress("glGetProgramStringARB"))) return 0;
	if( ! (pfglGetVertexAttribdvARB = ( void * )pfwglGetProcAddress("glGetVertexAttribdvARB"))) return 0;
	if( ! (pfglGetVertexAttribfvARB = ( void * )pfwglGetProcAddress("glGetVertexAttribfvARB"))) return 0;
	if( ! (pfglGetVertexAttribivARB = ( void * )pfwglGetProcAddress("glGetVertexAttribivARB"))) return 0;
	if( ! (pfglGetVertexAttribPointervARB = ( void * )pfwglGetProcAddress("glGetVertexAttribPointervARB"))) return 0;
	if( ! (pfglIsProgramARB = ( void * )pfwglGetProcAddress("glIsProgramARB"))) return 0;	
	 
	Com_Printf( "...enabling GL_ARB_vertex_program\n" );

	return 1;
}


////////////////////////////////////////////////////////////




//////////////////////////////
//
//	EXT Functions
//
//////////////////////////////


PRIVATE W8 CheckEXTTextureCompressionS3TC( const char *glext )
{
	if( strstr( glext, "GL_EXT_texture_compression_s3tc" ) == NULL )
	{
		Com_Printf( "...GL_EXT_texture_compression_s3tc not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_EXT_texture_compression_s3tc\n" );

	return 1;
}

PRIVATE W8 CheckEXTTextureEnvCombine( const char *glext )
{
	if( strstr( glext, "GL_EXT_texture_env_combine" ) == NULL )
	{
		Com_Printf( "...GL_EXT_texture_env_combine not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_EXT_texture_env_combine\n" );

	return 1;
}



PRIVATE W8 ConfigEXTSecondaryColor( const char *glext )
{
	if( strstr( glext, "GL_EXT_secondary_color" ) == NULL )
	{
		Com_Printf( "...GL_EXT_secondary_color not found\n" );
		return 0;
	}


	if( ! (pfglSecondaryColor3bEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3bEXT"))) return 0;
	if( ! (pfglSecondaryColor3bvEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3bvEXT"))) return 0;
	if( ! (pfglSecondaryColor3dEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3dEXT"))) return 0;
	if( ! (pfglSecondaryColor3dvEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3dvEXT"))) return 0;
	if( ! (pfglSecondaryColor3fEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3fEXT"))) return 0;
	if( ! (pfglSecondaryColor3fvEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3fvEXT"))) return 0;
	if( ! (pfglSecondaryColor3iEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3iEXT"))) return 0;
	if( ! (pfglSecondaryColor3ivEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3ivEXT"))) return 0;
	if( ! (pfglSecondaryColor3sEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3sEXT"))) return 0;
	if( ! (pfglSecondaryColor3svEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3svEXT"))) return 0;
	if( ! (pfglSecondaryColor3ubEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3ubEXT"))) return 0;
	if( ! (pfglSecondaryColor3ubvEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3ubvEXT"))) return 0;
	if( ! (pfglSecondaryColor3uiEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3uiEXT"))) return 0;
	if( ! (pfglSecondaryColor3uivEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3uivEXT"))) return 0;
	if( ! (pfglSecondaryColor3usEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3usEXT"))) return 0;
	if( ! (pfglSecondaryColor3usvEXT = ( void * )pfwglGetProcAddress("glSecondaryColor3usvEXT"))) return 0;
	if( ! (pfglSecondaryColorPointerEXT = ( void * )pfwglGetProcAddress("glSecondaryColorPointerEXT"))) return 0;

	Com_Printf( "...enabling GL_EXT_secondary_color\n" );

	return 1;
}

PRIVATE W8 ConfigEXTBlendColor( const char *glext )
{
	if( strstr( glext, "GL_EXT_blend_color" ) == NULL )
	{
		Com_Printf( "...GL_EXT_blend_color not found\n" );
		return 0;
	}

	if( ! (pfglBlendColorEXT = (void *)pfwglGetProcAddress( "glBlendColorEXT" ))) return 0;

	Com_Printf( "...enabling GL_EXT_blend_color\n" );

	return 1;
}

PRIVATE W8 ConfigEXTVertexShader( const char *glext )
{			
	GLint glnum;

	if( strstr( glext, "GL_EXT_vertex_shader" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglBeginVertexShaderEXT = ( void * )pfwglGetProcAddress("glBeginVertexShaderEXT"))) return 0;
	if( ! (pfglEndVertexShaderEXT = ( void * )pfwglGetProcAddress("glEndVertexShaderEXT"))) return 0;
	if( ! (pfglBindVertexShaderEXT = ( void * )pfwglGetProcAddress("glBindVertexShaderEXT"))) return 0;
	if( ! (pfglGenVertexShadersEXT = ( void * )pfwglGetProcAddress("glGenVertexShadersEXT"))) return 0;
	if( ! (pfglDeleteVertexShaderEXT = ( void * )pfwglGetProcAddress("glDeleteVertexShaderEXT"))) return 0;
	if( ! (pfglShaderOp1EXT = ( void * )pfwglGetProcAddress("glShaderOp1EXT"))) return 0;
	if( ! (pfglShaderOp2EXT = ( void * )pfwglGetProcAddress("glShaderOp2EXT"))) return 0;
	if( ! (pfglShaderOp3EXT = ( void * )pfwglGetProcAddress("glShaderOp3EXT"))) return 0;
	if( ! (pfglSwizzleEXT = ( void * )pfwglGetProcAddress("glSwizzleEXT"))) return 0;
	if( ! (pfglWriteMaskEXT = ( void * )pfwglGetProcAddress("glWriteMaskEXT"))) return 0;
	if( ! (pfglInsertComponentEXT = ( void * )pfwglGetProcAddress("glInsertComponentEXT"))) return 0;
	if( ! (pfglExtractComponentEXT = ( void * )pfwglGetProcAddress("glExtractComponentEXT"))) return 0;
	if( ! (pfglGenSymbolsEXT = ( void * )pfwglGetProcAddress("glGenSymbolsEXT"))) return 0;
	if( ! (pfglSetInvariantEXT = ( void * )pfwglGetProcAddress("glSetInvariantEXT"))) return 0;
	if( ! (pfglSetLocalConstantEXT = ( void * )pfwglGetProcAddress("glSetLocalConstantEXT"))) return 0;
	if( ! (pfglVariantPointerEXT = ( void * )pfwglGetProcAddress("glVariantPointerEXT"))) return 0;
	if( ! (pfglEnableVariantClientStateEXT = ( void * )pfwglGetProcAddress("glEnableVariantClientStateEXT"))) return 0;
	if( ! (pfglDisableVariantClientStateEXT = ( void * )pfwglGetProcAddress("glDisableVariantClientStateEXT"))) return 0;
	if( ! (pfglBindLightParameterEXT = ( void * )pfwglGetProcAddress("glBindLightParameterEXT"))) return 0;
	if( ! (pfglBindMaterialParameterEXT = ( void * )pfwglGetProcAddress("glBindMaterialParameterEXT"))) return 0;
	if( ! (pfglBindTexGenParameterEXT = ( void * )pfwglGetProcAddress("glBindTexGenParameterEXT"))) return 0;
	if( ! (pfglBindTextureUnitParameterEXT = ( void * )pfwglGetProcAddress("glBindTextureUnitParameterEXT"))) return 0;
	if( ! (pfglBindParameterEXT = ( void * )pfwglGetProcAddress("glBindParameterEXT"))) return 0;
	if( ! (pfglIsVariantEnabledEXT = ( void * )pfwglGetProcAddress("glIsVariantEnabledEXT"))) return 0;
	if( ! (pfglGetVariantBooleanvEXT = ( void * )pfwglGetProcAddress("glGetVariantBooleanvEXT"))) return 0;
	if( ! (pfglGetVariantIntegervEXT = ( void * )pfwglGetProcAddress("glGetVariantIntegervEXT"))) return 0;
	if( ! (pfglGetVariantFloatvEXT = ( void * )pfwglGetProcAddress("glGetVariantFloatvEXT"))) return 0;
	if( ! (pfglGetVariantPointervEXT = ( void * )pfwglGetProcAddress("glGetVariantPointervEXT"))) return 0;
	if( ! (pfglGetInvariantBooleanvEXT = ( void * )pfwglGetProcAddress("glGetInvariantBooleanvEXT"))) return 0;
	if( ! (pfglGetInvariantIntegervEXT = ( void * )pfwglGetProcAddress("glGetInvariantIntegervEXT"))) return 0;
	if( ! (pfglGetInvariantFloatvEXT = ( void * )pfwglGetProcAddress("glGetInvariantFloatvEXT"))) return 0;
	if( ! (pfglGetLocalConstantBooleanvEXT = ( void * )pfwglGetProcAddress("glGetLocalConstantBooleanvEXT"))) return 0;
	if( ! (pfglGetLocalConstantIntegervEXT = ( void * )pfwglGetProcAddress("glGetLocalConstantIntegervEXT"))) return 0;
	if( ! (pfglGetLocalConstantFloatvEXT = ( void * )pfwglGetProcAddress("glGetLocalConstantFloatvEXT"))) return 0;

//
//	Checks to see if hardware is able to run our stuff
//
	pfglGetIntegerv( GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT, &glnum );
	if( glnum < MY_GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTR ) 
	{
		return 0;
	}

	
	pfglGetIntegerv( GL_MAX_VERTEX_SHADER_LOCALS_EXT, &glnum );
	if( glnum < MY_GL_MAX_VERTEX_SHADER_LOCALS_EXT )
	{
		Com_Printf( "...Some vertex programs may fail to load" );
		return 0;
	}

	
	pfglGetIntegerv( GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT, &glnum );
	if( glnum < MY_GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT ) 
	{
		return 0;	
	}


	pfglGetIntegerv( GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT, &glnum );
	if( glnum < MY_GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT ) 
	{
		return 0;
	}


	pfglGetIntegerv( GL_MAX_VERTEX_SHADER_VARIANTS_EXT, &glnum );
	if( glnum < MY_GL_MAX_VERTEX_SHADER_VARIANTS_EXT ) 
	{
		return 0;
	}


	Com_Printf( "...enabling GL_EXT_vertex_shader\n" );

	return 1;	
}


PRIVATE W8 ConfigEXTVertexWeighting( const char *glext )
{
	if( strstr( glext, "GL_EXT_vertex_weighting" ) == NULL )
	{
		Com_Printf( "...GL_EXT_vertex_weighting not found\n" );
		return 0;
	}


	if( ! (pfglVertexWeightfEXT = ( void * )pfwglGetProcAddress( "glVertexWeightfEXT" ))) return 0;
	if( ! (pfglVertexWeightfvEXT = ( void * )pfwglGetProcAddress( "glVertexWeightfvEXT" ))) return 0;
	if( ! (pfglVertexWeightPointerEXT = ( void * )pfwglGetProcAddress( "glVertexWeightPointerEXT" ))) return 0;

	Com_Printf( "...enabling GL_EXT_vertex_weighting\n" );

	return 1;
}

PRIVATE W8 CheckEXTTextureFilterAnisotropic( const char *glext )
{
	if( strstr( glext, "GL_EXT_texture_filter_anisotropic" ) == NULL )
	{
		Com_Printf( "...GL_EXT_texture_filter_anisotropic not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_EXT_texture_filter_anisotropic\n" );

	return 1;
}

////////////////////////////////////////////////////////////





//////////////////////////////
//
//	Nvidia Functions
//
//////////////////////////////

PRIVATE W8 ConfigNVVertexArrayRange( const char *glext )
{
	// Test if VAR is present.
	if( strstr( glext, "GL_NV_vertex_array_range" ) == NULL )
	{
		return 0;
	}

	// Test if Fence
	if( strstr( glext, "GL_NV_fence" ) == NULL )
	{
		return 0;
	}


	// Setup Vertex Array Range
	if( ! (pfglFlushVertexArrayRangeNV = ( void * )pfwglGetProcAddress("glFlushVertexArrayRangeNV")))return 0;
	if( ! (pfglVertexArrayRangeNV = ( void * )pfwglGetProcAddress("glVertexArrayRangeNV")))return 0;

#ifdef _WIN32

	if( ! (pfwglAllocateMemoryNV = ( void * )pfwglGetProcAddress("wglAllocateMemoryNV"))) return 0;
	if( ! (pfwglFreeMemoryNV = ( void * )pfwglGetProcAddress("wglFreeMemoryNV"))) return 0;

#elif __unix__

	if( ! (pfwglAllocateMemoryNV = ( void * )pfwglGetProcAddress( "glXAllocateMemoryNV" ))) return 0;
    if( ! (pfwglFreeMemoryNV = ( void * )pfwglGetProcAddress( "glXFreeMemoryNV" ))) return 0;

#else

	#error "Please define OS interface to GL_NV_vertex_array_range"

#endif


	// Setup fence
	if( ! (pfglDeleteFencesNV = ( void * )pfwglGetProcAddress( "glDeleteFencesNV" ))) return 0;
	if( ! (pfglGenFencesNV = ( void * )pfwglGetProcAddress( "glGenFencesNV" ))) return 0;
	if( ! (pfglIsFenceNV = ( void * )pfwglGetProcAddress( "glIsFenceNV" ))) return 0;
	if( ! (pfglTestFenceNV = ( void * )pfwglGetProcAddress( "glTestFenceNV" ))) return 0;
	if( ! (pfglGetFenceivNV = ( void * )pfwglGetProcAddress( "glGetFenceivNV" ))) return 0;
	if( ! (pfglFinishFenceNV = ( void * )pfwglGetProcAddress( "glFinishFenceNV" ))) return 0;
	if( ! (pfglSetFenceNV = ( void * )pfwglGetProcAddress( "glSetFenceNV" ))) return 0;

	Com_Printf( "...enabling GL_NV_vertex_array_range & GL_NV_fence\n" );

	return 1;
}

PRIVATE W8	CheckNVVertexArrayRange2( const char *glext )
{
	if( strstr( glext, "GL_NV_vertex_array_range2" ) == NULL )
	{
		Com_Printf( "...GL_NV_vertex_array_range2 not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_NV_vertex_array_range2\n" );

	return 1;
}

PRIVATE W8 CheckNVTextureEnvCombine4( const char *glext )
{
	if( strstr( glext, "GL_NV_texture_env_combine4") == NULL )
	{
		Com_Printf( "...GL_NV_texture_env_combine4 not found\n" );
		return 0;
	}

	Com_Printf( "...enabling GL_NV_texture_env_combine4\n" );

	return 1;
}

PRIVATE W8 ConfigNVVertexProgram( const char *glext )
{	
	if( strstr( glext, "GL_NV_vertex_program" ) == NULL )
		return 0;


	if( ! (pfglAreProgramsResidentNV = ( void * )pfwglGetProcAddress("glAreProgramsResidentNV"))) return 0;
	if( ! (pfglBindProgramNV = ( void * )pfwglGetProcAddress("glBindProgramNV"))) return 0;
	if( ! (pfglDeleteProgramsNV = ( void * )pfwglGetProcAddress("glDeleteProgramsNV"))) return 0;
	if( ! (pfglExecuteProgramNV = ( void * )pfwglGetProcAddress("glExecuteProgramNV"))) return 0;
	if( ! (pfglGenProgramsNV = ( void * )pfwglGetProcAddress("glGenProgramsNV"))) return 0;
	if( ! (pfglGetProgramParameterdvNV = ( void * )pfwglGetProcAddress("glGetProgramParameterdvNV"))) return 0;
	if( ! (pfglGetProgramParameterfvNV = ( void * )pfwglGetProcAddress("glGetProgramParameterfvNV"))) return 0;
	if( ! (pfglGetProgramivNV = ( void * )pfwglGetProcAddress("glGetProgramivNV"))) return 0;
	if( ! (pfglGetProgramStringNV = ( void * )pfwglGetProcAddress("glGetProgramStringNV"))) return 0;
	if( ! (pfglGetTrackMatrixivNV = ( void * )pfwglGetProcAddress("glGetTrackMatrixivNV"))) return 0;
	if( ! (pfglGetVertexAttribdvNV = ( void * )pfwglGetProcAddress("glGetVertexAttribdvNV"))) return 0;
	if( ! (pfglGetVertexAttribfvNV = ( void * )pfwglGetProcAddress("glGetVertexAttribfvNV"))) return 0;
	if( ! (pfglGetVertexAttribivNV = ( void * )pfwglGetProcAddress("glGetVertexAttribivNV"))) return 0;
	if( ! (pfglGetVertexAttribPointervNV = ( void * )pfwglGetProcAddress("glGetVertexAttribPointervNV"))) return 0;
	if( ! (pfglIsProgramNV = ( void * )pfwglGetProcAddress("glIsProgramNV"))) return 0;
	if( ! (pfglLoadProgramNV = ( void * )pfwglGetProcAddress("glLoadProgramNV"))) return 0;
	if( ! (pfglProgramParameter4dNV = ( void * )pfwglGetProcAddress("glProgramParameter4dNV"))) return 0;
	if( ! (pfglProgramParameter4dvNV = ( void * )pfwglGetProcAddress("glProgramParameter4dvNV"))) return 0;
	if( ! (pfglProgramParameter4fNV = ( void * )pfwglGetProcAddress("glProgramParameter4fNV"))) return 0;
	if( ! (pfglProgramParameter4fvNV = ( void * )pfwglGetProcAddress("glProgramParameter4fvNV"))) return 0;
	if( ! (pfglProgramParameters4dvNV = ( void * )pfwglGetProcAddress("glProgramParameters4dvNV"))) return 0;
	if( ! (pfglProgramParameters4fvNV = ( void * )pfwglGetProcAddress("glProgramParameters4fvNV"))) return 0;
	if( ! (pfglRequestResidentProgramsNV = ( void * )pfwglGetProcAddress("glRequestResidentProgramsNV"))) return 0;
	if( ! (pfglTrackMatrixNV = ( void * )pfwglGetProcAddress("glTrackMatrixNV"))) return 0;
	if( ! (pfglVertexAttribPointerNV = ( void * )pfwglGetProcAddress("glVertexAttribPointerNV"))) return 0;
	if( ! (pfglVertexAttrib1dNV = ( void * )pfwglGetProcAddress("glVertexAttrib1dNV"))) return 0;
	if( ! (pfglVertexAttrib1dvNV = ( void * )pfwglGetProcAddress("glVertexAttrib1dvNV"))) return 0;
	if( ! (pfglVertexAttrib1fNV = ( void * )pfwglGetProcAddress("glVertexAttrib1fNV"))) return 0;
	if( ! (pfglVertexAttrib1fvNV = ( void * )pfwglGetProcAddress("glVertexAttrib1fvNV"))) return 0;
	if( ! (pfglVertexAttrib1sNV = ( void * )pfwglGetProcAddress("glVertexAttrib1sNV"))) return 0;
	if( ! (pfglVertexAttrib1svNV = ( void * )pfwglGetProcAddress("glVertexAttrib1svNV"))) return 0;
	if( ! (pfglVertexAttrib2dNV = ( void * )pfwglGetProcAddress("glVertexAttrib2dNV"))) return 0;
	if( ! (pfglVertexAttrib2dvNV = ( void * )pfwglGetProcAddress("glVertexAttrib2dvNV"))) return 0;
	if( ! (pfglVertexAttrib2fNV = ( void * )pfwglGetProcAddress("glVertexAttrib2fNV"))) return 0;
	if( ! (pfglVertexAttrib2fvNV = ( void * )pfwglGetProcAddress("glVertexAttrib2fvNV"))) return 0;
	if( ! (pfglVertexAttrib2sNV = ( void * )pfwglGetProcAddress("glVertexAttrib2sNV"))) return 0;
	if( ! (pfglVertexAttrib2svNV = ( void * )pfwglGetProcAddress("glVertexAttrib2svNV"))) return 0;
	if( ! (pfglVertexAttrib3dNV = ( void * )pfwglGetProcAddress("glVertexAttrib3dNV"))) return 0;
	if( ! (pfglVertexAttrib3dvNV = ( void * )pfwglGetProcAddress("glVertexAttrib3dvNV"))) return 0;
	if( ! (pfglVertexAttrib3fNV = ( void * )pfwglGetProcAddress("glVertexAttrib3fNV"))) return 0;
	if( ! (pfglVertexAttrib3fvNV = ( void * )pfwglGetProcAddress("glVertexAttrib3fvNV"))) return 0;
	if( ! (pfglVertexAttrib3sNV = ( void * )pfwglGetProcAddress("glVertexAttrib3sNV"))) return 0;
	if( ! (pfglVertexAttrib3svNV = ( void * )pfwglGetProcAddress("glVertexAttrib3svNV"))) return 0;
	if( ! (pfglVertexAttrib4dNV = ( void * )pfwglGetProcAddress("glVertexAttrib4dNV"))) return 0;
	if( ! (pfglVertexAttrib4dvNV = ( void * )pfwglGetProcAddress("glVertexAttrib4dvNV"))) return 0;
	if( ! (pfglVertexAttrib4fNV = ( void * )pfwglGetProcAddress("glVertexAttrib4fNV"))) return 0;
	if( ! (pfglVertexAttrib4fvNV = ( void * )pfwglGetProcAddress("glVertexAttrib4fvNV"))) return 0;
	if( ! (pfglVertexAttrib4sNV = ( void * )pfwglGetProcAddress("glVertexAttrib4sNV"))) return 0;
	if( ! (pfglVertexAttrib4svNV = ( void * )pfwglGetProcAddress("glVertexAttrib4svNV"))) return 0;
	if( ! (pfglVertexAttrib4ubvNV = ( void * )pfwglGetProcAddress("glVertexAttrib4ubvNV"))) return 0;
	if( ! (pfglVertexAttribs1dvNV = ( void * )pfwglGetProcAddress("glVertexAttribs1dvNV"))) return 0;
	if( ! (pfglVertexAttribs1fvNV = ( void * )pfwglGetProcAddress("glVertexAttribs1fvNV"))) return 0;
	if( ! (pfglVertexAttribs1svNV = ( void * )pfwglGetProcAddress("glVertexAttribs1svNV"))) return 0;
	if( ! (pfglVertexAttribs2dvNV = ( void * )pfwglGetProcAddress("glVertexAttribs2dvNV"))) return 0;
	if( ! (pfglVertexAttribs2fvNV = ( void * )pfwglGetProcAddress("glVertexAttribs2fvNV"))) return 0;
	if( ! (pfglVertexAttribs2svNV = ( void * )pfwglGetProcAddress("glVertexAttribs2svNV"))) return 0;
	if( ! (pfglVertexAttribs3dvNV = ( void * )pfwglGetProcAddress("glVertexAttribs3dvNV"))) return 0;
	if( ! (pfglVertexAttribs3fvNV = ( void * )pfwglGetProcAddress("glVertexAttribs3fvNV"))) return 0;
	if( ! (pfglVertexAttribs3svNV = ( void * )pfwglGetProcAddress("glVertexAttribs3svNV"))) return 0;
	if( ! (pfglVertexAttribs4dvNV = ( void * )pfwglGetProcAddress("glVertexAttribs4dvNV"))) return 0;
	if( ! (pfglVertexAttribs4fvNV = ( void * )pfwglGetProcAddress("glVertexAttribs4fvNV"))) return 0;
	if( ! (pfglVertexAttribs4svNV = ( void * )pfwglGetProcAddress("glVertexAttribs4svNV"))) return 0;
	if( ! (pfglVertexAttribs4ubvNV = ( void * )pfwglGetProcAddress("glVertexAttribs4ubvNV"))) return 0;


	Com_Printf( "...enabling GL_NV_vertex_program\n" );

	return 1;
}





PRIVATE W8 CheckNVTextureShader( const char *glext )
{
	if( strstr( glext, "GL_NV_texture_shader" ) == NULL )
	{
		return 0;
	}

	Com_Printf( "...enabling GL_NV_texture_shader\n" );

	return 1;
}

////////////////////////////////////////////////////////////


//////////////////////////////
//
//	ATI Functions
//
//////////////////////////////


PRIVATE W8 ConfigATIVertexArrayObject( const char *glext )
{
	if( strstr( glext, "GL_ATI_vertex_array_object" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglBlendColorEXT = ( void * )pfwglGetProcAddress("glBlendColorEXT"))) return 0;
	if( ! (pfglNewObjectBufferATI = ( void * )pfwglGetProcAddress("glNewObjectBufferATI"))) return 0;
	if( ! (pfglIsObjectBufferATI = ( void * )pfwglGetProcAddress("glIsObjectBufferATI"))) return 0;
	if( ! (pfglUpdateObjectBufferATI = ( void * )pfwglGetProcAddress("glUpdateObjectBufferATI"))) return 0;
	if( ! (pfglGetObjectBufferfvATI = ( void * )pfwglGetProcAddress("glGetObjectBufferfvATI"))) return 0;
	if( ! (pfglGetObjectBufferivATI = ( void * )pfwglGetProcAddress("glGetObjectBufferivATI"))) return 0;
	
	if( ! (pfglFreeObjectBufferATI = ( void * )pfwglGetProcAddress("glFreeObjectBufferATI"))) return 0;
	
	if( ! (pfglArrayObjectATI = ( void * )pfwglGetProcAddress("glArrayObjectATI"))) return 0;
	if( ! (pfglGetArrayObjectfvATI = ( void * )pfwglGetProcAddress("glGetArrayObjectfvATI"))) return 0;
	if( ! (pfglGetArrayObjectivATI = ( void * )pfwglGetProcAddress("glGetArrayObjectivATI"))) return 0;
	if( ! (pfglVariantArrayObjectATI = ( void * )pfwglGetProcAddress("glVariantArrayObjectATI"))) return 0;
	if( ! (pfglGetVariantArrayObjectfvATI = ( void * )pfwglGetProcAddress("glGetVariantArrayObjectfvATI"))) return 0;
	if( ! (pfglGetVariantArrayObjectivATI = ( void * )pfwglGetProcAddress("glGetVariantArrayObjectivATI"))) return 0;

	Com_Printf( "...enabling GL_ATI_vertex_array_object\n" );

	return 1;
}


PRIVATE W8 ConfigATIMapObjectBuffer( const char *glext )
{		
	if( strstr( glext, "GL_ATI_map_object_buffer" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglMapObjectBufferATI = ( void * )pfwglGetProcAddress("glMapObjectBufferATI"))) return 0;
	if( ! (pfglUnmapObjectBufferATI = ( void * )pfwglGetProcAddress("glUnmapObjectBufferATI"))) return 0;
	
	Com_Printf( "...enabling GL_ATI_map_object_buffer\n" );

	return 1;	
}




PRIVATE W8 ConfigATIFragmentShader( const char *glext )
{
	if( strstr( glext, "GL_ATI_fragment_shader" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglGenFragmentShadersATI = ( void * )pfwglGetProcAddress("glGenFragmentShadersATI"))) return 0;
	if( ! (pfglBindFragmentShaderATI = ( void * )pfwglGetProcAddress("glBindFragmentShaderATI"))) return 0;
	if( ! (pfglDeleteFragmentShaderATI = ( void * )pfwglGetProcAddress("glDeleteFragmentShaderATI"))) return 0;
	if( ! (pfglBeginFragmentShaderATI = ( void * )pfwglGetProcAddress("glBeginFragmentShaderATI"))) return 0;
	if( ! (pfglEndFragmentShaderATI = ( void * )pfwglGetProcAddress("glEndFragmentShaderATI"))) return 0;
	if( ! (pfglPassTexCoordATI = ( void * )pfwglGetProcAddress("glPassTexCoordATI"))) return 0;
	if( ! (pfglSampleMapATI = ( void * )pfwglGetProcAddress("glSampleMapATI"))) return 0;
	if( ! (pfglColorFragmentOp1ATI = ( void * )pfwglGetProcAddress("glColorFragmentOp1ATI"))) return 0;
	if( ! (pfglColorFragmentOp2ATI = ( void * )pfwglGetProcAddress("glColorFragmentOp2ATI"))) return 0;
	if( ! (pfglColorFragmentOp3ATI = ( void * )pfwglGetProcAddress("glColorFragmentOp3ATI"))) return 0;
	if( ! (pfglAlphaFragmentOp1ATI = ( void * )pfwglGetProcAddress("glAlphaFragmentOp1ATI"))) return 0;
	if( ! (pfglAlphaFragmentOp2ATI = ( void * )pfwglGetProcAddress("glAlphaFragmentOp2ATI"))) return 0;
	if( ! (pfglAlphaFragmentOp3ATI = ( void * )pfwglGetProcAddress("glAlphaFragmentOp3ATI"))) return 0;
	if( ! (pfglSetFragmentShaderConstantATI = ( void * )pfwglGetProcAddress("glSetFragmentShaderConstantATI"))) return 0;

	Com_Printf( "...enabling GL_ATI_fragment_shader\n" );

	return 1;
}

PRIVATE W8 CheckATITextureEnvCombine3( const char *glext )
{
	if( strstr( glext, "GL_ATI_texture_env_combine3" ) == NULL )
	{
		return 0;
	}

	Com_Printf( "...enabling GL_ATI_texture_env_combine3\n" );

	return 1;
}


PRIVATE W8 CheckATIXTextureEnvRoute(const char *glext)
{	
	if( strstr( glext, "GL_ATIX_texture_env_route" ) == NULL )
	{
		return 0;
	}

	Com_Printf( "...enabling GL_ATIX_texture_env_route\n" );
	
	return 1;
}


PRIVATE W8 ConfigATIEnvMapBumpMap( const char *glext )
{
	if( strstr( glext, "GL_ATI_envmap_bumpmap" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglTexBumpParameterivATI = ( void * ) pfwglGetProcAddress("glTexBumpParameterivATI"))) return 0;
	if( ! (pfglTexBumpParameterfvATI = ( void * ) pfwglGetProcAddress("glTexBumpParameterfvATI"))) return 0;
	if( ! (pfglGetTexBumpParameterivATI = ( void * ) pfwglGetProcAddress("glGetTexBumpParameterivATI"))) return 0;
	if( ! (pfglGetTexBumpParameterfvATI = ( void * ) pfwglGetProcAddress("glGetTexBumpParameterfvATI"))) return 0;		

	Com_Printf( "...enabling GL_ATI_envmap_bumpmap\n" );

	return 1;
}
/*
PRIVATE W8 ConfigARBFragmentProgram( const char *glext )
{
	if( strstr( glext, "GL_ARB_fragment_program" ) == NULL )
	{
		return 0;
	}

	if( ! (pfglProgramStringARB = ( void * )pfwglGetProcAddress("glProgramStringARB"))) return 0;
	if( ! (pfglBindProgramARB = ( void * )pfwglGetProcAddress("glBindProgramARB"))) return 0;
	if( ! (pfglDeleteProgramsARB = ( void * )pfwglGetProcAddress("glDeleteProgramsARB"))) return 0;
	if( ! (pfglGenProgramsARB = ( void * )pfwglGetProcAddress("glGenProgramsARB"))) return 0;
	if( ! (pfglProgramEnvParameter4dARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4dARB"))) return 0;
	if( ! (pfglProgramEnvParameter4dvARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4dvARB"))) return 0;
	if( ! (pfglProgramEnvParameter4fARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4fARB"))) return 0;
	if( ! (pfglProgramEnvParameter4fvARB = ( void * )pfwglGetProcAddress("glProgramEnvParameter4fvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameter4dARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4dARB"))) return 0;
	if( ! (pfglGetProgramLocalParameter4dvARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4dvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameter4fARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4fARB"))) return 0;
	if( ! (pfglGetProgramLocalParameter4fvARB = ( void * )pfwglGetProcAddress("glProgramLocalParameter4fvARB"))) return 0;
	if( ! (pfglGetProgramEnvParameterdvARB = ( void * )pfwglGetProcAddress("glGetProgramEnvParameterdvARB"))) return 0;
	if( ! (pfglGetProgramEnvParameterfvARB = ( void * )pfwglGetProcAddress("glGetProgramEnvParameterfvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameterdvARB = ( void * )pfwglGetProcAddress("glGetProgramLocalParameterdvARB"))) return 0;
	if( ! (pfglGetProgramLocalParameterfvARB = ( void * )pfwglGetProcAddress("glGetProgramLocalParameterfvARB"))) return 0;
	if( ! (pfglGetProgramivARB = ( void * )pfwglGetProcAddress("glGetProgramivARB"))) return 0;
	if( ! (pfglGetProgramStringARB = ( void * )pfwglGetProcAddress("glGetProgramStringARB"))) return 0;
	if( ! (pfglIsProgramARB = ( void * )pfwglGetProcAddress("glIsProgramARB"))) return 0;

	return 1;
}


PRIVATE W8 ConfigARBVertexBufferObject( const char *glext )
{
	if( strstr( glext, "GL_ARB_vertex_buffer_object" ) == NULL )
		return 0;

	if( ! (pfglBindBufferARB = ( void * )pfwglGetProcAddress("glBindBufferARB"))) return 0;
	if( ! (pfglDeleteBuffersARB = ( void * )pfwglGetProcAddress("glDeleteBuffersARB"))) return 0;
	if( ! (pfglGenBuffersARB = ( void * )pfwglGetProcAddress("glGenBuffersARB"))) return 0;
	if( ! (pfglIsBufferARB = ( void * )pfwglGetProcAddress("glIsBufferARB"))) return 0;
	if( ! (pfglBufferDataARB = ( void * )pfwglGetProcAddress("glBufferDataARB"))) return 0;
	if( ! (pfglBufferSubDataARB = ( void * )pfwglGetProcAddress("glBufferSubDataARB"))) return 0;
	if( ! (pfglGetBufferSubDataARB = ( void * )pfwglGetProcAddress("glGetBufferSubDataARB"))) return 0;
	if( ! (pfglMapBufferARB = ( void * )pfwglGetProcAddress("glMapBufferARB"))) return 0;
	if( ! (pfglUnmapBufferARB = ( void * )pfwglGetProcAddress("glUnmapBufferARB"))) return 0;
	if( ! (pfglGetBufferParameterivARB = ( void * )pfwglGetProcAddress("glGetBufferParameterivARB"))) return 0;
	if( ! (pfglGetBufferPointervARB = ( void * )pfwglGetProcAddress("glGetBufferPointervARB"))) return 0;	
	
	return 1;
}*/




#ifdef _WIN32

PRIVATE W8 ConfigWGLARBPBuffer( const char *glext )
{
	if( strstr( glext, "WGL_ARB_pbuffer" ) == NULL )
	{
		Com_Printf( "...WGL_ARB_pbuffer not found\n" );
		return 0;
	}

	if( ! (pfwglCreatePbufferARB = ( void * )pfwglGetProcAddress( "wglCreatePbufferARB" ))) return 0;
	if( ! (pfwglGetPbufferDCARB = ( void * )pfwglGetProcAddress( "wglGetPbufferDCARB" ))) return 0;
	if( ! (pfwglReleasePbufferDCARB = ( void * )pfwglGetProcAddress( "wglReleasePbufferDCARB" ))) return 0;
	if( ! (pfwglDestroyPbufferARB = ( void * )pfwglGetProcAddress( "wglDestroyPbufferARB" ))) return 0;
	if( ! (pfwglQueryPbufferARB = ( void * )pfwglGetProcAddress( "wglQueryPbufferARB" ))) return 0;

	Com_Printf( "...enabling WGL_ARB_pbuffer\n" );

	return 1;
}


PRIVATE W8 ConfigWGLARBPixelFormat ( const char *glext )
{
	if( strstr( glext, "WGL_ARB_pixel_format" ) == NULL )
	{
		Com_Printf( "...WGL_ARB_pixel_format not found\n" );
		return 0;
	}

	if( ! (pfwglGetPixelFormatAttribivARB = ( void * )pfwglGetProcAddress( "wglGetPixelFormatAttribivARB" ))) return 0;
	if( ! (pfwglGetPixelFormatAttribfvARB = ( void * )pfwglGetProcAddress( "wglGetPixelFormatAttribfvARB" ))) return 0;
	if( ! (pfwglChoosePixelFormatARB = ( void * )pfwglGetProcAddress( "wglChoosePixelFormatARB" ))) return 0;

	Com_Printf( "...enabling WGL_ARB_pixel_format\n" );

	return 1;
}

PRIVATE W8 ConfigWGLEXTSwapControl( const char *glext )
{
	if( strstr( glext, "WGL_EXT_swap_control" ) == NULL )
	{
		Com_Printf( "...WGL_EXT_swap_control not found\n" );
		return 0;
	}

	if( ! (pfwglSwapIntervalEXT = (WGLSWAPINTERVALEXT)pfwglGetProcAddress( "wglSwapIntervalEXT" ) ) ) return 0;
	if( ! (pfwglGetSwapIntervalEXT = (WGLGETSWAPINTERVALEXT)pfwglGetProcAddress( "wglGetSwapIntervalEXT" ) ) ) return 0;

	Com_Printf( "...enabling WGL_EXT_swap_control\n" );

	return 1;
}

#endif /* _WIN32 */










/*
-----------------------------------------------------------------------------
 Function: GL_ConfigExtensions -
 
 Parameters: glext -[in] Pointer to string containing the gl extension names.            
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void GL_ConfigExtensions( const char *glext )
{
	GLint ntext;


	memset( &gl_ext, 0, sizeof( GL_Extensions ) );

	gl_ext.ARBMultiTexture = ConfigARBMultiTexture( glext );
	if( gl_ext.ARBMultiTexture )
	{
		pfglGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &ntext );
		
		// There could be more than MY_MAX_ARB_TEXTURESTAGES, but we only 
		// require MY_MAX_ARB_TEXTURESTAGES texture stages so take min.
		gl_ext.nTextureStages = ( ntext < ( (GLint)MY_MAX_ARB_TEXTURESTAGES) ? ntext : MY_MAX_ARB_TEXTURESTAGES );		
	}
	

	gl_ext.EXTTextureFilterAnisotropic = CheckEXTTextureFilterAnisotropic( glext );
	if( gl_ext.EXTTextureFilterAnisotropic )
	{
		pfglGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &gl_ext.nMaxAnisotropy );

		if( gl_ext.nMaxAnisotropy < 2.0f )
		{
			gl_ext.nMaxAnisotropy = 0;
		}
			
	}
	

	gl_ext.EXTTextureEnvCombine = CheckEXTTextureEnvCombine( glext );


	gl_ext.ARBTextureCompression = ConfigARBTextureCompression( glext );


	gl_ext.EXTTextureCompressionS3TC = (gl_ext.ARBTextureCompression && CheckEXTTextureCompressionS3TC( glext ) ); 


	gl_ext.ARBTextureCubeMap = CheckARBTextureCubeMap( glext );


	gl_ext.EXTSecondaryColor = ConfigEXTSecondaryColor( glext );


	gl_ext.EXTBlendColor = ConfigEXTBlendColor( glext );


	// nVidia extensions
	// -----------------


	gl_ext.NVVertexArrayRange = ConfigNVVertexArrayRange( glext );	
	if( gl_ext.NVVertexArrayRange )
	{
		GLint nverts;
		pfglGetIntegerv( (GLenum)GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV, &nverts );
		gl_ext.NVVertexArrayRangeMaxVertex = nverts;
	}


	gl_ext.EXTVertexWeighting = ConfigEXTVertexWeighting( glext );


	gl_ext.NVVertexArrayRange2 = CheckNVVertexArrayRange2( glext );
	if( gl_ext.NVVertexArrayRange2 )
	{
		// VBHard swap without flush of the VAR.
		gl_ext.NVStateVARWithoutFlush = GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV;
	}
	else
	{
		// VBHard with unusefull flush of the VAR.
		gl_ext.NVStateVARWithoutFlush = GL_VERTEX_ARRAY_RANGE_NV;
	}


	gl_ext.NVTextureEnvCombine4 = CheckNVTextureEnvCombine4( glext );


	if( ! gl_ext.DisableHardwareVertexProgram )
	{		
		gl_ext.NVVertexProgram = ConfigNVVertexProgram( glext );
		gl_ext.EXTVertexShader = ConfigEXTVertexShader( glext );
		gl_ext.ARBVertexProgram= ConfigARBVertexProgram( glext );				
	}
	else
	{
		gl_ext.NVVertexProgram = false;
		gl_ext.EXTVertexShader = false;
		gl_ext.ARBVertexProgram = false;
	}

	
	gl_ext.NVTextureShader = CheckNVTextureShader( glext );
	if( gl_ext.NVVertexProgram && ! gl_ext.NVTextureShader )
	{
		// if GL_NV_texture_shader is not here, 
		// then we are not on a GeForce3.
		gl_ext.NVVertexProgramEmulated = true;			
	}


	// ATI extensions
	// --------------

	gl_ext.ATIVertexArrayObject = ConfigATIVertexArrayObject( glext );
	gl_ext.ATIMapObjectBuffer = ConfigATIMapObjectBuffer( glext );		
	
	
	gl_ext.ATITextureEnvCombine3 = CheckATITextureEnvCombine3( glext );
	
	gl_ext.ATIXTextureEnvRoute = CheckATIXTextureEnvRoute( glext );
	
	gl_ext.ATIEnvMapBumpMap = ConfigATIEnvMapBumpMap( glext );
	
//	gl_ext.ATIFragmentShader = setupATIFragmentShader( glext );



	


#ifdef _WIN32


	ConfigWGLARBPBuffer( glext );


	ConfigWGLARBPixelFormat( glext );


	ConfigWGLEXTSwapControl( glext );


#endif

	

	
}
