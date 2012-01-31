/*

	Copyright (C) 2004 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	files.c: Interface to file i/o layer.
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

	This  module accesses data  through a  hierarchal file system, but  the
	contents  of the file  system can be transparently  merged from several
	sources.

	The  "base  directory"  is  the  path  to  the  directory  holding  the 
	executable  and  all  game  directories.  The  sys_* files pass this to 
	host_init  in  quakeparms_t->basedir.  This  can be overridden with the 
	"-basedir"  command  line  parm  to allow code debugging in a different 
	directory.  The  base  directory  is  only   used  during  file  system 
	initialization.

	The "game directory" is the first tree on the search path and directory
	that  all  generated  files  (save  games, screen  shots, demos, config 
	files)  will  be  saved  to.  This  can  be overridden with the "-game" 
	command line parameter.   The game directory can never be changed while 
	the  application  is  executing.  This is a precaution against having a 
	malicious  server  instruct  clients  to  write  files  over areas they 
	shouldn't.

*/

#include "../wolfiphone.h"



PRIVATE char fs_gamedir[ MAX_OSPATH ];



/*
-----------------------------------------------------------------------------
 Function: FS_Gamedir -Get root directory.
 
 Parameters: Nothing.

 Returns: String with the name of the root directory.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *FS_Gamedir( void )
{
	return fs_gamedir;
}


/*
-----------------------------------------------------------------------------
 Function: FS_ListFiles -List files.
 
 Parameters:

 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE char **FS_ListFiles( char *findname, int *numfiles, unsigned musthave, unsigned canthave )
{
	char *s;
	int nfiles = 0;
	char **list = 0;

	s = FS_FindFirst( findname, musthave, canthave );
	while ( s )
	{
		if ( s[strlen(s)-1] != '.' )
			nfiles++;
		s = FS_FindNext( musthave, canthave );
	}
	FS_FindClose ();

	if ( !nfiles )
		return NULL;

	nfiles++; // add space for a guard
	*numfiles = nfiles;

	list = MM_MALLOC( sizeof( char * ) * nfiles );
	if( list == NULL )
	{
		MM_OUTOFMEM( "list" );
	}

	memset( list, 0, sizeof( char * ) * nfiles );

	s = FS_FindFirst( findname, musthave, canthave );
	nfiles = 0;
	while( s )
	{
		if( s[ strlen( s ) - 1 ] != '.' )
		{
			list[ nfiles ] = strdup( s );

			(void)my_strlwr( list[ nfiles ] );

			nfiles++;
		}
		s = FS_FindNext( musthave, canthave );
	}
	FS_FindClose();

	return list;
}


/*
-----------------------------------------------------------------------------
 Function: FS_InitFilesystem -Initialize file system.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void FS_InitFilesystem( void )
{
	char *p;
	p = getenv("CWD");
	sprintf( fs_gamedir, "%s/base", p );
}
