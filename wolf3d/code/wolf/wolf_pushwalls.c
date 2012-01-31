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
 *	wolf_pushwalls.c:  Wolfenstein3-D push-wall handler.
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

Pwall_t PWall;


/*
-----------------------------------------------------------------------------
 Function: PushWall_Reset() -Reset pushwall status. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PushWall_Reset(void)
{
	memset( &PWall, 0, sizeof( Pwall_t ) );
}


/*
-----------------------------------------------------------------------------
 Function: PushWall_Push() -Try to move push-wall. 
 
 Parameters: x, y -[in] Coordinates in tilemap.
			 dir -[in] Direction in which push-wall is intended to move.
 
 Returns: true if push successful, otherwise false.
 
 Notes: Called whenever someone tries to push a secret wall.

-----------------------------------------------------------------------------
*/
PUBLIC _boolean PushWall_Push( int x, int y, dir4type dir )
{
	int dx, dy;


	if( PWall.active ) 
	{
		return false; // another PWall is moving [only one at a time!]
	}

	dx = dx4dir[ dir ];
	dy = dy4dir[ dir ];

	if( r_world->tilemap[ x + dx ][ y + dy ] & (SOLID_TILE | DOOR_TILE) )
	{ // noway (smth is blocking)
		return true;
	}
	
// remove secret flag & make everything needed when pushwall used!
	r_world->tilemap[ x ][ y ] &= (~SECRET_TILE);
	r_world->tilemap[ x ][ y ] &= (~WALL_TILE);
	r_world->tilemap[ x ][ y ] |= PUSHWALL_TILE;
	
	if ( ++levelstate.found_secrets == levelstate.total_secrets ) {
		iphoneSetNotifyText( "You found the last secret!" );
	} else {
		iphoneSetNotifyText( "You found a secret!" );
	}

	if( g_version->value == SPEAROFDESTINY && currentMap.episode >= 6 && currentMap.episode < 9)//added the episode check... gsh ).. TODO: fix sfx and other media
	{
		Sound_StartSound( NULL, 1, CHAN_AUTO, Sound_RegisterSound( "sfx/030.wav" ), 1, ATTN_STATIC, 0 );
	}
	else
	{
		Sound_StartSound( NULL, 1, CHAN_AUTO, Sound_RegisterSound( "sfx/034.wav" ), 1, ATTN_STATIC, 0 );
	}

// good way to avoid stuckness; [un]comment one more down!
// it makes a tile behind pushwall unpassable
	r_world->tilemap[ x + dx ][ y + dy ] |= PUSHWALL_TILE;
	r_world->wall_tex_x[ x + dx ][ y + dy ] = r_world->wall_tex_x[ x ][ y ];
	r_world->wall_tex_y[ x + dx ][ y + dy ] = r_world->wall_tex_y[ x ][ y ];

// write down PWall info
	PWall.active = true;
	PWall.PWtilesmoved = PWall.PWpointsmoved = 0;
	PWall.dir = dir;
	PWall.x = x; PWall.y = y;
	PWall.dx = dx; PWall.dy = dy;
	PWall.tex_x = r_world->wall_tex_x[ x ][ y ];
	PWall.tex_y = r_world->wall_tex_y[ x ][ y ];

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: PushWall_Process() -Process push-walls. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PushWall_Process( void )
{
	if( ! PWall.active )
	{
		return; // no active PWall to work with
	}

	PWall.PWpointsmoved += tics;
	
	if( PWall.PWpointsmoved < 128 )
	{
		return;
	}

	PWall.PWpointsmoved -= 128;
	PWall.PWtilesmoved++;
// Free tile
	r_world->tilemap[ PWall.x ][ PWall.y ] &= (~PUSHWALL_TILE);
// Occupy new tile
	PWall.x += PWall.dx;
	PWall.y += PWall.dy;

// Shall we move further?
	if( r_world->tilemap[ PWall.x + PWall.dx ][ PWall.y + PWall.dy ] & (SOLID_TILE | DOOR_TILE | ACTOR_TILE | POWERUP_TILE) ||
	   PWall.PWtilesmoved == 3 )
	{
		r_world->tilemap[ PWall.x ][ PWall.y ] &= (~PUSHWALL_TILE); // wall now
		r_world->tilemap[ PWall.x ][ PWall.y ] |= WALL_TILE; // wall now
		r_world->wall_tex_x[ PWall.x ][ PWall.y ] = PWall.tex_x;
		r_world->wall_tex_y[ PWall.x ][ PWall.y ] = PWall.tex_y;
		PWall.active = false; // Free Push Wall
	}
	else
	{
		r_world->tilemap[ PWall.x + PWall.dx ][ PWall.y + PWall.dy ] |= PUSHWALL_TILE;
	}

}
