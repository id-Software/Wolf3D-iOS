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

#import "UITableViewScrolling.h"
#import <UIKit/UITableView.h>

void UITableViewScrollingPageDown( UITableView * table, int totalRows ) {
	// If the user taps the page down button, we want to scroll the table view so that the first
	// non-visible row becomes visible. To do this, we can get the array of visible index paths,
	// find the maximum row within the array, and scroll to the row just after that.
	
	NSArray* visibleIndexPaths = [table indexPathsForVisibleRows];
	
	int maxRow = 0;
	
	for ( NSIndexPath* path in visibleIndexPaths ) {
		maxRow = maxRow < path.row ? path.row: maxRow;
	}
	
	const int rowToMakeVisible = maxRow + 1;
	
	UITableViewScrollingScrollToRow( table,rowToMakeVisible, totalRows );
}


void UITableViewScrollingPageUp( UITableView * table, int totalRows ) {

	// If the user taps the page up button, we want to scroll the table view so that the first
	// non-visible row becomes visible. To do this, we can get the array of visible index paths,
	// find the minimum row within the array, and scroll to the row just before that.
	
	NSArray* visibleIndexPaths = [table indexPathsForVisibleRows];
	
	int minRow = totalRows - 1;
	
	for ( NSIndexPath* path in visibleIndexPaths ) {
		minRow = minRow < path.row ? minRow: path.row;
	}
	
	const int rowToMakeVisible = minRow - 1;
	
	UITableViewScrollingScrollToRow( table,rowToMakeVisible, totalRows );
}


void UITableViewScrollingScrollToRow( UITableView * table, int row, int maxRow ) {
	
	const int clampedRowMax = row < maxRow? row: maxRow-1;
	const int clampedRow = clampedRowMax < 0? 0: clampedRowMax;
	
	NSIndexPath* pathToMakeVisible = [NSIndexPath indexPathForRow:clampedRow inSection:0];
	
	[table scrollToRowAtIndexPath:pathToMakeVisible atScrollPosition:UITableViewScrollPositionNone animated:YES];
}
