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
 *	zmem.c:	Zone memory management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:

	Add the following line in your initization function:
	z_chain.next = z_chain.prev = &z_chain;

*/

#include "../wolfiphone.h"


//  just cleared malloc with counters now...


#define	Z_MAGIC		0x1d1d


PRIVATE int		z_count, z_bytes;

zhead_t	z_chain;



/*
-----------------------------------------------------------------------------
 Function: Z_Free -Deallocates or frees a zone memory block.
 
 Parameters:  	memblock -[in] Previously allocated zone memory block to be freed.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Z_Free( void *memblock )
{
	zhead_t	*z;

	z = ( (zhead_t *)memblock ) - 1;

	if( z->magic != Z_MAGIC )
	{
		Com_Error( ERR_FATAL, "Z_Free: bad magic" );
	}

	z->prev->next = z->next;
	z->next->prev = z->prev;

	z_count--;
	z_bytes -= z->size;

	MM_FREE( z );
}

/*
-----------------------------------------------------------------------------
 Function: Z_Stats_f -Console function to list zone memory usage.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: Lists number of bytes and blocks of zone memory allocated.
-----------------------------------------------------------------------------
*/
PUBLIC void Z_Stats_f( void )
{
	Com_Printf( "%i bytes in %i blocks\n", z_bytes, z_count );
}


/*
-----------------------------------------------------------------------------
 Function: Z_FreeTags -Free allocated zone memory blocks based on tag.
 
 Parameters:  	tag -[in] Tag of zone memory blocks to free (see header for tag).
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Z_FreeTags( int tag )
{
	zhead_t	*z, *next;

	for( z = z_chain.next; z != &z_chain; z = next )
	{
		next = z->next;
		if( z->tag == tag )
		{
			Z_Free( (void *)(z+1) );
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: Z_TagMalloc -Allocates zone memory blocks.
 
 Parameters: 
 			size -[in] Bytes to allocate. 	
 			tag -[in] Tag to associate with memory (see header for tag).
 
 Returns: 
 		A void pointer to the allocated space, or will shutdown application 
		if there is insufficient memory available.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void *Z_TagMalloc( size_t size, int tag )
{
	zhead_t	*z;

	// Allocate memory
	size += sizeof( zhead_t );
	z = MM_MALLOC( size );
	
	
	if( ! z )
	{
		Com_Error( ERR_FATAL, "Z_Malloc: failed on allocation of %i bytes", size );
	}

	// Set memory block to zero and fill in header.
	memset( z, 0, size );
	z_count++;
	z_bytes += size;
	z->magic = Z_MAGIC;
	z->tag = tag;
	z->size = size;

	// Add new memory block to chain.
	z->next = z_chain.next;
	z->prev = &z_chain;
	z_chain.next->prev = z;
	z_chain.next = z;

	return (void *)(z+1);
}

/*
-----------------------------------------------------------------------------
 Function: Z_Malloc -Allocates zone memory blocks.
 
 Parameters: size -[in] Bytes to allocate. 			
 
 Returns: 
 		A void pointer to the allocated space, or will shutdown application 
		if there is insufficient memory available.
 
 Notes: Calls Z_TagMalloc() with tag set to zero.
-----------------------------------------------------------------------------
*/
PUBLIC void *Z_Malloc( size_t size )
{
	return Z_TagMalloc( size, 0 );
}
