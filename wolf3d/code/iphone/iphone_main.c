/*

	Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 
	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
	Copyright (C) 1997-2001 Id Software, Inc.

	
	This file is part of the WOLF3D iOS v2.1 GPL Source Code. 


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

 does clearing color and depth at the same time offer any beneft?
 should we use the depth sense reversing trick to avoid depth clears?
 
 */

#include "../wolfiphone.h"
#include "iphone_store.h"

cvar_t	*controlScheme;
cvar_t	*sensitivity;
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
#ifdef VOLUMEHACK
cvar_t	*volumeFireUp; //gsh
cvar_t	*volumeFireDown; //gsh
cvar_t	*volumeFireUpSetting; //gsh
cvar_t	*volumeFireDownSetting; //gsh
#endif
cvar_t	*hudAlpha; //gsh
cvar_t	*music;
cvar_t	*showTilt;
cvar_t	*showTime;
cvar_t	*cropSprites;
cvar_t	*blends;
cvar_t	*gunFrame;
cvar_t	*slowAI;
cvar_t	*mapScale;
cvar_t	*hideControls;
cvar_t	*autoFire;

W32	sys_frame_time;

#ifdef STOREKIT
extern void CheckForStorekitExistence(); //gsh
#endif

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
	char	*s;
	int		start = Sys_Milliseconds();
	static bool firstInit = true;
	
	if ( !firstInit ) {
		return;
	}
	
	firstInit = false;
	
#ifdef STOREKIT
	//check for storekit framework and set appropriate flags... gsh
	CheckForStorekitExistence();
#endif
	
	InAppPurchaseInit();
	
	// temporary 
	const char *systemVersion = SysIPhoneGetOSVersion();
	printf( "systemVersion = %s\n", systemVersion );
	
	z_chain.next = z_chain.prev = &z_chain;
	
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
	
	Com_Printf( "\n====== Application Initialized ======\n\n" );	
	
	Sound_Activate( true );
	consoleActive = 0;
	
	controlScheme = Cvar_Get( "controlScheme", "0", CVAR_ARCHIVE );
	sensitivity = Cvar_Get( "sensitivity", "0.5", CVAR_ARCHIVE );
	stickTurnBase = Cvar_Get( "stickTurnBase", "200", CVAR_ARCHIVE );
	stickTurnScale = Cvar_Get( "stickTurnScale", "400", CVAR_ARCHIVE );
	stickMoveBase = Cvar_Get( "stickMoveBase", "2000", CVAR_ARCHIVE );
	stickMoveScale = Cvar_Get( "stickMoveScale", "4000", CVAR_ARCHIVE );
	stickDeadBand = Cvar_Get( "stickDeadBand", "0.2", CVAR_ARCHIVE );
	tiltTurn = Cvar_Get( "tiltTurn", "0", CVAR_ARCHIVE );
	tiltMove = Cvar_Get( "tiltMove", "0", CVAR_ARCHIVE );
	tiltFire = Cvar_Get( "tiltFire", "0", CVAR_ARCHIVE );
#ifdef VOLUMEHACK
	volumeFireUp = Cvar_Get( "volumeFireUp", "0", 0 );  //gsh
	volumeFireDown = Cvar_Get( "volumeFireDown", "0", 0 );  //gsh
	volumeFireUpSetting = Cvar_Get( "volumeFireUpSetting", "1", CVAR_ARCHIVE );  //gsh      // 1 = primary
	volumeFireDownSetting = Cvar_Get( "volumeFireDownSetting", "0", CVAR_ARCHIVE );  //gsh  // 0 = secondary
