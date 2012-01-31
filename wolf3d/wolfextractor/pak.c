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
 *	pak.c:   Creates PAK files for Wolfenstein 3-D Redux
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004  
 *
 */

#include <string.h>

#include "wolf/wolf_def.h"
#include "mac/mac.h"
#include "memory/memory.h"
#include "../../common/arch.h"
#include "../../common/common_utils.h"
#include "string/com_string.h"
#include "filesys/file.h"
#include "zip/zip.h"

#include "../../zlib/zlib.h"


#define SCRIPTNAME		"DEFAULT.CFG"
#define GVERSION_TEXT	"set g_version "

#define SCRIPT_DIR		"script"


PRIVATE struct zlist *zfchain = NULL; // Zip file chain



/*
-----------------------------------------------------------------------------
 Function: writelocalfilechunk() -Zip file.
 
 Parameters: filename -[in] Pointer to a NUL-terminated string that specifies 
                      the path of the file to zip. 
			fout -[in] file to zip to.
 
 Returns: On success pointer to zlist structure, otherwise NULL.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE struct zlist *writelocalfilechunk( const char *filename, FILE *fout )
{
	W8 *data;
	struct zlist *zentry;
	struct filestats fs;
	int err;
	W8 *compr;
	W32 retval;
	z_stream c_stream; /* compression stream */


	zentry = MM_MALLOC( sizeof( *zentry ) );

	memset( zentry, 0, sizeof( *zentry ) );
	zentry->next = NULL;
	
	zentry->versionmadeby = VMB_VFAT;
	zentry->versionneeded = 20;
	zentry->disknumstart = 0;
	zentry->compression_method = CM_DEFLATED;

	zentry->deletefile = 1;

	zentry->uncompressed_size = FS_LoadFile( filename, &data );

	if( zentry->uncompressed_size == -1 || data == NULL )
	{
		printf( "Could not open file (%s)\n", filename );
		MM_FREE( zentry );

		return NULL;
	}	


	FS_GetFileAttributes( filename, &fs );

	zentry->timedate = UnixTimeToDosTime( &fs.lastwritetime );
	

