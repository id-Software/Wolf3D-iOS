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

#import "SkillViewController.h"
#import "EpisodeViewController.h"
#import "wolf_local.h"
#import "wolf3dAppDelegate.h"

@interface SkillViewController ()

- (void)setSelectionFrame;

@property (nonatomic, retain) UIView* selectionFrame;

@end

@implementation SkillViewController

@synthesize selectionFrame;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

- (void)setSelectionFrame {

	int skillTag = skill->value + 1;

	UIView *currentDifficulty = [self.view viewWithTag:skillTag];
	self.selectionFrame.center = currentDifficulty.center;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
#if TARGET_OS_TV
    self.selectionFrame.hidden = YES;
#endif
}

- (void)viewDidLayoutSubviews {
    [self setSelectionFrame];
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    self.selectionFrame = nil;
}

- (void)dealloc {
    [super dealloc];
}



- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)next:(id)sender {
    wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	EpisodeViewController *evc = [[EpisodeViewController alloc] initWithNibName:[app GetNibNameForDevice:@"EpisodeView"] bundle:nil];
	[self.navigationController pushViewController:evc animated:YES];
	[evc release];
}


-(IBAction)canIPlayDaddy:(id)sender {
	Cvar_SetValue( skill->name, 0 );
	[self setSelectionFrame];
#if TARGET_OS_TV
    [self next:sender];
#endif
}

-(IBAction)dontHurtMe:(id)sender {
	Cvar_SetValue( skill->name, 1 );
	[self setSelectionFrame];
#if TARGET_OS_TV
    [self next:sender];
#endif
}

-(IBAction)BringEmOn:(id)sender {
	Cvar_SetValue( skill->name, 2 );
	[self setSelectionFrame];
#if TARGET_OS_TV
    [self next:sender];
#endif
}

-(IBAction)IAmDeathIncarnate:(id)sender {
	Cvar_SetValue( skill->name, 3 );
	[self setSelectionFrame];
#if TARGET_OS_TV
    [self next:sender];
#endif
}

@end
