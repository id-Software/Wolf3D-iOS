/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>

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
 *	wolf_pm.c:	Decode Wolfenstein3-D Page data.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wolf_def.h"

#include "../../../common/arch.h"
#include "../../../common/common_utils.h"
#include "../loaders/tga.h"
#include "../hq2x.h"
#include "../loaders/wav.h"
#include "../filesys/file.h"
#include "../string/com_string.h"
#include "../memory/memory.h"


extern char gamepal[];

#define PAGEFNAME   "VSWAP"





#define SAMPLERATE      7000    // In Hz


typedef	struct
{
    W32  offset; // Offset of chunk into file
    W16  length; // Length of the chunk
			
} PageList_t;

typedef struct
{
	W16 leftpix, rightpix;
	W16	dataofs[ 64 ];
    // table data after dataofs[ rightpix - leftpix + 1 ]

} t_compshape;


PRIVATE PageList_t	*PMPages;

PRIVATE FILE	*PageFile = NULL;

PRIVATE W16 PMNumBlocks;
PRIVATE W16 PMSpriteStart, PMSoundStart;



/*
-----------------------------------------------------------------------------
 Function: CAL_GetGrChunkLength() -Opens the page file and sets up 
                                    the page info. 
 
 Parameters: extension -[in] Pointer to a null-terminated string that 
                            specifies the file extension.
                            (must be in '.XXX' format) 
 
 Returns: 1 on success, otherwise 0.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE W8 PML_OpenPageFile( const char *extension )
{
	W32    i;	
	W32   size;
	void   *buf;
	char filename[ 16 ];
	W32  *offsetptr;
	W16 *lengthptr;
	PageList_t *page;
	
	if( ! extension || ! *extension )
	{
	   printf( "Invalid file extension passed to PML_OpenPageFile()\n" );
	   return 0;
    }

    cs_strlcpy( filename, PAGEFNAME, sizeof( filename ) );
    cs_strlcat( filename, extension, sizeof( filename ) );
	
	PageFile = fopen( cs_strupr( filename ), "rb" );
	if( PageFile == NULL )
	{
		PageFile = fopen( cs_strlwr( filename ), "rb" );
		if( PageFile == NULL )
		{
			printf( "Could not open file (%s) for read!\n", filename );
			return 0; 
		}
	}

	// Read in header variables
	fread( &PMNumBlocks,  sizeof( PMNumBlocks ), 1, PageFile );
	fread( &PMSpriteStart, sizeof( PMSpriteStart ), 1, PageFile );
	fread( &PMSoundStart, sizeof( PMSoundStart ), 1, PageFile );
	

	// Allocate and clear the page list
	PMPages = (PageList_t *) MM_MALLOC( sizeof( PageList_t ) * PMNumBlocks );
	if( PMPages == NULL )
	{
		return 0;
	}

	memset( PMPages, 0, sizeof( PageList_t ) * PMNumBlocks );

	// Read in the chunk offsets
	size = sizeof( W32 ) * PMNumBlocks;
	
    buf = MM_MALLOC( size );
	if( buf == NULL )
	{
		return 0;
	}

	if( fread( buf, 1, size, PageFile ) == 0 )
	{
		printf( "PML_OpenPageFile: Length read failed\n" );
	}
	offsetptr = (PW32) buf;
	for( i = 0, page = PMPages; i < PMNumBlocks; i++, page++ )
	{
		page->offset = *offsetptr++;
    }
	MM_FREE( buf );

	// Read in the chunk lengths
	size = sizeof( W16 ) * PMNumBlocks;

	buf = MM_MALLOC( size );
	if( buf == NULL )
	{
		return 0;
	}

	if( fread( buf, 1, size, PageFile ) == 0 )
	{
		printf( "PML_OpenPageFile: Length read failed\n" );
	}
	
    lengthptr = (PW16)buf;
	for( i = 0, page = PMPages; i < PMNumBlocks; ++i, page++ )
	{
		page->length = *lengthptr++;
	}
	
    MM_FREE( buf );
    
    return 1;
}

/*
-----------------------------------------------------------------------------
 Function: PML_ReadFromFile() -Reads in data from Page file. 
 
 Parameters: buf -[out] Storage location for data.
			 offset -[in] Number of bytes from beginning of file.
			 length -[in] Maximum number of items to be read.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PML_ReadFromFile( W8 *buf, SW32 offset, W16 length )
{
	if( ! buf )
	{
		printf( "[PML_ReadFromFile]: NULL pointer\n" );
		return;
	}

	if( ! offset )
	{
		printf( "[PML_ReadFromFile]: Zero offset\n" );
		return;
	}

	if( fseek( PageFile, offset, SEEK_SET ) )
	{
		printf( "[PML_ReadFromFile]: Seek failed\n" );
		return;
	}

	if( ! fread( buf, 1, length, PageFile ) )
	{
		printf( "[PML_ReadFromFile]: Read failed\n" );
		return;
	}
}

/*
-----------------------------------------------------------------------------
 Function: PML_LoadPage() -Reads in data from Page file. 
 
 Parameters: buf -[out] Storage location for data.
			 offset -[in] Number of bytes from beginning of file.
			 clength -[in] Maximum number of items to be read.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void *PML_LoadPage( W32 pagenum, W16 *clength )
{
    W8 *addr;
    PageList_t *page;

    page = &PMPages[ pagenum ];
	if( page->length == 0 )
	{
		return NULL;
	}

    *clength = page->length;

    addr = MM_MALLOC( page->length ); 
	if( addr == NULL )
	{
		return NULL;
	}

    PML_ReadFromFile( addr, page->offset, page->length );
	
    return addr;
}

void OutlineImage( W8 *rgba, int width, int height ) {
	W8 *data_p;
	W8 *copy_p;
	W8	*copy = (W8 *)_alloca( width * height * 4 );
	int		x, y;

	memcpy( copy, rgba, width * height * 4 );
	data_p = rgba;
	copy_p = copy;

	for ( y = 0 ; y < height ; y++ ) {
		for ( x = 0 ; x < width ; x++, data_p+=4, copy_p+=4 ) {
			if ( data_p[3] != 0 ) {
				continue;
			}
			if ( x < width-1 && copy_p[7] != 0 ) {
				*(int *)data_p = ((int *)copy_p)[1];
			} else if ( x > 0 && copy_p[-1] != 0 ) {
				*(int *)data_p = ((int *)copy_p)[-1];
			} else if ( y < height-1 && copy_p[width*4+3] != 0 ) {
				*(int *)data_p = ((int *)copy_p)[width];
			} else if ( y > 0 && copy_p[-width*4+3] != 0 ) {
				*(int *)data_p = ((int *)copy_p)[-width];
			}
			data_p[3] = 1;
		}
	}
}

typedef struct {
	int		hasAlpha;
	int		srcWidth;
	int		srcHeight;
	int		uploadWidth;
	int		uploadHeight;

	// track the outlines of up to two boxes of non-transparent pixels.
	// The reason for two boxes is that the common lights have something
	// at the top and something at the bottom, with nothing inbetween.
	// These are inclusive bounds of the rows / columns with non-0 alpha 
	int		numBounds;
	int		bounds[2][2][2];
} picHeader_t;

int RowClear( W8 *rgba, int w, int h, int y ) {
	int	x;
	for ( x = 0 ; x < w ; x++ ) {
		if ( rgba[(y*w+x)*4+3] != 0 ) {
			return 0;
		}
	}
	return 1;
}

void Write5551( const char *tgaName, int w, int h, W8 *rgba, int hasAlpha ) {
	unsigned short *s = _alloca( w*h*4 );
	FILE *f;
	picHeader_t	*header = (picHeader_t *)s;
	unsigned short *s_p;
	int		i;
	int		shorts;
	W8		*tempMip;
	W8		*rgba_p;
	int		b, x, y, c;
	char	newName[1024];
	char	*ext;

	memset( header, 0, sizeof( *header ) );
	header->hasAlpha = hasAlpha;
	header->srcWidth = w;
	header->srcHeight = h;
	header->uploadWidth = w;
	header->uploadHeight = h;

	if ( !hasAlpha ) {
		// convert from 24 bit to 32 bit
		W8	*newPic = _alloca( w * h * 4 );
		for ( i = 0 ; i < w*h ; i++ ) {
			newPic[i*4+0] = rgba[i*3+0];
			newPic[i*4+1] = rgba[i*3+1];
			newPic[i*4+2] = rgba[i*3+2];
			newPic[i*4+3] = 255;
		}
		rgba = newPic;
	}

	// find the bounding boxes for more efficient drawing
	header->numBounds = 1;
	for ( y = 0 ; y < h ; y++ ) {
		if ( !RowClear( rgba, w, h, y ) ) {
			// this row is needed
			header->bounds[0][0][1] = y;
			break;
		}
	}
	for ( y = h-1 ; y >= 0 ; y-- ) {
		if ( !RowClear( rgba, w, h, y ) ) {
			// this row is needed
			header->bounds[0][1][1] = y;
			break;
		}
	}

	// if the middle row is clear, make two boxes
	// We could make a better test, but this catches the ones we care about...
	if ( header->bounds[0][0][1] < h/2 && header->bounds[0][1][1] > h / 2 && RowClear( rgba, w, h, h/2 ) ) {
		header->numBounds = 2;
		header->bounds[1][1][1] = header->bounds[0][1][1];

		for ( y = h/2-1 ; y >= 0 ; y-- ) {
			if ( !RowClear( rgba, w, h, y ) ) {
				header->bounds[0][1][1] = y;
				break;
			}
		}
		for ( y = h/2+1 ; y < h ; y++ ) {
			if ( !RowClear( rgba, w, h, y ) ) {
				header->bounds[1][0][1] = y;
				break;
			}
		}
	}

	for ( b = 0 ; b < header->numBounds ; b++ ) {
		for ( x = 0 ; x < w ; x++ ) {
			for ( y = header->bounds[b][0][1] ; y <= header->bounds[b][1][1] ; y++ ) {
				if ( rgba[(y*w+x)*4+3] != 0 ) {
					// this column is needed
					header->bounds[b][0][0] = x;
					break;
				}
			}
			if ( y <= header->bounds[b][1][1] ) {
				break;
			}
		}
		for ( x = w-1 ; x >= 0 ; x-- ) {
			for ( y = header->bounds[b][0][1] ; y <= header->bounds[b][1][1] ; y++ ) {
				if ( rgba[(y*w+x)*4+3] != 0 ) {
					// this column is needed
					header->bounds[b][1][0] = x;
					break;
				}
			}
			if ( y <= header->bounds[b][1][1] ) {
				break;
			}
		}
	}

	s_p = (unsigned short *)(header+1);

	while ( 1 ) {
		rgba_p = rgba;

		// convert to 5551
		for ( i = 0 ; i < w*h ; i++, rgba_p+=4 ) {
			int	r = rgba_p[0];
			int	g = rgba_p[1];
			int	b = rgba_p[2];
			int	a = rgba_p[3];

			*s_p++ = ((r>>3)<<11) | ((g>>3)<<6) | ((b>>3)<<1) | (a>>7);
		}

		if ( w == 1 && h == 1 ) {
			break;
		}
		// mip map
		w >>= 1;
		if ( w == 0 ) {
			w = 1;
		}
		h >>= 1;
		if ( h == 0 ) {
			h = 1;
		}
		tempMip = _alloca( w * h * 4 );
		for ( y = 0 ; y < h ; y++ ) {
			for ( x = 0 ; x < w ; x++ ) {
				for ( c = 0 ; c < 4 ; c++ ) {
					tempMip[(y*w+x)*4+c] = (
						rgba[((y*2+0)*w*2+(x*2+0))*4+c] +
						rgba[((y*2+0)*w*2+(x*2+1))*4+c] +
						rgba[((y*2+1)*w*2+(x*2+0))*4+c] +
						rgba[((y*2+1)*w*2+(x*2+1))*4+c] ) >> 2;
				}
			}
		}
		rgba = tempMip;
	}
	shorts = s_p - s;

	// byte swap
#if 0
	for ( i = 0 ; i < shorts ; i++ ) {
		int temp = ((W8 *)s)[i*2+0];
		((W8 *)s)[i*2+0] = ((W8 *)s)[i*2+1];
		((W8 *)s)[i*2+1] = temp;
	}
#endif

	// write
	strcpy( newName, tgaName );
	ext = strstr( newName, ".tga" );
	strcpy( ext, ".5551" );
	f = fopen( newName, "wb" );   
    if( f == NULL ) {
		printf( "Could not open file (%s) for write!\n", newName );
		return;
	}
	fwrite( s, 2, shorts, f );
	fclose( f );
}

#include <assert.h>
void UpdateSingleSprite( const char *srcTGA, const char *destTGA ) {
	W8	*rgba;
	int	width, height;
	int	i;
	
	// HACK HACK HACK just to convert the single health pack sprite without the
	// trademarked "red cross"

	LoadTGA( srcTGA, &rgba, &width, &height );
	assert( rgba );
	assert( width == 64 && height == 64 );

	// JDC: outline the image to avoid dark halos with filtering
	for ( i = 0 ; i < 8 ; i++ ) {
		OutlineImage( rgba, 64, 64 );
	}
	for ( i = 0 ; i < 64*64 ; i++ ) {
		if ( rgba[i*4+3] == 1 ) {
			rgba[i*4+3] = 0;
		}
	}
	WriteTGA( destTGA, 32, 64, 64, rgba, 0, 1 );	
	Write5551( destTGA, 64, 64, rgba, 1 );	
}

/*
-----------------------------------------------------------------------------
 Function: PML_LoadPage() -Save image data from page file. 
 
 Parameters: nPage -[in] Page number to save.
			 filename -[in] Pointer to string with filename.
			 buffer -[in] Allocated memory buffer to hold image data.
			 buffer2 -[in] Allocated memory buffer to hold hq2x data.

 Returns: Non-zero on success, otherwise zero.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE W8 PML_SaveGFXPage( W32 nPage, const char *filename,
                     W8 *buffer, W8 *buffer2, _boolean iswall, W32 GunFlash )
{
	W16 x, y;
	W8 *data;
	W8 *ptr;
	W16 temp;
	W8 r,g,b;
	W16 clength; // Chunk length
	
	data = (PW8)PML_LoadPage( nPage, &clength );
	if( data == NULL )
    {
        return 0;
    }

    if( nPage < PMSpriteStart )
	{
		// simple 64x64 image
		W8	*rgb = (W8 *)_alloca( 64*64*3 );
	   for( x = 0; x < 64; ++x )
	   {
            for( y = 0; y < 64; ++y )
            {
                temp = ( data[ (x<<6)+y ] ) * 3;			
			
				// gamepal is in 6 bit color
                r = gamepal[ temp ];
                g = gamepal[ temp+1 ];
                b = gamepal[ temp+2 ];

                ptr = rgb + ( ( (y << 6) + x ) * 3 );
                ptr[ 0 ] = ( r << 2 ) | ( r >> 4 );
                ptr[ 1 ] = ( g << 2 ) | ( g >> 4 );
                ptr[ 2 ] = ( b << 2 ) | ( b >> 4 );
            }
        }
		WriteTGA( filename, 24, 64, 64, rgb, 0, 1 );
		Write5551( filename, 64, 64, rgb, 0 );
    }
    else if( nPage >= PMSpriteStart && nPage < PMSoundStart )
    {
        W16 *cmdptr;
        short *linecmds;
        t_compshape *shape;
		int	x, y;
		W8	*rgba = (W8 *)_alloca( 64*64*4 );
		int		i;

        // all transparent at the beginning
		memset( rgba, 0, 64*64*4 );
        
		// draw the spans into the buffer
        shape = (t_compshape *)data;
        cmdptr = shape->dataofs;
        for( x = shape->leftpix; x <= shape->rightpix; ++x )
        {
            linecmds = (short *)( data + *cmdptr++ );
            for( ; *linecmds; linecmds += 3 )
            {
                i = linecmds[ 2 ] / 2 + linecmds[ 1 ];
                for( y = linecmds[ 2 ] / 2; y < linecmds[ 0 ] / 2; ++y, ++i )
                {
				    temp = ( data[ i ] ) * 3;
						
                    r = gamepal[ temp ];
                    g = gamepal[ temp+1 ];
                    b = gamepal[ temp+2 ];

                    ptr = rgba + ( (y * 64 + x) * 4 );
				    ptr[ 0 ] = ( r << 2 ) | ( r >> 4 );
					ptr[ 1 ] = ( g << 2 ) | ( g >> 4 );
					ptr[ 2 ] = ( b << 2 ) | ( b >> 4 );
					ptr[ 3 ] = 255;
                }
            }
        }

		// JDC: outline the image to avoid dark halos with filtering
		for ( i = 0 ; i < 8 ; i++ ) {
			OutlineImage( rgba, 64, 64 );
		}
		for ( i = 0 ; i < 64*64 ; i++ ) {
			if ( rgba[i*4+3] == 1 ) {
				rgba[i*4+3] = 0;
			}
		}
		WriteTGA( filename, 32, 64, 64, rgba, 0, 1 );	
		Write5551( filename, 64, 64, rgba, 1 );	
    } 
    else
    {
        MM_FREE( data );
        printf( "Out of bounds page number passed into PML_SavePage()!\n" );
        return 0;
    }
	
	
	MM_FREE( data );
		
    return 1;
}

/*
-----------------------------------------------------------------------------
 Function: PML_SaveSoundPage() -Save sound data from Page file. 
 
 Parameters: nPage -[in] Page number to save.
			 filename -[in] Pointer to string with filename.
			 buffer -[in] Allocated memory buffer to hold sound data.
			 size -[in] Size of allocated memory buffer.

 Returns: Non-zero on success, otherwise zero.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE W8 PML_SaveSoundPage( W32 nPage, char *filename, 
								W8 *buffer,
								W32 size )
{
    static W16 totallength = 0;
    W8 *data;
    W16 clength; // Chunk length
    
    if( nPage < PMSoundStart || nPage > PMNumBlocks )
    {
        printf( "Out of bounds page number passed into PML_SaveSound()!\n" );
        return 1;
    }
    
    data = (PW8) PML_LoadPage( nPage, &clength );
	if( data == NULL )
    {
        return 1;
    }
    
    if( totallength > size )
    {
        printf( "[wolf_pmc] Buffer not large enough!\n" );
        return 2;
    }
    
    memcpy( buffer + totallength, data, clength );
    
    totallength += clength;
    if( clength < 4096 )
    {                    
        write_wav( filename, buffer, totallength, 1, SAMPLERATE, 1 );    
        totallength = 0;
    }        
        
    MM_FREE( data );    
    
    return 0;
}

/*
-----------------------------------------------------------------------------
 Function: PML_Shutdown() -Shutdown page cache. 
 
 Parameters: Nothing.

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PML_Shutdown()
{
    if( PageFile )
    {
        fclose( PageFile );
        PageFile = NULL;
    }
  
    if( PMPages )
    {
        MM_FREE( PMPages );
    } 
}


/*
-----------------------------------------------------------------------------
 Function: PExtractor() -Interface to page manager. 
 
 Parameters: extension -[in] Ponter to string with file extenion of data
							files.

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean PExtractor( const char *extension, W16 version )
{
    W32 i, j;
    char filename[ 256 ];
    W8 *buffer, *buffer2;
	W32 Flash;
    

//
// Setup
//

	if( 0 == FS_Mkdir( GFXWALLDIR ) )
	{
		printf( "[%s] Could not create directory (%s)!\n", "PExtractor", GFXWALLDIR );
		
		return false;
	}	

	if( version == SOD_PAK || version == SDM_PAK )
	{
		if( 0 == FS_Mkdir( SODGFXSPRITEDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "PExtractor", GFXSPRITEDIR );
			
			return false;
		}

		if( 0 == FS_Mkdir( SODSFXDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "PExtractor", SODSFXDIR );
			
			return false;
		}
	}
	else
	{
		if( 0 == FS_Mkdir( GFXSPRITEDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "PExtractor", GFXSPRITEDIR );
			
			return false;
		}

		if( 0 == FS_Mkdir( SFXDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "PExtractor", SFXDIR );
			
			return false;
		}
	}

	
    if( ! PML_OpenPageFile( extension ) )
    {
		PML_Shutdown();
        
		return false;
    } 
	
//
// Allocate buffers
//

	buffer = MM_MALLOC( 64 * 64 * 2 );
	if( buffer == NULL )
	{
		PML_Shutdown();
		
		return false;
	}

    buffer2 = MM_MALLOC( 128 * 128 * 4 );	
	if( buffer2 == NULL )
	{
		MM_FREE( buffer );
		PML_Shutdown();
		
		return false;
	}

//
// Decode Page data
//

	printf( "Decoding Page Data...\n" );

	for( i = 0, j = 0; i < PMSpriteStart; ++i, ++j )
    { 
		// Hacks
		if( version == WL6_PAK || version == WL1_PAK )
		{
			if( 98 == j )
			{
				j = 126;
			}
		}

		cs_snprintf( filename, sizeof( filename ), "%s/%.3d.tga", GFXWALLDIR, j );

		PML_SaveGFXPage( i, filename, buffer, buffer2, 1, 0  );
	}

    for( i = PMSpriteStart, j = 0; i < PMSoundStart; ++i, ++j )
    { 
		// Hacks
		if( version == WL1_PAK )
		{
			if( j == 50 )
			{
				j = 54;
			}

			if( j == 191 )
			{
				j = 300;
				i += 109;
			}

			if( j == 311 )
			{
				j = 431;
				i += 101;				
			}

			if( j == 439 )
			{
				j = 514;				
			}
		}

		if( version == WL6_PAK )
		{
			if( j == 50 )
			{
				j = 54;
			}

			if( j == 389 )
			{
				j = 408;
			}

			if( j == 439 )
			{
				j = 514;
			}
		}

		if( version == SDM_PAK )
		{
			if( j == 401 )
			{
				j = 514;
			}
		}

		if( version == SOD_PAK )
		{
			if( j == 292 )
			{
				j = 374;
			}

			if( j == 408 )
			{
				j = 439;
			}
		}

		if( version == SOD_PAK || version == SDM_PAK )
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.tga", SODGFXSPRITEDIR, j );
		}
		else
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.tga", GFXSPRITEDIR, j );
		}


		if( j == 531 ||
			j == 532 ||
			j == 526 ||
			j == 521 )
		{
			Flash = j;
		}
		else
		{
			Flash = 0;
		}


        PML_SaveGFXPage( i, filename, buffer, buffer2, 0, Flash );
    }  				

    for( i = PMSoundStart, j = 0; i < PMNumBlocks-1; ++i, ++j )
    { 
		if( version == SOD_PAK || version == SDM_PAK )
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.wav", SODSFXDIR, j );
		}
		else
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.wav", SFXDIR, j );
		}

        PML_SaveSoundPage( i, filename, buffer2, 128 * 128 * 4  );
    }

//
// Shutdown
//
	 
    MM_FREE( buffer );
    MM_FREE( buffer2 );
  
    PML_Shutdown();

	return true;
}

