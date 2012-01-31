/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 1997-2001 Id Software, Inc.
	Copyright (C) 1995 Spencer Kimball and Peter Mattis

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
 *	tga.c:   Handle Targa file format.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was 
 *	originally written by id Software, Inc.
 *
 *	Portion of this code was derived from The GIMP -- an image manipulation
 *	program, and was originally written by Spencer Kimball and Peter Mattis.
 */

#include <string.h>
#include <stdio.h>

#include "../../../common/arch.h"
#include "../memory/memory.h"
#include "../../../common/common_utils.h"


/*
-----------------------------------------------------------------------------
 Function: rle_write -Run length encode scanline.
 
 Parameters: fp -[in] Pointer to valid FILE structure.
			 buffer -[in] Scanline data.
		     width -[in] Image scanline width.
			 bytes -[in] Bytes per pixel.			 
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void rle_write( FILE   *fp,
						W8	*buffer,
						W32	width,
						W32	bytes )
{
	SW32    repeat = 0;
	SW32    direct = 0;
	W8	 *from   = buffer;
	W32    x;

	for( x = 1 ; x < width ; ++x )
	{
		if( memcmp( buffer, buffer + bytes, bytes ) )
		{
			/* next pixel is different */
			if( repeat )
			{
				putc( 128 + repeat, fp );
				fwrite( from, bytes, 1, fp );
				from = buffer + bytes; /* point to first different pixel */
				repeat = 0;
				direct = 0;
			}
			else
			{
				direct += 1;
			}
		}
		else
		{
			/* next pixel is the same */
			if( direct )
			{
				putc( direct - 1, fp );
				fwrite( from, bytes, direct, fp );
				from = buffer; /* point to first identical pixel */
				direct = 0;
				repeat = 1;
			}
			else
			{
				repeat += 1;
			}
		}

		if( repeat == 128 )
		{
			putc( 255, fp );
			fwrite( from, bytes, 1, fp );
			from = buffer + bytes;
			direct = 0;
			repeat = 0;
		}
		else if( direct == 128 )
		{
			putc( 127, fp );
			fwrite( from, bytes, direct, fp );
			from = buffer + bytes;
			direct = 0;
			repeat = 0;
		}

		buffer += bytes;
    }

	if( repeat > 0 )
	{
		putc( 128 + repeat, fp );
		fwrite( from, bytes, 1, fp );
	}
	else
	{
		putc( direct, fp );
		fwrite( from, bytes, direct + 1, fp );
	}
}


