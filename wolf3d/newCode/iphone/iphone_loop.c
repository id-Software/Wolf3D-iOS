/*
 remove wolf_render.c
 for the iphone build, we only need the following lsfx files:
 
 001
 003
 008
 009
 012
 023
 028
 030
 031
 033
 034
 035
 036
 037
 038
 040
 044
 045
 061
 062
 064
 069
 076
 078
 080
 085
 086

 */

#include "../wolfiphone.h"

currentMap_t currentMap;

vec3_t	vnull = { 0, 0, 0 };

int		iphoneFrameNum;
int		intermissionTriggerFrame;
int		slowAIFrame;

// console mode
int consoleActive;

// the native iPhone code should set the following each frame:
int	numTouches;
int	touches[5][2];	// [0] = x, [1] = y in landscape mode, raster order with y = 0 at top
float	tilt;		// -1.0 to 1.0
float	tiltPitch;

#define MAX_TILT_HISTORY 64
float	tiltHistory[MAX_TILT_HISTORY][4];
int		tiltHistoryNum;

// so we can detect button releases
int	numPrevTouches;
int prevTouches[5][2];


// the layout drawing code sets these, which are then used
// by the touch processing
int	fireButtonX, fireButtonY, fireButtonSize;
int	moveAxisX, moveAxisY, moveAxisSize;
int	turnAxisX, turnAxisY, turnAxisSize;

texture_t *numberPics[10];

int damageflash;
int bonusFrameNum;
int attackDirTime[2];

/*
 ==================
 iphoneActivateConsole
 
 ==================
 */
void iphoneActivateConsole() {
	extern float scr_conlines;
	scr_conlines = 0.5f;
	consoleActive = 1;
}

/*
 ==================
 iphoneDeactivateConsole
 
 ==================
 */
void iphoneDeactivateConsole() {
	extern float scr_conlines;
	scr_conlines = 0.0f;
	consoleActive = 0;
}

/*
 ==================
 iphoneExecuteCommandLine
 
 ==================
 */
void iphoneExecuteCommandLine() {
	const char * GetCurrentCommandLine();
	void SetCurrentCommandLine(const char *);
	char buf[256];
	strcpy( buf, GetCurrentCommandLine() );
	
	Cmd_ExecuteString( buf );
	SetCurrentCommandLine( "" );
}



/*
 ==================
 iphoneSavePrevTouches
 
 ==================
 */
void iphoneSavePrevTouches() {
	numPrevTouches = numTouches;
	memcpy( prevTouches, touches, sizeof( prevTouches ) );
#if 0	
	// display the touch locations
	for ( int i = 0 ; i < numTouches ; i++ ) {
		int	w = 32;
		iphoneDrawPic( touches[i][0] - w/2, touches[i][1] - w/2, w, w, "iphone/diractional_02.tga" );
	}
#endif
}

/*
 ==================
 iphoneCenterText
 
 Returns the width in pixels
 ==================
 */
extern font_t *myfonts[ 1 ];
int iphoneCenterText( int x, int y, const char *str ) {
	int l = strlen( str );
	int	i;
	font_t *myfont = myfonts[0];
	int		scale;
	int		step = 10;
	
	scale = 16;
	
	x -= l * step / 2;
	
	R_Bind( myfont->texfont->texnum );
	pfglBegin( GL_QUADS );

	for ( i = 0 ; i < l ; i++, x += step ) { 
		int	row, col; 
		float frow, fcol;
		int	num = str[i];
		
		if ( num == ' ' ) {
			continue;
		}
		
		row = (num >> 4) - 2;
		col = num & 15;
		
		frow = row * myfont->hFrac;
		fcol = col * myfont->wFrac;
	
		pfglTexCoord2f( fcol, frow );							
		pfglVertex2i( x, y );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow );					
		pfglVertex2i( x+scale, y );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow+myfont->hFrac );	
		pfglVertex2i( x+scale, y+scale );
		
		pfglTexCoord2f( fcol, frow+myfont->hFrac );
		pfglVertex2i( x, y+scale );			
	}
	
	pfglEnd();
	
	return l * step;
}


