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
 *	file.h:   Access methods to file system services.
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
	This module is implemented by file.c

	Non-portable methods are implemented in:
	win32/win_file.c for Windows
	unix/unix_file.c for unix

*/

#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include <time.h>

#include "../../../common/arch.h"


// filename/path manipulation
extern char *FS_SkipPath( char *pathname );
extern void FS_StripExtension( char *in, char *out );
extern char *FS_FileExtension( char *in );
extern void FS_FileBase( char *in, char *out );
extern void FS_FilePath( char *in, char *out );



extern W32 UnixTimeToDosTime( time_t *t );


/////////////////////////////////////////////////////////////////////
//
//	PORTABLE FILE SYSTEM SERVICES
//
/////////////////////////////////////////////////////////////////////
extern SW32 FS_FileLength( FILE *filestream );
extern void FS_Read( void *buffer, int len, FILE *f );
extern SW32 FS_FOpenFile( const char *filename, FILE **file );
extern SW32 FS_LoadFile( const char *path, void **buffer );
extern void FS_FreeFile( void *buffer );


 




/////////////////////////////////////////////////////////////////////
//
//	NON-PORTABLE FILE SYSTEM SERVICES
//
/////////////////////////////////////////////////////////////////////
extern W8 FS_Mkdir( const char *dirname );
extern W8 FS_ChangeCurrentDirectory( const char *path );

extern _boolean FS_DeleteFile( const char *filename );
extern _boolean FS_RemoveDirectory( const char *pathname );


// directory/file attributes
#define FA_ARCH		0x01
#define FA_HIDDEN	0x02
#define FA_RDONLY	0x04
#define FA_DIR		0x08
#define FA_SYSTEM	0x10

// pass in an attribute mask of things you wish to REJECT
extern char *FS_FindFirst( const char *path, W32 musthave, W32 canthave );
extern char *FS_FindNext( W32 musthave, W32 canthave );
extern void FS_FindClose( void );


struct filestats
{
	W32 attributes;			/* The following attributes are defined as FA_ARCH to FA_SYSTEM*/
	W32 creationtime;		/* Time when file data last modified */		
	W32 lastaccesstime;		/* Time when file data last accessed */
	W32 lastwritetime;		/* Time when file data last modified */
};

extern _boolean FS_GetFileAttributes( const char *filename, struct filestats *fs );


#endif /* __FILE_H__ */
