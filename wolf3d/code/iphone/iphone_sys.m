/*
 
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 

 This file is part of the WOLF3D iOS v2.1 GPL Source Code. 

 
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
#include "iphone_store.h"

#import "wolf3dAppDelegate.h"

#import <AudioToolbox/AudioServices.h>
#import <UIKit/UIDevice.h>
#import <Foundation/NSUserDefaults.h>

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

	AudioSessionPropertyID  propOtherAudioIsPlaying = kAudioSessionProperty_OtherAudioIsPlaying;
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

/*
 ==================
 SysIPhoneGetContentVersion
 
 Checks the NSUserDefaults and returns true if the user has purchased the Platinum edition.
 ==================
 */
contentVersion_t SysIPhoneGetContentVersion(void) {
#ifndef LITE
	return CONTENT_PLATINUM;
#else

	// For the "Lite" app, we must check the user defaults to see if the user has purchased
	// the platinum edition.
	
	bool platinum = InAppPurchaseHasPurchased( PLATINUM_UPGRADE_STRING );
	
	if ( platinum ) {
		return CONTENT_PLATINUM;
	}
	
	return CONTENT_LITE;

#endif
}

/*
 ==================
 SysIPhoneGetPathToMainBundleLength
 
 Fills in the string
 ==================
 */
int SysIPhoneGetPathToMainBundleLength( void )  {
	NSBundle *mainBundle = [NSBundle mainBundle];
	
	NSString *path = [mainBundle bundlePath];
	
	return [path length];

}

/*
 ==================
 SysIPhoneGetPathToMainBundle
 
 Fills in the string outPath with the path to the main bundle. If the path is longer than
 maxLength, it will be truncated. Call SysIPhoneGetPathToMainBundleLength to get the string
 size needed without truncation.
 ==================
 */
void SysIPhoneGetPathToMainBundle( char * outPath, int maxLength ) {
	NSBundle *mainBundle = [NSBundle mainBundle];
	
	NSString *path = [mainBundle bundlePath];
	
	[path getCString:outPath maxLength:maxLength encoding:NSASCIIStringEncoding];

}

/*
 ==================
 iphoneRotateForLandscape
 
 ==================
 */
void iphoneRotateForLandscape() {
	switch ( deviceOrientation ) {
		case ORIENTATION_LANDSCAPE_LEFT:
			pfglRotatef( 90.0f, 0.0f, 0.0f, 1.0f );
			break;
		default:
			pfglRotatef( -90.0f, 0.0f, 0.0f, 1.0f );
			break;
	}
}

/*
 ==================
 iphoneTouchEvent
 
 ==================
 */
void iphoneTouchEvent( int _numTouches, int _touches[16] ) {
	numTouches = _numTouches;
	for ( int i = 0 ; i < numTouches ; i++ ) {
		switch( deviceOrientation ) {
			case ORIENTATION_LANDSCAPE_RIGHT:
				touches[i][0] = _touches[i*2+1] * touchCoordinateScale;
				touches[i][1] = ( (viddef.height / touchCoordinateScale ) - 1 - _touches[i*2+0]) * touchCoordinateScale;
				break;
				
			default:
				touches[i][0] = ( ( viddef.width / touchCoordinateScale ) - 1 - _touches[i*2+1]) * touchCoordinateScale;
				touches[i][1] = _touches[i*2+0] * touchCoordinateScale;
				break;
		}
	}
}

/*
 ==================
 iphonePromptToBuyPlatinum
 
 ==================
 */
void iphonePromptToBuyPlatinum() {
	currentYesNoBox = YESNO_BUY_PLATINUM;
	iphoneYesNoBox( "Purchase Platinum", "This level is not available in "
										 "Wolfenstein 3D Lite. Would you like to upgrade to "
										 "Platinum?" );
}



/*
 ==================
 LoadPNG
 
 Uses the iPhone's Core Graphics library to easily load PNG files.
 ==================
 */
