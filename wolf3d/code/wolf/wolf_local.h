/*

	Copyright (C) 2004 Michael Liebscher

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
 *	wolf_local.h:  Wolfenstein3-D init.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Wolfenstein3-D, and was originally
 *	written by Id Software, Inc. 
 *
 */

/*
	Notes:	
	This module is implemented by wolf_main.c

*/

#ifndef __WOLF_LOCAL_H__
#define __WOLF_LOCAL_H__


// Game Version
#define WOLFENSTEINWL6	0x00
#define SPEAROFDESTINY	0x01



#define TILEGLOBAL	0x10000
#define HALFTILE	0x8000
#define TILESHIFT	16

#define MINDIST		(0x5800)





typedef enum difficulty_e
{
	gd_baby,
	gd_easy,
	gd_medium,
	gd_hard

} difficulty_t;

//
// this structure is cleared as each map is entered
//
typedef struct
{
	int			framenum;
	float		time;

	char		level_name[ MAX_OSPATH ];	// the descriptive name (Outer Base, etc)
	char		mapname[ MAX_OSPATH ];		// the server name (base1, etc)
	char		nextmap[ MAX_OSPATH ];		// go here when fraglimit is hit

	// intermission state
	W32			levelCompleted;				// in case the game was saved at the intermission
	
	W32			floornum;
	float		fpartime;
	char		spartime[6];
	
	W32			total_secrets;
	W32			found_secrets;

	W32			total_treasure;
	W32			found_treasure;

	W32			total_monsters;
	W32			killed_monsters;
	

} level_locals_t;



extern level_locals_t	levelstate;



typedef struct 
{
	W32	total_secrets;
	W32	found_secrets;

	W32	total_treasure;
	W32	found_treasure;

	W32	total_monsters;
	W32	killed_monsters;
	W32 time;

} LRstruct;


extern LRstruct LevelRatios;



extern cvar_t	*g_version;
extern cvar_t	*episode;
extern cvar_t	*skill;

extern int tics; 


W32 floornumber;


extern void Game_Init( void );
extern void Game_Reset( void );



extern void ProcessGuards( void );


#define WL6SPRITESDIRNAME	"sprites"
#define SODSPRITESDIRNAME	"sodsprites"


extern char *spritelocation;



#endif /* __WOLF_LOCAL_H__ */
