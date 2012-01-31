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

#import "wolf3dAppDelegate.h"
#import "MainMenuViewController.h"
#import "wolf3dViewController.h"
#import "EAGLView.h"
#import <AudioToolbox/AudioServices.h>
#include "../wolfiphone.h"
#include <pthread.h>

//added for downloading SOD content from the internet... added by gsh
extern void Com_Printf(const char* fmt, ... );
//extern void AppendData(NSData* data);
extern void AppendDataToFile(NSData* data);
extern void AppendUserDataToFile(NSData* data);
//extern void SaveData();

//used for downloading custom made map
extern void FinalizeUserDownload();

yesNoBoxType_t currentYesNoBox = YESNO_NONE;

#if SPEARSTOREKIT
//was used for storekit
extern void FinalizeDownload();
extern int IsSpearPurchased();
extern int IsSpearInstalled();
extern void BeginStoreKit();
extern void GetSpear();
#endif

extern void DownloadURLConnection(char *url);

extern void iphoneSet2D();


//extern bool isAlive;



@interface UIApplication (Private)

- (void)setSystemVolumeHUDEnabled:(BOOL)enabled forAudioCategory:(NSString *)category;
- (void)setSystemVolumeHUDEnabled:(BOOL)enabled;

@end


char iphoneDocDirectory[1024];
char iphoneAppDirectory[1024];


void SysIPhoneVibrate() {
	AudioServicesPlaySystemSound( kSystemSoundID_Vibrate );
}

@implementation wolf3dAppDelegate

@synthesize window;
@synthesize viewController;
@synthesize navigationController;
@synthesize waitingView;
@synthesize player;

- (void)initMenuMusicPlayer {
	NSString *soundFilePath =
				[[NSBundle mainBundle] pathForResource: @"wondering"
												ofType: @"caf"];
	 
	NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: soundFilePath];
	 
	AVAudioPlayer *newPlayer =
				[[AVAudioPlayer alloc] initWithContentsOfURL: fileURL
													   error: nil];
	[fileURL release];
	
	if ( s_masterVolume != NULL ) {
		newPlayer.volume = s_masterVolume->value * 0.5f;
	} else {
		newPlayer.volume = 0.2f;
	}

	
	newPlayer.numberOfLoops = -1;
	
	self.player = newPlayer;
	[newPlayer release];
	 
	[player prepareToPlay];
}

- (void)startMenuMusic {

	[player play];
}

- (void)stopMenuMusic {

	[player stop];
}



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	// get the documents directory, where we will write configs and save games
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	[documentsDirectory getCString: iphoneDocDirectory 
							maxLength: sizeof( iphoneDocDirectory ) - 1
							encoding: NSASCIIStringEncoding ];
	
	// get the app directory, where our data files live
	paths = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSUserDomainMask, YES);
	NSString *appDirectory = documentsDirectory = [paths objectAtIndex:0];
	[appDirectory getCString: iphoneAppDirectory 
							maxLength: sizeof( iphoneAppDirectory ) - 1
							encoding: NSASCIIStringEncoding ];
	
	// start the flow of accelerometer events
	UIAccelerometer *accelerometer = [UIAccelerometer sharedAccelerometer];
	accelerometer.delegate = self;
	accelerometer.updateInterval = 1.0 / 30.0;
	
	// do all the game startup work
	//iphoneStartup();
	
	
#if SPEARSTOREKIT
	//check if user downloaded spear but didn't purchase
	if (IsSpearInstalled() && !IsSpearPurchased())
	{
		alertPurchaseSpear = [[UIAlertView alloc] initWithTitle:@""//Title" 
												message:@"You have downloaded and installed Spear of Destiny.  Would you like to unlock those levels by purchasing it for $1.99?"
											   delegate:self 
									  cancelButtonTitle:@"No" 
									  otherButtonTitles:@"Yes", nil];	
		
		[alertPurchaseSpear show];
	}
