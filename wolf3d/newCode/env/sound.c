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
 *	sound.c:
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


#define MAX_PLAYSOUNDS		128

#define MAX_CHANNELS		64

PRIVATE playSound_t	s_playSounds[ MAX_PLAYSOUNDS ];
PRIVATE playSound_t	s_freePlaySounds;
PRIVATE playSound_t	s_pendingPlaySounds;

PRIVATE channel_t	s_channels[ MAX_CHANNELS ];
PRIVATE int			s_numChannels;

PRIVATE listener_t	s_listener;

PRIVATE int			s_frameCount;

PRIVATE _boolean	s_activeApp;

_boolean			sound_initialized = false;

cvar_t	*s_initSound;
cvar_t	*s_masterVolume;
cvar_t	*s_sfxVolume;
cvar_t	*s_musicVolume;
cvar_t	*s_minDistance;
cvar_t	*s_maxDistance;
cvar_t	*s_rolloffFactor;
cvar_t	*s_dopplerFactor;
cvar_t	*s_dopplerVelocity;



/////////////////////////////////////////////////////////////////////
//
//	Sound Channels
//
/////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: Sound_AllocChannels -Allocate sound channels. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Sound_AllocChannels( void )
{
	channel_t	*ch;
	int			i;

	for( i = 0, ch = s_channels ; i < MAX_CHANNELS ; ++i, ++ch )
	{
		pfalGenSources( 1, &ch->sourceName );

		if( pfalGetError() != AL_NO_ERROR )
		{
			break;
		}

		s_numChannels++;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Sound_FreeChannels -Free sound channels. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Sound_FreeChannels( void )
{
	channel_t	*ch;
	int			i;

	for( i = 0, ch = s_channels; i < s_numChannels; i++, ch++)
	{
		pfalDeleteSources( 1, &ch->sourceName );

		memset( ch, 0, sizeof( *ch ) );
	}

	s_numChannels = 0;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_FreeChannels -Free sound channels. 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE int Sound_ChannelState( channel_t *ch )
{
	int state;

	pfalGetSourcei( ch->sourceName, AL_SOURCE_STATE, &state );

	return state;
}


PRIVATE void Sound_PlayChannel( channel_t *ch, sfx_t *sfx )
{
	ch->sfx = sfx;

	pfalSourcei( ch->sourceName, AL_BUFFER, sfx->bufferNum );
	pfalSourcei( ch->sourceName, AL_LOOPING, ch->loopSound );
	pfalSourcei( ch->sourceName, AL_SOURCE_RELATIVE, AL_FALSE );
	pfalSourcePlay( ch->sourceName );
}


PRIVATE void Sound_StopChannel( channel_t *ch )
{
	ch->sfx = NULL;

	pfalSourceStop( ch->sourceName );
	pfalSourcei( ch->sourceName, AL_BUFFER, 0 );
#ifdef IPHONE 
	// OpenAL on the iphone doesn't seem to stop sounds properly.
	// Maybe regenning the source will work...
	pfalDeleteSources( 1, &ch->sourceName );
	pfalGenSources( 1, &ch->sourceName );
#endif
}


PRIVATE void Sound_SpatializeChannel( channel_t *ch )
{
//	vec3_t	position, velocity;

	// Update position and velocity
	if( ch->entNum == 0 || ! ch->distanceMult )
	{
		pfalSourcefv( ch->sourceName, AL_POSITION, s_listener.position );
		pfalSourcefv( ch->sourceName, AL_VELOCITY, s_listener.velocity );
	}
	else 
	{
		if( ch->fixedPosition )
		{
			pfalSource3f( ch->sourceName, AL_POSITION, ch->position[1], ch->position[2], -ch->position[0] );
			pfalSource3f( ch->sourceName, AL_VELOCITY, 0, 0, 0 );
		}
/*		else 
		{
			if( ch->loopSound )
			{
				Client_GetEntitySoundSpatialization( ch->loopNum, position, velocity );
			}
			else
			{
				Client_GetEntitySoundSpatialization( ch->entNum, position, velocity );
			}

			pfalSource3f( ch->sourceName, AL_POSITION, position[1], position[2], -position[0] );
			pfalSource3f( ch->sourceName, AL_VELOCITY, velocity[1], velocity[2], -velocity[0] );
		}
*/
	}

	// Update min/max distance
	if( ch->distanceMult )
	{
		pfalSourcef( ch->sourceName, AL_REFERENCE_DISTANCE, s_minDistance->value * ch->distanceMult );
	}
	else
	{
		pfalSourcef( ch->sourceName, AL_REFERENCE_DISTANCE, s_maxDistance->value );
	}

	pfalSourcef( ch->sourceName, AL_MAX_DISTANCE, s_maxDistance->value );

	// Update volume and rolloff factor
	pfalSourcef( ch->sourceName, AL_GAIN, s_sfxVolume->value * ch->volume );
	pfalSourcef( ch->sourceName, AL_ROLLOFF_FACTOR, s_rolloffFactor->value );
}


/*
-----------------------------------------------------------------------------
 Function: Sound_PickChannel -
 
 Parameters:
 
 Returns: 
 
 Notes: 
	Tries to find a free channel, or tries to replace an active channel.
-----------------------------------------------------------------------------
*/
PUBLIC channel_t *Sound_PickChannel( W32 entNum, W32 entChannel )
{
	channel_t	*ch;
	int			i;
	int			firstToDie = -1;
	int			oldestTime = iphoneFrameNum;

	for( i = 0, ch = s_channels ; i < s_numChannels ; ++i, ++ch )
	{
		// Don't let game sounds override streaming sounds
		if( ch->streaming )
		{
			continue;
		}

		// Check if this channel is active
		if( ! ch->sfx )
		{
			// Free channel
			firstToDie = i;
			break;
		}

		// Channel 0 never overrides
		if( entChannel != 0 && (ch->entNum == entNum && ch->entChannel == entChannel ) )
		{
			// Always override sound from same entity
			firstToDie = i;
			break;
		}

		// Replace the oldest sound
		if( ch->startTime < oldestTime )
		{
			oldestTime = ch->startTime;
			firstToDie = i;
		}
	}

	if( firstToDie == -1 )
	{
		return NULL;
	}

	ch = &s_channels[ firstToDie ];
	
	ch->entNum = entNum;
	ch->entChannel = entChannel;
	ch->startTime = iphoneFrameNum;

	// Make sure this channel is stopped
	Sound_StopChannel( ch );

	return ch;
}

/////////////////////////////////////////////////////////////////////
//	End of Sound Channels
/////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: Sound_AddLoopingSounds -
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
	Entities with a a->sound field will generate looping sounds that are
	automatically started and stopped as the entities are sent to the
	client.
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_AddLoopingSounds( void )
{
	
}



PRIVATE playSound_t *Sound_AllocPlaySound( void )
{
	playSound_t	*ps;

	ps = s_freePlaySounds.next;
	if( ps == &s_freePlaySounds )
	{
		return NULL;		// No free playSounds
	}

	ps->prev->next = ps->next;
	ps->next->prev = ps->prev;

	return ps;
}


PRIVATE void Sound_FreePlaySound( playSound_t *ps )
{
	ps->prev->next = ps->next;
	ps->next->prev = ps->prev;

	// Add to free list
	ps->next = s_freePlaySounds.next;
	s_freePlaySounds.next->prev = ps;
	ps->prev = &s_freePlaySounds;
	s_freePlaySounds.next = ps;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_IssuePlaySounds -
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 
	Take the next playsound and begin it on the channel.
	This is never called directly by Sound_StartSound*, but only by the update loop.
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_IssuePlaySounds( void )
{
	playSound_t	*ps;
	channel_t	*ch;

	while( 1 )
	{
		ps = s_pendingPlaySounds.next;
		if( ps == &s_pendingPlaySounds )
		{
			break;		// No more pending playSounds
		}

		if( ps->beginTime > iphoneFrameNum )
		{
			break;		// No more pending playSounds this frame
		}

		// Pick a channel and start the sound effect
		ch = Sound_PickChannel( ps->entNum, ps->entChannel );
		if( ! ch )
		{
			if( ps->sfx->name[ 0 ] == '#' )
				Com_DPrintf( "Dropped sound %s\n", &ps->sfx->name[1]);
			else
				Com_DPrintf( "Dropped sound sound/%s\n", ps->sfx->name);

			Sound_FreePlaySound( ps );
			continue;
		}

		ch->loopSound = false;
		ch->fixedPosition = ps->fixedPosition;
		vectorCopy( ps->position, ch->position );
		ch->volume = ps->volume;

		if( ps->attenuation != ATTN_NONE )
		{
			ch->distanceMult = 1.0f / ps->attenuation;
		}
		else
		{
			ch->distanceMult = 0.0;
		}

		Sound_SpatializeChannel( ch );

		Sound_PlayChannel( ch, ps->sfx );

		// Free the playSound
		Sound_FreePlaySound( ps );
	}
}


/*
-----------------------------------------------------------------------------
 Function: Sound_StartSound -
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 
	Validates the parms and queues the sound up.
	If origin is NULL, the sound will be dynamically sourced from the
	entity.
	entChannel 0 will never override a playing sound.
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_StartSound( const vec3_t position, int entNum, int entChannel, sfx_t *sfx, float volume, float attenuation, int timeOfs )
{
	playSound_t	*ps, *sort;

	if( ! sound_initialized )
	{
		return;
	}

	if( ! sfx )
	{
		return;
	}


	// Make sure the sound is loaded
	if( ! Sound_LoadSound( sfx ) )
	{
		return;
	}

	// Allocate a playSound
	ps = Sound_AllocPlaySound();
	if( ! ps )
	{
		if( sfx->name[0] == '#' )
			Com_DPrintf( "Dropped sound %s\n", &sfx->name[1] );
		else
			Com_DPrintf( "Dropped sound sound/%s\n", sfx->name);

		return;
	}

	ps->sfx = sfx;
	ps->entNum = entNum;
	ps->entChannel = entChannel;

	if( position )
	{
		ps->fixedPosition = true;
		vectorCopy( position, ps->position );
	}
	else
	{
		ps->fixedPosition = false;
	}

	ps->volume = volume;
	ps->attenuation = attenuation;
	ps->beginTime = iphoneFrameNum;

	// Sort into the pending playSounds list
	for( sort = s_pendingPlaySounds.next ; sort != &s_pendingPlaySounds && sort->beginTime < ps->beginTime ; sort = sort->next )
	{
		;
	}

	ps->next = sort;
	ps->prev = sort->prev;

	ps->next->prev = ps;
	ps->prev->next = ps;
}


PUBLIC void Sound_StartLocalSound( const char *filename )
{
	sfx_t	*sfx;

	if( ! sound_initialized )
	{
		return;
	}

	sfx = Sound_RegisterSound( filename );
	if( ! sfx )
	{
		Com_Printf( "Sound_StartLocalSound: could not cache (%s)\n", filename );
		return;
	}
		
	Sound_StartSound( NULL, 0, 0, sfx, 1, ATTN_NONE, 0 );
}


PUBLIC void Sound_StopAllSounds( void )
{
	channel_t	*ch;
	int			i;

	if( ! sound_initialized )
	{
		return;
	}

	// Clear all the playSounds
	memset( s_playSounds, 0, sizeof( s_playSounds ) );

	s_freePlaySounds.next = s_freePlaySounds.prev = &s_freePlaySounds;
	s_pendingPlaySounds.next = s_pendingPlaySounds.prev = &s_pendingPlaySounds;

	for( i = 0 ; i < MAX_PLAYSOUNDS ; ++i )
	{
		s_playSounds[ i ].prev = &s_freePlaySounds;
		s_playSounds[ i ].next = s_freePlaySounds.next;
		s_playSounds[ i ].prev->next = &s_playSounds[ i ];
		s_playSounds[ i ].next->prev = &s_playSounds[ i ];
	}

	// Stop all the channels
	for( i = 0, ch = s_channels ; i < s_numChannels ; ++i, ++ch )
	{
		if( ! ch->sfx )
		{
			continue;
		}

		Sound_StopChannel( ch );
	}

	// Reset frame count
	s_frameCount = 0;
}


/*
-----------------------------------------------------------------------------
 Function: Sound_Update -
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
	Called once each time through the main loop.
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_Update( const vec3_t position, const vec3_t velocity, const vec3_t at, const vec3_t up)
{
	channel_t	*ch;
	int			i, total = 0;

	if( ! sound_initialized )
	{
		return;
	}

	// Bump frame count
	s_frameCount++;

	// Set up listener
	vectorSet( s_listener.position, position[1], position[2], -position[0] );
	vectorSet( s_listener.velocity, velocity[1], velocity[2], -velocity[0] );
	vectorSet( &s_listener.orientation[0], at[1], -at[2], -at[0] );
	vectorSet( &s_listener.orientation[3], up[1], -up[2], -up[0] );

	pfalListenerfv( AL_POSITION, s_listener.position );
	pfalListenerfv( AL_VELOCITY, s_listener.velocity );
	pfalListenerfv( AL_ORIENTATION, s_listener.orientation );
	pfalListenerf( AL_GAIN, (s_activeApp) ? s_masterVolume->value : 0.0);

	// Set state
	pfalDistanceModel( AL_INVERSE_DISTANCE_CLAMPED );

	pfalDopplerFactor( s_dopplerFactor->value );
	pfalDopplerVelocity( s_dopplerVelocity->value );

	// Stream background track
	Sound_StreamBGTrack();
	
	// Add looping sounds
	Sound_AddLoopingSounds();

	// Issue playSounds
	Sound_IssuePlaySounds();

	// Update spatialization for all sounds
	for( i = 0, ch = s_channels ; i < s_numChannels ; ++i, ++ch )
	{
		if( ! ch->sfx )
		{
			continue;		// Not active
		}

		// Check for stop
		if( ch->loopSound )
		{
			if( ch->loopFrame != s_frameCount )
			{
				Sound_StopChannel( ch );

				continue;
			}
		}
		else 
		{
			if( Sound_ChannelState(ch) == AL_STOPPED )
			{
				Sound_StopChannel( ch );

				continue;
			}
		}

		// Respatialize channel
		Sound_SpatializeChannel( ch );

		total++;
	}


}


PUBLIC void Sound_Activate( _boolean active )
{
	s_activeApp = active;

	if( ! sound_initialized )
	{
		return;
	}

	pfalListenerf( AL_GAIN, ( active ) ? s_masterVolume->value : 0.0 );

}


/////////////////////////////////////////////////////////////////////
//
//	Console Commands
//
/////////////////////////////////////////////////////////////////////

PRIVATE void Sound_Play_f( void )
{

	int 	i = 1;
	char	name[ MAX_GAMEPATH ];

	if( Cmd_Argc() == 1 )
	{
		Com_Printf( "Usage: play <soundfile>\n" );
		return;
	}
	
	while( i < Cmd_Argc() )
	{
		my_strlcpy( name, Cmd_Argv( i ), sizeof( name ) );

		Sound_StartLocalSound( name );

		i++;
	}
}

PRIVATE void Sound_StopSound_f( void )
{
	Sound_StopAllSounds();
}


/*
-----------------------------------------------------------------------------
 Function: Sound_Restart_f -Console function to restart the sound module.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
	Restart the sound subsystem so it can pick up new parameters and flush
	all sounds.
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_Restart_f( void )
{
	Sound_Shutdown();
	Sound_Init();
}

/////////////////////////////////////////////////////////////////////
//	End of Console Commands
/////////////////////////////////////////////////////////////////////


extern void Sound_SoundList_f( void );


PRIVATE void Sound_Register( void )
{
	s_initSound = Cvar_Get( "s_initSound", "1", CVAR_INIT );	
	s_masterVolume	= Cvar_Get( "s_masterVolume", "1.0", CVAR_ARCHIVE );
	s_sfxVolume		= Cvar_Get( "s_sfxVolume", "1.0", CVAR_ARCHIVE );
	s_musicVolume	= Cvar_Get( "s_musicVolume", "1.0", CVAR_ARCHIVE );
	s_minDistance	= Cvar_Get( "s_minDistance", "0.0", CVAR_ARCHIVE );
	s_maxDistance	= Cvar_Get( "s_maxDistance", "1.0", CVAR_ARCHIVE );
	s_rolloffFactor = Cvar_Get( "s_rolloffFactor", "1.0", CVAR_ARCHIVE );
	s_dopplerFactor = Cvar_Get( "s_dopplerFactor", "1.0", CVAR_ARCHIVE );
	s_dopplerVelocity = Cvar_Get( "s_dopplerVelocity", "0.0", CVAR_ARCHIVE );

	Cmd_AddCommand( "play", Sound_Play_f );
	Cmd_AddCommand( "stopsound", Sound_StopSound_f );
	Cmd_AddCommand( "listSounds", Sound_SoundList_f );
	Cmd_AddCommand( "snd_restart", Sound_Restart_f );
}


PUBLIC void Sound_Init( void )
{
	Com_Printf( "\n------- Sound Initialization -------\n" );

	
	Sound_Register();


	if( ! Sound_Device_Setup() )
	{
		Com_Printf( "------------------------------------\n" );
		return;
	}

	sound_initialized = true;

	Sound_AllocChannels();
	Sound_StopAllSounds();


	Com_Printf( "------------------------------------\n" );
}


PUBLIC void Sound_Shutdown( void )
{
	Cmd_RemoveCommand( "play" );
	Cmd_RemoveCommand( "stopsound" );
	Cmd_RemoveCommand( "listSounds" );
	Cmd_RemoveCommand( "snd_restart" );

	if( ! sound_initialized )
	{
		return;
	}

	Sound_FreeSounds();
	Sound_FreeChannels();

	Sound_Device_Shutdown();

	sound_initialized = false;
}
