/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 1997-2001 Id Software, Inc.
	Copyright (C) 1995 Spencer Kimball and Peter Mattis

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
 *	tga.h:   Handle Targa file format.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was 
 *	originally written by id Software, Inc.
 *
 *	Portion of this code was derived from The GIMP -- an image manipulation
 *	program, and was originally written by Spencer Kimball and Peter Mattis.
 */

#ifndef __TGA_H__
#define __TGA_H__

extern void LoadTGA( const char *filename, W8 **pic, W16 *width, W16 *height, W16 *bytes );



extern W8 WriteTGA( const char *filename, W16 bpp, W16 width, W16 height, 
            void *Data, W8 upsideDown, W8 rle );


#endif /* __TGA_H__ */

