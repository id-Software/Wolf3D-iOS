/*

	Copyright (C) 2004 Michael Liebscher	
	Copyright (C) 1997-2001 Id Software, Inc.
	Copyright (C) 1995 Spencer Kimball and Peter Mattis.

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
 *	texture_manager.c:	Texture manager.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	Portion of this code was derived from
 *	The GIMP (an image manipulation program) and was originally
 *	written by Spencer Kimball and Peter Mattis.
 *
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"

PRIVATE texture_t	ttextures[ MAX_TEXTURES ];
PRIVATE int			numttextures;

PRIVATE texture_t	*r_notexture;		// use for bad textures

cvar_t	*gl_round_down;

int registration_sequence;
W32 texture_registration_sequence;

extern int currentTextures[ 4 ];
extern int currenttmu;
extern int glMaxTexSize;

PRIVATE INLINECALL GLenum WrapToGL( TWrapMode mode )
{
	if( mode == Repeat )
	{
		return GL_REPEAT;
	}
	else
	{
		return GL_CLAMP_TO_EDGE;
	}
}

PRIVATE INLINECALL GLenum MagFilterToGL( TMagFilter MagFilter )
{	
	switch( MagFilter )
	{
		case Nearest: 
			return GL_NEAREST;
			
		case Linear: 
			return GL_LINEAR;
			
		default: 
			break;
	}
	
	return GL_LINEAR;
}



PRIVATE INLINECALL GLenum MinFilterToGL( _boolean MipMap, TMinFilter MinFilter )
{
	if( MipMap )
	{
		switch( MinFilter )
		{
			case NearestMipMapOff: 
				return GL_NEAREST;
				
			case NearestMipMapNearest: 
				return GL_NEAREST_MIPMAP_NEAREST;
				
			case NearestMipMapLinear: 
				return GL_NEAREST_MIPMAP_LINEAR;
				
			case LinearMipMapOff: 
				return GL_LINEAR;
				
			case LinearMipMapNearest: 
				return GL_LINEAR_MIPMAP_NEAREST;
				
			case LinearMipMapLinear: 
				return GL_LINEAR_MIPMAP_LINEAR;
				
			default: 
				break;
		}
	}
	else
	{
		switch( MinFilter )
		{
			case NearestMipMapOff:
			case NearestMipMapNearest:
			case NearestMipMapLinear:
				return GL_NEAREST;
				
			case LinearMipMapOff:
			case LinearMipMapNearest:
			case LinearMipMapLinear:
				return GL_LINEAR;
				
			default: 
				break;
		}
	}
	
	return GL_LINEAR;
}


/*
-----------------------------------------------------------------------------
 Function: TM_TextureList_f	-Console function to list loaded textures.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void TM_TextureList_f( void )
{
	int		i;
	texture_t	*image;
	int		texels;
	const char *palstrings[ 2 ] =
	{
		"RGB",
		"PAL"
	};

	Com_Printf( "------------------\n" );
	texels = 0;

	for( i = 0, image = ttextures ; i < numttextures ; ++i, ++image )
	{
		if( image->texnum <= 0 )
			continue;
		
		texels += image->upload_width * image->upload_height;
		
		switch( image->type )
		{
		case TT_Sprite:
			Com_Printf( "S" );
			break;
			
		case TT_Wall:
			Com_Printf( "W" );
			break;
			
		case TT_Pic:
			Com_Printf( "P" );
			break;
			
		default:
			Com_Printf( " " );
			break;
		}

		Com_Printf( " %3i %3i %s: %s\n",
			image->upload_width, image->upload_height, palstrings[ 0 ], image->name );
	}
	
	Com_Printf( "Total texel count (not counting mipmaps): %i\n", texels );
	
}

texture_t *TM_AllocateTexture( const char *name ) {
	texture_t *tex;
	int		i;
	
	assert( strlen( name ) < sizeof( tex->name ) );
	
	// find a free texture_t space
	for( i = 0, tex = ttextures; i < numttextures; ++i, ++tex )
	{
		if( ! tex->texnum )
		{
			break;
		}
	}
	
	if( i == numttextures )
	{
		if( numttextures == MAX_TEXTURES )
		{
			Com_DPrintf( "MAX_TEXTURES reached\n" );
			return r_notexture;
		}
		
		numttextures++;
	}		
	tex = &ttextures[ i ];	
	memset( tex, 0, sizeof( *tex ) );
	my_strlcpy( tex->name, name, MAX_GAMEPATH );
	tex->registration_sequence = texture_registration_sequence;

   // don't let R_Bind skip the next bind call
   currentTextures[ currenttmu ] = -1;
   
   pfglGenTextures( 1, &tex->texnum );
   pfglBindTexture( GL_TEXTURE_2D, tex->texnum );
		   
	return tex;
}

/*
-----------------------------------------------------------------------------
 Function: TM_LoadTexture	-Load raw image into video memory.
 
 Parameters: 
			name -[in] Name of texture image.
			data -[in] Raw pixel data in the format described by PixelFormat.
			width -[in] Width of image in pixels.
			height -[in] Height of image in pixels.
			type -[in]
			PixelFormat [in]
 
 Returns: Pointer to filled out texture_t structure.
 
 Notes: Any texture that was not touched on this registration sequence will be freed.
-----------------------------------------------------------------------------
*/
PUBLIC texture_t *TM_LoadTexture( const char *name, W8 *data, int width, int height, texturetype_t type, W16 bytes )
{
	texture_t	*tex;
	W8 *scaled;
	W16 scaled_width, scaled_height;

	tex = TM_AllocateTexture( name );
	
	tex->width = width;
	tex->height = height;
	tex->type = type;
	tex->bytes = bytes;
	
	switch( type )
	{
		case TT_Pic: 
			tex->MipMap = false;	
			tex->WrapS = Clamp;
			tex->WrapT = Clamp;
			tex->MinFilter = Nearest;
			tex->MagFilter = NearestMipMapOff;
			break;

		case TT_Wall:
			tex->MipMap = true;	
			tex->WrapS = Repeat;
			tex->WrapT = Repeat;
//			tex->MinFilter = LinearMipMapLinear;
			tex->MinFilter = LinearMipMapNearest;
			tex->MagFilter = Linear;
			break;

		default:
			tex->WrapS = Repeat;
			tex->WrapT = Repeat;
			tex->MinFilter = Nearest;
			tex->MagFilter = NearestMipMapOff;
			break;
	}


	for( scaled_width = 1 ; scaled_width < tex->width ; scaled_width <<= 1 )
	{
		;
	}
	
	if( gl_round_down->value && scaled_width > tex->width && tex->MipMap ) 
	{
		scaled_width >>= 1;
	}
	
	for( scaled_height = 1 ; scaled_height < tex->height ; scaled_height <<= 1 )
	{
		;
	}
	
	if( gl_round_down->value && scaled_height > tex->height && tex->MipMap )
	{
		scaled_height >>= 1;
	}

	// let people sample down the world textures for speed
	if( tex->MipMap )
	{
		scaled_width >>= (int)gl_picmip->value;
		scaled_height >>= (int)gl_picmip->value;
	}

	// don't ever bother with > glMaxTexSize textures
	if( scaled_width > glMaxTexSize )
	{
		scaled_width = glMaxTexSize;
	}
	if( scaled_height > glMaxTexSize )
	{
		scaled_height = glMaxTexSize;
	}
	
	if( scaled_width < 1 )
	{
		scaled_width = 1;
	}
	if( scaled_height < 1 )
	{
		scaled_height = 1;
	}
	
	tex->upload_width = scaled_width;
	tex->upload_height = scaled_height;
	
	if( scaled_width == tex->width && scaled_height == tex->height )
	{
		scaled = data;
	}
	else
	{
		scaled = Z_Malloc( scaled_width * scaled_height * tex->bytes );
		TM_ResampleTexture( data, tex->width, tex->height, scaled, scaled_width, scaled_height, tex->bytes, INTERPOLATION_NONE );
	}
	
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL( tex->WrapS ) );
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL( tex->WrapT ) );
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL( tex->MipMap, tex->MinFilter ) );
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL( tex->MagFilter ) );		
	
