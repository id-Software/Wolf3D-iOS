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
 *	common.h:  Common definitions between client and server.
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
	This module is implemented by common.c.

*/


#ifndef __COMMON_H__
#define __COMMON_H__


//============================================================================

typedef struct sizebuf_s
{
	_boolean	allowoverflow;	// if false, do a Com_Error
	_boolean	overflowed;		// set to true if the buffer size failed
	W8	*data;
	int		maxsize;
	int		cursize;
	int		readcount;

} sizebuf_t;

extern void SZ_Init( sizebuf_t *buf, PW8 data, int length );
extern void SZ_Clear( sizebuf_t *buf );
extern void *SZ_GetSpace( sizebuf_t *buf, int length );
extern void SZ_Write( sizebuf_t *buf, void *data, int length );
extern void SZ_Print( sizebuf_t *buf, W8 *data );	// memcpy onto the sizebuf

//============================================================================


extern int	COM_Argc( void );
extern char *COM_Argv( int arg );	// range and null checked
extern void COM_ClearArgv( int arg );
extern int COM_CheckParm( char *parm );
extern void COM_AddParm( char *parm );

extern void COM_Init( void );
extern void COM_InitArgv( int argc, char *argv[] );


extern char *COM_Parse( char **data_p );
// data is an in/out parm, returns a parsed out token



//============================================================================




/////////////////////////////////////////////////////////////////////
//
//	Colour
//
/////////////////////////////////////////////////////////////////////

extern colour3_t	colourBlack;
extern colour3_t	colourRed;
extern colour3_t	colourBlue;
extern colour3_t	colourGreen;
extern colour3_t	colourWhite;


/////////////////////////////////////////////////////////////////////
//	End Colour
/////////////////////////////////////////////////////////////////////




/*
==============================================================

	MISC

==============================================================
*/


#define	ERR_FATAL		0		// exit the entire game with a popup window
#define	ERR_DROP		1		// print to console and disconnect from game
#define	ERR_QUIT		2		// not an error, just a normal exit
#define	ERR_DISCONNECT	4		// don't kill server

#define	EXEC_NOW	0		// don't return until completed
#define	EXEC_INSERT	1		// insert at current position, but don't run yet
#define	EXEC_APPEND	2		// add to end of the command buffer

#define	PRINT_ALL		0
#define PRINT_DEVELOPER	1	// only print when "developer 1"

extern void Com_BeginRedirect( int target, char *buffer, int buffersize, void (*flush) );
extern void Com_EndRedirect( void );
extern void Com_Printf( const char *fmt, ... );
extern void Com_DPrintf( const char *fmt, ... );
extern void Com_Error( int code, const char *fmt, ... );


extern char *va( char *format, ... );



extern	cvar_t	*developer;
extern	cvar_t	*log_stats;
extern	cvar_t	*logfile_active;
extern	FILE *log_stats_file;


extern void common_Init( int argc, char *argv[] );
extern void common_Frame( int msec );




/*
==============================================================

	NON-PORTABLE SYSTEM SERVICES

==============================================================
*/
extern char	*Sys_ConsoleInput( void );
extern void	Sys_ConsoleOutput( const char *string );
extern void	Sys_SendKeyEvents( void );
extern void	Sys_Error( const char *format, ... );
extern void	Sys_Quit( void );
extern char	*Sys_GetClipboardData( void );
extern void	Sys_CopyProtect( void );


/*
==============================================================

	CLIENT / SERVER SYSTEMS

==============================================================
*/
extern void Client_Init( void );




//
// button bits
//
#define	BUTTON_ATTACK		1
#define	BUTTON_USE			2
#define	BUTTON_CHANGE_WEAPON 4
#define BUTTON_ALTERNATE_ATTACK	8	//gsh
#define	BUTTON_ANY			128			// any key whatsoever




// usercmd_t is sent to the server each client frame
typedef struct usercmd_s
{
	W8	msec;
	W8	buttons;
	short	angles[ 3 ];
	short	forwardmove, sidemove, upmove;
	W8	impulse;		// remove?
	W8	lightlevel;		// light level the player is standing on

} usercmd_t;






#endif /* __COMMON_H__ */