/*
 ==================
 TouchDown
 
 Checks all touches against a square
 ==================
 */
int	TouchDown( int x, int y, int w, int h ) {
	int	i;
	for ( i = 0 ; i < numTouches ; i++ ) {
		if ( touches[i][0] >= x && touches[i][1] >= y
			&& touches[i][0] < x + w && touches[i][1] < y + h ) {
			return 1;
		}
	}
	return 0;
}

/*
 ==================
 TouchReleased
 
 Perform an action when released in the box.
 If not down this frame, but down the previous frame, it is released
 ==================
 */
int	TouchReleased( int x, int y, int w, int h ) {
	int	i;
	int	downPrev = 0;
	int downNow = 0;
	
	for ( i = 0 ; i < numPrevTouches ; i++ ) {
		if ( prevTouches[i][0] >= x && prevTouches[i][1] >= y
			&& prevTouches[i][0] < x + w && prevTouches[i][1] < y + h ) {
			downPrev = 1;
			break;
		}
	}
	
	// see if not down this frame
	for ( i = 0 ; i < numTouches ; i++ ) {
		if ( touches[i][0] >= x && touches[i][1] >= y
			&& touches[i][0] < x + w && touches[i][1] < y + h ) {
			downNow = 1;
			break;
		}
	}
	
	if ( !downPrev ) {
		if ( downNow ) {
			Sound_StartLocalSound( "iphone/bdown_01.wav" );
		}
		// wasn't down the previous frame
		return 0;
	}
	
	if ( downNow ) {
		// still down
		return 0;
	}
	
	if ( numTouches != 0 ) {
		// finger dragged off
		Sound_StartLocalSound( "iphone/baborted_01.wav" );
		return 0;
	}
	
	// released
	Sound_StartLocalSound( "iphone/baction_01.wav" );
	return 1;
}

/*
 ==================
 iphoneSet2D
 
 ==================
 */
void iphoneSet2D( void ) {
	pfglViewport( 0,0, 480, 320 );
	pfglMatrixMode( GL_MODELVIEW );
    pfglLoadIdentity();
	pfglDisable( GL_DEPTH_TEST );
	pfglDisable( GL_CULL_FACE );
	pfglEnable( GL_BLEND );
	pfglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	pfglDisable( GL_ALPHA_TEST );
	pfglColor4f( 1,1,1,1 );
	
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	pfglRotatef( 90, 0, 0, 1 );
	pfglOrtho( 0, 480, 320, 0, -99999, 99999 );
}


/*
 ==================
 DeadBandAdjust
 
 Compresses the 0.0 - 1.0 range into deadband - 1.0
 ==================
 */
float DeadBandAdjust( float f, float deadBand ) {
	if ( f < 0 ) {
		return -DeadBandAdjust( -f, deadBand );
	}
	if ( f > 1.0 ) {
		return 1.0;
	}
	if ( f < deadBand ) {
		return 0;
	}
	return (f-deadBand) / (1.0 - deadBand);
}

/*
==================
AxisHit

Returns a -1 to 1 range

If activeFraction is less than 1.0, the range will clamp
to the limits before the edge of the box is hit.
==================
*/
float	AxisHit( int x, int y, int size, int isXaxis, float activeFraction ) {
	int	i;
	
	// allow the touches to go outside the indicated bounds with clamping
	x -= size/2;
	y -= size/2;
	size *= 2;
	activeFraction *= 0.5f;
	
	for ( i = 0 ; i < numTouches ; i++ ) {
		if ( touches[i][0] >= x && touches[i][1] >= y
			&& touches[i][0] < x + size && touches[i][1] < y + size ) {
				float	f;
				if ( isXaxis ) {
					f = (float)( touches[i][0] - x ) / size * 2.0f - 1.0f;
				} else {
					f = (float)( touches[i][1] - y ) / size * 2.0f - 1.0f;
				}
				f /= activeFraction;
				if ( f > 1.0f ) {
					f = 1.0f;
				} else if ( f < -1.0f ) {
					f = -1.0f;
				}
				if ( f > -stickDeadBand->value && f < stickDeadBand->value ) {
					f = 0;
				}
				return f;
		}
	}
	return 0;
}

