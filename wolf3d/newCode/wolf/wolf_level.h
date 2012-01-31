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
 *	wolf_level.h:  Wolfenstein3-D level management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Wolfenstein3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by wolf_areas.c, wolf_doors.c, wolf_level.c and
	wolf_pushwalls.c


*/

#ifndef __WOLF_LEVEL_H__
#define __WOLF_LEVEL_H__


///////////////////
//
//	Areas
//
///////////////////

#define NUMAREAS		37				// number of areas
#define FIRSTAREA		0x6B			// first area in map data (it is by the way a way to the secret floor!)
#define AMBUSHTILE		0x6A			// def guard
#define AMBUSH			-2

///////////////////
//
//	Doors
//
///////////////////
#define MAX_DOORS		256
#define DOOR_TIMEOUT	300
#define DOOR_MINOPEN	50

#define DOOR_FULLOPEN	63


#define DOOR_VERT		255
#define DOOR_HORIZ		254
#define DOOR_E_VERT		253
#define DOOR_E_HORIZ	252

#define DOOR_G_VERT		251
#define DOOR_G_HORIZ	250
#define DOOR_S_VERT		249
#define DOOR_S_HORIZ	248

#define FIRST_DOOR		248
#define LAST_LOCK		251




#define TEX_DOOR	126

// texture IDs used by cache routines
#define TEX_DDOOR	(0 + TEX_DOOR)        // Simple Door
#define TEX_PLATE	(2 + TEX_DOOR)        // Door Plate
#define TEX_DELEV	(4 + TEX_DOOR)        // Elevator Door
#define TEX_DLOCK	(6 + TEX_DOOR)        // Locked Door



///////////////////
//
//	Level
//
///////////////////
#define WALL_TILE		1
#define PUSHWALL_TILE		(1 << 20)
#define DOOR_TILE			2
#define SECRET_TILE			4
#define DRESS_TILE		8
#define BLOCK_TILE		16
#define ACTOR_TILE		32
#define DEADACTOR_TILE	64
#define POWERUP_TILE		128
#define AMBUSH_TILE		256
#define EXIT_TILE		512
#define SECRETLEVEL_TILE		1024
#define ELEVATOR_TILE 	(1 << 11)
#define TILE_IS_E_TURN		(1 << 12)
#define TILE_IS_NE_TURN		(1 << 13)
#define TILE_IS_N_TURN		(1 << 14)
#define TILE_IS_NW_TURN		(1 << 15)
#define TILE_IS_W_TURN		(1 << 16)
#define TILE_IS_SW_TURN		(1 << 17)
#define TILE_IS_S_TURN		(1 << 18)
#define TILE_IS_SE_TURN		(1 << 19)

#define SOLID_TILE (WALL_TILE | BLOCK_TILE | PUSHWALL_TILE)
#define BLOCKS_MOVE_TILE	(WALL_TILE | BLOCK_TILE | PUSHWALL_TILE | ACTOR_TILE)
#define WAYPOINT_TILE	(TILE_IS_E_TURN | TILE_IS_NE_TURN | TILE_IS_N_TURN | TILE_IS_NW_TURN | TILE_IS_W_TURN | TILE_IS_SW_TURN | TILE_IS_S_TURN | TILE_IS_SE_TURN )

///////////////////
//
//	Doors
//
///////////////////
typedef enum
{
	dr_closing = -1,
	dr_closed,
	dr_opening,
	dr_open

} dr_state;

typedef struct
{
	int tilex, tiley;
	_boolean vertical;
	int ticcount;

	dr_state action;
	
	int area1, area2;
/*DOOR_VERT			255
	DOOR_HORIZ		254
	DOOR_E_VERT		253
	DOOR_E_HORIZ	252
	DOOR_G_VERT		251
	DOOR_G_HORIZ	250
	DOOR_S_VERT		249
	DOOR_S_HORIZ	248*/
	int type;
	
	int texture;

}  doors_t;

typedef struct
{
	int doornum;
    doors_t *Doors[ 256 ];
	doors_t DoorMap[ 64 ][ 64 ];	

} LevelDoors_t;

///////////////////
//
//	Level
//
///////////////////
typedef struct
{	
	char fname[ 32 ]; /* Map filename */

	W16 Plane1[ 64 * 64 ]; /* walls */
	W16 Plane2[ 64 * 64 ]; /* objects */
	W16 Plane3[ 64 * 64 ]; /* other */
	
	
	long tilemap[ 64 ][ 64 ];	// wall values only
	W8 spotvis[ 64 ][ 64 ];
//    objtype		*actorat[ 64 ][ 64 ];

    // this is an array of references to texture descriptions
// the renderer must know what to draw by this number
	int wall_tex_x[ 64 ][ 64 ]; // x_wall
	int wall_tex_y[ 64 ][ 64 ]; // y_wall

// this is a (0-based) array of area numbers!
// must be all filled by level loading sub
// if -1 it is a wall, if -2 it is a door, if -3 it is unknown
	int areas[ 64 ][ 64 ];

	LevelDoors_t Doors;

	placeonplane_t pSpawn; // player spawn place


	char mapName[128];		/* Map name */
	char musicName[128];	/* Music file name */
	
	colour3_t ceilingColour, floorColour;

	W8 tileEverVisible[ 64 ][ 64 ];	// for automap
} LevelData_t;


typedef struct statinfo_t
{
	_boolean block;
	int powerup;

} statinfo_t;


extern LevelData_t *r_world;


extern LevelData_t *Level_LoadMap( const char *levelname );
extern void Level_PrecacheTextures_Sound( LevelData_t *lvl );
extern _boolean Level_CheckLine( SW32 x1, SW32 y1, SW32 x2, SW32 y2, LevelData_t *lvl );
extern void Level_ScanInfoPlane( LevelData_t *lvl );

///////////////////
//
//	Doors
//
///////////////////
extern void Door_ResetDoors( LevelDoors_t *lvl );
extern int Door_SpawnDoor( LevelDoors_t *lvl, int x, int y, int type );
extern void Door_SetAreas( LevelDoors_t *lvl, int (*areas)[64] );
extern void Door_OpenDoor( doors_t *Door );
extern void Door_ProcessDoors_e( LevelDoors_t *lvl, int t_tk, int t_ms );
extern int Door_Opened( LevelDoors_t *lvl, int x, int y );
extern _boolean Door_TryUse( doors_t *Door, int keys );

///////////////////
//
//	Areas
//
///////////////////
extern	_boolean	areabyplayer[ NUMAREAS ];

extern void Areas_ConnectAreas( int areanumber );
extern void Areas_InitAreas( int areanumber );
extern void Areas_JoinAreas( int area1, int area2 );
extern void Areas_DisconnectAreas( int area1, int area2 );


///////////////////
//
//	Push Walls
//
///////////////////
typedef struct
{
	_boolean active;
	int PWtilesmoved;
	int PWpointsmoved;
	dir4type dir;
	int x, y;
	int dx, dy;
	int tex_x, tex_y;

} Pwall_t;


extern Pwall_t PWall;


extern void PushWall_Reset( void );
extern _boolean PushWall_Push( int x, int y, dir4type dir );
extern void PushWall_Process( void );


#endif /* __WOLF_LEVEL_H__ */
