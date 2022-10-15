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
#import "wolfiphone.h"

@interface CreditsViewController ()

@property (nonatomic, retain) UITableView *creditsList;

@end

@implementation CreditsViewController

@synthesize creditsList;

#define CREDITS_LINES 48

// 1 - heading 1
// 2 - heading 2
// 3 - normal text
// 4 - tiny text

static const char * const CreditText[CREDITS_LINES] = {
    "Wolfenstein 3D Classic Platinum",
    "",
    "Programming",
    "",
    "John Carmack",
    "Jeff Farrand",
    "Ryan Gerleve",
    "Greg Hodges",
    "",
    "Art",
    "",
    "John Burnett",
    "Mike Horton",
    "",
    "Audio",
    "",
    "Christian Antkow",
    "",
    "Production",
    "",
    "Rafael Brown",
    "",
    "QA Testing",
    "",
    "Sean Palomino",
    "",
    "",
    "Wolfenstein 3D and Spear of Destiny",
    "originally created by id Software",
    "",
    "",
    "Programming",
    "",
    "John Carmack",
    "",
    "Design",
    "",
    "John Romero",
    "Tom Hall",
    "",
    "Art",
    "",
    "Adrian Carmack",
    "Kevin Cloud",
    "",
    "Audio",
    "",
    "Bobby Prince"
};

static const int CreditSizes[CREDITS_LINES] = {
    1,
    4,
    2,
    4,
    3,
    3,
    3,
    3,
    4,
    2,
    4,
    3,
    3,
    4,
    2,
    4,
    3,
    4,
    2,
    4,
    3,
    4,
    2,
    4,
    3,
    4,
    4,
    5,
    5,
    4,
    4,
    2,
    4,
    3,
    4,
    2,
    4,
    3,
    3,
    4,
    2,
    4,
    3,
    3,
    4,
    2,
    4,
    3
};


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)dealloc {
    [super dealloc];
}

- (IBAction)back:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

/*
 ========================
 
 UITableView interface
 
 ========================
 */

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return CREDITS_LINES;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *MyIdentifier = @"MyIdentifier";
    
    UITableViewCell *cell = (UITableViewCell*)[self.creditsList dequeueReusableCellWithIdentifier:MyIdentifier];
    
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:MyIdentifier] autorelease];
    }
    
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    cell.textLabel.text = [NSString stringWithCString:CreditText[indexPath.row] encoding:NSASCIIStringEncoding];
    
    cell.backgroundColor = UIColor.clearColor;
    
    cell.textLabel.textColor = UIColor.whiteColor;
    
    CGFloat points = cell.textLabel.font.pointSize;
    
    switch (CreditSizes[indexPath.row]) {
        case 1:
            points = 22;
            break;
        case 2:
            points = 20;
            break;
        case 3:
            points = 17;
            break;
        case 4:
            points = 14;
            break;
        case 5:
            points = 20;
            break;

        default:
            break;
    }
    
    if (IS_IPAD) {
        cell.textLabel.font =[UIFont systemFontOfSize:points weight:UIFontWeightBold];
    } else {
        cell.textLabel.font =[UIFont systemFontOfSize:points-5 weight:UIFontWeightBold];
    }
    
    cell.textLabel.textAlignment = NSTextAlignmentCenter;
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    CGFloat size = 40;
    
    switch (CreditSizes[indexPath.row]) {
        case 1:
            size = 60;
            break;
        case 2:
            size = 40;
            break;
        case 3:
            size = 20;
            break;
        case 4:
            size = 10;
            break;
        case 5:
            size = 24;
            break;

        default:
            break;
    }
    
    return size;
}

@end
