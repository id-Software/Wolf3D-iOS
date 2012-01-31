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

#import "TriviaViewController.h"

#define TRIVIA_COUNT	4

@interface TriviaViewController ()

    @property (nonatomic, retain) IBOutlet UIView *firstTrivia;
	@property (nonatomic, retain) IBOutlet UIView *lastTrivia;
	
    @property (nonatomic, retain) UIView *currentTrivia;

@end	
	
@implementation TriviaViewController

@synthesize firstTrivia, lastTrivia, currentTrivia;

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


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.currentTrivia = self.firstTrivia;
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
	
	self.firstTrivia = nil;
	self.lastTrivia = nil;
	
	self.currentTrivia = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (IBAction)nextTrivia:(id)sender {
	// Hide the current trivia
	self.currentTrivia.hidden = YES;
	
	if ( self.currentTrivia == self.lastTrivia ) {
		// If we are on the last trivia, we need to loop back to the first one.
		self.currentTrivia = self.firstTrivia;
	} else {
		// Otherwise, just go to the trivia with the next tag.
		NSInteger currentTriviaTag = self.currentTrivia.tag;
		NSInteger nextTriviaTag = currentTriviaTag + 1;
		
		self.currentTrivia = [self.view viewWithTag:nextTriviaTag];
	}
	
	// Now that the currentTrivia has been reassigned, we can show the new one.
	self.currentTrivia.hidden = NO;
}

- (IBAction)previousTrivia:(id)sender {
	// Hide the current trivia
	self.currentTrivia.hidden = YES;
	
	if ( self.currentTrivia == self.firstTrivia ) {
		// If we are on the first trivia, we need to loop back to the last one.
		self.currentTrivia = self.lastTrivia;
	} else {
		// Otherwise, just go to the trivia with the previous tag.
		NSInteger currentTriviaTag = self.currentTrivia.tag;
		NSInteger nextTriviaTag = currentTriviaTag - 1;
		
		self.currentTrivia = [self.view viewWithTag:nextTriviaTag];
	}
	
	// Now that the currentTrivia has been reassigned, we can show the new one.
	self.currentTrivia.hidden = NO;
}

- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

@end