void WolfensteinTouches( int _numTouches, int _touches[16] ) {
	numTouches = _numTouches;
	for ( int i = 0 ; i < numTouches ; i++ ) {
		touches[i][0] = 480 - _touches[i*2+1];
		touches[i][1] = _touches[i*2+0];
	}
}

void WolfensteinTilts( float *tilts ) {
	int		i;
	int		j;
	int		c;
	float	sum[3];
	static float prevTime;
	
	c = tiltAverages->value;
	if ( c < 1 ) {
		c = 1;
	} else if ( c > MAX_TILT_HISTORY ) {
		c = MAX_TILT_HISTORY;
	}
	
	// acc[0] - [2] are accelerometer values, ax[3] is the timestamp
	for ( i = 0 ; i < 3 ; i++ ) {
		tiltHistory[tiltHistoryNum&(MAX_TILT_HISTORY-1)][i] = tilts[i];
		sum[i] = 0;
		for ( j = 0 ; j < c ; j++ ) {
			sum[i] += tiltHistory[(tiltHistoryNum-j)&(MAX_TILT_HISTORY-1)][i];
		}
		sum[i] /= c;
	}
	// save the timestamp for analysis
	tiltHistory[tiltHistoryNum&(MAX_TILT_HISTORY-1)][3] = tilts[3] - prevTime;	
	prevTime = tilts[3];
	tiltHistoryNum++;
	
	tilt = sum[1];
	tiltPitch = sum[0];
//	Com_Printf( "%4.2f %4.2f %4.2f\n", tilts[0], tilts[1], tilts[2] );
}

void ShowTilt() {
	int	i;
	int	axis = (int)showTilt->value;
	colour4_t fillColor = { 255, 0, 0, 255 };
	colour4_t whiteColor = { 255, 255, 255, 255 };
	colour4_t nowColor = { 0, 255, 0, 255 };
	float	x;
	
	if ( axis < 0 || axis > 2 ) {
		return;
	}
	for ( i = 0 ; i < MAX_TILT_HISTORY ; i++ ) {
		x = tiltHistory[(tiltHistoryNum-1-i)&(MAX_TILT_HISTORY-1)][axis] * ( 10 / 0.018168604 );
		if ( x < 0 ) {
			R_Draw_Fill( 240 + x, i*4, -x, 4, fillColor );
		} else if ( x > 0 ) {
			R_Draw_Fill( 240, i*4, x, 4, fillColor );
		}
	}
	x = tilt * ( 10 / 0.018168604 );
	if ( x < 0 ) {
		R_Draw_Fill( 240 + x, i*4, -x, 4, nowColor );
	} else if ( x > 0 ) {
		R_Draw_Fill( 240, i*4, x, 4, nowColor );
	}
	R_Draw_Fill( 240, 0, 1, MAX_TILT_HISTORY*4, whiteColor );
}

					 
/*
==================
CreateIphoneUserCmd

Build the movement, angles, and buttons for a frame of action:

Player.position.angle
Player.cmd.buttons
Player.cmd.forwardMove
Player.cmd.sideMove
==================
*/
PRIVATE void CreateIphoneUserCmd()
{
	float	stickTurnValue;
	float	stickMoveValue;
	
	stickTurnValue = stickTurnBase->value + stickTurnScale->value * sensitivity->value;
	stickMoveValue = stickMoveBase->value + stickMoveScale->value * sensitivity->value;
	
	usercmd_t *cmd = &Player.cmd;
	memset( cmd, 0, sizeof( *cmd ) );
		   
	if ( TouchDown( fireButtonX, fireButtonY, fireButtonSize, fireButtonSize ) ) {
		cmd->buttons |= BUTTON_ATTACK;
	}
	if ( tiltFire->value > 0 && tiltPitch < tiltFire->value ) {
		cmd->buttons |= BUTTON_ATTACK;
	}
	
	cmd->forwardmove = -stickMoveValue * AxisHit( moveAxisX, moveAxisY, moveAxisSize, 0, 0.8f );
   
	if( controlScheme->value == 0 || controlScheme->value == 1 ) {
		Player.position.angle += -stickTurnValue * AxisHit( moveAxisX, moveAxisY, moveAxisSize, 1, 0.8f );
	} else {
		Player.position.angle += -stickTurnValue * AxisHit( turnAxisX, turnAxisY, turnAxisSize, 1, 0.8f );
		cmd->sidemove = stickMoveValue * AxisHit( moveAxisX, moveAxisY, moveAxisSize, 1, 0.8f );
	}

	// accelerometer tilting
	cmd->sidemove += tiltMove->value * DeadBandAdjust( tilt, tiltDeadBand->value );
	Player.position.angle -= tiltTurn->value * DeadBandAdjust( tilt, tiltDeadBand->value );
	
	// always use
	if ( iphoneFrameNum & 1 ) {
		cmd->buttons |= BUTTON_USE;
	}
}


