/*
 
 Copyright (C) 2009 Id Software, Inc.
 
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

#include "../wolfiphone.h"

int		intermissionStartFrameNum;
int		hasReleased;

menuState_t menuState;
colour4_t highlightColor = { 128, 128, 128, 255 };
colour4_t colorPressed = { 128, 128, 0, 255 };

/*
 ==================
 iphoneDrawPic
 
 ==================
 */
void iphoneDrawPic( int x, int y, int w, int h, const char *pic ) {
	R_Draw_StretchPic( x, y, w, h, pic );
}

/*
 ==================
 iphoneDrawPicWithTouch
 
 ==================
 */
int iphoneDrawPicWithTouch( int x, int y, int w, int h, const char *pic ) {
	int	r = TouchReleased( x, y, w, h );
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
	R_Draw_StretchPic( x, y, w, h, pic );
	if ( TouchDown( x, y, w, h ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( x, y, w, h, color );
	}
	return r;
}



/*
 ==================
 iphoneSlider
 
 ==================
 */
void iphoneSlider( int x, int y, int w, int h, const char *title, cvar_t *cvar, 
				  float min, float max ) {
	float value = cvar->value;
	char	str[80];
	float	f = ( value - min ) / ( max - min );
	colour4_t backColor = { 32, 32, 80, 255 };
	colour4_t highlightColor = { 64, 64, 160, 255 };
	
	if ( f < 0 ) {
		f = 0;
	}
	if ( f > 1 ) {
		f = 1;
	}
	
	// draw the background
	R_Draw_Fill( x, y, w, h, backColor );
	
	// draw the current range
	R_Draw_Fill( x+2, y+2, f*(w-4), h-4, highlightColor );
	
	// draw the title and fraction
	sprintf( str, "%s : %i%%", title, (int)(f*100) );
	iphoneCenterText( x+ w/2, y+h/2-8, str );
	
	// check for touches
	if ( numTouches > 0 && touches[0][0] >= x && touches[0][0] < x + w
		&& touches[0][1] >= y && touches[0][1] < y+ h ) {
		float newValue;
		float	delta;
		
		f = (float)( touches[0][0] - x ) / w;
		// round to tenths
		f = (int)( ( f + 0.05 ) * 10 ) * 0.1f;
		if ( f < 0 ) {
			f = 0;
		}
		if ( f > 1.0 ) {
			f = 1.0;
		}
		
		newValue = min + f * ( max - min );
		delta = fabs( newValue - cvar->value );
		if ( f == 0 && cvar->value == 0 ) {
			// special case of disable-at-0
		} else if ( delta > 0.01 ) {
			Cvar_SetValue( cvar->name, newValue );
			Sound_StartLocalSound( "iphone/slide_01.wav" );
		}
	}
	
}

/*
 ==================
 BackButton
 
 ==================
 */
int BackButton() {	
	if ( iphoneDrawPicWithTouch( 0, 0, 64, 32, "iphone/button_back.tga" ) ) {
		return 1;
	}
	return 0;
}	

void GetMoreLevels( int x, int y ) {
	if ( iphoneDrawPicWithTouch( x, y, 128, 64, "iphone/button_levels.tga" ) ) {
		// directly to the app store for more levels
		OpenURL( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=304694876" );
	}
}


/*
 ==================
 SaveTheGame
 
 ==================
 */
extern W8	areaconnect[ NUMAREAS ][ NUMAREAS ];
extern _boolean	areabyplayer[ NUMAREAS ];

void SaveTheGame() {
	FILE	*f;
	char	path[1024];
	int		version = SAVEGAME_VERSION;
	int		i;
	
	my_snprintf( path, sizeof( path ), "%s/savegame.bin", iphoneDocDirectory );
	f = fopen( path, "wb" );
	if( ! f ) {
		Com_Printf( "Could not open savegame.bin.\n" );
		return;
	}
	
	// turn the r_world->Doors.doors from pointers to indexes
	// ok to be destructive, because we are quiting
	for ( i = 0 ; i < r_world->Doors.doornum ; i++ ) {
		int	index = r_world->Doors.Doors[i] - &r_world->Doors.DoorMap[0][0];
		assert( index >= 0 && index < 4096 );
		r_world->Doors.Doors[i] = (void *)index;
	}
	
	// this is only used for the mutant death face, so just
	// clear it instead of fixing it up
	Player.LastAttacker = NULL;
	
	currentMap.version = SAVEGAME_VERSION;
	fwrite( &currentMap, 1,sizeof(currentMap), f );
	fwrite( r_world, 1,sizeof(*r_world), f );
	fwrite( &LevelRatios, 1,sizeof(LevelRatios), f );
	fwrite( &levelstate, 1,sizeof(levelstate), f );
	fwrite( Guards, 1,sizeof(Guards), f );
	fwrite( areaconnect, 1,sizeof(areaconnect), f );
	fwrite( areabyplayer, 1,sizeof(areabyplayer), f );
	fwrite( &PWall, 1,sizeof(PWall), f );
	fwrite( &Player, 1,sizeof(Player), f );
	fwrite( &version, 1,sizeof(version), f );
	
	fclose( f );
}

/*
 ==================
 LoadTheGame
 
 ==================
 */
int LoadTheGame() {
	FILE	*f;
	char	path[1024];
	int		version;
	int		i;
	int		oldCompleted;
	
	my_snprintf( path, sizeof( path ), "%s/savegame.bin", iphoneDocDirectory );
	f = fopen( path, "rb" );
	if( ! f ) {
		Com_Printf( "Could not open savegame.bin.\n" );
		return 0;
	}
	
	fread( &currentMap, 1,sizeof(currentMap) , f);
	
	if ( currentMap.version != SAVEGAME_VERSION ) {
		Com_Printf( "Savegame header version mismatch: %i != %i\n", currentMap.version, SAVEGAME_VERSION );
		fclose( f );
		return 0;
	}
	
	// do a normal map start
	Cvar_SetValue( skill->name, currentMap.skill );
	PL_NewGame( &Player );
	
	oldCompleted = currentMap.levelCompleted;
	iphoneStartMap( currentMap.episode, currentMap.map, currentMap.skill );
	currentMap.levelCompleted = oldCompleted;
	
	// load modifiactions on top
	r_world = Z_Malloc( sizeof( *r_world ) );
	fread( r_world, 1,sizeof(*r_world), f);
	fread( &LevelRatios, 1,sizeof(LRstruct), f );
	fread( &levelstate, 1,sizeof(levelstate), f );
	fread( Guards, 1,sizeof(Guards), f );
	fread( areaconnect, 1,sizeof(areaconnect), f );
	fread( areabyplayer, 1,sizeof(areabyplayer), f );
	fread( &PWall, 1,sizeof(PWall), f );
	fread( &Player, 1,sizeof(Player), f );
	fread( &version, 1,sizeof(version), f );
	
	fclose( f );
	
	if ( version != SAVEGAME_VERSION ) {
		Com_Printf( "Savegame trailer version mismatch: %i != %i\n", version, SAVEGAME_VERSION );
		return 0;
	}
	
	// turn the r_world->Doors.doors back to pointers
	for ( i = 0 ; i < r_world->Doors.doornum ; i++ ) {
		int	index = (int)r_world->Doors.Doors[i];
		assert( index >= 0 && index < 4096 );
		r_world->Doors.Doors[i] = &r_world->Doors.DoorMap[0][0] + index;
	}
	return 1;
}


/*
 ==================
 iphoneStartMap
 
 This does not reset the player, so call PL_NewGame( &Player ) if it is a new start.
 ==================
 */
void iphoneStartMap( int episodeNum, int mapNum, int skillLevel ) {
	char	command[128];
	int		levelNum = episodeNum*10+mapNum;
	
	Com_Printf( "iphoneStartMap( %i, %i, %i )\n", episodeNum, mapNum, skillLevel );
	
	// get the sound playing
	Sound_Update( vnull, vnull, vnull, vnull );

	// clean up game feedback
	damageflash = 0;
	bonusFrameNum = 0;
	attackDirTime[0] = 0;
	attackDirTime[1] = 0;
	
	// note that this has been tried now
	currentMap.mapFlags[currentMap.skill][levelNum] |= MF_TRIED;

	// start the game
	currentMap.episode = episodeNum;
	currentMap.map = mapNum;
	currentMap.skill = skillLevel;
	currentMap.levelCompleted = 0;

	Cvar_SetValue( skill->name, skillLevel );
	Cvar_SetValue( episode->name, episodeNum );
	sprintf( command, "w%i%i", currentMap.episode, currentMap.map );
	Client_PrepRefresh( command );	
	
	menuState = IPM_GAME;
}

/*
 ==================
 iphoneMainMenu
 
 ==================
 */
void iphoneMainMenu() {	
	char	str[80];
	float	scale = 40 / 32.0;
	
	iphoneDrawPic( 480-256, 0, 256, 128, "iphone/wolf_logo.tga" );
#ifdef EPISODE1
	iphoneDrawPic( -20, 0, 256, 64, "iphone/ep_1.tga" );
	GetMoreLevels( 0, 96 );
#else
	iphoneDrawPic( -20, 0, 256, 64, "iphone/ep_1_6.tga" );
#endif
	iphoneDrawPic( 0, 320 - 128, 128, 128, "iphone/id_logo.tga" );
	
	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 80, 128*scale, 64*scale, "iphone/button_resume.tga" ) ) {
		// if the game was saved at the intermission screen, immediately
		// bring it back up when it is loaded
		if ( currentMap.levelCompleted ) {
			iphoneStartIntermission( 0 );
		} else {
			menuState = IPM_GAME;
		}
	}
	sprintf( str, "E%iM%i", currentMap.episode+1, currentMap.map+1 );
	iphoneCenterText( 300, 80+34*scale, str );
	
	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 170, 128*scale, 32*scale, "iphone/button_control.tga" ) ) {
		menuState = IPM_CONTROLS;
	}
	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 220, 128*scale, 32*scale, "iphone/button_new.tga" ) ) {
		menuState = IPM_SKILL;
	}
	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 270, 128*scale, 32*scale, "iphone/button_web.tga" ) ) {
		OpenURL( "http://www.idsoftware.com/wolfenstein3dclassic/" );
	}
	
}


