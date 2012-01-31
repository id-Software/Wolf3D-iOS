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
 *	openal_binding.c:   Interface to OpenAL library.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 */


#include "../wolfiphone.h"


#ifdef _WIN32

	HINSTANCE hinstOpenAL;

#elif __unix__

	void *OpenALLib;

#elif IPHONE 

	void *OpenALLib;
	// from here on in this file, behave like unix
	#define __unix__  1 

#else

	#error "Please define interface to OpenAL library!"

#endif


/*
-----------------------------------------------------------------------------
 Function: OpenAL_Shutdown -Shutdown interface to OpenAL.
 
 Parameters: Nothing.            
 
 Returns: Nothing.
 
 Notes: 
	Unloads the specified Dynamic Link Library then NULLs out all the
	proc pointers.

-----------------------------------------------------------------------------
*/
PUBLIC void OpenAL_Shutdown( void )
{

#ifdef _WIN32

	if( hinstOpenAL )
	{
		FreeLibrary( hinstOpenAL );
		hinstOpenAL = NULL;
	}

	hinstOpenAL = NULL;

#elif __unix__

	if( OpenALLib )
	{
		dlclose( OpenALLib );
		OpenALLib = NULL;
	}

	OpenALLib = NULL;

#else

	#error "Please define interface to OpenAL library!"

#endif


	pfalcCloseDevice			= NULL;
	pfalcCreateContext			= NULL;
	pfalcDestroyContext			= NULL;
	pfalcGetContextsDevice		= NULL;
	pfalcGetCurrentContext		= NULL;
	pfalcGetEnumValue			= NULL;
	pfalcGetError				= NULL;
	pfalcGetIntegerv			= NULL;
	pfalcGetProcAddress			= NULL;
	pfalcGetString				= NULL;
	pfalcIsExtensionPresent		= NULL;
	pfalcMakeContextCurrent		= NULL;
	pfalcOpenDevice				= NULL;
	pfalcProcessContext			= NULL;
	pfalcSuspendContext			= NULL;


	pfalGenBuffers				= NULL;
	pfalDeleteBuffers			= NULL;
	pfalIsBuffer				= NULL;
	pfalBufferData				= NULL;
	pfalGetBufferf				= NULL;
	pfalGetBufferi				= NULL;
	pfalEnable					= NULL;
	pfalDisable					= NULL;
	pfalDopplerFactor			= NULL;
	pfalDopplerVelocity			= NULL;
	pfalDistanceModel			= NULL;
	pfalGetBoolean				= NULL;
	pfalGetBooleanv				= NULL;
	pfalGetDouble				= NULL;
	pfalGetDoublev				= NULL;
	pfalGetFloat				= NULL;
	pfalGetFloatv				= NULL;
	pfalGetInteger				= NULL;
	pfalGetIntegerv				= NULL;
	pfalGetEnumValue			= NULL;
	pfalGetError				= NULL;
	pfalGetProcAddress			= NULL;
	pfalGetString				= NULL;
	pfalIsExtensionPresent		= NULL;
	pfalIsEnabled				= NULL;
	pfalListenerf				= NULL;
	pfalListener3f				= NULL;
	pfalListenerfv				= NULL;
	pfalListeneri				= NULL;
	pfalGetListenerf			= NULL;
	pfalGetListener3f			= NULL;
	pfalGetListenerfv			= NULL;
	pfalGetListeneri			= NULL;
	pfalGenSources				= NULL;
	pfalDeleteSources			= NULL;
	pfalIsSource				= NULL;
	pfalSourcef					= NULL;
	pfalSourcefv				= NULL;
	pfalSource3f				= NULL;
	pfalSourcei					= NULL;
	pfalGetSourcef				= NULL;
	pfalGetSource3f				= NULL;
	pfalGetSourcefv				= NULL;
	pfalGetSourcei				= NULL;
	pfalSourcePlay				= NULL;
	pfalSourcePlayv				= NULL;
	pfalSourcePause				= NULL;
	pfalSourcePausev			= NULL;
	pfalSourceStop				= NULL;
	pfalSourceStopv				= NULL;
	pfalSourceRewind			= NULL;
	pfalSourceRewindv			= NULL;
	pfalSourceQueueBuffers		= NULL;
	pfalSourceUnqueueBuffers	= NULL;

}



#ifdef _WIN32

	#define GPA( a ) GetProcAddress( hinstOpenAL, a )

#elif __unix__

	#define GPA( a ) dlsym( OpenALLib, a )

#else

	#error "Please define interface to OpenAL library!"

#endif





