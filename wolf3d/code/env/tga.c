/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	tga.h:   Handle Targa file format.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	Portion of this code was derived from The GIMP -- an image manipulation
 *	program, and was originally written by Spencer Kimball and Peter Mattis.
 *
 */

/*
	Notes:

*/

#include "../wolfiphone.h"


#define TGA_HEADER_SIZE		18

#ifndef IPHONE 
PRIVATE W8 *p_buf;	// current pointer to tga data block
#endif


 /* TRUEVISION-XFILE magic signature string */
static W8 magic[ 18 ] =
{
  0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 0x4f,
  0x4e, 0x2d, 0x58, 0x46, 0x49, 0x4c, 0x45, 0x2e, 0x0
};


typedef struct _TargaHeader 
{
	W8 	idLength;
	W8	colorMapType;

	W8	imageType;
/* Known image types. */
#define TGA_TYPE_MAPPED      1
#define TGA_TYPE_COLOR       2
#define TGA_TYPE_GRAY        3

	W8 imageCompression;
/* Only known compression is RLE */
#define TGA_COMP_NONE        0
#define TGA_COMP_RLE         1

/* Color Map Specification. */
	W16	colorMapIndex;
	W16 colorMapLength;
	W8	colorMapSize;

/* Image Specification. */
	W16	xOrigin;
	W16 yOrigin;

	W16 width;
	W16 height;

	W8 bpp;
	W8 bytes;

	W8 alphaBits;
	W8 flipHoriz;
	W8 flipVert;

} TargaHeader;



PRIVATE void flip_line( W8 *buffer, TargaHeader *info )
{
	W8  temp;
	W8 *alt;
	SW32    x, s;

	alt = buffer + (info->bytes * (info->width - 1));

	for( x = 0; x * 2 <= info->width; ++x )
	{
		for( s = 0; s < info->bytes; ++s )
		{
			temp = buffer[ s ];
			buffer[ s ] = alt[ s ];
			alt[ s ] = temp;
		}

		buffer += info->bytes;
		alt -= info->bytes;
	}
}

PRIVATE void upsample( W8 *dest, W8 *src,
					W32 width, W32 bytes, W8 alphaBits )
{
	W32 x;

	for( x = 0 ; x < width ; ++x )
	{
		dest[0] =  ((src[1] << 1) & 0xf8);
		dest[0] += (dest[0] >> 5);

		dest[1] =  ((src[0] & 0xe0) >> 2) + ((src[1] & 0x03) << 6);
		dest[1] += (dest[1] >> 5);

		dest[2] =  ((src[0] << 3) & 0xf8);
		dest[2] += (dest[2] >> 5);

		switch( alphaBits )
		{
			case 1:
				dest[ 3 ] = (src[ 1 ] & 0x80) ? 0 : 255;
				dest += 4;
				break;

			default:
				dest += 3;
		}

		src += bytes;
	}
}

PRIVATE void bgr2rgb( W8 *dest, W8 *src,
						W32 width, W32 bytes, W32 alpha )
{
	W32 x;

	if( alpha )
	{
		for( x = 0 ; x < width ; ++x )
		{
			*(dest++) = src[2];
			*(dest++) = src[1];
			*(dest++) = src[0];

			*(dest++) = src[3];

			src += bytes;
		}
	}
	else
	{
		for( x = 0 ; x < width ; ++x )
		{
			*(dest++) = src[2];
			*(dest++) = src[1];
			*(dest++) = src[0];

			src += bytes;
		}
	}

    
}

PRIVATE SW32 rle_read(	filehandle_t *fp, W8 *buffer,
						TargaHeader *info )
{
	static SW32   repeat = 0;
	static SW32   direct = 0;
	static W8 sample[ 4 ];
	SW32 head;
	W8	temphead;
	SW32 x, k;

	for( x = 0; x < info->width; ++x )
	{
		if( repeat == 0 && direct == 0 )
		{
			FS_ReadFile( &temphead, 1, 1, fp );
			head = temphead;

			if( head >= 128 )
			{
				repeat = head - 127;

				if( FS_ReadFile( sample, info->bytes, 1, fp ) < 1 )
				{
					return EOF;
				}
			}
			else
			{
				direct = head + 1;
			}
		}

		if( repeat > 0 )
		{
			for( k = 0 ; k < info->bytes ; ++k )
			{
				buffer[ k ] = sample[ k ];
			}

			repeat--;
		}
		else /* direct > 0 */
		{
			if( FS_ReadFile( buffer, info->bytes, 1, fp ) < 1 )
			{
				return EOF;
			}

			direct--;
		}

		buffer += info->bytes;
	}

	return 0;
}


