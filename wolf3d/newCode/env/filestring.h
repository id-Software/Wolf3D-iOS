/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	filesystem.h:  Interface to filesystem layer.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by filestring.c.

*/

#ifndef __FILESTRING_H__
#define __FILESTRING_H__


extern void FS_CreatePath( char *path );

extern void FS_FilePath( char *in, char *out );

extern char *FS_SkipPath( char *pathname );

extern void FS_StripExtension( char *in, char *out );

extern char *FS_FileExtension( char *in );

extern void FS_FileBase( char *in, char *out );

#endif /* __FILESTRING_H__ */