#ifdef IPHONE
	if ( type == TT_Wall ) {
		pfglTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f );
	} else {
		pfglTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f );
	}
#endif
	
	{
		// upload base image	
		GLenum internalFormat[] = { GL_LUMINANCE, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };
#if 0		
		GLenum externalFormat[] = { GL_LUMINANCE, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_SHORT_5_6_5,  GL_UNSIGNED_SHORT_5_5_5_1 };
		pfglTexImage2D( GL_TEXTURE_2D, 0, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, externalFormat[ tex->bytes ], scaled );
#else		
		pfglTexImage2D( GL_TEXTURE_2D, 0, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, scaled );
#endif
		
		
		// upload mipmaps if required
#ifdef IPHONE
		glGenerateMipmapOES( GL_TEXTURE_2D );
#else		
		if( tex->MipMap )
		{
			int miplevel = 0;
			
			while( TM_MipMap( scaled, &scaled_width, &scaled_height, tex->bytes ) )
			{
				pfglTexImage2D( GL_TEXTURE_2D, ++miplevel, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, scaled );
			}
		}
#endif		
	}
	
	if ( scaled != data ) {
		Z_Free( scaled );
	}

	return tex;
}


/*
-----------------------------------------------------------------------------
 Function: TM_FreeUnusedTextures	-Free unused textures.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: Any texture that was not touched on this registration sequence will be freed.
-----------------------------------------------------------------------------
*/
PUBLIC void TM_FreeUnusedTextures( void )
{
#if 0	
	int		i;
	texture_t	*tex;

	// never free r_notexture texture
	r_notexture->registration_sequence = texture_registration_sequence;	

	for( i = 0, tex = ttextures ; i < numttextures ; ++i, ++tex )
	{
		if( tex->registration_sequence == texture_registration_sequence )
			continue;		// used this sequence
			
		if( ! tex->registration_sequence )
			continue;		// free image_t slot
			
		if( tex->type == TT_Pic )
			continue;		// don't free pics
			
		// free texture
		R_DeleteTexture( tex->texnum );
		memset( tex, 0, sizeof( *tex ) );
	}
#endif	
}


