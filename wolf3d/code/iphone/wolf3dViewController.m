/*
 
 Copyright (C) 2011 id Software LLC, a ZeniMax Media company. 

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

#import <QuartzCore/QuartzCore.h>

#import "wolf3dViewController.h"
#import "EAGLView.h"
#import "wolfiphone.h"
//#import "wolf3dAppDelegate.h"

#import <GameController/GameController.h>

@interface wolf3dViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@property (strong, nonatomic) GCController *mainController;
@end

@implementation wolf3dViewController

@synthesize animating, context, displayLink, active;


- (id)initWithNibName:(NSString*)file bundle:(NSBundle*)bundle
{
    [super initWithNibName:file bundle:bundle];
    
	// Get the application's window dimensions.
	//EAGLView *glView = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
    
//    CGRect rect;
//    if (self.view.frame.size.width < self.view.frame.size.height)
//        rect = CGRectMake(self.view.frame.origin.x, self.view.frame.origin.y, self.view.frame.size.width, self.view.frame.size.height);
//    else
//        rect = CGRectMake(self.view.frame.origin.y, self.view.frame.origin.x, self.view.frame.size.height, self.view.frame.size.width);
    
    CGRect screenBound = [[UIScreen mainScreen] bounds];
    CGSize screenSize = screenBound.size;
    
    CGFloat screenHeight = screenSize.height;
    CGFloat screenWidth = screenSize.width;
    
    EAGLView *glView = [[EAGLView alloc] initWithFrame:CGRectMake(0, 0, screenWidth, screenHeight)];
    glView.bounds = CGRectMake(0, 0, screenWidth, screenHeight);
    
    NSLog(@"initWithNibName glView.bounds: %f %f", glView.bounds.size.width, glView.bounds.size.height);
    
	self.view = glView;
	[glView release];

    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
	
    animating = FALSE;
    animationFrameInterval = DEFAULT_FRAME_INTERVAL;
    self.displayLink = nil;
    
    // notifications for controller (dis)connect
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(controllerWasConnected:) name:GCControllerDidConnectNotification object:nil];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(controllerWasDisconnected:) name:GCControllerDidDisconnectNotification object:nil];

    if (IS_IPHONE_X) {
        isiPhoneX = true;
    } else {
        isiPhoneX = false;
    }
    
	// Now that we have a context, we can init the render system.
	iphoneStartup();
	
	return self;
}



- (void)awakeFromNib
{
    [super awakeFromNib];
    
//    EAGLView *glView = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
    
//    CGRect rect;
//    if (self.view.frame.size.width < self.view.frame.size.height)
//        rect = CGRectMake(self.view.frame.origin.x, self.view.frame.origin.y, self.view.frame.size.width, self.view.frame.size.height);
//    else
//        rect = CGRectMake(self.view.frame.origin.y, self.view.frame.origin.x, self.view.frame.size.height, self.view.frame.size.width);
//    

    CGRect screenBound = [[UIScreen mainScreen] bounds];
    CGSize screenSize = screenBound.size;
    
    CGFloat screenHeight = screenSize.height;
    CGFloat screenWidth = screenSize.width;
    
    EAGLView *glView = [[EAGLView alloc] initWithFrame:CGRectMake(0, 0, screenWidth, screenHeight)];
    glView.bounds = CGRectMake(0, 0, screenWidth, screenHeight);
    
    NSLog(@"awakeFromNib glView.bounds: %f %f", glView.bounds.size.width, glView.bounds.size.height);
    
	self.view = glView;
	[glView release];
	
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
    animating = FALSE;
    animationFrameInterval = DEFAULT_FRAME_INTERVAL;
    self.displayLink = nil;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    

    [self stopAnimation];
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    self.context = nil;
}

- (void)dealloc {
	// Stop orientation notifications.
	[[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];

	// Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[self setActive:NO];
	
    [super viewWillDisappear:animated];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if ( !animating )
    {		
		// The UIScreen method is not supported pre-4.0.
		// Not worrying about supporting external displays yet, so just create a default display link.
        //CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
		CADisplayLink *aDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if ( animating )
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
	// Check out framerate
	//CFTimeInterval frameTime = [displayLink duration];
	//CFTimeInterval stamp = [displayLink timestamp];
	
	//if ( frameTime < 0.016666 ) {
	//	NSLog( @"Frame timestamp: %f\n", stamp );
	//}
	
	//[ (wolf3dAppDelegate *)[UIApplication sharedApplication].delegate restartAccelerometerIfNeeded];
	
	if ( !active ) {
		// We are in the menus, only need to update sound.
		Sound_Update( vnull, vnull, vnull, vnull );
		return;
	}
	
	picTimingClear();
	
	[(EAGLView *)self.view setFramebuffer];
	
	NSTimeInterval startTime = [NSDate timeIntervalSinceReferenceDate];
	iphoneFrame();
	
	NSTimeInterval gameTime = [NSDate timeIntervalSinceReferenceDate] - startTime;
	if ( gameTime > 0.0166 ) {
/*		NSLog (@"Frame took %.8f seconds.", gameTime);
		NSLog (@"    Sound_Update took %.8f seconds.", soundTime );
		NSLog (@"    iphoneDrawMenus took %.8f seconds.", menuTime );
		soundTime = 0;
		menuTime = 0;
		picTimingPrint();
*/	}

	[(EAGLView *)self.view presentFramebuffer];
	
}

// Clears the renderbuffer and immediately displays it.
- (void)clearAndPresentRenderbuffer {
	qglClear( GL_COLOR_BUFFER_BIT );
	[(EAGLView *)self.view presentFramebuffer];
}

