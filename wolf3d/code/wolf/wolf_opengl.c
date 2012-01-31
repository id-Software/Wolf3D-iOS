/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 2000-2002 by DarkOne the Hacker
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
 *	wolf_opengl.c:  Wolfenstein3-D OpenGL renderer.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker.
 *
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */
 
#include "../wolfiphone.h"

// width and height in 2D
#define WIDTH_2D		640
#define HEIGHT_2D		480




/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void GL_SetDefaultState( void )
{
	pfglViewport( 0,0, viddef.height, viddef.width );
	pfglClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	pfglEnable( GL_TEXTURE_2D );
	pfglDisable( GL_DEPTH_TEST );
	pfglDisable( GL_ALPHA_TEST );
	pfglEnable( GL_CULL_FACE );
	pfglDisable( GL_BLEND );
	pfglColor4f( 1, 1, 1, 1 );
	pfglShadeModel( GL_FLAT );
	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}



/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_DrawBox( int x, int y, int w, int h, W32 color )
{
	pfglDisable( GL_TEXTURE_2D );

//	pfglEnable( GL_BLEND );
//	pfglBlendFunc( GL_SRC_COLOR, GL_DST_COLOR );

	pfglColor4ubv( (GLubyte *) & color );

	pfglBegin( GL_TRIANGLE_STRIP );

	pfglVertex2i( x, y );
	pfglVertex2i( x, y + h);
	pfglVertex2i( x + w, y );
	pfglVertex2i( x + w, y + h );
	
	pfglEnd();

	pfglColor3f( 1, 1, 1 );
//	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	pfglDisable( GL_BLEND );
	pfglEnable( GL_TEXTURE_2D );
}

