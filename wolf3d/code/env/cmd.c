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
 *	cmd.c:	Script command processing module.
 *	
 *	Author:	Michael Liebscher 	 
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"



//void Cmd_ForwardToServer( void );

#define	MAX_ALIAS_NAME	32

typedef struct cmdalias_s
{
	struct	cmdalias_s	*next;
	char	name[ MAX_ALIAS_NAME ];
	W32		id;
	char	*value;

} cmdalias_t;

cmdalias_t	*cmd_alias;

_boolean	cmd_wait;

#define	ALIAS_LOOP_COUNT	16
int		alias_count;		// for detecting runaway loops


//=============================================================================

/*
-----------------------------------------------------------------------------
 Function: Cmd_Wait_f -Wait command.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
	Causes execution of the remainder of the command buffer to be delayed until
	next frame. This allows commands like:
	bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_Wait_f( void )
{
	cmd_wait = true;
}


/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

PRIVATE sizebuf_t	cmd_text;

PRIVATE W8	cmd_text_buf[ 8192 ];
PRIVATE W8	defer_text_buf[ 8192 ];


/*
-----------------------------------------------------------------------------
 Function: Cmd_Init -Allocates an initial text buffer that will grow as needed.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: This must be called before using command buffer.

-----------------------------------------------------------------------------
*/
PRIVATE void Cbuf_Init( void )
{
	SZ_Init( &cmd_text, cmd_text_buf, sizeof( cmd_text_buf ) );
}


