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


#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "wolf3dAppDelegate.h"

#include "wolfiphone.h"

//gsh
//#define NOTIFYLISTEN  //uncomment to receive notifications

struct AVSystemControllerPrivate;

@interface AVSystemController : NSObject
{
    struct AVSystemControllerPrivate *_priv;
}

+ (void)initialize;
+ (id)sharedAVSystemController;
- (id)init;
- (void)dealloc;
- (BOOL)changeActiveCategoryVolumeBy:(float)fp8 fallbackCategory:(id)fp12 resultVolume:(float *)fp16 affectedCategory:(id *)fp20;
- (BOOL)changeActiveCategoryVolumeBy:(float)fp8;
- (BOOL)setActiveCategoryVolumeTo:(float)fp8 fallbackCategory:(id)fp12 resultVolume:(float *)fp16 affectedCategory:(id *)fp20;
- (BOOL)setActiveCategoryVolumeTo:(float)fp8;
- (BOOL)getActiveCategoryVolume:(float *)fp8 andName:(id *)fp12 fallbackCategory:(id)fp16;
- (BOOL)getActiveCategoryVolume:(float *)fp8 andName:(id *)fp12;
- (BOOL)changeActiveCategoryVolumeBy:(float)fp8 forRoute:(id)fp12 andDeviceIdentifier:(id)fp16;
- (BOOL)setActiveCategoryVolumeTo:(float)fp8 forRoute:(id)fp12 andDeviceIdentifier:(id)fp16;
- (BOOL)activeCategoryVolumeDidChangeTo:(float)fp8 forRoute:(id)fp12 andDeviceIdentifier:(id)fp16;
- (BOOL)getActiveCategoryVolume:(float *)fp8 andName:(id *)fp12 forRoute:(id)fp16 andDeviceIdentifier:(id)fp20;
- (BOOL)toggleActiveCategoryMuted;
- (BOOL)toggleActiveCategoryMutedForRoute:(id)fp8 andDeviceIdentifier:(id)fp12;
- (BOOL)getActiveCategoryMuted:(char *)fp8;
- (BOOL)getActiveCategoryMuted:(char *)fp8 forRoute:(id)fp12 andDeviceIdentifier:(id)fp16;
- (BOOL)changeVolumeBy:(float)fp8 forCategory:(id)fp12;
- (BOOL)setVolumeTo:(float)fp8 forCategory:(id)fp12;
- (BOOL)getVolume:(float *)fp8 forCategory:(id)fp12;
- (id)routeForCategory:(id)fp8;
- (id)volumeCategoryForAudioCategory:(id)fp8;
- (id)attributeForKey:(id)fp8;
- (BOOL)setAttribute:(id)fp8 forKey:(id)fp12 error:(id *)fp16;
- (void)makeError:(id *)fp8 withDescription:(id)fp12 code:(long)fp16;
- (BOOL)okToNotifyFromThisThread;
- (void)handleServerDied;

@end

AVSystemController *SharedAVSystemController;
EAGLView *eaglview = nil;

// A class extension to declare private methods
@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@dynamic context;

// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithFrame:(CGRect)aRect {
    self = [super initWithFrame:aRect];
	
	eaglview = self;

	if ( self ) {
		[self setMultipleTouchEnabled:YES];
		
		// Support rendering at native resolution on devices with Retina displays.
		if( [self respondsToSelector:@selector(contentScaleFactor)] ) {
			self.contentScaleFactor = deviceScale;
		}
	
		// Get the layer
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		
		eaglLayer.opaque = TRUE;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										
										[NSNumber numberWithBool:NO], 
										kEAGLDrawablePropertyRetainedBacking,
										kEAGLColorFormatRGB565,
										kEAGLDrawablePropertyColorFormat,
										nil];
		

		    
		//self.multipleTouchEnabled = true;

#ifdef VOLUMEHACK
		//-------------------
		// Volume Button Hack
		//gsh
		// Note: MediaPlayer framework required for this trick
		//create a MPVolumeView to hack the volume button
		CGRect frame = CGRectMake(0, -30, 180, 10);  //put this thing offscreen
		volumeView = [[[MPVolumeView alloc] initWithFrame:frame] autorelease];
		[volumeView sizeToFit];
		[self addSubview:volumeView];
		
		// Find the volume view slider 
		for (UIView *view in [volumeView subviews]){
			if ([[[view class] description] isEqualToString:@"MPVolumeSlider"]) {
				volumeViewSlider = (UISlider *)view;
			}
		}
		
		//listen for volume changes
		[[NSNotificationCenter defaultCenter] addObserver:self 
												 selector:@selector(volumeListener:) 
													 name:@"AVSystemController_SystemVolumeDidChangeNotification" 
												   object:nil];
	
	//---------------------