//
//	Compression
//
	c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;

	err = deflateInit( &c_stream, Z_DEFAULT_COMPRESSION );
	if( err != Z_OK )
	{
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	zentry->compressed_size = (zentry->uncompressed_size / 10) + 12 + zentry->uncompressed_size;


	compr = MM_MALLOC( zentry->compressed_size );


	
	

	c_stream.next_out = compr;
	c_stream.avail_out = (uInt)zentry->compressed_size;

	c_stream.next_in = data;
	c_stream.avail_in = (uInt)zentry->uncompressed_size;
	

	err = deflate( &c_stream, Z_FINISH );
	if( err != Z_STREAM_END )
	{
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	err = deflateEnd( &c_stream );
    if( err != Z_OK )
	{
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	//	When using the deflate method, ZLib adds a 2 byte head
	//	and a 4 byte tail. The head must be removed for zip
	//	compatability and the tail is not necessary. 
	zentry->compressed_size = c_stream.total_out - 6;

//
//	End of compression
//


	zentry->crc32 = crc32( 0, data, zentry->uncompressed_size );

	cs_strlcpy( zentry->filename, filename, sizeof( zentry->filename ) );
	zentry->filename_length = strlen( zentry->filename );

	zentry->offset = ftell( fout );

	// Write header to file
	if( ! zip_writelocalfileheader( zentry, fout ) )
	{
		printf( "Error writing local header to zip file\n" );
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}

	// Write data to file
	retval = fwrite( compr+2, 1, zentry->compressed_size, fout );
	if( retval != zentry->compressed_size )
	{
		printf( "Error writing data after local header to zip file\n" );
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}

	MM_FREE( compr );
	MM_FREE( data );


	return zentry;
}

/*
-----------------------------------------------------------------------------
 Function: writecentralchunk() -Write central headers for Zip file.
 
 Parameters: z -[in] Chain of zlist structures.
			fout -[in] file to write to.
 
 Returns: On success true, otherwise false.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE _boolean writecentralchunk( struct zlist *z, FILE *fout )
{
	W32 central_offset;
	W32 central_size;
	W32 num = 0;
	struct zlist *ztemp;

	if( z == NULL )
	{
		printf( "NULL zip list passed into writecentralchunk().\n" );

		return false;
	}

	central_offset = ftell( fout );

	ztemp = z;
	do
	{
		++num;
		if( ! zip_writecentral( ztemp, fout ) )
		{
			printf( "Error writing central header to zip file\n" );
			return false;
		}

		ztemp = ztemp->next;

	} while( ztemp );

	central_size = ftell( fout ) - central_offset;

	if( ! zip_writeend( num, central_size, central_offset, 0, NULL, fout ) )
	{
		printf( "Error writing end header to zip file\n" );
		
		return false;
	}

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: parsedirectory() -Write central headers for Zip file.
 
 Parameters: path -[in] path to parse.
			fout -[in] zip file to write to.
 
 Returns: On success pointer to zlist structure, otherwise NULL.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE _boolean parsedirectory( const char *path, FILE *f )
{
	char temp[ 256 ];
	char *ptr;
	struct zlist *znewnode = NULL;


	cs_strlcpy( temp, path, sizeof( temp ) );

	if( strstr( temp, "*" ) == NULL )
	{
		cs_strlcat( temp, "/*", sizeof( temp ) );
	}

	// run findfirst once so we can use findnext in a loop.
	// This will return the current directory
	(void)FS_FindFirst( temp, 0, 0 );

	// Look for files
	while( (ptr = FS_FindNext( 0, FA_DIR )) != NULL )
	{
		znewnode = writelocalfilechunk( ptr, f );
		if( znewnode == NULL )
		{
			continue;
		}

		// add new link to chain
		znewnode->next = zfchain;
		zfchain = znewnode;

	}

	FS_FindClose();

	return true; // return pointer to start of chain
}

/*
-----------------------------------------------------------------------------
 Function: addscripttozipfile() -Add script file to zip file.
 
 Parameters: filename -[in] File name of script file.
			fout -[in] zip file to write to.
			version -[in] 
 
 Returns: On success pointer to zlist structure, otherwise NULL.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE struct zlist *addscripttozipfile( char *filename, FILE *fout, W16 version )
{
	W8 *data;
	struct zlist *zentry;
	struct filestats fs;
	int err;
	W32 retval;
	W8 *compr;
	z_stream c_stream; /* compression stream */
	char commandline[ 256 ];
	int value = 0;
	FILE *fin;

	fin = fopen( filename, "rb" );
	if( fin == NULL )
	{
		return NULL;
	}

	retval = FS_FileLength( fin );
	
	if( retval == -1 )
	{
		printf( "Could not open file (%s)\n", filename );
		
		return NULL;
	}	

	// add g_version command
	if( version == SDM_PAK || version == SOD_PAK )
	{
		value = 1;
	}	
	else
	{
		value = 0;
	}
	cs_snprintf( commandline, sizeof( commandline ), "\n%s%d\n", GVERSION_TEXT, value );


	data = MM_MALLOC( retval + strlen( commandline ) + 1 );

	if( fread( data, 1, retval, fin ) != retval )
	{
		printf( "Could not read from file (%s)\n", filename );

		MM_FREE( data )

		return NULL;
	}

	fclose( fin );

	memcpy( data + retval, commandline, strlen( commandline ) );


	zentry = MM_MALLOC( sizeof( *zentry ) );
	memset( zentry, 0, sizeof( *zentry ) );
	zentry->next = NULL;

	zentry->versionmadeby = VMB_VFAT;
	zentry->versionneeded = 20;
	zentry->disknumstart = 0;
	zentry->compression_method = CM_DEFLATED;

	zentry->deletefile = 0;

	zentry->uncompressed_size = retval + strlen( commandline );

	FS_GetFileAttributes( filename, &fs );

	zentry->timedate = UnixTimeToDosTime( &fs.lastwritetime );	

//
//	Compression
//
	c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;

	err = deflateInit( &c_stream, Z_DEFAULT_COMPRESSION );
	if( err != Z_OK )
	{
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	zentry->compressed_size = (zentry->uncompressed_size / 10) + 12 + zentry->uncompressed_size;


	compr = MM_MALLOC( zentry->compressed_size );


	
	

	c_stream.next_out = compr;
	c_stream.avail_out = (uInt)zentry->compressed_size;

	c_stream.next_in = data;
	c_stream.avail_in = (uInt)zentry->uncompressed_size;
	

	err = deflate( &c_stream, Z_FINISH );
	if( err != Z_STREAM_END )
	{
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	err = deflateEnd( &c_stream );
    if( err != Z_OK )
	{
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}


	//	When using the deflate method, ZLib adds a 2 byte head
	//	and a 4 byte tail. The head must be removed for zip
	//	compatability and the tail is not necessary. 
	zentry->compressed_size = c_stream.total_out - 6;

//
//	End of compression
//


	zentry->crc32 = crc32( 0, data, zentry->uncompressed_size );

	cs_strlcpy( zentry->filename, filename, sizeof( zentry->filename ) );
	zentry->filename_length = strlen( zentry->filename );

	zentry->offset = ftell( fout );

	// Write header to file
	if( ! zip_writelocalfileheader( zentry, fout ) )
	{
		printf( "Error writing local header to zip file\n" );
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}

	// Write data to file
	retval = fwrite( compr+2, 1, zentry->compressed_size, fout );
	if( retval != zentry->compressed_size )
	{
		printf( "Error writing data after local header to zip file\n" );
		MM_FREE( compr );
		MM_FREE( data );
		MM_FREE( zentry );

		return NULL;
	}

	MM_FREE( compr );
	MM_FREE( data );


	return zentry;
}

/*
-----------------------------------------------------------------------------
 Function: deletezlist() -delete zip file chain.
 
 Parameters: in -[in] zlist structure chain to delete.
			deletefile -[in] Delete the file?
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void deletezlist( struct zlist *in, _boolean deletefile )
{
	struct zlist *z1;

	if( in == NULL )
	{
		printf( "NULL zip list passed into deletezlist.\n" );

		return;
	}

	if( deletefile )
	{
		printf( "Removing cached files.\n" );
	}
	
	z1 = in->next;
	do
	{
		if( in )
		{
			// delete file
			if( deletefile && in->deletefile )
			{
				if( ! FS_DeleteFile( in->filename ) )
				{
					printf( "Unable to delete file (%s)\n", in->filename );
				}
			}

			MM_FREE( in );
		}
		
		if( z1 )
		{
			in = z1;
			z1 = in->next;
		}

	} while( in );

}

/*
-----------------------------------------------------------------------------
 Function: PAK_builder() -Builds a PAK file for Wolfenstein 3-D Redux.
 
 Parameters: path -[in] game path.
			packname -[in] Name of PAK file to create.
			version -[in] Versions of game files create/found.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void PAK_builder( const char *packname, W16 version )
{
	FILE *fout;

	printf( "\n\nGenerating pack file (%s)\nThis could take a few minutes.\n", packname );


	fout = fopen( packname, "wb" );
	if( fout == NULL )
	{
		printf( "[PAK_builder]: Could not create file (%s)\n", packname );
		return;
	}	


	// Script file should be first
	zfchain = addscripttozipfile( SCRIPTNAME, fout, version );
			

	if( version & WL1_PAK || version & WL6_PAK ||
		version & SDM_PAK || version & SOD_PAK  )
	{
		parsedirectory( MAPDIR, fout );
		parsedirectory( LGFXDIR, fout );
		parsedirectory( GFXWALLDIR, fout );
		parsedirectory( MUSICDIR, fout );

		if( version & WL1_PAK || version & WL6_PAK )
		{
			parsedirectory( SCRIPT_DIR, fout );
			parsedirectory( SFXDIR, fout );
			parsedirectory( GFXSPRITEDIR, fout );
			parsedirectory( LSFXDIR, fout );
		}

		if( version & SDM_PAK || version & SOD_PAK )
		{
			parsedirectory( SODSFXDIR, fout );
			parsedirectory( SODGFXSPRITEDIR, fout );
			parsedirectory( SODLSFXDIR, fout );
		}	
	}

	if( version & MAC_PAK )
	{
		parsedirectory( DIRPATHSPRITES, fout );
		parsedirectory( DIRPATHWALLS, fout );
		parsedirectory( DIRPATHPICS, fout );
		parsedirectory( DIRPATHMIDI, fout );		
	}


	if( ! writecentralchunk( zfchain, fout ) )
	{

		deletezlist( zfchain, false );

		// close and delete zip file.
		fclose( fout );

		FS_DeleteFile( packname );

		return;
	}


	// close zip file.
	fclose( fout );

	deletezlist( zfchain, true );
	zfchain = NULL;	

}

