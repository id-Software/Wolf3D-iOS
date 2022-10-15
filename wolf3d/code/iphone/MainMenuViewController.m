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

#import "MainMenuViewController.h"
#import "wolf3dAppDelegate.h"
#import "wolfiphone.h"

#import "SkillViewController.h"
#import "CreditsViewController.h"
#import "LegalViewController.h"
#import "TriviaViewController.h"

#if !TARGET_OS_TV
#import "SettingsViewController.h"
#endif

@interface MainMenuViewController ()

    @property (nonatomic, retain) IBOutlet UIButton *resumeButton;
    @property (nonatomic, retain) IBOutlet UIButton *startNewGameButton;
    @property (nonatomic, retain) IBOutlet UIButton *settingsButton;
    @property (nonatomic, retain) IBOutlet UIButton *aboutButton;
	@property (nonatomic, retain) IBOutlet UIButton *extrasButton;
	@property (nonatomic, retain) IBOutlet UIView *resumeStar;
	
	@property (nonatomic, retain) IBOutlet UIButton *creditsButton;
	@property (nonatomic, retain) IBOutlet UIButton *legalButton;
	
	@property (nonatomic, retain) IBOutlet UIButton *idGamesButton;
	@property (nonatomic, retain) IBOutlet UIButton *idSoftwareButton;
	@property (nonatomic, retain) IBOutlet UIButton *triviaButton;
	
	@property (nonatomic, retain) IBOutlet UIButton *backButton;
	
	- (void) setMainHidden:(BOOL)hide;
	- (void) setAboutHidden:(BOOL)hide;
	- (void) setExtrasHidden:(BOOL)hide;
	
@end

@implementation MainMenuViewController

@synthesize resumeButton, startNewGameButton, settingsButton, aboutButton, extrasButton, resumeStar;
@synthesize creditsButton, legalButton;
@synthesize idGamesButton, idSoftwareButton, triviaButton;
@synthesize backButton;

BOOL subMenu = NO;

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
    self.resumeButton = nil;
    self.startNewGameButton = nil;
    self.settingsButton = nil;
    self.aboutButton = nil;
    self.extrasButton = nil;
    self.resumeStar = nil;
    
    self.idGamesButton = nil;
    self.idSoftwareButton = nil;
    self.triviaButton = nil;
    
    self.creditsButton = nil;
    self.legalButton = nil;
    
    self.backButton = nil;
}

- (IBAction)resume:(id)sender {
	wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app showOpenGL];
	iphoneResume();
}


- (IBAction)newGame:(id)sender {
    wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	SkillViewController *svc = [[SkillViewController alloc] initWithNibName:[app GetNibNameForDevice:@"SkillView"] bundle:nil];
	[self.navigationController pushViewController:svc animated:YES];
	[svc release];	
}

- (IBAction)settings:(id)sender {
#if !TARGET_OS_TV
 	SettingsViewController *evc = [[SettingsViewController alloc] initWithNibName:@"SettingsView" bundle:nil];
	[self.navigationController pushViewController:evc animated:YES];
	[evc release];
#endif
}

- (IBAction)about:(id)sender {
	[self setMainHidden:YES];
	[self setAboutHidden:NO];
}

- (IBAction)extras:(id)sender {
	[self setMainHidden:YES];
	[self setExtrasHidden:NO];
}

- (IBAction)credits:(id)sender {
    wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	CreditsViewController *cvc = [[CreditsViewController alloc] initWithNibName:[app GetNibNameForDevice:@"CreditsView"] bundle:nil];
	[self.navigationController pushViewController:cvc animated:YES];
	[cvc release];
}

- (IBAction)legal:(id)sender {
    wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	LegalViewController *lvc = [[LegalViewController alloc] initWithNibName:[app GetNibNameForDevice:@"LegalView"] bundle:nil];
	[self.navigationController pushViewController:lvc animated:YES];
	[lvc release];
}

- (IBAction)idGames:(id)sender {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"itms-apps://itunes.com/apps/idsoftware"] options:@{} completionHandler:nil];
}

- (IBAction)idSoftware:(id)sender {
	currentYesNoBox = YESNO_GO_TO_WEBSITE;
	iphoneYesNoBox("Website", "This will navigate you to idsoftware.com.  Continue?");
}

- (IBAction)trivia:(id)sender {
    wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	TriviaViewController *tvc = [[TriviaViewController alloc] initWithNibName:[app GetNibNameForDevice:@"TriviaView"] bundle:nil];
	[self.navigationController pushViewController:tvc animated:YES];
	[tvc release];
	
	//menuState = IPM_TRIVIA;
	//wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	//[app showOpenGL];
}

- (IBAction)back:(id)sender {
	[self setAboutHidden:YES];
	[self setExtrasHidden:YES];
	[self setMainHidden:NO];
}

- (void)setMainHidden:(BOOL)hide {
	// Set the main menu visibility
	[self.resumeButton setHidden:hide];
	[self.startNewGameButton setHidden:hide];
	[self.settingsButton setHidden:hide];
	[self.aboutButton setHidden:hide];
	[self.extrasButton setHidden:hide];
	[self.resumeStar setHidden:hide];
    subMenu = hide;
}

- (void)setAboutHidden:(BOOL)hide {
	// Set the about menu visibility
	[self.creditsButton setHidden:hide];
	[self.legalButton setHidden:hide];
	[self.backButton setHidden:hide];
}

- (void)setExtrasHidden:(BOOL)hide {
	// Set the extras menu visibility
	[self.idGamesButton setHidden:hide];
	[self.idSoftwareButton setHidden:hide];
	[self.triviaButton setHidden:hide];
	[self.backButton setHidden:hide];
}


// REMOVE WHEN NEW MENU IS READY
- (IBAction)oldNewGame:(id)sender {
	menuState = IPM_SKILL;
	wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	[app showOpenGL];
}

#if TARGET_OS_TV
- (void)pressesBegan:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress* press in presses) {
        switch (press.type) {
            case UIPressTypeMenu:
                if (subMenu) {
                    break;
                } else {
                    [super pressesBegan: presses withEvent: event];
                }
            default:
                [super pressesBegan: presses withEvent: event];
                break;
        }
    }
}

- (void)pressesEnded:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress* press in presses) {
        switch (press.type) {
            case UIPressTypeMenu:
                if (subMenu) {
                    [self setAboutHidden:YES];
                    [self setExtrasHidden:YES];
                    [self setMainHidden:NO];
                } else {
                    [super pressesEnded: presses withEvent: event];
                }
                break;
            default:
                [super pressesEnded: presses withEvent: event];
                break;
        }
    }
}

#endif

@end
