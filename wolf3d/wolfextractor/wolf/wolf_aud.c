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
 *	wolf_aud.c:   Decode Wolfenstein 3-D Adlib audio data.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "wolf_def.h"

#include "../string/com_string.h"
#include "../adlib/adlib.h"
#include "../adlib/fmopl.h"
#include "../loaders/wav.h"
#include "../filesys/file.h"
#include "../../../common/common_utils.h"
#include "../../../common/arch.h"
#include "../memory/memory.h"

#define AHEADFNAME "AUDIOHED"
#define AUDIOFNAME "AUDIOT"
				 

#define MAX_CHUNK_SIZE	500


#define WL6_STARTMUSIC	262
#define SOD_STARTMUSIC	243



PRIVATE FILE *audiohandle;

PRIVATE W32 *audiostarts;


/*
-----------------------------------------------------------------------------
 Function: CAL_SetupAudioFile() -Setup for decoding audio data.
 
 Parameters: fextension -[in] Pointer to string with file extension.
 
 Returns: Non-zero on success, otherwise zero.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE W8 CAL_SetupAudioFile( const char *fextension )
{
	FILE *handle;
	SW32 length;
	W32 count;
	char fname[ 13 ];

	if( ! fextension || ! *fextension )
	{
		printf( "NULL extension passed into CAL_SetupAudioFile!\n" );
		
		return 0;	
	}

//
// load audiohed.XXX (offsets and lengths for audio file)
//
	cs_strlcpy( fname, AHEADFNAME, sizeof( fname ) );
	cs_strlcat( fname, fextension, sizeof( fname ) );
	
    handle = fopen( cs_strupr( fname ), "rb" );
	if( handle == NULL )
	{
		handle = fopen( cs_strlwr( fname ), "rb" );
		
		if( handle == NULL )
		{
			printf( "Can not open file (%s) for read!\n", fname );
			return 0;
		}
    }

	length = FS_FileLength( handle );
	if( length < 4 )
	{
		fclose( handle );
		printf( "Incorrect audio header size on file: %s\n", fname );
		return 0;
	}

	audiostarts = (PW32) MM_MALLOC( length );
	if( audiostarts == NULL )
	{
		return 0;
	}
	
	count = fread( audiostarts, sizeof( W32 ), length >> 2, handle );
	if( count != (W32)(length >> 2) )
    {
        fclose( handle );
        printf( "[Error]: Read error on file: (%s)", fname  );
		return 0;
	}
	
	
	fclose( handle );

//
// open the Audio data file
//
	cs_strlcpy( fname, AUDIOFNAME, sizeof( fname ) );
	cs_strlcat( fname, fextension, sizeof( fname ) );
	
	audiohandle = fopen( cs_strupr( fname ), "rb" );
	if( audiohandle == NULL )
	{
		audiohandle = fopen( cs_strlwr( fname ), "rb" );
		if( audiohandle == NULL )
		{
			printf( "Could not open file (%s) for read!\n", fname ); 
			return 0;
		}
    }
    
    return 1;
}

/*
-----------------------------------------------------------------------------
 Function: CA_CacheAudioChunk() -Cache audio data.
 
 Parameters: chunk -[in] Chunk number to cache.
			 BuffChunk -[in] Allocated memory block to hold data.
 
 Returns: Non-zero on success, otherwise zero.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE W8 CA_CacheAudioChunk( W32 chunk, W8 *BuffChunk )
{
	W32	pos, length, count;


//
// load the chunk into a buffer
//
	pos = audiostarts[ chunk ];
	length = audiostarts[ chunk+1 ] - pos;
	if( length < 1 || length > MAX_CHUNK_SIZE )
	{
		printf( "[CA_CacheAudioChunk]: Chunk length not valid\n" );
		return 0;
	}
	

	if( fseek( audiohandle, pos, SEEK_SET ) != 0 )
	{
		printf( "[CA_CacheAudioChunk]: Could not seek!\n" );
		return 0;
	}


	count = fread( BuffChunk, 1, length, audiohandle );
	if( count != length )
	{
		printf( "[CA_CacheAudioChunk]: Read error!\n" );
		return 0;	
	}

	return 1;
}

/*
-----------------------------------------------------------------------------
 Function: CA_SaveAudioChunk() -Decode and save audio data.
 
 Parameters: chunk -[in] Chunk number to cache.
			 filename -[in] Save as filename.
			 BuffChunk -[in] Sound data to decode.
			 BuffWav -[in] Allocated memory block to hold decoded data.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void CA_SaveAudioChunk( W32 chunk, const char *filename,
				  W8 *BuffChunk, W8 *BuffWav )
{     
	W32 length;

	if( ! filename || ! *filename )
	{
	   return;	
	}

	if( ! CA_CacheAudioChunk( chunk, BuffChunk ) )
	{
		return;
	}

	if( ADLIB_DecodeSound( (AdLibSound *)BuffChunk, BuffWav, &length ) == 0 )
	{
	   return;
    }	

	write_wav( filename, BuffWav, length, 1, 22050, 2  );
}


extern W32 ADLIB_UpdateMusic( W32 size, void *buffer );
extern W32 ADLIB_getLength( void *musbuffer );
extern void ADLIB_LoadMusic( void *musbuffer );

extern int vorbis_encode( const char *filename, void *data, W32 size, W32 in_channels, W32 in_samplesize,
			   W32 rate, W32 quality, W32 max_bitrate, W32 min_bitrate  );


#define NCH 1		// channels
#define BPS 16		// bit per second

PRIVATE void CA_SaveMusicChunk( W32 chunk, const char *filename )
{
	W8 *data, *BuffWav;
	W32 pos, length, uncompr_length;
	W32 len;


	pos = audiostarts[ chunk ];
	length = audiostarts[ chunk+1 ] - pos;

	data = MM_MALLOC( length );
	if( data == NULL )
	{
		return;
	}

	if( fseek( audiohandle, pos, SEEK_SET ) != 0 )
	{
		printf( "[CA_SaveMusicChunk]: Could not seek!\n" );
		
		MM_FREE( data );

		return;
	}

	if( fread( data, 1, length, audiohandle ) != length )
	{
		printf( "[CA_SaveMusicChunk]: Read error!\n" );

		MM_FREE( data );

		return;	
	}


	uncompr_length = ADLIB_getLength( data );
	if( uncompr_length == 1 )
	{
		MM_FREE( data );

		return;	
	}
	

	ADLIB_LoadMusic( data );

	BuffWav = MM_MALLOC( uncompr_length * 64 * 2 );
	if( BuffWav == NULL )
	{
		MM_FREE( data );

		return;
	}

	len = ADLIB_UpdateMusic( uncompr_length, BuffWav );


#if 1

	vorbis_encode( filename, BuffWav, len, 1, 16, 44100, 0, 0, 0 );

#else

	write_wav( filename, BuffWav, len, 1, 44100, 2  );

#endif

	MM_FREE( BuffWav );
	MM_FREE( data );
}

/*
-----------------------------------------------------------------------------
 Function: CAL_ShutdownAudioFile() -Decode and save audio data.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void CAL_ShutdownAudioFile()
{    
    if( audiohandle )
    {
        fclose( audiohandle );
        audiohandle = NULL;
    }
    
    if( audiostarts )
    {
        MM_FREE( audiostarts );
    }
    
}

/*
-----------------------------------------------------------------------------
 Function: AudioRipper() -Interface to audio decoder.
 
 Parameters: fextension -[in] file extension string.
			 start -[in] Chunk number for start of audio data.
			 end -[in] Chunk number for end of audio data.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC _boolean AudioRipper( const char *fextension, 
						W32 start, W32 end, W16 version )
{
    W32 i, j;
	char filename[ 64 ];
	W8 *buffChunk;
	W8 *buffWav;
	W32 startofmusic = WL6_STARTMUSIC - 1;
	W32 endofmusic = LASTMUSIC;

//
// Setup
//

	if( version == SOD_PAK || version == SDM_PAK )
	{
		if( 0 == FS_Mkdir( SODLSFXDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "wolf_aud.c", SODLSFXDIR );
			
			return false;
		}

		startofmusic = SOD_STARTMUSIC;
		endofmusic = SOD_LASTMUSIC;
	}
	else
	{
		if( 0 == FS_Mkdir( LSFXDIR ) )
		{
			printf( "[%s] Could not create directory (%s)!\n", "wolf_aud.c", LSFXDIR );
			
			return false;
		}
	}
	

	if( 0 == FS_Mkdir( MUSICDIR ) )
	{
		printf( "[%s] Could not create directory (%s)!\n", "wolf_aud.c", LSFXDIR );
		
		return false;
	}




	
    if( ! CAL_SetupAudioFile( fextension ) )
    {
		CAL_ShutdownAudioFile();
        
		return false;
    }		

    if( ! ADLIB_Init( 22050 ) )
	{
		CAL_ShutdownAudioFile();
		
		return false;
	}

//
// Allocate buffers
//

	buffChunk = MM_MALLOC( MAX_CHUNK_SIZE );
	if( buffChunk == NULL )
	{
		ADLIB_Shutdown();
		CAL_ShutdownAudioFile();
		
		return false;
	}

	buffWav = MM_MALLOC( MAX_WAV_SIZE );
	if( buffWav == NULL )
	{
		ADLIB_Shutdown();
		CAL_ShutdownAudioFile();
		MM_FREE( buffChunk );
		
		return false;
	} 

//
// Decode Audio data
//
   
	printf( "Decoding Audio Data...\n" );
    
	for( i = start, j = 0; i < end; ++i, ++j )
    {        
		if( version == SOD_PAK || version == SDM_PAK )
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.wav", SODLSFXDIR, j );
		}
		else
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%.3d.wav", LSFXDIR, j );
		}

		CA_SaveAudioChunk( i, filename, buffChunk, buffWav );
    }

	ADLIB_Shutdown();

	MM_FREE( buffWav );
	MM_FREE( buffChunk );


//
//	Decode Music data
//

	if( ! ADLIB_Init( 44100 ) )
	{
		CAL_ShutdownAudioFile();
		
		return false;
	}

	printf( "Decoding Music Data...\n" );

	for( i = 0 ; i < endofmusic ; ++i )
	{
		if( version == SOD_PAK || version == SDM_PAK )
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%s.ogg", MUSICDIR, GetMusicFileName_SOD( i ) );
		}
		else
		{
			cs_snprintf( filename, sizeof( filename ), "%s/%s.ogg", MUSICDIR, GetMusicFileName_WL6( i ) );
		}

		CA_SaveMusicChunk( startofmusic + i, filename );
	}

	ADLIB_Shutdown();

//
// Shutdown
//	
    
    
    CAL_ShutdownAudioFile();
    
    
    
    return true;
}
