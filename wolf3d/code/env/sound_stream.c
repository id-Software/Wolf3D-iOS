/*

	Copyright (C) 2005 Michael Liebscher	

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
 *	sound_stream.c:	Sound Stream manager.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II Evolved.
 *
 */

#include "../wolfiphone.h"


typedef struct 
{
	char		introName[ MAX_GAMEPATH ];
	char		loopName[ MAX_GAMEPATH ];
	_boolean	looping;

	filehandle_t *hFile;


	int			start;
	int			rate;
	unsigned	format;
	void		*vorbisFile;

} musicTrack_t;


// anything greater than 1<<13 caused crashes on iphone OS 2.1 (on a 3G iphone)
#define BUFFER_SIZE		(1<<11)

PRIVATE musicTrack_t	bgTrack;

PRIVATE channel_t	*s_streamingChannel;


extern void Sound_StopBGTrack( void );



/*
-----------------------------------------------------------------------------
 Function: ovc_read -OGG read Callback. Reads data from a stream.
 
 Parameters: 
		ptr -[in] Storage location for data.
		size -[in] Item size in bytes.
		nmemb -[in] Maximum number of items to be read.
		datasource -[in] music track data structure.
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE size_t ovc_read( void *ptr, size_t size, size_t nmemb, void *datasource )
{
	musicTrack_t	*track = (musicTrack_t *)datasource;

	
	if( ! size || ! nmemb )
	{
		return 0;
	}

	
	return FS_ReadFile( ptr, size, nmemb, track->hFile );
}

/*
-----------------------------------------------------------------------------
 Function: ovc_read -OGG seek Callback. Moves the file pointer to a specified
					location.
 
 Parameters: 
			datasource -[in] music track data structure.
			offset -[in] Number of bytes from whence.
			whence -[in] Initial position.

 
 Returns: 
		If successful, fseek returns 0. Otherwise, it returns a nonzero
		value.	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE int ovc_seek( void *datasource, ogg_int64_t offset, int whence )
{
	musicTrack_t	*track = (musicTrack_t *)datasource;

	return FS_FileSeek( track->hFile, offset, whence );
}

/*
-----------------------------------------------------------------------------
 Function: ovc_close -OGG close Callback. Closes a stream.
 
 Parameters: datasource -[in] music track data structure.
 
 Returns: 0 if the stream is successfully closed, otherwise nonzero.	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE int ovc_close( void *datasource )
{
	return 0;
}

/*
-----------------------------------------------------------------------------
 Function: ovc_tell -OGG tell Callback. Gets the current position of a file
					pointer.
 
 Parameters: datasource -[in] music track data structure.
 
 Returns: The current file position.	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE long ovc_tell( void *datasource )
{
	musicTrack_t	*track = (musicTrack_t *)datasource;

	return FS_FileTell( track->hFile );
}

/*
-----------------------------------------------------------------------------
 Function: Sound_OpenBGTrack -OGG read Callback.
 
 Parameters: 
			name -[in] File name to open.
			track -[in/out] Music track data structure.
 
 Returns: False on error, otherwise true.	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE _boolean Sound_OpenBGTrack( const char *name, musicTrack_t *track )
{
	OggVorbis_File	*vorbisFile;
	vorbis_info		*vorbisInfo;
	ov_callbacks	vorbisCallbacks = {ovc_read, ovc_seek, ovc_close, ovc_tell};
	int ret;
	extern cvar_t *music;
	
	if ( music->value == 0 || SysIPhoneOtherAudioIsPlaying() ) {
		return 0;
	}
	
	track->hFile = FS_OpenFile( name, 0 );
	if( ! track->hFile )
	{
		return false;
	}


	track->vorbisFile = vorbisFile = Z_Malloc( sizeof( OggVorbis_File ) );

	if( (ret = ov_open_callbacks( track, vorbisFile, NULL, 0, vorbisCallbacks )) < 0 )
	{
		switch( ret )
		{
			case OV_EREAD:
				Com_DPrintf( "A read from media returned an error.(%s)\n", name );
				break;
			case OV_ENOTVORBIS:
				Com_DPrintf( "Bitstream is not Vorbis data.(%s)\n", name );
				break;
			case OV_EVERSION:
				Com_DPrintf( "Vorbis version mismatch.(%s)\n", name );
				break;
			case OV_EBADHEADER:
				Com_DPrintf( "Invalid Vorbis bitstream header.(%s)\n", name );
				break;
			case OV_EFAULT:
				Com_DPrintf( "Internal logic fault; indicates a bug or heap/stack corruption.(%s)\n", name );
				break;

		}
		Com_DPrintf( "Could not open OGG stream (%s)\n", name );
		
		return false;
	}

	vorbisInfo = ov_info( vorbisFile, -1 );
	if( vorbisInfo->channels != 1 && vorbisInfo->channels != 2 )
	{
		Com_DPrintf( "Only mono and stereo OGG files supported (%s)\n", name );
		
		return false;
	}

	track->start = ov_raw_tell( vorbisFile );
	track->rate = vorbisInfo->rate;
	track->format = (vorbisInfo->channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_CloseBGTrack -Close out background music track.
 
 Parameters: track -[in] Music track to close.
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_CloseBGTrack( musicTrack_t *track )
{
	if( track->vorbisFile )
	{
		ov_clear( track->vorbisFile );

		Z_Free( track->vorbisFile );
		track->vorbisFile = NULL;
	}

	if( track->hFile )
	{
		FS_CloseFile( track->hFile );
	}
}

/*
-----------------------------------------------------------------------------
 Function: Sound_StreamBGTrack -Called each frame to update streaming music
								track.
 
 Parameters: Nothing
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StreamBGTrack( void )
{

	W8		data[BUFFER_SIZE];
	int			processed, queued, state;
	int			size, read, dummy;
	unsigned	buffer;

	if( ! s_musicVolume->value )
	{
		return;
	}

	if( ! s_streamingChannel )
	{
		return;
	}

	// Unqueue and delete any processed buffers
	pfalGetSourcei( s_streamingChannel->sourceName, AL_BUFFERS_PROCESSED, &processed );
	AL_CheckErrors();
	
	if( processed > 0 )
	{
		while (processed--)
		{
			pfalSourceUnqueueBuffers( s_streamingChannel->sourceName, 1, &buffer );
			AL_CheckErrors();
			//printf( "OpenAL unqueued %d, about to delete it!\n", buffer);
			pfalDeleteBuffers( 1, &buffer );
			AL_CheckErrors();
		}
	}

	// Make sure we always have at least 4 buffers in the queue
	pfalGetSourcei( s_streamingChannel->sourceName, AL_BUFFERS_QUEUED, &queued );
	AL_CheckErrors();
	while( queued < 4 )
	{
		size = 0;

		// Stream from disk
		while( size < BUFFER_SIZE )
		{
			read = ov_read( bgTrack.vorbisFile, (char *)data + size, BUFFER_SIZE - size, &dummy );
			if( read == 0 )
			{
				// End of file
				if( ! bgTrack.looping)
				{
					// Close the intro track
					Sound_CloseBGTrack( &bgTrack );

					// Open the loop track
					if( ! Sound_OpenBGTrack( bgTrack.loopName, &bgTrack ) )
					{
						Sound_StopBGTrack();
						return;
					}

					bgTrack.looping = true;
				}

				// Restart the track, skipping over the header
				ov_raw_seek( bgTrack.vorbisFile, (ogg_int64_t)bgTrack.start );

				// Try streaming again
				read = ov_read( bgTrack.vorbisFile, (char *)data + size, BUFFER_SIZE - size, &dummy );
			}

			if( read <= 0 )
			{
				// An error occurred
				Sound_StopBGTrack();
				return;
			}

			size += read;
		}

		// Upload and queue the new buffer
		pfalGenBuffers( 1, &buffer );
		AL_CheckErrors();
		
		pfalBufferData( buffer, bgTrack.format, data, size, bgTrack.rate );
		AL_CheckErrors();
		
		pfalSourceQueueBuffers( s_streamingChannel->sourceName, 1, &buffer );
		AL_CheckErrors();

		queued++;
	}

	// Update volume
	pfalSourcef( s_streamingChannel->sourceName, AL_GAIN, s_musicVolume->value );
	AL_CheckErrors();

	// If not playing, then do so
	pfalGetSourcei( s_streamingChannel->sourceName, AL_SOURCE_STATE, &state );
	AL_CheckErrors();
	
	if( state != AL_PLAYING )
	{
		pfalSourcePlay(s_streamingChannel->sourceName);
		AL_CheckErrors();
	}
}

/*
-----------------------------------------------------------------------------
 Function: Sound_StartStreaming -Start streaming background music track.
 
 Parameters: Nothing
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StartStreaming( void )
{
	if( ! sound_initialized )
	{
		return;
	}

	if( s_streamingChannel )
	{
		return;
	}

	s_streamingChannel = Sound_PickChannel( 0, 0 );
	if( ! s_streamingChannel )
	{
		return;
	}

	s_streamingChannel->streaming = true;

	// hmmm...
	pfalDeleteSources( 1, &s_streamingChannel->sourceName );
	AL_CheckErrors();
	
	pfalGenSources( 1, &s_streamingChannel->sourceName );
	AL_CheckErrors();

	// Set up the source
	pfalSourcei( s_streamingChannel->sourceName, AL_BUFFER, 0 );
	AL_CheckErrors();
	pfalSourcei( s_streamingChannel->sourceName, AL_LOOPING, AL_FALSE );
	AL_CheckErrors();
	pfalSourcei( s_streamingChannel->sourceName, AL_SOURCE_RELATIVE, AL_TRUE );
	AL_CheckErrors();
	pfalSourcefv( s_streamingChannel->sourceName, AL_POSITION, vec3_origin );
	AL_CheckErrors();
	pfalSourcefv( s_streamingChannel->sourceName, AL_VELOCITY, vec3_origin );
	AL_CheckErrors();
	pfalSourcef( s_streamingChannel->sourceName, AL_REFERENCE_DISTANCE, 1.0 );
	AL_CheckErrors();
	pfalSourcef( s_streamingChannel->sourceName, AL_MAX_DISTANCE, 1.0 );
	AL_CheckErrors();
	pfalSourcef( s_streamingChannel->sourceName, AL_ROLLOFF_FACTOR, 0.0 );
	AL_CheckErrors();
}

/*
-----------------------------------------------------------------------------
 Function: Sound_StopStreaming -Stop playing streaming music track.
 
 Parameters: Nothing
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StopStreaming( void )
{
	int			processed;
	unsigned	buffer;

	if( ! sound_initialized )
	{
		return;
	}

	if( ! s_streamingChannel )
	{
		return;
	}

	s_streamingChannel->streaming = false;


	pfalSourceStop( s_streamingChannel->sourceName );
	AL_CheckErrors();

	pfalGetSourcei( s_streamingChannel->sourceName, AL_BUFFERS_PROCESSED, &processed );
	AL_CheckErrors();
	if( processed > 0 )
	{
		while( processed-- )
		{
			pfalSourceUnqueueBuffers( s_streamingChannel->sourceName, 1, &buffer );
			AL_CheckErrors();
			pfalDeleteBuffers( 1, &buffer );
			AL_CheckErrors();
		}
	}

	pfalSourcei( s_streamingChannel->sourceName, AL_BUFFER, 0 );
	AL_CheckErrors();
	
	// hmmm...
	pfalDeleteSources( 1, &s_streamingChannel->sourceName );
	AL_CheckErrors();
	
	pfalGenSources( 1, &s_streamingChannel->sourceName );
	AL_CheckErrors();

	s_streamingChannel = NULL;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_StartBGTrack -Play background music track.
 
 Parameters: 
			introTrack -[in] File name of intro track.
			loopTrack -[in] File name of loop track.	
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StartBGTrack( const char *introTrack, const char *loopTrack )
{
	if( ! sound_initialized )
	{
		return;
	}

	Sound_StopBGTrack();


	my_strlcpy( bgTrack.introName, introTrack, sizeof( bgTrack.introName ) );
	my_strlcpy( bgTrack.loopName, loopTrack, sizeof( bgTrack.loopName) );

	Sound_StartStreaming();

	if( ! Sound_OpenBGTrack( bgTrack.introName, &bgTrack ) )
	{
		Sound_StopBGTrack();
		return;
	}

	Sound_StreamBGTrack();
}

/*
-----------------------------------------------------------------------------
 Function: Sound_StopBGTrack -Stop playing background track.
 
 Parameters: Nothing
 
 Returns: Nothing	
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StopBGTrack( void )
{
	if( ! sound_initialized )
	{
		return;
	}

	Sound_StopStreaming();

	Sound_CloseBGTrack( &bgTrack );

	memset( &bgTrack, 0, sizeof( musicTrack_t ) );
}
