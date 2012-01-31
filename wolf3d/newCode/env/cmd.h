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
 *	cmd.h:	Command text buffering and command execution.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

/*

	Notes:

	Any number of commands can be added in a frame, from several different sources.
	Most commands come from either keybindings or console line input, but remote
	servers can also send across commands and entire text files can be execed.

	The + command line options are also added to the command buffer.

	The game starts with a Cbuf_AddText( "exec DEFAULT.CFG\n" ); Cbuf_Execute();

*/

#ifndef __CMD_H__
#define __CMD_H__

#include "arch.h"


typedef enum {

	EXEC_NOW,		// don't return until completed
	EXEC_INSERT,	// insert at current position, but don't run yet
	EXEC_APPEND		// add to end of the command buffer

} execwhen_t;


extern void Cbuf_AddText( const char *text );
// as new commands are generated from the console or keybindings,
// the text is added to the end of the command buffer.

extern void Cbuf_InsertText( char *text );
// when a command wants to issue other commands immediately, the text is
// inserted at the beginning of the buffer, before any remaining unexecuted
// commands.

extern void Cbuf_ExecuteText( execwhen_t exec_when, char *text );
// this can be used in place of either Cbuf_AddText or Cbuf_InsertText

extern void Cbuf_AddEarlyCommands( _boolean clear );
// adds all the +set commands from the command line

extern _boolean Cbuf_AddLateCommands( void );
// adds all the remaining + commands from the command line
// Returns true if any late commands were added, which
// will keep the demoloop from immediately starting

extern void Cbuf_Execute( void );
// Pulls off \n terminated lines of text from the command buffer and sends
// them through Cmd_ExecuteString.  Stops when the buffer is empty.
// Normally called once per frame, but may be explicitly invoked.
// Do not call inside a command function!

extern void Cbuf_CopyToDefer( void );
extern void Cbuf_InsertFromDefer( void );
// These two functions are used to defer any pending commands while a map
// is being loaded

//===========================================================================

#define	MAX_STRING_CHARS	1024	// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS	80		// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS		128		// max length of an individual token

/*

Command execution takes a NUL-terminated string, breaks it into tokens,
then searches for a command or variable that matches the first token.

*/

typedef void (*xcommand_t) (void);

extern void	Cmd_Init( void );

extern void	Cmd_AddCommand( char *cmd_name, xcommand_t function );
// called by the init functions of other parts of the program to
// register commands and functions to call for them.
// The cmd_name is referenced later, so it should not be in temp memory
// if function is NULL, the command will be forwarded to the server
// as a clc_stringcmd instead of executed locally
extern void Cmd_RemoveCommand( char *cmd_name );

extern _boolean Cmd_Exists( char *cmd_name );
// used by the cvar code to check for cvar / command name overlap

extern char	*Cmd_CompleteCommand( char *partial );
// attempts to match a partial command for automatic command line completion
// returns NULL if nothing fits

extern int	Cmd_Argc( void );
extern char	*Cmd_Argv( int arg );
extern char	*Cmd_Args( void );
// The functions that execute commands get their parameters with these
// functions. Cmd_Argv () will return an empty string, not a NULL
// if arg > argc, so string operations are always safe.

extern void	Cmd_TokenizeString( char *text, _boolean macroExpand );
// Takes a NUL-terminated string.  Does not need to be /n terminated.
// breaks the string up into arg tokens.

extern void	Cmd_ExecuteString( char *text );
// Parses a single line of text into arguments and tries to execute it
// as if it was typed at the console

//extern void	Cmd_ForwardToServer( void );
// adds the current command line as a clc_stringcmd to the client message.
// things like godmode, noclip, etc, are commands directed to the server,
// so when they are typed in at the console, they will need to be forwarded.






#endif /* __CMD_H__ */
