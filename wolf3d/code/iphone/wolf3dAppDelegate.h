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

#import <UIKit/UIKit.h>
#import <UIKit/UIAccelerometer.h>
#import <AVFoundation/AVAudioPlayer.h>

#if !TARGET_OS_TV
#import <CoreMotion/CoreMotion.h>
#endif

#import "iphone_store.h"

@class wolf3dViewController;

#if TARGET_OS_TV
@interface wolf3dAppDelegate : NSObject <UIApplicationDelegate> {
#else
@interface wolf3dAppDelegate : NSObject <UIApplicationDelegate, UIAccelerometerDelegate, UIAlertViewDelegate> {
#endif

    UIWindow *window;
	UINavigationController *navigationController;
	wolf3dViewController *viewController;
	int		lastAccelUpdateMsec;
    BOOL    glVisible;

@private	
	UIView *waitingView;
//	UIAlertView *alertPurchaseSpear;
	AVAudioPlayer * player;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet wolf3dViewController *viewController;
@property (nonatomic, retain) UINavigationController *navigationController;
@property (nonatomic, retain) UIView *waitingView;
@property (nonatomic, retain) AVAudioPlayer *player;
#if !TARGET_OS_TV
@property (nonatomic, strong) CMMotionManager * motionManager;
#endif

- (void)initMenuMusicPlayer;

- (void)startMenuMusic;
- (void)stopMenuMusic;


//- (void)restartAccelerometerIfNeeded;
- (void)showOpenGL;
- (void)GLtoMainMenu;
- (void)GLtoPreviousMenu;
- (void)didRotate:(NSNotification *)notification;
- (BOOL)isGLVisible;
#if !TARGET_OS_TV
- (void)setScreenForOrientation:(UIDeviceOrientation)orientation;
#endif
- (NSString*) GetNibNameForDevice:(NSString*) nibName;

- (void)dismissWaitingView;

@end

// Callback for in-app purchase.
void inAppPurchaseCallback( InAppPurchaseResult result );
