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


float cur_x_fov, cur_y_fov; // x & y field of view (in degrees)
float ratio; // viewport width/height


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

	pfglClearColor( 1,0, 0.5 , 0.5 );
	pfglCullFace( GL_FRONT );
	pfglEnable( GL_TEXTURE_2D );

	pfglEnable( GL_ALPHA_TEST );
	pfglAlphaFunc( GL_GREATER, 0.666f );

	pfglDisable( GL_DEPTH_TEST );
	pfglDisable( GL_CULL_FACE );
	pfglDisable( GL_BLEND );

	pfglColor4f( 1, 1, 1, 1 );
#ifndef IPHONE
	pfglPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
#endif
	pfglShadeModel( GL_FLAT );


	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );



	GL_UpdateSwapInterval();

}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void R_CheckFOV( void )
{
/*
	if(!vid_fov->modified) return;
	vid_fov->modified=false;

	if( vid_fov->value<1 || vid_fov->value>179)
	{
		Com_Printf("Wrong FOV: %f\n", vid_fov->value);
		Cvar_SetValue(vid_fov->name, (cur_x_fov>=1 && cur_x_fov<=179)?cur_x_fov:DEFAULT_FOV);
	}
*/
	ratio = (float) viddef.width / (float)viddef.height; // FIXME: move somewhere
	cur_x_fov = 75;
	cur_y_fov = CalcFov( cur_x_fov, (float)viddef.width, (float)viddef.height );

}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_SetGL3D( placeonplane_t viewport )
{
	R_CheckFOV();

	pfglMatrixMode( GL_PROJECTION );
	pfglLoadIdentity();
#ifdef IPHONE
	pfglRotatef( 90, 0, 0, 1 );
#endif
	MYgluPerspective( cur_y_fov - 2.0f, ratio, 0.2f, 64.0f );	// tweak fov in to avoid edge tile clips
	pfglMatrixMode( GL_MODELVIEW );
	pfglLoadIdentity();

	pfglRotatef( (float)(90 - FINE2DEG( viewport.angle )), 0, 1, 0 );
	pfglTranslatef( -viewport.origin[ 0 ] / FLOATTILE, 0, viewport.origin[ 1 ] / FLOATTILE );

	pfglCullFace( GL_BACK );

	pfglEnable( GL_DEPTH_TEST );
	pfglEnable( GL_CULL_FACE );
	pfglEnable( GL_BLEND );	
	pfglDisable( GL_BLEND );		// !@# draw all the walls opaque without alpha test
	pfglDisable( GL_ALPHA_TEST );
	qglDepthMask( GL_TRUE );
	
	// clear depth buffer
	pfglClear( GL_DEPTH_BUFFER_BIT );
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

	pfglBegin( GL_QUADS );

	pfglVertex2i( x, y );
	pfglVertex2i( x, y + h);
	pfglVertex2i( x + w, y + h );
	pfglVertex2i( x + w, y );
	
	pfglEnd();

	pfglColor3f( 1, 1, 1 );
//	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	pfglDisable( GL_BLEND );
	pfglEnable( GL_TEXTURE_2D );
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
	texture_t *twall;

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

	assert( tex >= 0 && tex < 1000 );
	twall = wallTextures[tex];
	if ( !twall ) {
		char	name[1024];
		my_snprintf( name, sizeof( name ), "walls/%.3d.tga", tex );
		twall = wallTextures[tex] = TM_FindTexture( name, TT_Wall );
	}
    R_Bind( twall->texnum );


	pfglBegin( GL_QUADS );

	pfglTexCoord2f( 1.0, 0.0 ); pfglVertex3f( x1, z2, y1 );
	pfglTexCoord2f( 0.0, 0.0 ); pfglVertex3f( x2, z2, y2 );
	pfglTexCoord2f( 0.0, 1.0 ); pfglVertex3f( x2, z1, y2 );
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
	texture_t *twall;

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

	assert( tex >= 0 && tex < 1000 );
	twall = wallTextures[tex];
	if ( !twall ) {
		char name[1024];
		my_snprintf( name, sizeof( name ), "walls/%.3d.tga", tex );
		twall = wallTextures[tex] = TM_FindTexture( name, TT_Wall );
	}
	
    R_Bind( twall->texnum );


	pfglBegin( GL_QUADS );

	pfglTexCoord2f( backside ? 0.0f : 1.0f, 0.0 ); pfglVertex3f( x1, z2, y1 );
	pfglTexCoord2f( backside ? 1.0f : 0.0f, 0.0 ); pfglVertex3f( x2, z2, y2 );
	pfglTexCoord2f( backside ? 1.0f : 0.0f, 1.0 ); pfglVertex3f( x2, z1, y2 );
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

	//pfglEnable( GL_ALPHA_TEST );
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
		
		pfglBegin( GL_QUADS );

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
				pfglTexCoord2f( x2, y2 );	pfglVertex3f( Ex - x2 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y2), -Ey + x2 * 2*sina );
				pfglTexCoord2f( x2, y1 );	pfglVertex3f( Ex - x2 * 2*cosa, -(LOWERZCOORD + (UPPERZCOORD - LOWERZCOORD) * y1), -Ey + x2 * 2*sina );
			}
		} else {
			Ex = Dx = vislist[ n ].x / FLOATTILE;
			Ey = Dy = vislist[ n ].y / FLOATTILE;
			Ex += cosa; Ey += sina;
			Dx -= cosa; Dy -= sina;

			pfglTexCoord2f( 0.0, 0.0 );	pfglVertex3f( Ex, UPPERZCOORD, -Ey );
			pfglTexCoord2f( 0.0, 1.0 );	pfglVertex3f( Ex, LOWERZCOORD, -Ey );
			pfglTexCoord2f( 1.0, 1.0 );	pfglVertex3f( Dx, LOWERZCOORD, -Dy );
			pfglTexCoord2f( 1.0, 0.0 );	pfglVertex3f( Dx, UPPERZCOORD, -Dy );
		}
		
		pfglEnd();
	}

	//pfglDisable( GL_ALPHA_TEST );		// !@# reanable just for sprites
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_DrawWeapon( void )
{
	char name[ 32 ];
	texture_t *tex;	
	static int w = 128;
	static int h = 128;
	static int scale = 2;
	int x = (viddef.width - (128 * scale)) >> 1;
	int y = viddef.height - (128 * scale) - 79;

	my_snprintf( name, sizeof( name ), "%s/%d.tga", spritelocation, Player.weapon * 5 + Player.weaponframe + SPR_KNIFEREADY );

	tex = TM_FindTexture( name, TT_Pic );


	R_Bind( tex->texnum );

	

	pfglAlphaFunc( GL_GREATER, 0.3f );
	
	pfglEnable( GL_BLEND );
		
	pfglBegin( GL_QUADS );

	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 1.0f, 0.0f );	pfglVertex2i( x + w * scale, y );
	pfglTexCoord2f( 1.0f, 1.0f );	pfglVertex2i( x + w * scale, y + h * scale );
	pfglTexCoord2f( 0.0f, 1.0f );	pfglVertex2i( x, y + h * scale );
	
	pfglEnd();
	
	pfglDisable( GL_BLEND );

	pfglAlphaFunc( GL_GREATER, 0.666f );
	

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
		pfglTexCoord2f( fcol+w, 0 );	pfglVertex2i( x+18, y );
		pfglTexCoord2f( fcol+w, 1 );	pfglVertex2i( x+18, y+32 );
		pfglTexCoord2f( fcol,	1 );	pfglVertex2i( x, y+32 );

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
		pfglTexCoord2f( fcol+w, frow );		pfglVertex2i( mx+32, y );
		pfglTexCoord2f( fcol+w, frow+h );	pfglVertex2i( mx+32, y+32 );
		pfglTexCoord2f( fcol, frow+h );		pfglVertex2i( mx, y+32 );
	
	
			
		mx += wfont[ (num & 127) - 32 ];
		++string;
	}

	pfglEnd();
}

