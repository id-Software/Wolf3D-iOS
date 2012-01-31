/*

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

#include "../wolfiphone.h"


char	com_token[128];




/*
============================================================================

					BYTE ORDER FUNCTIONS

============================================================================
*/



/*
-----------------------------------------------------------------------------
 Function: va() -Does a varargs printf into a temp buffer, so I don't need to
				have varargs versions of all text functions. 
 
 Parameters: format -[in] Format-control string.
			 ... -[in] Optional arguments.
 
 Returns: Formatted string.
 
 Notes: 
	If format string is longer than 1024 it will be truncated.
-----------------------------------------------------------------------------
*/
PUBLIC char *va( char *format, ... )
{
	va_list	argptr;
	static char	string[ 1024 ];
	
	va_start( argptr, format );
	(void)vsnprintf( string, sizeof( string ), format, argptr );
	va_end( argptr );

	string[ sizeof( string ) - 1 ] = '\0';

	return string;	
}


/*
-----------------------------------------------------------------------------
 Function: COM_Parse() -Parse a token out of a string. 
 
 Parameters: data_p -[in] String to parse. 
 
 Returns: On success it will return the token string, otherwise it will
			return "".
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *COM_Parse( char **data_p )
{
	int		c;
	int		len;
	char	*data;

	data = *data_p;
	len = 0;
	com_token[ 0 ] = 0;
	
	if( ! data )
	{
		*data_p = NULL;
		return "";
	}
		
// skip whitespace
skipwhite:
	while( (c = *data) <= ' ')
	{
		if( c == 0 )
		{
			*data_p = NULL;
			return "";
		}
		data++;
	}
	
// skip // comments
	if( c == '/' && data[ 1 ] == '/' )
	{
		while( *data && *data != '\n' )
		{
			data++;
		}
		goto skipwhite;
	}

// handle quoted strings specially
	if( c == '\"' )
	{
		data++;
		while( 1 )
		{
			c = *data++;
			if( c == '\"' || ! c )
			{
				com_token[ len ] = 0;
				*data_p = data;
				return com_token;
			}
			if( len < MAX_TOKEN_CHARS )
			{
				com_token[ len ] = c;
				len++;
			}
		}
	}

// parse a regular word
	do
	{
		if( len < MAX_TOKEN_CHARS )
		{
			com_token[ len ] = c;
			len++;
		}
		data++;
		c = *data;

	} while( c > 32 );

	if( len == MAX_TOKEN_CHARS )
	{
//		Com_Printf ("Token exceeded %i chars, discarded.\n", MAX_TOKEN_CHARS);
		len = 0;
	}
	com_token[ len ] = 0;

	*data_p = data;

	return com_token;
}


/*
-----------------------------------------------------------------------------
 Function: Com_PageInMemory() 
 
 Parameters:
 
 Returns: 
 
 Notes: 

-----------------------------------------------------------------------------
*/
int	paged_total;

PUBLIC void Com_PageInMemory( PW8 buffer, int size )
{
	int		i;

	for( i = size - 1 ; i > 0 ; i -= 4096 )
	{
		paged_total += buffer[ i ];
	}
}

