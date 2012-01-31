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

#include "../wolfiphone.h"



#define BJRUNSPEED	2048
#define BJJUMPSPEED	680

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
void SpawnBJVictory( void )
{
	entity_t *bj;

	bj = SpawnActor( en_bj, POS2TILE(Player.position.origin[0]), POS2TILE(Player.position.origin[1]), dir4_north, r_world );
	if( ! bj )
	{
		return;
	}

	bj->x = Player.position.origin[ 0 ];
	bj->y = Player.position.origin[ 1 ];
	bj->state = st_path1;
	bj->speed = BJRUNSPEED;
	bj->flags = FL_NONMARK; // FL_NEVERMARK;
	bj->temp2 = 6;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
void T_BJRun( entity_t *Guard )
{
//	MoveObj(Guard, Guard->speed);
	
	if( ! Guard->distance )
	{
		Guard->distance = TILEGLOBAL;
		if ( !(--Guard->temp2) )
		{
			A_StateChange( Guard, st_shoot1 );
			Guard->speed = BJJUMPSPEED;
			return;
		}
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
void T_BJJump( entity_t *Guard )
{
//	MoveObj(Guard, Guard->speed);
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
void T_BJYell( entity_t *Guard )
{
	Sound_StartSound( NULL, 0, CHAN_VOICE, Sound_RegisterSound( "sfx/082.wav" ), 1, ATTN_NORM, 0 );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
void T_BJDone( entity_t *Guard )
{
	Player.playstate = ex_victory;				// exit castle tile
}