/*
-----------------------------------------------------------------------------
 Function: TM_FindTexture	-Find texture.
 
 Parameters: name -[in] Name of the texture to find.
			 type -[in] Type of texture (see texturetype_t).
 
 Returns: r_notexture if the texture is not found, otherwise it will
			return a valid texture_t structure.
 
 Notes:
-----------------------------------------------------------------------------
*/
PUBLIC texture_t *TM_FindTexture( const char *name, texturetype_t type )
{
	texture_t	*tex;
	int	i, len;
	W8	*data;			/* raw texture data */
	W16	width, height;	/* width, height of texture */
	W16 bytes;
	char	digested[1024];
	filehandle_t		*fh;
	
	if( ! name || ! *name )
	{
		return r_notexture;
	}

	// Check for file extension
	len = strlen( name );
	if( len < 5 )
	{
		return r_notexture;
	}
	
	// look for it in the texture cache
	for( i = 0, tex = ttextures; i < numttextures; ++i, ++tex )
	{
		if( ! strcmp( name, tex->name ) )
		{
			tex->registration_sequence = texture_registration_sequence;
			return tex;
		}
	}

	//
	// load the texture from disk
	//
	data = NULL;
	const char* const extension = name + len - 4;
	if( strcmp( extension, ".tga" ) && strcmp( extension, ".png" ) ) {
		return r_notexture;
	}

	//gsh
	//Com_Printf( "Loading texture: %s\n", name );
	
	// look for the pre-digested 5551 version
	strcpy( digested, name );
	strcpy( digested + len - 4, ".5551" );
	fh = FS_OpenFile( digested, 0 );
	if ( fh ) {
		typedef struct {
			int		internalFormat;
			int		externalFormat;
			int		bpp;
		} formatInfo_t;
		
		static formatInfo_t formatInfo[7] = {
// the wall exporter always saved as 5551 even when there was no alpha
			{ GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16 },
			{ GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 16 },
			{ GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 16 },
			{ GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, GL_UNSIGNED_BYTE, 4 },
			{ GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, GL_UNSIGNED_BYTE, 4 },
			{ GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, GL_UNSIGNED_BYTE, 2 },
			{ GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, GL_UNSIGNED_BYTE, 2 },
		};
		
		picHeader_t *ph = (picHeader_t *)fh->filedata;
		
		int noMips = 0;
		formatInfo_t *fi;
		if ( ph->picFormat & PF_NO_MIPS ) {
			noMips = 1;
			fi = &formatInfo[ph->picFormat&~PF_NO_MIPS];
		} else {
			fi = &formatInfo[ph->picFormat];
		}
		int	w = ph->uploadWidth;
		int h = ph->uploadHeight;
		int	l = 0;
		texture_t *tx = TM_AllocateTexture( name );
		tx->width = ph->srcWidth;
		tx->height = ph->srcHeight;
		tx->upload_width = w;
		tx->upload_height = h;
		tx->header = *ph;
		tx->maxS = (float)ph->srcWidth / ph->uploadWidth;
		tx->maxT = (float)ph->srcHeight / ph->uploadHeight;
		unsigned char *s = (unsigned char *)(ph+1);
		
		if ( noMips ) {
			pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		} else {
			pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		}
		pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		if ( type == TT_Wall ) {
			pfglTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f );
			pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		} else {
			pfglTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f );
			pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		}
		pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		
		while( 1 ) {
			int	size = (w*h*fi->bpp)/8;
			if ( fi->bpp < 16 ) {
				if ( size < 32 ) {
					// minimum PVRTC size
					size = 32;
				}
				qglCompressedTexImage2D( GL_TEXTURE_2D, l, fi->internalFormat, w, h, 0, 
							   size, s );
			} else {
				qglTexImage2D( GL_TEXTURE_2D, l, fi->internalFormat, w, h, 0, 
							   fi->internalFormat, fi->externalFormat, s );
			}
		   if ( w == 1 && h == 1 ) {
			   break;
		   }
			if ( noMips ) {
				break;
			}
		   l++;
		   s += size;
		   w >>= 1;
		   if ( w == 0 ) {
			   w = 1;
		   }
		   h >>= 1;
		   if ( h == 0 ) {
			   h = 1;
		   }
		}
		FS_CloseFile( fh );
		
		return tx;
	}

	// load a normal PNG or TGA. Prioritize PNGs, if not found, fall back to TGA.
	size_t filenameLength = strlen( name );
	char * pngName = MM_MALLOC( filenameLength );
	strcpy( pngName, name );
	strcpy( pngName + filenameLength - 3, "png" );
	
	// Try to load a PNG version.
	LoadPNG( pngName, &data, &width, &height, &bytes );
	
	MM_FREE( pngName );
	pngName = NULL;
	
	//  Try finding a TGA if the PNG load failed.
	if ( ( data == NULL ) && strcmp( extension, ".tga" ) == 0 ) {
		LoadTGA( name, &data, &width, &height, &bytes );
	}
	
	if ( data ) {
		tex = TM_LoadTexture( name, data, width, height, type, bytes );
		MM_FREE( data );
		tex->maxS = tex->maxT = 1.0f;
		return tex;
	}
	
	// load a jpg
	{
		int jpgSize = 0;
		W8 *jpgData;
		// try jpeg if no tga exists
		strcpy( digested, name );
		strcpy( digested + len - 4, ".jpg" );
		fh = FS_OpenFile( digested, 0 );
		if ( fh == NULL ) {
			Com_Printf( "Failed to find texture %s\n", name );
			return r_notexture;
		} //else {   //added the else...gsh
		jpgSize = FS_GetFileSize( fh );
		jpgData = fh->ptrStart;
		
		SysIPhoneLoadJPG( jpgData, jpgSize, &data, &width, &height, &bytes );
		FS_CloseFile( fh );

		if ( ! data ) {
			free( jpgData );
			return r_notexture; 
		} //else {   //added the else
		tex = TM_LoadTexture( name, data, width, height, type, bytes );
		MM_FREE( data );
		tex->maxS = tex->maxT = 1.0f;
		return tex;
		
		
	}
	
	/*
	Com_Printf("Trying to find texture made it to the end\n");
	
	//gsh.. couldn't find it... try doing it again, but looking in a new location
	if (spritelocation == SODSPRITESDIRNAME && spritelocation != WL6SPRITESDIRNAME)
	{
		//need to remove the 'sod'
		if (strncmp(spritelocation, name, strlen(spritelocation)) == 0)
		{
			char buffer[64];
			char tempName[64];
			int offset = strlen(spritelocation) + 1;
			for (int i = 0; i < strlen(name) - offset; ++i)
			{
				buffer[i] = name[i+offset];
			}
			buffer[i] = '\0'; //just in case
			
			spritelocation = WL6SPRITESDIRNAME;
			
			//TODO: 
			my_snprintf(tempName, sizeof(tempName), "%s/%s", spritelocation, buffer);
			
			Com_Printf("tempName: %s\n", tempName);
			Com_Printf("buffer: %s\n", buffer);
			
			spritelocation = SODSPRITESDIRNAME;  //return to sodsprites
			tex = TM_FindTexture( tempName, type);
			return tex;
		}
	}

	return r_notexture;*/
	return NULL;
}

