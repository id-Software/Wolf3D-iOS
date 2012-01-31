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
 *	openal_binding.h: Interface to OpenAL library.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	
 *	Date:	2004
 *
 */


#ifndef __OPENAL_BINDING_H__
#define __OPENAL_BINDING_H__

extern _boolean OpenAL_Init( const char *dllname );
extern void OpenAL_Shutdown( void );



typedef ALCAPI ALCvoid ( * ALCCLOSEDEVICE)(ALCdevice *pDevice);
typedef ALCAPI ALCcontext*( * ALCCREATECONTEXT)(ALCdevice *device, const ALCint *attrList);
typedef ALCAPI ALCvoid ( * ALCDESTROYCONTEXT)(ALCcontext *context);
typedef ALCAPI ALCdevice* ( * ALCGETCONTEXTSDEVICE)(ALCcontext *pContext);
typedef ALCAPI ALCcontext * ( * ALCGETCURRENTCONTEXT)(ALCvoid);
typedef ALCAPI ALCenum ( * ALCGETENUMVALUE)(ALCdevice *device, const ALCubyte *enumName);
typedef ALCAPI ALCenum ( * ALCGETERROR)(ALCdevice *device);
typedef ALCAPI ALCvoid ( * ALCGETINTEGERV)(ALCdevice *device,ALCenum param,ALsizei size,ALCint *data);
typedef ALCAPI ALCvoid *  ( * ALCGETPROCADDRESS)(ALCdevice *device, const ALCubyte *funcName);
typedef ALCAPI const ALCubyte* ( * ALCGETSTRING)(ALCdevice *device,ALCenum param);
typedef ALCAPI ALCboolean ( * ALCISEXTENSIONPRESENT)(ALCdevice *device, const ALCubyte *extName);
typedef ALCAPI ALCboolean ( * ALCMAKECONTEXTCURRENT)(ALCcontext *context);
typedef ALCAPI ALCdevice* ( * ALCOPENDEVICE)(const ALCubyte *deviceName);
typedef ALCAPI ALCvoid ( * ALCPROCESSCONTEXT)(ALCcontext *pContext);
typedef ALCAPI ALCvoid ( * ALCSUSPENDCONTEXT)(ALCcontext *pContext);



