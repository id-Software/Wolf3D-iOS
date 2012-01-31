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

#import <UIKit/UIKit.h>

@interface MainMenuViewController : UIViewController {

@private
	// Main menu
	IBOutlet UIButton *resumeButton;
	IBOutlet UIButton *newGameButton;
	IBOutlet UIButton *settingsButton;
	IBOutlet UIButton *aboutButton;
	IBOutlet UIButton *extrasButton;
	IBOutlet UIView* resumeStar;
	
	
	// About menu
	IBOutlet UIButton *creditsButton;
	IBOutlet UIButton *legalButton;
	
	// Extras menu
	IBOutlet UIButton *idGamesButton;
	IBOutlet UIButton *idSoftwareButton;
	IBOutlet UIButton *triviaButton;
	
	// Shared
	IBOutlet UIButton *backButton;
}

- (IBAction)resume:(id)sender;
- (IBAction)newGame:(id)sender;
- (IBAction)settings:(id)sender;
- (IBAction)about:(id)sender;
- (IBAction)extras:(id)sender;

- (IBAction)credits:(id)sender;
- (IBAction)legal:(id)sender;
- (IBAction)back:(id)sender;

- (IBAction)idGames:(id)sender;
- (IBAction)idSoftware:(id)sender;
- (IBAction)trivia:(id)sender;

- (IBAction)oldNewGame:(id)sender;

@end