/*
-----------------------------------------------------------------------------
 Function: TM_GetTextureSize	-Find texture.
 
 Parameters:width -[out] Width of texture.
			height -[out] Height of texture.
			name -[in] Name of the texture to get dimensions of.
			
 
 Returns: Nothing.
 
 Notes: If texture is not found, width and height are -1.
-----------------------------------------------------------------------------
*/
PUBLIC void TM_GetTextureSize( SW32 *width, SW32 *height, const char *name )
{
	texture_t *tex;

	tex = TM_FindTexture( name, TT_Pic );
	if( ! tex )
	{
		*width = *height = -1;
		return;
	}

	*width = tex->width;
	*height = tex->height;
}







/* Note: cubic function no longer clips result */
PRIVATE INLINECALL double
cubic (double dx,
       int    jm1,
       int    j,
       int    jp1,
       int    jp2)
{
  /* Catmull-Rom - not bad */
  return (double) ((( ( - jm1 + 3 * j - 3 * jp1 + jp2 ) * dx +
                       ( 2 * jm1 - 5 * j + 4 * jp1 - jp2 ) ) * dx +
                     ( - jm1 + jp1 ) ) * dx + (j + j) ) / 2.0;
}

_boolean pixel_region_has_alpha( int bytes )
{
	if( bytes == 2 || bytes == 4 )
	{
		return true;
	}
	else
	{
		return false;
	}
}


