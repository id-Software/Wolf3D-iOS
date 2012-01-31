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

// define this to get only the first episode on selections, and the
// automatic sell screen at the end of episode 1
//#define EPISODE_ONE_ONLY

// this is the version number displayed on the menu screen
#define WOLF_IPHONE_VERSION 1.1

extern viddef_t viddef;

typedef enum menuState {
	IPM_GAME,
	IPM_MAIN,
	IPM_SKILL,
	IPM_EPISODE,
	IPM_MAPS,
	IPM_CONTROLS,
	IPM_INTERMISSION,
	IPM_VICTORY,
	IPM_AUTOMAP,
	IPM_STATS,
	IPM_HUDEDIT
	
} menuState_t;

extern menuState_t menuState;

void iphoneDrawMenus();

// bumped to 107 on moving powerups structure into leveldata
// bumped to 108 with custom huds
#define SAVEGAME_VERSION	108

#define MAX_SKILLS		4
#define MAX_MAPS		60

#define MF_TRIED		1
#define MF_COMPLETED	2
#define MF_KILLS		4
#define MF_SECRETS		8
#define MF_TREASURE		16
#define MF_TIME			32

typedef struct {
	int		episode;
	int		map;
	int		skill;
	int		levelCompleted;		// already at intermission when saved
	int		version;
	int		mapFlags[MAX_SKILLS][MAX_MAPS];
} currentMap_t;

extern currentMap_t currentMap;

void iphoneStartMap( int episodeNum, int mapNum, int skillLevel );

extern char iphoneDocDirectory[1024];
extern char iphoneAppDirectory[1024];

extern texture_t *numberPics[10];
#define NUM_MUGSHOTS	23
extern char *mugshotnames[ NUM_MUGSHOTS ];

extern vec3_t	vnull;

void Client_PrepRefresh( const char *r_mapname );

extern int		iphoneFrameNum;
extern int		intermissionTriggerFrame;
extern int		consoleActive;

extern cvar_t	*controlScheme;
extern cvar_t	*sensitivity;
extern cvar_t	*stickTurnBase;
extern cvar_t	*stickTurnScale;
extern cvar_t	*stickMoveBase;
extern cvar_t	*stickMoveScale;
extern cvar_t	*stickDeadBand;
extern cvar_t	*tiltTurn;
extern cvar_t	*tiltMove;
extern cvar_t	*tiltDeadBand;
extern cvar_t	*tiltAverages;
extern cvar_t	*tiltFire;
extern cvar_t	*music;
extern cvar_t	*showTilt;
extern cvar_t	*showTime;
extern cvar_t	*cropSprites;
extern cvar_t	*blends;
extern cvar_t	*gunFrame;
extern cvar_t	*slowAI;
extern cvar_t	*revLand;
extern cvar_t	*mapScale;
extern cvar_t	*hideControls;
extern cvar_t	*autoFire;

// the native iPhone code should set the following each frame:
extern int	numTouches;
extern int	touches[5][2];	// [0] = x, [1] = y in landscape mode, raster order with y = 0 at top
extern float	tilt;		// -1.0 to 1.0
extern float	tiltPitch;

// so we can detect button releases
extern int	numPrevTouches;
extern int prevTouches[5][2];

typedef struct {
	int		enterFrame;
	int		beforeSwap;
	int		afterSwap;
} logTime_t;
#define MAX_LOGGED_TIMES	512
extern logTime_t	loggedTimes[MAX_LOGGED_TIMES];	// indexed by iphoneFrameNum

void LoadWallTexture( int wallPicNum );

int	TouchDown( int x, int y, int w, int h );
int	TouchReleased( int x, int y, int w, int h );
int iphoneCenterText( int x, int y, const char *str );
void iphoneDrawNumber( int x, int y, int number, int charWidth, int charHeight );
void iphoneDrawPic( int x, int y, int w, int h, const char *pic );
int iphoneDrawPicWithTouch( int x, int y, int w, int h, const char *pic );
void iphoneDrawPicNum( int x, int y, int w, int h, int glTexNum );
void R_Draw_Blend( int x, int y, int w, int h, colour4_t c );
void SaveTheGame();
int LoadTheGame();
void StartGame();
void iphoneOpenAutomap();
void iphoneDrawFace();
void iphoneDrawNotifyText();
void iphonePreloadBeforePlay();

void InitImmediateModeGL();
void iphoneRotateForLandscape();
void iphoneCheckForLandscapeReverse();

extern colour4_t colorPressed;

extern int damageflash;
extern int bonusFrameNum;
extern int attackDirTime[2];


#define HF_DISABLED				1

typedef struct {
	int		x, y;
	int		width, height;
	int		glTexNum;
	int		hudFlags;
} hudPic_t;

//#define ALLOW_MAP_VIEW_HUD

typedef struct {
	hudPic_t	forwardStick;
	hudPic_t	sideStick;
	hudPic_t	turnStick;
	hudPic_t	fire;
	hudPic_t	menu;
	hudPic_t	map;
	hudPic_t	ammo;
#ifdef ALLOW_MAP_VIEW_HUD	
	hudPic_t	mapView;
#endif	
} hud_t;

extern hud_t	huds;

void HudSetForScheme( int schemeNum );
void HudSetTexnums();
void HudEditFrame();



//---------------------------------------
// interfaces from the original game code
//---------------------------------------
void iphoneStartBonusFlash();
void iphoneStartDamageFlash( int points );
void iphoneSetAttackDirection( int dir );
void iphoneStartIntermission( int framesFromNow );
void iphoneSetNotifyText( const char *str, ... );
	
//---------------------------------------
// interfaces to Objective-C land
//---------------------------------------
void SysIPhoneSwapBuffers();
void SysIPhoneVibrate();
void SysIPhoneOpenURL( const char *url );
void SysIPhoneSetUIKitOrientation( int isLandscapeRight );
void SysIPhoneLoadJPG( W8* jpegData, int jpegBytes, W8 **pic, W16 *width, W16 *height, W16 *bytes );
const char * SysIPhoneGetConsoleTextField();
void SysIPhoneSetConsoleTextField(const char *);
void SysIPhoneInitAudioSession();
int SysIPhoneOtherAudioIsPlaying();
const char *SysIPhoneGetOSVersion();

//---------------------------------------
// interfaces from Objective-C land
//---------------------------------------
void iphoneStartup();
void iphoneShutdown();
void iphoneFrame();
void iphoneTiltEvent( float *tilts );
void iphoneTouchEvent( int numTouches, int touches[16] );
void iphoneActivateConsole();
void iphoneDeactivateConsole();
void iphoneExecuteCommandLine();

