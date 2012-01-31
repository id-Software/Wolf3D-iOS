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
 *	filestring.c:   Portable file path/name manipulation methods.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"


/*
-----------------------------------------------------------------------------
 Function: FS_CreatePath -Creates given path.
 
 Parameters: path -[in] Pointer to NULL terminated string that contains directory 
				   path.		 
 
 Returns: Nothing.
 
 Notes: Creates any directories needed to store the given filename.

-----------------------------------------------------------------------------
*/
PUBLIC void FS_CreatePath( char *path )
{
	char	*ofs;
	
	for( ofs = path + 1; *ofs; ofs++ )
	{
		if( *ofs == '/' )
		{	// create the directory
			*ofs = '\0';
			FS_CreateDirectory( path );
			*ofs = '/';
		}
	}

}

PUBLIC void FS_FilePath( char *in, char *out )
{
	char *s;
	
	s = in + strlen( in ) - 1;
	
	while( s != in && *s != '/' )
	{
		s--;
	}

	strncpy( out, in, s-in );
	out[ s - in ] = '\0'; // NUL-terminate string.
}

PUBLIC char *FS_SkipPath( char *pathname )
{
	char	*last;
	
	last = pathname;
	while( *pathname )
	{
		if( *pathname == '/' )
		{
			last = pathname + 1;
		}
		pathname++;
	}

	return last;
}

PUBLIC void FS_StripExtension( char *in, char *out )
{
	while( *in && *in != '.' )
	{
		*out++ = *in++;
	}

	*out = '\0'; // NUL-terminate string.
}

PUBLIC char *FS_FileExtension( char *in )
{
	static char exten[ 8 ];
	int		i;

	while( *in && *in != '.' )
	{
		in++;
	}

	if( ! *in )
	{
		return "";
	}

	in++;
	for( i = 0 ; i < 7 && *in ; i++, in++ )
	{
		exten[ i ] = *in;
	}

	exten[ i ] = '\0'; // NUL-terminate string.

	return exten;
}

PUBLIC void FS_FileBase( char *in, char *out )
{
	char *s, *s2;
	
	s = in + strlen( in ) - 1;
	
	while( s != in && *s != '.' )
	{
		s--;
	}
	
	for( s2 = s ; s2 != in && *s2 != '/' ; s2-- )
	{
		;
	}
	
	if( s - s2 < 2 )
	{
		out[ 0 ] = '\0'; // NUL-terminate string.
	}
	else
	{
		s--;
		strncpy( out, s2 + 1, s - s2 );
		out[ s - s2 ] = '\0'; // NUL-terminate string.
	}
}