PRIVATE void
expand_line( double               *dest,
             double               *src,
             int                   bytes,
             int                   old_width,
             int                   width )
{
	double  ratio;
	int     x,b;
	int     src_col;
	double  frac;
	double *s;

	ratio = old_width / (double) width;

	/* we can overflow src's boundaries, so we expect our caller to have
		allocated extra space for us to do so safely (see scale_region ()) */

	/* this could be optimized much more by precalculating the coefficients for
		each x */

	for( x = 0; x < width; ++x )
	{
		src_col = ((int) (x * ratio + 2.0 - 0.5)) - 2;
		/* +2, -2 is there because (int) rounds towards 0 and we need
			to round down */
		frac = (x * ratio - 0.5) - src_col;
		s = &src[ src_col * bytes ];
		for( b = 0 ; b < bytes ; b++ )
			dest[ b ] = cubic( frac, (int)s[ b - bytes ], (int)s[ b ], (int)s[ b + bytes ], (int)s[ b + bytes * 2 ] );

		dest += bytes;
	}

}


PRIVATE void
shrink_line( double               *dest,
             double               *src,
             int                   bytes,
             int                   old_width,
             int                   width )
{
	int          x;
	int          b;
	double      *srcp;
	double      *destp;
	double       accum[4];
	double       slice;
	const double avg_ratio = (double) width / old_width;
	const double inv_width = 1.0 / width;
	int          slicepos;      /* slice position relative to width */

#if 0

	Com_DPrintf( "shrink_line bytes=%d old_width=%d width=%d interp=%d "
              "avg_ratio=%f\n",
              bytes, old_width, width, interp, avg_ratio);

#endif

//	g_return_if_fail( bytes <= 4 );

	/* This algorithm calculates the weighted average of pixel data that
		each output pixel must receive, taking into account that it always
		scales down, i.e. there's always more than one input pixel per each
		output pixel.  */

	srcp = src;
	destp = dest;

	slicepos = 0;

	/* Initialize accum to the first pixel slice.  As there is no partial
		pixel at start, that value is 0.  The source data is interleaved, so
		we maintain BYTES accumulators at the same time to deal with that
		many channels simultaneously.  */
	for( b = 0 ; b < bytes ; ++b )
	{
		accum[ b ] = 0.0;
	}

	for( x = 0 ; x < width ; x++ )
    {
		/* Accumulate whole pixels.  */
		do
		{
			for( b = 0 ; b < bytes ; b++ )
				accum[ b ] += *srcp++;

			slicepos += width;
        }
		while( slicepos < old_width );
		slicepos -= old_width;

		if( ! (slicepos < width))
			Com_Printf( "Assertion (slicepos < width) failed. Please report.\n" );

		if( slicepos == 0 )
		{
			/* Simplest case: we have reached a whole pixel boundary.  Store
				the average value per channel and reset the accumulators for
				the next round.

				The main reason to treat this case separately is to avoid an
				access to out-of-bounds memory for the first pixel.  */
			for (b = 0; b < bytes; b++)
			{
				*destp++ = accum[b] * avg_ratio;
				accum[b] = 0.0;
			}
		}
		else
        {
			for( b = 0; b < bytes; b++ )
            {
				/* We have accumulated a whole pixel per channel where just a
					slice of it was needed.  Subtract now the previous pixel's
					extra slice.  */
				slice = srcp[- bytes + b] * slicepos * inv_width;
				*destp++ = (accum[b] - slice) * avg_ratio;

				/* That slice is the initial value for the next round.  */
				accum[b] = slice;
            }
        }
    }

	/* Sanity check: srcp should point to the next-to-last position, and
		slicepos should be zero.  */
	if( ! (srcp - src == old_width * bytes && slicepos == 0) )
	{
		Com_Printf ("Assertion (srcp - src == old_width * bytes && slicepos == 0)"
               " failed. Please report.");
	}
}

PRIVATE void pixel_region_get_row( W8 *src, int y, int width, W8 *tmp_src, int BytesPerPixel )
{
	int i;
    unsigned long k = 0;
    unsigned char *scanline = tmp_src;
	unsigned char *ptr = src;
    
	for( i = 0 ; i < (width * BytesPerPixel) ; ++i )
	{
		scanline[ k++ ] = ptr[ y * width * BytesPerPixel + i ];
	}		
}

PRIVATE void pixel_region_set_row( W8 *dest,
		      int         BytesPerPixel,
		      int         y,
		      int         width,
		      W8 *data )
{
	int i;
	unsigned long k = 0;
    unsigned char *scanline = dest;
	unsigned char *ptr = data;

	for( i = 0 ; i < (width * BytesPerPixel) ; ++i )
	{
		scanline[ y * width * BytesPerPixel + i ] = ptr[ k++ ];
	}	
}

