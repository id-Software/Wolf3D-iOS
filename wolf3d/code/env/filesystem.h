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
	This module is implemented by files.c.

*/

#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "arch.h"

#define	MAX_GAMEPATH	256	// max length of a game pathname
#define	MAX_OSPATH		256	// max length of a filesystem pathname


extern void	FS_InitFilesystem(void);
extern char	*FS_Gamedir(void);
//extern char *FS_ForceGamedir(void); //gsh



/////////////////////////////////////////////////////////////////////
//
//	PORTABLE FILE SYSTEM SERVICES
//
/////////////////////////////////////////////////////////////////////

typedef struct
{
	FILE *hFile;

	/* Following is used when the file is loaded into memory */
	_boolean bLoaded;			/* Was file loaded into memory? */
	W32	filesize;				/* Size of file data in bytes */

	W8	*ptrStart;				/* pointer to start of file data block */
	W8	*ptrCurrent;			/* pointer to current position in file data block */
 	W8	*ptrEnd;				/* pointer to end of file data block */
	
	void *filedata;				/* file data loaded into memory */

} filehandle_t;


/* Desired Access Flags */
#define DA_GENERIC_READ			0x01	
#define DA_GENERIC_WRITE		0x02	

/* Flags and Attributes */
#define FA_FILE_FLAG_LOAD		0x01
#define FA_FILE_FLAG_CACHE		0x02
#define FA_FILE_IPHONE_DOC_DIR 	0x04

extern filehandle_t *FS_OpenFile( const char *filename, W32 FlagsAndAttributes );
extern void FS_CloseFile( filehandle_t *fhandle );
// note: this can't be called from another DLL, due to MS libc issues

extern SW32 FS_ReadFile( void *buffer, W32 size, W32 count, filehandle_t *fhandle );

extern SW32 FS_FileTell( filehandle_t *fhandle );

extern W32 FS_FileSeek( filehandle_t *fhandle, SW32 offset, W32 origin );

extern SW32 FS_GetFileSize( filehandle_t *fhandle );

extern void *FS_GetLoadedFilePointer( filehandle_t *fhandle, W32 origin );



extern void	FS_CreatePath( char *path );

extern void FS_FilePath( char *in, char *out );

extern char *FS_SkipPath( char *pathname );

/////////////////////////////////////////////////////////////////////
//
//	NON-PORTABLE FILE SYSTEM SERVICES
//
/////////////////////////////////////////////////////////////////////

extern W8 FS_CreateDirectory( const char *dirname );
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





#endif /* __FILESYSTEM_H__ */

