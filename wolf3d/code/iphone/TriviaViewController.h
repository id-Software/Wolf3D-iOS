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

// This class handles the trivia menu. It is based on tags - to make it work, each trivia view
// needs a unique, incrementing tag number, and the default trivia item but be set up as an
// IBOutlet.
@interface TriviaViewController : UIViewController {

@private
	IBOutlet UIView* firstTrivia;
	IBOutlet UIView* lastTrivia;
	
	UIView* currentTrivia;
}

- (IBAction)nextTrivia:(id)sender;
- (IBAction)previousTrivia:(id)sender;

- (IBAction)back:(id)sender;

@end