PRIVATE void
get_premultiplied_double_row( W8 *in,
                              int PRbytes,
                              int         x,
                              int         y,
                              int         w,
                              double     *row,
                              W8      *tmp_src,
                              int         n )
{
	int b;
	int bytes = PRbytes;

	pixel_region_get_row( in, y, w, tmp_src, bytes );

	if( pixel_region_has_alpha( bytes ) )
    {
		/* premultiply the alpha into the double array */
		double *irow  = row;
		int     alpha = bytes - 1;
		double  mod_alpha;

		for( x = 0; x < w; ++x )
		{
			mod_alpha = tmp_src[ alpha ] / 255.0;
			for( b = 0; b < alpha; ++b )
			{
				irow[ b ] = mod_alpha * tmp_src[ b ];
			}
				
			irow[ b ] = tmp_src[ alpha ];
			irow += bytes;
			tmp_src += bytes;
		}
    }
	else /* no alpha */
	{
		for( x = 0; x < w * bytes; ++x )
		{
			row[ x ] = tmp_src[ x ];
		}
	}

	/* set the off edge pixels to their nearest neighbor */
	for( b = 0; b < 2 * bytes; b++ )
	{
		row[ b - 2 * bytes ] = row[ b % bytes ];
	}

	for( b = 0; b < bytes * 2; b++ )
	{
		row[ b + w * bytes ] = row[ (w - 1) * bytes + b % bytes ];
	}
}


PRIVATE INLINECALL void
rotate_pointers( W8 **p, W32 n )
{
	W32  i;
	W8  *tmp;

	tmp = p[ 0 ];
	for( i = 0 ; i < n-1 ; i++ )
    {
		p[ i ] = p[ i + 1 ];
    }

	p[ i ] = tmp;
}

PRIVATE void
get_scaled_row( double              **src,
                int                   y,
                int                   new_width,
                double               *row,
                W8			        *src_tmp,
                W8			        *srcPR,
                int old_width,
                int old_height,
                int bytes )
{
	/* get the necesary lines from the source image, scale them,
		and put them into src[] */
	rotate_pointers( (unsigned char  **)src, 4 );
	
	if( y < 0 )
	{
		y = 0;
	}

	if( y < old_height )
	{
		get_premultiplied_double_row( srcPR, bytes, 0, y, old_width,
                                    row, src_tmp, 1 );
		if( new_width > old_width )
		{
			expand_line( src[3], row, bytes, old_width, new_width );
		}
		else if( old_width > new_width )
		{
			shrink_line( src[3], row, bytes, old_width, new_width );
		}
		else /* no scailing needed */
		{
			memcpy( src[3], row, sizeof( double ) * new_width * bytes );
		}
	}
	else
	{
		memcpy( src[3], src[2], sizeof( double ) * new_width * bytes );
	}
}


/* 
non-interpolating scale_region.
 */
PRIVATE void
scale_region_no_resample( W8 *in, int inwidth, int inheight,
                          W8 *out, int outwidth, int outheight, char bytes )
{
	int   *x_src_offsets;
	int   *y_src_offsets;
	W8 *src;
	W8 *dest;
	int    width, height, orig_width, orig_height;
	int    last_src_y;
	int    row_bytes;
	int    x, y, b;
  

	orig_width = inwidth;
	orig_height = inheight;

	width = outwidth;
	height = outheight;
  

	/*  the data pointers...  */
	x_src_offsets = (int *) MM_MALLOC( sizeof( int ) * width * bytes );
	y_src_offsets = (int *) MM_MALLOC( sizeof( int ) * height );
	src  = (unsigned char *) MM_MALLOC( orig_width * bytes);
	dest = (unsigned char *) MM_MALLOC( width * bytes);

	/*  pre-calc the scale tables  */
	for( b = 0; b < bytes; b++ )
	{
		for( x = 0; x < width; x++ )
		{
			x_src_offsets[ b + x * bytes ] =
				b + bytes * ((x * orig_width + orig_width / 2) / width);
		}
	}

	for( y = 0; y < height; y++ )
	{
		y_src_offsets[ y ] = (y * orig_height + orig_height / 2) / height;
	}

	/*  do the scaling  */
	row_bytes = width * bytes;
	last_src_y = -1;
	for( y = 0; y < height; y++ )
    {
		/* if the source of this line was the same as the source
		*  of the last line, there's no point in re-rescaling.
		*/
		if( y_src_offsets[ y ] != last_src_y )
        {
			pixel_region_get_row( in, y_src_offsets[ y ], orig_width, src, bytes );
			//pixel_region_get_row( srcPR, 0, y_src_offsets[y], orig_width, src, 1 );
			for( x = 0 ; x < row_bytes ; x++ )
            {
				dest[ x ] = src[ x_src_offsets[ x ] ];
            }
			last_src_y = y_src_offsets[ y ];
        }
		
		pixel_region_set_row( out, bytes, y, width, dest );
    }

	MM_FREE( x_src_offsets );
	MM_FREE( y_src_offsets );
	MM_FREE( src );
	MM_FREE( dest );
}


