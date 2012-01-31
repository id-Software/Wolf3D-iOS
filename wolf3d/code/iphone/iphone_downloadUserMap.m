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
#import "iphone_alerts.h"

#include <CoreFoundation/CoreFoundation.h>
#include <unistd.h>

extern  void  Com_Printf(const char* fmt, ... );
extern  void  my_snprintf( char *dest, size_t size, const char *format, ... );

extern  char  iphoneDocDirectory[1024];
extern menuState_t menuState;


int inf(FILE *source, FILE *dest);
void zerr(int ret);


char mapName[1024];// = "/";

//this is used for determining if we have the correctly sized data
const unsigned int DownloadUserFileSize = 4096; //max size of maps?


//-----------------------------
// DownloadURLConnection
// This connects to a server and downloads the file located there
//-----------------------------
void DownloadURLConnection( char *url )
{
	Com_Printf( "ConnectURL char *: %s\n", url );
	
	int length = strlen(url);
	if (length <= 4)
	{
		iphoneMessageBox("error", "url is not a valid map name.  Maps must end in \".map\"");
		return;
	}
	
	length = strlen(url);
	//acquire file name of map
	int pos = length;
	while (pos > 0)
	{
		--pos;
		if (url[pos] == '/')
			break;
	}
	++pos;
	strcpy(mapName, url + pos);
	
	//-------------------------------------------------
	//check if this file already exists... if so, delete the old one
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//get maps directory
	NSString *mapsDirectory = [documentsDirectory stringByAppendingString:@"/usermaps/"];
		
	DIR *dp = opendir ([mapsDirectory UTF8String]);
	if (dp != NULL)
	{
		struct dirent *ep;
		while ( ( ep = readdir (dp) ) )
		{
			//if you find a .DS_Store file... ignore it!
			if ( strcmp(ep->d_name, ".DS_Store") == 0 )
				continue;
			if ( strcmp(ep->d_name, ".") == 0 )
				continue;
			if ( strcmp(ep->d_name, "..") == 0 )
				continue;
			
			if (strcmp(ep->d_name, mapName) == 0)
			{
				printf("found a file with name: %s\n", mapName);
				printf("overwiting file\n");
				//TODO: delete this file
				char buffer[2048];
				sprintf(buffer, "%s%s", [mapsDirectory UTF8String], mapName);
				printf("removing at: %s\n", buffer);
				remove(buffer);
			}
		}
	}
	//-------------------------------------------------
	
	
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
		iphoneMessageBox("Connection Failed", "Can not download.  Check your internet connection, file url and try again later.");
		
		//return to main menu
		menuState = IPM_MAIN;
	}
	
	menuState = IPM_DOWNLOADPROGRESS;
}

unsigned int userDataAmount = 0;
unsigned int userDataTotalAmount = 0;
extern unsigned int totalDownloaded;

//============================
// AppendData
// adds a packet of data directly to file
//============================
void AppendUserDataToFile(NSData* data)
{
	NSUInteger length = [data length];
	
	userDataAmount = (unsigned int)length;
	userDataTotalAmount += userDataAmount;
	totalDownloaded = userDataTotalAmount;  //update the download screen with the total downloaded
	
	//get documents directory
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	
	//get maps directory
	NSString *mapsDirectory = [documentsDirectory stringByAppendingString:@"/usermaps/"];
	
	//check if maps directory exists, if not create maps directory
	mkdir([mapsDirectory UTF8String], 0755);
	
	//create/open file for saving
	NSString *fileatpath = [mapsDirectory stringByAppendingString:[NSString stringWithUTF8String:mapName]];
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

//================================
// FinalizeUserDownload 
//================================ 
extern int Level_VerifyMap( const char *levelname );
extern int iphoneGetUserMapLevelByName(const char *mapName);
void FinalizeUserDownload()
{
	char buffer[1024];
	sprintf(buffer, "usermaps/%s", mapName);

	//we need to verify that this is a true .map file
	if (Level_VerifyMap(buffer) == 0)
	{
		//it wasn't a valid map.  Inform user and delete
		iphoneMessageBox("invalid map", "downloaded file is an invalid map");
		
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [paths objectAtIndex:0];

		char buffer2[2048];
		sprintf(buffer2, "%s/usermaps/%s", [documentsDirectory UTF8String], mapName);
		remove(buffer2);
		menuState = IPM_MAIN;
		return;
	}

	//start this map immediately!
	menuState = IPM_EPISODE;
	iphonePreloadBeforePlay();
	int level = iphoneGetUserMapLevelByName(mapName);
	iphoneStartUserMap(10, level, currentMap.skill, mapName);
}
