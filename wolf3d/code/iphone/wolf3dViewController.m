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

@interface wolf3dViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation wolf3dViewController

@synthesize animating, context, displayLink, active;


- (id)initWithNibName:(NSString*)file bundle:(NSBundle*)bundle
{
	// Get the application's window dimensions.
	EAGLView *glView = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
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
				
	// Now that we have a context, we can init the render system.
	iphoneStartup();
	
	return self;
}



- (void)awakeFromNib
{
	EAGLView *glView = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
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
    
    // Release any cached data, images, etc. that aren't in use.
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

- (void)viewDidUnload
{
	[super viewDidUnload];

	[self stopAnimation];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
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

@end
