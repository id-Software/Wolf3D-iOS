//
//  EAGLView.m
//  wolf3d
//
//  Created by Cass Everitt on 2/20/09.
//  Copyright Id Software 2009. All rights reserved.
//



#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "wolf3dAppDelegate.h"

#include "wolfiphone.h"

EAGLView *eaglview;

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (void) destroyFramebuffer;
- (void) swapBuffers;

@end


@implementation EAGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
    self = [super initWithCoder:coder];
	
	eaglview = self;
		
	// Get the layer
	CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
	
	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
									
									[NSNumber numberWithBool:NO], 
									kEAGLDrawablePropertyRetainedBacking, 
									
									kEAGLColorFormatRGB565,
									/* kEAGLColorFormatRGBA8, */
									kEAGLDrawablePropertyColorFormat, 
									
									nil];
	
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	assert( context );
	
	if ( ![EAGLContext setCurrentContext:context]) {
		[self release];
		return nil;
	}        
	self.multipleTouchEnabled = true;
	
    [EAGLContext setCurrentContext:context];
	
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    }
	
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.032 
														   target:self 
														 selector:@selector(drawView) 
														 userInfo:nil repeats:YES];
    return self;
}

- (void)drawView {
	int	start, end;
	
    [EAGLContext setCurrentContext:context];
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);

	[ (wolf3dAppDelegate *)[UIApplication sharedApplication].delegate restartAccelerometerIfNeeded];
	
	start = Sys_Milliseconds();
	
	extern void iphoneFrame();
	iphoneFrame();

	end = Sys_Milliseconds();
//	Com_Printf( "msec: %i\n", end - start );

	[self swapBuffers];
}

void GLimp_EndFrame() {
	[eaglview swapBuffers];
}

- (void)swapBuffers {
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)layoutSubviews {
    [self drawView];
}



- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
	glDeleteRenderbuffersOES(1, &depthRenderbuffer);
	depthRenderbuffer = 0;
}


- (void)dealloc {
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];  
    [super dealloc];
}

void WolfensteinTouches( int numTouches, int touches[16] );

- (void) handleTouches:(NSSet*)touches withEvent:(UIEvent*)event {
	int touchCount = 0;
	int points[16];
	static int previousTouchCount;
	
	NSSet *t = [event allTouches];
    for (UITouch *myTouch in t)
    {
        CGPoint touchLocation = [myTouch locationInView:nil];

		points[ 2 * touchCount + 0 ] = touchLocation.x;
		points[ 2 * touchCount + 1 ] = touchLocation.y; // ( h - 1 ) - touchLocation.y;
		
		touchCount++;
		
        if (myTouch.phase == UITouchPhaseBegan) {
            // new touch handler
        }
        if (myTouch.phase == UITouchPhaseMoved) {
            // touch moved handler
        }
        if (myTouch.phase == UITouchPhaseEnded) {
			touchCount--;
        }
    }
	
	// toggle the console with four touches
	if ( touchCount == 4 && previousTouchCount != 4 ) {			
		if ( textField == nil ) {
			void iphoneActivateConsole();
			textField = [UITextField alloc];
			[textField initWithFrame:CGRectMake( 0, 0, 20, 20 ) ];
			[self addSubview:textField];
			[textField release];
			textField.hidden = true;
			textField.delegate = self;
			textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
			textField.autocorrectionType = UITextAutocorrectionTypeNo;
			[textField becomeFirstResponder];

			iphoneActivateConsole();
		} else {
			void iphoneDeactivateConsole();
			[textField resignFirstResponder];
			[textField removeFromSuperview];
			textField = nil;
			
			iphoneDeactivateConsole();
		}		
	}
	previousTouchCount = touchCount;
	
	WolfensteinTouches( touchCount, points );
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



@end


@implementation EAGLView (UITextFieldDelegate)

- (BOOL)textFieldShouldReturn:(UITextField *)_textField 
{
	void iphoneExecuteCommandLine();
	iphoneExecuteCommandLine();
	return YES;
}

@end

const char * GetCurrentCommandLine() {
	assert( eaglview->textField != nil );
	return [ eaglview->textField.text UTF8String ];
}

void SetCurrentCommandLine( const char * str) {
	assert( eaglview->textField != nil );	
	eaglview->textField.text = [ NSString stringWithUTF8String: str ];
}

void OpenURL( const char *url ) {
	Com_Printf( "OpenURL char *: %s\n", url );
	
	NSString *nss = [NSString stringWithCString: url encoding: NSASCIIStringEncoding];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString: nss]];
}


void iPhoneLoadJPG( W8* jpegData, int jpegBytes, W8 **pic, W16 *width, W16 *height, W16 *bytes ) {
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

