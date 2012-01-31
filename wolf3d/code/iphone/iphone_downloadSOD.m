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

#import "../wolfiphone.h"
#import "wolf3dAppDelegate.h"
//#import "zlib.h"
#import "iphone_alerts.h"

#include <CoreFoundation/CoreFoundation.h>
#include <unistd.h>

//gsh
#import <SystemConfiguration/SCNetworkReachability.h>  //this is for testing network availability

//Note: in order to link the zlib library into the build from usr/lib/libz.ylib
//      the flag -lz was added to the OTHER_LDFLAGS under the build options.
//      To get there: right click on the wolf3d project.  choose get info.
//      Click on build and scroll to "Other Link Flags" under "Linking"


extern  void  Com_Printf(const char* fmt, ... );
extern  void  my_snprintf( char *dest, size_t size, const char *format, ... );

#ifdef SPEARSTOREKIT
//extern  int   StartUntgz(char *TGZfile);
extern void PurchaseSOD();
extern bool isStorekitAvailable;
#endif
extern  char  iphoneDocDirectory[1024];
extern menuState_t menuState;


int inf(FILE *source, FILE *dest);
void zerr(int ret);


//this is used for determining if we have the correctly sized data
const unsigned int DownloadFileSize = 45583; //size of maps

//-----------------------------
// TestURLConnection
// Test for internet access
//-----------------------------
int TestURLConnection()
{
	Com_Printf("Testing URL Connection\n");
	
	//We wish to be able to test connectability to both apple and id
	//because we need to communicate to both in order to complete the Spear of Destiny Transaction
	
	char *hostid = "www.idsoftware.com";
	char *hostApple = "www.apple.com";
	SCNetworkReachabilityFlags		flags[2];
    SCNetworkReachabilityRef reachability;
	BOOL gotFlags[2];
	
	reachability = SCNetworkReachabilityCreateWithName(NULL, hostid);
	gotFlags[0] = SCNetworkReachabilityGetFlags(reachability, &flags[0]);
	CFRelease(reachability);
	
	reachability = SCNetworkReachabilityCreateWithName(NULL, hostApple);
	gotFlags[1] = SCNetworkReachabilityGetFlags(reachability, &flags[1]);
	CFRelease(reachability);
	
	
	if (gotFlags[0] && gotFlags[1])
	{
		// kSCNetworkReachabilityFlagsReachable indicates that the specified nodename or address can
		// be reached using the current network configuration.
		if ((flags[0] & kSCNetworkReachabilityFlagsReachable) && (flags[1] & kSCNetworkReachabilityFlagsReachable))
		{
			Com_Printf("Both Hosts were reached\n");
			return 1;
		}
	}

	//TODO: alert user they cannot download SOD
	iphoneMessageBox("Host Server Unavailable", "Check your internet connection and try again later.");

	return 0; 
}

//-----------------------------
// OpenURLConnection
// This connects to a server and downloads the file located there
//-----------------------------
void OpenURLConnection( const char *url )
{
	Com_Printf( "ConnectURL char *: %s\n", url );
	
	//convert url to nsstring
	NSString *nssURL = [NSString stringWithUTF8String: url];
	
	// create the request
	NSURLRequest *theRequest=[NSURLRequest requestWithURL:[NSURL URLWithString:nssURL]
											  cachePolicy:NSURLRequestUseProtocolCachePolicy
										  timeoutInterval:60.0];
	
	// create the connection with the request
	// and start loading the data
	NSURLConnection *theConnection=[[NSURLConnection alloc] initWithRequest:theRequest 
																   delegate:(wolf3dAppDelegate *)[UIApplication sharedApplication].delegate];

	if (!theConnection)
	{
		// inform the user that the download could not be made
		Com_Printf("Connection failed... no download plausible\n");
		
		//inform user
		iphoneMessageBox("Connection Failed", "The install was unable to download.  Check your internet connection and try again later.");
		
		//return to main menu
		menuState = IPM_MAIN;
	}
}

//============================
// DisplayWrongVersionedOS
// Display to the user they have the wrong OS version and cannot use storekit
//=============================
void DisplayWrongVersionedOS()
{
	iphoneMessageBox("Feature Unavailable", "Your device is not updated.  This feature requires OS 3.0 or higher.");
}

//============================
// BeginStoreKit
// This starts the purchasing process of the storekit framework
//=============================
#ifdef SPEARSTOREKIT
void BeginStoreKit()
{
	//only start the storekit API if it exists
	if (isStorekitAvailable)
	{
		if (TestURLConnection())  //check URL connection again
		{
			menuState = IPM_STOREKIT;
			Com_Printf("BeginStoreKit() called\n");
			PurchaseSOD();
		}
	}
	else
		DisplayWrongVersionedOS();
}
#endif


