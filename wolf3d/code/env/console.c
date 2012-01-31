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
 *	console.c:  Console drawing and management.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"

extern viddef_t viddef;
extern int consoleActive;


console_t	con;

float scr_conlines;
float scr_con_current;

cvar_t		*con_notifytime;
cvar_t		*scr_conspeed;


#define		MAXCMDLINE	256

char	key_lines[ 32 ][ MAXCMDLINE ];
int		edit_line;
int		key_linepos;


colour3_t colourconLGray = { 220, 220, 220 };
colour3_t colourconLLGray = { 192, 192, 192 };
		
/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void DrawString( int x, int y, char *s )
{
	Font_put_line( FONT0, x, y, s );
}

/*
-----------------------------------------------------------------------------
 Function: Key_ClearTyping -Clear any keys that where typed.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
void Key_ClearTyping( void )
{
	key_lines[ edit_line ][ 1 ] = 0;	// clear any typing
	key_linepos = 1;
}

/*
-----------------------------------------------------------------------------
 Function: Con_ToggleConsole_f -Get ready to enter console.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Con_ToggleConsole_f( void )
{
	Key_ClearTyping();
	Con_ClearNotify();
}

/*
-----------------------------------------------------------------------------
 Function: Con_Clear_f -Clear console text buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
	Zero sets con.text memory block.
-----------------------------------------------------------------------------
*/
PRIVATE void Con_Clear_f( void )
{
	memset( con.text, ' ', CON_TEXTSIZE );
}


/*
-----------------------------------------------------------------------------
 Function: Con_Dump_f -Save the console contents out to a file.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Con_Dump_f( void )
{
	int		length, x;
	char	*line;
	FILE	*f;
	char	buffer[1024];
	char	name[MAX_OSPATH];

	if( Cmd_Argc() != 2 )
	{
		Com_Printf( "usage: conDump <filename>\n" );
		return;
	}

	my_snprintf( name, sizeof( name ), "%s/%s.txt", FS_Gamedir(), Cmd_Argv( 1 ) );

	Com_Printf( "Dumped console text to %s.\n", name );
	FS_CreatePath( name );
	f = fopen( name, "w" );
	if( ! f )
	{
		Com_Printf( "ERROR: couldn't open.\n" );
		return;
	}

	// skip empty lines
	for( length = con.current - con.totallines + 1; length <= con.current; ++length )
	{
		line = con.text + (length % con.totallines) * con.linewidth;
		for( x = 0; x < con.linewidth; ++x )
		{
			if( line[ x ] != ' ' )
			{
				break;
			}
		}

		if( x != con.linewidth )
		{
			break;
		}
	}

	// write the remaining lines
	buffer[ con.linewidth ] = '\0';
	for( ; length <= con.current ; ++length )
	{
		line = con.text + (length % con.totallines) * con.linewidth;
		my_strlcpy( buffer, line, con.linewidth );
		for( x = con.linewidth - 1; x >= 0; --x )
		{
			if( buffer[ x ] == ' ' )
			{
				buffer[ x ] = '\0';	// NUL-terminate string
			}
			else
			{
				break;
			}
		}
		for( x = 0; buffer[ x ]; ++x )
		{
			buffer[ x ] &= 0x7f;
		}

		fprintf( f, "%s\n", buffer );
	}

	fclose( f );
}

/*
-----------------------------------------------------------------------------
 Function: Con_ClearNotify -Clear console con.times.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Con_ClearNotify( void )
{
	int	i;
	
	for( i = 0; i < NUM_CON_TIMES; ++i )
	{
		con.times[ i ] = 0;
	}
}

					
/*
-----------------------------------------------------------------------------
 Function: Con_CheckResize -If the line width has changed, reformat the buffer.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Con_CheckResize( void )
{
	int		i, j, width, oldwidth, oldtotallines, numlines, numchars;
	char	tbuf[ CON_TEXTSIZE ];

	width = (viddef.width >> 3) - 2;

	if( width == con.linewidth )
	{
		return;
	}

	if( width < 1 )			// video hasn't been initialized yet
	{
		width = 38;
		con.linewidth = width;
		con.totallines = CON_TEXTSIZE / con.linewidth;
		memset( con.text, ' ', CON_TEXTSIZE );
	}
	else
	{
		oldwidth = con.linewidth;
		con.linewidth = width;
		oldtotallines = con.totallines;
		con.totallines = CON_TEXTSIZE / con.linewidth;
		numlines = oldtotallines;

		if( con.totallines < numlines )
		{
			numlines = con.totallines;
		}

		numchars = oldwidth;
	
		if( con.linewidth < numchars )
		{
			numchars = con.linewidth;
		}

		memcpy( tbuf, con.text, CON_TEXTSIZE );
		memset( con.text, ' ', CON_TEXTSIZE );

		for( i = 0; i < numlines; ++i )
		{
			for( j = 0; j < numchars; ++j )
			{
				con.text[(con.totallines - 1 - i) * con.linewidth + j] =
						tbuf[((con.current - i + oldtotallines) %
							  oldtotallines) * oldwidth + j];
			}
		}		

		Con_ClearNotify();
	}

	con.current = con.totallines - 1;
	con.display = con.current;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Con_Init( void )
{
	con.linewidth = -1;

	Con_CheckResize();
	
	Com_Printf( "Console Initialized\n" );

//
// register our commands
//
	con_notifytime = Cvar_Get( "con_notifytime", "90", CVAR_INIT );
	scr_conspeed = Cvar_Get( "scr_conspeed", "90", CVAR_INIT );

	Cmd_AddCommand( "toggleconsole", Con_ToggleConsole_f );
	Cmd_AddCommand( "clear", Con_Clear_f );
	Cmd_AddCommand( "conDump", Con_Dump_f );

	con.initialized = true;
}

/*
-----------------------------------------------------------------------------
 Function: Con_Linefeed -Fill rest of line with spaces.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Con_Linefeed( void )
{
	con.x = 0;
	if( con.display == con.current )
	{
		con.display++;
	}
	con.current++;
	memset( &con.text[ (con.current % con.totallines) * con.linewidth ] 
		, ' ', con.linewidth );
}

/*
-----------------------------------------------------------------------------
 Function: Con_Print -Print formatted message to the console.
 
 Parameters: txt -[in] Text message to print
 
 Returns: Nothing.
 
 Notes: 
	Handles cursor positioning, line wrapping, etc
	All console printing must go through this in order to be logged to disk
	If no console is visible, the text will appear at the top of the game window
-----------------------------------------------------------------------------
*/
PUBLIC void Con_Print( char *txt )
{
	int		y;
	int		c, wordlength;
	static int	cr;
	int		mask;

#ifdef IPHONE
	printf( "%s", txt );
#endif
	
	if( ! con.initialized )
	{
		return;
	}

	if( txt[ 0 ] == 1 || txt[ 0 ] == 2 )
	{
		mask = 128;		// go to colored text
		txt++;
	}
	else
	{
		mask = 0;
	}


	while( (c = *txt) )
	{
	// count word length
		for( wordlength = 0 ; wordlength < con.linewidth ; ++wordlength )
		{
			if( txt[ wordlength ] <= ' ')
			{
				break;
			}
		}

	// word wrap
		if( wordlength != con.linewidth && (con.x + wordlength > con.linewidth) )
		{
			con.x = 0;
		}

		txt++;

		if( cr )
		{
			con.current--;
			cr = false;
		}

		
		if( ! con.x )
		{
			Con_Linefeed();
		// mark time for transparent overlay
			if( con.current >= 0 )
			{
				con.times[ con.current % NUM_CON_TIMES ] = iphoneFrameNum;
			}
		}

		switch( c )
		{
			case '\n':
				con.x = 0;
				break;

			case '\r':
				con.x = 0;
				cr = 1;
				break;

			default:	// display character and advance
				y = con.current % con.totallines;
				con.text[ y * con.linewidth + con.x] = c | mask | con.ormask;
				con.x++;
				if( con.x >= con.linewidth )
				{
					con.x = 0;
				}
				break;
		}
		
	}
}