#endif
	hudAlpha = Cvar_Get("hudAlpha", "1.0", CVAR_ARCHIVE); //gsh
	music = Cvar_Get( "music", "1", CVAR_ARCHIVE );
	tiltDeadBand = Cvar_Get( "tiltDeadBand", "0.08", CVAR_ARCHIVE );
	tiltAverages = Cvar_Get( "tiltAverages", "3", CVAR_ARCHIVE );
	cropSprites = Cvar_Get( "cropSprites", "1", 0 );
	showTilt = Cvar_Get( "showTilt", "-1", 0 );
	showTime = Cvar_Get( "showTime", "0", 0 );
	blends = Cvar_Get( "blends", "1", 0 );
	gunFrame = Cvar_Get( "gunFrame", "0", 0 );
	slowAI = Cvar_Get( "slowAI", "0", 0 );
	mapScale = Cvar_Get( "mapScale", "10", CVAR_ARCHIVE );
	hideControls = Cvar_Get( "hideControls", "0", CVAR_ARCHIVE );
	autoFire = Cvar_Get( "autoFire", "0", 0 );
	g_version = Cvar_Get( "g_version", "1", CVAR_ARCHIVE );
	//this call fixes the bug of crashed apps during sod map loads
	//( it only occured on devices updated from ( 1.1 || 1.0 )->1.2 )
	Cvar_Set( "g_version", "1" );

	/*  //if we don't preload the ogg files then we have faster start ups... gsh
	// preload all the ogg FM synth sounds
	Com_Printf( "before ogg preload: %i msec\n", Sys_Milliseconds() - start );
	
	Sound_RegisterSound( "lsfx/001.wav" );
	Sound_RegisterSound( "lsfx/003.wav" );
	Sound_RegisterSound( "lsfx/008.wav" );
	Sound_RegisterSound( "lsfx/009.wav" );
	Sound_RegisterSound( "lsfx/012.wav" );
	Sound_RegisterSound( "lsfx/023.wav" );
	Sound_RegisterSound( "lsfx/028.wav" );
	Sound_RegisterSound( "lsfx/030.wav" );
	Sound_RegisterSound( "lsfx/031.wav" );
	Sound_RegisterSound( "lsfx/033.wav" );
	Sound_RegisterSound( "lsfx/034.wav" );
	Sound_RegisterSound( "lsfx/035.wav" );
	Sound_RegisterSound( "lsfx/036.wav" );
	Sound_RegisterSound( "lsfx/037.wav" );
	Sound_RegisterSound( "lsfx/038.wav" );
	Sound_RegisterSound( "lsfx/040.wav" );
	Sound_RegisterSound( "lsfx/045.wav" );
	Sound_RegisterSound( "lsfx/061.wav" );
	Sound_RegisterSound( "lsfx/062.wav" );
	Sound_RegisterSound( "lsfx/064.wav" );
	Sound_RegisterSound( "lsfx/069.wav" );
	Sound_RegisterSound( "lsfx/076.wav" );
	Sound_RegisterSound( "lsfx/078.wav" );
	Sound_RegisterSound( "lsfx/080.wav" );
	Sound_RegisterSound( "lsfx/085.wav" );
	Sound_RegisterSound( "lsfx/086.wav" );*/
	
	// these should get overwritten by LoadTheGame
	memset( &currentMap, 0, sizeof( currentMap ) );
	currentMap.skill = 1;
	currentMap.episode = 0;
	HudSetForScheme( 0 );
	
		
	Com_Printf( "before LoadTheGame: %i msec\n", Sys_Milliseconds() - start );

	if ( !LoadTheGame() ) {
		PL_NewGame( &Player );
		iphoneStartMap( currentMap.episode, currentMap.map, currentMap.skill );
		HudSetForScheme( 0 );
	}

	// always start at main menu
	menuState = IPM_MAIN;
	
	// Start menu music
	iphoneInitMenuMusic();
	iphoneStartMenuMusic();
	
	Com_Printf( "startup time: %i msec\n", Sys_Milliseconds() - start );
}

/*
 ===================
 iphoneStartGameplay
 
 Loads the saved game if there is one, otherwise starts a new game.
 ===================
*/
void iphoneStartGameplay() {

}

/*
 ===================
 iphonePreloadBeforePlay
 
 This could all be done at startup, but moving a bit of the delay
 to after pressing the resume button works a little better.
 ===================
*/
void iphonePreloadBeforePlay() {
	int	start = Sys_Milliseconds();
	
	// the texnums might have been different in the savegame
	HudSetTexnums();
	
	// preload all the other game gui textures that might pop up
	TM_FindTexture( "iphone/gold_key.tga", TT_Pic );
	TM_FindTexture( "iphone/silver_key.tga", TT_Pic );
	TM_FindTexture( "iphone/L_damage.tga", TT_Pic );
	TM_FindTexture( "iphone/R_damage.tga", TT_Pic );
	
	for ( int i = 0 ; i < 10 ; i++ ) {
		char	name[64];
		sprintf( name, "iphone/%i.tga", i );
		numberPics[i] = TM_FindTexture( name, TT_Pic );
	}
	
	for ( int i = 0 ; i < NUM_MUGSHOTS ; i++ ) {
		TM_FindTexture( mugshotnames[ i ], TT_Pic );
	}
	
	Sound_RegisterSound( "sfx/001.wav" );
	Sound_RegisterSound( "sfx/002.wav" );
	Sound_RegisterSound( "sfx/007.wav" );
	Sound_RegisterSound( "sfx/010.wav" );
	Sound_RegisterSound( "sfx/011.wav" );
	Sound_RegisterSound( "sfx/012.wav" );
	Sound_RegisterSound( "sfx/013.wav" );
	Sound_RegisterSound( "sfx/015.wav" );
	Sound_RegisterSound( "sfx/022.wav" );
	Sound_RegisterSound( "sfx/024.wav" );
	Sound_RegisterSound( "sfx/025.wav" );
	Sound_RegisterSound( "sfx/026.wav" );
	Sound_RegisterSound( "sfx/027.wav" );
	Sound_RegisterSound( "sfx/035.wav" );
	Sound_RegisterSound( "sfx/037.wav" );
	Sound_RegisterSound( "sfx/046.wav" );
	Sound_RegisterSound( "sfx/049.wav" );
	Sound_RegisterSound( "sfx/071.wav" );
	Sound_RegisterSound( "sfx/074.wav" );
	Sound_RegisterSound( "sfx/076.wav" );
	Sound_RegisterSound( "sfx/086.wav" );
	Sound_RegisterSound( "sfx/088.wav" );
	Sound_RegisterSound( "sfx/105.wav" );
	Sound_RegisterSound( "sfx/107.wav" );
	
	Com_Printf( "preloadBeforePlay(): %i msec\n", Sys_Milliseconds() - start );	
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
	//Sound_StopBGTrack();
	iphoneWriteConfig();
	SaveTheGame();
	//exit( 0 );
}