/*
 ==================
 iphoneControlMenu
 
 ==================
 */
void iphoneControlMenu() {
	int		i;
	
	if ( BackButton() ) {
		menuState = IPM_MAIN;
	}
	
	for ( i = 0 ; i < 4 ; i++ ) {
		char	str[128];
		int remap[4] = { 3,4,1,2};	// artist named them differently than intended...
		sprintf( str, "iphone/layout_%i.tga", remap[i] );
		if ( i != controlScheme->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( iphoneDrawPicWithTouch( 120 * i, 40, 120, 120, str ) ) {
			Cvar_SetValue( controlScheme->name, i );
		}
		pfglColor3f( 1, 1, 1 );
	}
	
	iphoneSlider( 20, 170, 440, 40, "sensitivity", sensitivity, 0, 1 );
	
	iphoneSlider( 20, 220, 440, 40, "tilt move speed", tiltMove, 5000, 30000 );	
	if ( tiltMove->value == 5000 ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	if ( tiltMove->value ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	iphoneSlider( 20, 270, 440, 40, "tilt turn speed", tiltTurn, 500, 3000 );
	if ( tiltTurn->value == 500 ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	if ( tiltTurn->value ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}

	//iphoneSlider( 20, 280, 440, 40, "tilt fire", tiltFire, 0, 1 );
	
}

/*
 ==================
 iphoneSkillMenu
 
 ==================
 */
void iphoneSkillMenu() {
	int		s;
	char	str[64];
	
	if ( BackButton() ) {
		menuState = IPM_MAIN;
	}
	
	// highlight the current skill selection
	s = (int)skill->value;
//	R_Draw_Fill( 80, 40+64*s, 320, 64, highlightColor );
	
	for ( s = 0 ; s < 4 ; s++ ) {
		my_snprintf( str, sizeof( str ), "iphone/button_skill%i.tga", s+1 );

		if ( s != (int)skill->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( iphoneDrawPicWithTouch( 112, 40+64*s, 256, 64, str ) ) {
			Cvar_SetValue( skill->name, s );
			menuState = IPM_EPISODE;
		}
		pfglColor3f( 1, 1, 1 );
	}
}	

/*
 ==================
 iphoneEpisodeMenu
 
 ==================
 */
void iphoneEpisodeMenu() {
	int		e;
	char	str[64];
#ifdef EPISODE1
	int		numE = 1;
#else
	int		numE = 6;
#endif
	
	if ( BackButton() ) {
		menuState = IPM_SKILL;
	}
	
	// 96 x 48 images
	for ( e = 0 ; e < numE ; e++ ) {
		my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
		
		if ( e != (int)episode->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( iphoneDrawPicWithTouch( 48, 32+48*e, 384, 48, str ) ) {
			Cvar_SetValue( episode->name, e );
			menuState = IPM_MAPS;
		}
		pfglColor3f( 1, 1, 1 );
	}

#ifdef EPISODE1
	// buy more episodes button
	GetMoreLevels( 240 - 64, 200 );	
#endif	
}	

/*
 ==================
 iphoneMapMenu
 
 ==================
 */
void iphoneMapMenu() {
	int		e, m, s;
	char	str[64];
	
	if ( BackButton() ) {
		menuState = IPM_EPISODE;
	}

	// draw the level selection
	e = episode->value;
	if ( e < 0 ) {
		e = 0;
	}
	if ( e > 5 ) {
		e = 5;
	}
	s = skill->value;
	if ( s < 0 ) {
		s = 0;
	}
	if ( s > 3 ) {
		s = 3;
	}
	
	// draw the episode selection
	my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
	iphoneDrawPicWithTouch( 96, 0, 384, 48, str );
	
	// draw the individual maps
	for ( m = 0 ; m < 10 ; m++ ) {
		int		x;
		int		y;
		colour4_t colorSecret = { 32, 32, 32, 255 };
		colour4_t colorNoTry = { 0, 0, 0, 255 };
		colour4_t colorTried = { 80, 80, 0, 255 };
		colour4_t colorCompleted = { 0, 80, 0, 255 };
		
		if ( m == 9 ) {
			sprintf( str, "SECRET" );
			x = 120;
			y = 90 + 160;
		} else if ( m == 8 ) {
			sprintf( str, "BOSS" );
			x = 360;
			y = 90 + 160;
		} else {
			sprintf( str, "LEVEL %i", m+1 );
			x = 60 + 120 * ( m % 4 );
			y = 90 + 80 * ( m / 4 );
		}
		
		unsigned char *color = colorNoTry;
		// decide on the background color
		int levelNum = e*10+m;
		int ch = currentMap.mapFlags[s][levelNum];
		// bit1 = attempted
		// bit2 = completed
		// bit3 = 100% kills
		// bit4 = 100% secrets
		// bit5 = 100% treasure
		if ( m == 9  && !( ch & MF_TRIED ) ) {
			color = colorSecret;
		} if ( ch & MF_COMPLETED ) {
			color = colorCompleted;
		} else if ( ch & MF_TRIED ) {
			color = colorTried;
		} else {
			color = colorNoTry;
		}
		
		// blink the level you are currently on
		if ( ( iphoneFrameNum & 8 ) && levelNum == currentMap.map && e == currentMap.episode ) {
			color = colorNoTry;
		}
		
		// draw the level text and check for button hit
		if ( TouchDown( x-46, y-9, 88, 32 ) ) {
			color = colorPressed;
		}
		R_Draw_Fill( x-46, y-9, 88, 32, color );
		iphoneCenterText( x, y, str );
		
		// draw awards
		if ( ch & MF_KILLS ) {
			iphoneDrawPic( x-46,y+23, 22, 22, "iphone/kills.tga" );
		}
		if ( ch & MF_SECRETS ) {
			iphoneDrawPic( x-24,y+23, 22, 22, "iphone/secrets.tga" );
		}
		if ( ch & MF_TREASURE ) {
			iphoneDrawPic( x-2,y+23, 22, 22, "iphone/treasure.tga" );
		}
		if ( ch & MF_TIME ) {
			iphoneDrawPic( x+20,y+23, 22, 22, "iphone/partime.tga" );
		}
		
		// don't let them go to the secret level unless they earned it
		if ( m == 9  && !( ch & MF_TRIED ) ) {
			continue;
		}
		
		if ( TouchReleased( x - 46, y - 9, 88, 32 ) ) {
			PL_NewGame( &Player );
			iphoneStartMap( e, m, s );
		}
	}
}

/*
 ==================
 iphoneStartIntermission
 
 The framesFromNow value allow boss death animations to be triggered
 ==================
 */
void iphoneStartIntermission( int framesFromNow ) {
	// this goes in the savegame if they save at the intermission point,
	// which will cause it to come back up there on return
	currentMap.levelCompleted = 1;
	
	// mark this level as having been completed for the level selection menu
	int mapNum = currentMap.episode * 10 + currentMap.map;
	
	// note that this has been tried now
	currentMap.mapFlags[currentMap.skill][mapNum] |= MF_COMPLETED;

	// mark the awards
	if ( levelstate.time / 70.0f < levelstate.fpartime * 60 ) {	// fpartime is in minutes, time is in tics
		currentMap.mapFlags[currentMap.skill][mapNum] |= MF_TIME;
	}	
	if( levelstate.killed_monsters == levelstate.total_monsters ) {
		currentMap.mapFlags[currentMap.skill][mapNum] |= MF_KILLS;
	}
	if( levelstate.found_secrets == levelstate.total_secrets ) {
		currentMap.mapFlags[currentMap.skill][mapNum] |= MF_SECRETS;
	}
	if( levelstate.found_treasure == levelstate.total_treasure ) {
		currentMap.mapFlags[currentMap.skill][mapNum] |= MF_TREASURE;
	}

	intermissionStartFrameNum = iphoneFrameNum;
	
	if ( framesFromNow ) {
		intermissionTriggerFrame = iphoneFrameNum + framesFromNow;
		return;
	}
		
	intermissionTriggerFrame = 0;
	menuState = IPM_INTERMISSION;
	hasReleased = 0;	// ensure touch up before skipping intermission
}

/*
 ==================
 DrawDigit
 
 ==================
 */
void DrawDigit( int x, int y, int digit ) {	
	R_Bind( numberPics[digit]->texnum );
	pfglBegin( GL_QUADS );
	
	pfglTexCoord2f( 0, 0 );	pfglVertex2i( x, y );
	pfglTexCoord2f( 1, 0 );	pfglVertex2i( x+32, y );
	pfglTexCoord2f( 1, 1 );	pfglVertex2i( x+32, y+32 );
	pfglTexCoord2f( 0, 1 );	pfglVertex2i( x, y+32 );
	
	pfglEnd();
}

void DrawDoubleDigit( int x, int y, int number ) {
	int	step = 32;
	if ( number >= 100 ) {
		// cram three digits into the same space
		DrawDigit( x-8, y, number / 100 );
		number %= 100;
		x += 16;
		step = 24;
	}
	if ( number >= 10 ) {
		DrawDigit( x, y, number / 10 );
		number %= 10;
	}
	DrawDigit( x+step, y, number );
}

void DrawTime( int x, int y, int seconds ) {
	int	min = seconds / 60;
	int sec = seconds % 60;
	
	DrawDoubleDigit( x, y, min );
//	DrawDoubleDigit( x+76, y, sec );
	// always print both digits of seconds, so 2:00 prints correctly
	DrawDigit( x+76, y, sec / 10 );
	DrawDigit( x+76+32, y, sec % 10 );
}

void DrawRatio( int y, int got, int total, const char *bonusPic ) {
	DrawDoubleDigit( 285, y, got );
	DrawDoubleDigit( 361, y, total );
	
	// draw the award icon
	if ( got == total ) {
		iphoneDrawPic( 480 - 40, y, 32, 32, bonusPic );
	}
}

/*
 ==================
 iphoneIntermission
 
 ==================
 */
void iphoneIntermission() {
	int		nextLevel = 0;
	char	str[128];
	
	iphoneDrawPic( 0, 0, 480, 320, "iphone/intermission.tga" );

	// episode
	my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", currentMap.episode+1 );
	iphoneDrawPic( 0, 0, 384, 48, str );

	// level
	iphoneDrawNumber( 430, 0, currentMap.map + 1, 48, 48 );

	// par / time
	DrawTime( 51, 63, levelstate.fpartime * 60 );	// fpartime is in minutes
	DrawTime( 285, 63, levelstate.time / 70 );	// levelstate.time is in tics
	if ( levelstate.time/70 <= levelstate.fpartime * 60 ) {
		iphoneDrawPic( 480 - 40, 63, 32, 32, "iphone/partime.tga" );
	}
	
	// ratios
	DrawRatio( 124, levelstate.killed_monsters, levelstate.total_monsters, "iphone/kills.tga" );
	DrawRatio( 189, levelstate.found_secrets, levelstate.total_secrets, "iphone/secrets.tga" );
	DrawRatio( 255, levelstate.found_treasure, levelstate.total_treasure, "iphone/treasure.tga" );

	// require all touches off before the intermission can exit
	if ( numTouches == 0 && hasReleased == 0 ) {
		hasReleased = 1;
		return;			// don't let the TouchReleased immediately fire
	}
	if ( !hasReleased ) {
		return;
	}
	
	//----------------------
	// tap for next level
	//----------------------
	if ( !TouchReleased( 0, 0, 480, 320 ) ) {
		return;
	}
	
	menuState = IPM_GAME;
	
	PL_NextLevel( &Player );
	
	if( g_version->value == SPEAROFDESTINY ) {
	}
	else
	{
		int		currentLevel = currentMap.episode * 10 + currentMap.map;
		
		if( Player.playstate == ex_secretlevel ) {
			switch( currentLevel ) {
				case 0: nextLevel = 9; break;
				case 10: nextLevel = 19; break;
				case 26: nextLevel = 29; break;
				case 32: nextLevel = 39; break;
				case 44: nextLevel = 49; break;
				case 52: nextLevel = 59; break;
			}
		} else {
			switch ( currentLevel ) {
				case 8:
				case 18:
				case 28:
				case 38:
				case 48:
				case 58:
					// go back to the episode select screen
					menuState = IPM_VICTORY;
					Sound_StartBGTrack( "music/URAHERO.ogg", "music/URAHERO.ogg" );
					return;
				case 9: nextLevel = 1; break;
				case 19: nextLevel = 11; break;
				case 29: nextLevel = 27; break;
				case 39: nextLevel = 33; break;
				case 49: nextLevel = 44; break;
				case 59: nextLevel = 53; break;
				default: nextLevel = currentLevel + 1; break;
			}
		}
	}
	
	iphoneStartMap( (nextLevel/10), (nextLevel%10), skill->value );
}

/*
 ==================
 iphoneVictory
 
 ==================
 */
void iphoneVictory() {
	iphoneDrawPic( 0, 0, 480, 320, "iphone/victory.tga" );
	if ( !TouchReleased( 0, 0, 480, 320 ) ) {
		return;
	}
	menuState = IPM_EPISODE;
}

/*
 ==================
 iphoneAutomap
 
 ==================
 */
float	mapOrigin[2];
float	mapCenterY;
float	mapScale;

typedef struct {
	W8	x, y;
	short	texnum;
} mapTile_t;
#define MAPTILE_SPRITE_FLAG 1024
#define MAX_MAP_TILES 5000		// 4096 tiles + sprites
mapTile_t	mapTiles[MAX_MAP_TILES];
int			numMapTiles;
		
int MapTileSort( const void *a, const void *b ) {
	return ((mapTile_t *)a)->texnum - ((mapTile_t *)b)->texnum;
}

void iphoneOpenAutomap() {
	mapTile_t *mt = mapTiles;
	numMapTiles = 0;
	int		x, y;
	int		xx, yy;
	W32 tx, ty, n;
	sprite_t* sprt;
	
	mapOrigin[0] = Player.position.origin[0] / (float)TILEGLOBAL;
	mapOrigin[1] = Player.position.origin[1] / (float)TILEGLOBAL;
	mapScale = 10;
	menuState = IPM_AUTOMAP;

	// identify all the tiles to fill in
	for( x = 0 ; x < 64; ++x ) {
		for( y = 0 ; y < 64; ++y ) {
			if ( r_world->tilemap[ x ][ y ] & ( WALL_TILE | PUSHWALL_TILE ) ) {
				int		visible = 0;
				// check all 8 surrounding tiles for visibility
				for ( xx = -1 ; xx <= 1 ; xx++ ) {
					if ( x + xx < 0 ) {
						continue;
					}
					if ( x + xx > 63 ) {
						continue;
					}
					for ( yy = -1 ; yy <= 1 ; yy++ ) {
						if ( y + yy < 0 ) {
							continue;
						}
						if ( y + yy > 63 ) {
							continue;
						}
						if ( r_world->tileEverVisible[x+xx][y+yy] ) {
							visible = 1;
							break;
						}
					}
				}
				if ( !visible ) {
					continue;
				}
				int tex = r_world->wall_tex_y[ x ][ y ];
				// special hack for the elevator switch tile, which is always
				// facing east west for the switch, and north south for the railing
				if ( tex == 40 && ( ( x>0 && r_world->tileEverVisible[x+1][y] )
									 || ( x < 63 && r_world->tileEverVisible[x-1][y] ) ) ) {
					tex = r_world->wall_tex_x[ x ][ y ];
				}
				if ( tex < 0x6a ) {	// skip pushwall destinations
					assert( tex >= 0 && tex < 1000 );
					mt->x = x;
					mt->y = y;
					mt->texnum = tex;
					if ( !wallTextures[mt->texnum] ) {
						char	name[1024];
						my_snprintf( name, sizeof( name ), "walls/%.3d.tga", mt->texnum );
						wallTextures[mt->texnum] = TM_FindTexture( name, TT_Wall );
						assert( wallTextures[mt->texnum] ); 
					}
					mt++;
					continue;
				}
			}
			if ( !r_world->tileEverVisible[x][y] ) {
				continue;
			} 
			if( r_world->tilemap[ x ][ y ] & DOOR_TILE ) {
				mt->x = x;
				mt->y = y;
				mt->texnum = r_world->Doors.DoorMap[ x ][ y ].texture;
				if ( !wallTextures[ mt->texnum] ) {
					char	name[1024];
					my_snprintf( name, sizeof( name ), "walls/%.3d.tga", mt->texnum );
					wallTextures[mt->texnum] = TM_FindTexture( name, TT_Wall );
					assert( wallTextures[mt->texnum] ); 
				}
				mt++;
				continue;
			}
			// solid floor
			mt->x = x;
			mt->y = y;
			mt->texnum = -1;
			mt++;			
		}
	}

	// add solid sprite objects	
	for( n = 0, sprt = Spr_Sprites; n < n_of_sprt; ++n, ++sprt ) {
		if( sprt->flags & SPRT_REMOVE ) {
			continue;
		}
		if ( sprt->tex[0] >= SPR_GRD_S_1 ) {
			// don't draw enemies, only static sprites
			continue;
		}
		
		tx = sprt->tilex;
		ty = sprt->tiley;
		
		if( tx > 63 ) {
			continue;
		}
		if( ty > 63 ) {
			continue;
		}
		if ( !r_world->tileEverVisible[tx][ty] ) {
			continue;
		}
		mt->x = tx;
		mt->y = ty;
		mt->texnum = MAPTILE_SPRITE_FLAG | sprt->tex[0];
		mt++;			

		if ( !spriteTextures[ sprt->tex[0] ] ) {
			char	name[1024];
			my_snprintf( name, sizeof( name ), "sprites/%.3d.tga", sprt->tex[0] );
			spriteTextures[sprt->tex[0]] = TM_FindTexture( name, TT_Sprite );
		}
		
		if ( mt == &mapTiles[MAX_MAP_TILES] ) {
			break;	// list is full, some items won't show (shouldn't happen)
		}
	}

	// sort the tiles to be drawn by texture
	numMapTiles = mt - mapTiles;
	
	qsort( mapTiles, numMapTiles, sizeof( mapTiles[0] ), MapTileSort );	
}

void iphoneAutomap() {
	mapTile_t	*mt;
	float	px, py;
	float	angle, c, s;
	int		texnum;
	
	// do touch ops before drawing for minimum latency
	
	// drag for scrolling
	if ( numTouches == 1 ) {
		if ( numPrevTouches == 1 ) {
			mapOrigin[0] -= ( touches[0][0] - prevTouches[0][0] ) / mapScale;
			mapOrigin[1] += ( touches[0][1] - prevTouches[0][1] ) / mapScale;
		}
	}
	
	// pinch for scaling
	if ( numTouches == 2 ) {
		if ( numPrevTouches == 2 ) {
			float	curDist = sqrt( 
								   ( touches[0][0] - touches[1][0] ) * ( touches[0][0] - touches[1][0] ) +
								   ( touches[0][1] - touches[1][1] ) * ( touches[0][1] - touches[1][1] ) );
			float	prevDist = sqrt( 
									( prevTouches[0][0] - prevTouches[1][0] ) * ( prevTouches[0][0] - prevTouches[1][0] ) +
									( prevTouches[0][1] - prevTouches[1][1] ) * ( prevTouches[0][1] - prevTouches[1][1] ) );
			if ( prevDist == 0 ) {
				prevDist = curDist;
			}
			mapScale *= curDist / prevDist;
			if ( mapScale < 4 ) {
				mapScale = 4;
			}
			if ( mapScale > 64 ) {
				mapScale = 64;
			}
		}
		
	}
	
	// todo -- double tap for center on player
	
	
	// set up matrix for drawing in tile units
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	pfglRotatef( 90, 0, 0, 1 );
	pfglOrtho( mapOrigin[0]-240.0 / mapScale, mapOrigin[0]+240.0 / mapScale,
			  mapOrigin[1]-160.0 / mapScale, mapOrigin[1]+160.0 / mapScale, -99999, 99999 );

	mt = mapTiles;
	texnum = 99999;
	for ( int i = 0 ; i < numMapTiles ; i++, mt++ ) {
		if ( texnum != mt->texnum ) {
			texnum = mt->texnum;
			if ( i != 0 ) {
				pfglEnd();
			}
			if ( mt->texnum == -1 ) {
				qglDisable( GL_TEXTURE_2D );
				pfglColor3f( r_world->floorColour[0]/255.0, r_world->floorColour[1]/255.0, r_world->floorColour[2]/255.0 );
			} else if ( mt->texnum & MAPTILE_SPRITE_FLAG ) {
				qglEnable( GL_TEXTURE_2D );
				pfglColor3f( 1, 1, 1 );
				R_Bind( spriteTextures[mt->texnum&~MAPTILE_SPRITE_FLAG]->texnum );
			} else {
				qglEnable( GL_TEXTURE_2D );
				pfglColor3f( 1, 1, 1 );
				R_Bind( wallTextures[mt->texnum]->texnum );
			}
			pfglBegin( GL_QUADS );
		}
		pfglTexCoord2f( 0, 1 );
		pfglVertex2i( mt->x, mt->y );
		pfglTexCoord2f( 1, 1 );
		pfglVertex2i( mt->x+1, mt->y );
		pfglTexCoord2f( 1, 0 );
		pfglVertex2i( mt->x+1, mt->y+1 );
		pfglTexCoord2f( 0, 0 );
		pfglVertex2i( mt->x, mt->y+1 );
	}
	pfglEnd();						
	
	// draw the yellow player triangle
	qglDisable( GL_TEXTURE_2D );
	if ( iphoneFrameNum & 8 ) {	// blink it
		pfglColor3f( 1, 1, 0 );
	} else {
		pfglColor3f( 0.5, 0.5, 0 );
	}
	angle = M_PI * Player.position.angle / (float)ANG_180;
	c = cos( angle );
	s = sin( angle );
	px = Player.position.origin[0] / (float)TILEGLOBAL;
	py = Player.position.origin[1] / (float)TILEGLOBAL;
	pfglBegin( GL_TRIANGLES );
	pfglVertex3f( px + c * 0.5, py + s * 0.5, 0 );
	pfglVertex3f( px - c * 0.5 - s * 0.3, py - s * 0.5 + c * 0.3, 0 );
	pfglVertex3f( px - c * 0.5 + s * 0.3, py - s * 0.5 - c * 0.3, 0 );
	pfglEnd();
	
	qglEnable( GL_TEXTURE_2D );
	pfglColor3f( 1, 1, 1 );
	
	// back button for returning to game
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	pfglRotatef( 90, 0, 0, 1 );
	pfglOrtho( 0, 480, 320, 0, -99999, 99999 );
	if ( BackButton() ) {
		menuState = IPM_GAME;
	}
}

void iphoneDrawMenus() {
	iphoneDrawPic( 0, 0, 480, 320, "iphone/background_1.tga" );
	
	switch ( menuState ) {
		case IPM_MAIN: iphoneMainMenu(); break;
		case IPM_SKILL: iphoneSkillMenu(); break;
		case IPM_EPISODE: iphoneEpisodeMenu(); break;
		case IPM_MAPS: iphoneMapMenu(); break;
		case IPM_CONTROLS: iphoneControlMenu(); break;
		case IPM_INTERMISSION: iphoneIntermission(); break;
		case IPM_VICTORY: iphoneVictory(); break;
		case IPM_AUTOMAP: iphoneAutomap(); break;
	}
}