#endif
	
	// Sign up for rotation notifications
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
                selector:@selector(didRotate:) 
                name:UIDeviceOrientationDidChangeNotification
                object:nil];
	
	// Support rendering at native resolution on devices with Retina displays.
	if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)] ) {
		deviceScale = UIScreen.mainScreen.scale;
		touchCoordinateScale = UIScreen.mainScreen.scale;
	}
	
	// Screen is initially landscape-left.
	// BEWARE! For UI*Interface*Orientation, Left/Right refers to the location of the home button.
	// BUT, for UI*Device*Orientation, Left/Right refers to the location of the side OPPOSITE the home button!!
	[self setScreenForOrientation:UIDeviceOrientationLandscapeRight];
	
	// Create the window programmatically instead of loading from a nib file.
	self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	
	// We will create the OpenGL view here so we can get a context and preload textures, but
	// don't actually add the view to the window until the player enters a level.
	wolf3dViewController *vc = [[wolf3dViewController alloc] initWithNibName:nil bundle:nil];
	self.viewController = vc;
	[self.viewController setActive:NO];
	[vc release];
	
	MainMenuViewController *rootController = [[MainMenuViewController alloc] initWithNibName:@"MainMenuView" bundle:nil];
	navigationController = [[UINavigationController alloc] initWithRootViewController:rootController];
	[navigationController setNavigationBarHidden:YES];
	[rootController release];
	
	[window addSubview:navigationController.view];
	[window makeKeyAndVisible];

	return YES;
}

- (void)showOpenGL {
	// Maybe clearing the OpenGL view before displaying will fix the old frame flashing.
	// I don't mind if this blocks until a vsync, becasue it's just a menu.
	[self.viewController clearAndPresentRenderbuffer];
	
	[[navigationController view] removeFromSuperview];
	
	[self.viewController setActive:YES];
	[window addSubview:[self.viewController view]];

	[self.viewController startAnimation];
}

- (void)didRotate:(NSNotification *)notification {
	UIDeviceOrientation orient = [[UIDevice currentDevice] orientation];
	
	[self setScreenForOrientation:orient];
}

- (void)setScreenForOrientation:(UIDeviceOrientation) orientation {
	// Note the the UIDeviceOrientations are REVERSED from the UIInterface orientations!
	switch (orientation) {	
		case UIDeviceOrientationLandscapeLeft:
			deviceOrientation = ORIENTATION_LANDSCAPE_RIGHT;
			viddef.width = [UIScreen mainScreen].bounds.size.height * deviceScale;
			viddef.height = [UIScreen mainScreen].bounds.size.width * deviceScale;
			//[UIApplication sharedApplication].statusBarOrientation = UIInterfaceOrientationLandscapeRight;
			break;
			
		case UIDeviceOrientationLandscapeRight:
			deviceOrientation = ORIENTATION_LANDSCAPE_LEFT;
			viddef.width = [UIScreen mainScreen].bounds.size.height * deviceScale;
			viddef.height = [UIScreen mainScreen].bounds.size.width * deviceScale;
			//[UIApplication sharedApplication].statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
			break;
			
		default:
			break;
	}
	
	float widthRatio = viddef.width / REFERENCE_WIDTH;
	float heightRatio = viddef.height / REFERENCE_HEIGHT;
	
	if ( widthRatio < heightRatio ) {
		screenScale = widthRatio;
	} else {
		screenScale = heightRatio; 
	}
}

//this is so that we can respond to alertView events (messageboxes)
//but this should only respond to the alertPurchaseSpear
- (void)alertView:(UIAlertView *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 1) {
		switch ( currentYesNoBox ) {
			case YESNO_BUY_PLATINUM: {
				// They clicked yes to buy the platinum edition.
				
				// Set up a fullscreen view with the activity indicator in the middle while
				// we wait for the in-app purchase to complete.
				UIView* topView = [[self.navigationController topViewController] view];
				CGRect waitFrame = topView.frame;
				
				
				[waitingView release];
				waitingView = [[UIView alloc] initWithFrame:waitFrame];
				waitingView.backgroundColor = [UIColor blackColor];
				waitingView.opaque = NO;
				waitingView.alpha = 0.75;
				
				
				UIActivityIndicatorView *indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
				[indicator startAnimating];
				
				[waitingView addSubview:indicator];
				// This mask centers the view in its superview
				indicator.center = indicator.superview.center;
				[indicator release];
				
				[topView addSubview:waitingView];
				
				// Start the request to the app store
				InAppPurchaseSetCallback( inAppPurchaseCallback );
				InAppPurchaseStartPurchase( PLATINUM_UPGRADE_STRING );
				
				break;
			}
				
			case YESNO_GO_TO_WEBSITE:
				//if they clicked yes to going to the idsoftware website
				SysIPhoneOpenURL( "http://www.idsoftware.com/wolfenstein-3d-classic-platinum/" );
				break;
			
			case YESNO_DOWNLOAD_INSTRUCTIONS:
				//this is if they clicked yes to going to the map instructions website
				SysIPhoneOpenURL( "http://www.idsoftware.com/wolfenstein-3d-classic-platinum/mapinstructions/" );
				break;
			
			default:
				break;
		}
	}
	
	/*
	if (alertPurchaseSpear && (alertPurchaseSpear == actionSheet))
	{
		if (buttonIndex != actionSheet.cancelButtonIndex)
			BeginStoreKit();
	}*/
}

