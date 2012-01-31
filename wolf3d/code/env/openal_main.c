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
 *	openal_main.c:   Interface to Sound Device.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 */

#include "../wolfiphone.h"


#ifdef _WIN32

	#define OPENAL_DLL_NAME		"openal32.dll"

#elif __unix__

	#define OPENAL_DLL_NAME		"libopenal.so"

#elif IPHONE

#define OPENAL_DLL_NAME		"/System/Library/Frameworks/OpenAL.framework/OpenAL"

#else

	#error "Please define OPENAL_DLL_NAME"

#endif

PRIVATE ALCcontext *Context;
PRIVATE ALCdevice *Device;


cvar_t  *s_driver;
cvar_t  *s_device;


char *deviceList;
char *sound_devices[ 12 ];

W16 numSoundDevices, numDefaultSoundDevice;



/*
-----------------------------------------------------------------------------
 Function: Sound_Device_getDeviceList -Get OpenAL device list.
 
 Parameters: Nothing.            
 
 Returns: Nothing.
 
 Notes:
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_Device_getDeviceList( void )
{
	char deviceName[ 256 ];
	ALboolean isExtensionPresent = AL_FALSE;
	
	my_strlcpy( deviceName, s_device->string, sizeof( deviceName ) );
	isExtensionPresent = pfalcIsExtensionPresent( NULL, (ALubyte*)"ALC_ENUMERATION_EXT");
	ALC_CheckErrors();
	if( isExtensionPresent == AL_TRUE ) 
	{	
		// try out enumeration extension
		deviceList = (char *)pfalcGetString( NULL, ALC_DEVICE_SPECIFIER );
		ALC_CheckErrors();
		for( numSoundDevices = 0 ; numSoundDevices < 12 ; ++numSoundDevices ) 
		{
			sound_devices[ numSoundDevices ] = NULL;
		}

		for( numSoundDevices = 0 ; numSoundDevices < 12 ; ++numSoundDevices )
		{
			sound_devices[ numSoundDevices ] = deviceList;
			if( strcmp( sound_devices[ numSoundDevices ], deviceName ) == 0 )
			{
				numDefaultSoundDevice = numSoundDevices;
			}
			deviceList += strlen( deviceList );
			if( deviceList[ 0 ] == 0 )
			{
				if( deviceList[ 1 ] == 0 )
				{
					break;
				} 
				else 
				{
					deviceList += 1;
				}
			}

		} // End for numSoundDevices = 0 ; numSoundDevices < 12 ; ++numSoundDevices
	}

}


/*
-----------------------------------------------------------------------------
 Function: Sound_Device_Register -Register OpenAL cvars.
 
 Parameters: Nothing.            
 
 Returns: Nothing.
 
 Notes:
-----------------------------------------------------------------------------
*/
PRIVATE void Sound_Device_Register( void )
{
	s_driver = Cvar_Get( "s_driver", OPENAL_DLL_NAME, CVAR_ARCHIVE );
	s_device = Cvar_Get( "s_device", "", CVAR_LATCH | CVAR_ARCHIVE );
}

/*
-----------------------------------------------------------------------------
 Function: Sound_Device_Setup -Setup OpenAL sound device.
 
 Parameters: Nothing.            
 
 Returns: true on success, otherwise false.
 
 Notes: Call Sound_Device_Shutdown() when you are done.
-----------------------------------------------------------------------------
*/
PUBLIC _boolean Sound_Device_Setup( void )
{
	Com_Printf( "...Initializing AudioSession\n" );
	SysIPhoneInitAudioSession();
	
	Com_Printf( "...Initializing OpenAL subsystem\n" );

	Sound_Device_Register();

	// Initialize our OpenAL dynamic bindings
	if( ! OpenAL_Init( s_driver->string ) )
	{		
		Com_Printf( "[%s]: Dynamic binding of (%s) failed\n", "openal_main.c", s_driver->string );

		goto failed;
	}

	
	Sound_Device_getDeviceList();


	Device = pfalcOpenDevice( (ALCubyte *)( (s_device->string[ 0 ]) ? s_device->string : NULL ) );
	if( Device == NULL )
	{
		Com_Printf( "Failed to Initialize OpenAL\n" );

		goto failed;
	}

	// Create context(s)
	Context = pfalcCreateContext( Device, NULL );
	if( Context == NULL )
	{
		Com_Printf( "Failed to initialize OpenAL\n" );

		goto failed;
	}

	

	// Set active context
	pfalcGetError( Device );
	pfalcMakeContextCurrent( Context );
	if( pfalcGetError( Device ) != ALC_NO_ERROR )
	{
		Com_Printf( "Failed to Make Context Current\n" );
		
		goto failed;
	}

	return true;



failed:

	if( Context )
	{
		pfalcDestroyContext( Context );
		ALC_CheckErrors();
		Context = NULL;
	}

	if( Device )
	{
		pfalcCloseDevice( Device );
		ALC_CheckErrors();
		Device = NULL;
	}
	
	OpenAL_Shutdown();

	return false;
}

/*
-----------------------------------------------------------------------------
 Function: Sound_Device_Shutdown -Shutdown OpenAL sound device.
 
 Parameters: Nothing.            
 
 Returns: Nothing.
 
 Notes:
-----------------------------------------------------------------------------
*/
PUBLIC void Sound_Device_Shutdown( void )
{
	if( Context )
	{
		pfalcMakeContextCurrent( NULL );
		ALC_CheckErrors();
		pfalcDestroyContext( Context );
		ALC_CheckErrors();

		Context = NULL;
	}

	if( Device )
	{
		pfalcCloseDevice( Device );
		ALC_CheckErrors();

		Device = NULL;
	}

	OpenAL_Shutdown();
}