/*
==================
iphoneHighlightPicWhenTouched

Draw transparent except when touched
=================
*/
void iphoneHighlightPicWhenTouched( int x, int y, int w, int h, const char *pic ) {
	if ( TouchDown( x, y, w, h ) ) {
		pfglColor4f(1,1,1,1);
	} else {
		pfglColor4f(1,1,1,0.5);
	}
	R_Draw_StretchPic( x, y, w, h, pic );
	pfglColor4f(1,1,1,1);
}
int iphoneActivatePicWhenTouched( int x, int y, int w, int h, const char *pic ) {
	iphoneHighlightPicWhenTouched( x, y, w, h, pic );
	return TouchReleased( x, y, w, h );
}


/*
==================
iphoneDrawControls

 scheme 0: turnstick on left
 scheme 1: turnstick on right
 scheme 2: dualstick, move on left
 scheme 3: dualstick, move on right
=================
*/
static const int BUTTON_SIZE = 100;
void iphoneDrawControls( void ) {
	int	ss = (int)stickSize->value;

	switch ( (int)controlScheme->value ) {
		case 0:
			moveAxisX = 0;
			moveAxisY = 320 - ss;
			moveAxisSize = ss;
			iphoneHighlightPicWhenTouched( moveAxisX, moveAxisY, moveAxisSize, moveAxisSize, "iphone/diractional_02.tga" );
			
			fireButtonX = 480 - BUTTON_SIZE;
			fireButtonY = 320 - BUTTON_SIZE;
			fireButtonSize = BUTTON_SIZE;
			break;
			
		case 1:
			moveAxisX = 480 - ss;
			moveAxisY = 320 - ss;
			moveAxisSize = ss;
			iphoneHighlightPicWhenTouched( moveAxisX, moveAxisY, moveAxisSize, moveAxisSize, "iphone/diractional_02.tga" );
			
			fireButtonX = 0;
			fireButtonY = 320 - BUTTON_SIZE;
			fireButtonSize = BUTTON_SIZE;
			break;
			
		case 2:
			moveAxisX = 0;
			moveAxisY = 320 - ss;
			moveAxisSize = ss;
			iphoneHighlightPicWhenTouched( moveAxisX, moveAxisY, moveAxisSize, moveAxisSize, "iphone/diractional_01.tga" );
			
			turnAxisX = 480 - ss;
			turnAxisY = 320 - ss;
			turnAxisSize = ss;
			iphoneHighlightPicWhenTouched( turnAxisX, turnAxisY, turnAxisSize, turnAxisSize, "iphone/diractional_03.tga" );
			
			fireButtonX = 480-BUTTON_SIZE;
			fireButtonY = 0;
			fireButtonSize = BUTTON_SIZE;
			break;
		case 3:
			moveAxisX = 480 - ss;
			moveAxisY = 320 - ss;
			moveAxisSize = ss;
			iphoneHighlightPicWhenTouched( moveAxisX, moveAxisY, moveAxisSize, moveAxisSize, "iphone/diractional_01.tga" );
			
			turnAxisX = 0;
			turnAxisY = 320 - ss;
			turnAxisSize = ss;
			iphoneHighlightPicWhenTouched( turnAxisX, turnAxisY, turnAxisSize, turnAxisSize, "iphone/diractional_03.tga" );
			
			fireButtonX = 480-BUTTON_SIZE;
			fireButtonY = 0;
			fireButtonSize = BUTTON_SIZE;
			break;
	}

	iphoneHighlightPicWhenTouched( fireButtonX, fireButtonY, fireButtonSize, fireButtonSize, "iphone/shoot.tga" );
}

