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
 *	wolf_actors.h:  Wolfenstein3-D entity management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 *	Portion of this code was derived from Wolfenstein3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by wolf_actors.c

*/

#ifndef __WOLF_ACTORS_H__
#define __WOLF_ACTORS_H__

#define SPDPATROL	512
#define SPDDOG		1500

#define FL_SHOOTABLE		1
#define FL_BONUS			2
#define FL_NEVERMARK		4
#define FL_VISABLE			8
#define FL_ATTACKMODE		16
#define FL_FIRSTATTACK		32
#define FL_AMBUSH			64
#define FL_NONMARK			128

#define MAX_GUARDS	255
#define NUMENEMIES	31
#define NUMSTATES	34

#define MINACTORDIST	0x10000	// minimum dist from player center to any actor center

typedef enum
{
	en_guard,
	en_officer,
	en_ss,
	en_dog,
	en_boss,
	en_schabbs,
	en_fake,
	en_mecha,
	en_hitler,
	en_mutant,
	en_blinky,
	en_clyde,
	en_pinky,
	en_inky,
	en_gretel,
	en_gift,
	en_fat,
// --- Projectiles
	en_needle,
	en_fire,
	en_rocket,
	en_smoke,
	en_bj,
// --- Spear of destiny!
	en_spark,
	en_hrocket,
	en_hsmoke,

	en_spectre,
	en_angel,
	en_trans,
	en_uber,
	en_will,
	en_death

} enemy_t;

typedef enum
{
	st_stand,
	st_path1,	st_path1s,	st_path2,	st_path3,	st_path3s,	st_path4,
	st_pain, st_pain1,
	st_shoot1, st_shoot2, st_shoot3, st_shoot4, st_shoot5, st_shoot6, st_shoot7, st_shoot8, st_shoot9,
	st_chase1, st_chase1s, st_chase2, st_chase3, st_chase3s, st_chase4,
	st_die1, st_die2, st_die3, st_die4, st_die5, st_die6, st_die7, st_die8, st_die9,
	st_dead,
	st_remove

} en_state;

typedef struct entity_s
{
	int x, y, angle;
	int type;
	int health;
	int max_health;
	int speed;
	int ticcount;
	int temp2;
	int distance;
	char tilex, tiley;
	char areanumber;
	int		waitfordoorx, waitfordoory;	// waiting on this door if non 0
	W8 flags;				//	FL_SHOOTABLE, etc
	en_state state;
	dir8type dir;
	int sprite;

} entity_t;

typedef void (*think_t)( entity_t *self );

typedef struct
{
	char rotate; // 1-if object can be rotated, 0 if one sprite for every direction
	int texture; // base object's state texture if rotation is on facing player
	int timeout; // after how man ticks change state to .next_state
	think_t think; // what to do every frame
	think_t action; // what to do once per state
	en_state next_state; // next state

} stateinfo;



extern entity_t Guards[ MAX_GUARDS + 1 ];
extern entity_t *New;
extern W16 NumGuards;
extern stateinfo objstate[ NUMENEMIES ][ NUMSTATES ];

extern void ResetGuards(void);


extern entity_t *GetNewActor( void );
extern entity_t *SpawnActor( enemy_t which, int x, int y, dir4type dir, LevelData_t *lvl );
extern void A_StateChange( entity_t *Guard, en_state NewState );


extern void SpawnStand( enemy_t which, int tilex, int tiley, int dir, LevelData_t *lvl );
extern void SpawnPatrol( enemy_t which, int tilex, int tiley, int dir );
extern void SpawnDeadGuard( enemy_t which, int x, int y );
extern void SpawnBoss( enemy_t which, int x, int y );
extern void SpawnGhosts( enemy_t which, int x, int y );


#endif /* __WOLF_ACTORS_H__ */