/*
-----------------------------------------------------------------------------
 Function: OpenAL_Init -Setup interface to OpenAL.
 
 Parameters: dllname -[in] Name of the OpenAL dynamic link library.            
 
 Returns: true on success, otherwise false.
 
 Notes: 
		This is responsible for binding our al function pointers to 
		the appropriate OpenAL stuff. In Windows this means doing a 
		LoadLibrary and a bunch of calls to GetProcAddress. On other
		operating systems we need to do the right thing, whatever that
		might be.

-----------------------------------------------------------------------------
*/
PUBLIC _boolean OpenAL_Init( const char *dllname )
{

#ifdef _WIN32

	char buffer[ 1024 ], *ptr;

	SearchPath( NULL, dllname, NULL, sizeof( buffer ) - 1, buffer, &ptr );

	Com_Printf( "...calling LoadLibrary( %s ): ", buffer );
	
	if( ( hinstOpenAL = LoadLibrary( dllname ) ) == 0 )
	{
		char *buf = NULL;

		Com_Printf( "failed\n" );

		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
						NULL, GetLastError(), 
						MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
						(LPTSTR) &buf, 0, NULL );

		Com_Printf( "%s\n", buf );
		
		return false;
	}	

#elif __unix__

	Com_Printf( "...calling dlopen( %s ): ", dllname );
	
	if( ( OpenALLib = dlopen( dllname, RTLD_LAZY | RTLD_GLOBAL ) ) == 0 )
	{
		Com_Printf( "failed\n" );

		Com_Printf( "%s\n", dlerror() );
		
		return false;
	}

#else

	#error "Please define interface to OpenAL library!"