/*
 ==================
 iphoneDrawWeapon
 
 ==================
 */
void iphoneDrawWeapon( void ) {
	char name[ 32 ];
	texture_t *tex;	
	static int w = 200;
	static int h = 200;
	int x = (viddef.width - w ) >> 1;
	int y = viddef.height - 80 - h;
	int	frame;
	
	if ( gunFrame->value ) {
		// screenshots look better with the muzzle flash
		frame = Player.weapon * 5 + gunFrame->value + SPR_KNIFEREADY;
	} else {
		frame = Player.weapon * 5 + Player.weaponframe + SPR_KNIFEREADY;
	}
	
	my_snprintf( name, sizeof( name ), "%s/%d.tga", spritelocation, frame);
	tex = TM_FindTexture( name, TT_Pic );

	R_Bind( tex->texnum );

	pfglBegin( GL_QUADS );
	
	pfglTexCoord2f( 0.01f, 0.01f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 0.99f, 0.01f );	pfglVertex2i( x + w, y );
	pfglTexCoord2f( 0.99f, 0.99f );	pfglVertex2i( x + w, y + h );
	pfglTexCoord2f( 0.01f, 0.99f );	pfglVertex2i( x, y + h );
	
	pfglEnd();
}

/*
 ==================
 iphoneDrawNumber
 
 ==================
 */
void iphoneDrawNumber( int x, int y, int number, int charWidth, int charHeight ) {	
	texture_t *tex;
	int i;
	char string[ 20 ];
	W32 length;
	float charStep = charWidth * 0.8;	// trim off extra width

	if ( number < 0 ) {
		number = 0;
	}
	my_snprintf( string, sizeof( string ), "%d", number );
	length = strlen( string );
	
	x -= length * charStep / 2;
	
	for( i = 0 ; i < length ; i++ ) {
		int digit = string[i] - '0';
		tex = numberPics[digit];
		R_Bind( tex->texnum );
		pfglBegin( GL_QUADS );
		
		pfglTexCoord2f( 0, 0 );	pfglVertex2i( x, y );
		pfglTexCoord2f( 1, 0 );	pfglVertex2i( x+charWidth, y );
		pfglTexCoord2f( 1, 1 );	pfglVertex2i( x+charWidth, y+charHeight );
		pfglTexCoord2f( 0, 1 );	pfglVertex2i( x, y+charHeight );
		
		pfglEnd();
		x += charStep;
	}
}

/*
 ==================
 iphoneDrawHUD
 
 ==================
 */