/*
-----------------------------------------------------------------------------
 Function: Cbuf_AddText -Adds command text at the end of the buffer.
 
 Parameters: text -[in] Ponter to NUL-terminated string.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_AddText( const char *text )
{
	W32	length;
	
	length = strlen( text );

	if( cmd_text.cursize + length >= cmd_text.maxsize )
	{
		Com_Printf( "Cbuf_AddText: overflow\n" );
		return;
	}
	
	SZ_Write( &cmd_text, (void *)text, length );
}

/*
-----------------------------------------------------------------------------
 Function: Cbuf_InsertText -Adds command text immediately after the current 
							command.
 
 Parameters: text -[in] Command text to add to buffer.
 
 Returns: Nothing.
 
 Notes: 
	Adds a \n to the text
	FIXME: actually change the command buffer to do less copying
-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_InsertText( char *text )
{
	char	*temp;
	size_t	templen;

// copy off any commands still remaining in the exec buffer
	templen = cmd_text.cursize;
	if( templen )
	{
		temp = Z_Malloc( templen );
		memcpy( temp, cmd_text.data, templen );
		SZ_Clear( &cmd_text );
	}
	else
	{
		temp = NULL;	// shut up compiler
	}
		
// add the entire text of the file
	Cbuf_AddText( text );
	
// add the copied off data
	if( templen )
	{
		SZ_Write( &cmd_text, temp, templen );
		Z_Free( temp );
	}
}


/*
-----------------------------------------------------------------------------
 Function: Cbuf_CopyToDefer -Copy command buffer to defer buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_CopyToDefer( void )
{
	memcpy( defer_text_buf, cmd_text_buf, cmd_text.cursize );
	defer_text_buf[ cmd_text.cursize ] = 0;
	cmd_text.cursize = 0;
}

/*
-----------------------------------------------------------------------------
 Function: Cbuf_InsertFromDefer -Insert commands from defer buffer to command
								buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_InsertFromDefer( void )
{
	Cbuf_InsertText( (char *)defer_text_buf );
	defer_text_buf[ 0 ] = 0;
}


/*
-----------------------------------------------------------------------------
 Function: Cbuf_ExecuteText -Execute string.
 
 Parameters: exec_when -[in] see execwhen_t definition.
			 text -[in] string with command to execute.
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_ExecuteText( execwhen_t exec_when, char *text )
{
	switch( exec_when )
	{
		case EXEC_NOW:
			Cmd_ExecuteString( text );
			break;

		case EXEC_INSERT:
			Cbuf_InsertText( text );
			break;

		case EXEC_APPEND:
			Cbuf_AddText( text );
			break;

		default:
			Com_DPrintf( "Cbuf_ExecuteText: bad exec_when" );
	}
}

/*
-----------------------------------------------------------------------------
 Function: Cbuf_Execute -execute string from command buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_Execute( void )
{
	int		i;
	char	*text;
	char	line[ 1024 ];
	int		quotes;

	alias_count = 0;		// don't allow infinite alias loops

	while( cmd_text.cursize )
	{
// find a \n or ; line break
		text = (char *)cmd_text.data;

		quotes = 0;
		for( i = 0; i < cmd_text.cursize; ++i )
		{
			if( text[ i ] == '"' )
			{
				quotes++;
			}

			if( !(quotes & 1) &&  text[ i ] == ';' )
			{
				break;	// don't break if inside a quoted string
			}

			if( text[ i ] == '\n' || text[ i ] == '#' )
			{
				break; // break on a newline or a hash mark
			}
		}
			
				
		memcpy( line, text, i );
		line[ i ] = '\0';	// NUL-terminate string
		
// delete the text from the command buffer and move remaining commands down
// this is necessary because commands (exec, alias) can insert data at the
// beginning of the text buffer

		if( i == cmd_text.cursize )
		{
			cmd_text.cursize = 0;
		}
		else
		{
			i++;
			cmd_text.cursize -= i;
			memmove( text, text+i, cmd_text.cursize );
		}

// execute the command line
		Cmd_ExecuteString( line );

		
		if( cmd_wait )
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		}
	}
}



/*
-----------------------------------------------------------------------------
 Function: Cbuf_AddEarlyCommands -Adds command line parameters as script 
									statements.
 
 Parameters: clear -[in] Remove item from global array com_argv.
 
 Returns: Nothing.
 
 Notes: 
 		Adds command line parameters as script statements
		Commands lead with a +, and continue until another +

		Set commands are added early, so they are guaranteed to be set 
		before the client and server initialize for the first time.

		Other commands are added late, after all initialization is complete.
-----------------------------------------------------------------------------
*/
PUBLIC void Cbuf_AddEarlyCommands( _boolean clear )
{
	int		i;
	char	*s;

	for( i = 0; i < COM_Argc(); ++i )
	{
		s = COM_Argv( i );

		if( strcmp( s, "+set" ) )
			continue;

		Cbuf_AddText( va("set %s %s\n", COM_Argv(i+1), COM_Argv(i+2)));
		if( clear )
		{
			COM_ClearArgv( i );
			COM_ClearArgv( i+1 );
			COM_ClearArgv( i+2 );
		}
		i += 2;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Cbuf_AddLateCommands -Adds command line parameters as script 
								statements.
 
 Parameters: Nothing.
 
 Returns: true if any late commands were added, otherwise false.
 
 Notes: 
	Commands lead with a + and continue until another + or -
	application.exe +map amlev1

	Returns true if any late commands were added, which
	will keep the demoloop from immediately starting
-----------------------------------------------------------------------------
*/
PUBLIC _boolean Cbuf_AddLateCommands( void )
{
	int		i, j;
	int		s;
	char	*text, *build, c;
	int		argc;
	_boolean	ret;

// build the combined string to parse from
	s = 0;
	argc = COM_Argc();
	for( i = 1; i < argc; ++i )
	{
		s += strlen( COM_Argv( i ) ) + 1;
	}

	if( ! s )
	{
		return false;
	}
		
	text = Z_Malloc( s + 1 );
	text[ 0 ] = '\0';	// Start with a NUL-terminated string.

	for( i = 1; i < argc; ++i )
	{
		my_strlcat( text, COM_Argv( i ), s );
		if( i != argc-1 )
		{
			my_strlcat( text, " ", s );
		}
	}
	
// pull out the commands
	build = Z_Malloc( s + 1 );
	build[ 0 ] = '\0'; // Start with a NUL-terminated string.
	
	for( i = 0; i < s-1; ++i )
	{
		if( text[ i ] == '+' )
		{
			i++;

			for( j = i ; (text[ j ] != '+') && (text[ j ] != '-') && (text[ j ] != 0) ; j++ )
				;

			c = text[ j ];
			text[ j ] = 0;
			
			my_strlcat( build, text+i, s+1 );
			my_strlcat( build, "\n", s+1 );
			text[ j ] = c;
			i = j - 1;
		}
	}

	ret = (build[ 0 ] != 0);
	if( ret )
	{
		Cbuf_AddText (build);
	}
	
	Z_Free( text );
	Z_Free( build );

	return ret;
}


/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/



/*
-----------------------------------------------------------------------------
 Function: Cmd_Exec_f -Execute script file.
 
 Parameters: Nothing.            
 
 Returns: Nothing.
 
 Notes: Console function, exec <filename>

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_Exec_f( void )
{
	filehandle_t *hfile;
	char	*f2;
	int		len;

	if( Cmd_Argc () != 2 )
	{
		Com_Printf( "exec <filename> : execute a script file\n" );
		return;
	}

	hfile = FS_OpenFile( Cmd_Argv( 1 ), FA_FILE_IPHONE_DOC_DIR );
	if( ! hfile )
	{
		Com_Printf( "couldn't exec %s\n", Cmd_Argv( 1 ) );
		return;
	}

	len = FS_GetFileSize( hfile );

	Com_Printf( "execing %s\n", Cmd_Argv( 1 ) );
	
	// the file doesn't have a trailing 0, so we need to copy it off
	f2 = Z_Malloc( len + 1 );
	memcpy( f2, hfile->filedata, len );
	f2[ len ] = 0;

	printf( "%s", f2 );		// !@#
	Cbuf_InsertText( f2 );

	Z_Free( f2 );

	FS_CloseFile( hfile );
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_Echo_f -Prints the rest of the line to the console.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_Echo_f( void )
{
	int	i;
	
	for( i = 1 ; i < Cmd_Argc() ; ++i )
	{
		Com_Printf( "%s ",Cmd_Argv( i ) );
	}
	
	Com_Printf( "\n" );
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_Alias_f -Creates a new command that executes a command 
						string (possibly ; seperated).
 
 Parameters: Nothing.
 
 Returns: Nothing. 
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_Alias_f( void )
{
	cmdalias_t	*a;
	char		cmd[ 1024 ];
	int			i, c;
	char		*s;
	W32	hashid;

	if( Cmd_Argc() == 1 )
	{
		Com_Printf( "Current alias commands:\n" );
		for( a = cmd_alias ; a ; a = a->next )
		{
			Com_Printf( "%s : %s\n", a->name, a->value );
		}
		return;
	}

	s = Cmd_Argv( 1 );
	if( strlen( s ) >= MAX_ALIAS_NAME )
	{
		Com_Printf( "Alias name is too long\n" );
		return;
	}

	hashid = my_strhash( s );

	// if the alias already exists, reuse it
	for( a = cmd_alias ; a ; a = a->next )
	{
		if( hashid == a->id )
		{
			Z_Free( a->value );
			break;
		}
	}

	if( ! a )
	{
		a = Z_Malloc( sizeof( cmdalias_t ) );
		a->next = cmd_alias;
		cmd_alias = a;
	}	
	my_strlcpy( a->name, s, sizeof( a->name ) );
	a->id = hashid;

// copy the rest of the command line
	cmd[ 0 ] = '\0';	// start out with a NUL-terminated string
	c = Cmd_Argc();
	for( i = 2; i < c; ++i )
	{
		my_strlcat( cmd, Cmd_Argv( i ), sizeof( cmd ) );
		if( i != (c - 1) )
		{
			my_strlcat( cmd, " ", sizeof( cmd ) );
		}
	}
	my_strlcat( cmd, "\n", sizeof( cmd ) );
	
	a->value = my_CopyString( cmd );
}

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

typedef struct cmd_function_s
{
	struct cmd_function_s	*next;
	char					*name;
	W32					id;
	xcommand_t				function;

} cmd_function_t;


PRIVATE	int			cmd_argc;
PRIVATE	char		*cmd_argv[ MAX_STRING_TOKENS ];
PRIVATE	char		*cmd_null_string = "";
PRIVATE	char		cmd_args[ MAX_STRING_CHARS ];

PRIVATE	cmd_function_t	*cmd_functions;		// possible commands to execute

/*
-----------------------------------------------------------------------------
 Function: Cmd_Argc -How many arguments are passed in.
 
 Parameters: Nothing.
 
 Returns: How many arguments are passed in.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC int Cmd_Argc( void )
{
	return cmd_argc;
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_Argv -Retrieve one argument.
 
 Parameters: arg -[in] Which argument to retrieve.
 
 Returns: NULL if outside argument index, otherwise returns the argument
			string.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cmd_Argv( int arg )
{
	if( arg >= cmd_argc )
	{
		return cmd_null_string;
	}

	return cmd_argv[ arg ];	
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_Args -Retrieve all arguments.
 
 Parameters: Nothing.
 
 Returns: A single string containing argv(1) to argv(argc()-1)
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cmd_Args( void )
{
	return cmd_args;
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_MacroExpandString
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE char *Cmd_MacroExpandString( char *text )
{
	int		i, j, count, len;
	_boolean	inquote;
	char	*scan;
	static	char	expanded[ MAX_STRING_CHARS ];
	char	temporary[MAX_STRING_CHARS];
	char	*token, *start;

	inquote = false;
	scan = text;

	len = strlen( scan );
	if( len >= MAX_STRING_CHARS )
	{
		Com_Printf( "Line exceeded %i chars, discarded.\n", MAX_STRING_CHARS );
		return NULL;
	}

	count = 0;

	for( i = 0 ; i < len ; ++i )
	{
		if (scan[i] == '"')
			inquote ^= 1;

		if (inquote)
			continue;	// don't expand inside quotes

		if (scan[i] != '$')
			continue;

		// scan out the complete macro
		start = scan+i+1;
		token = COM_Parse( &start );
		if (!start)
			continue;
	
		token = Cvar_VariableString (token);

		j = strlen(token);
		len += j;
		if (len >= MAX_STRING_CHARS)
		{
			Com_Printf ("Expanded line exceeded %i chars, discarded.\n", MAX_STRING_CHARS);
			return NULL;
		}

		strncpy( temporary, scan, i );
		my_strlcpy( temporary+i, token, sizeof( temporary ) - i );
		my_strlcpy( temporary+i+j, start, sizeof( temporary ) - i - j );

		my_strlcpy( expanded, temporary, sizeof( expanded ) );
		scan = expanded;
		i--;

		if( ++count == 100 )
		{
			Com_Printf( "Macro expansion loop, discarded.\n" );
			return NULL;
		}
	}

	if( inquote )
	{
		Com_Printf( "Line has unmatched quote, discarded.\n" );
		return NULL;
	}

	return scan;
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_TokenizeString -Parses the given string into command line tokens.
 
 Parameters: text -[in] string to tokenize.
			 macroExpand -[in] Expand macro, true for yes, otherwise false.
 
 Returns: Nothing
 
 Notes: 
	$Cvars will be expanded unless they are in a quoted token
-----------------------------------------------------------------------------
*/
PUBLIC void Cmd_TokenizeString( char *text, _boolean macroExpand )
{
	int		i;
	char	*com_token;

// clear the args from the last string
	for( i = 0 ; i < cmd_argc ; ++i )
	{
		Z_Free( cmd_argv[ i ] );
	}
		
	cmd_argc = 0;
	cmd_args[ 0 ] = 0;
	
	// macro expand the text
	if( macroExpand )
	{
		text = Cmd_MacroExpandString( text );
	}

	if( ! text )
	{
		return;
	}

	while( 1 )
	{
// skip whitespace up to a /n
		while( *text && *text <= ' ' && *text != '\n' )
		{
			text++;
		}
		
		if( *text == '\n' )
		{	// a newline seperates commands in the buffer
			text++;
			break;
		}

		if( ! *text )
		{
			return;
		}

		// set cmd_args to everything after the first arg
		if( cmd_argc == 1 )
		{
			int		l;

			my_strlcpy( cmd_args, text, sizeof( cmd_args ) - 1 );

			// strip off any trailing whitespace
			l = strlen( cmd_args ) - 1;
			for( ; l >= 0 ; --l )
			{
				if (cmd_args[l] <= ' ')
				{
					cmd_args[l] = 0;
				}
				else
				{
					break;
				}
			}
		}
			
		com_token = COM_Parse( &text );
		if( ! text )
		{
			return;
		}

		if( cmd_argc < MAX_STRING_TOKENS )
		{
			cmd_argv[ cmd_argc ] = Z_Malloc( strlen( com_token ) + 1 );
			my_strlcpy( cmd_argv[ cmd_argc ], com_token, strlen( com_token ) + 1 );
			cmd_argc++;
		}
	}
	
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_AddCommand	-Add a command name and function to the cmd repository.
 
 Parameters: 
 			cmd_name -[in] Pointer to a NUL-terminated string that constains a 
 						command name. 
			function -[in] Function to associate with cmd_name.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Cmd_AddCommand( char *cmd_name, xcommand_t function )
{
	cmd_function_t	*cmd;
	W32 hashid;
	
// fail if the command is a variable name
	if( Cvar_VariableString( cmd_name )[ 0 ] )
	{
		Com_Printf( "Cmd_AddCommand: \"%s\" already defined as a var\n", cmd_name );
		return;
	}

	hashid = my_strhash( cmd_name );
	
// fail if the command already exists
	for( cmd = cmd_functions ; cmd ; cmd = cmd->next )
	{
		if( hashid == cmd->id )
		{
			Com_Printf( "Cmd_AddCommand: \"%s\" already defined\n", cmd_name );
			return;
		}
	}

	cmd = Z_Malloc( sizeof( cmd_function_t ) );
	cmd->name = cmd_name;
	cmd->id = hashid;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
	
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_RemoveCommand -Remove command.
 
 Parameters: cmd_name -[in] name of command to remove.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cmd_RemoveCommand( char *cmd_name )
{
	cmd_function_t	*cmd, **back;
	W32 hashid;

	hashid = my_strhash( cmd_name );

	back = &cmd_functions;
	while( 1 )
	{
		cmd = *back;
		if( ! cmd )
		{
			Com_DPrintf( "Cmd_RemoveCommand: %s was not added\n", cmd_name );
			return;
		}

		if( hashid == cmd->id )
		{
			*back = cmd->next;
			Z_Free( cmd );
			return;
		}

		back = &cmd->next;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_Exists -Check to see if command already exists.
 
 Parameters: cmd_name -[in] name of command to check.
 
 Returns: true if the command already exists, otherwise false.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean Cmd_Exists( char *cmd_name )
{
	cmd_function_t	*cmd;
	W32 hashid = my_strhash( cmd_name );

	for( cmd = cmd_functions ; cmd ; cmd = cmd->next )
	{
		if( hashid == cmd->id )
		{
			return true;
		}
	}

	return false;
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_CompleteCommand -Complete partial command on console 
									command-line.
 
 Parameters: partial -[in] Partial name of command.
 
 Returns: NULL if no command exists, otherwise returns the complete name of
			command.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cmd_CompleteCommand( char *partial )
{
	cmd_function_t	*cmd;
	int				len;
	cmdalias_t		*a;
	W32 hashid;
	
	len = strlen( partial );
	
	if( ! len )
	{
		return NULL;
	}

	

//	
// Check for exact match.
//
	hashid = my_strhash( partial );

	for( cmd = cmd_functions ; cmd ; cmd = cmd->next )
	{
		if( hashid == cmd->id )
		{
			return cmd->name;
		}
	}

	for( a = cmd_alias ; a ; a = a->next )
	{
		if( hashid == a->id )
		{
			return a->name;
		}
	}

//
// Check for partial match.
//
	for( cmd = cmd_functions ; cmd ; cmd = cmd->next )
	{
		if( ! strncmp( partial, cmd->name, len ) )
		{
			return cmd->name;
		}
	}

	for( a = cmd_alias ; a ; a = a->next )
	{
		if( ! strncmp( partial, a->name, len ) )
		{
			return a->name;
		}
	}

	return NULL;
}


/*
-----------------------------------------------------------------------------
 Function: Cmd_ExecuteString -Execute command string.
 
 Parameters: text -[in] text string to execute.
 
 Returns: Nothing.
 
 Notes: 
	A complete command line has been parsed, so try to execute it
	FIXME: lookupnoadd the token to speed search?
-----------------------------------------------------------------------------
*/
PUBLIC void Cmd_ExecuteString( char *text )
{	
	cmd_function_t	*cmd;
	cmdalias_t		*a;
	W32	hashid;

	Cmd_TokenizeString( text, true );
			
	// execute the command line
	if( ! Cmd_Argc() )
	{
		return;		// no tokens
	}

	hashid = my_strhash( cmd_argv[ 0 ] );

	// check functions
	for( cmd = cmd_functions ; cmd ; cmd = cmd->next )
	{
		if( hashid == cmd->id )
		{
			if( ! cmd->function )
			{	// forward to server command
				Cmd_ExecuteString( va( "cmd %s", text ) );
			}
			else
			{
				cmd->function();
			}
			
			return;
		}
	}

	// check alias
	for( a = cmd_alias ; a ; a = a->next )
	{
		if( ! my_stricmp( cmd_argv[ 0 ], a->name ) )
		{
			if( ++alias_count == ALIAS_LOOP_COUNT )
			{
				Com_Printf( "ALIAS_LOOP_COUNT\n" );
				return;
			}
			Cbuf_InsertText( a->value );
			return;
		}
	}
	
	// check cvars
	if( Cvar_Command() )
	{
		return;
	}

	// send it as a server command if we are connected
//	Cmd_ForwardToServer();
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_List_f -Callback function that list commands.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: List commands and total number of commands.
-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_List_f( void )
{
	cmd_function_t	*cmd;
	int				i = 0;

	for( cmd = cmd_functions; cmd; cmd = cmd->next, ++i )
	{
		Com_Printf( "%s\n", cmd->name );
	}
	
	Com_Printf( "%i commands\n", i );
}

/*
-----------------------------------------------------------------------------
 Function: Cmd_Init -Initialize the command buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Cmd_Init( void )
{
	Cbuf_Init();

//
// register our commands
//
	Cmd_AddCommand( "listCmds",	Cmd_List_f );
	Cmd_AddCommand( "exec",		Cmd_Exec_f );
	Cmd_AddCommand( "echo",		Cmd_Echo_f );
	Cmd_AddCommand( "alias",	Cmd_Alias_f);
	Cmd_AddCommand( "wait",		Cmd_Wait_f );
}

