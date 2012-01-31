/*
 
 Copyright (C) 2009 Id Software, Inc.
 
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

#import <AudioToolbox/AudioServices.h>
//#import <UIDevice.h>

void interruptionListener( void *inUserData, UInt32 inInterruption)
{
	printf("Session interrupted! --- %s ---", inInterruption == kAudioSessionBeginInterruption ? "Begin Interruption" : "End Interruption");
	
	if ( inInterruption == kAudioSessionEndInterruption )
	{
		// make sure we are again the active session
		UInt32 audioCategory = kAudioSessionCategory_AmbientSound;
		AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);
		AudioSessionSetActive( true );
		// do we need to re-initialize the sound subsystem in this case?
	}
}

int otherAudioIsPlaying;

void SysIPhoneInitAudioSession() {
	OSStatus status = 0;
	status = AudioSessionInitialize(NULL, NULL, interruptionListener, NULL);	// else "couldn't initialize audio session"
	UInt32 audioCategory = kAudioSessionCategory_AmbientSound;
	status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);

	UInt32  propOtherAudioIsPlaying = 'othr'; // kAudioSessionProperty_OtherAudioIsPlaying
	UInt32  size = sizeof( otherAudioIsPlaying );
	AudioSessionGetProperty( propOtherAudioIsPlaying, &size, &otherAudioIsPlaying );

	status = AudioSessionSetActive(true);                                       // else "couldn't set audio session active\n"	
} 

int SysIPhoneOtherAudioIsPlaying() {
	static int called = 0;
	if ( called == 0 ) {
		Com_Printf("OtherAudioIsPlaying = %d\n", otherAudioIsPlaying );
		called = 1;
	}
	return otherAudioIsPlaying;
}

char osVersion[32];

const char *SysIPhoneGetOSVersion() {
	static int called = 0;
	if ( called == 0 ) {
		called = 1;
		strcpy( osVersion, [ [ [ UIDevice currentDevice ] systemVersion ] UTF8String ] );
	}

	return osVersion;
}
