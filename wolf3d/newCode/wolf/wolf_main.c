/*

	Copyright (C) 2004-2005 Michael Liebscher

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

level_locals_t	levelstate;

LRstruct LevelRatios;

cvar_t	*g_version; // Wolfenstein or Spear of Destiny
cvar_t	*episode;
cvar_t	*skill;


char *spritelocation = WL6SPRITESDIRNAME;


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Game_Reset( void )
{
	memset( &levelstate, 0, sizeof( levelstate ) );
}

extern void Map_f( void );


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Game_Init( void )
{
	Com_Printf( "\n------ Game Init ------\n" );

	episode = Cvar_Get( "episode", "0", CVAR_ARCHIVE );
	skill = Cvar_Get( "skill", "1", CVAR_ARCHIVE );
	g_version = Cvar_Get( "g_version", "0", CVAR_ARCHIVE ); 

#ifndef EPISODE1	
	Cmd_AddCommand( "map", Map_f );
#endif
	
	G_Build_Tables();
	Powerup_Reset();
	Sprite_Reset();
	Game_Reset();
	PL_Init();

	Com_Printf( "\n-----------------------\n" );
	
}