/*
-----------------------------------------------------------------------------
 Function: TM_ResampleTexture	-Resize texture.
 
 Parameters: 
			in -[in] Original texture data.
			inwidth -[in] Original width of texture in pixels.
			inheight -[in] Original height of texture in pixels.
			out -[in/out] Resized texture data.
			outwidth -[in] New width of texture in pixels.
			outheight -[in] New height of texture in pixels.
			bytes -[in] Number of bytes per pixel.
			interpolation -[in] see InterpolationType
				
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void TM_ResampleTexture( W8 *in, int inwidth, int inheight, W8 *out, int outwidth, int outheight, W8 bytes, InterpolationType interpolation )
{
	double *src[ 4 ];
	W8  *src_tmp;
	W8  *dest;
	double *row, *accum;
	int     b;
	int     width, height;
	int     orig_width, orig_height;
	double  y_rat;
	int     i;
	int     old_y = -4;
	int     new_y;
	int     x, y;


	if( interpolation == INTERPOLATION_NONE )
	{
		scale_region_no_resample( in, inwidth, inheight, out, outwidth, outheight, bytes );
		return;
    }

	
	orig_width = inwidth;
	orig_height = inheight;

	width = outwidth;
	height = outheight;

#if 0

  Com_DPrintf( "scale_region: (%d x %d) -> (%d x %d)\n",
              orig_width, orig_height, width, height );

#endif

	/*  find the ratios of old y to new y  */
	y_rat = (double) orig_height / (double) height;
	

	/*  the data pointers...  */
	for( i = 0 ; i < 4 ; ++i )
	{
		src[ i ] = (double *) MM_MALLOC( sizeof( double ) * width * bytes );
	}

	dest = (PW8) MM_MALLOC( width * bytes);

	src_tmp = (PW8) MM_MALLOC( orig_width * bytes );

	/* offset the row pointer by 2*bytes so the range of the array
		is [-2*bytes] to [(orig_width + 2)*bytes] */
	row = (double *) MM_MALLOC( sizeof( double ) * (orig_width + 2 * 2) * bytes );
	row += bytes * 2;

	accum = (double *) MM_MALLOC( sizeof( double ) * width * bytes );


	/*  Scale the selected region  */

	for( y = 0 ; y < height ; y++ )
    {
		
		if( height < orig_height )
        {
			int          max;
			double       frac;
			const double inv_ratio = 1.0 / y_rat;

			if( y == 0 ) /* load the first row if this is the first time through */
			{
				get_scaled_row( &src[0], 0, width, row, src_tmp, in, orig_width, orig_height, bytes );
			}

			new_y = (int)(y * y_rat);
			frac = 1.0 - (y * y_rat - new_y);
			for( x = 0 ; x < width * bytes; ++x )
			{
				accum[x] = src[3][x] * frac;
			}

			max = (int) ((y + 1) * y_rat) - new_y - 1;

			get_scaled_row( &src[ 0 ], ++new_y, width, row, src_tmp, in, orig_width, orig_height, bytes  );

			while( max > 0 )
            {
				for( x = 0 ; x < width * bytes ; ++x )
				{
					accum[x] += src[ 3 ][ x ];
				}
				
				get_scaled_row( &src[ 0 ], ++new_y, width, row, src_tmp, in, orig_width, orig_height, bytes );
				max--;
            }

			frac = (y + 1) * y_rat - ((int) ((y + 1) * y_rat));
			for( x = 0 ; x < width * bytes ; ++x )
			{
				accum[ x ] += frac * src[ 3 ][ x ];
				accum[ x ] *= inv_ratio;
			}
		}
		else if( height > orig_height )
        {
			double p0, p1, p2, p3;
			double dy;

			new_y = (int)floor( y * y_rat - 0.5 );

			while( old_y <= new_y )
            {
				/* get the necesary lines from the source image, scale them,
					and put them into src[] */
				get_scaled_row( &src[ 0 ], old_y + 2, width, row, src_tmp, in, orig_width, orig_height, bytes );
				old_y++;
			}			
			
			dy = (y * y_rat - 0.5) - new_y;

			p0 = cubic( dy, 1, 0, 0, 0 );
			p1 = cubic( dy, 0, 1, 0, 0 );
			p2 = cubic( dy, 0, 0, 1, 0 );
			p3 = cubic( dy, 0, 0, 0, 1 );
                
			for( x = 0 ; x < width * bytes ; ++x )
			{
				accum[ x ] = ( p0 * src[ 0 ][ x ] + p1 * src[ 1 ][ x ] +
                              p2 * src[ 2 ][ x ] + p3 * src[ 3 ][ x ] );
			}
			
              
        }
		else /* height == orig_height */
		{
			get_scaled_row( &src[ 0 ], y, width, row, src_tmp, in, orig_width, orig_height, bytes );
			memcpy( accum, src[ 3 ], sizeof( double ) * width * bytes );
        }

		if( pixel_region_has_alpha( bytes ) )
        {
			/* unmultiply the alpha */
			double  inv_alpha;
			double *p = accum;
			int     alpha = bytes - 1;
			int     result;
			W8		*d = dest;

			for( x = 0 ; x < width ; ++x )
            {
				if( p[ alpha ] > 0.001 )
                {
					inv_alpha = 255.0 / p[ alpha ];
					for( b = 0 ; b < alpha ; b++ )
                    {
						result = RINT( inv_alpha * p[ b ] );
						if( result < 0 )
						{
							d[ b ] = 0;
						}
						else if( result > 255 )
						{
							d[ b ] = 255;
						}
						else
						{
							d[ b ] = result;
						}
                    }
					result = RINT( p[ alpha ] );
					if( result > 255 )
					{
						d[ alpha ] = 255;
					}
					else
					{
						d[ alpha ] = result;
					}
                }
				else /* alpha <= 0 */
				{
					for( b = 0 ; b <= alpha ; ++b )
					{
						d[ b ] = 0;
					}
				}

				d += bytes;
				p += bytes;
            }
        }
		else
        {
			int w = width * bytes;

			for( x = 0 ; x < w ; ++x )
            {
				if( accum[ x ] < 0.0 )
				{
					dest[ x ] = 0;
				}
				else if( accum[ x ] > 255.0 )
				{
					dest[ x ] = 255;
				}
				else
				{
					dest[ x ] = RINT( accum[ x ] );
				}
            }
        }
		pixel_region_set_row( out, bytes, y, width, dest );
    }

	/*  free up temporary arrays  */
	MM_FREE( accum );

	for( i = 0 ; i < 4 ; ++i )
	{
		MM_FREE( src[ i ] );
	}

	MM_FREE( src_tmp );
	MM_FREE( dest );

	row -= 2 * bytes;
	MM_FREE( row );
}


