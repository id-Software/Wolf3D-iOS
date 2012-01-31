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
UIAlertView *alert;

void InitAlert()
{
	alert = [[UIAlertView alloc] initWithTitle:@"Title" 
									   message:@"Message"
									  delegate:nil 
							 cancelButtonTitle:@"OK" 
							 otherButtonTitles: nil];
}


void iphoneMessageBox(char *title, char *message)
{
	//check if alert exists and initialize if it isn't
	if (!alert)
	{
		InitAlert();
	}
	
	NSString *nsTitle = [[NSString alloc] initWithCString:title];
	NSString *nsMessage = [[NSString alloc] initWithCString:message];
	
	alert.title = nsTitle;
	alert.message = nsMessage;
	
	[alert show];
}

void iphoneKillMessageBox()
{
	[alert dismissWithClickedButtonIndex:alert.cancelButtonIndex animated:NO];
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
	UIAlertView *newAlert;
	newAlert = [[UIAlertView alloc] initWithTitle:@"Title" 
									   message:@"Message"
									  delegate:nil 
							 cancelButtonTitle:@"OK" 
							 otherButtonTitles: nil];
	
	
	NSString *nsTitle = [[NSString alloc] initWithCString:title];
	NSString *nsMessage = [[NSString alloc] initWithCString:message];
	
	newAlert.title = nsTitle;
	newAlert.message = nsMessage;
	
	[newAlert show];
	[newAlert release];
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
UIAlertView *alertYesNo;

void InitAlertYesNo()
{
	alertYesNo = [[UIAlertView alloc] initWithTitle:@"Title" 
									   message:@"Message"
									  delegate:(wolf3dAppDelegate *)[UIApplication sharedApplication].delegate//nil 
							 cancelButtonTitle:@"No" 
							 otherButtonTitles:@"Yes", nil];	
}

void iphoneYesNoBox(char *title, char *message)
{
	if (!alertYesNo)
	{
		InitAlertYesNo();
	}
	
	NSString *nsTitle = [[NSString alloc] initWithCString:title];
	NSString *nsMessage = [[NSString alloc] initWithCString:message];
	
	alertYesNo.title = nsTitle;
	alertYesNo.message = nsMessage;
	
	[alertYesNo show];
}
#endif
