/*

	Copyright (C) 1997-2001 Id Software, Inc.

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

#include "../wolfiphone.h"


viddef_t	viddef;

float		gldepthmin, gldepthmax;

glconfig_t gl_config;
glstate_t  gl_state;





//
// view origin
//
vec3_t	vup;
vec3_t	vpn;
vec3_t	vright;
vec3_t	r_origin;

cvar_t	*r_norefresh;
cvar_t	*r_speeds;
cvar_t	*r_novis;
cvar_t	*r_nocull;
cvar_t	*r_lefthand;


cvar_t	*gl_nosubimage;


cvar_t	*gl_vertex_arrays;



cvar_t	*gl_ext_swapinterval;
cvar_t	*gl_ext_palettedtexture;
cvar_t	*gl_ext_multitexture;
cvar_t	*gl_ext_pointparameters;
cvar_t	*gl_ext_compiled_vertex_array;

//cvar_t	*gl_ext_TextureCompressionS3TC;

cvar_t	*gl_bitdepth;
cvar_t	*gl_drawbuffer;
cvar_t  *gl_driver;
cvar_t	*gl_lightmap;
cvar_t	*gl_shadows;
cvar_t	*gl_mode;
cvar_t	*gl_dynamic;
cvar_t	*gl_modulate;
cvar_t	*gl_nobind;
cvar_t	*gl_round_down;
cvar_t	*gl_picmip;
cvar_t	*gl_skymip;
cvar_t	*gl_showtris;
cvar_t	*gl_ztrick;
cvar_t	*gl_finish;
cvar_t	*gl_clear;
cvar_t	*gl_cull;
cvar_t	*gl_polyblend;
cvar_t	*gl_flashblend;
cvar_t	*gl_playermip;
cvar_t  *gl_saturatelighting;
cvar_t	*gl_swapinterval;
cvar_t	*gl_texturemode;

cvar_t	*r_fullscreen;
cvar_t	*vid_gamma;
cvar_t	*r_ref;


/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void MYgluPerspective( GLdouble fovy, GLdouble aspect,
		     GLdouble zNear, GLdouble zFar )
{
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan( fovy * M_PI / 360.0 );
   ymin = -ymax;

   xmin = ymin * aspect;
   xmax = ymax * aspect;

   xmin += -( 2 * 0 ) / zNear;
   xmax += -( 2 * 0 ) / zNear;

   pfglFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void R_ScreenShot_f( void ) 
{
	W8		*buffer;
	char	picname[ 80 ]; 
	char	checkname[ MAX_OSPATH ];
	int		i;
	FILE	*f;

	// create the scrnshots directory if it doesn't exist
	my_snprintf( checkname, sizeof( checkname ), "%s/scrnshot", FS_Gamedir() );
	FS_CreateDirectory( checkname );

// 
// find a file name to save it to 
// 
	my_strlcpy( picname, "scrn00.tga", sizeof( picname ) );

	for( i = 0 ; i <= 99 ; ++i ) 
	{ 
		picname[ 4 ] = i / 10 + '0'; 
		picname[ 5 ] = i % 10 + '0'; 
		my_snprintf( checkname, sizeof( checkname ), "%s/scrnshot/%s", FS_Gamedir(), picname );
		f = fopen( checkname, "rb" );
		if( ! f )
		{
			break;	// file doesn't exist
		}

		fclose( f );
	} 

	if( i == 100 ) 
	{
		Com_Printf( "R_ScreenShot_f: Couldn't create a file\n" ); 
		return;
 	}


	buffer = MM_MALLOC( viddef.width * viddef.height * 3 );
	

	pfglReadPixels( 0, 0, viddef.width, viddef.height, GL_RGB, GL_UNSIGNED_BYTE, buffer ); 

	
	WriteTGA( checkname, 24, viddef.width, viddef.height, buffer, 1, 1 );
	

	MM_FREE( buffer );
	Com_Printf( "Wrote %s\n", picname );
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void R_Strings_f( void )
{
	Com_Printf( "GL_VENDOR: %s\n", gl_config.vendor_string );
	Com_Printf( "GL_RENDERER: %s\n", gl_config.renderer_string );
	Com_Printf( "GL_VERSION: %s\n", gl_config.version_string );
	Com_Printf( "GL_EXTENSIONS: %s\n", gl_config.extensions_string );
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void R_Register( void )
{
	gl_round_down = Cvar_Get ("gl_round_down", "1", CVAR_INIT);
	
	r_lefthand = Cvar_Get( "hand", "0", CVAR_USERINFO | CVAR_ARCHIVE );
	r_norefresh = Cvar_Get ("r_norefresh", "0", CVAR_INIT);
	r_novis = Cvar_Get ("r_novis", "0", CVAR_INIT);
	r_nocull = Cvar_Get ("r_nocull", "0", CVAR_INIT);
	r_speeds = Cvar_Get ("r_speeds", "0", CVAR_INIT);


	gl_nosubimage = Cvar_Get( "gl_nosubimage", "0", CVAR_INIT );


	gl_modulate = Cvar_Get ("gl_modulate", "1", CVAR_ARCHIVE );
	gl_bitdepth = Cvar_Get( "gl_bitdepth", "0", CVAR_INIT );
	gl_mode = Cvar_Get( "gl_mode", "0", CVAR_ARCHIVE );
	gl_lightmap = Cvar_Get ("gl_lightmap", "0", CVAR_INIT);
	gl_dynamic = Cvar_Get ("gl_dynamic", "1", CVAR_INIT);
	gl_nobind = Cvar_Get ("gl_nobind", "0", CVAR_INIT);
	gl_picmip = Cvar_Get ("gl_picmip", "0", CVAR_INIT);
	gl_skymip = Cvar_Get ("gl_skymip", "0", CVAR_INIT);
	gl_showtris = Cvar_Get( "gl_showtris", "0", CVAR_INIT );
	gl_ztrick = Cvar_Get( "gl_ztrick", "0", CVAR_INIT );
	gl_finish = Cvar_Get( "gl_finish", "0", CVAR_ARCHIVE );
	gl_clear = Cvar_Get( "gl_clear", "0", CVAR_INIT );
	gl_cull = Cvar_Get( "gl_cull", "1", CVAR_INIT );
	gl_polyblend = Cvar_Get( "gl_polyblend", "1", CVAR_INIT );
	gl_flashblend = Cvar_Get( "gl_flashblend", "0", CVAR_INIT );
	gl_playermip = Cvar_Get( "gl_playermip", "0", CVAR_INIT );
	gl_driver = Cvar_Get( "gl_driver", OPENGL_DLL_NAME, CVAR_ARCHIVE );

	gl_vertex_arrays = Cvar_Get( "gl_vertex_arrays", "0", CVAR_ARCHIVE );

	gl_ext_swapinterval = Cvar_Get( "gl_ext_swapinterval", "1", CVAR_ARCHIVE );
	gl_ext_palettedtexture = Cvar_Get( "gl_ext_palettedtexture", "1", CVAR_ARCHIVE );
	gl_ext_multitexture = Cvar_Get( "gl_ext_multitexture", "1", CVAR_ARCHIVE );
	gl_ext_pointparameters = Cvar_Get( "gl_ext_pointparameters", "1", CVAR_ARCHIVE );
	gl_ext_compiled_vertex_array = Cvar_Get( "gl_ext_compiled_vertex_array", "1", CVAR_ARCHIVE );

	gl_drawbuffer = Cvar_Get( "gl_drawbuffer", "GL_BACK", CVAR_INIT );
	gl_swapinterval = Cvar_Get( "gl_swapinterval", "1", CVAR_ARCHIVE );

//	gl_saturatelighting = Cvar_Get( "gl_saturatelighting", "0", CVAR_INIT );

	r_fullscreen = Cvar_Get( "r_fullscreen", "0", CVAR_ARCHIVE );
	vid_gamma = Cvar_Get( "vid_gamma", "1.0", CVAR_ARCHIVE );
	r_ref = Cvar_Get( "r_ref", "gl", CVAR_ARCHIVE );

	Cmd_AddCommand( "screenshot", R_ScreenShot_f );
	Cmd_AddCommand( "r_strings", R_Strings_f );
}



/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void R_Init()
{	
	char renderer_buffer[ 1000 ];
	char vendor_buffer[ 1000 ];
	int		err;
	int		a, b;


	Com_Printf( "\n------ Display Initialization ------\n" );

	Com_Printf( "Initializing OpenGL Subsystem\n" );
	
	R_Register();

	// set our "safe" modes
	gl_state.prev_mode = 0;

	viddef.width = 480;
	viddef.height = 320;

	//	get various GL strings
	gl_config.vendor_string = (char *)pfglGetString( GL_VENDOR );
	Com_Printf( "GL_VENDOR: %s\n", gl_config.vendor_string );
	
	gl_config.renderer_string = (char *)pfglGetString( GL_RENDERER );
	Com_Printf( "GL_RENDERER: %s\n", gl_config.renderer_string );
	
	gl_config.version_string = (char *)pfglGetString( GL_VERSION );
	Com_Printf( "GL_VERSION: %s\n", gl_config.version_string );
	
	gl_config.extensions_string = (char *)pfglGetString( GL_EXTENSIONS );
	Com_Printf( "GL_EXTENSIONS: %s\n", gl_config.extensions_string );

	my_strlcpy( renderer_buffer, gl_config.renderer_string, sizeof( renderer_buffer ) );
	(void)my_strlwr( renderer_buffer );

	my_strlcpy( vendor_buffer, gl_config.vendor_string, sizeof( vendor_buffer ) );
	(void)my_strlwr( vendor_buffer );


	sscanf( gl_config.version_string, "%d.%d", &a, &b );
	if( a >= 1 && b >= 2 )
	{
		gl_config.Version_1_2 = true;
	}

	pfglGetIntegerv( GL_MAX_TEXTURE_SIZE, &glMaxTexSize );
	Com_Printf( "GL_MAX_TEXTURE_SIZE: %d\n", glMaxTexSize);

	GL_SetDefaultState();

	TM_Init();
	Font_Init();

	err = pfglGetError();
	if( err != GL_NO_ERROR )
	{
		Com_Printf( "glGetError() = 0x%x\n", err );
	}
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void R_Shutdown( void )
{	
//	Cmd_RemoveCommand ("modellist");
//	Cmd_RemoveCommand ("screenshot");
//	Cmd_RemoveCommand ("imagelist");
//	Cmd_RemoveCommand ("gl_strings");

//	Mod_FreeAll ();

	TM_Shutdown();

	/*
	** shut down OS specific OpenGL stuff like contexts, etc.
	*/
	GLimp_Shutdown();

	/*
	** shutdown our OpenGL subsystem
	*/
