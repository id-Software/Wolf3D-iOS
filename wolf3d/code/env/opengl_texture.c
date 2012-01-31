/*

	Copyright (C) 2004 Michael Liebscher
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

/*
 *	opengl_texture.c:  OpenGL Texture Manager.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */
 
#include "../wolfiphone.h"

int currentTextures[ 4 ];
int currenttmu;

int glMaxTexSize;

// ***************************************************************************

// ***************************************************************************




/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_DeleteTexture( unsigned int texnum )
{
	pfglDeleteTextures( 1, &texnum );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_TexEnv( GLenum mode )
{
	static int lastmodes[ 4 ] = { -1, -1, -1, -1 };

	if ( mode != lastmodes[ currenttmu ] )
	{
		pfglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode );
		lastmodes[ currenttmu ] = mode;
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
PUBLIC void R_SelectTexture( GLenum texture )
{
	int tmu;

#ifndef IPHONE
	if( ! pfglActiveTextureARB )
	{
		return;
	}
#endif

	if( texture == GL_TEXTURE0 )
	{
		tmu = 0;
	}
	else if( texture == GL_TEXTURE1 )
	{
		tmu = 1;
	}
	else if( texture == GL_TEXTURE2 )
	{
		tmu = 2;
	}
	else
	{
		tmu = 3;
	}

	if( tmu == currenttmu )
	{
		return;
	}
	

	currenttmu = tmu;

	pfglActiveTextureARB( texture );
	pfglClientActiveTextureARB( texture );

}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_Bind( int texnum )
{
	// Is this texture already bound 
	if( currentTextures[ currenttmu ] == texnum )
	{
		return;
	}

	currentTextures[ currenttmu ] = texnum;

	pfglBindTexture( GL_TEXTURE_2D, texnum );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_MBind( GLenum target, int texnum )
{
	R_SelectTexture( target );

	if( target == GL_TEXTURE0 )
	{
		if ( currentTextures[ 0 ] == texnum )
		{
			return;
		}
	}
	else if( target == GL_TEXTURE1 )
	{
		if( currentTextures[ 1 ] == texnum )
		{
			return;
		}
	}
	else if( target == GL_TEXTURE2 )
	{
		if( currentTextures[ 2 ] == texnum )
		{
			return;
		}
	}
	else
	{
		if( currentTextures[ 3 ] == texnum )
		{
			return;
		}
	}

	R_Bind( texnum );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_EnableMultitexture( _boolean enable )
{
#ifndef IPHONE
	if( ! pfglActiveTextureARB )
	{
		return;
	}
#endif
	
	if( enable )
	{
		R_SelectTexture( GL_TEXTURE1 );
		pfglEnable( GL_TEXTURE_2D );
		R_TexEnv( GL_REPLACE );
	}
	else
	{
		R_SelectTexture( GL_TEXTURE1 );
		pfglDisable( GL_TEXTURE_2D );
		R_TexEnv( GL_REPLACE );
	}

	R_SelectTexture( GL_TEXTURE0 );
	R_TexEnv( GL_REPLACE );
	
}