/*
-----------------------------------------------------------------------------
 Function: R_DrawBoxFloat
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_DrawBoxFloat( float x, float y, float w, float h, W32 color )
{
	pfglDisable( GL_TEXTURE_2D );

//	pfglEnable( GL_BLEND );
//	pfglBlendFunc( GL_SRC_COLOR, GL_DST_COLOR );

	pfglColor4ubv( (GLubyte *) & color );

	pfglBegin( GL_TRIANGLE_STRIP );

	pfglVertex2f( x, y );
	pfglVertex2f( x, y + h);
	pfglVertex2f( x + w, y + h );
	pfglVertex2f( x + w, y );
	
	pfglEnd();

	pfglColor3f( 1, 1, 1 );
//	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	pfglDisable( GL_BLEND );
	pfglEnable( GL_TEXTURE_2D );
}

/*
 ====================
 LoadWallTexture
 
 Returns with the texture bound and glColor set to the right intensity.
 Loads an image from the filesystem if necessary.
 Used both during gameplay and for preloading during level parse.
 
 Wolfenstein was very wasteful with texture usage, making almost half of
 the textures just dim versions to provide "lighting" on the different
 wall sides.  With only a few exceptions for things like the elevator tiles
 and outdoor tiles that could only be used in particular orientations
 ====================
*/ 
float	wallBrightness[1000];
void LoadWallTexture( int wallPicNum ) {
	assert( wallPicNum >= 0 && wallPicNum < 1000 );
	texture_t *twall = wallTextures[wallPicNum];
	if ( !twall ) {
		if ( ( wallPicNum & 1 ) &&  
			wallPicNum != 31 &&
			wallPicNum != 41 &&
			wallPicNum != 43 &&
			wallPicNum != 133 ) {
			// this wallPicNum is just a dim version of another image
			
			// load the brighter version
			LoadWallTexture( wallPicNum - 1 );
			// use the same texture
			twall = wallTextures[wallPicNum] = wallTextures[wallPicNum - 1];
			// at a dimmer intensity
			wallBrightness[wallPicNum] = 0.7f;
		} else {
			// this wallPicNum has a real image associated with it
			char	name[1024];
			my_snprintf( name, sizeof( name ), "walls/%.3d.tga", wallPicNum );
			twall = wallTextures[wallPicNum] = TM_FindTexture( name, TT_Wall );
			wallBrightness[wallPicNum] = 1.0f;
		}
	}

    R_Bind( twall->texnum );
	
	// almost half of the walls are just slightly dimmer versions of
	// the "bright side", and are not stored as separate textures
	float f = wallBrightness[wallPicNum];
	pfglColor3f( f, f, f );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

           north (y)
          __________
          |        |
 west (x) |        | east (x)
          |________|
           south (y)

-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_Wall( float x, float y, float z1, float z2, int type, int tex )
{
	float x1, x2, y1, y2;

	switch( type )
	{
	// X wall
		case dir4_east:
			x1 = x2 = x + 1;
			y1 = -1 - y;
			y2 = -y;
			break;

		case dir4_west:
			x1 = x2 = x;
			y1 = -y;
			y2 = -1 - y;
			break;

	// Y wall
		case dir4_north:
			y1 = y2 = -y - 1;
			x1 = x;
			x2 = x + 1;
			break;

		case dir4_south:
			y1 = y2 = -y;
			x1 = x + 1;
			x2 = x;
			break;
	}

	LoadWallTexture( tex );
	
	pfglBegin( GL_TRIANGLE_STRIP );

	pfglTexCoord2f( 0.0, 0.0 ); pfglVertex3f( x2, z2, y2 );
	pfglTexCoord2f( 0.0, 1.0 ); pfglVertex3f( x2, z1, y2 );
	pfglTexCoord2f( 1.0, 0.0 ); pfglVertex3f( x1, z2, y1 );
	pfglTexCoord2f( 1.0, 1.0 ); pfglVertex3f( x1, z1, y1 );
	
	pfglEnd();
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_Draw_Door( int x, int y, float z1, float z2, _boolean vertical, _boolean backside, int tex, int amount )
{
	float x1, x2, y1, y2, amt;

	if( amount == DOOR_FULLOPEN )
	{
		return;
	}	
	
	amt = (float)amount / DOOR_FULLOPEN;


	if( vertical )
	{
		x1 = x2 = (float)x + 0.5f;
		y1 = -((float)y - amt);
		y2 = -((float)y - amt); // -1
		if( backside )
		{
			y1 -= 1;
		}
		else
		{
			y2 -= 1;
		}
	}
	else
	{
		y1 = y2 = -(float)y - 0.5f;
		x1 = (float)x + amt; // +1
		x2 = (float)x + amt;
		if( backside )
		{
			x2 += 1;
		}
		else
		{
			x1 += 1;
		}
	}

	LoadWallTexture( tex );

	pfglBegin( GL_TRIANGLE_STRIP );

	pfglTexCoord2f( backside ? 1.0f : 0.0f, 0.0 ); pfglVertex3f( x2, z2, y2 );
	pfglTexCoord2f( backside ? 1.0f : 0.0f, 1.0 ); pfglVertex3f( x2, z1, y2 );
	pfglTexCoord2f( backside ? 0.0f : 1.0f, 0.0 ); pfglVertex3f( x1, z2, y1 );
	pfglTexCoord2f( backside ? 0.0f : 1.0f, 1.0 ); pfglVertex3f( x1, z1, y1 );
	
	pfglEnd();
}


/*
-----------------------------------------------------------------------------
 Function: R_DrawSprites -Draws all visible sprites.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_DrawSprites( void )
{
	float sina, cosa;
	float Ex, Ey, Dx, Dy;
	int n_sprt, n, ang;
	texture_t *twall;
	extern cvar_t *cropSprites;
	
// build visible sprites list
	n_sprt = Sprite_CreateVisList();
	if( ! n_sprt )
	{
		return; // nothing to draw
	}

// prepare values for billboarding
	ang = NormalizeAngle( Player.position.angle + ANG_90 );
	sina = (float)(0.5 * SinTable[ ang ]);
	cosa = (float)(0.5 * CosTable[ ang ]);

	pfglEnable( GL_BLEND );
	qglDepthMask( GL_FALSE );
	for( n = 0; n < n_sprt; ++n )
	{
		int	texnum = vislist[ n ].tex;
		if( vislist[ n ].dist < MINDIST / 2 ) 
		{
			continue; // little hack to save speed & z-buffer
		}


	
		assert( texnum >= 0 && texnum < 1000 );
		twall = spriteTextures[texnum];
		if ( !twall ) {	
			char	name[1024];
			my_snprintf( name, sizeof( name ), "%s/%.3d.tga", spritelocation, (vislist[ n ].tex) );
			twall = spriteTextures[texnum] = TM_FindTexture( name, TT_Wall );
		}
		R_Bind( twall->texnum );
		
		pfglBegin( GL_TRIANGLE_STRIP );

		if ( cropSprites->value && twall->header.numBounds > 0 ) {
			// draw one or two subrects to avoid blending all the empty space
			int	b;
			
			for ( b = 0 ; b < twall->header.numBounds ; b++ ) {
				// include a bit extra for filtering
				float x1 = (float)(twall->header.bounds[b][0][0]-1) / (twall->header.uploadWidth-1);
				float y1 = (float)(twall->header.bounds[b][0][1]-1) / (twall->header.uploadHeight-1);
				float x2 = (float)(twall->header.bounds[b][1][0]+1) / (twall->header.uploadWidth-1);
				float y2 = (float)(twall->header.bounds[b][1][1]+1) / (twall->header.uploadHeight-1);
				if ( x1 < 0 ) {
					x1 = 0;
				} else if ( x2 > 1.0 ) {
					x2 = 1.0;
				}
				if ( y1 < 0 ) {
					y1 = 0;
				} else if ( y2 > 1.0 ) {
					y2 = 1.0;
				}
				Ex = vislist[ n ].x / FLOATTILE + cosa;
				Ey = vislist[ n ].y / FLOATTILE + sina;

				pfglTexCoord2f( x1, y1 );	pfglVertex3f( Ex - x1 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y1), -Ey + x1 * 2*sina );
				pfglTexCoord2f( x1, y2 );	pfglVertex3f( Ex - x1 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y2), -Ey + x1 * 2*sina );				
				pfglTexCoord2f( x2, y1 );	pfglVertex3f( Ex - x2 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y1), -Ey + x2 * 2*sina );
				pfglTexCoord2f( x2, y2 );	pfglVertex3f( Ex - x2 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y2), -Ey + x2 * 2*sina );
			}
		} else {
			Ex = Dx = vislist[ n ].x / FLOATTILE;
			Ey = Dy = vislist[ n ].y / FLOATTILE;
			Ex += cosa; Ey += sina;
			Dx -= cosa; Dy -= sina;

			pfglTexCoord2f( 0.0, 0.0 );	pfglVertex3f( Ex, UPPERZCOORD, -Ey );
			pfglTexCoord2f( 0.0, 1.0 );	pfglVertex3f( Ex, LOWERZCOORD, -Ey );
			pfglTexCoord2f( 1.0, 0.0 );	pfglVertex3f( Dx, UPPERZCOORD, -Dy );
			pfglTexCoord2f( 1.0, 1.0 );	pfglVertex3f( Dx, LOWERZCOORD, -Dy );
		}
		
		pfglEnd();
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
PUBLIC void R_DrawNumber( int x, int y, int number )
{
	texture_t *tex;
	int	col; 
	float fcol;
	static float w = 0.1f;	
	int i;
	char string[ 20 ];
	W32 length;


	my_snprintf( string, sizeof( string ), "%d", number );
	length = strlen( string );

	tex = TM_FindTexture( "pics/N_NUMPIC.tga", TT_Pic );


	pfglEnable( GL_TEXTURE_2D );
	
	R_Bind( tex->texnum );

	pfglBegin( GL_QUADS );

	for( i = length-1 ; i >= 0 ; --i )
	{
		col = string[ i ] - 48;

		fcol = col * w;		
		
		pfglTexCoord2f( fcol,	0 );	pfglVertex2i( x, y );
		pfglTexCoord2f( fcol,	1 );	pfglVertex2i( x, y+32 );
		pfglTexCoord2f( fcol+w, 1 );	pfglVertex2i( x+18, y+32 );
		pfglTexCoord2f( fcol+w, 0 );	pfglVertex2i( x+18, y );

		x -= 18;
	}

	pfglEnd();

}


W8 wfont[ ] = {	
			    	32, 15, 32, 32, 32, 32, 32, 12, 32, 32, 32, 32, 32, 32, 32, 32,
			    	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 16, 32, 32, 32, 32, 32,
			    	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
			    	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_put_line( int x, int y, const char *string )
{
	texture_t *tex;
	int mx = x;
	int num;
	float frow, fcol;
	static float	h = 0.25f;	// (32 / 128.0f);
	static float	w = 0.0625f; // (32 / 512.0f);


	tex = TM_FindTexture( "pics/L_FONTPIC.tga", TT_Pic );
	

	R_Bind( tex->texnum );
	
	pfglBegin( GL_QUADS );

	while( *string )
	{
		if( *string == '\n' )
		{
			mx = x;
			y += 32;
			++string;
			continue;
		}

		num = *string;

		num &= 255;
	
		if( (num & 127) == 32 )
		{			
			mx += 32;
			++string;
			continue;		// space	
		}


		frow = ((num >> 4) - 2) * h;
		fcol = (num & 15) * w;

	
		pfglTexCoord2f( fcol, frow );		pfglVertex2i( mx, y );
		pfglTexCoord2f( fcol, frow+h );		pfglVertex2i( mx, y+32 );
		pfglTexCoord2f( fcol+w, frow+h );	pfglVertex2i( mx+32, y+32 );
		pfglTexCoord2f( fcol+w, frow );		pfglVertex2i( mx+32, y );
		
	
	
			
		mx += wfont[ (num & 127) - 32 ];
		++string;
	}

	pfglEnd();
}