#endif


	Com_Printf( "succeeded\n" );


	if( ! (pfalcCloseDevice				= (ALCCLOSEDEVICE)GPA( "alcCloseDevice" )) ) return false;
	if( ! (pfalcCreateContext			= (ALCCREATECONTEXT)GPA( "alcCreateContext" )) ) return false;
	if( ! (pfalcDestroyContext			= (ALCDESTROYCONTEXT)GPA( "alcDestroyContext" )) ) return false;
	if( ! (pfalcGetContextsDevice		= (ALCGETCONTEXTSDEVICE)GPA( "alcGetContextsDevice" )) ) return false;
	if( ! (pfalcGetCurrentContext		= (ALCGETCURRENTCONTEXT)GPA( "alcGetCurrentContext" )) ) return false;
	if( ! (pfalcGetEnumValue			= (ALCGETENUMVALUE)GPA( "alcGetEnumValue" )) ) return false;
	if( ! (pfalcGetError				= (ALCGETERROR)GPA( "alcGetError" )) ) return false;
	if( ! (pfalcGetIntegerv				= (ALCGETINTEGERV)GPA( "alcGetIntegerv" )) ) return false;
	if( ! (pfalcGetProcAddress			= (ALCGETPROCADDRESS)GPA( "alcGetProcAddress" )) ) return false;
	if( ! (pfalcGetString				= (ALCGETSTRING)GPA( "alcGetString" )) ) return false;
	if( ! (pfalcIsExtensionPresent		= (ALCISEXTENSIONPRESENT)GPA( "alcIsExtensionPresent" )) ) return false;
	if( ! (pfalcMakeContextCurrent		= (ALCMAKECONTEXTCURRENT)GPA( "alcMakeContextCurrent" )) ) return false;
	if( ! (pfalcOpenDevice				= (ALCOPENDEVICE)GPA( "alcOpenDevice" )) ) return false;
	if( ! (pfalcProcessContext			= (ALCPROCESSCONTEXT)GPA( "alcProcessContext" )) ) return false;
	if( ! (pfalcSuspendContext			= (ALCSUSPENDCONTEXT)GPA( "alcSuspendContext" )) ) return false;


	if( ! (pfalGenBuffers				= (ALGENBUFFERS)GPA("alGenBuffers")) ) return false;
	if( ! (pfalDeleteBuffers			= (ALDELETEBUFFERS)GPA("alDeleteBuffers")) ) return false;
	if( ! (pfalIsBuffer					= (ALISBUFFER)GPA("alIsBuffer")) ) return false;
	if( ! (pfalBufferData				= (ALBUFFERDATA)GPA("alBufferData")) ) return false;
	if( ! (pfalGetBufferf				= (ALGETBUFFERF)GPA("alGetBufferf")) ) return false;
	if( ! (pfalGetBufferi				= (ALGETBUFFERI)GPA("alGetBufferi")) ) return false;
	if( ! (pfalEnable					= (ALENABLE)GPA("alEnable")) ) return false;
	if( ! (pfalDisable					= (ALDISABLE)GPA("alDisable")) ) return false;
	if( ! (pfalDopplerFactor			= (ALDOPPLERFACTOR)GPA("alDopplerFactor")) ) return false;
	if( ! (pfalDopplerVelocity			= (ALDOPPLERVELOCITY)GPA("alDopplerVelocity")) ) return false;
	if( ! (pfalDistanceModel			= (ALDISTANCEMODEL)GPA("alDistanceModel")) ) return false;
	if( ! (pfalGetBoolean				= (ALGETBOOLEAN)GPA("alGetBoolean")) ) return false;
	if( ! (pfalGetBooleanv				= (ALGETBOOLEANV)GPA("alGetBooleanv")) ) return false;
	if( ! (pfalGetDouble				= (ALGETDOUBLE)GPA("alGetDouble")) ) return false;
	if( ! (pfalGetDoublev				= (ALGETDOUBLEV)GPA("alGetDoublev")) ) return false;
	if( ! (pfalGetFloat					= (ALGETFLOAT)GPA("alGetFloat")) ) return false;
	if( ! (pfalGetFloatv				= (ALGETFLOATV)GPA("alGetFloatv")) ) return false;
	if( ! (pfalGetInteger				= (ALGETINTEGER)GPA("alGetInteger")) ) return false;
	if( ! (pfalGetIntegerv				= (ALGETINTEGERV)GPA("alGetIntegerv")) ) return false;
	if( ! (pfalGetEnumValue				= (ALGETENUMVALUE)GPA("alGetEnumValue")) ) return false;
	if( ! (pfalGetError					= (ALGETERROR)GPA("alGetError")) ) return false;
	if( ! (pfalGetProcAddress			= (ALGETPROCADDRESS)GPA("alGetProcAddress")) ) return false;
	if( ! (pfalGetString				= (ALGETSTRING)GPA("alGetString")) ) return false;
	if( ! (pfalIsExtensionPresent		= (ALISEXTENSIONPRESENT)GPA("alIsExtensionPresent")) ) return false;
	if( ! (pfalIsEnabled				= (ALISENABLED)GPA("alIsEnabled")) ) return false;
	if( ! (pfalListenerf				= (ALLISTENERF)GPA("alListenerf")) ) return false;
	if( ! (pfalListener3f				= (ALLISTENER3F)GPA("alListener3f")) ) return false;
	if( ! (pfalListenerfv				= (ALLISTENERFV)GPA("alListenerfv")) ) return false;
	if( ! (pfalListeneri				= (ALLISTENERI)GPA("alListeneri")) ) return false;
	if( ! (pfalGetListenerf				= (ALGETLISTENERF)GPA("alGetListenerf")) ) return false;
	if( ! (pfalGetListener3f			= (ALGETLISTENER3F)GPA("alGetListener3f")) ) return false;
	if( ! (pfalGetListenerfv			= (ALGETLISTENERFV)GPA("alGetListenerfv")) ) return false;
	if( ! (pfalGetListeneri				= (ALGETLISTENERI)GPA("alGetListeneri")) ) return false;
	if( ! (pfalGenSources				= (ALGENSOURCES)GPA("alGenSources")) ) return false;
	if( ! (pfalDeleteSources			= (ALDELETESOURCES)GPA("alDeleteSources")) ) return false;
	if( ! (pfalIsSource					= (ALISSOURCE)GPA("alIsSource")) ) return false;
	if( ! (pfalSourcef					= (ALSOURCEF)GPA("alSourcef")) ) return false;
	if( ! (pfalSourcefv					= (ALSOURCEFV)GPA("alSourcefv")) ) return false;
	if( ! (pfalSource3f					= (ALSOURCE3F)GPA("alSource3f")) ) return false;
	if( ! (pfalSourcei					= (ALSOURCEI)GPA("alSourcei")) ) return false;
	if( ! (pfalGetSourcef				= (ALGETSOURCEF)GPA("alGetSourcef")) ) return false;
	if( ! (pfalGetSource3f				= (ALGETSOURCE3F)GPA("alGetSource3f")) ) return false;
	if( ! (pfalGetSourcefv				= (ALGETSOURCEFV)GPA("alGetSourcefv")) ) return false;
	if( ! (pfalGetSourcei				= (ALGETSOURCEI)GPA("alGetSourcei")) ) return false;
	if( ! (pfalSourcePlay				= (ALSOURCEPLAY)GPA("alSourcePlay")) ) return false;
	if( ! (pfalSourcePlayv				= (ALSOURCEPLAYV)GPA("alSourcePlayv")) ) return false;
	if( ! (pfalSourcePause				= (ALSOURCEPAUSE)GPA("alSourcePause")) ) return false;
	if( ! (pfalSourcePausev				= (ALSOURCEPAUSEV)GPA("alSourcePausev")) ) return false;
	if( ! (pfalSourceStop				= (ALSOURCESTOP)GPA("alSourceStop")) ) return false;
	if( ! (pfalSourceStopv				= (ALSOURCESTOPV)GPA("alSourceStopv")) ) return false;
	if( ! (pfalSourceRewind				= (ALSOURCEREWIND)GPA("alSourceRewind")) ) return false;
	if( ! (pfalSourceRewindv			= (ALSOURCEREWINDV)GPA("alSourceRewindv")) ) return false;
	if( ! (pfalSourceQueueBuffers		= (ALSOURCEQUEUEBUFFERS)GPA("alSourceQueueBuffers")) ) return false;
	if( ! (pfalSourceUnqueueBuffers		= (ALSOURCEUNQUEUEBUFFERS)GPA("alSourceUnqueueBuffers")) ) return false;


	return true;

}

