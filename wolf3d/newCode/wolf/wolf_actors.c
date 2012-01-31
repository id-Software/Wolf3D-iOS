/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 2001 by DarkOne the Hacker

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
 *	wolf_actors.c: Wolfenstein3-D actor manager.
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

#include "wolf_act_stat.h"

entity_t Guards[ MAX_GUARDS + 1 ], *New;
W16 NumGuards = 0;
W8 add8dir[ 9 ]	= { 4, 5, 6, 7, 0, 1, 2, 3, 0 }; 
W8 r_add8dir[ 9 ]= { 4, 7, 6, 5, 0, 1, 2, 3, 0 };



/*
-----------------------------------------------------------------------------
 Function: A_StateChange -Changes guard's state to that defined in NewState.

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void A_StateChange( entity_t *ent, en_state NewState )
{
	ent->state = NewState;
	assert( ent->type >= 0 && ent->type < NUMENEMIES );
	if ( NewState == st_remove ) {
		ent->ticcount = 0;
	} else {
		assert( ent->state >= 0 && ent->state < NUMSTATES );
		ent->ticcount = objstate[ ent->type ][ ent->state ].timeout; //0;
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
PRIVATE int DoGuard( entity_t *ent ) // FIXME: revise!
{ // returns 0 if we must remove this Guard from Guards list, otherwise 1;
	think_t think;

	assert( ent->tilex >= 0 && ent->tilex < 64 );
	assert( ent->tiley >= 0 && ent->tiley < 64 );
	assert( ent->dir >= 0 && ent->dir <= 8 );
	
	// ticcounts fire discrete actions separate from think functions
	if ( ent->ticcount ) {
		ent->ticcount -= tics;
		while( ent->ticcount <= 0 )
		{
			assert( ent->type >= 0 && ent->type < NUMENEMIES );
			assert( ent->state >= 0 && ent->state < NUMSTATES );
			think = objstate[ ent->type ][ ent->state ].action; // end of state action
			if( think )
			{
				think( ent );
				if( ent->state == st_remove )
				{
					return 0;
				}
			}

			ent->state = objstate[ ent->type ][ ent->state ].next_state;
			if( ent->state == st_remove )
			{
				return 0;
			}

			if( ! objstate[ ent->type ][ ent->state ].timeout )
			{
				ent->ticcount = 0;
				break;
			}

			ent->ticcount += objstate[ ent->type ][ ent->state ].timeout;
		}
	}
	
//
// think
//
	assert( ent->type >= 0 && ent->type < NUMENEMIES );
	assert( ent->state >= 0 && ent->state < NUMSTATES );
	think = objstate[ ent->type ][ ent->state ].think;
	if( think )
	{
		think( ent );
		if( ent->state == st_remove )
		{
			return 0;
		}
	}

	return 1;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RemoveActor( entity_t *actor )
{
	Sprite_RemoveSprite( actor->sprite );
	memmove( actor, actor+1, (int)(&Guards[ NumGuards ]) - (int)(actor+1) );
	NumGuards--;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void ProcessGuards( void )
{
	int n, tex;
	assert( NumGuards < MAX_GUARDS );
	for( n = 0 ; n < NumGuards ; ++n )
	{
		if( ! DoGuard( &Guards[ n ] ) )
		{ // remove guard from the game forever!
			RemoveActor( &Guards[ n-- ] );
			continue;
		}

		Sprite_SetPos( Guards[ n ].sprite, Guards[ n ].x, Guards[ n ].y, Guards[ n ].angle );
		tex = objstate[ Guards[ n ].type ][ Guards[ n ].state ].texture;
		
		if( objstate[ Guards[ n ].type ][ Guards[ n ].state ].rotate )
		{
			if( Guards[ n ].type == en_rocket || Guards[ n ].type == en_hrocket )
			{
				tex += r_add8dir[ Get8dir( angle_wise( FINE2RAD(Player.position.angle), FINE2RAD(Guards[ n ].angle) ) ) ];				
			}
			else
			{
				tex += add8dir[ Get8dir( angle_wise( FINE2RAD(Player.position.angle), FINE2RAD(Guards[ n ].angle) ) ) ];				
			}
		}
		
		Sprite_SetTex( Guards[ n ].sprite, 0, tex );
	}
}


/*
-----------------------------------------------------------------------------
 Function: ResetGuards -Reset actors status

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void ResetGuards( void )
{
	memset( Guards, 0, sizeof( Guards ) );
	NumGuards = 0;
	New = NULL;
}



/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC entity_t *GetNewActor( void )
{
	if( NumGuards > MAX_GUARDS ) 
	{
		return NULL;
	}
	
	memset( &Guards[ NumGuards ], 0, sizeof( Guards[ 0 ] ) );
	
	return &Guards[ NumGuards++ ];
}



/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC entity_t *SpawnActor( enemy_t which, int x, int y, dir4type dir, LevelData_t *lvl )
{
	entity_t *new_actor;

	new_actor = GetNewActor();
	if( ! new_actor )
	{
		return NULL;
	}

	new_actor->x = TILE2POS( x );
	new_actor->y = TILE2POS( y );

	new_actor->tilex = x;
	new_actor->tiley = y;

	assert( dir >= 0 && dir <= 4 );
	new_actor->angle = dir4angle[ dir ];
	new_actor->dir = dir4to8[ dir ];

	new_actor->areanumber = lvl->areas[ x ][ y ];
//	Com_Printf( "Actor at %i,%i had areaNum: %i\n", x, y, new_actor->areanumber );
	if ( new_actor->areanumber < 0 ) {
		// ambush marker tiles are listed as -3 area
		new_actor->areanumber = 0;
	}
	
	assert( new_actor->areanumber >= 0 && new_actor->areanumber < NUMAREAS );
	new_actor->type = which;

	new_actor->health = starthitpoints[ (int)skill->value ][ which ];
	new_actor->sprite = Sprite_GetNewSprite();

	return new_actor;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void SpawnStand( enemy_t which, int x, int y, int dir, LevelData_t *lvl )
{
	entity_t *self;

	self = SpawnActor( which, x, y, dir, r_world );
	if( ! self )
	{
		return;
	}

	self->state = st_stand;
	self->speed = SPDPATROL;
	self->ticcount = objstate[ which ][ st_stand ].timeout ? US_RndT() % objstate[ which ][ st_stand ].timeout + 1 : 0;
	self->flags |= FL_SHOOTABLE;
	if( lvl->tilemap[ x ][ y ] & AMBUSH_TILE )
	{
		self->flags |= FL_AMBUSH;
	}

	levelstate.total_monsters++;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void SpawnPatrol( enemy_t which, int x, int y, int dir )
{
	entity_t *self;

	self = SpawnActor( which, x, y, dir, r_world );
	if( ! self )
	{
		return;
	}
	
	self->state = st_path1;
	self->speed = (which == en_dog) ? SPDDOG : SPDPATROL;
	self->distance = TILEGLOBAL;
	self->ticcount = objstate[ which ][ st_path1 ].timeout ? US_RndT() % objstate[ which ][ st_path1 ].timeout + 1 : 0;
	self->flags |= FL_SHOOTABLE;

	levelstate.total_monsters++;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void SpawnDeadGuard( enemy_t which, int x, int y )
{
	entity_t *self;

	self = SpawnActor( which, x, y, dir4_nodir, r_world );
	if( ! self ) 
	{
		return;
	}
	
	self->state = st_dead;
	self->speed = 0;
	self->health = 0;
	self->ticcount = objstate[ which ][ st_dead ].timeout ? US_RndT() % objstate[ which ][ st_dead ].timeout + 1 : 0;
  
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void SpawnBoss( enemy_t which, int x, int y )
{
	entity_t *self;
	dir4type face;

	switch( which )
	{
		case en_boss:
		case en_schabbs:
		case en_fat:
		case en_hitler:
			face = dir4_south;
			break;

		case en_fake:
		case en_gretel:
		case en_gift:
			face = dir4_north;
			break;

		case en_trans:
		case en_uber:
		case en_will:
		case en_death:
		case en_angel:
		case en_spectre:
			face = dir4_nodir;
			break;

		default:
			face = dir4_nodir;
			break;
	}

	self = SpawnActor( which, x, y, face, r_world );
	if( ! self )
	{
		return;
	}
	
	self->state = which == en_spectre ? st_path1 : st_stand;
	self->speed = SPDPATROL;
	self->health = starthitpoints[ (int)skill->value ][ which ];
 	self->ticcount = objstate[ which ][ st_stand ].timeout ? US_RndT() % objstate[ which ][ st_stand ].timeout + 1 : 0;
	self->flags |= FL_SHOOTABLE | FL_AMBUSH;

	levelstate.total_monsters++;
	
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void SpawnGhosts( enemy_t which, int x, int y )
{
	entity_t *self;

	self = SpawnActor( which, x, y, dir4_nodir, r_world );
	if( ! self )
	{
		return;
	}
	
	self->state = st_chase1;
	self->speed = SPDPATROL * 3;
	self->health = starthitpoints[ (int)skill->value ][ which ];
	self->ticcount = objstate[ which ][ st_chase1 ].timeout ? US_RndT() % objstate[ which ][ st_chase1 ].timeout + 1: 0;
	self->flags |= FL_AMBUSH;

	levelstate.total_monsters++;
}
