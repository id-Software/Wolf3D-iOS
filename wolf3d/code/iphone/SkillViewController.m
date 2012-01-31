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

	[self setSelectionFrame];
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	
	self.selectionFrame = nil;
}


- (void)dealloc {
    [super dealloc];
}



- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)next:(id)sender {
	EpisodeViewController *evc = [[EpisodeViewController alloc] initWithNibName:@"EpisodeView" bundle:nil];
	[self.navigationController pushViewController:evc animated:YES];
	[evc release];
}


-(IBAction)canIPlayDaddy:(id)sender {
	Cvar_SetValue( skill->name, 0 );
	[self setSelectionFrame];
}

-(IBAction)dontHurtMe:(id)sender {
	Cvar_SetValue( skill->name, 1 );
	[self setSelectionFrame];
}

-(IBAction)BringEmOn:(id)sender {
	Cvar_SetValue( skill->name, 2 );
	[self setSelectionFrame];
}

-(IBAction)IAmDeathIncarnate:(id)sender {
	Cvar_SetValue( skill->name, 3 );
	[self setSelectionFrame];
}

@end
