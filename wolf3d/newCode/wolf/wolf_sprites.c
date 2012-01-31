/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 2000-2002 by DarkOne the Hacker

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
 *	wolf_sprites.c:  Wolfenstein3-D sprite handling.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	This code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker.
 *
 */

#include "../wolfiphone.h"


// nobody should see this array!
sprite_t Spr_Sprites[ MAX_SPRITES ];
W32 n_of_sprt;



/*
-----------------------------------------------------------------------------
 Function: Sprite_Reset -Reset sprite status.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: Called only when client must reconnect will not set remove flag!
-----------------------------------------------------------------------------
*/
PUBLIC void Sprite_Reset( void )
{
	n_of_sprt = 0;
	memset( Spr_Sprites, 0, sizeof( Spr_Sprites ) );
}

/*
-----------------------------------------------------------------------------
 Function: Sprite_RemoveSprite -Remove sprite.
 
 Parameters: sprite_id -[in] sprite id to remove.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sprite_RemoveSprite( int sprite_id )
{
	if( sprite_id == -1 )
	{
		return;
	}

	Spr_Sprites[ sprite_id ].flags |= SPRT_REMOVE;
}


/*
-----------------------------------------------------------------------------
 Function: Sprite_GetNewSprite -Get sprite index.
 
 Parameters: Nothing.
 
 Returns: "sprite id" index.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int Sprite_GetNewSprite( void )
{
	W32 n;
	sprite_t* sprt;

	for( n = 0, sprt = Spr_Sprites ; n < n_of_sprt ; ++n, ++sprt )
	{
		if( sprt->flags & SPRT_REMOVE )
		{ // free spot: clear it first
			memset( sprt, 0, sizeof( sprite_t ) );
			return n;
		}
	}

	if( n_of_sprt >= MAX_SPRITES )
	{
		Com_Printf( "Warning n_of_sprt == MAX_SPRITES\n" );
		return -1;
	}

	return n_of_sprt++;
}



/*
-----------------------------------------------------------------------------
 Function: Sprite_SetPos -Set sprite position.
 
 Parameters: sprite_id -[in] sprite id to change.
			 x, y -[in] new x, y.
			 angle -[in] new angle to set.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sprite_SetPos( int sprite_id, int x, int y, int angle )
{
	if( sprite_id == -1 ) 
	{
		return;
	}

	Spr_Sprites[ sprite_id ].x = x;
	Spr_Sprites[ sprite_id ].y = y;
	Spr_Sprites[ sprite_id ].ang = angle;
	Spr_Sprites[ sprite_id ].tilex = POS2TILE( x );
	Spr_Sprites[ sprite_id ].tiley = POS2TILE( y );
	Spr_Sprites[ sprite_id ].flags |= SPRT_CHG_POS;

	if( ! (x & HALFTILE) ) // (x%TILEGLOBAL>=HALFTILE)
	{
		Spr_Sprites[ sprite_id ].tilex--;
	}

	if( ! (y & HALFTILE) )
	{
		Spr_Sprites[ sprite_id ].tiley--;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Sprite_SetTex -Set sprite texture.
 
 Parameters: sprite_id -[in] sprite id to change.
			 index -[in] texture index.
			 tex -[in] texture to set as.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sprite_SetTex( int sprite_id, int index, int tex )
{
	if( sprite_id == -1 )
	{
		return;
	}
	
	if( index == -1 )	// one texture for each phase
	{
		Spr_Sprites[ sprite_id ].tex[ 0 ] = tex;
		Spr_Sprites[ sprite_id ].flags |= SPRT_ONE_TEX;
	}
	else
	{
		Spr_Sprites[ sprite_id ].tex[ index ] = tex;
	}
	
	Spr_Sprites[ sprite_id ].flags |= SPRT_CHG_TEX;
}


#define MAXVISABLE 128
visobj_t vislist[ MAXVISABLE ];


/*
-----------------------------------------------------------------------------
 Function: Sprite_CreateVisList -Compare function for vislist sorting.
 
 Parameters: vis1, vis2 -[in] Two values to compare.
 
 Returns: 
		<0	elem1 further than elem2
		 0	elem1 equal distance to elem2
		>0	elem1 closer than elem2
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE int Sprite_cmpVis( const void *elem1, const void *elem2 )
{
	// macro to get distance from a void pointer to visobj_t
	#define vis_dist( vis )	( ((visobj_t *)vis)->dist )

	if( vis_dist( elem1 ) == vis_dist( elem2 ) )
	{
		return 0; // process equal distance
	}
	else
	{
		// if dist > sprite must be first
		return vis_dist( elem1 ) < vis_dist( elem2 ) ? 1 : -1;
	}
}


/*
-----------------------------------------------------------------------------
 Function: Sprite_CreateVisList -Build and sort visibility list of sprites.
 
 Parameters: Nothing.
 
 Returns: Number of visible sprites.
 
 Notes: 
	List is sorted from far to near.
	List is based on tile visibility array, made by raycaster.
	Called only by client.
-----------------------------------------------------------------------------
*/
PUBLIC int Sprite_CreateVisList( void )
{
	W32 tx, ty, n, num_visible;
	visobj_t *visptr;
	sprite_t* sprt;

	visptr = vislist;
	num_visible = 0;

	for( n = 0, sprt = Spr_Sprites; n < n_of_sprt; ++n, ++sprt )
	{
		if( sprt->flags & SPRT_REMOVE )
		{
			continue;
		}

		tx = sprt->tilex;
		ty = sprt->tiley;
	
		if( tx > 63 )
			tx = 63;
		if( ty > 63 )
			ty = 63;
		
	// can be in any of 4 surrounding tiles; not 9 - see definition of tilex & tiley
		if( tile_visible[ tx ][ ty ] || tile_visible[ tx + 1 ][ ty ] ||
			 tile_visible[ tx ][ ty + 1 ] || tile_visible[ tx + 1 ][ ty + 1 ] )
		{ // player spoted it
			visptr->dist = LineLen2Point( sprt->x - Player.position.origin[ 0 ],
																 sprt->y-Player.position.origin[ 1 ],
																 Player.position.angle ); //FIXME viewport
			visptr->x = sprt->x;
			visptr->y = sprt->y;
			visptr->ang = sprt->ang;
			visptr->tex = sprt->tex[ 0 ]; //FIXME!
			if( ++num_visible > MAXVISABLE ) 
			{
				break; // vislist full
			}

			visptr++;
		}
	}

// sorting list
	if( num_visible ) // do not sort if no entries
	{
		qsort( vislist, num_visible, sizeof( visobj_t ), Sprite_cmpVis );
	}

	return num_visible;
}

