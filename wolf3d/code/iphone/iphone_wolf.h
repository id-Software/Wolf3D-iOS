/*
 
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 

 This file is part of the WOLF3D iOS v2.1 GPL Source Code. 

 
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

#include "video.h"
#include "texture_manager.h"
#include "vector.h"
#include "cvar.h"
#include "arch.h"

// define this to get only the first episode on selections, and the
// automatic sell screen at the end of episode 1
//#define EPISODE_ONE_ONLY

// this is the version number displayed on the menu screen
#define WOLF_IPHONE_VERSION 2.1

extern viddef_t viddef;

typedef enum menuState {
	IPM_GAME,
	IPM_MAIN,
	IPM_SKILL,
	IPM_EPISODE,
	IPM_MAPS,
	IPM_MAPSELECTOR, //gsh
	IPM_CONTROLS,
	IPM_INTERMISSION,
	IPM_VICTORY,
	IPM_AUTOMAP,
	IPM_STATS,
	IPM_HUDEDIT,
	IPM_DOWNLOADPROGRESS,  //gsh
	IPM_STOREKIT, //gsh	
//	IPM_LOADING, //gsh
	IPM_TRIVIA, //gsh
	IPM_MORE, //gsh
	IPM_DOWNLOADINSTRUCTIONS, //gsh
} menuState_t;

extern menuState_t menuState;

void iphoneDrawMenus();

// bumped to 107 on moving powerups structure into leveldata
// bumped to 108 with custom huds
#define SAVEGAME_VERSION	108

#define MAX_SKILLS		4
#define MAX_MAPS		81 //60    changed by gsh to allow for SOD levels

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

//gsh
typedef struct {
	int x;
	int y;
	int width;
	int height;
} rect_t;
rect_t RectMake(int x, int y, int width, int height);

typedef struct {
	float x;
	float y;
	float width;
	float height;
} rectFloat_t;

extern currentMap_t currentMap;

void iphoneStartMap( int episodeNum, int mapNum, int skillLevel );
void iphoneStartUserMap( int episodeNum, int mapNum, int skillLevel, char *mapName );  //gsh

extern char iphoneDocDirectory[1024];
extern char iphoneAppDirectory[1024];

extern texture_t *numberPics[10];
#define NUM_MUGSHOTS	26//23
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
#ifdef VOLUMEHACK
extern cvar_t	*volumeFireUp; //gsh
extern cvar_t	*volumeFireDown; //gsh
extern cvar_t	*volumeFireUpSetting; //gsh
extern cvar_t	*volumeFireDownSetting; //gsh
#endif
extern cvar_t	*hudAlpha; //gsh
extern cvar_t	*music;
extern cvar_t	*showTilt;
extern cvar_t	*showTime;
extern cvar_t	*cropSprites;
extern cvar_t	*blends;
extern cvar_t	*gunFrame;
extern cvar_t	*slowAI;
extern cvar_t	*mapScale;
extern cvar_t	*hideControls;
extern cvar_t	*autoFire;

// The size of UI elements will be scaled by this value in order to support different
// resolutions, such as Retina displays and the iPad.
extern float screenScale;

// the native iPhone code should set the following each frame:
extern int	numTouches;
extern int	touches[5][2];	// [0] = x, [1] = y in landscape mode, raster order with y = 0 at top
extern int isTouchMoving; //gsh

// On Retina displays, touch coordinates are still reported at "low resolution". Since the game
// compares touch coordinates directly to the on-screen positions of buttons, we will need to
// scale the touches on Retina displays.
extern int touchCoordinateScale;

// Store the device's contentScaleFactor ourselves. This is needed to support iOS
// versions < 4.0, as the UIView contentScaleFactor property did not exist before then.
// This should be 1 for all non-Retina displays, and 2 for Retina displays.
extern float deviceScale;

// The orientation of the device. Currently, we only support both landscape modes.
// LEFT and RIGHT correspond to the position of the home button as you look at the
// device from the front.
typedef enum {
	ORIENTATION_LANDSCAPE_LEFT,
	ORIENTATION_LANDSCAPE_RIGHT
} deviceOrientation_t;

extern deviceOrientation_t deviceOrientation;

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
void iphoneCenterTextWithColor( int x, int y, const char *str, colour4_t color );  //gsh
int iphoneDrawText( int x, int y, int width, int height, const char *str ); //gsh
int iphoneDrawArialText( int x, int y, float scale, const char *str ); //gsh
int iphoneCenterArialText( int x, int y, float scale, const char *str ); //gsh
int iphoneDrawArialTextInBox( rect_t paragraph, int lineLength, const char *str, rect_t boxRect ); //gsh
void iphoneDrawTextWithColor( rect_t rect, const char *str, float colors[4] ); //gsh
void iphoneDrawMapName( rect_t rect, const char *str ); //gsh
int iphoneDrawTextInBox( rect_t paragraph, int lineLength, const char *str, rect_t boxRect ); //gsh
void iphoneDrawNumber( int x, int y, int number, int charWidth, int charHeight );
void iphoneDrawPic( int x, int y, int w, int h, const char *pic );
void iphoneDrawPicFloat( float x, float y, float w, float h, const char *pic );
void iphoneDrawPicRect( rectFloat_t rect, const char * pic );
int iphoneDrawPicWithTouch( int x, int y, int w, int h, const char *pic );
int iphoneDrawPicRectWithTouch( rectFloat_t rect, const char *pic );
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

void ScaleToScreen( int * value);
void ScalePosition( float * x, float * y );
void ScalePositionInt( int * x, int * y );
void ScalePositionAndSize( float * x, float * y, float * w, float * h );
void ScalePositionAndSizeInt( int * x, int * y, int * w, int * h );
void ScaleRect( rect_t * rect );
void ScaleRectFloat( rectFloat_t * rect );
rectFloat_t MakeScaledRectFloat( float x, float y, float width, float height );

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
	int		drawWidth, drawHeight;		//from here down (in the struct) is from doom iphone... gsh
	int		touchWidth, touchHeight;
	texture_t *texture;
	bool	drawAsLimit;
	float	touchState;
	//struct touch_s *touch;
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
void iphoneHudEditFrame();


//---------------------------------------
// Content version management for Lite and Platinum
//---------------------------------------
typedef enum {
	CONTENT_LITE,
	CONTENT_PLATINUM
} contentVersion_t;

//---------------------------------------
// interfaces from the original game code
//---------------------------------------
void iphoneStartBonusFlash();
void iphoneStartDamageFlash( int points );
void iphoneSetAttackDirection( int dir );
void iphoneStartIntermission( int framesFromNow );
void iphoneSetNotifyText( const char *str, ... );
void iphoneSetLevelNotifyText(); //gsh
	
//---------------------------------------
// interfaces to Objective-C land
//---------------------------------------
void SysIPhoneSwapBuffers();
void SysIPhoneVibrate();
void SysIPhoneOpenURL( const char *url );
void SysIPhoneLoadJPG( W8* jpegData, int jpegBytes, W8 **pic, W16 *width, W16 *height, W16 *bytes );
const char * SysIPhoneGetConsoleTextField();
void SysIPhoneSetConsoleTextField(const char *);
void SysIPhoneInitAudioSession();
int SysIPhoneOtherAudioIsPlaying();
const char *SysIPhoneGetOSVersion();
contentVersion_t SysIPhoneGetContentVersion();
int SysIPhoneGetPathToMainBundleLength( void );
void SysIPhoneGetPathToMainBundle( char * outPath, int maxLength );

void iphoneStartPreviousMenu();
void iphoneStartMainMenu();
void iphonePromptToBuyPlatinum();

void iphoneInitMenuMusic();
void iphoneStartMenuMusic();
void iphoneStopMenuMusic();

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
void iphoneStartGameplay();

void iphoneResume();

void LoadPNG( const char *filename, W8 **pic, W16 *width, W16 *height, W16 *bytes );

// The width and height the assets were originally designed for. Used in scaling to different
// resolutions for preserving aspect ratio.
#define REFERENCE_WIDTH		480.0f
#define REFERENCE_HEIGHT	320.0f

// Constants for HUD buttons.
#define BACK_BUTTON_WIDTH	64.0f
#define BACK_BUTTON_HEIGHT	64.0f

// These variables store the scaled dimensions of certain HUD elements that are needed
// in various parts of the code.
extern float faceWidth;
extern float faceHeight;

extern float weaponWidth;
extern float weaponHeight;


// The following items support in-app purchases.
#define PLATINUM_UPGRADE_STRING "com.idsoftware.wolf3dlite.platupgrade"


// An enum to help keep track of YesNo boxes.
typedef enum {
	YESNO_NONE,
	YESNO_BUY_PLATINUM,
	YESNO_GO_TO_WEBSITE,
	YESNO_DOWNLOAD_INSTRUCTIONS
} yesNoBoxType_t;

extern yesNoBoxType_t currentYesNoBox;

// This determines the default framerate of the game.
// 1 = 60Hz, 2 = 30Hz, etc.
#define DEFAULT_FRAME_INTERVAL 1

// Determine phone or tablet device at runtime.
typedef enum {
	DEVICE_UNKNOWN,
	DEVICE_PHONE,
	DEVICE_TABLET
} deviceType_t;

deviceType_t iphoneGetDeviceType(void);