/*
-----------------------------------------------------------------------------
 Function: WriteTGA -Write targa image file.
 
 Parameters: filename -[in] Name of TGA file to save as.
			 depth -[in] Bytes per pixel. (16, 24 or 32).
		     width -[in] Width of image.
			 height -[in] Height of image.
			 Data -[in] Raw image data.
			 upsideDown -[in] Is the data upside down? 1 yes, 0 no.
			 rle -[in] Run Length encode? 1 yes, 0 no.
 
 Returns: 0 on error, otherwise 1.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC W8 WriteTGA( const char *filename, W16 bpp, W16 width, W16 height, 
            void *Data, W8 upsideDown, W8 rle )
{
    W16	i, x, y, BytesPerPixel;
	W8	*scanline;
	W8 header[ 18 ];
	FILE *filestream;
	W8 *ptr = (PW8) Data;
	W8 temp;

	BytesPerPixel = bpp >> 3;

	filestream = fopen( filename, "wb" );   
    if( filestream == NULL )
	{
		printf( "Could not open file (%s) for write!\n", filename );
		return 0;
	}
	
	memset( header, 0, 18 );    
    header[2] = rle ? 10 : 2;

    header[12] = width & 255;	// width low
    header[13] = width >> 8;	// width high 

    header[14] = height & 255;	// height low
    header[15] = height >> 8;	// height high

    header[16] = bpp & 255;	// pixel size

    if( upsideDown )
	{
		header[17] |= 1 << 5; // Image Descriptor
    }
    

	fwrite( header, sizeof( W8 ), sizeof( header ), filestream  );	



	scanline = (PW8) MM_MALLOC( width * BytesPerPixel );	
    if( scanline == NULL )
	{
		fclose( filestream );		
		return 0;
	}

	for( y = 0; y < height; ++y )
	{		
		W32 k = 0;

		for( i = 0; i < (width * BytesPerPixel); ++i )
		{
			scanline[ k++ ] = ptr[ (height - y - 1) * width * BytesPerPixel + i ];
		}

	
		
		if( bpp == 24 || bpp == 32 )
		{
			// swap rgb to bgr
			for( x = 0 ; x < (width * BytesPerPixel) ; x += BytesPerPixel )
			{
				temp = scanline[ x ];
				scanline[ x ] = scanline[ x + 2 ];
				scanline[ x + 2 ] = temp;
			}
		}

		
		if( rle )
		{
			rle_write( filestream, scanline, width, BytesPerPixel );
		}
		else
		{
			fwrite( scanline, sizeof( W8 ), width * BytesPerPixel, filestream );
		}
	}
	
    MM_FREE( scanline );

    fclose( filestream );
	
	return 1;
}

//=======================================================================

typedef struct TargaHeader_s {
	unsigned char 	id_length;
	unsigned char	colormap_type;
	unsigned char	image_type;
	unsigned short	colormap_index;
	unsigned short	colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin;
	unsigned short	y_origin;
	unsigned short	width, height;
	unsigned char	pixel_size;
	unsigned char	attributes;
} TargaHeaeder_t;

static const int TGA_HEADER_SIZE = 18;

/*
========================
LoadTGAFromBuffer

Load a TGA from a buffer containing a TGA file.
========================
*/
int LoadTGAFromBuffer( const char *name, const W8 *buffer, const int bufferSize, 
			    W8 **pic, int *width, int *height ) {
	int			columns, rows, numPixels;
	size_t		numBytes;
	W8			*pixbuf;
	int			row, column;
	const W8	*buf_p;
	struct TargaHeader_s	targa_header;
	W8		*targa_rgba;

	*pic = NULL;

	buf_p = buffer;

	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;
	
	targa_header.colormap_index = *(short *)buf_p;
	buf_p += 2;
	targa_header.colormap_length = *(short *)buf_p;
	buf_p += 2;
	targa_header.colormap_size = *buf_p++;
	targa_header.x_origin = *(short *)buf_p;
	buf_p += 2;
	targa_header.y_origin = *(short *)buf_p;
	buf_p += 2;
	targa_header.width = *(short *)buf_p;
	buf_p += 2;
	targa_header.height = *(short *)buf_p;
	buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;

	if ( targa_header.image_type != 2 && targa_header.image_type != 10 && targa_header.image_type != 3 ) {
		printf( "LoadTGA( %s ): Only type 2 (RGB), 3 (gray), and 10 (RGB) TGA images supported", name );
		return false;
	}

	if ( targa_header.colormap_type != 0 ) {
		printf( "LoadTGA( %s ): colormaps not supported", name );
		return false;
	}

	if ( ( targa_header.pixel_size != 32 && targa_header.pixel_size != 24 ) && targa_header.image_type != 3 ) {
		printf( "LoadTGA( %s ): Only 32 or 24 bit images supported (no colormaps)", name );
		return false;
	}

	if ( targa_header.image_type == 2 || targa_header.image_type == 3 ) {
		numBytes = targa_header.width * targa_header.height * ( targa_header.pixel_size >> 3 );
		if ( numBytes > bufferSize - TGA_HEADER_SIZE - targa_header.id_length ) {
			printf( "LoadTGA( %s ): incomplete file", name );
			return false;
		}
	}

	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;

	if ( width ) {
		*width = columns;
	}
	if ( height ) {
		*height = rows;
	}

	targa_rgba = (W8 *)malloc( numPixels*4 );
	*pic = targa_rgba;

	if ( targa_header.id_length != 0 ) {
		buf_p += targa_header.id_length;  // skip TARGA image comment
	}
	
	if ( targa_header.image_type == 2 || targa_header.image_type == 3 ) { 
		unsigned char red,green,blue,alphabyte;
		switch( targa_header.pixel_size ) {
		case 8:
			// Uncompressed gray scale image
			for( row = rows - 1; row >= 0; row-- ) {
				pixbuf = targa_rgba + row*columns*4;
				for( column = 0; column < columns; column++ ) {
					blue = *buf_p++;
					green = blue;
					red = blue;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = 255;
				}
			}
			break;
		case 24:
			// Uncompressed RGB image
			for( row = rows - 1; row >= 0; row-- ) {
				pixbuf = targa_rgba + row*columns*4;
				for( column = 0; column < columns; column++ ) {
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = 255;
				}
			}
			break;
		case 32:
			// Uncompressed RGBA image
			for( row = rows - 1; row >= 0; row-- ) {
				pixbuf = targa_rgba + row*columns*4;
				for( column = 0; column < columns; column++ ) {
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alphabyte = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = alphabyte;
				}
			}
			break;
		default:
			printf( "LoadTGA( %s ): illegal pixel_size '%d'", name, targa_header.pixel_size );
			free( *pic );
			*pic = NULL;
			return false;
		}
	}
	else if ( targa_header.image_type == 10 ) {   // Runlength encoded RGB images
		unsigned char red,green,blue,alphabyte,packetHeader,packetSize,j;

		red = 0;
		green = 0;
		blue = 0;
		alphabyte = 0xff;

		for( row = rows - 1; row >= 0; row-- ) {
			pixbuf = targa_rgba + row*columns*4;
			for( column = 0; column < columns; ) {
				packetHeader= *buf_p++;
				packetSize = 1 + (packetHeader & 0x7f);
				if ( packetHeader & 0x80 ) {        // run-length packet
					switch( targa_header.pixel_size ) {
						case 24:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = 255;
								break;
						case 32:
								blue = *buf_p++;
								green = *buf_p++;
								red = *buf_p++;
								alphabyte = *buf_p++;
								break;
						default:
							printf( "LoadTGA( %s ): illegal pixel_size '%d'", name, targa_header.pixel_size );
							free( *pic );
							*pic = NULL;
							return false;
					}
	
					for( j = 0; j < packetSize; j++ ) {
						*pixbuf++=red;
						*pixbuf++=green;
						*pixbuf++=blue;
						*pixbuf++=alphabyte;
						column++;
						if ( column == columns ) { // run spans across rows
							column = 0;
							if ( row > 0) {
								row--;
							}
							else {
								goto breakOut;
							}
							pixbuf = targa_rgba + row*columns*4;
						}
					}
				} else {                            // non run-length packet
					for( j = 0; j < packetSize; j++ ) {
						switch( targa_header.pixel_size ) {
							case 24:
									blue = *buf_p++;
									green = *buf_p++;
									red = *buf_p++;
									*pixbuf++ = red;
									*pixbuf++ = green;
									*pixbuf++ = blue;
									*pixbuf++ = 255;
									break;
							case 32:
									blue = *buf_p++;
									green = *buf_p++;
									red = *buf_p++;
									alphabyte = *buf_p++;
									*pixbuf++ = red;
									*pixbuf++ = green;
									*pixbuf++ = blue;
									*pixbuf++ = alphabyte;
									break;
							default:
								printf( "LoadTGA( %s ): illegal pixel_size '%d'", name, targa_header.pixel_size );
								free( *pic );
								*pic = NULL;
								return false;
						}
						column++;
						if ( column == columns ) { // pixel packet run spans across rows
							column = 0;
							if ( row > 0 ) {
								row--;
							}
							else {
								goto breakOut;
							}
							pixbuf = targa_rgba + row*columns*4;
						}						
					}
				}
			}
			breakOut: ;
		}
	}

	if ( (targa_header.attributes & (1<<5)) ) {			// image flp bit
//		R_VerticalFlip( *pic, *width, *height );
	}

	return true;
}

/*
========================
LoadTGA

Load TGA directly from a file.
========================
*/
int LoadTGA( const char *name, W8 **pic, int *width, int *height ) {
	FILE	*f = fopen( name, "rb" );
	int		len;
	W8		*buf;
	int		ret;

	if ( !f ) {
		return 0;
	}

	fseek( f, 0, SEEK_END );
	len = ftell( f );
	fseek( f, 0, SEEK_SET );
	buf = malloc( len );
	fread( buf, 1, len, f );
	fclose( f );

	ret = LoadTGAFromBuffer( name, buf, len, pic, width, height );

	free( buf );
	
	return ret;
}

