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

#import "CreditsViewController.h"
#import <QuartzCore/CAAnimation.h>

#define CREDITS_ANIMATION_POINTS_PER_SECOND		( 320.0f / 6.0f ) // Six seconds to scroll up
																  // an iPhone screen.

@interface CreditsViewController ()

	@property (nonatomic, retain) UIView* creditsRoll;

@end

@implementation CreditsViewController

@synthesize creditsRoll;

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
	
	// This is the starting position of the credits text.
	CGPoint startPoint = self.creditsRoll.center;
	
	// Set up the end point. We can stop the animation as soon as the bottom of the credits
	// get to the top of the screen.
	CGPoint endPoint = startPoint;
	
	endPoint.y = -self.creditsRoll.bounds.size.height;
	
	// Start credits animation
	CABasicAnimation *theAnimation;
 
	theAnimation=[CABasicAnimation animationWithKeyPath:@"position"];
	
	CGFloat pointDistance = endPoint.y - startPoint.y;
	
	theAnimation.duration= fabs( pointDistance ) * ( 1.0f / CREDITS_ANIMATION_POINTS_PER_SECOND );
	
	theAnimation.fromValue=[NSValue valueWithCGPoint:startPoint];
	theAnimation.toValue=[NSValue valueWithCGPoint:endPoint];
	
	[self.creditsRoll.layer addAnimation:theAnimation forKey:@"animatePosition"];
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
	self.creditsRoll = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

@end
