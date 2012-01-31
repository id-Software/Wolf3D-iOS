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
 *	wolf_renderer.c:  Wolfenstein 3-D renderer.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"

LevelData_t *r_world;


/*
-----------------------------------------------------------------------------
 Function: R_BeginRegistration -Start the rendering registration sequence. 
 
 Parameters: map -[in] The name of the map to load.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void R_BeginRegistration( const char *map )
{
	char	fullname[ MAX_GAMEPATH ];

	if( ! map || ! *map )
	{
		return;
	}	

	++texture_registration_sequence;


	my_snprintf( fullname, sizeof( fullname ), "maps/%s.map", map );

//	Door_ResetDoors( &r_world->Doors );
	Powerup_Reset();
	Sprite_Reset();
	Areas_InitAreas( Player.areanumber );
	PushWall_Reset();

	memset( &levelstate, 0, sizeof( levelstate ) ); // Reset gamestate
	ResetGuards();

	r_world = Level_LoadMap( fullname );

	if( r_world == NULL )
	{
		Com_Printf( "Could not load map (%s)\n", map );
		return;
	}	

	levelstate.floornum = floornumber;

	if( g_version->value == SPEAROFDESTINY )
	{
		if( strlen( map ) >= 2 )
		{
			levelstate.floornum = atoi( map+1 );

			if( levelstate.floornum == 20 )
			{
				levelstate.floornum = 17;
			}
		}
	}
	
}
