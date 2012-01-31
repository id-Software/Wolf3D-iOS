/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	opengl_draw.c:	OpenGL drawing routines.
 *	
 *	Author:	Michael Liebscher
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */


#include "../wolfiphone.h"

texture_t		*draw_chars;

//extern	_boolean	scrap_dirty;
//void Scrap_Upload (void);



/*
-----------------------------------------------------------------------------
 Function: R_Draw_Char -Draw ASCII character to the screen. 
 
 Parameters: x -[in] x-coordinate.
			 y -[in] y-coordinate. 
			 num -[in] ASCII character value.
			 myfont -[in] Valid pointer to font_t structure.
 
 Returns: Nothing.
 
 Notes: 
	It can be clipped to the top of the screen to allow the console to be
	smoothly scrolled off.

-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_Character( int x, int y, int num, font_t *myfont )
{
	int	row, col; 
	int scale, sh; // scaled width, height
	float frow, fcol;

	num &= 255;
	
	if( (num & 127) == 32 )
	{
		return;		// space
	}

	if( y <= -myfont->nMaxHeight )
	{
		return;			// totally off screen
	}

	scale = myfont->nSize;
	sh = myfont->nMaxHeight;

	row = (num >> 4) - 2;
	col = num & 15;

	frow = row * myfont->hFrac;
	fcol = col * myfont->wFrac;


	pfglColor4ubv( myfont->colour );

	pfglEnable( GL_BLEND );
	

	R_Bind( myfont->texfont->texnum );
	

	pfglBegin( GL_TRIANGLE_STRIP );

	
	pfglTexCoord2f( fcol, frow );							
	pfglVertex2i( x, y );
	
	pfglTexCoord2f( fcol, frow+myfont->hFrac );
	pfglVertex2i( x, (y+sh*scale) );
	
	pfglTexCoord2f( fcol+myfont->wFrac, frow );					
	pfglVertex2i( x+myfont->nMaxWidth*scale, y );
	
	pfglTexCoord2f( fcol+myfont->wFrac, frow+myfont->hFrac );	
	pfglVertex2i( x+myfont->nMaxWidth*scale, (y+sh*scale) );


	
	pfglEnd();

	pfglDisable( GL_BLEND );
	pfglColor3f( 1, 1, 1 );
}


/*
-----------------------------------------------------------------------------
 Function: R_Draw_StretchPic -Draw stretched image to the screen. 
 
 Parameters: x -[in] x-coordinate.
			 y -[in] y-coordinate. 
			 w -[in] width of region.
			 h -[in] height of region.
			 pic -[in] Image filename to stretch.
 
 Returns: Nothing.
 
 Notes: 
	
-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_StretchPic( int x, int y, int w, int h, const char *pic )
{
	texture_t *gl;

	gl = TM_FindTexture( pic, TT_Pic );
	if( ! gl )
	{
		Com_Printf( "Can't find pic: %s\n", pic );
		return;
	}

	R_Bind( gl->texnum );
	
	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 0.0f, 1.0f );	pfglVertex2i( x, y+h );
	pfglTexCoord2f( 1.0f, 0.0f );	pfglVertex2i( x+w, y );
	pfglTexCoord2f( 1.0f, 1.0f );	pfglVertex2i( x+w, y+h );
	
	pfglEnd();
}



/*
-----------------------------------------------------------------------------
 Function: R_Draw_Fill -Fills a box of pixels with a single color. 
 
 Parameters: x -[in] x-coordinate.
			 y -[in] y-coordinate. 
			 w -[in] width of region.
			 h -[in] height of region.
			 c -[in] Colour to fill region.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_Fill( int x, int y, int w, int h, colour3_t c )
{
#if 1
	// as of 2.2 OS, doing a clear with a small scissor rect is MUCH slower
	// than drawing geometry, so they must not be optimizing that case...
	colour4_t	c4;
	c4[0] = c[0];
	c4[1] = c[1];
	c4[2] = c[2];
	c4[3] = 255;
	R_Draw_Blend( x, y, w, h, c4 );
#else
	if ( deviceOrientation == ORIENTATION_LANDSCAPE_RIGHT ) {
		qglScissor( x, y, w, h );
	} else {
		qglScissor( x, 320-(y+h), w, h );
	}
	qglEnable( GL_SCISSOR_TEST );
	qglClearColor( c[0] / 255.0f,  c[1] / 255.0f,  c[2] / 255.0f, 1.0f );
	qglClear( GL_COLOR_BUFFER_BIT );
	qglDisable( GL_SCISSOR_TEST );
#endif
}

PUBLIC void R_Draw_Blend( int x, int y, int w, int h, colour4_t c )
{
	pfglDisable( GL_TEXTURE_2D );	
	pfglColor4ubv( c );

	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglVertex2i( x, y );
	pfglVertex2i( x, y+h );
	pfglVertex2i( x+w, y );
	pfglVertex2i( x+w, y+h );
	
	pfglEnd();
	
	pfglColor3f( 1, 1, 1 );
	pfglEnable( GL_TEXTURE_2D );
}

/*
-----------------------------------------------------------------------------
 Function: R_Draw_Line -Draw a line on the screen. 
 
 Parameters: nXStart -[in] x-coordinate of starting point. 
			 nYStart -[in] y-coordinate of starting point. 
			 nXEnd -[in] x-coordinate of ending point.
			 nYEnd -[in] y-coordinate of ending point.
			 c -[in] Colour value.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_Line( int nXStart, int nYStart, int nXEnd, int nYEnd, int width, colour3_t c )
{
	pfglDisable( GL_TEXTURE_2D );


	pfglColor3ubv( c );


	pfglLineWidth( (float)width );

	pfglBegin( GL_LINES );
		
	pfglVertex2i( nXStart, nYStart );
	pfglVertex2i( nXEnd, nYEnd );

	pfglEnd();

	pfglColor3f( 1, 1, 1 );
	pfglEnable( GL_TEXTURE_2D );
}


