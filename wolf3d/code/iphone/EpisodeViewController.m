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

#import "EpisodeViewController.h"
#import "LevelSelectViewController.h"

#import "wolfiphone.h"

#define TOTAL_EPISODES 10

static const char * const EpisodeNames[TOTAL_EPISODES][2] = {
	{ "Episode 1", "Escape from Wolfenstein" },
	{ "Episode 2", "Operation: Eisenfaust" },
	{ "Episode 3", "Die, Fuhrer, Die!" },
	{ "Episode 4", "A Dark Secret" },
	{ "Episode 5", "Trail of the Madman" },
	{ "Episode 6", "Confrontation" },
	{ "Spear of Destiny", "Tunnels" },
	{ "Spear of Destiny", "Dungeons" },
	{ "Spear of Destiny", "Castle" },
	{ "Spear of Destiny", "Ramparts" }
};
	

// A class extension to declare private methods
@interface EpisodeViewController (PrivateMethods)
- (void)startLevelSelect;
- (void)setCellSelected:(BOOL)selected atIndexPath:(NSIndexPath*)indexPath;
- (void)setCellSelected:(BOOL)selected cell:(UITableViewCell*)cell;
- (void)handleSelectionAtIndexPath:(NSIndexPath*)indexPath;
- (void)scrollToRow:(int)row;
@end

@interface EpisodeViewController ()

@property (nonatomic, retain) UITableView *episodeList;

@end

@implementation EpisodeViewController

@synthesize episodeList;
@synthesize episodeCell;

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
	
	// TODO: Select the current episode. Haven't yet figured out a good way to get the table
	// view to load with a default row selected, so for now always select episode 1.
	int initialEpisode = 0;
	
	NSIndexPath *initialPath = [NSIndexPath indexPathForRow:initialEpisode inSection:0];
	
	[self.episodeList selectRowAtIndexPath:initialPath animated:YES scrollPosition:UITableViewScrollPositionNone];
	[self handleSelectionAtIndexPath:initialPath];
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
	
	[self.episodeList release];
	self.episodeList = nil;
}


- (void)dealloc {
    [super dealloc];
}

// Sets the current episode with the sender's tag. IF the user selects a platinum episode but
// only has the lite version of the app, prompt him to buy the platinum version.
- (IBAction)setEpisode:(id)sender {
	NSInteger requestedEpisode = [sender tag];
	
	if ( SysIPhoneGetContentVersion() == CONTENT_PLATINUM || requestedEpisode == 0 ) {
		Cvar_SetValue( episode->name, [sender tag] );
		[self startLevelSelect];
	} else {
		// Prompt user to buy platinum version
		iphonePromptToBuyPlatinum();
	}
}

- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)next:(id)sender {
	LevelSelectViewController *lsc = [[LevelSelectViewController alloc] initWithNibName:@"LevelSelectView" bundle:nil];
	[self.navigationController pushViewController:lsc animated:YES];
	[lsc release];
}


- (IBAction)pageDown:(id)sender {
	// If the user taps the page down button, we want to scroll the table view so that the first
	// non-visible row becomes visible. To do this, we can get the array of visible index paths,
	// find the maximum row within the array, and scroll to the row just after that.
	
	NSArray* visibleIndexPaths = [self.episodeList indexPathsForVisibleRows];
	
	int maxRow = 0;
	
	for ( NSIndexPath* path in visibleIndexPaths ) {
		maxRow = maxRow < path.row ? path.row: maxRow;
	}
	
	const int rowToMakeVisible = maxRow + 1;
	
	[self scrollToRow:rowToMakeVisible];
	
}


- (IBAction)pageUp:(id)sender {

	// If the user taps the page up button, we want to scroll the table view so that the first
	// non-visible row becomes visible. To do this, we can get the array of visible index paths,
	// find the minimum row within the array, and scroll to the row just before that.
	
	NSArray* visibleIndexPaths = [self.episodeList indexPathsForVisibleRows];
	
	int minRow = TOTAL_EPISODES - 1;
	
	for ( NSIndexPath* path in visibleIndexPaths ) {
		minRow = minRow < path.row ? minRow: path.row;
	}
	
	const int rowToMakeVisible = minRow - 1;
	
	[self scrollToRow:rowToMakeVisible];

}

- (void)scrollToRow:(int)row {
	const int clampedRowMax = row < TOTAL_EPISODES? row: TOTAL_EPISODES-1;
	const int clampedRow = clampedRowMax < 0? 0: clampedRowMax;
	
	NSIndexPath* pathToMakeVisible = [NSIndexPath indexPathForRow:clampedRow inSection:0];
	
	[self.episodeList scrollToRowAtIndexPath:pathToMakeVisible atScrollPosition:UITableViewScrollPositionNone animated:YES];
}