void iphoneDrawHUD( void ) {
	int		y;
	
	if( Player.items & ITEM_KEY_1 ) {
		R_Draw_Pic( 0, 100, "iphone/GOLDKEYPIC.tga" );
	}
	
	if( Player.items & ITEM_KEY_2 ) {
		R_Draw_Pic( 0, 130, "iphone/SILVERKEYPIC.tga" );
	}
	
	iphoneDrawNumber( 240, 304, Player.health, 16, 16 );
	if ( fireButtonY < 160 ) {
		y = fireButtonY + fireButtonSize + 8;
	} else {
		y = fireButtonY - 8 - 48;
	}
	iphoneDrawNumber( fireButtonX + fireButtonSize/2, y, Player.ammo[AMMO_BULLETS], 48, 48 );
}

/*
 ==================
 iphoneDrawFace
 
 ==================
 */
void iphoneDrawFace( void ) {
	int i;
	int w = 64;
	int h = 80;
	int x = (viddef.width - w ) >> 1;
	int y = viddef.height - h;
	const char *pic;
	static const char *mugshotnames[ 24 ] =
	{
		"iphone/newhead/FACE1APIC.tga",
		"iphone/newhead/FACE1BPIC.tga",
		"iphone/newhead/FACE1CPIC.tga",
		
		"iphone/newhead/FACE2APIC.tga",
		"iphone/newhead/FACE2BPIC.tga",
		"iphone/newhead/FACE2CPIC.tga",
		
		"iphone/newhead/FACE3APIC.tga",
		"iphone/newhead/FACE3BPIC.tga",
		"iphone/newhead/FACE3CPIC.tga",
		
		"iphone/newhead/FACE4APIC.tga",
		"iphone/newhead/FACE4BPIC.tga",
		"iphone/newhead/FACE4CPIC.tga",
		
		"iphone/newhead/FACE5APIC.tga",
		"iphone/newhead/FACE5BPIC.tga",
		"iphone/newhead/FACE5CPIC.tga",
		
		"iphone/newhead/FACE6APIC.tga",
		"iphone/newhead/FACE6BPIC.tga",
		"iphone/newhead/FACE6CPIC.tga",
		
		"iphone/newhead/FACE7APIC.tga",
		"iphone/newhead/FACE7BPIC.tga",
		"iphone/newhead/FACE7CPIC.tga",
		
		"iphone/newhead/FACE8APIC.tga"
	};
	
	static const char *godmugshotnames[] =
	{
		"iphone/newhead/GODMODEFACE0PIC.tga",
		"iphone/newhead/GODMODEFACE1PIC.tga",
		"iphone/newhead/GODMODEFACE2PIC.tga"	
	};

	iphoneDrawPic( 240 - 64, 320 - 80, 128, 80, "iphone/status_hud.tga" );
	
	Player.facecount += tics;
	if ( Player.face_gotgun && Player.facecount > 0 ) {
		// gotgun will set facecount to a negative number initially, go back
		// to normal face with random look after expired.
		Player.face_gotgun = false;
	}
	if( Player.facecount > US_RndT() )
	{
		Player.face_gotgun = Player.face_ouch = false;
		Player.faceframe = US_RndT() >> 6;
		if( Player.faceframe == 3 )
		{
			Player.faceframe = 0;
		}
		
		Player.facecount = 0;
	}

	if( Player.health )
	{
		if( g_version->value == SPEAROFDESTINY && Player.flags & FL_GODMODE )
		{
			pic = godmugshotnames[ Player.faceframe ];		
		}
		else if( Player.face_gotgun )
		{
			pic = "iphone/newhead/GOTGATLINGPIC.tga";		
		}
		else
		{
			int h = Player.health;
			if ( h > 100 ) {
				h = 100;
			}
			if ( h < 0 ) {
				h = 0;
			}
			pic = mugshotnames[ 3*((100-h)/16)+Player.faceframe ];
		}
	}
	else
	{
#if 0		// forgot to convert the mutant face pic...
		if( Player.LastAttacker && Player.LastAttacker->type == en_needle )
		{
			pic = "iphone/MUTANTBJPIC.tga";
		}
		else
#endif			
		{
			pic = "iphone/newhead/FACE8APIC.tga";
		}
	}

	R_Draw_StretchPic( x, y, w, h, pic );
	
	// blend the right / left damage indicators on the side
	for ( i = 0 ; i < 2 ; i++ ) {
		float	f;
		if ( attackDirTime[i] == 0 ) {
			continue;
		}
		f = iphoneFrameNum - attackDirTime[i];
		if ( f > 30 ) {
			attackDirTime[i] = 0;
			continue;
		}
		if ( f < 20 ) {
			f = 1.0;
		} else {
			f = ( 30 - f ) * 0.1;
		}
		pfglColor4f( 1, 1, 1, f );
		if ( i == 0 ) {
			iphoneDrawPic( 240 - 64, 320 - 80, 40, 80, "iphone/L_damage.tga" );
		} else {
			iphoneDrawPic( 240 + 64 - 40, 320 - 80, 40, 80, "iphone/R_damage.tga" );
		}
		pfglColor4f( 1, 1, 1, 1 );
	}
}