#endif
	
#ifdef NOTIFYLISTEN	//gsh
		//this is a general purpose listener
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notificationListener:)
													 name:nil
												   object:nil];
#endif
	}

    return self;
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext)
    {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer)
    {
        [EAGLContext setCurrentContext:context];
		
        // Create default framebuffer object.
        glGenFramebuffersOES(1, &defaultFramebuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffersOES(1, &colorRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
			
        [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &framebufferWidth);
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &framebufferHeight);
        
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
		
		// Create the depth render buffer and allocate backing store.
		glGenRenderbuffersOES(1, &depthRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, framebufferWidth, framebufferHeight);
		
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
		
        if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		}
		
		// We can bind the color buffer here to avoid doing it every frame.
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    }
}

- (void)deleteFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer)
        {
            glDeleteFramebuffersOES(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
        {
            glDeleteRenderbuffersOES(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
		
		if (depthRenderbuffer)
		{
			glDeleteFramebuffersOES(1, &depthRenderbuffer);
			depthRenderbuffer = 0;
		}
    }
}

- (void)setFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
		//glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        
        //glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
		
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
		
		if ( gl_config.framebuffer_discard ) {
			const GLenum discards[]  = {GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES,1,discards);
		}
		
        
		loggedTimes[iphoneFrameNum&(MAX_LOGGED_TIMES-1)].beforeSwap = Sys_Milliseconds();
        success = [context presentRenderbuffer:GL_RENDERBUFFER_OES];
		loggedTimes[iphoneFrameNum&(MAX_LOGGED_TIMES-1)].afterSwap = Sys_Milliseconds();
    }
    
    return success;
}


- (void)layoutSubviews {
	//float widthRatio = ( self.bounds.size.width * deviceScale ) / REFERENCE_WIDTH;
	//float heightRatio = ( self.bounds.size.height * deviceScale ) / REFERENCE_HEIGHT;
	
	[self deleteFramebuffer];
	
	/*
	if ( widthRatio < heightRatio ) {
		screenScale = widthRatio;
	} else {
		screenScale = heightRatio; 
	}
	
	// Set the global width and height so the game code can access it.
	// since the game runs in landscape, we must switch width and height.
	viddef.width = self.bounds.size.width * deviceScale;
	viddef.height = self.bounds.size.height * deviceScale;
	*/
	// Need to re-arrange the HUD now that the screen changed.
	if ( controlScheme ) {
		HudSetForScheme( controlScheme->value );
	}
}


- (void)dealloc {    
	[self deleteFramebuffer];
    [context release];
	
    [super dealloc];
}

- (void) handleTouches:(NSSet*)touches withEvent:(UIEvent*)event {
	int touchCount = 0;
	int points[16];
	static int previousTouchCount = 0;
	
	//gsh
	if (previousTouchCount == 0)
		isTouchMoving = 0;
	
	NSSet *t = [event allTouches];
    for (UITouch *myTouch in t)
    {
        CGPoint touchLocation = [myTouch locationInView:self];

		points[ 2 * touchCount + 0 ] = touchLocation.x;
		points[ 2 * touchCount + 1 ] = touchLocation.y; // ( h - 1 ) - touchLocation.y;
		
		touchCount++;
		
        if (myTouch.phase == UITouchPhaseBegan) {
            // new touch handler
        }
        if (myTouch.phase == UITouchPhaseMoved) {
            // touch moved handler
			//gsh, use this for swipe events in the scrolling menus
			isTouchMoving = 1;
        }
        if (myTouch.phase == UITouchPhaseEnded) {
			touchCount--;
		}
    }
	
	// toggle the console with four touches
	if ( touchCount == 4 && previousTouchCount != 4 ) {			
		if ( textField == nil ) {
			// do this before starting the textField, which
			// takes a long time
			iphoneActivateConsole();
			
			textField = [UITextField alloc];
			[textField initWithFrame:CGRectMake( 0, 0, 20, 20 ) ];
			[self addSubview:textField];
			[textField release];
			textField.hidden = true;
			textField.delegate = self;
			textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
			textField.autocorrectionType = UITextAutocorrectionTypeNo;
			[textField becomeFirstResponder];
		} else {
			void iphoneDeactivateConsole();
			[textField resignFirstResponder];
			[textField removeFromSuperview];
			textField = nil;
			
			iphoneDeactivateConsole();
		}		
	}
	previousTouchCount = touchCount;
	
	iphoneTouchEvent( touchCount, points );
}


- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
	[self handleTouches:touches withEvent:event];
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
	[self handleTouches:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[self handleTouches:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[self handleTouches:touches withEvent:event];
}

// gsh
#ifdef NOTIFYLISTEN
- (void) notificationListener:(NSNotification *)notify
{
	Com_Printf("notificationListener: %s\n", [notify.name UTF8String]);
//NSString
	
	if ([notify.name isEqualToString:@"CPDistributedMessagingCenterServerDidTerminateNotification"] && menuState == IPM_STOREKIT)
	{
		iphoneMessageBox("Apple Store Failed", "Connection to app store has terminated.  Please try again later.");
		menuState = IPM_MAIN;
	}
	//sometimes after requrestProductData() is called we get these notifications
	//and the storekit no longer responds (making it appear as a crash)
	/*	
notificationListener: CPDistributedMessagingCenterServerDidTerminateNotification
notificationListener: SKNotificationRequestFinished
notificationListener: SKNotificationTransactionsRefreshed
notificationListener: CPDistributedMessagingCenterServerDidTerminateNotification
*/	
}
#endif

#ifdef VOLUMEHACK
//-------------------
// Volume Button Hack
// gsh
// currently this is problematic...
// it's slow if the user holds the volume button
// let's see if inlining this with the normal game loop is faster
// than listening for the event... it's not
// Note:  MediaPlayer framework required for this trick
//-------------------
- (void) volumeListener:(NSNotification *)notify
{
	//TODO: provide left/right click attacks
	if (volumeViewSlider.value < s_masterVolume->value)
	{
	
		[volumeViewSlider setValue:s_masterVolume->value animated:NO];//volumeSetting animated:NO];
		[volumeViewSlider _commitVolumeChange];  //again, ignoring compiler warning complaints
												//this might have a warning because it's currently undocumented?
		Cvar_Set("volumeFireDown", "1");
	}
	else if (volumeViewSlider.value > s_masterVolume->value)//volumeSetting)
	{
		[volumeViewSlider setValue:s_masterVolume->value animated:NO];//volumeSetting animated:NO];
		[volumeViewSlider _commitVolumeChange];  //again, ignoring compiler warning complaints
		Cvar_Set("volumeFireUp", "1");
	}
}
#endif


@end


@implementation EAGLView (UITextFieldDelegate)

- (BOOL)textFieldShouldReturn:(UITextField *)_textField 
{
	iphoneExecuteCommandLine();
	return YES;
}

@end


const char * SysIPhoneGetConsoleTextField() {
	if ( eaglview->textField == nil ) {
		return "";
	}
	return [ eaglview->textField.text UTF8String ];
}

void SysIPhoneSetConsoleTextField( const char * str) {
	assert( eaglview->textField != nil );	
	eaglview->textField.text = [ NSString stringWithUTF8String: str ];
}

void SysIPhoneOpenURL( const char *url ) {
	Com_Printf( "OpenURL char *: %s\n", url );
	
	NSString *nss = [NSString stringWithCString: url encoding: NSASCIIStringEncoding];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString: nss]];
}

void SysIPhoneLoadJPG( W8* jpegData, int jpegBytes, W8 **pic, W16 *width, W16 *height, W16 *bytes ) {
	CFDataRef data;
	int dataBytes = 0;
	UIImage *img = [ UIImage imageWithData: [NSData dataWithBytes: (const char *)jpegData length: (NSUInteger)jpegBytes ] ];
	int imgBytes;
	*width = img.size.width;
	*height = img.size.height;	
	imgBytes = (int)(*width) * (int)(*height) * 4;
	data = CGDataProviderCopyData( CGImageGetDataProvider( img.CGImage ) );
	dataBytes = CFDataGetLength( data );
	*bytes = 4;
	if ( dataBytes > imgBytes ) {
		*pic = NULL;
		return;
	}
	*pic = (W8 *)malloc( imgBytes );
	CFDataGetBytes( data, CFRangeMake(0, dataBytes), *pic );
	// convert BGRA to RGBA
	for ( imgBytes = 0; imgBytes < dataBytes; imgBytes+= 4 ) {
		W8 tmp = pic[0][ imgBytes + 0 ];
		pic[0][ imgBytes + 0 ] = pic[0][ imgBytes + 2 ];
		pic[0][ imgBytes + 2 ] = tmp;
	}
}

