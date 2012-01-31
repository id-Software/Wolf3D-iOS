/*

	Copyright (C) 2004 Michael Liebscher
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
 *	zmem.h: Zone memory management.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc. 
 *
 */

/*
	Notes:	
	This module is implemented by zmem.c

*/

#ifndef __ZMEM_H__
#define __ZMEM_H__

#include <stdlib.h>

// memory tags to allow dynamic memory to be cleaned up
#define	TAG_GAME		765		/* clear when unloading the dll */
#define	TAG_LEVEL		766		/* clear when loading a new level */
#define	TAG_LEVEL_SCP	767		/* clear when unloading level script */



typedef struct zhead_s
{
	struct zhead_s	*prev, *next;

	short	magic;
	short	tag;			// for group free
	int		size;

} zhead_t;

extern zhead_t	z_chain;


// Returns 0 filled memory block
extern void *Z_Malloc( size_t size );

extern void *Z_TagMalloc( size_t size, int tag );
extern void Z_Free( void *memblock );
extern void Z_FreeTags( int tag );

extern void Z_Stats_f( void );


#endif /* __ZMEM_H__ */