PRIVATE void read_line( filehandle_t	*fp,
						W8		*row,
						W8		*buffer,
						TargaHeader     *info )
{
	if( info->imageCompression == TGA_COMP_RLE )
	{
		if( rle_read( fp, buffer, info ) == EOF )
		{
			return;
		}
	}
	else
	{
		FS_ReadFile( buffer, info->bytes, info->width, fp );
	}

	if( info->flipHoriz )
	{
		flip_line( buffer, info );
	}

	if( info->imageType == TGA_TYPE_COLOR )
	{
		if( info->bpp == 16 || info->bpp == 15 )
		{
			upsample( row, buffer, info->width, info->bytes, info->alphaBits );
		}
		else
		{
			bgr2rgb( row, buffer, info->width, info->bytes, info->bytes == 4 ? 1 : 0 );
		}
	}
	else
	{
		memcpy( row, buffer, info->width * info->bpp );
	}
}



PUBLIC void LoadTGA( const char *filename, W8 **pic, W16 *width, W16 *height, W16 *bytes )
{
	TargaHeader		targa_header;
	W8    header[ 18 ];
	W8    footer[ 26 ];
	W8    extension[ 495 ];
	W32  cmap_bytes;
	SW32 offset;
	W8 tga_cmap[4 * 256], gimp_cmap[3 * 256];
	W8 *buffer, *data, *row;
	int i;
	SW32 datalength;
	filehandle_t *hFile;



	*pic = NULL;

//
// Load the file
//
	hFile = FS_OpenFile( filename, 0 );
	if( ! hFile )
    {
		Com_DPrintf( "Could not open (%s) for reading\n", filename );

		goto TGALOADFAILED;
    }

	datalength = FS_GetFileSize( hFile );

	if( ! FS_FileSeek( hFile, -26L, SEEK_END ) ) 
	{ 
		/* Is file big enough for a footer? */
		if( FS_ReadFile( footer, sizeof( footer ), 1, hFile ) != 1 )
		{
			Com_DPrintf( "Cannot read footer from (%s)\n" , filename );
			
			goto TGALOADFAILED;
		}
		else if( memcmp( footer + 8, magic, sizeof( magic ) ) == 0 )
		{
			/* Check the signature. */
			offset = footer[ 0 ] + (footer[ 1 ] * 256) + (footer[ 2 ] * 65536)
                          + (footer[ 3 ] * 16777216);

			if( offset != 0 )
			{
				if( FS_FileSeek( hFile, offset, SEEK_SET ) ||
					FS_ReadFile( extension, sizeof( extension ), 1, hFile ) != 1 )
				{
					Com_DPrintf( "Cannot read extension from '%s'\n", filename );
					
					goto TGALOADFAILED;
				}

				/* Eventually actually handle version 2 TGA here */

			}
		}

	}


//
// Get header information.
//
	if( datalength < TGA_HEADER_SIZE )
	{
		Com_Printf( "Could not read header from (%s)\n", filename );

		goto TGALOADFAILED;
	}

	if( FS_FileSeek( hFile, 0, SEEK_SET ) ||
		FS_ReadFile( header, sizeof( header ), 1, hFile ) != 1 )
	{
		Com_Printf( "Cannot read header from (%s)\n", filename );

		goto TGALOADFAILED;
	}



	targa_header.idLength = header[ 0 ];
	targa_header.colorMapType = header[ 1 ];

	switch( header[ 2 ] )
    {
		case 1:
		  targa_header.imageType = TGA_TYPE_MAPPED;
		  targa_header.imageCompression = TGA_COMP_NONE;
		  break;

		case 2:
		  targa_header.imageType = TGA_TYPE_COLOR;
		  targa_header.imageCompression = TGA_COMP_NONE;
		  break;

		case 3:
		  targa_header.imageType = TGA_TYPE_GRAY;
		  targa_header.imageCompression = TGA_COMP_NONE;
		  break;



		case 9:
		  targa_header.imageType = TGA_TYPE_MAPPED;
		  targa_header.imageCompression = TGA_COMP_RLE;
		  break;

		case 10:
		  targa_header.imageType = TGA_TYPE_COLOR;
		  targa_header.imageCompression = TGA_COMP_RLE;
		  break;

		case 11:
		  targa_header.imageType = TGA_TYPE_GRAY;
		  targa_header.imageCompression = TGA_COMP_RLE;
		  break;


		default:
		  targa_header.imageType = 0;
    }
	
	targa_header.colorMapIndex  = header[ 3 ] + header[ 4 ] * 256;
	targa_header.colorMapLength = header[ 5 ] + header[ 6 ] * 256;
	targa_header.colorMapSize   = header[ 7 ];

	targa_header.xOrigin = header[ 8  ] + header[ 9  ] * 256;
	targa_header.yOrigin = header[ 10 ] + header[ 11 ] * 256;
	targa_header.width   = header[ 12 ] + header[ 13 ] * 256;
	targa_header.height  = header[ 14 ] + header[ 15 ] * 256;

	targa_header.bpp       = header[ 16 ];
	targa_header.bytes     = (targa_header.bpp + 7) / 8;
	targa_header.alphaBits = header[ 17 ] & 0x0f; /* Just the low 4 bits */
	targa_header.flipHoriz = (header[ 17 ] & 0x10) ? 1 : 0;
	targa_header.flipVert  = (header[ 17 ] & 0x20) ? 0 : 1;

//
// Analyze header information.
//
	switch( targa_header.imageType )
	{
		case TGA_TYPE_MAPPED:
			if( targa_header.bpp != 8 )
			{
				Com_DPrintf( "Unhandled sub-format in (%s)\n", filename );

				goto TGALOADFAILED;
			}

			goto TGALOADFAILED;

			break;

		case TGA_TYPE_COLOR:
			if( targa_header.bpp != 15 && targa_header.bpp != 16 && targa_header.bpp != 24
					 && targa_header.bpp != 32 )
			{
				Com_DPrintf( "Unhandled sub-format in (%s)\n", filename );
				goto TGALOADFAILED;
			}
			break;

		case TGA_TYPE_GRAY:
			if( targa_header.bpp != 8 && (targa_header.alphaBits != 8 || (targa_header.bpp != 16 && targa_header.bpp != 15 )))
			{
				Com_DPrintf( "Unhandled sub-format in (%s)\n", filename );
				goto TGALOADFAILED;
			}


			goto TGALOADFAILED;


			break;

		default:
			Com_DPrintf( "Unknown image type for (%s)\n", filename );
			goto TGALOADFAILED;

    } /* end of switch targa_header.imageType */

	/* Plausible but unhandled formats */
	if( targa_header.bytes * 8 != targa_header.bpp && ! (targa_header.bytes == 2 && targa_header.bpp == 15) )
    {
		Com_DPrintf( "No support yet for TGA with these parameters\n" );

		goto TGALOADFAILED;
    }

	/* Check that we have a color map only when we need it. */
	if( targa_header.imageType == TGA_TYPE_MAPPED && targa_header.colorMapType != 1 )
    {
		Com_DPrintf( "Indexed image has invalid color map type %d\n",
					targa_header.colorMapType );

		goto TGALOADFAILED;
    }
	else if( targa_header.imageType != TGA_TYPE_MAPPED && targa_header.colorMapType != 0 )
    {
		Com_DPrintf( "Non-indexed image has invalid color map type %d\n",
					targa_header.colorMapType );

		goto TGALOADFAILED;
    }

	/* Skip the image ID field. */
	if( targa_header.idLength && FS_FileSeek( hFile, targa_header.idLength, SEEK_CUR ) )
    {
		Com_DPrintf( "File (%s) is truncated or corrupted\n", filename );
		
		goto TGALOADFAILED;
    }



	/* Handle colormap */
	if( targa_header.colorMapType == 1 )
    {
		cmap_bytes = (targa_header.colorMapSize + 7 ) / 8;
		if( cmap_bytes <= 4 &&
			FS_ReadFile( tga_cmap, targa_header.colorMapLength * cmap_bytes, 1, hFile ) == 1 )
		{
			if( targa_header.colorMapSize == 32 )
			{
				bgr2rgb( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, 1);
			}
			else if( targa_header.colorMapSize == 24 )
			{
				bgr2rgb( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, 0);
			}
			else if( targa_header.colorMapSize == 16 || targa_header.colorMapSize == 15 )
			{
				upsample( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, targa_header.alphaBits);
			}

		}
		else
		{
			Com_DPrintf( "File (%s) is truncated or corrupted\n", filename );
			
			goto TGALOADFAILED;
		}
    }


	/* Allocate the data. */
	data = MM_MALLOC( targa_header.width * targa_header.height * targa_header.bytes );
	if( data == NULL )
	{
		MM_OUTOFMEM( "data" );
	}

	buffer = (PW8) MM_MALLOC( targa_header.width * targa_header.bytes );
	if( buffer == NULL )
	{
		MM_FREE( data );
		MM_OUTOFMEM( "buffer" );
	}
		
	if( targa_header.flipVert )
	{
		for( i = targa_header.height-1 ; i >= 0 ; --i )
		{
			row = data + (targa_header.width * targa_header.bytes * i);
			read_line( hFile, row, buffer, &targa_header );
		}
	}
	else
	{
		for( i = 0 ; i < targa_header.height ; ++i )
		{
			row = data + (targa_header.width * targa_header.bytes * i);
			read_line( hFile, row, buffer, &targa_header );
		}
	}

	
	MM_FREE( buffer );

	FS_CloseFile( hFile );

	*pic = data;
	*width = targa_header.width;
	*height = targa_header.height;
	*bytes = targa_header.bytes;

	return;



TGALOADFAILED:
	*pic = NULL;
	*width = 0;
	*height = 0;
	*bytes = 0;

	if( hFile )
	{
		FS_CloseFile( hFile );
	}

}

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
			 bpp -[in] Bits per pixel. (16, 24 or 32).
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
		Com_DPrintf( "Could not open file (%s) for write!\n", filename );
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
			for( x = 0; x < (width * BytesPerPixel); x += BytesPerPixel )
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
