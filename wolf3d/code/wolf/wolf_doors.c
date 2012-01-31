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
 *	wolf_doors.c:  Wolfenstein 3-D door management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"

#define CLOSEWALL	MINDIST	// Space between wall & player
#define MAXDOORS	64		// max number of sliding doors




/*
-----------------------------------------------------------------------------
 Function: Door_ResetDoors -Resets doors status
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Door_ResetDoors( LevelDoors_t *lvldoors )
{
	lvldoors->doornum = 0;

	memset( lvldoors->Doors, 0, sizeof( lvldoors->Doors ) );
	memset( lvldoors->DoorMap, 0, sizeof( lvldoors->DoorMap ) );
}




/*
-----------------------------------------------------------------------------
 Function: Door_SpawnDoor -Spawns door at x, y, position.
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC int Door_SpawnDoor( LevelDoors_t *lvldoors, int x, int y, int type )
{
	if( lvldoors->doornum >= MAXDOORS )
	{
		Com_DPrintf( "[%s]: Too many Doors on level! (%d)\n", "wolf_doors.c", lvldoors->doornum );
		return 0;
	}

	switch( type )
	{
		case 0x5A:
			lvldoors->DoorMap[ x ][ y ].type	= DOOR_VERT;
			lvldoors->DoorMap[ x ][ y ].vertical= true;
			lvldoors->DoorMap[ x ][ y ].texture	= TEX_DDOOR + 1;
			break;

		case 0x5B:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_HORIZ;
			lvldoors->DoorMap[ x ][ y ].vertical = false;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DDOOR;
			break;

		case 0x5C:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_G_VERT;
			lvldoors->DoorMap[ x ][ y ].vertical = true;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DLOCK;
			break;

		case 0x5D:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_G_HORIZ;
			lvldoors->DoorMap[ x ][ y ].vertical = false;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DLOCK;
			break;

		case 0x5E:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_S_VERT;
			lvldoors->DoorMap[ x ][ y ].vertical = true;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DLOCK + 1;
			break;

		case 0x5F:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_S_HORIZ;
			lvldoors->DoorMap[ x ][ y ].vertical = false;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DLOCK + 1;
			break;

		case 0x64:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_E_VERT;
			lvldoors->DoorMap[ x ][ y ].vertical = true;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DELEV + 1;
			break;

		case 0x65:
			lvldoors->DoorMap[ x ][ y ].type = DOOR_E_HORIZ;
			lvldoors->DoorMap[ x ][ y ].vertical = false;
			lvldoors->DoorMap[ x ][ y ].texture = TEX_DELEV;
			break;

		default:
			Com_DPrintf( "Door_SpawnDoor: Unknown door type: %d\n", type );
			return 0;
	}

	lvldoors->DoorMap[ x ][ y ].tilex = x;
	lvldoors->DoorMap[ x ][ y ].tiley = y;
	lvldoors->DoorMap[ x ][ y ].action = dr_closed;

	lvldoors->Doors[ lvldoors->doornum ] = &lvldoors->DoorMap[ x ][ y ];
	lvldoors->doornum++;

	return lvldoors->doornum - 1;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Door_SetAreas( LevelDoors_t *lvldoors, int (*areas)[64] )
{
	int n, x, y;

	for( n = 0 ; n < lvldoors->doornum ; ++n )
	{
		x = lvldoors->Doors[ n ]->tilex;
		y = lvldoors->Doors[ n ]->tiley;

		if( lvldoors->Doors[ n ]->vertical )
		{
			lvldoors->Doors[ n ]->area1 = areas[ x + 1 ][ y ] >= 0 ? areas[ x + 1 ][ y ] : 0;
			lvldoors->Doors[ n ]->area2 = areas[ x - 1 ][ y ] >= 0 ? areas[ x - 1 ][ y ] : 0;
		}
		else
		{
			lvldoors->Doors[ n ]->area1 = areas[ x ][ y + 1 ] >= 0 ? areas[ x ][ y + 1 ] : 0;
			lvldoors->Doors[ n ]->area2 = areas[ x ][ y - 1 ] >= 0 ? areas[ x ][ y - 1 ] : 0;
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
PRIVATE W8 CanCloseDoor( int x, int y, char vert )
{
	int n;

	if( POS2TILE( Player.position.origin[ 0 ] ) == x &&
		POS2TILE( Player.position.origin[ 1 ] ) == y ) 
	{
		return 0;
	}

	if( vert )
	{
		if( POS2TILE( Player.position.origin[ 1 ] ) == y )
		{
			if( POS2TILE( Player.position.origin[ 0 ] + CLOSEWALL ) == x )
			{
				return 0;
			}

			if( POS2TILE( Player.position.origin[ 0 ] - CLOSEWALL ) == x )
			{
				return 0;
			}
		}

		for( n = 0 ; n < NumGuards ; ++n )
		{
			if( Guards[ n ].tilex == x && Guards[ n ].tiley == y ) 
			{
				return 0; // guard in door
			}

			if( Guards[ n ].tilex == x - 1 && 
				Guards[ n ].tiley == y && 
				POS2TILE( Guards[ n ].x + CLOSEWALL ) == x )
			{
				return 0; // guard in door
			}

			if( Guards[ n ].tilex == x + 1 &&
				Guards[ n ].tiley == y && 
				POS2TILE( Guards[ n ].x - CLOSEWALL ) == x )
			{
				return 0; // guard in door
			}
		}
	}
	else
	{
		if( POS2TILE( Player.position.origin[ 0 ] ) == x )
		{
			if( POS2TILE( Player.position.origin[ 1 ] + CLOSEWALL ) == y )
			{
				return 0;
			}
			
			if( POS2TILE( Player.position.origin[ 1 ] - CLOSEWALL ) == y )
			{
				return 0;
			}
		}
		for( n = 0 ; n < NumGuards ; ++n )
		{
			if( Guards[ n ].tilex == x && Guards[ n ].tiley == y )
			{
				return 0; // guard in door
			}
			
			if( Guards[ n ].tilex == x && 
				Guards[ n ].tiley == y - 1 && 
				POS2TILE( Guards[ n ].y + CLOSEWALL ) == y )
			{
				return 0; // guard in door
			}

			if( Guards[ n ].tilex == x &&
				Guards[ n ].tiley == y + 1 &&
				POS2TILE( Guards[ n ].y - CLOSEWALL ) == y )
			{
				return 0; // guard in door
			}
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
PUBLIC void Door_OpenDoor( doors_t *Door )
{
	if( Door->action == dr_open )
	{
		Door->ticcount = 0;		// reset opened time
	}
	else
	{
		Door->action = dr_opening;	// start opening it
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
PRIVATE void Door_ChangeDoorState( doors_t *Door )
{
	if( Door->action < dr_opening )
	{
		Door_OpenDoor( Door );
	}
	else if( Door->action == dr_open && CanCloseDoor( Door->tilex, Door->tiley, Door->vertical ) )
	{
		// !@# for the iphone with automatic using, don't allow any door close actions		
		// Door->action = dr_closing;
		// Door->ticcount = DOOR_FULLOPEN;
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
PUBLIC void Door_ProcessDoors_e( LevelDoors_t *lvldoors, int t_tk )
{
	int n;

	for( n = 0 ; n < lvldoors->doornum ; ++n )
	{
		switch( lvldoors->Doors[ n ]->action )
		{
			case dr_closed: // this door is closed!
				continue;
				
			case dr_opening:
				if( lvldoors->Doors[ n ]->ticcount >= DOOR_FULLOPEN ) // door fully opened!
				{
					lvldoors->Doors[ n ]->action = dr_open;
					lvldoors->Doors[ n ]->ticcount = 0;
				}
				else // opening!
				{
					if( lvldoors->Doors[ n ]->ticcount == 0 )
					{ // door is just starting to open, so connect the areas
						Areas_JoinAreas( lvldoors->Doors[ n ]->area1, lvldoors->Doors[ n ]->area2 );
						Areas_ConnectAreas( Player.areanumber );
						if( areabyplayer[ lvldoors->Doors[ n ]->area1 ] ) // Door Opening sound!
						{
							Sound_StartSound( NULL, 1, CHAN_AUTO, Sound_RegisterSound( "sfx/010.wav" ), 1, ATTN_STATIC, 0 );						
						}
					}

					lvldoors->Doors[n]->ticcount += t_tk;

					if( lvldoors->Doors[ n ]->ticcount > DOOR_FULLOPEN )
					{
						lvldoors->Doors[ n ]->ticcount = DOOR_FULLOPEN;
					}
				}
				break;

			case dr_closing:
				if( lvldoors->Doors[ n ]->ticcount <= 0 ) // door fully closed! disconnect areas!
				{
					Areas_DisconnectAreas( lvldoors->Doors[ n ]->area1, lvldoors->Doors[ n ]->area2 );
					Areas_ConnectAreas( Player.areanumber );
					lvldoors->Doors[ n ]->ticcount = 0;
					lvldoors->Doors[ n ]->action = dr_closed;
				}
				else // closing!
				{
					if( lvldoors->Doors[ n ]->ticcount == DOOR_FULLOPEN )
					{
						if( areabyplayer[ lvldoors->Doors[ n ]->area1 ] ) // Door Closing sound!
						{
							Sound_StartSound( NULL, 1, CHAN_AUTO, Sound_RegisterSound( "sfx/007.wav" ), 1, ATTN_STATIC, 0 );						
						}
					}
					lvldoors->Doors[ n ]->ticcount -= t_tk;
					if( lvldoors->Doors[ n ]->ticcount < 0 )
					{
						lvldoors->Doors[ n ]->ticcount = 0;
					}
				}
				break;

			case dr_open:
				if( lvldoors->Doors[ n ]->ticcount > DOOR_MINOPEN )
				{ // If player or something is in door do not close it!
					if( ! CanCloseDoor( lvldoors->Doors[ n ]->tilex, lvldoors->Doors[ n ]->tiley, lvldoors->Doors[ n ]->vertical ) )
					{
						lvldoors->Doors[ n ]->ticcount = DOOR_MINOPEN; // do not close door immediately!
					}
				}
				if( lvldoors->Doors[ n ]->ticcount >= DOOR_TIMEOUT )
				{ // Door timeout, time to close it!
					lvldoors->Doors[ n ]->action = dr_closing;
					lvldoors->Doors[ n ]->ticcount = DOOR_FULLOPEN;
				}
				else
				{ // Increase timeout!
					lvldoors->Doors[ n ]->ticcount += t_tk;
				}
				break;

		} // End switch lvldoors->Doors[ n ].action		

	} // End for n = 0 ; n < lvldoors->doornum ; ++n 

}

/*
-----------------------------------------------------------------------------
 Function: Door_Opened -Check to see if a door is open.
 
 Parameters:
 
 Returns: DOOR_FULLOPEN		Door is opened 
		  0					Door is closed
		  >0 <DOOR_FULLOPEN	Door is partially opened.
 
 Notes: 
	If there are no doors in tile assume a closed door!
-----------------------------------------------------------------------------
*/
PUBLIC int Door_Opened( LevelDoors_t *lvldoors, int x, int y )
{     
	return lvldoors->DoorMap[ x ][ y ].action == dr_open ? DOOR_FULLOPEN : lvldoors->DoorMap[ x ][ y ].ticcount;
}

/*
-----------------------------------------------------------------------------
 Function: Door_TryUse -Try to use a door with keys that the player has.
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean Door_TryUse( doors_t *Door, int keys )
{
	switch( Door->type )
	{
		case DOOR_VERT:
		case DOOR_HORIZ:
		case DOOR_E_VERT:
		case DOOR_E_HORIZ:
			Door_ChangeDoorState( Door ); // does not require key!
			break;

		case DOOR_G_VERT:
		case DOOR_G_HORIZ:
			if( keys & ITEM_KEY_1 )
			{
				Door_ChangeDoorState( Door );
			}
			else
			{
				iphoneSetNotifyText( "You need a gold key" );
			}
			break;

		case DOOR_S_VERT:
		case DOOR_S_HORIZ:
			if( keys & ITEM_KEY_2 )
			{
				Door_ChangeDoorState( Door );
			}
			else
			{
				iphoneSetNotifyText( "You need a silver key" );
			}
			break;
	}

	return true; // FIXME

}
