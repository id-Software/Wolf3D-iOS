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

#import "iphone_alerts.h"
#import "wolf3dAppDelegate.h"


/*
 ==================================
 MessageBox
 Provides a basic pop-up message box
 ===================================
 */

// For now, no message boxes will be shown in tvOS. The messages will just go into oblivion.
// We need to transfer it to use UIAlertViewControllers and also be able to be presented from a utility class
// and I just don't want to spend the time on that right now -tkidd

#if !TARGET_OS_TV
UIAlertView *alert;
#endif

void InitAlert()
{
#if !TARGET_OS_TV
	alert = [[UIAlertView alloc] initWithTitle:@"Title"
									   message:@"Message"
									  delegate:nil 
							 cancelButtonTitle:@"OK" 
							 otherButtonTitles: nil];
#endif
}


void iphoneMessageBox(char *title, char *message)
{
	//check if alert exists and initialize if it isn't
#if !TARGET_OS_TV
	if (!alert)
	{
		InitAlert();
	}
	
    NSString *nsTitle = [[NSString alloc] initWithCString:title encoding:NSUTF8StringEncoding];
	NSString *nsMessage = [[NSString alloc] initWithCString:message encoding:NSUTF8StringEncoding];
	
	alert.title = nsTitle;
	alert.message = nsMessage;
	
	[alert show];
#endif
}

void iphoneKillMessageBox()
{
#if !TARGET_OS_TV
[alert dismissWithClickedButtonIndex:alert.cancelButtonIndex animated:NO];
#endif
}

/*
 ========================
 iphoneNewMessageBox()
 creates a new message box without
 drestroying the old one
 ========================
 */
void iphoneNewMessageBox(char *title, char *message)
{
#if !TARGET_OS_TV
	UIAlertView *newAlert;
	newAlert = [[UIAlertView alloc] initWithTitle:@"Title" 
									   message:@"Message"
									  delegate:nil 
							 cancelButtonTitle:@"OK" 
							 otherButtonTitles: nil];
	
	
	NSString *nsTitle = [[NSString alloc] initWithCString:title encoding:NSUTF8StringEncoding];
	NSString *nsMessage = [[NSString alloc] initWithCString:message encoding:NSUTF8StringEncoding];
	
	newAlert.title = nsTitle;
	newAlert.message = nsMessage;
	
	[newAlert show];
	[newAlert release];
#endif
}


#if 1
/*
 ==================================
 YesNoBox
 Provides a yes/no box.  The
 appdelegate responds to this
 through the UIAlerViewDelegate
 ClickedButton call.
 ===================================
 */
#if !TARGET_OS_TV
UIAlertView *alertYesNo;
#endif

void InitAlertYesNo()
{
#if !TARGET_OS_TV
	alertYesNo = [[UIAlertView alloc] initWithTitle:@"Title"
									   message:@"Message"
									  delegate:(wolf3dAppDelegate *)[UIApplication sharedApplication].delegate//nil 
							 cancelButtonTitle:@"No" 
							 otherButtonTitles:@"Yes", nil];
#endif
}

void iphoneYesNoBox(char *title, char *message)
{
#if !TARGET_OS_TV
	if (!alertYesNo)
	{
		InitAlertYesNo();
	}
	
	NSString *nsTitle = [[NSString alloc] initWithCString:title encoding:NSUTF8StringEncoding];
	NSString *nsMessage = [[NSString alloc] initWithCString:message encoding:NSUTF8StringEncoding];
	
	alertYesNo.title = nsTitle;
	alertYesNo.message = nsMessage;
	
	[alertYesNo show];
#endif
}
#endif