void LoadPNG( const char *filename, W8 **pic, W16 *outWidth, W16 *outHeight, W16 *bytes ) {
	CGImageRef spriteImage;
	CGContextRef spriteContext;
	
	GLubyte *spriteData = NULL;
	size_t width = 0;
	size_t height = 0;
	size_t bytesPerPixel = 0;
	
	// Get just the filename from the path passed in.
	const char * lastSeparator = strrchr( filename, '/' );
	
	const char * filenameOnly = filename;
	
	if ( lastSeparator ) {
		filenameOnly = lastSeparator + 1;
	}
	
	// Creates a Core Graphics image from an image file
	spriteImage = [UIImage imageNamed:[NSString stringWithCString:filenameOnly encoding:NSASCIIStringEncoding]].CGImage;
	// Get the width and height of the image
	width = CGImageGetWidth(spriteImage);
	height = CGImageGetHeight(spriteImage);
	bytesPerPixel = CGImageGetBitsPerPixel(spriteImage) / 8;

	// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
	// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.

	// HACK to fix alpha/non-alpha images.
	// If an image has alpha, it will be 4 bytes per pixel.
	// If an image doesn't have alpha, it will be 3 bytes per pixel.
	CGImageAlphaInfo alpha = kCGImageAlphaPremultipliedLast;
	size_t byteWidth = width * 4;
	if ( bytesPerPixel == 4 ) {
		alpha = kCGImageAlphaPremultipliedLast;
		byteWidth = width * 4;
	}

	if(spriteImage) {
		// Allocated memory needed for the bitmap context
		spriteData = (GLubyte *) MM_CALLOC(width * height * 4, sizeof(GLubyte));
		// Uses the bitmap creation function provided by the Core Graphics framework. 
		spriteContext = CGBitmapContextCreate(spriteData, width, height,
											  8,
											  byteWidth,
											  CGImageGetColorSpace(spriteImage),
											  alpha);
		
		if ( spriteContext ) {
			// After you create the context, you can draw the sprite image to the context.
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
			// You don't need the context at this point, so you need to release it to avoid memory leaks.
			CGContextRelease(spriteContext);
			
			if ( outWidth ) {
				*outWidth = width;
			}
			if ( outHeight ) {
				*outHeight = height;
			}
			if ( pic ) {
				*pic = spriteData;
			}
			if ( bytes ) {
				*bytes = bytesPerPixel;
			}
			return;
		}
	}
	
	if ( outWidth ) {
		*outWidth = 0;
	}
	if ( outHeight ) {
		*outHeight = 0;
	}
	if ( pic ) {
		*pic = NULL;
	}
	if ( bytes ) {
		*bytes = 0;
	}
	return;
}

/*
 ==================
 iphoneGetDeviceType
 
 Uses the iPhone's Core Graphics library to easily load PNG files.
 ==================
 */
deviceType_t iphoneGetDeviceType(void) {
	deviceType_t result = DEVICE_UNKNOWN;

	switch ( UI_USER_INTERFACE_IDIOM() ) {
		case UIUserInterfaceIdiomPhone:
			result = DEVICE_PHONE;
			break;
			
		case UIUserInterfaceIdiomPad:
			result = DEVICE_TABLET;
			break;

		default:
			break;
	}
	
	return result;
}

/*
 ==================
 iphoneInitMenuMusic
 ==================
 */
void iphoneInitMenuMusic() {

	wolf3dAppDelegate * app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	
	[app initMenuMusicPlayer];

}

/*
 ==================
 iphoneStartMenuMusic
 ==================
 */
void iphoneStartMenuMusic() {
	wolf3dAppDelegate * app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	
	[app startMenuMusic];
}


/*
 ==================
 iphoneStopMenuMusic
 ==================
 */

void iphoneStopMenuMusic() {
	wolf3dAppDelegate * app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	
	[app stopMenuMusic];
}



