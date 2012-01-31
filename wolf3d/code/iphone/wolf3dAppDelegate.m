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

#import "wolf3dAppDelegate.h"
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

#if SPEARSTOREKIT
//was used for storekit
extern void FinalizeDownload();
extern int IsSpearPurchased();
extern int IsSpearInstalled();
extern void BeginStoreKit();
extern void GetSpear();
#endif

extern void DownloadURLConnection(char *url);

extern int wasCalledFromDownloadInstructionsMenu;
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
@synthesize glView;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	application.statusBarHidden = YES;
	application.statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
	
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
	accelerometer.updateInterval = 0.01;
	
	// do all the game startup work
	iphoneStartup();
	
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
	
}

//this is so that we can respond to alertView events (messageboxes)
//but this should only respond to the alertPurchaseSpear
- (void)alertView:(UIAlertView *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	//this is if they clicked yes to going to the map instructions website
	if (wasCalledFromDownloadInstructionsMenu && buttonIndex == 1) {
		SysIPhoneOpenURL( "http://www.idsoftware.com/wolfenstein-3d-classic-platinum/mapinstructions/" );
		return;
	}
	
	//if they clicked yes to going to the idsoftware website
	if (buttonIndex == 1)
		SysIPhoneOpenURL( "http://www.idsoftware.com/wolfenstein-3d-classic-platinum/" );
	
	/*
	if (alertPurchaseSpear && (alertPurchaseSpear == actionSheet))
	{
		if (buttonIndex != actionSheet.cancelButtonIndex)
			BeginStoreKit();
	}*/
}

- (void)applicationWillResignActive:(UIApplication *)application {
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
}

- (void)applicationWillTerminate:(UIApplication *)application {
	iphoneShutdown();
}

#if 1
extern char urlbuffer[1024];
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
	// wolf3d:foo should launch wolf3d now... next, add useful URL parameter encoding

	
#ifdef LITE
	iphoneMessageBox("Lite version", "This is a Lite version.  You must purchase Wolfenstein3D to get this feature.");
	return NO;
#endif
	
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
	[window release];
	[glView release];
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
		accelerometer.updateInterval = 0.01;
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
@end