- (void)controllerWasConnected:(NSNotification *)notification {
    
    // a controller was connected
    GCController *controller = (GCController *)notification.object;
    NSString *status = [NSString stringWithFormat:@"Controller connected\nName: %@\n", controller.vendorName];
    NSLog(@"%@", status);
    
    self.mainController = controller;
    
    iPhoneSetControllerConnected(true);
    [self reactToInput];
}

- (void)controllerWasDisconnected:(NSNotification *)notification {
    
    // a controller was disconnected
    GCController *controller = (GCController *)notification.object;
    NSString *status = [NSString stringWithFormat:@"Controller disconnected:\n%@", controller.vendorName];
    NSLog(@"%@", status);

//    self.mainController = nil;
    iPhoneSetControllerConnected(false);
}

- (void)reactToInput {
    
    NSLog(@"%s", "reactToInput");
    
    // register block for input change detection
    GCExtendedGamepad *profile = self.mainController.extendedGamepad;
    profile.valueChangedHandler = ^(GCExtendedGamepad *gamepad, GCControllerElement *element)
    {
        NSString *message = @"";
        CGPoint position = CGPointMake(0, 0);
        
        // left trigger
        if (gamepad.leftTrigger == element && gamepad.leftTrigger.isPressed) {
            message = [message stringByAppendingString:@"Left Trigger"];
        }
        
        // right trigger
        if (gamepad.rightTrigger == element) {
            iPhoneSetRightTriggerPressed(gamepad.rightTrigger.isPressed);
        }
        
        // left shoulder button
        if (gamepad.leftShoulder == element && gamepad.leftShoulder.isPressed) {
            message = [message stringByAppendingString:@"Left Shoulder Button"];
        }
        
        // right shoulder button
        if (gamepad.rightShoulder == element && gamepad.rightShoulder.isPressed) {
            message = [message stringByAppendingString:@"Right Shoulder Button"];
        }
        
        // A button
        if (gamepad.buttonA == element && gamepad.buttonA.isPressed) {
            message = [message stringByAppendingString:@"A Button"];
        }
        
        // B button
        if (gamepad.buttonB == element && gamepad.buttonB.isPressed) {
            message = [message stringByAppendingString:@"B Button"];
        }
        
        // X button
        if (gamepad.buttonX == element && gamepad.buttonX.isPressed) {
            message = [message stringByAppendingString:@"X Button"];
        }
        
        // Y button
        if (gamepad.buttonY == element && gamepad.buttonY.isPressed) {
            message = [message stringByAppendingString:@"Y Button"];
        }
        
        // d-pad
        if (gamepad.dpad == element) {
            if (gamepad.dpad.up.isPressed) {
                message = [message stringByAppendingString:@"D-Pad Up"];
            }
            if (gamepad.dpad.down.isPressed) {
                message = [message stringByAppendingString:@"D-Pad Down"];
            }
            if (gamepad.dpad.left.isPressed) {
                message = [message stringByAppendingString:@"D-Pad Left"];
            }
            if (gamepad.dpad.right.isPressed) {
                message = [message stringByAppendingString:@"D-Pad Right"];
            }
        }
        
        // left stick
        if (gamepad.leftThumbstick == element) {
            if (gamepad.leftThumbstick.up.isPressed || gamepad.leftThumbstick.down.isPressed) {
                iPhoneSetLeftThumbstickYValue(gamepad.leftThumbstick.yAxis.value);
            } else {
                iPhoneSetLeftThumbstickYValue(0);
            }
            
            if (gamepad.leftThumbstick.left.isPressed || gamepad.leftThumbstick.right.isPressed) {
                iPhoneSetLeftThumbstickXValue(gamepad.leftThumbstick.xAxis.value);
            } else {
                iPhoneSetLeftThumbstickXValue(0);
            }
            
            position = CGPointMake(gamepad.leftThumbstick.xAxis.value, gamepad.leftThumbstick.yAxis.value);
        }
        
        // right stick
        if (gamepad.rightThumbstick == element) {
            if (gamepad.rightThumbstick.up.isPressed || gamepad.rightThumbstick.down.isPressed) {
                iPhoneSetRightThumbstickYValue(gamepad.rightThumbstick.yAxis.value);
            } else {
                iPhoneSetRightThumbstickYValue(0);
            }
            
            if (gamepad.rightThumbstick.left.isPressed || gamepad.rightThumbstick.right.isPressed) {
                iPhoneSetRightThumbstickXValue(gamepad.rightThumbstick.xAxis.value);
            } else {
                iPhoneSetRightThumbstickXValue(0);
            }

            position = CGPointMake(gamepad.rightThumbstick.xAxis.value, gamepad.rightThumbstick.yAxis.value);
        }
        
        //NSLog(@"%@", message);
        
    };
    
    // we need a weak self here for in-block access
//    __weak typeof(self) weakSelf = self;
//
//    self.mainController.controllerPausedHandler = ^(GCController *controller){
//
//        // check if we're currently paused or not
//        // then bring up or remove the paused view controller
//        if (weakSelf.currentlyPaused) {
//
//            weakSelf.currentlyPaused = NO;
//            [weakSelf dismissViewControllerAnimated:YES completion:nil];
//
//        } else {
//
//            weakSelf.currentlyPaused = YES;
//            [weakSelf presentViewController:weakSelf.pausedViewController animated:YES completion:nil];
//        }
//
//    };
}

@end