//============================
// DownloadSOD
// starts the download process of SOD
// this is called after purchase of the SOD levels is confirmed
//=============================
void DownloadSOD()
{
	menuState = IPM_DOWNLOADPROGRESS; //change the menu to the download screen
	                                 
	/*
	//TODO: alert user that the download is beginning and will take a while
	*/
	//iphoneMessageBox("Download in Progress", "You have not been charged.  Purchasing comes at the end of this process.  Download may take several minutes.");
		
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
		
	//check if download.tgz already exists and delete if it does
	//create/open file for saving
	NSString *fileatpath = [documentsDirectory stringByAppendingString:@"/downloadedfile.tgz"];
	NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:fileatpath];
	
	if (file) //file exists... delete it, we do not care to append to a pre-exisiting file
	{
		if( remove( [fileatpath UTF8String] ) != 0 )
			Com_Printf("Error deleting buffer\n");
		else
			Com_Printf("Deleted buffer\n");
	}
	
	//TODO: change this url to what will eventually be the permanent location
	OpenURLConnection("http://gregory.hodges.googlepages.com/spearMaps.tgz");
	
	//after this the control automatically goes back to the appDelegate where it receives the packets of downloaded information
}
/*
//============================
// AppendData
// adds a packet of data to our buffer
//============================
void AppendData(NSData* data)
{
	Com_Printf("appending data\n");
	
	NSUInteger length = [data length];
	NSLog(@"Received %d bytes of data", length);
	
//	[receivedData appendData:data];
}*/

unsigned int dataAmount = 0;
unsigned int dataTotalAmount = 0;
extern unsigned int totalDownloaded;
//int hasBeenMessaged = 0;

//============================
// AppendData
// adds a packet of data directly to file
//============================
void AppendDataToFile(NSData* data)
{
	NSUInteger length = [data length];
	
	dataAmount = (unsigned int)length;
	dataTotalAmount += dataAmount;
	totalDownloaded = dataTotalAmount;  //update the download screen with the total downloaded
/*
	if (dataTotalAmount > DownloadFileSize * 7/10  && !hasBeenMessaged)
	{
		iphoneMessageBox("Extraction in Progress", "SOD is installing.  This may take several minutes.");
		hasBeenMessaged = 1;
	}
*/	
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//create/open file for saving
	NSString *fileatpath = [documentsDirectory stringByAppendingString:@"/downloadedfile.tgz"];
	NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:fileatpath];
	
	if (!file) //file does not exist... create and append file
	{
		NSLog(@"open file failed\ncreating file with path: %@\n", fileatpath);
		[[NSFileManager defaultManager] createFileAtPath:fileatpath
												contents:data
											  attributes:nil];
		return;
	}
	
	//seek to end of file and append data
	[file seekToEndOfFile];
	[file writeData:data];
	[file closeFile];
}