typedef ALCAPI ALvoid ( * ALGENBUFFERS)(ALsizei n, ALuint* bufferNames);
typedef ALCAPI ALvoid ( * ALDELETEBUFFERS)(ALsizei n, const ALuint* bufferNames);
typedef ALCAPI ALboolean ( * ALISBUFFER)(ALuint bufferName);
typedef ALCAPI ALvoid ( * ALBUFFERDATA)(ALuint bufferName, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq);
typedef ALCAPI ALvoid ( * ALGETBUFFERF)(ALuint bufferName, ALenum param, ALfloat* value);
typedef ALCAPI ALvoid ( * ALGETBUFFERI)(ALuint bufferName, ALenum param, ALint* value);
typedef ALCAPI ALvoid ( * ALENABLE)(ALenum capability);
typedef ALCAPI ALvoid ( * ALDISABLE)(ALenum capability);
typedef ALCAPI ALvoid ( * ALDOPPLERFACTOR)(ALfloat value);
typedef ALCAPI ALvoid ( * ALDOPPLERVELOCITY)(ALfloat value);
typedef ALCAPI ALvoid ( * ALDISTANCEMODEL)(ALenum value);
typedef ALCAPI ALboolean ( * ALGETBOOLEAN)(ALenum param);
typedef ALCAPI ALvoid ( * ALGETBOOLEANV)(ALenum param, ALboolean* data);
typedef ALCAPI ALdouble ( * ALGETDOUBLE)(ALenum param);
typedef ALCAPI ALvoid ( * ALGETDOUBLEV)(ALenum param, ALdouble* data);
typedef ALCAPI ALfloat ( * ALGETFLOAT)(ALenum param);
typedef ALCAPI ALvoid ( * ALGETFLOATV)(ALenum param, ALfloat* data);
typedef ALCAPI ALint ( * ALGETINTEGER)(ALenum param);
typedef ALCAPI ALvoid ( * ALGETINTEGERV)(ALenum param, ALint* data);
typedef ALCAPI ALenum ( * ALGETENUMVALUE)(const ALubyte* ename);
typedef ALCAPI ALenum ( * ALGETERROR)(ALvoid);
typedef ALCAPI ALvoid* ( * ALGETPROCADDRESS)(const ALubyte* fname);
typedef ALCAPI const ALubyte* ( * ALGETSTRING)(ALenum param);
typedef ALCAPI ALboolean ( * ALISEXTENSIONPRESENT)(const ALubyte* ename);
typedef ALCAPI ALboolean ( * ALISENABLED)(ALenum capability);
typedef ALCAPI ALvoid ( * ALLISTENERF)(ALenum param, ALfloat value);
typedef ALCAPI ALvoid ( * ALLISTENER3F)(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
typedef ALCAPI ALvoid ( * ALLISTENERFV)(ALenum param, const ALfloat* values);
typedef ALCAPI ALvoid ( * ALLISTENERI)(ALenum param, ALint value);
typedef ALCAPI ALvoid ( * ALGETLISTENERF)(ALenum param, ALfloat* value);
typedef ALCAPI ALvoid ( * ALGETLISTENER3F)(ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
typedef ALCAPI ALvoid ( * ALGETLISTENERFV)(ALenum param, ALfloat* values);
typedef ALCAPI ALvoid ( * ALGETLISTENERI)(ALenum param, ALint* value);
typedef ALCAPI ALvoid ( * ALGENSOURCES)(ALsizei n, ALuint* sourceNames);
typedef ALCAPI ALvoid ( * ALDELETESOURCES)(ALsizei n, const ALuint* sourceNames);
typedef ALCAPI ALboolean ( * ALISSOURCE)(ALuint sourceName);
typedef ALCAPI ALvoid ( * ALSOURCEF)(ALuint sourceName, ALenum param, ALfloat value);
typedef ALCAPI ALvoid ( * ALSOURCEFV)(ALuint sourceName, ALenum param, const ALfloat* values);
typedef ALCAPI ALvoid ( * ALSOURCE3F)(ALuint sourceName, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
typedef ALCAPI ALvoid ( * ALSOURCEI)(ALuint sourceName, ALenum param, ALint value);
typedef ALCAPI ALvoid ( * ALGETSOURCEF)(ALuint sourceName, ALenum param, ALfloat* value);
typedef ALCAPI ALvoid ( * ALGETSOURCE3F)(ALuint sourceName, ALenum param, ALfloat* v1, ALfloat* v2, ALfloat* v3);
typedef ALCAPI ALvoid ( * ALGETSOURCEFV)(ALuint sourceName, ALenum param, ALfloat* values);
typedef ALCAPI ALvoid ( * ALGETSOURCEI)(ALuint sourceName, ALenum param, ALint* value);
typedef ALCAPI ALvoid ( * ALSOURCEPLAY)(ALuint sourceName);
typedef ALCAPI ALvoid ( * ALSOURCEPLAYV)(ALsizei n, const ALuint* sourceNames);
typedef ALCAPI ALvoid ( * ALSOURCEPAUSE)(ALuint sourceName);
typedef ALCAPI ALvoid ( * ALSOURCEPAUSEV)(ALsizei n, const ALuint* sourceNames);
typedef ALCAPI ALvoid ( * ALSOURCESTOP)(ALuint sourceName);
typedef ALCAPI ALvoid ( * ALSOURCESTOPV)(ALsizei n, const ALuint* sourceNames);
typedef ALCAPI ALvoid ( * ALSOURCEREWIND)(ALuint sourceName);
typedef ALCAPI ALvoid ( * ALSOURCEREWINDV)(ALsizei n, const ALuint* sourceNames);
typedef ALCAPI ALvoid ( * ALSOURCEQUEUEBUFFERS)(ALuint sourceName, ALsizei n, const ALuint* buffers);
typedef ALCAPI ALvoid ( * ALSOURCEUNQUEUEBUFFERS)(ALuint sourceName, ALsizei n, ALuint* buffers);



ALCCLOSEDEVICE			pfalcCloseDevice;			
ALCCREATECONTEXT		pfalcCreateContext;
ALCDESTROYCONTEXT		pfalcDestroyContext;			
ALCGETCONTEXTSDEVICE	pfalcGetContextsDevice;
ALCGETCURRENTCONTEXT	pfalcGetCurrentContext;		
ALCGETENUMVALUE			pfalcGetEnumValue;		
ALCGETERROR				pfalcGetError;			
ALCGETINTEGERV			pfalcGetIntegerv;
ALCGETPROCADDRESS		pfalcGetProcAddress;
ALCGETSTRING			pfalcGetString;		
ALCISEXTENSIONPRESENT	pfalcIsExtensionPresent;
ALCMAKECONTEXTCURRENT	pfalcMakeContextCurrent;	
ALCOPENDEVICE			pfalcOpenDevice;	
ALCPROCESSCONTEXT		pfalcProcessContext;
ALCSUSPENDCONTEXT		pfalcSuspendContext;

ALGENBUFFERS			pfalGenBuffers;			
ALDELETEBUFFERS			pfalDeleteBuffers;
ALISBUFFER				pfalIsBuffer;			
ALBUFFERDATA			pfalBufferData;
ALGETBUFFERF			pfalGetBufferf;				
ALGETBUFFERI			pfalGetBufferi;				
ALENABLE				pfalEnable;				
ALDISABLE				pfalDisable;					
ALDOPPLERFACTOR			pfalDopplerFactor;
ALDOPPLERVELOCITY		pfalDopplerVelocity;
ALDISTANCEMODEL			pfalDistanceModel;		
ALGETBOOLEAN			pfalGetBoolean;			
ALGETBOOLEANV			pfalGetBooleanv;				
ALGETDOUBLE				pfalGetDouble;			
ALGETDOUBLEV			pfalGetDoublev;				
ALGETFLOAT				pfalGetFloat;				
ALGETFLOATV				pfalGetFloatv;				
ALGETINTEGER			pfalGetInteger;				
ALGETINTEGERV			pfalGetIntegerv;				
ALGETENUMVALUE			pfalGetEnumValue;			
ALGETERROR				pfalGetError;			
ALGETPROCADDRESS		pfalGetProcAddress;
ALGETSTRING				pfalGetString;			
ALISEXTENSIONPRESENT	pfalIsExtensionPresent;
ALISENABLED				pfalIsEnabled;		
ALLISTENERF				pfalListenerf;				
ALLISTENER3F			pfalListener3f;				
ALLISTENERFV			pfalListenerfv;				
ALLISTENERI				pfalListeneri;				
ALGETLISTENERF			pfalGetListenerf;
ALGETLISTENER3F			pfalGetListener3f;			
ALGETLISTENERFV			pfalGetListenerfv;			
ALGETLISTENERI			pfalGetListeneri;			
ALGENSOURCES			pfalGenSources;			
ALDELETESOURCES			pfalDeleteSources;
ALISSOURCE				pfalIsSource;			
ALSOURCEF				pfalSourcef;				
ALSOURCEFV				pfalSourcefv;				
ALSOURCE3F				pfalSource3f;				
ALSOURCEI				pfalSourcei;				
ALGETSOURCEF			pfalGetSourcef;
ALGETSOURCE3F			pfalGetSource3f;				
ALGETSOURCEFV			pfalGetSourcefv;			
ALGETSOURCEI			pfalGetSourcei;			
ALSOURCEPLAY			pfalSourcePlay;				
ALSOURCEPLAYV			pfalSourcePlayv;				
ALSOURCEPAUSE			pfalSourcePause;			
ALSOURCEPAUSEV			pfalSourcePausev;			
ALSOURCESTOP			pfalSourceStop;			
ALSOURCESTOPV			pfalSourceStopv;				
ALSOURCEREWIND			pfalSourceRewind;			
ALSOURCEREWINDV			pfalSourceRewindv;			
ALSOURCEQUEUEBUFFERS	pfalSourceQueueBuffers;
ALSOURCEUNQUEUEBUFFERS	pfalSourceUnqueueBuffers;



#endif /* __OPENAL_BINDING_H__ */
