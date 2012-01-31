/*

	Copyright (C) 2009 Id Software, Inc.
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

#include "../wolfiphone.h"

filehandle_t *fh;

PRIVATE size_t ovc_read( void *ptr, size_t size, size_t nmemb, void *dataSource )
{
	if( ! size || ! nmemb )
	{
		return 0;
	}
		
	return FS_ReadFile( ptr, size, nmemb, fh );
}

PRIVATE int ovc_seek( void *dataSource, ogg_int64_t offset, int whence )
{	
	return FS_FileSeek( fh, offset, whence );
}

PRIVATE int ovc_close( void *dataSource )
{
	return 0;
}

PRIVATE long ovc_tell( void *dataSource )
{
	return FS_FileTell( fh );
}



/*
-----------------------------------------------------------------------------
 Function: LoadOggInfo -Load ogg file.
 
 Parameters: filename -[in] Name of wav file to load.
			 wav -[out] wav data.
			 info -[out] wav sound info.
 
 Returns: True if file loaded, otherwise false.
 
 Notes: Caller is responsible for freeing wav data by calling Z_Free.

-----------------------------------------------------------------------------
*/
PUBLIC _boolean LoadOggInfo( const char *filename, W8 **wav, soundInfo_t *info )
{
	W8	*data;
	int size;
	int dummy;
	char *newFilename;
	int len;
	OggVorbis_File	vorbisFile;
	vorbis_info		vorbisInfo;
	ov_callbacks	vorbisCallbacks = {ovc_read, ovc_seek, ovc_close, ovc_tell};
	int ret;
	
	newFilename = strdup( filename );
	
	len = strlen( newFilename );
	if ( len < 5 || strcmp( newFilename + len - 4, ".wav" ) ) {
		free( newFilename );
		return false;
	}
	
	newFilename[ len - 3 ] = 'o';
	newFilename[ len - 2 ] = 'g';
	newFilename[ len - 1 ] = 'g';
		
	fh = FS_OpenFile( newFilename, 0 );
	if( ! fh )
	{
		free( newFilename );
		return false;
	}

	if( (ret = ov_open_callbacks( fh, &vorbisFile, NULL, 0, vorbisCallbacks )) < 0 ) {
		free( newFilename );
		return false;
	}

	vorbisInfo = *ov_info( &vorbisFile, -1 );
	if( vorbisInfo.channels != 1 && vorbisInfo.channels != 2 )
	{
		Com_Printf( "Only mono and stereo OGG files supported (%s)\n", newFilename );
		free( newFilename );
		return false;
	}
	
	info->channels = vorbisInfo.channels;
	info->sample_rate = vorbisInfo.rate;
	info->sample_size = 2;

#define BUFFER_SIZE ( 128 * 1024 )
	
	data = (W8 *)malloc( BUFFER_SIZE );
	size = 0;
	
	while( size < BUFFER_SIZE )
	{
		int read = 0;
		read = ov_read( &vorbisFile, (char *)data + size, BUFFER_SIZE - size, &dummy );
		if( read == 0 )
		{
			break;
		}
		
		if( read <= 0 )
		{
			Com_Printf( "Only mono and stereo OGG files supported (%s)\n", newFilename );
			free( newFilename );
			return false;
		}
		
		size += read;
	}

	info->samples = size / ( info->channels * info->sample_size );

//	Com_Printf("Loaded %s: channels=%d, sample_rate=%d, sample_size=%d, samples=%d.  \n", newFilename, info->channels, info->sample_rate, info->sample_size, info->samples );
	free( newFilename );

	*wav = (W8 *)Z_Malloc( size );
	memcpy(*wav, data, size );
	free( data );

	
	FS_CloseFile( fh );

	return true;
}