- (void)startLevelSelect {
	
}






/*
========================

UITableView interface

========================
*/

- (void)handleSelectionAtIndexPath:(NSIndexPath*)indexPath {
	
	// Prompt for In-App Purchase when the user selects a level that is not currently available.
	if ( SysIPhoneGetContentVersion() == CONTENT_LITE ) {
		if ( indexPath.row > 0 ) {
			iphonePromptToBuyPlatinum();
			return;
		}
	}
	
	Cvar_SetValue( episode->name, indexPath.row );
	
	[self setCellSelected:YES atIndexPath:indexPath];
	
	/*
	int episodeIndex = episode->value;
	int levelIndex = indexPath.row;	

	
	

	realEpisode = episodeIndex;
	realMap = levelIndex;
	 
	if ( episodeIndex >= NUM_ORIGINAL_EPISODES ) {
		switch (episodeIndex)
		{
			case 6:
				if (levelIndex == 5) {
					realEpisode = 7;
					realMap = 8;
				}
				break;
			case 7: 
				realEpisode = 6;
				realMap += 5;
				break;
			case 8:
				realEpisode = 7;
				if (levelIndex == 6)
					realMap = 9;
				break;
			case 9:
				if ( levelIndex == 0) { realEpisode = 7; realMap = 6; }
				if ( levelIndex == 1) { realEpisode = 7; realMap = 7; }
				if ( levelIndex == 2) { realEpisode = 8; realMap = 0; }
				break;
			default:
				break;
		}
	}
	*/
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return TOTAL_EPISODES;
}

static CGRect maximumNameLabelFrame = { { 0.0, 0.0 }, { 0.0, 0.0 } };

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *MyIdentifier = @"MyIdentifier";
    UITableViewCell *cell = (UITableViewCell*)[self.episodeList dequeueReusableCellWithIdentifier:MyIdentifier];
	
    if (cell == nil) {
        //cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:MyIdentifier] autorelease];
		[[NSBundle mainBundle] loadNibNamed:@"EpisodeCell" owner:self options:nil];
        
		if ( episodeCell == nil ) {
			// Couldn't create from nib file, load a default cell.
			cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:MyIdentifier] autorelease];
		} else {
			cell = episodeCell;
			self.episodeCell = nil;
			
			// Save the good label size here before it gets modified by the code below.
			UILabel *episodeNameLabel;
			episodeNameLabel = (UILabel *)[cell viewWithTag:2];
			
			maximumNameLabelFrame = episodeNameLabel.frame;
		}
    }
	
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
	
	BOOL isCellSelected = ( episode->value == indexPath.row)? YES: NO;
	
	[self setCellSelected:isCellSelected cell:cell];
	
	UILabel *episodeNumberLabel;
    episodeNumberLabel = (UILabel *)[cell viewWithTag:1];
    episodeNumberLabel.text = [NSString stringWithCString:EpisodeNames[indexPath.row][0] encoding:NSASCIIStringEncoding];
 
	UILabel *episodeNameLabel;
    episodeNameLabel = (UILabel *)[cell viewWithTag:2];
    NSString* episodeNameText = [NSString stringWithCString:EpisodeNames[indexPath.row][1] encoding:NSASCIIStringEncoding];
	
	CGSize expectedLabelSize = [episodeNameText sizeWithFont:episodeNameLabel.font 
                        constrainedToSize:maximumNameLabelFrame.size 
                        lineBreakMode:episodeNameLabel.lineBreakMode]; 

	//adjust the label the the new height.
	CGRect newFrame = maximumNameLabelFrame;
	newFrame.size.height = expectedLabelSize.height;
	episodeNameLabel.frame = newFrame;
		
	episodeNameLabel.text = episodeNameText;
	
	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

	[self handleSelectionAtIndexPath:indexPath];
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath {
	[self setCellSelected:NO atIndexPath:indexPath];
}

- (void)setCellSelected:(BOOL)selected atIndexPath:(NSIndexPath*)indexPath {
	// Get the cell that was selected.
	UITableViewCell * cell = [episodeList cellForRowAtIndexPath:indexPath];

	[self setCellSelected:selected cell:cell];
}

- (void)setCellSelected:(BOOL)selected cell:(UITableViewCell*)cell {
	// Get the "selected" image
	UIView * selectionFrame = [cell viewWithTag:3];
	
	// Show the selected image
	selectionFrame.hidden = !selected;


}

@end