/*
 ==================
 iphoneSetNotifyText
 
 Notify text is a single centered line for "got a key", "found a secret", etc
 ==================
 */
char	notifyText[128];
int		notifyFrameNum;
void iphoneSetNotifyText( const char *str, ... ) {
	va_list		argptr;

	va_start( argptr, str );
	(void)vsnprintf( notifyText, sizeof( notifyText )-1, str, argptr );
	va_end( argptr );

	notifyFrameNum = iphoneFrameNum;
}

void iphoneDrawNotifyText() {
	if ( notifyFrameNum == 0 ) {
		return;
	}
	// display for three seconds, then fade over 0.3
	float f = iphoneFrameNum - notifyFrameNum - 80;
	if ( f < 0 ) {
		f = 1.0;
	} else {
		f = 1.0 - f * 0.1f;
		if ( f < 0 ) {
			notifyFrameNum = 0;
			return;
		}
	}
	
	pfglColor4f( 1, 1, 1, f );
	iphoneCenterText( 240, 20, notifyText );
	pfglColor4f( 1, 1, 1, 1 );
}

void iphoneStartBonusFlash() {
	bonusFrameNum = iphoneFrameNum;
}

void iphoneStartDamageFlash( int damage ) {
	damageflash += damage;
	if ( damageflash > 64 ) {
		damageflash = 64;
	}
	if ( damageflash < 10 ) {
		damageflash = 10;
	}
}

/*
 ==================
 iphoneSetAttackDirection
 
 +1 = attacked from the left
 -1 = attacked from the right
 ==================
 */
void iphoneSetAttackDirection( int dir ) {
	if ( dir > 0 ) {
		attackDirTime[0] = iphoneFrameNum;
	}
	if ( dir < 0 ) {
		attackDirTime[1] = iphoneFrameNum;
	}
}


