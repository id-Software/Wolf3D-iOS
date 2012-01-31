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
 *	wolf_areas.c:  Wolfenstein3-D area management.
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

	Open doors connect two areas, so sounds will travel between them and sight
	will be checked when the player is in a connected area.

	Areaconnect is incremented/decremented by each door. If >0 they connect.

	Every time a door opens or closes the areabyplayer matrix gets recalculated.
	An area is true if it connects with the player's current spor.

*/

#include "../wolfiphone.h"




W8			areaconnect[ NUMAREAS ][ NUMAREAS ];
_boolean	areabyplayer[ NUMAREAS ];



/*
-----------------------------------------------------------------------------
 Function: Areas_RecursiveConnect() -Scans outward from playerarea, 
									marking all connected areas. 
 
 Parameters: areanumber -[in] area. 
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Areas_RecursiveConnect( int areanumber )
{
	int	i;

	for( i = 0 ; i < NUMAREAS ; ++i )
	{
		if( areaconnect[ areanumber ][ i ] && ! areabyplayer[ i ] )
		{
			areabyplayer[ i ] = true;
			Areas_RecursiveConnect( i );
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: Areas_ConnectAreas() -Connect area. 
 
 Parameters: areanumber -[in] area. 
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Areas_ConnectAreas( int areanumber )
{
	int	c = 0;
	int	i;
	
	assert( areanumber < NUMAREAS );
	memset( areabyplayer, 0, sizeof( areabyplayer ) );
	areabyplayer[ areanumber ] = true;
	Areas_RecursiveConnect( areanumber );
	for ( i = 0 ; i < NUMAREAS ; i++ ) {
		if ( areabyplayer[i] ) {
			c++;
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
PUBLIC void Areas_InitAreas( int areanumber )
{
	memset( areaconnect, 0, sizeof( areaconnect ) );
	memset( areabyplayer, 0, sizeof( areabyplayer ) );
	areabyplayer[ areanumber ] = true;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Areas_JoinAreas( int area1, int area2 )
{// FIXME: check for overflow!
	areaconnect[ area1 ][ area2 ]++;
	areaconnect[ area2 ][ area1 ]++;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Areas_DisconnectAreas( int area1, int area2 )
{// FIXME: check for underflow!
	areaconnect[ area1 ][ area2 ]--;
	areaconnect[ area2 ][ area1 ]--;
}
