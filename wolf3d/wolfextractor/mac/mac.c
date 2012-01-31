/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
	Copyright (C) 2000 Steven Fuller <relnev@atdot.org>

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
 *	mac.c:	This module is used to extract data from the Macintosh 
 *			version of Wolfenstein 3-D.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from code that was originally
 *	written by Steven Fuller.
 *
 */



#include <stdio.h>
#include <string.h>

#include "mac.h"
#include "../wolf/wolf_def.h"
#include "../../../common/arch.h"
#include "../../../common/common_utils.h"
#include "../loaders/tga.h"
#include "../memory/memory.h"
#include "../string/com_string.h"
#include "../filesys/file.h"




/* Resource file Palette offsets */
#define MACPAL		898370L
#define IDPAL		899142L
#define GAMEPAL		946811L
#define INTERPAL	997211L
#define YUMMYPAL	1011345L
#define TITLEPAL	2235007L

#define PALETTE_SIZE	768



#define MACBINFILENAME	"Wolfenstein 3D"
#define FILETYPECREATOR	"APPLWOLF"


#define DATAFORKLENGTH	105754L
#define RESFORKLENGTH	2424697L


PRIVATE W8 *macPalette;

PRIVATE FILE *fResHandle;


/*
-----------------------------------------------------------------------------
 Function: getResourceBlock -Get block of resource data.
 
 Parameters: offset -[in] Number of bytes from start of file.
			length -[in] Length of resource block.
			glen -[out] next four bytes after block.
 
 Returns: NULL on error, otherwise pointer to block of memory.
 
 Notes: Caller is responsible for freeing memory block.
-----------------------------------------------------------------------------
*/
PRIVATE W8 *getResourceBlock( W32 offset, W32 length, W32 *glen )
{
	W8 *buf;
	
	fseek( fResHandle, offset, SEEK_SET );

	buf = (PW8)MM_MALLOC( length );
	if( buf == NULL )
	{
		printf( "could not malloc data\n" );
		
		return NULL;
	}

	if( fread( buf, 1, length, fResHandle ) != length )
	{
		printf( "read error on resource file\n" );

		return NULL;
	}

	if( fread( glen, 1, 4, fResHandle ) != 4 )
	{
		printf( "read error on resource file\n" );

		return NULL;
	}

	*glen = BigLong( *glen );

	return buf;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: 
 
 Returns: 
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void Decode_LZSS( W8 *dst, const W8 *src, W32 Length )
{
	W32 bitBucket;
	W32 runCount;
	W32 Fun;
	W8 *ptrBack;
	
	if( ! Length ) 
	{
		return;
	}

	bitBucket = (W32) src[ 0 ] | 0x100;
	++src;

	do 
	{
		if( bitBucket & 1 ) 
		{
			dst[ 0 ] = src[ 0 ];
			++src;
			++dst;
			--Length;
		} 
		else
		{
			runCount = (W32) src[ 0 ] | ((W32) src[ 1 ] << 8);
			Fun = 0x1000 - (runCount & 0xFFF);
			ptrBack = dst - Fun;
			runCount = ((runCount >> 12) & 0x0F) + 3;
			if( Length >= runCount ) 
			{
				Length -= runCount;
			} 
			else 
			{
				runCount = Length;
				Length = 0;
			}

			while( runCount-- ) 
			{
				*dst++ = *ptrBack++;
			}
			src += 2;
		}

		bitBucket >>= 1;
		if( bitBucket == 1 ) 
		{
			bitBucket = (W32) src[ 0 ] | 0x100;
			++src;
		}

	} while( Length );
}

///////////////////////////////////////////////////////////////////////////////
//
//	Palette Routines
//
///////////////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: ConvertPaletteToRGB -Convert 256 palette data to RGB.
 
 Parameters: dst -[in/out] Destination for RGB raw image data.
			src -[in] 256 palette data.
			length -[in] length of source data.
			palette -[in] Pointer to 256*3 array.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void ConvertPaletteToRGB( W8 *dst, W8 *src, W32 length, W8 *palette )
{
	W32 i;

	for( i = 0 ; i < length ; ++i ) 
	{
		dst[ i * 3 ]		= palette[ src[ i ] * 3 ];
		dst[ i * 3 + 1 ]	= palette[ src[ i ] * 3 + 1 ];
		dst[ i * 3 + 2 ]	= palette[ src[ i ] * 3 + 2 ];
	}
}

/*
-----------------------------------------------------------------------------
 Function: setPalette -Set global palette.
 
 Parameters: offset -[in] offset of palette in resource file.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void setPalette( W32 offset )
{
	W32 junk;

	if( macPalette )
	{
		MM_FREE( macPalette );
	}

	macPalette = getResourceBlock( offset, PALETTE_SIZE, &junk );

}

///////////////////////////////////////////////////////////////////////////////
//
//	Screen Routines
//
///////////////////////////////////////////////////////////////////////////////

PRIVATE void DecodeBJMapImage( W32 offset, W32 length )
{
	W8 *ptrResource;
	W8 *buffer;
	char filename[ 32 ];
	W32 junk;

	ptrResource = (PW8)getResourceBlock( offset, length, &junk );
	if( ! ptrResource )
	{
		return;
	}

	buffer = (PW8) MM_MALLOC( 16 * 16 * 3 );

	ConvertPaletteToRGB( buffer, ptrResource, 16 * 16, macPalette );

	cs_snprintf( filename, sizeof( filename ), "%s/bjautomap.tga", DIRPATHPICS );

	WriteTGA( filename, 24, 16, 16, buffer, 0, 1 );

	MM_FREE( buffer );

}

PRIVATE void DecodeBJIntermImages( W32 offset, W32 length )
{
	W32 *ptrResource;
	W32 uncomprLength;
	W8 *uncompr, *buffer;
	W32 indexs[3];
	W16 *ptr;
	int width, height, i;
	W32 junk;
	char filename[ 32 ];
	
	ptrResource = (PW32)getResourceBlock( offset, length, &junk );
	if( ! ptrResource )
	{
		return;
	}
	
	uncomprLength = BigLong( ptrResource[ 0 ] );

	uncompr = (PW8)MM_MALLOC( uncomprLength );

	Decode_LZSS( uncompr, (PW8)&ptrResource[ 1 ], uncomprLength );
	
	MM_FREE( ptrResource );
	
	memcpy( indexs, uncompr, 12 );
	
	indexs[ 0 ] = BigLong( indexs[ 0 ] );
	indexs[ 1 ] = BigLong( indexs[ 1 ] );
	indexs[ 2 ] = BigLong( indexs[ 2 ] );
	
		
	buffer = MM_MALLOC( 256 * 256 * 3 );
	for( i = 0 ; i < 3 ; ++i )
	{
		ptr = (PW16)&uncompr[ indexs[ i ]  ];
		width = BigShort( ptr[ 0 ] );
		height = BigShort( ptr[ 1 ] );
		
		ConvertPaletteToRGB( buffer, (PW8)&ptr[ 2 ], width * height, macPalette );
		
		cs_snprintf( filename, sizeof( filename ), "%s/bj%d.tga", DIRPATHPICS, i );

		WriteTGA( filename, 24, width, height, buffer, 0, 1 );
	}
	
	MM_FREE( buffer );
	
	MM_FREE( uncompr );
}

/*
-----------------------------------------------------------------------------
 Function: DecodeScreen -Decode screen from Macintosh resource fork.
 
 Parameters: 
			offset -[in] offset in resource file.
			length -[in] length of data.
			filename -[in] Name of file to save as.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void DecodeScreen( W32 offset, W32 length, const char *filename )
{
	W8 *ptrResource;
	W32 uncomprLength;
	W16 *uncompr;
	W16 width, height;
	W8 *buffer;
	W32 junk;

	ptrResource = getResourceBlock( offset, length, &junk );
	if( ! ptrResource )
	{
		return;
	}

	uncomprLength = BigLong( *((PW32)ptrResource) );
	uncompr = (PW16)MM_MALLOC( uncomprLength );

	Decode_LZSS( (PW8)uncompr, ptrResource+4, uncomprLength );

	width = BigShort( uncompr[ 0 ] );
	height = BigShort( uncompr[ 1 ] );

	buffer = MM_MALLOC( width * height * 3 );
	
	ConvertPaletteToRGB( buffer, (PW8)&uncompr[ 2 ], width * height, macPalette );

	WriteTGA( filename, 24, width, height, buffer, 0, 1 );

	MM_FREE( buffer );
    MM_FREE( uncompr );
	MM_FREE( ptrResource );
}


/*
-----------------------------------------------------------------------------
 Function: RipScreens -Extract screens from Macintosh resource fork.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RipScreens( void )
{
	char path[ 256 ];

	setPalette( MACPAL );
	cs_snprintf( path, sizeof( path ), "%s/macplaypic.tga", DIRPATHPICS );
	DecodeScreen( 899914, 46893, path );

	setPalette( IDPAL );
	cs_snprintf( path, sizeof( path ), "%s/idpic.tga", DIRPATHPICS );
	DecodeScreen( 899914, 46893, path );
	
	setPalette( INTERPAL );
	cs_snprintf( path, sizeof( path ), "%s/intermissionpic.tga", DIRPATHPICS );
	DecodeScreen( 947583, 49624, path );


	DecodeBJIntermImages( 1018931, 20766 );

	
	setPalette( GAMEPAL );
	cs_snprintf( path, sizeof( path ), "%s/getpsychedpic.tga", DIRPATHPICS );
	DecodeScreen( 997983, 3256, path );


	DecodeBJMapImage( 2391746, 1281 );



	setPalette( YUMMYPAL );
	cs_snprintf( path, sizeof( path ), "%s/yummypic.tga", DIRPATHPICS );
	DecodeScreen( 1002299, 9042, path );

	setPalette( TITLEPAL );
	cs_snprintf( path, sizeof( path ), "%s/titlepic.tga", DIRPATHPICS );
	DecodeScreen( 2067971, 167032, path ); 


}


///////////////////////////////////////////////////////////////////////////////
//
//	Wall Routines
//
///////////////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: obverseWall -Realign wall image.
 
 Parameters: 
			src -[in] source data.
			width -[in] width of wall image.
			height -[in] height of wall image.
 
 Returns: NULL on error, otherwise pointer to memory block with realigned
			wall image data.
 
 Notes: Caller is responsible for freeing returned memory block.
-----------------------------------------------------------------------------
*/
PRIVATE W8 *obverseWall( const W8 *src, W16 width, W16 height )
{
	W8 *target;
	W16 w, h;
	
	target = MM_MALLOC( width * height );
	
	for( h = 0; h < height; ++h )
	for( w = 0; w < width; ++w ) 
	{
		target[ h + width * w ] = src[ h * width + w ];
	}

	return target;
}


/*
-----------------------------------------------------------------------------
 Function: DecodeWall -Decode then save wall data as TARGA image file.
 
 Parameters: 
			offet -[in] offset in resource file.
			length -[in] length of data.
			retval -[out] Length of next block.
			filename -[in] Name of file to save as.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void DecodeWall( W32 offset, W32 length, W32 *retval, const char *filename )
{	
	W8 *ptrResource;
	W8 *uncompr;
	W8 *buffer;
	W8 *newwall;

	ptrResource = getResourceBlock( offset, length, retval );
	if( ! ptrResource )
	{
		return;
	}

	uncompr = (PW8)MM_MALLOC( 128 * 128 );
	
	Decode_LZSS( uncompr, ptrResource, 128 * 128 );

	newwall = obverseWall( uncompr, 128, 128 );
		
	buffer = MM_MALLOC( 128 * 128 * 3 );
	ConvertPaletteToRGB( buffer, newwall, 128 * 128, macPalette );

	WriteTGA( filename, 24, 128, 128, buffer, 0, 1 );

	MM_FREE( buffer );
	MM_FREE( newwall );
    MM_FREE( uncompr );
	MM_FREE( ptrResource );
}


/*
-----------------------------------------------------------------------------
 Function: RipWalls -Extract wall image data from resource fork.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RipWalls( void )
{
	int i;
	W32 noffset = 702256;
	W32 length = 6277;
	W32 retval;
	char name[ 256 ];

	for( i = 0 ; i < 35 ; ++i )
	{
		cs_snprintf( name, sizeof( name ), "%s/%.3d.tga", DIRPATHWALLS, i );
		DecodeWall( noffset, length, &retval,  name );		
		
		noffset += length + 4;
		length = retval;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	Sprite Routines
//
///////////////////////////////////////////////////////////////////////////////

typedef struct 
{
	W16 Topy;
	W16 Boty;
	W16 Shape;

} spriteRun;



/*
-----------------------------------------------------------------------------
 Function: DecodeSprite -Decode then save sprite data as TARGA image file.
 
 Parameters: 
			offet -[in] offset in resource file.
			length -[in] length of data.
			retval -[out] Length of next block.
			filename -[in] Name of file to save as.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void DecodeSprite( W32 offset, W32 length, W32 *retval, const char *filename )
{
	W8 *ptrResource;
	W32 uncomprLength;
	W8 *uncompr;
	W8 *buffer;
	W16 *ptr;
	int i, x, width, noffset;

	ptrResource = getResourceBlock( offset, length, retval );
	if( ! ptrResource )
	{
		return;
	}

	uncomprLength = ptrResource[ 0 ];
	uncomprLength |= ptrResource[ 1 ] << 8;

	uncompr = (PW8)MM_MALLOC( uncomprLength );
	Decode_LZSS( uncompr, ptrResource+2, uncomprLength );

		
	buffer = (W8 *)MM_MALLOC(128 * 128 * 4);
	memset( buffer, 0, 128 * 128 * 4 );
	

	ptr = (PW16)uncompr;

	width = BigShort( ptr[ 0 ] );
	
	noffset = 64 - width / 2;
	for( x = 0 ; x < width ; ++x ) 
	{
		spriteRun *p = (spriteRun *)&ptr[ BigShort( ptr[ x + 1 ] ) / 2 ];
		
		while( p->Topy != 0xFFFF ) 
		{
			for( i = BigShort( p->Topy ) / 2; i < BigShort( p->Boty ) / 2; ++i ) 
			{
				*(buffer + (i * 128 + x + noffset) * 4 + 0) = macPalette[ uncompr[ BigShort( p->Shape ) + BigShort( p->Topy ) / 2 + (i - BigShort( p->Topy ) / 2) ] * 3 + 0 ];
				*(buffer + (i * 128 + x + noffset) * 4 + 1) = macPalette[ uncompr[ BigShort( p->Shape ) + BigShort( p->Topy ) / 2 + (i - BigShort( p->Topy ) / 2) ] * 3 + 1 ];
				*(buffer + (i * 128 + x + noffset) * 4 + 2) = macPalette[ uncompr[ BigShort( p->Shape ) + BigShort( p->Topy ) / 2 + (i - BigShort( p->Topy ) / 2) ] * 3 + 2 ];
				*(buffer + (i * 128 + x + noffset) * 4 + 3) = 255;
			}
			p++;
		}
	}


	WriteTGA( filename, 32, 128, 128, buffer, 0, 1 );

	MM_FREE( buffer );
    MM_FREE( uncompr );
	MM_FREE( ptrResource );
}


/*
-----------------------------------------------------------------------------
 Function: RipSprites -Extract sprites from resource fork.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RipSprites( void )
{
	int i;
	W32 offset = 106345;
	W32 length = 524;
	W32 retval;
	char name[ 256 ];

	for( i = 0 ; i < 163 ; ++i )
	{
		cs_snprintf( name, sizeof( name ), "%s/%.3d.tga", DIRPATHSPRITES, i );
		DecodeSprite( offset, length, &retval, name );		
		offset += length + 4;
		length = retval;
	}


	offset = 579104;
	length = 868;
	for( i = 163 ; i < 171 ; ++i )
	{
		cs_snprintf( name, sizeof( name ), "%s/%.3d.tga", DIRPATHSPRITES, i );
		DecodeSprite( offset, length, &retval, name );		
		offset += length + 4;
		length = retval;
	}

	offset = 2356530;
	length = 7153;
	for( i = 171 ; i < 175 ; ++i )
	{
		cs_snprintf( name, sizeof( name ), "%s/%.3d.tga", DIRPATHSPRITES, i );
		DecodeSprite( offset, length, &retval, name );		
		offset += length + 4;
		length = retval;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	Item Routines
//
///////////////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: DecodeItem -Convert item data into RGB raw data. 
 
 Parameters: data -[in] data chunk to decode.
			pal -[in] palette.
 
 Returns: On success pointer to RGB data, otherwise NULL.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE W8 *DecodeItem( W8 *data, W8 *pal )
{
	W8 *buffer, *mask, *ptr;
	int x, y, w, h;
	
	buffer = (W8 *)MM_MALLOC( 128 * 128 * 4 );
	
	memset( buffer, 0, 128 * 128 * 4 );
	
	x = data[ 0 ] << 8 | data[ 1 ];
	y = data[ 2 ] << 8 | data[ 3 ];
	w = data[ 4 ] << 8 | data[ 5 ];
	h = data[ 6 ] << 8 | data[ 7 ];
	
	data += 8;
	mask = data + w * h;
	ptr = buffer + 512 * y + x * 4;
	
	do 
	{
		int w2 = w;
		do 
		{
			if( *mask == 0 )
			{
				ptr[ 0 ] = pal[ data[ 0 ] * 3 ];
				ptr[ 1 ] = pal[ data[ 0 ] * 3 + 1 ];
				ptr[ 2 ] = pal[ data[ 0 ] * 3 + 2 ];
				ptr[ 3 ] = 255;
			}
			ptr += 4;
			data++;
			mask++;

		} while( --w2 );

		ptr += 4 * (128 - w);

	} while( --h );
	
	return buffer;
}


/*
-----------------------------------------------------------------------------
 Function: RipItems -Extract pic images from resource file.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RipItems( void )
{
	W8 *ptrResource;
	W8 *ptrDst;
	W32 *ptrLong;
	W8 *buffer;
	W8 **gameItems;
	W32 junk;
	W32 uncomprLength;
	int i;
	char name[ 256 ];
	W16 *ptrShape1;
	W8 *ptrShape2;
	W16 width, height;
	W32 offset = 634799L;
	W32 length = 67453L;


	ptrResource = (PW8)getResourceBlock( offset, length, &junk );

	uncomprLength = BigLong( *((PW32)ptrResource) );
	gameItems = (W8 **)MM_MALLOC( uncomprLength );
	
	Decode_LZSS( (PW8)gameItems, ptrResource+4, uncomprLength );

	MM_FREE( ptrResource );

	ptrLong = (PW32)gameItems;
	ptrDst = (PW8)gameItems;
	for( i = 0; i < 47; ++i ) 
	{
		uncomprLength = BigLong( ptrLong[ i ] );
		gameItems[ i ] = ptrDst + uncomprLength;
	}
	
	for( i = 0 ; i < 6 * 4 ; ++i )
	{
		cs_snprintf( name, sizeof( name ), "%s/weapon%.2d.tga", DIRPATHPICS, i );
		buffer = DecodeItem( gameItems[ 12 + i ], macPalette );
		WriteTGA( name, 32, 128, 128, buffer, 0, 1 );

		MM_FREE( buffer );
	}

	for( i = 0 ; i < 47 ; ++i )
	{
		ptrShape1 = (PW16)gameItems[ i ];
		width = BigShort( ptrShape1[ 0 ] );	
		height = BigShort( ptrShape1[ 1 ] );	
		ptrShape2 = (PW8)&ptrShape1[ 2 ];

		buffer = MM_MALLOC( width * height * 3 );

		ConvertPaletteToRGB( buffer, ptrShape2, width * height, macPalette );

		cs_snprintf( name, sizeof( name ), "%s/%.2d.tga", DIRPATHPICS, i );
		WriteTGA( name, 24, width, height, buffer, 0, 1 );

		MM_FREE( buffer );

		// Skip over weapon frames
		if( 11 == i )
		{
			i = 35;
		}
	}
	
	MM_FREE( gameItems );
}

///////////////////////////////////////////////////////////////////////////////
//
//	Midi Routines
//
///////////////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: RipMidi -Extract midi files from resource fork. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void RipMidi( void )
{
	W8 *ptrResource;
	W32 retval;
	FILE *fhandle;
	char name[ 256 ];
	int i;
	W32 offset = 1899536L;
	W32 length = 8215L;

	for( i = 0 ; i < 8 ; ++i )
	{
		ptrResource = getResourceBlock( offset, length, &retval );
		if( ! ptrResource )
		{
			continue;
		}

		cs_snprintf( name, sizeof( name ), "%s/%d.mid", DIRPATHMIDI, i );
		fhandle = fopen( name, "wb" );
		if( ! fhandle )
		{
			continue;
		}

		fwrite( ptrResource, 1, length, fhandle );

		fclose( fhandle );
		MM_FREE( ptrResource );

		offset += length + 4;
		length = retval;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	PICT image
//
///////////////////////////////////////////////////////////////////////////////

#if 0

PRIVATE void DecodePICTimage( W32 offset, W32 length )
{
	FILE *hfOut;
	W8 *ptrResource;
	W32 junk;
	int i;

	ptrResource = getResourceBlock( offset, length, &junk );
	if( ! ptrResource )
	{
		return;
	}

	hfOut = fopen( "pict.pct", "wb" );
	if( ! hfOut )
	{
		return;
	}

	for( i ; i < 512 ; ++i )
	{
		fwrite( ptrResource, 1, 0, hfOut );
	}

	fwrite( ptrResource, 1, length, hfOut );

	fclose( hfOut );
}

#endif

///////////////////////////////////////////////////////////////////////////////
//
//	Parse MacBinary Header 
//
///////////////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: parseMacBinaryHead -Parse mac binary header.
 
 Parameters: Nothing.
 
 Returns: true if this is Wolfenstein Mac binary file, otherwise false.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE _boolean parseMacBinaryHead( void )
{
	W32 temp32 = 0;
	char name[ 64 ];

//
// Check file name
//
	fseek( fResHandle, 1, SEEK_SET );

	// get file name length (range is 1 to 31)
	fread( &temp32, 1, 1, fResHandle );
	if( temp32 < 1 || temp32 > 31 )
	{
		return false;
	}	
	
	fread( name, 1, temp32, fResHandle );
	name[ temp32 - 1 ] = '\0';
	if( strcmp( name, MACBINFILENAME ) != 0 )
	{
		return false;
	}

//
//	Check file type / creator
//
	fseek( fResHandle, 65, SEEK_SET );

	fread( &name, 1, 8, fResHandle );
	name[ 8 ] = '\0';
	if( strcmp( name, FILETYPECREATOR ) != 0 )
	{
		return false;
	}	

//
//	Check Data Fork length
//
	fseek( fResHandle, 83, SEEK_SET );

	fread( &temp32, 1, 4, fResHandle );

	temp32 = BigLong( temp32 );
	if( temp32 != DATAFORKLENGTH )
	{
		return false;
	}

//
//	Check Resource Fork length
//
	fread( &temp32, 1, 4, fResHandle );

	temp32 = BigLong( temp32 );
	if( temp32 != RESFORKLENGTH )
	{
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//
//	Interface
//
///////////////////////////////////////////////////////////////////////////////

/*
-----------------------------------------------------------------------------
 Function: ripMac -Interface to Macintosh data extractor.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void ripMac( void )
{
	char *fname;
	char ext[ 256 ];

	cs_strlcpy( ext, MAC_FEXT, sizeof( ext ) );

    	
	fname = FS_FindFirst( ext, 0, FA_DIR );
	FS_FindClose();

	if( fname == NULL )
	{
		// try again with lower case
		fname = FS_FindFirst( cs_strlwr( ext ), 0, FA_DIR );
		FS_FindClose();

		if( fname == NULL )
		{
			printf( "Could not find any mac files for read\n" );

			return;
		}
	}

	fResHandle = fopen( fname, "rb" );
	if( fResHandle == NULL )
	{
		printf( "Could not open file (%s) for read\n", fname );

		return;
	}

	if( ! parseMacBinaryHead() )
	{
		printf( "Unknown MacBinary file\n" );

		fclose( fResHandle );
		
		return;
	}



	FS_Mkdir( DIRPATHPICS );
	FS_Mkdir( DIRPATHSPRITES );
	FS_Mkdir( DIRPATHWALLS );
	FS_Mkdir( DIRPATHMIDI );


	printf( "Extracting Midi data...\n" );
	RipMidi();

	printf( "Decoding Screen data...\n" );
	RipScreens();
	

	setPalette( GAMEPAL );

	printf( "Decoding Wall data...\n" );
	RipWalls();
	printf( "Decoding Sprite data...\n" );
	RipSprites();
	printf( "Decoding Item data...\n" );
	RipItems();	

	fclose( fResHandle );
}
