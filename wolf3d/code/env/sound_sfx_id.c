/*

	Copyright (C) 2004-2005 Michael Liebscher
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

/*
 *	sound_sfx_id.c:
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	
 *	Date:	2004
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II Evolved.
 *
 */

#include "../wolfiphone.h"



#define SFX_HASHSIZE		256

#define	MAX_SFX				1024

PRIVATE sfx_t		*s_sfxHash[ SFX_HASHSIZE ];
PRIVATE sfx_t		*s_sfx[ MAX_SFX ];
PRIVATE int			s_numSfx;


PRIVATE _boolean s_registering = false;
PRIVATE W32	s_registration_sequence = 0;




void Sound_SoundList_f( void )
{
	
}



/*
-----------------------------------------------------------------------------
 Function: Sound_UploadSound -Upload sound data to buffer.
 
 Parameters: data -[in] Sound data.
			 sample_size -[in] Sound sample size. 
			 channels -[in] Number of sound channels.
			 sfx -[in] valid pointer to sfx_t structure.

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Sound_UploadSound( W8 *data, int sample_size, int channels, sfx_t *sfx )
{
	int	size;

	// Calculate buffer size
	size = sfx->samples * sample_size * channels;

	// Set buffer format
	if( sample_size == 2 )
	{
		if( channels == 2 )
		{
			sfx->format = AL_FORMAT_STEREO16;
		}
		else
		{
			sfx->format = AL_FORMAT_MONO16;
		}
		
		// Upload the sound
		pfalGenBuffers( 1, &sfx->bufferNum );
		AL_CheckErrors();
		
		pfalBufferData( sfx->bufferNum, sfx->format, data, size, sfx->rate );
		AL_CheckErrors();
	}
	else 
	{
		/*
		 We upsample the sound to 16 bit here because the iphone
		 pops at the beginning and end of buffers with 8 bit. -Cass
		 (Brian Harris wants to be acknowledeged for helping find
		 this workaround.)
		 */
		short *d = (short *)malloc( size * 2 );
		for ( int i = 0; i < size; i++ ) {
			d[i] = ((short)data[i] - 128) * 256;
		}
		if( channels == 2 )
		{
			sfx->format = AL_FORMAT_STEREO16;
		}
		else
		{
			sfx->format = AL_FORMAT_MONO16;
		}
		
		// Upload the sound
		pfalGenBuffers( 1, &sfx->bufferNum );
		AL_CheckErrors();
		
		pfalBufferData( sfx->bufferNum, sfx->format, d, size * 2, sfx->rate );
		AL_CheckErrors();
		free( d );
	}
}

/*
-----------------------------------------------------------------------------
 Function: Sound_LoadSound -Load sound data.
 
 Parameters: sfx -[in] Pointer to valid sfx_t structure.

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean Sound_LoadSound( sfx_t *sfx )
{
    char		name[ MAX_GAMEPATH ];
	W8		*data;
	soundInfo_t	info;

	if( sfx->name[ 0 ] == '*' )
	{
		return false;
	}

	// See if still in memory
	if( sfx->loaded )
	{
		return true;
	}

	my_strlcpy( name, sfx->name, sizeof( name ) );


	if( ! LoadWavInfo( name, &data, &info ) )
	{
		if ( ! LoadOggInfo( name, &data, &info ) ) {
			sfx->defaulted = true;
			
			//Com_Printf( "Could not find sound (%s)\n", name );
			
			return false;
		}
	}
	
	sfx->loaded = true;
	sfx->samples = info.samples;
	sfx->rate = info.sample_rate;

	Sound_UploadSound( data, info.sample_size, info.channels, sfx );

	Z_Free( data );

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_FindSound -Load sound data.
 
 Parameters: sfx -[in] Pointer to valid sfx_t structure.

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC sfx_t *Sound_FindSound( const char *name )
{
	sfx_t		*sfx;
	unsigned	hashKey;

	if( ! name || ! name[ 0 ] )
	{
		Com_Printf( "Sound_FindSound: NULL sound name\n" );

		return NULL;
	}

	if( strlen( name ) >= MAX_GAMEPATH )
	{
		Com_Printf( "Sound_FindSound: sound name exceeds MAX_GAMEPATH\n");

		return NULL;
	}

	// See if already loaded
	hashKey = (my_strhash( name ) % SFX_HASHSIZE);

	for( sfx = s_sfxHash[ hashKey ] ; sfx ; sfx = sfx->nextHash )
	{
		if( ! my_stricmp( sfx->name, name ) )
		{
			return sfx;
		}
	}

	// Create a new sfx_t
	if( s_numSfx == MAX_SFX )
	{
		Com_Printf( "Sound_FindSound: MAX_SFX hit\n" );

		return NULL;
	}

	s_sfx[ s_numSfx++ ] = sfx = Z_Malloc( sizeof( sfx_t ) );

	my_strlcpy( sfx->name, name, sizeof( sfx->name ) );

	// Add to hash table
	sfx->nextHash = s_sfxHash[ hashKey ];
	s_sfxHash[ hashKey ] = sfx;
	
	return sfx;
}


PUBLIC void Sound_BeginRegistration( void )
{
	s_registration_sequence++;
	s_registering = true;
}


PUBLIC sfx_t *Sound_RegisterSound( const char *name )
{
	sfx_t	*sfx;
	bool isSpearSound = false;

	if( ! sound_initialized )
	{
		return NULL;
	}
	
	if( g_version->value == SPEAROFDESTINY  && currentMap.episode >= 6 && strncmp(name, "iphone", 6) && currentMap.episode < 9)//added the episode & iphone check... gsh
	{
		isSpearSound = true;
		
		char tempname[ 256 ];
		my_snprintf( tempname, sizeof( tempname ), "sod%s", name );
		sfx = Sound_FindSound( tempname );
	
		//gsh
		//Com_Printf("Finding Sound: %s\n", tempname);		
	}
	else
	{
		sfx = Sound_FindSound( name );
		
		//gsh
		//Com_Printf("Finding Sound: %s\n", name);
	}
/*
	//original
	if( ! s_registering )
	{
		Sound_LoadSound( sfx );
	}
*/
	//gsh
	if( ! s_registering )
	{
		//if it couldn't be found and we tried finding it in sod
		//then it might exist in wolf3d
		if( !Sound_LoadSound( sfx ) && isSpearSound)
		{
			sfx = Sound_FindSound( name );		
			//Com_Printf("Finding Sound Again: %s\n", name);
			
			if( ! s_registering )
				Sound_LoadSound( sfx );  //try loading again
		}
	}
	
	return sfx;
}

PUBLIC void Sound_EndRegistration( void )
{

	s_registering = false;
}


