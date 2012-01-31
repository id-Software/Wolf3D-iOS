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
 *      zipfile.c:   Write zip file 
 *      
 *      Author:  Michael Liebscher    <johnnycanuck@users.sourceforge.net>  
 *
 */

/*

	Notes:

	Reference document:	
	http://www.pkware.com/company/standards/appnote/appnote.txt
	

*/


#include <stdio.h>

#include "../../../common/arch.h"
#include "../../../common/common_utils.h"
#include "zip.h"

/* Signatures for zip file information headers */
#define SIG_LOCAL			0x04034b50L
#define SIG_CENTRAL			0x02014b50L
#define SIG_END				0x06054b50L
#define SIG_EXTENDLOCAL		0x08074b50L
#define SIG_EXTENDSPLOCAL	0x30304b50L

/* Length of header (not counting the signature) */
#define LOCALHEAD_SIZE		26
#define CENTRALHEAD_SIZE	42
#define ENDHEAD_SIZE		18


/*
-----------------------------------------------------------------------------
 Function: zip_writelocalfileheader	-Write a local header to file.
 
 Parameters: 
			z -[in] zip entry to write local header for.
			f -[in] File to write to.

 Returns: 
 
 Notes: 

-----------------------------------------------------------------------------
*/
_boolean zip_writelocalfileheader( struct zlist *z, FILE *f )
{
	W32 temp32;
	W16 temp16;
	W32 retval;

	temp32 = LittleLong( SIG_LOCAL );
	retval = fwrite( &temp32, 1, 4, f );

	temp16 = LittleShort( z->versionneeded );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->localflag );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->compression_method );
	retval += fwrite( &temp16, 1, 2, f );

	temp32 = LittleLong( z->timedate );
	retval += fwrite( &temp32, 1, 4, f );
	
	temp32 = LittleLong( z->crc32 );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->compressed_size );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->uncompressed_size );
	retval += fwrite( &temp32, 1, 4, f );

	temp16 = LittleShort( z->filename_length );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->extrafield_length );
	retval += fwrite( &temp16, 1, 2, f );

	if( retval != LOCALHEAD_SIZE + 4 )
	{
		return false;
	}

	if( fwrite( z->filename, 1, z->filename_length, f ) != z->filename_length ||
		(z->extrafield && fwrite( z->extrafield, 1, z->extrafield_length, f) != z->extrafield_length) )
	{
		return false;
	}

	return true;
}



/*
-----------------------------------------------------------------------------
 Function: zip_writecentral	-Write a central header to file.
 
 Parameters: 
			z -[in] zip entry to write central header for.
			f -[in] File to write to.

 Returns: 
 
 Notes: 

-----------------------------------------------------------------------------
*/
_boolean zip_writecentral( struct zlist *z, FILE *f )
{
	W32 temp32;
	W16 temp16;
	W32 retval;

	temp32 = LittleLong( SIG_CENTRAL );
	retval = fwrite( &temp32, 1, 4, f );

	temp16 = LittleShort( z->versionmadeby );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->versionneeded );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->flag );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->compression_method );
	retval += fwrite( &temp16, 1, 2, f );

	temp32 = LittleLong( z->timedate );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->crc32 );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->compressed_size );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->uncompressed_size );
	retval += fwrite( &temp32, 1, 4, f );

	temp16 = LittleShort( z->filename_length );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->centralextra_length );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->comment_length );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->disknumstart );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( z->internalattribute );
	retval += fwrite( &temp16, 1, 2, f );

	temp32 = LittleLong( z->externalattribute );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( z->offset );
	retval += fwrite( &temp32, 1, 4, f );

	if( retval != CENTRALHEAD_SIZE + 4 )
	{
		return false;
	}

	if( fwrite(z->filename, 1, z->filename_length, f) != z->filename_length ||
		(z->centralextra_length && fwrite(z->centralextra, 1, z->centralextra_length, f) != z->centralextra_length) ||
		(z->comment_length && fwrite(z->comment, 1, z->comment_length, f) != z->comment_length) )
	{
		return false;
	}
	
	return true;
}

/*
-----------------------------------------------------------------------------
 Function: zip_writeend	-Write end of central directory data to file.
 
 Parameters: 
			num -[in] Number of entries in central directory.
			size -[in] Size of central directory.
			offset -[in] Offset of central directory.
			len -[in] Length of zip file comment (0 if none).
			comment -[in] Zip file comment if len != 0.
			f -[in] File to write to.

 Returns: 
 
 Notes: 

-----------------------------------------------------------------------------
*/
_boolean zip_writeend( SW32 num, W32 size, W32 offset, W16 len, char *comment, FILE *f )
{
	W32 temp32;
	W16 temp16;
	W32 retval;
	
	temp32 = LittleLong( SIG_END );
	retval = fwrite( &temp32, 1, 4, f );

	temp16 = 0;
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = 0;
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( num );
	retval += fwrite( &temp16, 1, 2, f );

	temp16 = LittleShort( num );
	retval += fwrite( &temp16, 1, 2, f );

	temp32 = LittleLong( size );
	retval += fwrite( &temp32, 1, 4, f );

	temp32 = LittleLong( offset );
	retval += fwrite( &temp32, 1, 4, f );

	temp16 = LittleShort( len );
	retval += fwrite( &temp16, 1, 2, f );

	if( retval != ENDHEAD_SIZE + 4 )
	{
		return false;
	}


	if( len && fwrite( comment, 1, len, f ) != len )
	{
		return false;
	}

	return true;
}
