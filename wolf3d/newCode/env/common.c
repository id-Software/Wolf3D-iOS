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
 *	common.c:	Misc functions used in client and server.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"


#define	MAXPRINTMSG	4096

#define MAX_NUM_ARGVS	50


colour3_t	colourBlack = {   0,   0,	0 };
colour3_t	colourRed	= { 255,   0,   0 };
colour3_t	colourGreen	= {   0, 255,   0 };
colour3_t	colourBlue	= {   0,   0, 255 };
colour3_t	colourWhite	= { 255, 255, 255 };


int		com_argc;
char	*com_argv[ MAX_NUM_ARGVS + 1 ];


jmp_buf abortframe;		// an ERR_DROP occured, exit the entire frame


FILE	*log_stats_file;

cvar_t	*log_stats;
cvar_t	*developer;
cvar_t	*logfile_active;	// 1 = buffer log, 2 = flush after each print

FILE	*logfile;


/*
============================================================================

CLIENT / SERVER interactions

============================================================================
*/
static int	rd_target;
static char	*rd_buffer;
static int	rd_buffersize;
static void	(*rd_flush)( int target, char *buffer );


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Com_BeginRedirect( int target, char *buffer, int buffersize, void (*flush) )
{
	if( ! target || ! buffer || ! buffersize || ! flush )
	{
		return;
	}

	rd_target = target;
	rd_buffer = buffer;
	rd_buffersize = buffersize;
	rd_flush = flush;

	*rd_buffer = 0;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Com_EndRedirect( void )
{
	rd_flush( rd_target, rd_buffer );

	rd_target = 0;
	rd_buffer = NULL;
	rd_buffersize = 0;
	rd_flush = NULL;
}

/*
-----------------------------------------------------------------------------
 Function: Com_Printf -print out message.
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 
	Both client and server can use this, and it will output
	to the apropriate place.
-----------------------------------------------------------------------------
*/
PUBLIC void Com_Printf( const char *fmt, ... )
{
	va_list		argptr;
	static char	msg[ MAXPRINTMSG ];

	va_start( argptr, fmt );
	(void)vsnprintf( msg, sizeof( msg ), fmt, argptr );
	va_end( argptr );

	msg[ sizeof( msg ) - 1 ] = '\0';

	if( rd_target )
	{
		if( (strlen( msg ) + strlen( rd_buffer ) ) > (rd_buffersize - 1) )
		{
			rd_flush( rd_target, rd_buffer );
			*rd_buffer = '\0';
		}
		my_strlcat( rd_buffer, msg, rd_buffersize );
		return;
	}

	Con_Print( msg );
#ifdef _WIN32
	OutputDebugString( msg );
#endif

	// also echo to debugging console
//	Sys_ConsoleOutput( msg );

	// logfile
	if( logfile_active && logfile_active->value )
	{
		char name[ MAX_GAMEPATH ];
		
		if( ! logfile )
		{
			my_snprintf( name, sizeof( name ), "%s/console.log", FS_Gamedir() );
			if( logfile_active->value > 2 )
			{
				logfile = fopen( name, "a" );
			}
			else
			{
				logfile = fopen( name, "w" );
			}
		}
		if( logfile )
		{
			fprintf( logfile, "%s", msg );
		}
		
		if( logfile_active->value > 1 )
		{
			fflush( logfile );		// force it to save every time
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: Com_DPrintf -A Com_Printf that only shows up if the "developer" 
						cvar is set
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Com_DPrintf( const char *fmt, ... )
{
	va_list		argptr;
	static char	msg[ MAXPRINTMSG ];
		
	if( ! developer || ! developer->value )
	{
		return;			// don't confuse non-developers with techie stuff...
	}

	va_start( argptr, fmt );
	(void)vsnprintf( msg, sizeof( msg ), fmt, argptr );
	va_end( argptr );

	msg[ sizeof( msg ) - 1 ] = '\0';
	
	Com_Printf( "%s", msg );
}


/*
-----------------------------------------------------------------------------
 Function: Com_Error
 
 Parameters:
 
 Returns:
 
 Notes: 
	Both client and server can use this, and it will
	do the apropriate things.
-----------------------------------------------------------------------------
*/
PUBLIC void Com_Error( int code, const char *fmt, ... )
{
	va_list		argptr;
	static char		msg[ MAXPRINTMSG ];
	static	_boolean	recursive;

	if( recursive )
	{
		Sys_Error( "recursive error after: %s", msg );
	}

	recursive = true;

	va_start (argptr,fmt);
	(void)vsnprintf( msg, sizeof( msg ), fmt, argptr );
	va_end (argptr);

	msg[ sizeof( msg ) - 1 ] = '\0';
	
	if( code == ERR_DISCONNECT )
	{
		recursive = false;
		longjmp (abortframe, -1);
	}
	else if( code == ERR_DROP )
	{
		Com_Printf( "********************\nERROR: %s\n********************\n", msg );
		recursive = false;
		longjmp( abortframe, -1 );
	}
	else
	{
	}

	if( logfile )
	{
		fclose( logfile );
		logfile = NULL;
	}

	Sys_Error( "%s", msg );
}



//===========================================================================


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void SZ_Init( sizebuf_t *buf, PW8 data, int length )
{
	memset( buf, 0, sizeof( *buf ) );
	buf->data = data;
	buf->maxsize = length;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void SZ_Clear( sizebuf_t *buf )
{
	buf->cursize = 0;
	buf->overflowed = false;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void *SZ_GetSpace( sizebuf_t *buf, int length )
{
	void	*data;
	
	if( buf->cursize + length > buf->maxsize )
	{
		if( ! buf->allowoverflow )
		{
			Com_Error( ERR_FATAL, "SZ_GetSpace: overflow without allowoverflow set" );
		}
		
		if( length > buf->maxsize )
		{
			Com_Error( ERR_FATAL, "SZ_GetSpace: %i is > full buffer size", length );
		}
			
		Com_Printf( "SZ_GetSpace: overflow\n" );
		SZ_Clear( buf ); 
		buf->overflowed = true;
	}

	data = buf->data + buf->cursize;
	buf->cursize += length;
	
	return data;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void SZ_Write( sizebuf_t *buf, void *data, int length )
{
	memcpy( SZ_GetSpace( buf, length ), data, length );		
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void SZ_Print( sizebuf_t *buf, W8 *data )
{
	int		len;
	
	len = strlen( (char *)data ) + 1;

	if (buf->cursize)
	{
		if( buf->data[ buf->cursize - 1 ] )
		{
			memcpy( (PW8)SZ_GetSpace( buf, len ), data, len ); // no trailing 0
		}
		else
		{
			memcpy( (PW8)SZ_GetSpace( buf, len - 1 ) - 1, data, len ); // write over trailing 0
		}
	}
	else
	{
		memcpy( (PW8)SZ_GetSpace( buf, len ),data,len );
	}
}


//============================================================================


/*
-----------------------------------------------------------------------------
 Function: COM_CheckParm 
 
 Parameters:
 
 Returns: 
		The position (1 to argc-1) in the program's argument list
		where the given parameter apears, or 0 if not present
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC int COM_CheckParm( char *parm )
{
	int		i;
	
	for( i = 1 ; i < com_argc ; ++i )
	{
		if( ! strcmp( parm, com_argv[ i ] ) )
		{
			return i;
		}
	}
		
	return 0;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC int COM_Argc (void)
{
	return com_argc;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *COM_Argv (int arg)
{
	if (arg < 0 || arg >= com_argc || !com_argv[arg])
		return "";
	return com_argv[arg];
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void COM_ClearArgv( int arg )
{
	if (arg < 0 || arg >= com_argc || !com_argv[arg])
	{
		return;
	}

	com_argv[ arg ] = "";
}


/*
-----------------------------------------------------------------------------
 Function: COM_InitArgv	-Set global argv values with commadline argv values.
 
 Parameters: 
 			argc -[in] An integer specifying how many arguments are in argv[]. 
			argv -[in] An array of null-terminated strings. The last pointer 
 					(argv[argc]) is NULL.
 
 Returns: Nothing.
 
 Notes: Sets global variables com_argc and com_argv.
-----------------------------------------------------------------------------
*/
PUBLIC void COM_InitArgv( int argc, char *argv[] )
{
	int i;

	if( argc > MAX_NUM_ARGVS )
	{
		argc = MAX_NUM_ARGVS;
		Com_DPrintf( "argc > MAX_NUM_ARGVS\n" );		
	}

	com_argc = argc;
	for( i = 0; i < argc; ++i )
	{
		if( ! argv[ i ] || strlen( argv[ i ] ) >= MAX_TOKEN_CHARS )
		{
			com_argv[ i ] = "";
		}
		else
		{
			com_argv[ i ] = argv[ i ];
		}
	}
}


/*
-----------------------------------------------------------------------------
 Function: COM_AddParm -Adds the given string at the end of the current 
						argument list
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void COM_AddParm( char *parm )
{
	if( com_argc == MAX_NUM_ARGVS )
	{
		Com_Error( ERR_FATAL, "COM_AddParm: MAX_NUM_ARGS" );
	}

	com_argv[ com_argc++ ] = parm;
}

/*
-----------------------------------------------------------------------------
 Function: Com_Error_f -Just throw a fatal error to test error shutdown 
						procedures.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Com_Error_f (void)
{
	Com_Error( ERR_FATAL, "%s", Cmd_Argv( 1 ) );
}


