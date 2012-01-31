/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
	Copyright (C) 1997-2001 Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
 *	com_string.h: Common string functions done in a portable manner.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>
 *	Date:	2004
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 *  Portion of this code was derived from code that was originally
 *  written by Todd C. Miller.
 *
 */

/*
	Notes:	
	This module is implemented by com_string.c.

*/

#ifndef __COM_STRING_H__
#define __COM_STRING_H__

#include <stdlib.h>

#include "../../../common/arch.h"


#define ISSPACE( c )	( ( c ) == ' ' || ( c ) == '\f' || ( c ) == '\n' || ( c ) == '\r' || ( c ) == '\t' || ( c ) == '\v' )
#define ISUPPER( c )	( ( c ) >= 'A' && ( c ) <= 'Z' )
#define ISLOWER( c )	( ( c ) >= 'a' && ( c ) <= 'z' )
#define ISALPHA( c )	( ISUPPER( c ) || ISLOWER( c ) )
#define TOUPPER( c )	( ISLOWER( c ) ? (c) - 'a' + 'A' : ( c ) )
#define TOLOWER( c )	( ISUPPER( c ) ? (c) - 'A' + 'a' : ( c ) )



extern size_t cs_strlcpy( char *dest, const char *source, size_t nMaxLength );
extern size_t cs_strlcat( char *dest, const char *source, size_t nMaxLength );


extern int cs_stricmp( const char *string1, const char *string2 );
extern int cs_strnicmp( const char *string1, const char *string2, size_t count );


extern void cs_snprintf( char *dest, size_t size, const char *format, ... );

extern char *cs_CopyString( const char *in );

extern W32 cs_strhash( const char *string );

extern char *cs_strupr( char *string );
extern char *cs_strlwr( char *string );


#endif /* __COM_STRING_H__ */
