/*
 
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 

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

#import "WolfSlider.h"


@implementation WolfSlider


- (CGRect)trackRectForBounds:(CGRect)bounds {

	UIImage* trackImage = [self minimumTrackImageForState:UIControlStateNormal];
	
	CGFloat trackImageHeight = trackImage.size.height;
    
    //    return CGRectMake(bounds.origin.x, bounds.origin.y, self.bounds.size.height, trackImageHeight);
    return CGRectMake(bounds.origin.x, bounds.origin.y, self.bounds.size.width, trackImageHeight);
}


@end