/*
-----------------------------------------------------------------------------
 Function: Con_CenteredPrint 	-Print message that is centered on screen.
 
 Parameters: text -[in] Message string.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Con_CenteredPrint( const char *text )
{
	int		length;
	char	buffer[ 1024 ];

	length = strlen( text );
	length = ( con.linewidth - length ) >> 1;
	if( length < 0 )
	{
		length = 0;
	}

	memset( buffer, ' ', length );
	my_strlcpy( buffer + length, text, sizeof( buffer ) - length );
	my_strlcat( buffer, "\n", sizeof( buffer ) );
	Con_Print( buffer );
}


/*
==============================================================================

	DRAWING

==============================================================================
*/


/*
-----------------------------------------------------------------------------
 Function: Con_DrawInput 	-The input line scrolls horizontally if typing 
							goes beyond the right edge.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void Con_DrawInput( void )
{
	int		y;
	int		i;
	char   *text;
	char	buf[256];
	W16 heightfont, charwidth;
	static char ccursor[ 2 ] = { ' ', '_' }; 

	
	heightfont = Font_GetSize( FONT0 ) + 4;
	
#ifdef IPHONE
	{
		if ( consoleActive == 0 ) {
			return;
		}
		strcpy( buf, SysIPhoneGetConsoleTextField() );
		key_linepos = strlen( buf );
		buf[key_linepos+1] = 0;
		text = buf;
	}
#else
	if( ClientStatic.key_dest != key_console )
	{
		return;		// don't draw anything (always draw if not active)
	}
		
	text = key_lines[ edit_line ];
#endif
	
// add the cursor frame
	text[ key_linepos ] = ccursor[ 0 + ((int)( iphoneFrameNum >> 3 ) & 1 ) ];
	
// fill out remainder with spaces
	for( i = key_linepos + 1 ; i < con.linewidth ; ++i )
	{
		text[ i ] = ' ';
	}
		
//	prestep if horizontally scrolling
	if( key_linepos >= con.linewidth )
	{
		text += 1 + key_linepos - con.linewidth;
	}
		
// draw it
	y = con.vislines - heightfont;

	charwidth = 8;
	for( i = 0 ; i < con.linewidth ; ++i )
	{
		charwidth += Font_put_character( FONT0, charwidth, con.vislines - 22, text[ i ] );
	}

// remove cursor
	key_lines[ edit_line ][ key_linepos ] = 0;
}

/*
-----------------------------------------------------------------------------
 Function: Con_DrawNotify 	-Draws the last few lines of output transparently 
							over the game top.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Con_DrawNotify( void )
{
	int		x, v;
	int		charwidth;
	char	*text;
	int		i;
	int		time;
//	char	*s;
//	int		skip;
	W16		size;

	
	Font_SetSize( FONT1, 1 );
	size = Font_GetSize( FONT1 );

	Font_SetColour( FONT1, colourWhite );

	v = 0;
	for( i = con.current - NUM_CON_TIMES+1 ; i <= con.current ; ++i )
	{
		if( i < 0 )
		{
			continue;
		}

		time = FloatToInt( con.times[ i % NUM_CON_TIMES ] );
		if( time == 0 )
		{
			continue;
		}

		time = iphoneFrameNum- time;
		if( time > con_notifytime->value )
		{
			continue;
		}

		text = con.text + (i % con.totallines) * con.linewidth;
		
		charwidth = 0;
		for( x = 0 ; x < con.linewidth ; ++x )
		{
			charwidth += Font_put_character( FONT1, charwidth, v, text[ x ] );
		}

		v += size;
	}

	Font_SetSize( FONT1, 2 );

}


/*
-----------------------------------------------------------------------------
 Function: Con_DrawConsole 	-Draws the console.
 
 Parameters: frac -[in] Fraction of the screen the console will take up.
						Range is 0.0 to 1.0
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void Con_DrawConsole( float frac )
{
	int				i, x, y;
	int				rows;
	char			*text;
	int				row;
	W32				lines;
//	int w, h;
	int heightfont, charwidth;

	lines = FloatToInt( viddef.height * frac );
	if( lines < 1 )
	{
		return;
	}


	Font_SetSize( FONT0, 1 );
	Font_SetColour( FONT0, colourconLLGray  );
	heightfont = Font_GetSize( FONT0 );

	

	if( lines > viddef.height )
	{
		lines = viddef.height;
	}

//
// Draw the background	
//
	R_Draw_Fill( 0, -viddef.height + lines, viddef.width, viddef.height, colourBlack );
	R_Draw_Fill( 0, lines-2, viddef.width, 2, colourconLGray );	

	Font_SetColour( FONT0, colourconLLGray );

//
// Draw the text
//
	con.vislines = lines;
	
#if 0

	rows = (lines - 8) >> 3;		// rows of text to draw

	y = lines - 24;

#else

	rows = (lines - 22) >> 3;		// rows of text to draw

	y = lines - 30;

#endif

// draw from the bottom up
	if( con.display != con.current )
	{
	// draw arrows to show the buffer is backscrolled
		for( x = 0; x < con.linewidth; x += 4 )
		{
			Font_put_character( FONT0, (x+1) << 3, y, '^' );
		}
	
		y -= heightfont;
		rows--;
	}
	
	row = con.display;
	for( i = 0 ; i < rows ; ++i, y -= heightfont, --row )
	{
		if( row < 0 )
		{
			break;
		}

		if( con.current - row >= con.totallines )
		{
			break;		// past scrollback wrap point
		}
			
		text = con.text + (row % con.totallines) * con.linewidth;

		charwidth = 0;
		for( x = 0; x < con.linewidth; ++x )
		{
			charwidth += Font_put_character( FONT0, charwidth, y, text[ x ] );
		}
	}

// draw the input prompt, user text, and cursor if desired
	Con_DrawInput();

	Font_SetSize( FONT0, 2 );
}

/*
 -----------------------------------------------------------------------------
 Function: Client_Screen_RunConsole -Figure out how the console should be
 drawn.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
 
 -----------------------------------------------------------------------------
 */
PUBLIC void Client_Screen_RunConsole( void )
{
	if( scr_conlines < scr_con_current )
	{
		scr_con_current -= scr_conspeed->value * iphoneFrameNum;
		if( scr_conlines > scr_con_current )
		{
			scr_con_current = scr_conlines;
		}
		
	}
	else if( scr_conlines > scr_con_current )
	{
		scr_con_current += scr_conspeed->value * iphoneFrameNum;
		if( scr_conlines < scr_con_current )
		{
			scr_con_current = scr_conlines;
		}
	}
	
}

/*
 -----------------------------------------------------------------------------
 Function: Client_Screen_DrawConsole -Draw the console on the screen.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
 
 -----------------------------------------------------------------------------
 */
PUBLIC void Client_Screen_DrawConsole( void )
{
	Con_CheckResize();
	
	if( scr_con_current )
	{
		Con_DrawConsole( scr_con_current );
	}
	else
	{
		Con_DrawNotify();
	}
}