/*
==================
iphoneFrame

==================
*/
void iphoneFrame() {
	unsigned char blendColor[4];
	
	int	msec = 14;	// fixed time

	iphoneFrameNum++;
	
	// check for delayed intermission trigger after boss kill
	if ( intermissionTriggerFrame > 0 && iphoneFrameNum >= intermissionTriggerFrame ) {
		iphoneStartIntermission( 0 );
	}
	
	// toggle / scroll down the console
	Client_Screen_RunConsole();

	
	// fixed frame timing, assume we go 30hz
	tics = 2;		// wolf's global rate counter
	
	Sound_Update( vnull, vnull, vnull, vnull );
	
	if ( consoleActive ) {	
		iphoneSet2D();	

		Client_Screen_DrawConsole();	
		
		iphoneSavePrevTouches();
		GLimp_EndFrame();
		return;
	}
	if ( menuState != IPM_GAME ) {
		iphoneSet2D();
		
		iphoneDrawMenus();
	
		iphoneSavePrevTouches();
		GLimp_EndFrame();
		return;
	}

	//------------------
	// normal gameplay
	//------------------

	if( Player.playstate != ex_dead )
	{
		CreateIphoneUserCmd();
		Player.position.angle = NormalizeAngle( Player.position.angle );

		PL_Process( &Player, r_world );	// Player processing
		if ( !slowAI->value || --slowAIFrame < 0 ) {
			// slowAIframe is for slow-motion screenshot capture
			slowAIFrame += slowAI->value;
			ProcessGuards();
		}
		PushWall_Process();
		Door_ProcessDoors_e( &r_world->Doors, tics, msec );
		
		levelstate.time += tics;
	}

	// fill the floor and ceiling
	R_Draw_Fill( 0, 0, viddef.width, viddef.height >> 1, r_world->ceilingColour );
	R_Draw_Fill( 0, viddef.height >> 1, viddef.width, viddef.height, r_world->floorColour );

	// draw 3D world 
	R_SetGL3D( Player.position );
	R_RayCast( Player.position, r_world );
	R_DrawSprites();

	// draw 2D overlays
	iphoneSet2D();	
	
	// do a full screen blend for damage, death, and bonus pickup
	if( Player.playstate == ex_dead ) {
		static int deathFadeIntensity;
		blendColor[0] = 255;
		blendColor[1] = 0;
		blendColor[2] = 0;
		blendColor[3] = deathFadeIntensity;
		deathFadeIntensity += 2;
		if( deathFadeIntensity >= 240 ) {
			deathFadeIntensity = 0;
			PL_NewGame( &Player );
			iphoneStartMap( currentMap.episode, currentMap.map, currentMap.skill );
		}
	} else {
		iphoneDrawWeapon();
		if( damageflash ) {
			blendColor[0] = 255;
			blendColor[1] = 0;
			blendColor[2] = 0;
			blendColor[3] = damageflash >= 64 ? 255 : damageflash * 4;
			
			if( (damageflash -= 1) < 0 ) {
				damageflash = 0;
			}
		} else if ( bonusFrameNum ) {
			float	f = ( iphoneFrameNum - bonusFrameNum ) * 0.1;
			if ( f > 1.0 ) {
				bonusFrameNum = 0;
			} else {
				blendColor[0] = 255;
				blendColor[1] = 255;
				blendColor[2] = 128;
				blendColor[3] = ( 1.0 - f ) * 64;
			}
		}
	}
	if ( blendColor[3] != 0 && blends->value != 0 ) {
		R_DrawBox( 0, 0, viddef.width, viddef.height, *(int *)blendColor );
	}
	
	iphoneDrawNotifyText();
	
	iphoneDrawFace();	

	iphoneDrawControls();

	iphoneDrawHUD();
	
	// draw menu and map buttons
	if ( controlScheme->value < 2 ) {
		if ( iphoneActivatePicWhenTouched( 480-50, 0, 50, 50, "iphone/menu.tga" ) ) {
			menuState = IPM_MAIN;
		}
		if ( iphoneActivatePicWhenTouched( 0, 0, 50, 50, "iphone/map.tga" ) ) {
			iphoneOpenAutomap();
		}
	} else {
		if ( iphoneActivatePicWhenTouched( 0, 50, 50, 50, "iphone/menu.tga" ) ) {
			menuState = IPM_MAIN;
		}
		if ( iphoneActivatePicWhenTouched( 0, 0, 50, 50, "iphone/map.tga" ) ) {
			iphoneOpenAutomap();
		}
	}
	
	Client_Screen_DrawConsole();	

	ShowTilt();
	
	iphoneSavePrevTouches();
	
	// do the swapbuffers
	GLimp_EndFrame();
}