- (void)applicationWillResignActive:(UIApplication *)application {
	[self.viewController stopAnimation]; 
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	[self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application {
	[self.viewController stopAnimation];
	
	// Save the game.
	iphoneShutdown();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	[self.viewController stopAnimation];
	
	// Save the game.
	iphoneShutdown();
}


- (void)dismissWaitingView {
	if ( waitingView != nil ) {
		[waitingView removeFromSuperview];
		[waitingView release];
		waitingView = nil;
	}
}

#if 1
extern char urlbuffer[1024];
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
	// wolf3d:foo should launch wolf3d now... next, add useful URL parameter encoding

	
	if ( SysIPhoneGetContentVersion() == CONTENT_LITE ) {
		iphoneMessageBox("Lite version", "This is a Lite version.  You must purchase Wolfenstein3D to get this feature.");
		return NO;
	}
	
	if (!url)
		return NO;

	NSString *str = [url absoluteString];   //in the future we may wish to parse this string
											//for commands that wolf3d might follow
											//for now we'll just start the storekit
	char buffer[1024];
	
	if(![str getCString:buffer maxLength:1024 encoding:NSUTF8StringEncoding])
	{
		iphoneMessageBox("Not Valid", "There were encoding errors.  Make sure the link is less than 1023 characters in length.");
		return NO;
	}

	//iphoneMessageBox("URL", buffer);
	/*
	if (strcmp(buffer, "wolf3d:foo") == 0)
		GetSpear();	
	*/
	char *pstr = buffer;//&buffer[strlen(buffer) - 5];
	pstr += strlen(buffer) - 4;
	
	if (strcmp(pstr, ".map") != 0)
	{
		iphoneMessageBox("Not Valid", "URL is not a valid map.  Maps must end in \".map\"");
		return NO;
	}
	
	
	if (strncmp(buffer, "wolf3dp:", 8) == 0)
	{
		
		char cURL[1024];
		char *http = "http:";
		strcpy(cURL, http);
		strcpy(cURL + 5, buffer + 8);
		//strcpy(cURL, "http://gregory.hodges.googlepages.com/SODmm.tgz"); //TODO: delete me
		Com_Printf("wolf3dp: installing map %s\n", cURL);
		strcpy(urlbuffer, cURL);
		//download and install this map
		//iphoneMessageBox("wolf3d:", "it's a map TODO install");
		//InstallUserMap();
		DownloadURLConnection(cURL);
	}
	else
	{
		iphoneMessageBox("Not Valid", "URL is not a valid map.  Identifier must start with \"wolf3dp:\"");
		return NO;
	}
	
	return YES;
}
#endif


- (void)dealloc {
	[viewController release];
	[navigationController release];
	[window release];
	
	[super dealloc];
}

- (void)restartAccelerometerIfNeeded {

	// I have no idea why this seems to happen sometimes...
	if ( Sys_Milliseconds() - lastAccelUpdateMsec > 1000 ) {
		static int count;
		if ( ++count < 5 ) {
			printf( "Restarting accelerometer updates.\n" );
		}
		UIAccelerometer *accelerometer = [UIAccelerometer sharedAccelerometer];
		accelerometer.delegate = self;
		accelerometer.updateInterval = 1.0 / 30.0;
	}
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{	
	float acc[4];
	acc[0] = acceleration.x;
	acc[1] = acceleration.y;
	acc[2] = acceleration.z;
	acc[3] = acceleration.timestamp;
	iphoneTiltEvent( acc );
	lastAccelUpdateMsec = Sys_Milliseconds();
}

//------------------------------------------------------------
// connection
// called by the app when data is received during download
// gsh
//-------------------------------------------------------------
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	// lock the game out temporarily
	pthread_mutex_t eventMutex;
	pthread_mutex_lock( &eventMutex );
/*		
	//display the network activity indicator
	UIApplication* app = [UIApplication sharedApplication]; 
	app.networkActivityIndicatorVisible = YES; // to stop it, set this to NO
	app.statusBarHidden = NO;
*/	
    // append the new data to file
	AppendUserDataToFile(data);
	
	// the game is free to copy the appendedData size now
	pthread_mutex_unlock( &eventMutex );
}

//------------------------------------------------------------------------
// connectionDidFinishLoading
//  this is called when the app finishes downloading
//						the new Spear of Destiny Levels
// gsh
//------------------------------------------------------------------------
- (void) connectionDidFinishLoading:(NSURLConnection *)connection
{
	Com_Printf("connectionDidFinishLoading called\n");
/*
	//no need to display network traffic anymore
	UIApplication* app = [UIApplication sharedApplication]; 
	app.networkActivityIndicatorVisible = NO; // to stop it, set this to NO
	app.statusBarHidden = YES;
*/	
	//release the connection
	[connection release];
	
	//uncompress/move files/delete unwanted	
	FinalizeUserDownload();
}

//------------------------------------------------------------------------
// connection didFailWithError
// if the connection fails during the download then this is called
// gsh
//------------------------------------------------------------------------
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	//let user know that the connection failed
	iphoneMessageBox("Connection Failed", "Please check your connection and try again later.");
	
	//return the user to the main menu
	menuState = IPM_MAIN;
/*	
	//no need to display network traffic anymore
	UIApplication* app = [UIApplication sharedApplication]; 
	app.networkActivityIndicatorVisible = NO; // to stop it, set this to NO
	app.statusBarHidden = YES;	*/
}
/*
- (void)setDownloadResponse:(NSURLResponse *)aDownloadResponse
{
    [aDownloadResponse retain];
    [downloadResponse release];
    downloadResponse = aDownloadResponse;
}

- (void)download:(NSURLDownload *)download didReceiveResponse:(NSURLResponse *)response
{
    // reset the progress, this might be called multiple times
    bytesReceived=0;
	
    // retain the response to use later
    [self setDownloadResponse:response];
}

- (void)download:(NSURLDownload *)download didReceiveDataOfLength:(unsigned)length
{
    long long expectedLength=[[self downloadResponse] expectedContentLength];
	
    bytesReceived=bytesReceived+length;
	
    if (expectedLength != NSURLResponseUnknownLength) {
        // if the expected content length is
        // available, display percent complete
        float percentComplete=(bytesReceived/(float)expectedLength)*100.0;
        NSLog(@"Percent complete - %f",percentComplete);
    } else {
        // if the expected content length is
        // unknown just log the progress
        NSLog(@"Bytes received - %d",bytesReceived);
    }
}
*/

- (void)GLtoMainMenu {
	[self.navigationController popToRootViewControllerAnimated:NO];
	[viewController setActive:NO];
	[[viewController view] removeFromSuperview];
	[window addSubview:navigationController.view];
}

- (void)GLtoPreviousMenu {
	[viewController setActive:NO];
	[[viewController view] removeFromSuperview];
	[window addSubview:navigationController.view];
}

@end

void iphoneStartMainMenu() {
	Sound_StopBGTrack();
	iphoneStartMenuMusic();
	numTouches = 0;
	numPrevTouches = 0;
	wolf3dAppDelegate *app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app GLtoMainMenu];
}

void iphoneStartPreviousMenu() {
	wolf3dAppDelegate *app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app GLtoPreviousMenu ];
}

void inAppPurchaseCallback( InAppPurchaseResult result ) {
	wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app dismissWaitingView];
}

