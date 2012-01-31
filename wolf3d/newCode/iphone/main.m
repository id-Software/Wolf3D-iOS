/*
 
 Copyright (C) 2009 Id Software, Inc.
 
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
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    {
		char cwd[256];
		strcpy( cwd, argv[0] );
		int len = strlen( cwd );
		for( int i = len-1; i >= 0; i-- ) {
			if ( cwd[i] == '/' ) {
				cwd[i] = 0;
				break;
			}
			cwd[i] = 0;
		}
		setenv( "CWD", cwd, 1 );
	}
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
