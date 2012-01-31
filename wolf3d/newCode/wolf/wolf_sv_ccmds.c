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

#include "../wolfiphone.h"

extern void Client_PrepRefresh( const char *r_mapname );


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void SV_GameMap_f( void )
{
	char		*map;
	char r_mapname[ 32 ];

	if( Cmd_Argc() != 2 )
	{
		Com_Printf( "USAGE: gamemap <map>\n" );
		return;
	}

//	Com_DPrintf( "SV_GameMap( %s )\n", Cmd_Argv( 1 ) );

	FS_CreatePath( va( "%s/save/current/", FS_Gamedir() ) );

	// check for clearing the current savegame
	map = Cmd_Argv( 1 );	

	// start up the next map
	my_strlcpy( r_mapname, Cmd_Argv( 1 ), sizeof( r_mapname ) );

	Client_PrepRefresh( r_mapname );	
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Map_f( void )
{
	char	*map;
	char	expanded[ MAX_GAMEPATH ];

	// Check to make sure the level exists.
	map = Cmd_Argv( 1 );
	if( ! strstr( map, "." ) )
	{
		my_snprintf( expanded, sizeof( expanded ), "maps/%s.map", map );
	}
	else
	{
		my_snprintf( expanded, sizeof( expanded ), "maps/%s", map );		
	}


//sv.state = ss_dead;		// don't save current level when changing
//	SV_WipeSavegame( "current" );
	SV_GameMap_f();
}