#ifndef IPHONE
	OpenGL_Shutdown();
#endif	
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void R_EndFrame( void )
{
	GLimp_EndFrame();
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void R_AppActivate( _boolean active )
{
	GLimp_AppActivate( active );
}


/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void GL_UpdateSwapInterval( void )
{
	if ( gl_swapinterval->modified )
	{
		gl_swapinterval->modified = false;

#ifdef _WIN32

		if ( pfwglSwapIntervalEXT )
		{
			pfwglSwapIntervalEXT( FloatToInt( gl_swapinterval->value ) );
		}

#endif

	}
}



/*
-----------------------------------------------------------------------------
 Function: PrintGLError -Print OpenGL error message.

 Parameters: err -[in] Error code.
			 from -[in] function name that produced the error.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void PrintGLError( W32 err, const char *from )
{
    if( err == GL_NO_ERROR )
	{
		return;
	}

    if( from != "" )
	{
		Com_Printf( "\n\n\nGL Error: %s\n", from );
	}

	switch( err )
	{
		case GL_NO_ERROR:
			Com_Printf( "GL_NO_ERROR:\nNo error has been recorded. The value of this symbolic constant is guaranteed to be zero.\n" );			
			break;

		case GL_INVALID_ENUM:
			Com_Printf( "GL_INVALID_ENUM:\nAn unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag.\n" );
			break;

		case GL_INVALID_VALUE:
			Com_Printf( "GL_INVALID_VALUE:\nA numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag.\n" );
			break;

		case GL_INVALID_OPERATION:
			Com_Printf( "GL_INVALID_OPERATION:\nThe specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag.\n" );
			break;

		case GL_STACK_OVERFLOW:
			Com_Printf( "GL_STACK_OVERFLOW:\nThis function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag.\n" );
			break;

		case GL_STACK_UNDERFLOW:
			Com_Printf( "GL_STACK_UNDERFLOW:\nThis function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag.\n" );
			break;

		case GL_OUT_OF_MEMORY:
			Com_Printf( "GL_OUT_OF_MEMORY:\nThere is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded.\n" );		
			break;

		default:
			Com_Printf( "Unknown GL error flag 0x%x\n", err );
	}
}
