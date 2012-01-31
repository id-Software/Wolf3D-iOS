/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
	Copyright (C) 1997-2001 Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
 *	unix_main.c: UNIX interface to application.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"


cvar_t	*controlScheme;
cvar_t	*sensitivity;
cvar_t	*stickSize;
cvar_t	*stickTurnBase;
cvar_t	*stickTurnScale;
cvar_t	*stickMoveBase;
cvar_t	*stickMoveScale;
cvar_t	*stickDeadBand;
cvar_t	*tiltTurn;
cvar_t	*tiltMove;
cvar_t	*tiltDeadBand;
cvar_t	*tiltAverages;
cvar_t	*tiltFire;
cvar_t	*music;
cvar_t	*showTilt;
cvar_t	*cropSprites;
cvar_t	*blends;
cvar_t	*gunFrame;
cvar_t	*slowAI;

W32	sys_frame_time;

void Sys_Error( const char *format, ... )
{ 
    va_list     argptr;
    char        string[ 1024 ];

    va_start( argptr, format );
    (void)vsnprintf( string, sizeof( string ), format, argptr );
    va_end( argptr );

	fprintf( stderr, "Error: %s\n", string );

	_exit( 1 );

} 

void Sys_Quit (void)
{
	_exit( 0 );
}

void Sys_SendKeyEvents (void)
{
}

char *Sys_GetClipboardData( void )
{
	return NULL;
}

void Reset_f() {
	memset( &currentMap, 0, sizeof( currentMap ) );
	currentMap.skill = 1;
	cvar_vars = NULL;	// don't write any cvars to the config file
	iphoneShutdown();
}

/*
 ==================
 iphoneStartup
 
 ==================
 */
void iphoneStartup() {
	int		i;
	char	*s;
	int		start = Sys_Milliseconds();
	
	z_chain.next = z_chain.prev = &z_chain;
	
	InitImmediateModeGL();
	
	// Prepare enough of the subsystems to handle
	// cvar and command buffer management.
	COM_InitArgv( 0, NULL );		// FIXME: get args...
	
	Cmd_Init();
	Cvar_Init();
	Con_Init();
	FS_InitFilesystem();	
	
	// We need to add the early commands twice, because
	// a basedir or cddir needs to be set before execing
	// config files, but we want other parms to override
	// the settings of the config files.
	Cbuf_AddEarlyCommands( false );
	Cbuf_Execute();
	
	R_Init();

	Cmd_AddCommand( "reset", Reset_f );

	developer = Cvar_Get( "developer", "0", CVAR_INIT );
	logfile_active = Cvar_Get( "logfile", "0", CVAR_INIT );
	
	s = va( "%s %s %s %s %s %s", APP_VERSION, RELEASENAME, CPUSTRING, __DATE__, __TIME__, BUILDSTRING );
	Cvar_Get( "version", s, CVAR_SERVERINFO | CVAR_NOSET );
	
	Con_Init();
	
	Sound_Init();
	
	Game_Init();	// game and player init
	
	Cbuf_AddText( "exec config.cfg\n" );
	Cbuf_AddEarlyCommands( true );
	Cbuf_Execute();

	// add + commands from command line
	Cbuf_AddLateCommands();
	Cbuf_Execute();
	
	for ( i = 0 ; i < 10 ; i++ ) {
		char	name[64];
		sprintf( name, "iphone/font/%i.tga", i );
		numberPics[i] = TM_FindTexture( name, TT_Pic );
	}
	
	Com_Printf( "\n====== Application Initialized ======\n\n" );	
	
	Sound_Activate( true );
	consoleActive = 0;
	
	controlScheme = Cvar_Get( "controlScheme", "0", CVAR_ARCHIVE );
	sensitivity = Cvar_Get( "sensitivity", "0.3", CVAR_ARCHIVE );
	
	stickSize = Cvar_Get( "stickSize", "120", CVAR_ARCHIVE );
	stickTurnBase = Cvar_Get( "stickTurnBase", "300", CVAR_ARCHIVE );
	stickTurnScale = Cvar_Get( "stickTurnScale", "500", CVAR_ARCHIVE );
	stickMoveBase = Cvar_Get( "stickMoveBase", "3000", CVAR_ARCHIVE );
	stickMoveScale = Cvar_Get( "stickMoveScale", "5000", CVAR_ARCHIVE );
	stickDeadBand = Cvar_Get( "stickDeadBand", "0.2", CVAR_ARCHIVE );
	tiltTurn = Cvar_Get( "tiltTurn", "0", CVAR_ARCHIVE );
	tiltMove = Cvar_Get( "tiltMove", "0", CVAR_ARCHIVE );
	tiltFire = Cvar_Get( "tiltFire", "0", CVAR_ARCHIVE );
	music = Cvar_Get( "music", "1", CVAR_ARCHIVE );
	tiltDeadBand = Cvar_Get( "tiltDeadBand", "0.08", CVAR_ARCHIVE );
	tiltAverages = Cvar_Get( "tiltAverages", "3", CVAR_ARCHIVE );
	cropSprites = Cvar_Get( "cropSprites", "1", 0 );
	showTilt = Cvar_Get( "showTilt", "-1", 0 );
	blends = Cvar_Get( "blends", "1", 0 );
	gunFrame = Cvar_Get( "gunFrame", "0", 0 );
	slowAI = Cvar_Get( "slowAI", "0", 0 );
	
	// these should get overwritten by LoadTheGame
	currentMap.skill = 1;
	currentMap.episode = 0;
	
	if ( !LoadTheGame() ) {
		memset( currentMap.mapFlags, 0,sizeof( currentMap.mapFlags ) );
		PL_NewGame( &Player );
		iphoneStartMap( 0, 0, 1 );
	}	
	
	// always start at main menu
	menuState = IPM_MAIN;
	
	Com_Printf( "startup time: %i msec\n", Sys_Milliseconds() - start );
}

/*
 ==================
 iphoneWriteConfig
 
 ==================
 */
void iphoneWriteConfig( void ) {
	FILE	*fp;
	char	path[ MAX_OSPATH];
	cvar_t	*var;
	char	buffer[1024];
	
	my_snprintf( path, sizeof( path ), "%s/config.cfg", iphoneDocDirectory );
	fp = fopen( path, "w" );
	if( ! fp ) {
		Com_Printf( "Could not write config.cfg.\n" );
		return;
	}
	
	// write out commands to set the archived cvars
	for( var = cvar_vars ; var ; var = var->next ) {
		if( var->flags & CVAR_ARCHIVE ) {
			my_snprintf( buffer, sizeof( buffer ), "set %s \"%s\"\n", var->name, var->string );
			fprintf( fp, "%s", buffer );
			Com_Printf( "%s", buffer );
		}
	}
	
	fclose( fp );
}


/*
 ==================
 iphoneShutdown
 
 Save the game at this position
 ==================
 */
void iphoneShutdown() {
	Sound_StopAllSounds();
	Sound_StopBGTrack();
	iphoneWriteConfig();
	SaveTheGame();
	exit( 0 );
}