/*
-----------------------------------------------------------------------------
 Function: TM_MipMap	-Generate MipMap.
 
 Parameters: 
				in -[in/out] Texture data.
				width -[in] Width of texture in pixels.
				height -[in] Height of texture in pixels.
 
 Returns: Nothing.
 
 Notes: Operates in place, quartering the size of the texture.
-----------------------------------------------------------------------------
*/
PUBLIC _boolean TM_MipMap( PW8 in, W16 *width, W16 *height, W16 bytes )
{
	W16 new_width, new_height;

	if( *width == 1 && *height == 1 ) 
	{
		return false;
	}

	
	if( *width < 2 )
	{
		new_width = 1;
	}
	else
	{
		new_width = *width >> 1;
	}

	if( *height < 2 )
	{
		new_height = 1;
	}
	else
	{
		new_height = *height >> 1;
	}
	
	TM_ResampleTexture( in, *width, *height, in, new_width, new_height, bytes, INTERPOLATION_CUBIC );

	*width = new_width;
	*height = new_height;

	return true;
}


/*
-----------------------------------------------------------------------------
 Function: TM_Init	-Initialize Texture Manager.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: Generates default texture.
-----------------------------------------------------------------------------
*/
PUBLIC void TM_Init( void )
{
	W8 *ptr;
	W8 *data;
	int x, y;

	gl_round_down = Cvar_Get ("gl_round_down", "1", CVAR_INIT);

	texture_registration_sequence = 1;	

// create a checkerboard texture
	data = MM_MALLOC( 16 * 16 * 4 );
	for( y = 0; y < 16; ++y )
	{
		for( x = 0; x < 16; ++x )
		{
			ptr = &data[ (y * 16 + x) * 4 ];
			if( (y < 8) ^ (x < 8) )
			{ 
				ptr[ 0 ] = ptr[ 1 ] = ptr[ 2 ] = 0x00; 
				ptr[ 3 ] = 0xFF; 
			}
			else
			{ 
				ptr[ 0 ] = ptr[ 1 ] = ptr[ 2 ] = 0xFF;
				ptr[ 3 ] = 0xFF; 
			}
		}
	}
	
	r_notexture = TM_LoadTexture( "***r_notexture***", data, 16, 16, TT_Pic, 4 );

	MM_FREE( data );


	Cmd_AddCommand( "listTextures", TM_TextureList_f );

	
}

/*
-----------------------------------------------------------------------------
 Function: TM_Shutdown	-Shutdown Texture Manager.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void TM_Shutdown( void )
{
	int		i;
	texture_t	*tex;

	for( i = 0, tex = ttextures; i < numttextures; ++i, ++tex )
	{
		if( ! tex->registration_sequence )
		{
			continue;		// free image_t slot
		}
		
		// free texture
		R_DeleteTexture( tex->texnum );
		memset( tex, 0, sizeof( *tex ) );
	}

	Cmd_RemoveCommand( "listTextures" );
}

