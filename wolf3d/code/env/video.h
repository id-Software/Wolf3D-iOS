/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 1997-2001 Id Software, Inc.

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

/*
 *	video.h -- video driver defs.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by vid_sys.c.

*/



#ifndef __VIDEO_H__
#define __VIDEO_H__

typedef struct vrect_s
{
	int	x, y, width, height;

} vrect_t;

typedef struct
{
	unsigned width, height;	// coordinates from main game
} viddef_t;

extern	viddef_t	viddef;	 // global video state

// Video module initialisation etc
extern void	Video_Init( void );
extern void	Video_Shutdown( void );
extern void	Video_CheckChanges( void );


extern void VID_NewWindow ( int width, int height );
extern _boolean VID_GetModeInfo( int *width, int *height, int mode );


#endif /* __VIDEO_H__ */
