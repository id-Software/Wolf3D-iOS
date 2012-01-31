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

#import "LevelSelectViewController.h"
#import "wolf3dAppDelegate.h"
#import "wolfiphone.h"
#import "MissionTableViewCell.h"
#import "UITableViewScrolling.h"

#define NUM_ORIGINAL_EPISODES	6
#define NUM_SPEAR_EPISODES		4

#define MAX_SPEAR_MAPS_PER_EPISODE	7

extern int returnButtonFrameNum;


static int numLevelsTable[] = {
	10, 10, 10, 10, 10, 10, // All six classic episodes have 10 levels
	// Spear of Destiny episodes
	6,		// Tunnels
	5,		// Dungeon
	7,		// Castle
	3 };	// Ramparts
	

// Private properties and methods
@interface LevelSelectViewController ()

@property (nonatomic, retain) UITableView *missionList;

@end

@implementation LevelSelectViewController

@synthesize missionList;

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
	
    [self.missionList reloadData];
	
	// Select the first row by default
	int levelToSelect = 0;
	
	if ( (int)episode->value == currentMap.episode ) {
		levelToSelect = currentMap.map;
	}
	
	NSIndexPath *ip=[NSIndexPath indexPathForRow:levelToSelect inSection:0];
    [self.missionList selectRowAtIndexPath:ip animated:YES scrollPosition:UITableViewScrollPositionNone];
	[self handleSelectionAtIndexPath:ip];
}


/*
========================

UITableView interface

========================
*/

- (void)handleSelectionAtIndexPath:(NSIndexPath*)indexPath {
	int episodeIndex = episode->value;
	int levelIndex = indexPath.row;	

	// Prompt for In-App Purchase when the user selects a level that is not currently available.
	if ( SysIPhoneGetContentVersion() == CONTENT_LITE ) {
		if ( levelIndex >= 3 ) {
			iphonePromptToBuyPlatinum();
			return;
		}
	}
	

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
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of levels based on the episode
	int episodeIndex = episode->value;
	

	
	return numLevelsTable[ episodeIndex ];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *MyIdentifier = @"MyIdentifier";
    MissionTableViewCell *cell = (MissionTableViewCell*)[self.missionList dequeueReusableCellWithIdentifier:MyIdentifier];
    if (cell == nil) {
        cell = [[[MissionTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:MyIdentifier] autorelease];
    }
    
	// Cell configuration
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
	
	// Cell text configuration	
	NSString* levelName = [NSString stringWithFormat:@"Level %d", indexPath.row + 1 ];
	
	int episodeIndex = episode->value;
	int levelIndex = indexPath.row;

	if ( episodeIndex < NUM_ORIGINAL_EPISODES ) {
		// Wolfenstein episodes are the first six, and they all follow the pattern of 8 levels,
		// one boss level, and one secret level.
		if ( levelIndex == 8 ) {
			levelName = @"Boss";
		} else if ( levelIndex == 9 ) {
			levelName = @"Secret";
		}
	} else {
		// These are the Spear of Destiny levels.
		
		// Level name lookup
		static const char* const spearLevelNameTable[ NUM_SPEAR_EPISODES ][ MAX_SPEAR_MAPS_PER_EPISODE ] = {
			{ "Level 1", "Level 2", "Level 3", "Level 4", "Boss", "Secret", NULL },
			{ "Level 1", "Level 2", "Level 3", "Level 4", "Boss", NULL, NULL },
			{ "Level 1", "Level 2", "Level 3", "Level 4", "Level 5", "Boss", "Secret" },
			{ "Ramparts", "Death Knight", "Dungeon Dimension", NULL, NULL, NULL, NULL } };
			
		const char * spearLevelName = "";
		
		int spearEpisode = episodeIndex - NUM_ORIGINAL_EPISODES;
		
		if ( 0 <= spearEpisode && spearEpisode < NUM_SPEAR_EPISODES ) {
			spearLevelName = spearLevelNameTable[ spearEpisode ][ levelIndex ];
		}
		
		levelName = [NSString stringWithCString:spearLevelName encoding:NSASCIIStringEncoding ];		
	}
	
	cell.textLabel.text = levelName;
	
	cell.textLabel.textColor = [UIColor lightGrayColor];
	cell.textLabel.highlightedTextColor = [UIColor colorWithRed:98.0/255.0 green:149.0/255.0 blue:212.0/255.0 alpha:1.0];
	
	return cell;
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell
										   forRowAtIndexPath:(NSIndexPath *)indexPath {
	
	int fontSize = 32;
	
	if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad ) {
		fontSize = 48;
	}
	
	cell.textLabel.font = [UIFont fontWithName:@"POSITYPE idSettler v10.2"
								  size:fontSize];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

	[self handleSelectionAtIndexPath:indexPath];
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
	
	self.missionList = nil;
	
}


- (void)dealloc {
    [super dealloc];
}


- (IBAction)next:(id)sender {
	
	int levelNum = realEpisode*10+realMap;
	int ch = currentMap.mapFlags[ (int)skill->value ][levelNum];

	// don't let them go to the secret level unless they earned it
	if ( (int)episode->value >= NUM_ORIGINAL_EPISODES ) {
		if ( (levelNum == 78 || levelNum == 79)  && !( ch & MF_TRIED ) ) {
			return;
		}
	} else {
		if ( realMap == 9 && !( ch & MF_TRIED ) ) {
			return;
		}
	}
	
	wolf3dAppDelegate* app = (wolf3dAppDelegate*)[[UIApplication sharedApplication] delegate];
	
	[app stopMenuMusic];
	
	[app showOpenGL];
	
	
	PL_NewGame( &Player );
	iphonePreloadBeforePlay();
	
	int clampedSkill = skill->value;
	if ( clampedSkill < 0 ) {
		clampedSkill = 0;
	}
	if ( clampedSkill > 3 ) {
		clampedSkill = 3;
	}
	
	iphoneStartMap( realEpisode, realMap, clampedSkill );
	iphoneFrameNum = 0;
	returnButtonFrameNum = 100;
	
	Sound_StartBGTrack( levelData.musicName, levelData.musicName);
}

- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)pageDown:(id)sender {
	UITableViewScrollingPageDown( self.missionList, numLevelsTable[(int)episode->value] );
}


- (IBAction)pageUp:(id)sender {
	UITableViewScrollingPageUp( self.missionList, numLevelsTable[(int)episode->value] );
}


@end