//===========================
// DecompressData
// Calls into untgz.c to start data inflation
// this function should not be getting called
//============================
void DecompressData()
{
	Com_Printf("Starting DecompressData");
	
	char	path[1024];
	my_snprintf(path, sizeof(path), "%s/downloadedfile.tgz", iphoneDocDirectory);
	
//	StartUntgz(path);  //this is where we would normally start decompression.
						//but we no longer need to decompress anything
						//so we might as well get rid of our zlib dependencies
}
/*
//================================
// CopyFolders
// Copies the contents of the base folder to the SODbase folder
// One folder at a time... hopefully to allow renderings
// This allows us to provide a smaller download
//================================ 
void CopyFolders()
{
	int numberOfFolders = 7;
	char sourcePath[1024];
	char destPath[1024];
	char folderNames[7][8] = {"iphone","lsfx","maps","music","sfx","sprites","walls"};
	
	char *appDir;
	appDir = getenv("CWD");
	
	NSString *source;
	NSString *toPath;
	
	//Draw a black colored bar
	colour4_t backgroundColor = { 10, 10, 10, 255 };
	R_Draw_Blend( 40, 150, 4*100, 20, backgroundColor );
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSError *error;
	
	//try creating the base directory
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/", iphoneDocDirectory);
	toPath = [[NSString alloc] initWithUTF8String:destPath];
	NSLog(@"creating Directory: %@", toPath);
	if([fileManager createDirectoryAtPath:toPath attributes:nil])
		Com_Printf("creating SODbase success!\n");
	else
		Com_Printf("creating SODbase failed :(\n");
	
	for (int i = 0; i < numberOfFolders; ++i)
	{
				
		my_snprintf(sourcePath, sizeof(sourcePath), "%s/base/%s", appDir, folderNames[i]);
		//	my_snprintf(destPath, sizeof(destPath), "%s/base/", iphoneDocDirectory);
		my_snprintf(destPath, sizeof(destPath), "%s/SODbase/%s", iphoneDocDirectory, folderNames[i]);
	
		source = [[NSString alloc] initWithUTF8String:sourcePath];
		toPath = [[NSString alloc] initWithUTF8String:destPath];
	
		NSLog(source);
		NSLog(toPath);

		//attempt the copy
		if ([fileManager copyItemAtPath:source toPath:toPath error:&error])//;
			Com_Printf("copy base %s success!\n", folderNames[i]);
		else
			Com_Printf("copy %s folder failed :(\n", folderNames[i]);
	
		//draw the next iteration
		colour4_t color = { 255, 0, 0, 255 };
		R_Draw_Blend( 40, 150, 400*i/numberOfFolders, 20, color );	
		
		SysIPhoneSwapBuffers();
		
		[source release];
		[toPath release];
	}
	
	//TODO: delete some of the copied files
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/012.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/017.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/024.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/042.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/046.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/047.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/048.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/050.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/051.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/052.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/053.5551", iphoneDocDirectory);
	remove(destPath);
	
}


//================================
// CopyFolder 
// Copies the contents of the base folder to the SODbase folder
// This allows us to provide a smaller download
// This is all very slow.  The POSIX library might've been faster.
//================================ 
void CopyFolder()
{
	
	char sourcePath[1024];
	char destPath[1024];
	
	char *appDir;
	appDir = getenv("CWD");
	
	my_snprintf(sourcePath, sizeof(sourcePath), "%s/base/", appDir);
	//	my_snprintf(destPath, sizeof(destPath), "%s/base/", iphoneDocDirectory);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/", iphoneDocDirectory);
	
	NSString *source = [[NSString alloc] initWithUTF8String:sourcePath];
	NSString *toPath = [[NSString alloc] initWithUTF8String:destPath];
	
	NSLog(source);
	NSLog(toPath);
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSError *error;
	
	//attempt the copy
	if ([fileManager copyItemAtPath:source toPath:toPath error:&error])//;
		Com_Printf("copy base folder success!\n");
	else
		Com_Printf("copy base folder failed :(\n");
	
	
	//TODO: delete some of the copied files
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/012.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/017.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/024.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/042.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/046.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/047.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/048.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/050.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/051.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/052.5551", iphoneDocDirectory);
	remove(destPath);
	my_snprintf(destPath, sizeof(destPath), "%s/SODbase/sprites/053.5551", iphoneDocDirectory);
	remove(destPath);
}*/

//================================
// IsSpearPurchased
// returns 1 if Spear was purchased
//================================ 
int IsSpearPurchased()
{
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//create/open file for saving
	NSString *fileatpath = [documentsDirectory stringByAppendingString:@"/SpearPurchased.log"];
	NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:fileatpath];
	
	if (file)
		return 1;
	
	return 0;
}

//================================
// IsSpearInstalled 
// returns 1 if Spear is installed
//================================ 
int IsSpearInstalled()
{
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//create/open file for saving
	NSString *fileatpath = [documentsDirectory stringByAppendingString:@"/SpearInstalled.log"];
	NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:fileatpath];
	
	if (file)
		return 1;
	
	return 0;
}

//================================
// WriteDownloadLog 
// records that we've finished installing SOD
// but haven't completed the purchase
//================================ 
void WriteInstallLog()
{
	char *buffer = "useless data";
	NSData *data = [[NSData alloc] initWithBytes: (void *)buffer length: strlen(buffer)];
	
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//create/open file for saving
	NSString *fileatpath = [documentsDirectory stringByAppendingString:@"/SpearInstalled.log"];
	NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:fileatpath];
	
	if (!file) //file does not exist... create and append file
	{
		NSLog(@"open file failed\ncreating file with path: %@\n", fileatpath);
		[[NSFileManager defaultManager] createFileAtPath:fileatpath
												contents:data
											  attributes:nil];
		return;
	}
	
}

//================================
// FinalizeDownload 
// Installs the needed files for SOD and
// removes any unwanted data
//================================ 
extern void iphoneWriteConfig();
void FinalizeDownload()
{
	// get the documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *fileName = [documentsDirectory stringByAppendingString:@"/downloadedfile.tgz"];
	
	
	//TODO: after save check that the file is of the correct size... if it isn't then something was corrupted
	//      and we should try again

	//inflate the data and store in its appropriate directory
	DecompressData();
	
	//TODO: After inflating data... we should delete the downloadedfile.tgz
	if( remove( [fileName UTF8String] ) != 0 )
		Com_Printf( "Error deleting downloadedfile.tgz\n" );
	else
		Com_Printf( "downloadedfile.tgz successfully deleted\n" );
	
	
	iphoneKillMessageBox();
	
	//write a file that lets wolf3d know that SOD is installed
	WriteInstallLog();

#ifdef SPEARSTOREKIT
	//start the storekit
	BeginStoreKit();
#endif
}
