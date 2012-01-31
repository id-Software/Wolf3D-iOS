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
 
 

//gsh
//#define NUM_TOUCHFRAME 10

#include "../wolfiphone.h"
#include "iphone_store.h"

int		intermissionStartFrameNum;
int		hasReleased;

menuState_t menuState;
colour4_t highlightColor = { 128, 128, 128, 255 };
colour4_t colorPressed = { 128, 128, 0, 255 };

//-------------
// These are for the download progress menu
extern unsigned int dataAmount; //gsh... provides us information with the amount of data we've downloaded
unsigned int totalDownloaded = 0;
extern const unsigned int DownloadFileSize;
extern unsigned int userDataAmount; //gsh... provides us information with the amount of data we've downloaded
extern const unsigned int DownloadUserFileSize;
//--------------
//gsh
int wasDLInstructionsCalledFromEpisodeMenu = 0;
/*
 =========================
 iphoneSetLevelNotifyText
 gsh
 =========================
 */
void iphoneSetLevelNotifyText()
{
	char str[128];

	if (currentMap.episode < 6)
		sprintf( str, "Entering level E%iM%i", currentMap.episode+1, currentMap.map+1 );
	else if (currentMap.episode < 10) {
		int currentLevel = currentMap.episode * 10 + currentMap.map;
		switch (currentLevel) {
			case 60: case 61: case 62: case 63: case 64:
				sprintf( str, "Entering Tunnels %i", currentLevel-60+1);
				break;
			case 78:
				sprintf( str, "Entering Tunnels Secret");//, 6);
				break;				
			case 65: case 66: case 67: case 68: case 69:
				sprintf( str, "Entering Dungeons %i", currentLevel-65+1);
				break;
			case 70: case 71: case 72: case 73: case 74: case 75:
				sprintf( str, "Entering Castle %i", currentLevel - 70 + 1);
				break;
			case 79:
				sprintf( str, "Entering Castle Secret");//, 6);
				break;		
			case 76:
				sprintf( str, "Entering Ramparts");
				break;
			case 77:
				sprintf( str, "Entering Death Knight");
				break;
			case 80:
				sprintf( str, "Entering Dungeon Dimension");
				break;
			default:
				sprintf( str, "  ");
				break;
		}
	}
	else 
		sprintf( str, "Entering level custom %i", /*currentMap.episode+1,*/ currentMap.map+1 );

	iphoneSetNotifyText( str );
}

/*
 ==================
 RectMake
 gsh
 ==================
 */
rect_t RectMake(int x, int y, int width, int height)
{
	rect_t rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	return rect;
}

/*
 ==================
 iphoneDrawPicNum
 
 ==================
 */
void iphoneDrawPicNum( int x, int y, int w, int h, int glTexNum ) {
	//ScalePositionAndSize( &x, &y, &w, &h );
	
	R_Bind( glTexNum );
	pfglBegin( GL_TRIANGLE_STRIP );

	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 0.0f, 1.0f );	pfglVertex2i( x, y+h );
	pfglTexCoord2f( 1.0f, 0.0f );	pfglVertex2i( x+w, y );
	pfglTexCoord2f( 1.0f, 1.0f );	pfglVertex2i( x+w, y+h );

	pfglEnd();
}

typedef struct drawPicTiming_tag {
	CFAbsoluteTime totalTime;
	CFAbsoluteTime bindTime;
	CFAbsoluteTime endTime;
	CFAbsoluteTime findTexTime;
	const char * pic;
} drawPicTiming;

#define NUM_PIC_TIMINGS 16

static drawPicTiming picTimings[NUM_PIC_TIMINGS];
int curPic = 0;

void picTimingPrint() {
	for ( int i = 0; i < NUM_PIC_TIMINGS; ++i) {
		if ( picTimings[i].pic != NULL ) {
			printf( "  iphoneDrawPic took %0.6f seconds. %s\n", picTimings[i].totalTime, picTimings[i].pic );
			if ( picTimings[i].totalTime > 0.01 ) {
				printf( "    R_Bind took %0.6f seconds.\n", picTimings[i].bindTime );
				printf( "    pfglEnd took %0.6f seconds.\n", picTimings[i].endTime );
				printf( "    TM_FindTexture took %0.6f seconds.\n", picTimings[i].findTexTime );
			}
		}
	}
}

void picTimingClear() {
	for ( int i = 0; i < NUM_PIC_TIMINGS; ++i) {
		picTimings[i].pic = NULL;
	}
	
	curPic = 0;
}

/*
 ==================
 iphoneDrawPicFloat
 
 ==================
 */
void iphoneDrawPicFloat( float x, float y, float w, float h, const char *pic ) {
	texture_t *gl;
	
	gl = TM_FindTexture( pic, TT_Pic );
	
	if( ! gl ) {
		Com_Printf( "Can't find pic: %s\n", pic );
		return;
	}

	R_Bind( gl->texnum );
	
	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2f( x, y );
	pfglTexCoord2f( 0.0f, gl->maxT );	pfglVertex2f( x, y+h );
	pfglTexCoord2f( gl->maxS, 0.0f );	pfglVertex2f( x+w, y );
	pfglTexCoord2f( gl->maxS, gl->maxT );	pfglVertex2f( x+w, y+h );
	
	pfglEnd();
}

#define DRAWPIC_PROFILING 0
//#define DRAWPIC_PROFILING 1


/*
 ==================
 iphoneDrawPic
 
 ==================
 */
void iphoneDrawPic( int x, int y, int w, int h, const char *pic ) {
		
	texture_t *gl;

#if DRAWPIC_PROFILING
	CFAbsoluteTime picStartTime = CFAbsoluteTimeGetCurrent();
	CFAbsoluteTime texStartTime = CFAbsoluteTimeGetCurrent();
#endif

	gl = TM_FindTexture( pic, TT_Pic );

#if DRAWPIC_PROFILING
	CFAbsoluteTime texEndTime = CFAbsoluteTimeGetCurrent();
	CFAbsoluteTime texTime = texEndTime - texStartTime;
#endif

	if( ! gl ) {
		Com_Printf( "Can't find pic: %s\n", pic );
		return;
	}

#if DRAWPIC_PROFILING
	CFAbsoluteTime bindStartTime = CFAbsoluteTimeGetCurrent();
#endif

	R_Bind( gl->texnum );
	
#if DRAWPIC_PROFILING
	CFAbsoluteTime bindEndTime = CFAbsoluteTimeGetCurrent();
	CFAbsoluteTime bindTime = bindEndTime - bindStartTime;
#endif

	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 0.0f, gl->maxT );	pfglVertex2i( x, y+h );
	pfglTexCoord2f( gl->maxS, 0.0f );	pfglVertex2i( x+w, y );
	pfglTexCoord2f( gl->maxS, gl->maxT );	pfglVertex2i( x+w, y+h );
	

#if DRAWPIC_PROFILING	
	CFAbsoluteTime endStartTime = CFAbsoluteTimeGetCurrent();
#endif

	pfglEnd();
	
#if DRAWPIC_PROFILING
	CFAbsoluteTime endEndTime = CFAbsoluteTimeGetCurrent();
	CFAbsoluteTime endTime = endEndTime - endStartTime;
	
	CFAbsoluteTime picEndTime = CFAbsoluteTimeGetCurrent();
	CFAbsoluteTime picTime = picEndTime - picStartTime;
	
	if ( curPic < NUM_PIC_TIMINGS ) {
		picTimings[curPic].totalTime = picTime;
		picTimings[curPic].bindTime = bindTime;
		picTimings[curPic].endTime = endTime;
		picTimings[curPic].findTexTime = texTime;
		picTimings[curPic].pic = pic;
		curPic++;
	}
#endif
}

/*
 ==================
 iphoneDrawPicRect
 
 ==================
 */
void iphoneDrawPicRect( rectFloat_t rect, const char *pic ) {
	iphoneDrawPic( rect.x, rect.y, rect.width, rect.height, pic );
}

//gsh
void iphoneDrawPicInBox( rect_t drawRect, const char *pic, rect_t boxRect )
{
	texture_t *gl;
	
	gl = TM_FindTexture( pic, TT_Pic );
	if( ! gl ) {
		Com_Printf( "Can't find pic: %s\n", pic );
		return;
	}

	
	int newX = drawRect.x;
	int newY = drawRect.y;
	int newW = drawRect.width;
	int newH = drawRect.height;
	
	float u0 = 0;
	float u1 = gl->maxS;
	float v0 = 0;
	float v1 = gl->maxT;
	
	
	if (drawRect.x > boxRect.x + boxRect.width)
		return; //we're not in box... //newX = boxX + boxW;
	if (drawRect.x + drawRect.width < boxRect.x)
		return;
	if (drawRect.y > boxRect.y + boxRect.height)
		return;
	if (drawRect.y + drawRect.height < boxRect.y)
		return;
	
	if (drawRect.x < boxRect.x) {
		newX = boxRect.x;   
		u0 = gl->maxS * ((float)(boxRect.x - drawRect.x))/((float)drawRect.width);
		newW = drawRect.width - (boxRect.x - drawRect.x);
	}
	if (drawRect.x + drawRect.width > boxRect.x + boxRect.width) {
		newW = boxRect.x + boxRect.width - drawRect.x;
		u1 = gl->maxS * ((float)(newW))/((float)drawRect.width);
	}
	if (drawRect.y < boxRect.y) {
		newY = boxRect.y;
		v0  = gl->maxT * ((float)(boxRect.y - drawRect.y))/((float)drawRect.height);
		newH = drawRect.height - (boxRect.y - drawRect.y);
	}
	if (drawRect.y + drawRect.height > boxRect.y + boxRect.height) {
		newH = boxRect.y + boxRect.height - drawRect.y;
		v1 = gl->maxT * ((float)(newH))/((float)drawRect.height);
	}
	
		
	R_Bind( gl->texnum );
	pfglBegin( GL_TRIANGLE_STRIP );
	
	//float u, v;
	//u = ((float)newX)/((float)x);
	//v = ((float)newY)/((float)y);
	pfglTexCoord2f( u0, v0 );	pfglVertex2i( newX, newY );
	pfglTexCoord2f( u0, v1 );	pfglVertex2i( newX, newY+newH );
	pfglTexCoord2f( u1, v0 );	pfglVertex2i( newX+newW, newY );
	pfglTexCoord2f( u1, v1 );	pfglVertex2i( newX+newW, newY+newH );
	
	pfglEnd();	
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
	iphoneDrawPic( x, y, w, h, pic );
	if ( TouchDown( x, y, w, h ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( x, y, w, h, color );
	}
	return r;
}

/*
 ==================
 iphoneDrawPicRectWithTouch
 
 ==================
 */
int iphoneDrawPicRectWithTouch( rectFloat_t rect, const char *pic ) {
	return iphoneDrawPicWithTouch( rect.x, rect.y, rect.width, rect.height, pic );
}

//gsh
int iphoneDrawPicWithTouchSpecified( rect_t drawRect, rect_t touchRect,  const char *pic) {//int x, int y, int w, int h, int touchX, int touchY, int touchW, int touchH, const char *pic ) {
	int	r = TouchReleased( touchRect.x, touchRect.y, touchRect.width, touchRect.height );
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
	iphoneDrawPic( drawRect.x, drawRect.y, drawRect.width, drawRect.height, pic );
	if ( TouchDown( touchRect.x, touchRect.y, touchRect.width, touchRect.height ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( drawRect.x, drawRect.y, drawRect.width, drawRect.height, color );
	}
	return r;
}
//gsh
int iphoneDrawPicInBoxWithTouchSpecified( rect_t drawRect, rect_t touchRect, const char *pic, rect_t boundingRect ) {
	ScaleRect( &drawRect );
	ScaleRect( &touchRect );
	ScaleRect( &boundingRect );
	
	//make sure touches are in drawing bounds
	if (touchRect.x < boundingRect.x) {
		touchRect.width -= boundingRect.x - touchRect.x;
		touchRect.x = boundingRect.x;
	}
	if (touchRect.x + touchRect.width > boundingRect.x + boundingRect.width)
		touchRect.width = boundingRect.x + boundingRect.width - touchRect.x;
	if (touchRect.y < boundingRect.y) {
		touchRect.height -= boundingRect.y - touchRect.y;
		touchRect.y = boundingRect.y;
	}
	if (touchRect.y + touchRect.height > boundingRect.y + boundingRect.height)
		touchRect.height = boundingRect.y + boundingRect.height - touchRect.y;
	
	int	r = TouchReleased( touchRect.x, touchRect.y, touchRect.width, touchRect.height );
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
//	iphoneDrawPic( x, y, w, h, pic );
//	iphoneDrawPicInBox( x, y, w, h, pic, boxX, boxY, boxW, boxH  );
	iphoneDrawPicInBox( drawRect, pic, boundingRect );
//	iphoneDrawPicInBox( int x, int y, int w, int h, const char *pic, int boxX, int boxY, int boxW, int boxH )
	if ( TouchDown( touchRect.x, touchRect.y, touchRect.width, touchRect.height ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, color );
	}
	return r;
}

//gsh
int iphoneDrawPicInBoxWithTouchAndVelocity( rectFloat_t drawRect, rectFloat_t touchRect, const char *pic, rectFloat_t boundingRect, int dragVelocity ) {
	
	//make sure touches are in drawing bounds
	if (touchRect.x < boundingRect.x) {
		touchRect.width -= boundingRect.x - touchRect.x;
		touchRect.x = boundingRect.x;
	}
	if (touchRect.x + touchRect.width > boundingRect.x + boundingRect.width)
		touchRect.width = boundingRect.x + boundingRect.width - touchRect.x;
	if (touchRect.y < boundingRect.y) {
		touchRect.height -= boundingRect.y - touchRect.y;
		touchRect.y = boundingRect.y;
	}
	if (touchRect.y + touchRect.height > boundingRect.y + boundingRect.height)
		touchRect.height = boundingRect.y + boundingRect.height - touchRect.y;
	
	int	r = TouchReleased( touchRect.x, touchRect.y, touchRect.width, touchRect.height );
	
	//if the object is moving, then it shouldn't be selectable
	if ( dragVelocity )
		r = 0;
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
	//	iphoneDrawPic( x, y, w, h, pic );
	//	iphoneDrawPicInBox( x, y, w, h, pic, boxX, boxY, boxW, boxH  );
	rect_t drawRectInt = RectMake( drawRect.x, drawRect.y, drawRect.width, drawRect.height );
	rect_t boundingRectInt = RectMake( boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height );
	iphoneDrawPicInBox( drawRectInt, pic, boundingRectInt );
	//	iphoneDrawPicInBox( int x, int y, int w, int h, const char *pic, int boxX, int boxY, int boxW, int boxH )
	if ( TouchDown( touchRect.x, touchRect.y, touchRect.width, touchRect.height ) && !dragVelocity) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, color );
	}
	return r;
}


//gsh
int iphoneDrawPicInBoxWithTouchSpecifiedAndColor( rect_t drawRect, rect_t touchRect, const char *pic, rect_t boundingRect, colour4_t color ) {
	
	//make sure touches are in drawing bounds
	if (touchRect.x < boundingRect.x) {
		touchRect.width -= boundingRect.x - touchRect.x;
		touchRect.x = boundingRect.x;
	}
	if (touchRect.x + touchRect.width > boundingRect.x + boundingRect.width)
		touchRect.width = boundingRect.x + boundingRect.width - touchRect.x;
	if (touchRect.y < boundingRect.y) {
		touchRect.height -= boundingRect.y - touchRect.y;
		touchRect.y = boundingRect.y;
	}
	if (touchRect.y + touchRect.height > boundingRect.y + boundingRect.height)
		touchRect.height = boundingRect.y + boundingRect.height - touchRect.y;
	
	int	r = TouchReleased( touchRect.x, touchRect.y, touchRect.width, touchRect.height );
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
	//	iphoneDrawPic( x, y, w, h, pic );
	//	iphoneDrawPicInBox( x, y, w, h, pic, boxX, boxY, boxW, boxH  );
	iphoneDrawPicInBox( drawRect, pic, boundingRect );
	R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, color );
	//	iphoneDrawPicInBox( int x, int y, int w, int h, const char *pic, int boxX, int boxY, int boxW, int boxH )
	if ( TouchDown( touchRect.x, touchRect.y, touchRect.width, touchRect.height ) ) {
		colour4_t colorWhite = { 255, 255, 255, 64 };
		R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, colorWhite );
	}
	return r;
}

//gsh
int iphoneDrawPicInBoxWithTouchColorAndVelocity( rectFloat_t drawRect, rectFloat_t touchRect, const char *pic, rectFloat_t boundingRect, colour4_t color, int dragVelocity ) {
	
	//make sure touches are in drawing bounds
	if (touchRect.x < boundingRect.x) {
		touchRect.width -= boundingRect.x - touchRect.x;
		touchRect.x = boundingRect.x;
	}
	if (touchRect.x + touchRect.width > boundingRect.x + boundingRect.width)
		touchRect.width = boundingRect.x + boundingRect.width - touchRect.x;
	if (touchRect.y < boundingRect.y) {
		touchRect.height -= boundingRect.y - touchRect.y;
		touchRect.y = boundingRect.y;
	}
	if (touchRect.y + touchRect.height > boundingRect.y + boundingRect.height)
		touchRect.height = boundingRect.y + boundingRect.height - touchRect.y;
	
	int	r = TouchReleased( touchRect.x, touchRect.y, touchRect.width, touchRect.height );
	
	if (dragVelocity)
		r = 0;
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		pfglColor3f( 1, 1, 1 );
	}
	//	iphoneDrawPic( x, y, w, h, pic );
	//	iphoneDrawPicInBox( x, y, w, h, pic, boxX, boxY, boxW, boxH  );
	rect_t drawRectInt = RectMake( drawRect.x, drawRect.y, drawRect.width, drawRect.height );
	rect_t boundingRectInt = RectMake( boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height );
	
	iphoneDrawPicInBox( drawRectInt, pic, boundingRectInt );
	R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, color );
	//	iphoneDrawPicInBox( int x, int y, int w, int h, const char *pic, int boxX, int boxY, int boxW, int boxH )
	if ( TouchDown( touchRect.x, touchRect.y, touchRect.width, touchRect.height ) && !dragVelocity ) {
		colour4_t colorWhite = { 255, 255, 255, 64 };
		R_Draw_Blend( touchRect.x, touchRect.y, touchRect.width, touchRect.height, colorWhite );
	}
	return r;
}




/*
 ==================
 iphoneDrawPicWithTouchAndColor
 gsh
 ==================
 */
int iphoneDrawPicWithTouchAndColor( int x, int y, int w, int h, const char *pic, colour4_t color ) {
	int	r = TouchReleased( x, y, w, h );
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
//		pfglColor3f( 1, 1, 1 );
	}
//	glColor4f( 0.5f, 0.5f, 1, 0 );
	iphoneDrawPic( x, y, w, h, pic );
	R_Draw_Blend( x, y, w, h, color );
	if ( TouchDown( x, y, w, h ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( x, y, w, h, color );
	}
//	glColor4f( 1, 1, 1, 1 );
	return r;
}

/*
 ==================
 iphoneDrawPicInBoxWithTouchAndColor
 gsh
 ==================
 */
int iphoneDrawPicInBoxWithTouchAndColor( rect_t drawRect, const char *pic, colour4_t color, rect_t boxRect ) {
	int	r = TouchReleased(drawRect.x, drawRect.y, drawRect.width, drawRect.height) 
			& TouchReleased(boxRect.x, boxRect.y, boxRect.width, boxRect.height);  //touch must be inside both boxes!
	
	if ( r ) {
		// make sure it is full intensity if it is touch-released, even if
		// it wasn't active previously
		//		pfglColor3f( 1, 1, 1 );
	}
	
	iphoneDrawPicInBox( drawRect, pic, boxRect  );
	
	if (drawRect.x + drawRect.width < boxRect.x)
		return 0;
	if (drawRect.x > boxRect.x + boxRect.width)
		return 0;
	if (drawRect.y + drawRect.height < boxRect.y)
		return 0;
	if (drawRect.y > boxRect.y + boxRect.height)
		return 0;
	
	if (drawRect.x < boxRect.x) { drawRect.x = boxRect.x; }
	if (drawRect.x + drawRect.width  > boxRect.x + boxRect.width) { drawRect.width  = boxRect.x + boxRect.width - drawRect.x; }
	if (drawRect.y < boxRect.y) { drawRect.y = boxRect.y; }
	if (drawRect.y + drawRect.height > boxRect.y + boxRect.height) { drawRect.height = boxRect.y + boxRect.height - drawRect.y; }
	
	R_Draw_Blend( drawRect.x, drawRect.y, drawRect.width, drawRect.height, color );
	if ( TouchDown( drawRect.x, drawRect.y, drawRect.width, drawRect.height ) ) {
		colour4_t color = { 255, 255, 255, 64 };
		R_Draw_Blend( drawRect.x, drawRect.y, drawRect.width, drawRect.height, color );
	}
	//	glColor4f( 1, 1, 1, 1 );
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
	
	if ( f < 0 ) {
		f = 0;
	}
	if ( f > 1 ) {
		f = 1;
	}
	
	// draw the background
	iphoneDrawPic( x, y, w, h, "iphone/slider_bar_underlay.tga");//stat_bar_2.tga" );
	
	// draw the current range
	texture_t *gl;
	
	gl = TM_FindTexture( "iphone/slider_bar.tga", TT_Pic );//stat_bar_1.tga", TT_Pic );
	assert( gl );
	R_Bind( gl->texnum );
	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0.0f, 0.0f );	pfglVertex2i( x, y );
	pfglTexCoord2f( 0.0f, 1.0f );	pfglVertex2i( x, y+h );
	pfglTexCoord2f( f, 0.0f );	pfglVertex2i( x+w*f, y );
	pfglTexCoord2f( f, 1.0f );	pfglVertex2i( x+w*f, y+h );
	
	
	pfglEnd();
	
	//gsh... draw the finger catch
	iphoneDrawPic( x+w*f-12, y, 24, h, "iphone/slider_knob.tga" );//finger_catch.tga" );
	
	
	// draw the title and fraction
	sprintf( str, "%s : %i%%", title, (int)(f*100) );
	//colour4_t color = {0, 0, 0, 1};
	pfglColor4f(0, 0, 0, 1);   //gsh
//	y -= 3;
	iphoneDrawArialText(x + w/6 + 1, y+h*8/10 + 1, 0.9f, str);
	iphoneDrawArialText(x + w/6 + 2, y+h*8/10 + 2, 0.9f, str);
	iphoneDrawArialText(x + w/6 + 3, y+h*8/10 + 3, 0.9f, str);
	//iphoneCenterTextWithColor(x+ w/2 +1, y+h/2-8 +1, str, color);
	//iphoneCenterTextWithColor(x+ w/2 +2, y+h/2-8 +2, str, color);
	//iphoneCenterTextWithColor(x+ w/2 +3, y+h/2-8 +3, str, color);
	//iphoneCenterText( x+ w/2, y+h/2-8, str );
	pfglColor4f(1, 1, 1, 1);
	iphoneDrawArialText(x + w/6, y+h*8/10, 0.9f, str);
	
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
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 0.0f, 0.0f, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT ), "iphone/button_back.tga" ) ) {
		return 1;
	}
	return 0;
}	

//gsh... a back button on the other side of the screen
int BackButton2() {	
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 480.0f-64.0f, 0.0f, 64.0f, 36.0f ), "iphone/button_back.tga" ) ) {
		return 1;
	}
	return 0;
}	

//gsh
int BackButton3( int x, int y ) {	
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( x, y, 64.0f, 36.0f ), "iphone/button_back.tga" ) ) {
		return 1;
	}
	return 0;
}

/*
 ==================
 MenuButton
 gsh
 ==================
 */
int MenuButton() {	
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 480.0f-64.0f, 0.0f, 64.0f, 64.0f ), "iphone/menu.tga" ) ) {
//	if ( iphoneDrawPicWithTouch( 64, 0, 64, 36, "iphone/menu.tga" ) ) {
//	if ( iphoneDrawPicWithTouch( 64, 0, 64, 36, "iphone/button_menu_yellow.tga" ) ) {
		iphoneStartMainMenu();
		return 1;
	}
	return 0;
}	


void GetMoreLevels( int x, int y ) {
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( x, y, 128.0f, 64.0f ), "iphone/button_levels.tga" ) ) {
		// directly to the app store for more levels
		SysIPhoneOpenURL( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=309470478" );
	}
}

//gsh
#if 0
extern void DownloadSOD();
extern void BeginStoreKit();
extern void DisplayWrongVersionedOS();
extern int TestURLConnection();//const char *url);
extern bool isStorekitAvailable;
//extern int IsSpearInstalled();
//extern void iphoneActivateConsole();
#endif

//=================================
// GetSpear
//gsh
//===================================
#if 0
void GetSpear(void)
{
	if (isStorekitAvailable)
	{
		if (TestURLConnection())  //check URL connection
		{
			/*  //it's been decided to pre-package spear of destiny with wolf3d
			    //however users will need to in-app purchase it to unlock
			//only download the data if doesn't exist, otherwise just enter the storekit
			if (!IsSpearInstalled())
				DownloadSOD();
			else*/
				BeginStoreKit();
		}
	}
	else
		DisplayWrongVersionedOS();
}
#endif
//=================================
// GetSpearOfDestiny
//gsh
//===================================
#if 0
void GetSpearOfDestiny( int x, int y )
{
	if ( iphoneDrawPicWithTouch( x, y, 128, 64, "iphone/button_levels.tga" ) )
	{
		totalDownloaded = 0; //reset the totalDownload to zero
		
		Com_Printf("GetSpearOfDestiny called\n");
		
		GetSpear();
	}	
}
#endif
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
	// Can't be destructive anymore, because the app can be suspended in the background now.
	
	// Quick fix for non-destructiveness: Just make a copy of the levelData, convert the
	// pointers to indices in the copy, and write the copy to the save data. Leave the actual
	// current levelData alone.
	LevelData_t copiedLevelData = levelData;
	
	for ( i = 0 ; i < copiedLevelData.Doors.doornum ; i++ ) {
		int	index = r_world->Doors.Doors[i] - &r_world->Doors.DoorMap[0][0];
		assert( index >= 0 && index < 4096 );
		copiedLevelData.Doors.Doors[i] = (void *)index;
	}
	
	// this is only used for the mutant death face, so just
	// clear it instead of fixing it up
	Player.LastAttacker = NULL;
	
	currentMap.version = SAVEGAME_VERSION;
	fwrite( &currentMap, 1,sizeof(currentMap), f );
	
	fwrite( &huds, 1,sizeof(huds), f);
		
	fwrite( &copiedLevelData, 1,sizeof(levelData), f );
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
	
	
	Com_Printf("episode: %i\nmap: %i\n", currentMap.episode, currentMap.map);
	
	// load the huds
	fread( &huds, 1,sizeof(huds), f);
	
	// do a normal map start
	Cvar_SetValue( skill->name, currentMap.skill );
	PL_NewGame( &Player );
	
	oldCompleted = currentMap.levelCompleted;
	if (currentMap.episode < 9)                  //gsh... added the episode check
		iphoneStartMap( currentMap.episode, currentMap.map, currentMap.skill );
	else
		iphoneStartUserMap(currentMap.episode, currentMap.map, currentMap.skill, NULL);
	
	currentMap.levelCompleted = oldCompleted;
	
	// load modifiactions on top
	fread( &levelData, 1,sizeof(levelData), f);
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
 iphoneGetUserMapName
 
 This matches the incoming mapNumber with the correct
 map name in the maps directory.  Returns 1 if successful
 and 0 if failed
 gsh
 ==================
 */
int iphoneGetUserMapName(int mapNumber, char *mapName)
{	
	int numMaps = 0;
	int returnValue = 0;
	
	DIR *dp;
	struct dirent *ep;
	char mapBuffer[1024];
	
	int length = strlen(iphoneDocDirectory);
	strcpy(mapBuffer, iphoneDocDirectory);
	strcpy(mapBuffer + length, "/usermaps/");
	
	dp = opendir (mapBuffer);
	
	if (dp != NULL)
	{
		while ( ( ep = readdir (dp) ) )
		{/*
			//if you find a .DS_Store file... ignore it!
			if ( strcmp(ep->d_name, ".DS_Store") == 0 )
				continue;
			
			if ( strcmp(ep->d_name, ".") == 0 )
				continue;
			
			if ( strcmp(ep->d_name, "..") == 0 )
				continue;
			*/
			++numMaps;
			if (numMaps < 3)  //skip the '.' and '..'
				continue;
			
			if (mapNumber == (numMaps - 3))
			{
				strcpy(mapName, ep->d_name);
				returnValue = 1;
				break;
			}
			
		}
		//(void) closedir (dp);
		closedir (dp);
	}
	
	Com_Printf("numMaps: %i\n", numMaps);
	
	return returnValue;
}

/*
 ==================
 iphoneGetUserMapName
 
 This matches the incoming mapNumber with the correct
 map name in the maps directory.  Returns 1 if successful
 and 0 if failed
 gsh
 ==================
 */
int iphoneGetUserMapLevelByName(const char *mapName)
{	
	int numMaps = 0;
	int returnValue = 0;
	
	DIR *dp;
	struct dirent *ep;
	char mapBuffer[1024];
	
	int length = strlen(iphoneDocDirectory);
	strcpy(mapBuffer, iphoneDocDirectory);
	strcpy(mapBuffer + length, "/usermaps/");
	
	dp = opendir (mapBuffer);
	
	if (dp != NULL)
	{
		while ( ( ep = readdir (dp) ) )
		{
			if (strcmp(ep->d_name, mapName) == 0)
			{
				return numMaps-3+1;
			}
			
			/*
		 //if you find a .DS_Store file... ignore it!
		 if ( strcmp(ep->d_name, ".DS_Store") == 0 )
		 continue;
		 
		 if ( strcmp(ep->d_name, ".") == 0 )
		 continue;
		 
		 if ( strcmp(ep->d_name, "..") == 0 )
		 continue;
		 */
			++numMaps;
			/*
			if (numMaps < 3)  //skip the '.' and '..'
				continue;
			
			if (mapNumber == (numMaps - 3))
			{
				strcpy(mapName, ep->d_name);
				returnValue = 1;
				break;
			}*/
			
		}
		//(void) closedir (dp);
		closedir (dp);
	}
	
	Com_Printf("numMaps: %i\n", numMaps);
	
	return returnValue;
}


/*
 ==================
 iphoneStartUserMap
 gsh
 This does not reset the player, so call PL_NewGame( &Player ) if it is a new start.
 ==================
 */
void iphoneStartUserMap( int episodeNum, int mapNum, int skillLevel, char *mapName ) {
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
	
	Com_Printf("episode: %i\nmap: %i\n", currentMap.episode, currentMap.map);
	
	if (episodeNum >= 10 && mapName)
	{
		sprintf( command, "usermaps/%s", mapName );
		Com_Printf("command: %s\n", command);
	}
	else if (episodeNum >= 10)  //mapName was null... we must find what it's called
	{
		Com_Printf("episode number check true\n");
		mapName = (char *)malloc(sizeof(char)*1024);  //sizeof(char) should just be 1 byte
		if (iphoneGetUserMapName(mapNum, mapName))
		{
			sprintf(command, "usermaps/%s", mapName);
		}
		else
		{
			Com_Printf("iphoneGetUserMapName failed");
			currentMap.episode = 0;
			currentMap.map = 0;
			sprintf( command, "w%i%i", currentMap.episode, currentMap.map );  //load first map if mapName not found
		}
		free(mapName);
	}
	else if (episodeNum >= 6)
	{
		FS_InitFilesystem();  //is this really needed?
		sprintf( command, "maps/s%i%i", currentMap.episode - 6, currentMap.map);
	}
	else
	{
		sprintf( command, "w%i%i", currentMap.episode, currentMap.map );
	}
	Com_Printf("command: %s\n", command);
	Client_PrepRefresh( command );	
	
	if ( r_world != NULL ) {
		menuState = IPM_GAME;
	} else {
		menuState = IPM_MAIN;
	}
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
	
	//added by gsh 6/9/2009... load the SOD maps if they were chosen
	if (episodeNum >= 6)
	{
	//	FS_InitFilesystem(); 
		sprintf( command, "maps/s%i%i", currentMap.episode - 6, currentMap.map);
	}
	else
	{
		sprintf( command, "w%i%i", currentMap.episode, currentMap.map );
	}
	Com_Printf("command: %s\n", command);
	Client_PrepRefresh( command );	
	
	if ( r_world != NULL ) {
		menuState = IPM_GAME;
	} else {
		menuState = IPM_MAIN;
	}
}

extern int notifyFrameNum; //gsh

/*
 ==================
 iphoneResume
 
 ==================
 */
void iphoneResume() {
	iphoneStopMenuMusic();
	
	//iphoneStartGameplay();
	iphonePreloadBeforePlay();
	
	Sound_StartBGTrack( levelData.musicName, levelData.musicName );
	
	// if the game was saved at the intermission screen, immediately
	// bring it back up when it is loaded
	if ( currentMap.levelCompleted ) {
		iphoneStartIntermission( 0 );
	} else {
		//reset the notification so that we can see which level we've loaded.. gsh
		iphoneSetLevelNotifyText();
		iphoneFrameNum = 0;
		notifyFrameNum = 60;
		if ( r_world != NULL ) {
			menuState = IPM_GAME;
		} else {
			menuState = IPM_MAIN;
		}
	}
}

/*
 ==================
 iphoneMainMenu
 
 ==================
 */
void iphoneMainMenu() {	
	char	str[80];
	float	scale = 40 / 32.0;
	
	//iphoneDrawPic( 480-256, 0, 256, 128, "iphone/wolf_logo.tga" );
	//iphoneDrawPic( 0, 0, 480, 320, "iphone/background_main.tga" ); //gsh
	
	//iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 264.0f, 250.0f ), "iphone/wolfplatinum_logo.tga");
	
	/*gsh
#ifdef LITE
	iphoneDrawPic( -20, 0, 256, 64, "iphone/ep_1.tga" );
	GetMoreLevels( 0, 96 );
#else
	iphoneDrawPic( -20, 0, 256, 64, "iphone/ep_1_6.tga" );
	if (g_version->value != SPEAROFDESTINY)  //gsh
		GetSpearOfDestiny( 0, 96 );
	else
		iphoneDrawPic( 10, 100, 200, 100, "iphone/spear_logo.tga" ); //gsh
#endif*/

	// we don't need the logo here now that we have a splash screen
	// iphoneDrawPic( 0, 320 - 128, 128, 128, "iphone/id_logo.tga" );
	
//	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 80, 128*scale, 64*scale, "iphone/button_resume.tga" ) ) {
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 360 - 64*scale, 20, 128*scale, 32*scale ),
									 "iphone/button_resume.tga" ) ) { //gsh
		iphoneResume();
	}
	
	//gsh
	if (currentMap.episode < 9)
		sprintf( str, "E%iM%i", currentMap.episode+1, currentMap.map+1 );
	else 
		sprintf( str, "custom %i", currentMap.map+1 );
	
//	iphoneCenterText( 300, 80+34*scale, str );
//	iphoneCenterText( 360, 34*scale, str ); //gsh
	
//	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 170, 128*scale, 32*scale, "iphone/button_control.tga" ) ) {
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 360 - 64*scale, 120, 128*scale, 32*scale ), "iphone/button_settings.tga" ) ) { //gsh
		menuState = IPM_CONTROLS;
	}
//	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 220, 128*scale, 32*scale, "iphone/button_new.tga" ) ) {
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 360 - 64*scale, 70, 128*scale, 32*scale ), "iphone/button_new.tga" ) ) { //gsh
		menuState = IPM_SKILL;//IPM_MAPSELECTOR;//IPM_SKILL; //gsh
	}
//	if ( iphoneDrawPicWithTouch( 300 - 64*scale, 270, 128*scale, 32*scale, "iphone/button_web.tga" ) ) {
	//if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 100 - 64*scale, 250, 64, 64 ), "iphone/id_logo.tga" ) ) { //gsh
	//	currentYesNoBox = YESNO_GO_TO_WEBSITE;
	//	iphoneYesNoBox("Website", "This will navigate you to idsoftware.com.  Continue?");
//		SysIPhoneOpenURL( "http://www.idsoftware.com/wolfenstein3dclassic/" );
	//}
	//gsh
//	if (iphoneDrawPicWithTouch(100 - 64*scale, 270, 128*scale, 32*scale, "iphone/button_control.tga" ) ) {
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 360 - 64*scale, 220, 128*scale, 32*scale ), "iphone/button_more.tga" ) ) { //gsh
		menuState = IPM_MORE;//IPM_TRIVIA;
	}

	int x = 360;
	int y = 170;
	if ( SysIPhoneOtherAudioIsPlaying() ) {
//		iphoneDrawPic( 480 - 64, 220, 64, 64, "iphone/music_off.tga" );
		iphoneDrawPicRect( MakeScaledRectFloat( x - 64*scale, y, 128*scale, 32*scale ), "iphone/music_off.tga" ); //gsh
	} else {
		if ( music->value ) {
			//if ( iphoneDrawPicWithTouch( 480 - 64, 220, 64, 64, "iphone/music_on.tga" ) ) {
			if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( x - 64*scale, y, 128*scale, 32*scale ), "iphone/music_on.tga" ) ) { //gsh
				Cvar_SetValue( music->name, 0 );
				Sound_StopBGTrack();
			}
		} else {
			//if ( iphoneDrawPicWithTouch( 480 - 64, 220, 64, 64, "iphone/music_off.tga" ) ) {
			if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( x - 64*scale, y, 128*scale, 32*scale ), "iphone/music_off.tga" ) ) { //gsh
				Cvar_SetValue( music->name, 1 );
				Sound_StartBGTrack( levelData.musicName, levelData.musicName );
			}
		}
	}
	
	sprintf( str, "v%3.1f", WOLF_IPHONE_VERSION );
	float versionX = 460.0f;
	float versionY = 300.0f;
	ScalePosition( &versionX, &versionY );
	iphoneCenterText( versionX, versionY, str );	
}

//==========================================
//  iphoneUpdateControlDrags()
//  Updates the touches for the scrolling maps
//==========================================
void iphoneUpdateControlDrags(int numPresets, int x, int y, int w, int h, int spacing, int *dragPosition, int *dragVelocity)
{
	//Update drag/touch info
//	int x = 0;
//	int w = 480;// - skillSides - 64;
	/*
	if (TouchDown(x, y, w, h))
	{
		if (!numPrevTouches)
			prevTouches[0][0] = touches[0][0];
		else if (numTouches == 1)
			*dragVelocity = touches[0][0] - prevTouches[0][0];
	}*/
	
	//update the drags
	*dragPosition += *dragVelocity;
	/*
	//int rightBoundary = 240 - 2*width;//480/2 - (numPresets*width/2 + spacing);
	if (*dragPosition < -270-10)//(numPresets*(width+spacing))/2 )
		*dragPosition = -270-10;//(numPresets*(width+spacing))/2;
	else if (*dragPosition > 70+10)//rightBoundary)
		*dragPosition = 70+10;//rightBoundary;
	
	if (*dragPosition < -270)
		*dragVelocity = 2;
	else if (*dragPosition > 70)
		*dragVelocity = -2;
	*/
	
	//retard the velocity
	if (*dragVelocity > 0)
		--*dragVelocity;
	else if (*dragVelocity < 0)
		++*dragVelocity;
	
}
#if 0
/*
 ==================
 iphoneControlMenu
 
 ==================
 */
void iphoneControlMenu() {
	int		i;
	
	int x = 0;
	int y = 0;
	
	iphoneDrawPic(0, 0, 480, 320, "iphone/submenus_background_image.tga");
	
	if ( BackButton() ) {
		menuState = IPM_MAIN;
	}
	
	

	x = 480-128*9/10;	
	if ( iphoneDrawPicWithTouch( x, y, 128*9/10, 36/*32*9/10*/, "iphone/advanced_button.tga" ) ) {
		Cvar_SetValue( controlScheme->name, -1 );
		iphonePreloadBeforePlay();	// make sure all the hud textures are loaded	
		menuState = IPM_HUDEDIT;
	}
	
	x = 240-108*9/10;
	iphoneDrawPic(x, y, 217*9/10, 50*9/10, "iphone/header_settings.tga");
	
	//update the drags
	int startPosition = 70;
	static int dragPosition = 70;
	static int dragVelocity = 0;
	
	y = 20;
	x = 0;
	
	if (iphoneDrawPicWithTouch(9, y+60, 64, 64, "iphone/button_left.tga") )
		dragVelocity = -20;//-18;
	if (iphoneDrawPicWithTouch(411, y+60, 64, 64, "iphone/button_right.tga") )
		dragVelocity = 20;//18;
	
	int size = 160;//180;//110;
	//iphoneUpdateControlDrags(4, 70, y+25, size*2, size - 53, 10, &dragPosition, &dragVelocity );
	
	//update the drags
	dragPosition += dragVelocity;

	//retard the drags
	if (dragVelocity > 0)
		--dragVelocity;
	else if (dragVelocity < 0)
		++dragVelocity;
	
	//stop dragging when we're slow and close to a node
	if ((dragVelocity < 10 && dragVelocity > 0) || (dragVelocity > -10 && dragVelocity < 0))
	{
		if (abs((dragPosition-startPosition)%(size+10)) < 7)
		{
			dragVelocity = 0;
			int tx = dragPosition - startPosition;
			int n = tx/(size+10);//480;
			dragPosition = n*(size+10) + startPosition;
			
		}
	}
	if (dragVelocity == 0)
	{
		int spacing = 10;
		int height = size;
		int n = (dragPosition - startPosition) / (height+spacing);
		
		int nodePosition = n*(height+spacing) + startPosition;
		if (nodePosition != dragPosition)
		{
			int nodePositionPlus = nodePosition + (height+spacing);
			int nodePositionMinus = nodePosition - (height+spacing);
			int distMinus = abs(nodePositionMinus - dragPosition);
			int distPlus = abs(nodePositionPlus - dragPosition);
			int dist = abs(nodePosition - dragPosition);
			
			
			int diff = nodePosition - dragPosition;
			
			if (distMinus < distPlus && distMinus < dist)
				diff = nodePositionMinus - dragPosition;
			else if (distPlus < dist && distPlus < distMinus)
				diff = nodePositionPlus - dragPosition;
			
			if (diff < 0) {
				dragVelocity -= 5;
			}
			else {
				dragVelocity += 5;
			}
		}
	}
	
	
	//draw the preset layouts
	for ( i = 0 ; i < 4 ; i++ ) {
		char	str[128];
		int remap[4] = { 3,4,1,2};	// artist named them differently than intended...
		sprintf( str, "iphone/iphone_preset_%i.tga", remap[i] );
		if ( i != controlScheme->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		int thisX = 5 + (size+10) * i + dragPosition;
		
		//wrap presets around in a circle
		if (thisX > 70 + 480 -140)
			thisX -= (size+10) * 4;
		
		if (thisX < 70 - 480 + 140)
			thisX += (size+10) * 4;
		
		if (dragPosition > 70 + (size+10) * 4 )
			dragPosition = 70;
		
		if (dragPosition < 70 - (size+10) * 4 )
			dragPosition = 70;
		
		if ( iphoneDrawPicInBoxWithTouchSpecified( RectMake(thisX, y+50, size, size/2),
												   RectMake(thisX, y + 25, size, size - 53),
												   str,
												   RectMake(70, 0, 480-140, 320) )
												   && !dragVelocity) 
		{
			Cvar_SetValue( controlScheme->name, i );
			HudSetForScheme( i );
		}
		pfglColor3f( 1, 1, 1 );
		
		char buffer[32];
		sprintf(buffer, "Preset %i", i+1);

		pfglColor3f( 0, 0, 0 );
		iphoneDrawTextInBox( RectMake(thisX+35+1, y+125+1, 16, 16), 480, buffer, RectMake(70, 0, 480-140+10, 320) );
		iphoneDrawTextInBox( RectMake(thisX+35+2, y+125+2, 16, 16), 480, buffer, RectMake(70, 0, 480-140+10, 320) );
		iphoneDrawTextInBox( RectMake(thisX+35+3, y+125+3, 16, 16), 480, buffer, RectMake(70, 0, 480-140+10, 320) );
		pfglColor3f( 1, 1, 1 );
		iphoneDrawTextInBox( RectMake(thisX+35, y+125, 16, 16), 480, buffer, RectMake(70, 0, 480-140+10, 320) );
	}

	
	int sliderHeight = 25;//35;
	int sliderSpacing = 10;
	int sliderY = y + size;// - 13;//190;//170;
	int sliderX = 80;//20;
	int sliderWidth = 320;//440;
	
	//sensitivity slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "sensitivity", sensitivity, 0, 1 );
	sliderY += sliderHeight + sliderSpacing;
	
	//tilt move speed slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "tilt move speed", tiltMove, 5000, 30000 );
	sliderY += sliderHeight + sliderSpacing;
	if ( tiltMove->value == 5000 ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	if ( tiltMove->value ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	
	//tilt turn speed slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "tilt turn speed", tiltTurn, 500, 3000 );
	sliderY += sliderHeight + sliderSpacing;
	if ( tiltTurn->value == 500 ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	if ( tiltTurn->value ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	
	//hud alpha slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "hud alpha", hudAlpha, 0, 1 );
	sliderY += sliderHeight + sliderSpacing;


#ifdef VOLUMEHACK
	//Draw volume up/down settings... gsh
	
	y = 0;
	x = 480-74;
	if (revLand->value)
	{
		y = 320-32;
		x = 74-64;
	}
	
	if ( iphoneDrawPicWithTouch( x, y, 64, 32, "iphone/button_pistol.tga" ) ) {
		if ((int)volumeFireUpSetting->value)
			Cvar_Set("volumeFireUpSetting", "0");
		else 
			Cvar_Set("volumeFireUpSetting", "1");
	}
	
	if ((int)volumeFireUpSetting->value)
		iphoneDrawPic(x, y, 64, 32, "iphone/button_pistol.tga");
	else
		iphoneDrawPic(x, y, 64, 32, "iphone/button_knife.tga");
	
	x = 480-145;
	if (revLand->value)
		x = 145-64;
	
	if ( iphoneDrawPicWithTouch( x, y, 64, 32, "iphone/button_pistol.tga" ) ) {
		if ((int)volumeFireDownSetting->value)
			Cvar_Set("volumeFireDownSetting", "0");
		else 
			Cvar_Set("volumeFireDownSetting", "1");
	}

	if ((int)volumeFireDownSetting->value)
		iphoneDrawPic(x, y, 64, 32, "iphone/button_pistol.tga");
	else
		iphoneDrawPic(x, y, 64, 32, "iphone/button_knife.tga");
#endif
}
#endif

#if 0
/*
 ==================
 iphoneControlMenu
 gsh... different from above
 ==================
 */
void iphoneControlMenu() {
	int		i;
	
	int x = 0;
	int y = 0;
	
	iphoneDrawPicRect( MakeScaledRectFloat( x, y, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	
	if ( BackButton() ) {
		menuState = IPM_MAIN;
		iphoneStartMainMenu();
	}
	
	x = 480-128*9/10;	
	
	/* Advanced controls are now accessed by the new menu system
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( x, y, 128.0f*9/10.0f, 36.0f ), "iphone/advanced_button.tga" ) ) {
		Cvar_SetValue( controlScheme->name, -1 );
		iphonePreloadBeforePlay();	// make sure all the hud textures are loaded	
		menuState = IPM_HUDEDIT;
	}
	*/
	
	x = 240-108*9/10;
	iphoneDrawPicRect( MakeScaledRectFloat( x, y, 217.0f*9/10.0f, 50*9/10.0f ), "iphone/header_settings.tga");
	
	//update the position of the control settings
	const int startPosition = 70;	
	static int currentPreset = 0;
	int numPresets = 4;
	
	//update the drags
	static int Position = 70;
	
	y = 20;
	x = 0;
	
	static int moveRight = 0;
	
	//move right or left, depending on button pushed
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 9.0f, y+60.0f, 64.0f, 64.0f ), "iphone/button_left.tga") ) {
		moveRight = 0;
		++currentPreset;
		if (currentPreset > numPresets-1) {
			currentPreset = 0;//1;
			
		}
	}
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 411.0f, y+60.0f, 64.0f, 64.0f ), "iphone/button_right.tga") ) {
		moveRight = 1;
		--currentPreset;
		if (currentPreset < 0) {
			currentPreset = numPresets-1;//2;
		}
	}
	
	
	int width = 160;//305;
	int spacing = 10;//215;//175;
	
	int size = width;// + spacing;
	
	int destinationPosition = startPosition - currentPreset*(width+spacing);
	
	//if we are not where we are supposed to be... get us there
	if (Position != destinationPosition)
	{
		int oldPosition = Position;
		int deltaPos = destinationPosition - Position;
		
		//move entities
		int moveDistance = 8;
		if ( moveRight )
		{			
			Position += moveDistance;
			if (destinationPosition < Position && destinationPosition > oldPosition)
				Position = destinationPosition;
		}
		else //move left
		{
			Position -= moveDistance;
			if (destinationPosition > Position && destinationPosition < oldPosition)
				Position = destinationPosition;
		}
		
		//check if the current position has crossed over the destination position
		if ( abs(deltaPos) <= 16)
			Position = destinationPosition;
	}

	
	//draw the preset layouts
	for ( i = 0 ; i < 4 ; i++ ) {
		char	str[128];
		int remap[4] = { 3,4,1,2};	// artist named them differently than intended...
		sprintf( str, "iphone/iphone_preset_%i.tga", remap[i] );
		if ( i != controlScheme->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		int thisX = 5 + (size+10) * i + Position;
		
		//wrap presets around in a circle
		if (thisX > 70 + 480 -140)
			thisX -= (size+10) * 4;
		
		if (thisX < 70 - 480 + 140)
			thisX += (size+10) * 4;
		
		if (Position > 70)//70 + (size+10) * 4 )
			Position = 70 - (size+10)*4;
		
		if (Position < 70 - (size+10) * 4 )
			Position = 70;
		
		if ( iphoneDrawPicInBoxWithTouchSpecified( RectMake(thisX, y+50, size, size/2),
												  RectMake(thisX, y + 25, size, size - 53),
												  str,
												  RectMake(70, 0, 480-140, 320) ) )
		{
			Cvar_SetValue( controlScheme->name, i );
			HudSetForScheme( i );
		}
		pfglColor3f( 1, 1, 1 );
		
		char buffer[32];
		sprintf(buffer, "Preset %i", i+1);
		
		rect_t textBoxRect = RectMake( 70, 0, 480-140+10, 320 );
		ScaleRect( &textBoxRect );
		
		rect_t paragraphBox1 = RectMake(thisX+35+1, y+125+1, 16, 16);
		ScaleRect( &paragraphBox1 );
		
		rect_t paragraphBox2 = RectMake(thisX+35+2, y+125+2, 16, 16);
		ScaleRect( &paragraphBox2 );
		
		rect_t paragraphBox3 = RectMake(thisX+35+3, y+125+3, 16, 16);
		ScaleRect( &paragraphBox3 );
		
		rect_t paragraphBox4 = RectMake(thisX+35, y+125, 16, 16);
		ScaleRect( &paragraphBox4 );
		
		
		pfglColor3f( 0, 0, 0 );
		
		iphoneDrawTextInBox( paragraphBox1, 480*screenScale, buffer, textBoxRect );
		iphoneDrawTextInBox( paragraphBox2, 480*screenScale, buffer, textBoxRect );
		iphoneDrawTextInBox( paragraphBox3, 480*screenScale, buffer, textBoxRect );
		pfglColor3f( 1, 1, 1 );
		iphoneDrawTextInBox( paragraphBox4, 480*screenScale, buffer, textBoxRect );
	}
	
	
	float sliderHeight = 25;//35;
	int sliderSpacing = 10;
	float sliderY = y + size;// - 13;//190;//170;
	float sliderX = 80;//20;
	float sliderWidth = 320;//440;
	
	//sensitivity slider
	ScalePositionAndSize( &sliderX, &sliderY, &sliderWidth, &sliderHeight );
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "sensitivity", sensitivity, 0, 1 );
	sliderY += sliderHeight + sliderSpacing;
	
	//tilt move speed slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "tilt move speed", tiltMove, 5000, 30000 );
	sliderY += sliderHeight + sliderSpacing;
	if ( tiltMove->value == 5000 ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	if ( tiltMove->value ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	
	//tilt turn speed slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "tilt turn speed", tiltTurn, 500, 3000 );
	sliderY += sliderHeight + sliderSpacing;
	if ( tiltTurn->value == 500 ) {
		Cvar_SetValue( tiltTurn->name, 0 );
	}
	if ( tiltTurn->value ) {
		Cvar_SetValue( tiltMove->name, 0 );
	}
	
	//hud alpha slider
	iphoneSlider( sliderX, sliderY, sliderWidth, sliderHeight, "hud alpha", hudAlpha, 0, 1 );
	sliderY += sliderHeight + sliderSpacing;
	
#ifdef VOLUMEHACK
	//Draw volume up/down settings... gsh
	
	y = 0;
	x = 480-74;
	if (revLand->value)
	{
		y = 320-32;
		x = 74-64;
	}
	
	if ( iphoneDrawPicWithTouch( x, y, 64, 32, "iphone/button_pistol.tga" ) ) {
		if ((int)volumeFireUpSetting->value)
			Cvar_Set("volumeFireUpSetting", "0");
		else 
			Cvar_Set("volumeFireUpSetting", "1");
	}
	
	if ((int)volumeFireUpSetting->value)
		iphoneDrawPic(x, y, 64, 32, "iphone/button_pistol.tga");
	else
		iphoneDrawPic(x, y, 64, 32, "iphone/button_knife.tga");
		
	
	x = 480-145;
	if (revLand->value)
		x = 145-64;
	
	if ( iphoneDrawPicWithTouch( x, y, 64, 32, "iphone/button_pistol.tga" ) ) {
		if ((int)volumeFireDownSetting->value)
			Cvar_Set("volumeFireDownSetting", "0");
		else 
			Cvar_Set("volumeFireDownSetting", "1");
	}
	
	if ((int)volumeFireDownSetting->value)
		iphoneDrawPic(x, y, 64, 32, "iphone/button_pistol.tga");
	else
		iphoneDrawPic(x, y, 64, 32, "iphone/button_knife.tga");
#endif
}


/*
 ==================
 iphoneSkillMenu
 
 ==================
 */
void iphoneSkillMenu() {
	int		s;
	char	str[64];
	
	//gsh
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	
	//gsh
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 48.0f ), "iphone/header_how_tough_are_you.tga");

	if ( BackButton() ) {
		menuState = IPM_MAIN;
		iphoneStartMainMenu();
	}
	
	// highlight the current skill selection
	s = (int)skill->value;
//	R_Draw_Fill( 80, 40+64*s, 320, 64, highlightColor );
	
	for ( s = 0 ; s < 4 ; s++ ) {
		my_snprintf( str, sizeof( str ), "iphone/button_skill%i.tga", s+1 );

		if ( s != (int)skill->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		//if ( iphoneDrawPicWithTouch( 112, 40+64*s, 256, 64, str ) ) {
		if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 112.0f, 60.0f+64.0f*s, 256.0f, 64.0f ), str ) ) {
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
	int		numE = 6;
	//gsh
	if (g_version->value == SPEAROFDESTINY)
		numE = 9;

	
	if ( BackButton() ) {
		menuState = IPM_SKILL;
	}
	
	// 96 x 48 images
	for ( e = 0 ; e < numE ; e++ ) {
		
		//gsh... added the if and button_epSOD
		if (e < 6)
			my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
		else
			my_snprintf(str, sizeof( str), "iphone/button_epSOD.tga");
		
		if ( e != (int)episode->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		float height = 32.0f;//48;//gsh... 96x32
		if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 48.0f, 32.0f+height*e, 384.0f, height), str ) ) {
			Cvar_SetValue( episode->name, e ); 
			menuState = IPM_MAPS;
		}/*gsh
		if ( iphoneDrawPicWithTouch( 48, 32+48*e, 384, 48, str ) ) {
			Cvar_SetValue( episode->name, e );
			menuState = IPM_MAPS;
		}*/
		pfglColor3f( 1, 1, 1 );
	}
}	

/*
 ==================
 iphoneUpdateScrollingDrags
 gsh
 ==================
 */
//int scrollingFlags = 0;
void iphoneUpdateScrollingDrags(int *dragPosition, int *dragVelocity, int upperLimit, int lowerLimit, int padding )
{
	static int oldVelocity = 0;
	
	
	//if Touch is in touchable area
//	if (TouchDown(touchRect.x, touchRect.y, touchRect.width, touchRect.height))
	if (TouchDown( 0, 0, viddef.width, viddef.height ) )//|| isTouchMoving)
	{
		if (!numPrevTouches)
			prevTouches[0][1] = touches[0][1];
		else if (numTouches == 1)
			*dragVelocity = ((float)(touches[0][1]) - prevTouches[0][1]) * ( 1.0f / screenScale );
		
		//if (*dragVelocity < 3)
		//	isTouchMoving = 0;
		
		/*
		Com_Printf("update drags:  Touch is down!\n");
		Com_Printf("dragVelocity: %i\n", *dragVelocity);
		Com_Printf("prevTouch: %i\n", prevTouches[0][1]);
		Com_Printf("touch: %i\n", touches[0][1]);
		*/
	}
//	else if (*dragVelocity == 0)  //this returns the position to a node position
	else if (abs(*dragVelocity) < 5)  //this returns the position to a node position
	{
		int startPosition = 70 - 10;
		int height = 48 * screenScale;
		int spacing = 20 * screenScale;
		int nodePosition = upperLimit + padding;
		
		
		int n = (*dragPosition - startPosition) / (height+spacing);
		
		nodePosition = n*(height+spacing) + startPosition;// - 5;
/*		if (nodePosition != *dragPosition)
		{
			int nodePositionPlus = nodePosition + (height+spacing);
			int nodePositionMinus = nodePosition - (height+spacing);
			int distMinus = abs(nodePositionMinus - *dragPosition);
			int distPlus = abs(nodePositionPlus - *dragPosition);
			int dist = abs(nodePosition - *dragPosition);
			
			
			int diff = nodePosition - *dragPosition;
			
			if (distMinus < distPlus && distMinus < dist)
				diff = nodePositionMinus - *dragPosition;
			else if (distPlus < dist && distPlus < distMinus)
				diff = nodePositionPlus - *dragPosition;
			
			if (diff < 0)
				--*dragVelocity;
			else
				++*dragVelocity;
			*
			if (oldVelocity > 0)
				++*dragVelocity;
			else if (oldVelocity < 0)
				--*dragVelocity;
		}*/
		
		//let's try a different way to continue our path
		if (nodePosition != *dragPosition && oldVelocity > 0)
		{
			int minSpeed = 5;
			if (oldVelocity > 0) {
				*dragVelocity = minSpeed;
				int newPosition = *dragPosition + *dragVelocity;
				
				Com_Printf("newPosition: %i\n", newPosition);
				Com_Printf("nodePosition: %i\n", nodePosition);
				Com_Printf("dragPosition: %i\n\n\n", *dragPosition);
				
				if ( newPosition > nodePosition ) {
					*dragPosition = nodePosition;
					*dragVelocity = 0;
				}
			}/*
			else if (oldVelocity < 0) {
				*dragVelocity = -minSpeed;
				int newPosition = *dragPosition + *dragVelocity;
				
				Com_Printf("newPosition: %i\n", newPosition);
				Com_Printf("nodePosition: %i\n", nodePosition);
				Com_Printf("dragPosition: %i\n\n\n", *dragPosition);
				
				if ( newPosition < nodePosition - height ){//(height + spacing)) {
					*dragPosition = nodePosition;
					*dragVelocity = 0;
				}
			}*/
		}
		
		nodePosition = (n-1)*(height+spacing) + startPosition;// - 10;//5;
//		if (nodePosition - (height + spacing - 5) != *dragPosition && oldVelocity < 0)
		if (nodePosition != *dragPosition && oldVelocity < 0)
		{
			int minSpeed = 5;
			
			//else if (oldVelocity < 0) {
				*dragVelocity = -minSpeed;
				int newPosition = *dragPosition + *dragVelocity;
//			nodePosition -= height + spacing - 5;
				
				Com_Printf("newPosition: %i\n", newPosition);
				Com_Printf("nodePosition: %i\n", nodePosition);
				Com_Printf("dragPosition: %i\n\n\n", *dragPosition);
				
				if ( newPosition < nodePosition /*- height*/ ){//(height + spacing)) {
					*dragPosition = nodePosition;
					*dragVelocity = 0;
				}
			//}
			
		}
	}
	
	
	oldVelocity = *dragVelocity;
	
	
	//update the drags
	*dragPosition += *dragVelocity;
	
	//boundary check our drags
	if (*dragPosition < lowerLimit - padding)
		*dragPosition = lowerLimit - padding;
	if (*dragPosition > upperLimit + padding)
		*dragPosition = upperLimit + padding;
	
	if (*dragPosition < lowerLimit)
		*dragVelocity = 2;
	if (*dragPosition > upperLimit)
		*dragVelocity = -2;
	
	//retard the velocity
	if (*dragVelocity > 0)
		--*dragVelocity;
	else if (*dragVelocity < 0)
		++*dragVelocity;
				
				
//	if (abs(*dragVelocity) < 5)
//			*dragVelocity = 0;
	
}

#endif

/*
 ==================
 iphoneDrawLoadingIndicator
 ==================
 */
void iphoneDrawLoadingIndicator() {
	//int loadingWidth = 128;
	//int loadingHeight = 128;
	
	//int halfWidth = loadingWidth / 2;
	//int halfHeight = loadingWidth / 2;
	
	// Keep track of ticks so we can rotate the indicator periodically.
	static int ticks = 0;
	
	// The graphic is designed on 45-degree angles, so that's how we want to rotate.
	float rotateInterval = 45.0f;
	
	// Rotate by rotateInterval degrees every tickInterval.
	int tickInterval = 10;
	
	static float rotateDegrees = 0.0f;
	
	if ( ticks >= tickInterval ) {
		rotateDegrees += rotateInterval;
		
		if ( rotateDegrees > 360.0f ) {
			rotateDegrees -= 360.0f;
		}
		
		ticks = 0;
	}	
	
	pfglTranslatef( (viddef.width / 2.0f),
					(viddef.height / 2.0f),
				    0.0f );
	pfglRotatef( rotateDegrees, 0.0f, 0.0f, 1.0f );
 
	
	//iphoneDrawPic( -halfWidth, -halfHeight, loadingWidth, loadingHeight, "iphone/loading.tga" );
	
	++ticks;
	
	return;
}

#if 0

/*
 ==================
 iphoneScrollingEpisodeMenu
 gsh
 ==================
 */
void iphoneScrollingEpisodeMenu() {
	int		e;
	char	str[64];
	int doesUserMapsExist = 0;
	
	//used for preventing accidental touches
	//user must hold touch down for 10 frames in order to load episode
	static int touchedFrameNum = 0;
	static int lastTouchFrame = 0;
	
	int		numE = 11;//10;
	
	//check for user maps
	DIR *dp;
	char mapBuffer[1024];
	
	int length = strlen(iphoneDocDirectory);
	strcpy(mapBuffer, iphoneDocDirectory);
	strcpy(mapBuffer + length, "/usermaps/");
	
	dp = opendir (mapBuffer);
	
	
	if (dp) {
		doesUserMapsExist = 1;
		//numE = 11;
		closedir(dp);
	}
	
	//Com_Printf("number of episodes: %i\n", numE);
	//gsh
	//if (g_version->value == SPEAROFDESTINY)
	//	numE = 9;
		
	
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 48.0f ), "iphone/header_episodes.tga");
	
	// If waiting for an in-app purchase, show the user an activity indicator.
	if ( InAppPurchaseIsInProgress() ) {
		iphoneDrawLoadingIndicator();
					   
		return;
	}
	
	
	if ( BackButton() ) {
		menuState = IPM_SKILL;
	}
	
	static int dragPosition = 70;
	static int dragVelocity = 0;
	
//	Com_Printf("dragPosition: %i  dragVelocity: %i\n", dragPosition, dragVelocity );
	
/*	int height=(320-48)/2;
	//iphoneDrawPic( 480-64, 60, 64, 64, "iphone/button_back.tga");
	if (iphoneDrawPicWithTouch( 480-64, 48, 64, height, "iphone/button_back.tga"))//TouchDown(480-64, 60, 64, 64))
		dragVelocity = -25;
	//iphoneDrawPic( 480-64, 60+64, 64, 64, "iphone/button_back.tga");
	if ( iphoneDrawPicWithTouch( 480-64, height+48, 64, height, "iphone/button_back.tga"))//TouchDown(480-64, 60+64, 64, 64)) 
		dragVelocity = 25;
*/	
	int lowerLimit = -320;
	lowerLimit = 70-(11-4)*(48+20);
	iphoneUpdateScrollingDrags(&dragPosition, &dragVelocity, 55, lowerLimit, 0 );
	
	// 96 x 48 images
	for ( e = 0 ; e < numE ; e++ ) {
	
		my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
		
		//light the episode we are currently on		
		if (currentMap.episode > 5 && currentMap.episode < 10)
		{
			//int tempEpisode = 6;
			int levelNum = currentMap.episode*10 + currentMap.map;
			
			//check for spear of destiny episodes
			if (levelNum == 78 || (levelNum >= 60 && levelNum < 65))
			{
				if ( e != 6 )
					pfglColor3f( 0.5, 0.5, 0.5 );
			}
			else if (levelNum >= 65 && levelNum < 70)
			{
				if ( e != 7 )
					pfglColor3f( 0.5, 0.5, 0.5 );
			}
			else if ((levelNum >= 70 && levelNum < 76) || levelNum == 79)
			{
				if ( e != 8 )
					pfglColor3f( 0.5, 0.5, 0.5 );
			}
			else if (levelNum == 76 || levelNum == 77 || levelNum == 80)
			{
				if ( e != 9 )
					pfglColor3f( 0.5, 0.5, 0.5 );
			}
				
		}
		else if ( e != (int)episode->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		
		//Com_Printf("isTouchMoving value: %i\n", isTouchMoving);
		
		
		int height = 48;
		int spacing = 20;//10;
		//if ( iphoneDrawPicInBoxWithTouch( 48, dragPosition+(height+spacing)*e, 384, height, str ) && !dragVelocity ) {
/*		if (iphoneDrawPicInBoxWithTouchSpecified(RectMake(0, dragPosition+(height+spacing)*e, 480, height), 
												 RectMake(0, dragPosition+(height+spacing)*e, 480, height),
												 str, 
												 RectMake(0, 48, 480, 320-48) )//RectMake(0, 42, 480, 238) )
											     && !dragVelocity
												 && !isTouchMoving )//touchedFrameNum >= NUM_TOUCHFRAME ) //must hold finger down for 10 frames
*///			iphoneDrawPicInBoxWithTouchAndVelocity
		if (iphoneDrawPicInBoxWithTouchAndVelocity(MakeScaledRectFloat(0.0f, dragPosition+(height+spacing)*e, 480.0f, height), 
												   MakeScaledRectFloat(0.0f, dragPosition+(height+spacing)*e, 480.0f, height),
												   str, 
												   MakeScaledRectFloat(0.0f, 48.0f, 480.0f, 320.0f-48.0f), dragVelocity | isTouchMoving ) )//RectMake(0, 42, 480, 238) )
				//&& !dragVelocity
				//&& !isTouchMoving )
		{
			//if they haven't downloaded any custom maps, but choose custom maps, goto the more menu
			if (e == 10 && !doesUserMapsExist)
			{
				wasDLInstructionsCalledFromEpisodeMenu = 1;
				menuState = IPM_DOWNLOADINSTRUCTIONS;
				pfglColor3f( 1, 1, 1 );
				break;
			}
			
			// If they have the Lite version, only allow episode 1.
			if ( SysIPhoneGetContentVersion() == CONTENT_PLATINUM || e == 0 ) {			
				Cvar_SetValue( episode->name, e ); 
				menuState = IPM_MAPS;
			} else {
				// Lite version, but user chose an episode that was not episode 1.
				// Prompt in-app purchase for platinum.
				// TODO: this.
				InAppPurchaseStartPurchase( PLATINUM_UPGRADE_STRING );
			}

		}
		pfglColor3f( 1, 1, 1 );
	}
	
	//update the number of frames the touch is down
	if ( TouchDown(0, 0, 480, 320) ) {
		int dframe = iphoneFrameNum - lastTouchFrame;
		touchedFrameNum += dframe;
	}
	else {
		touchedFrameNum = 0;
	}
	lastTouchFrame = iphoneFrameNum;
	

#ifdef SPEARSTOREKIT
	if (g_version->value != SPEAROFDESTINY)  //gsh
		GetSpearOfDestiny( 240 - 64, 232 );
#endif
	
	if ( MenuButton() ) {
		menuState = IPM_MAIN;
	}
}	

#endif

//gsh
extern void iphoneDrawRewards(int m, int s, int x, int y);
//gsh
extern int returnButtonFrameNum;
/*
//gsh... this is for the map selections
colour4_t colorSecret = { 80, 0, 0, 100 };
//colour4_t colorNoTry = { 0, 0, 0, 100 };
colour4_t colorNoTry = { 0, 0, 0, 0 };
colour4_t colorTried = { 120, 120, 0, 100 };
colour4_t colorCompleted = { 0, 0, 120, 80 };
*/
colour4_t colorSecret = { 120, 0, 60, 100 };
//colour4_t colorNoTry = { 0, 0, 0, 100 };
colour4_t colorNoTry = { 0, 0, 0, 150 };
colour4_t colorTried = { 80, 80, 80, 40 };
colour4_t colorCompleted = { 0, 120, 0, 80 };

#if 0

/*
 ========================
 iphoneDrawWolf3dMaps
 gsh
 ========================
 */
void iphoneDrawWolf3dMaps(int dragPosition, int dragVelocity, int e, int s, int height, int spacing, int touchedFrameNum)
{
	char	str[64];
	/*
	colour4_t colorSecret = { 64, 0, 0, 100 };
	colour4_t colorNoTry = { 0, 0, 0, 100 };
	colour4_t colorTried = { 80, 80, 0, 100 };
	colour4_t colorCompleted = { 0, 80, 0, 100 };
	*/
	
	// draw the individual maps
	for ( int m = 0 ; m < 10; m++ ) {
		int		y;
		
		switch(m) {
			case 9: sprintf( str, "SECRET" ); break;
			case 8: sprintf( str, "BOSS" ); break;
			default: sprintf( str, "LEVEL %i", m+1 ); break;
		}
		
		y = m*(height + spacing) + dragPosition; 
		
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
		} else if ( ch & MF_COMPLETED ) {
			color = colorCompleted;
		} else if ( ch & MF_TRIED ) {
			color = colorTried;
		} else {
			color = colorNoTry;
		}
		
		// blink the level you are currently on
		if ( ( iphoneFrameNum & 8 ) && m == currentMap.map && e == currentMap.episode ) {
			color = colorNoTry;
		}
		/*
		//TODO: delete this switch statement
		switch (m%4){
			case 0: color = colorSecret; break;
			case 1: color = colorTried; break;
			case 2: color = colorCompleted; break;
			case 3: color = colorNoTry; break;
		}*/
		
		rectFloat_t box = MakeScaledRectFloat(0.0f, 48.0f, 480.0f, 320.0f-48.0f);
		int borderWidth = 40;
/*		if (iphoneDrawPicInBoxWithTouchSpecifiedAndColor(RectMake(borderWidth, y - 9, 480-2*borderWidth, height),
														 RectMake(borderWidth, y - 9, 480-2*borderWidth, height),
														 "iphone/menu_bar.tga",
														 box,
														 color) && !dragVelocity
														 && !isTouchMoving)//touchedFrameNum >= 10)
*/			if (iphoneDrawPicInBoxWithTouchColorAndVelocity( MakeScaledRectFloat(borderWidth, y - 9.0f, 480.0f-2*borderWidth, height),
															 MakeScaledRectFloat(borderWidth, y - 9.0f, 480.0f-2*borderWidth, height),
															 "iphone/menu_bar.tga",
															 box,
															 color, dragVelocity | isTouchMoving) )// && !dragVelocity
				//&& !isTouchMoving)
		{
			// Only allow access to the first three levels if they have the Lite version.
			if ( SysIPhoneGetContentVersion() == CONTENT_PLATINUM || m < 3 ) {
				// don't let them go to the secret level unless they earned it
				if ( m == 9  && !( ch & MF_TRIED ) )
					continue;
				
				PL_NewGame( &Player );
				iphoneStartMap( e, m, s );
				iphoneFrameNum = 0;
				returnButtonFrameNum = 100;
				iphonePreloadBeforePlay();
			} else {
				// User has chosen a level he does not have acces to in the Lite version.
				// Prompt the user to buy Platinum with an in-app purchase.
				// TODO: This.
				InAppPurchaseStartPurchase( PLATINUM_UPGRADE_STRING );
			}

		}
		
		//draw the rewards
		iphoneDrawRewards(e*10+m, s, 0, y);
		
		//iphoneCenterText( x, y, str );
//		iphoneDrawTextInBox(RectMake(100, y, 16, 16), 500, str, box);
//		iphoneDrawText(100, y, 16, 16, str);
//		pfglColor4ub(0xE1, 0xA6, 0x0, 0xFF);
//		iphoneDrawText( 100, y , 16, 16, str );
//		pfglColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		iphoneDrawMapName( RectMake(100, y, 16, 16), str);
//		iphoneDrawMapName( RectMake(100, y, 32, 32), str);
//		iphoneFrameNum = 0;
//		returnButtonFrameNum = 100;//60;
	}	
}

/*
 ==================
 iphoneDrawSpearMaps
 gsh
 ==================
 */

void iphoneDrawSpearMaps(int dragPosition, int dragVelocity, int fakeEpisode, int s, int height, int spacing, int touchedFrameNum)
{
	char	str[64];
	/*
	colour4_t colorSecret = { 64, 0, 0, 100 };
	colour4_t colorNoTry = { 0, 0, 0, 100 };
	colour4_t colorTried = { 80, 80, 0, 100 };
	colour4_t colorCompleted = { 0, 80, 0, 100 };
	*
	//TODO: change these colors!!!!
	colour4_t colorSecret = { 80, 0, 0, 100 };
	colour4_t colorNoTry = { 0, 0, 0, 100 };
	colour4_t colorTried = { 120, 120, 0, 100 };
	colour4_t colorCompleted = { 0, 0, 120, 80 };
	*/
	int numMaps = 0;
	switch (fakeEpisode) {
		case 6: numMaps = 6; break;  //tunnels maps 1-5, 19
		//case 7: numMaps = 6; break;  //dungeon maps 6-10, 20
		//case 8: numMaps = 6; break;  //castle  maps 11-16
		case 7: numMaps = 5; break;  //dungeon maps 6-10
		case 8: numMaps = 7; break;  //castle  maps 11-16, 20
		case 9: numMaps = 3; break;  //final   maps 17, 18 and 21
		default: break;
	}
	
	//the real episode & map values
	int e = 6;
	int m = 0;
	
	// draw the individual maps
	for ( int fakeMap = 0; fakeMap < numMaps; fakeMap++ ) {
		int		y;
		
		if (fakeEpisode == 9)
		{
			switch(fakeMap) {
				case 0: sprintf( str, "Ramparts" ); break;
				case 1: sprintf( str, "Death Knight" ); break;
				case 2: sprintf( str, "Dungeon Dimension" ); break;
				default: sprintf( str, "LEVEL %i", fakeMap+1 ); break;
			}
		}
		else
		{
			switch(fakeMap) {
				case 6: if (fakeEpisode == 8) sprintf(str, "SECRET"); break;
				case 5: if (fakeEpisode == 8) sprintf( str, "BOSS" ); else sprintf( str, "SECRET" ); break;
				case 4: if (fakeEpisode != 8){ sprintf( str, "BOSS" ); break; }
				default: sprintf( str, "LEVEL %i", fakeMap+1 ); break;
			}
		}
		
		//get the real episode and map and store in e, m
		e = fakeEpisode;
		m = fakeMap;
		switch (fakeEpisode)
		{
			case 6:
				if (fakeMap == 5) {
					e = 7;
					m = 8;
				}
				break;
			case 7: 
				e = 6;
				m += 5;
				break;
			case 8:
				e = 7;
				if (fakeMap == 6)
					m = 9;
				break;
			case 9:
				if ( fakeMap == 0) { e = 7; m = 6; }
				if ( fakeMap == 1) { e = 7; m = 7; }
				if ( fakeMap == 2) { e = 8; m = 0; }
				break;
			default:
				break;
		}
		
		Com_Printf("fakeEpisode: %i  fakeMap: %i\n", fakeEpisode, fakeMap);
		Com_Printf("e: %i  m: %i\n", e, m);
		
		y = fakeMap*(height + spacing) + dragPosition; 

		// decide on the background color
		unsigned char *color = colorNoTry;
		int levelNum = e*10+m;
		int ch = currentMap.mapFlags[s][levelNum];
		
		// bit1 = attempted
		// bit2 = completed
		// bit3 = 100% kills
		// bit4 = 100% secrets
		// bit5 = 100% treasure
		if ( (levelNum == 78 || levelNum == 79)  && !( ch & MF_TRIED ) ) {
			color = colorSecret;
		} else if ( ch & MF_COMPLETED ) {
			color = colorCompleted;
		} else if ( ch & MF_TRIED ) {
			color = colorTried;
		} else {
			color = colorNoTry;
		}
		
		//blink the level you are currently on
		if ( ( iphoneFrameNum & 8 ) && m == currentMap.map && e == currentMap.episode ) {
			color = colorNoTry;
		}
		
		rectFloat_t box = MakeScaledRectFloat(0, 48, 480, 320-48);
		int borderWidth = 40;
/*		if (iphoneDrawPicInBoxWithTouchSpecifiedAndColor(RectMake(borderWidth, y - 9, 480-2*borderWidth, height),
														 RectMake(borderWidth, y - 9, 480-2*borderWidth, height),
														 "iphone/menu_bar.tga",
														 box,
														 color) && !dragVelocity
														 && !isTouchMoving)//touchedFrameNum >= NUM_TOUCHFRAME)
*/			if (iphoneDrawPicInBoxWithTouchColorAndVelocity(MakeScaledRectFloat(borderWidth, y - 9, 480-2*borderWidth, height),
															 MakeScaledRectFloat(borderWidth, y - 9, 480-2*borderWidth, height),
															 "iphone/menu_bar.tga",
															 box,
															 color, dragVelocity | isTouchMoving ) )
				//&& !isTouchMoving)
		{
			if ( SysIPhoneGetContentVersion() == CONTENT_PLATINUM ) {
				//diagnostic info
				Com_Printf("fakeEpisode: %i  fakeMap: %i\n", fakeEpisode, fakeMap);
				Com_Printf("e: %i  m: %i\n", e, m);
				
				// don't let them go to the secret level unless they earned it
				if ( (levelNum == 78 || levelNum == 79)  && !( ch & MF_TRIED ) )
					continue;

				PL_NewGame( &Player );
				iphonePreloadBeforePlay();
				iphoneStartMap(e, m, s);
				iphoneFrameNum = 0;
				returnButtonFrameNum = 100;
				break;  //break the loop... no need to keep looping
			} else {
				// User hasn't purchased platinum, prompt them to buy.
				InAppPurchaseStartPurchase( PLATINUM_UPGRADE_STRING );
			}

		}
		
		//draw the rewards
		iphoneDrawRewards(levelNum, s, 0, y);
		
		//draw map name
		iphoneDrawMapName( RectMake(100, y, 16, 16), str);
	}	
}

/*
 ==================
 iphoneDrawUserCreatedMaps
 gsh
 ==================
 */
int iphoneDrawUserCreatedMaps(int dragPosition, int dragVelocity, /*int fakeEpisode,*/ int skillValue, int height, int spacing, int touchedFrameNum)
{
	int numMaps = 0;
	
	DIR *dp;
	struct dirent *ep;
	char mapBuffer[1024];
	
	int length = strlen(iphoneDocDirectory);
	strcpy(mapBuffer, iphoneDocDirectory);
	strcpy(mapBuffer + length, "/usermaps/");
	
	dp = opendir (mapBuffer);
	
	if (dp != NULL)
	{
		/*
		//colour4_t colorSecret = { 64, 0, 0, 100 };
		colour4_t colorNoTry = { 0, 0, 0, 100 };
		colour4_t colorTried = { 80, 80, 0, 100 };
		colour4_t colorCompleted = { 0, 80, 0, 100 };
		 */
		unsigned char *color = colorNoTry;
		
		int y = dragPosition;//Yoffset;
				
		//draw the custom map spacing
		//y += height + spacing;
		iphoneDrawPic( 0, 0, 480, 48, "iphone/header_ep10.tga");
		
		Com_Printf("y: %i\n", y);
		
		while ( ( ep = readdir (dp) ) )
		{
			//if you find a .DS_Store file... ignore it!
			if ( strcmp(ep->d_name, ".DS_Store") == 0 )
				continue;
			
			++numMaps;
			if (numMaps < 3)  //skip the '.' and '..'
				continue;
			
			//Com_Printf("y: %i\n", y);
			
			//draw the names!
			if ( y >= 0 && y < 320 )
			{
				//grab map info
				int levelNum = numMaps-3 + 100;
				int ch = currentMap.mapFlags[skillValue][levelNum];
				
				// bit1 = attempted
				// bit2 = completed
				// bit3 = 100% kills
				// bit4 = 100% secrets
				// bit5 = 100% treasure
				if ( ch & MF_COMPLETED ) {
					color = colorCompleted;
				} else if ( ch & MF_TRIED ) {
					color = colorTried;
				} else {
					color = colorNoTry;
				}
				
				//comment to display completed user maps
				color = colorNoTry;
				
				//blink the level you are currently on
				if ( ( iphoneFrameNum & 8 ) && (currentMap.map+10*currentMap.episode)==levelNum ) {
					//color = colorNoTry;
					color = colorTried;
				}
				
				int borderWidth = 40;
				rectFloat_t box = MakeScaledRectFloat(0.0f, 48.0f, 480.0f, 320.0f-48.0f);
				if (iphoneDrawPicInBoxWithTouchColorAndVelocity( MakeScaledRectFloat(borderWidth, y - 9.0f, 480.0f-2.0f*borderWidth, height),
																 MakeScaledRectFloat(borderWidth, y - 9.0f, 480.0f-2.0f*borderWidth, height),
																 "iphone/menu_bar.tga",
																 box,
																 color, dragVelocity | isTouchMoving) )
				{
					//reset the player to new
					PL_NewGame( &Player );
					
					//set episode and map numbers and load map
					int e = 10; //10 will be used to show a custom map
					Cvar_SetValue( episode->name, e );
					iphonePreloadBeforePlay();				//make sure all necessary textures are loaded
					iphoneStartUserMap( e, numMaps - 3, skillValue, ep->d_name );
					iphoneFrameNum = 0;
					returnButtonFrameNum = 100;
					break; //no point in continuing the loop
				}
				
				
				pfglColor4f(0, 0, 0, 1);
				iphoneCenterArialText( 240+1, y +25+1, 0.9f, ep->d_name );
				iphoneCenterArialText( 240+2, y +25+2, 0.9f, ep->d_name );
				pfglColor4f(225.0f/255, 166.0f/255, 0, 1);
				pfglColor4f(225.0f/255, 242.0f/255, 0, 1);
				iphoneCenterArialText( 240, y +25, 0.9f, ep->d_name );
				pfglColor4f(1, 1, 1, 1);
				
				//draw the rewards
				//uncomment to display rewards
				//iphoneDrawRewards(numMaps-2+90, skillValue, 0, y);
			}
			
			y += height + spacing;
		}
		closedir (dp);
		
		--numMaps;
		--numMaps;
	}

	return numMaps;
}

#endif

#if 0

/*
 ==================
 iphoneScrollingMapMenu
 gsh
 ==================
 */
extern int forceDrawRewards;
void iphoneScrollingMapMenu()
{

# define NUM_MAPS 10

	//used for preventing accidental touches
	//user must hold touch down for 10 frames in order to load map
	static int touchedFrameNum = 0;
	static int lastTouchFrame = 0;	
	
	int		e, s;
	char	str[64];
	
	int height = 48;
	int spacing = 20;//10;
	
	static int dragPosition = 70;
	static int dragVelocity = 0;
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	
	// If waiting for an in-app purchase, show the user an activity indicator.
	if ( InAppPurchaseIsInProgress() ) {
		iphoneDrawLoadingIndicator();
		
		return;
	}
	
/*	
	//static int forceDrawRewards = 0;
	//This is only for debugging/testing out the look of the medals
	//TODO: delete or comment out
	if (iphoneDrawPicWithTouch(0, 320-40, 40, 40, "iphone/button_back.tga"))
	{
		if (forceDrawRewards == 0)
			forceDrawRewards = 1;
		else
			forceDrawRewards = 0;
	}
	
*/	
	// draw the level selection
	e = episode->value;
	if ( e < 0 ) {
		e = 0;
	}
	
	if ( e > 10 ) //gsh
		e = 10;
	
	s = skill->value;
	if ( s < 0 ) {
		s = 0;
	}
	if ( s > 3 ) {
		s = 3;
	}
	
	//gsh
	int numlevels = 10;
	if ( e == 8 )
		numlevels = 1;
	
	

	//----------------------------
	//Update Drags
/*	height=(320-48)/2;
	if (iphoneDrawPicWithTouch( 480-64, 48, 64, height, "iphone/button_back.tga"))
		dragVelocity = -25;
	if ( iphoneDrawPicWithTouch( 480-64, 48+height, 64, height, "iphone/button_back.tga")) 
		dragVelocity = 25;
*/	height = 48;
	
	static int lowerLimit = 65;
	/*
	if ( e < 6)
		lowerLimit = -250;
	else if ( e < 9)
		lowerLimit = -20;
	else if ( e < 10)
		lowerLimit = 65;
	*/
	if ( e < 6) {
		//lowerLimit = -250;
		lowerLimit = 70 - (10-4)*(height+spacing); //startPosition - (numberMaps - numberOnScreen)*(height + spacing)
	}
	else {
		switch (e) {
			case 6:       //tunnels
				lowerLimit = -20;
				lowerLimit = 70 - (6-4)*(height+spacing);
				break;
			case 7:       //dungeons
				lowerLimit = 20;
				lowerLimit = 70 - (5-4)*(height+spacing);
				break;
			case 8:       //castle
				lowerLimit = -85;
				lowerLimit = 70 - (7-4)*(height+spacing);
				break;
			case 9:       //ramparts
				lowerLimit = 65;
				break;
		}
	}
	/*
	if ( e == 7 )
		lowerLimit = 20;
	
	if ( e == 8 )
		lowerLimit = -85;
	*/
	iphoneUpdateScrollingDrags( &dragPosition, &dragVelocity, 65, lowerLimit, 0 );
	//----------------------------

	if ( e < 6 )
		iphoneDrawWolf3dMaps(dragPosition, dragVelocity, e, s, height, spacing, touchedFrameNum);
	else if ( e < 10 )
		iphoneDrawSpearMaps(dragPosition, dragVelocity, e, s, height, spacing, touchedFrameNum);
	else {
		int numMaps = iphoneDrawUserCreatedMaps(dragPosition, dragVelocity, s, height, spacing, touchedFrameNum);
		
		lowerLimit = 65;
		if (numMaps > 4) {
			//lowerLimit = 269 - (numMaps-1)*(spacing + height);
			//lowerLimit = 65 - (numMaps - 4)*(spacing + height) + 30;
			lowerLimit = 70 - (numMaps-4)*(height+spacing);
		}
	}

	
	//update the number of frames the touch is down
	if ( TouchDown(0, 0, 480, 320) ) {
		int dframe = iphoneFrameNum - lastTouchFrame;
		touchedFrameNum += dframe;
	}
	else {
		touchedFrameNum = 0;
	}
	lastTouchFrame = iphoneFrameNum;
	

	// draw the header for the episode
	my_snprintf( str, sizeof( str ), "iphone/header_ep%i.tga", e+1 );
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 48.0f ), str );
	
	if ( BackButton() ) {
		menuState = IPM_EPISODE;
	}
	if ( MenuButton() ) {
		menuState = IPM_MAIN;
	}
}
/*
 ==================
 iphoneMapMenu
 
 ==================
 */
void iphoneMapMenu() {

# define NUM_MAPS 10
	int		e, m, s;
	char	str[64];
	
	if ( BackButton() ) {
		menuState = IPM_EPISODE;
	}

	// draw the level selection
	e = episode->value;
	if ( e < 0 ) {
		e = 0;
	}/*gsh
	if ( e > 5 ) {
		e = 5;
	}*/
	if ( e > 8 ) //gsh
		e = 8;
	//if player chose SOD maps... draw all 21 SOD maps.. gsh
	/*	if ( e >= 6 ){
	 iphoneDrawSODMaps(); //this function didn't seem to be working out
	 return;
	 }
	 */
	s = skill->value;
	if ( s < 0 ) {
		s = 0;
	}
	if ( s > 3 ) {
		s = 3;
	}
	
	//gsh
	int numlevels = 10;
	if ( e == 8 )
		numlevels = 1;
	
	// draw the episode selection
	my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
	iphoneDrawPicWithTouch( 96, 0, 384, 48, str );
		
	// draw the individual maps
	for ( m = 0 ; m < NUM_MAPS; m++ ) {
		int		x;
		int		y;
		colour4_t colorSecret = { 64, 0, 0, 255 };
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
		
		Com_Printf("Map: %i   ch: %i\n", levelNum, ch);
		// bit1 = attempted
		// bit2 = completed
		// bit3 = 100% kills
		// bit4 = 100% secrets
		// bit5 = 100% treasure
		if ( m == 9  && !( ch & MF_TRIED ) ) {
			color = colorSecret;
		} else if ( ch & MF_COMPLETED ) {
			color = colorCompleted;
		} else if ( ch & MF_TRIED ) {
			color = colorTried;
		} else {
			color = colorNoTry;
		}
		
		// blink the level you are currently on
		if ( ( iphoneFrameNum & 8 ) && m == currentMap.map && e == currentMap.episode ) {
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
			iphonePreloadBeforePlay();			
		}
	}
}

#endif

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
	if ( levelstate.time / 60.0f <= levelstate.fpartime * 60 ) {	// fpartime is in minutes, time is in tics
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
	float width = 32.0f;
	float height = 32.0f;
	
	float xFloat = (float)x;
	float yFloat = (float)y;
	
	ScalePositionAndSize( &xFloat, &yFloat, &width, &height );
	
	R_Bind( numberPics[digit]->texnum );
	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0, 0 );	pfglVertex2f( xFloat,		yFloat );
	pfglTexCoord2f( 0, 1 );	pfglVertex2f( xFloat,		yFloat+height );
	pfglTexCoord2f( 1, 0 );	pfglVertex2f( xFloat+width,	yFloat );
	pfglTexCoord2f( 1, 1 );	pfglVertex2f( xFloat+width,	yFloat+height );
	
	
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
		iphoneDrawPicRect( MakeScaledRectFloat( 480.0f - 40.0f, y, 32.0f, 32.0f ), bonusPic );
	}
}

/*
 ==================
 DrawIntermissionStats
 
 Used for both the intermission and the stats view
 ==================
 */
void DrawIntermissionStats() {
	char	str[128];
	
	//gsh
//	iphoneDrawPic( 0, 0, 480, 320, "iphone/intermission_256.tga" );
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/intermission.tga" );

	// episode
	//if (currentMap.episode < 10)//6)   //gsh added the check for SOD
//	my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", currentMap.episode+1 );
	
	//display the correct episode header
	if (currentMap.episode < 6)
		my_snprintf( str, sizeof( str ), "iphone/header_ep%i.tga", currentMap.episode+1 );
	else if (currentMap.episode < 10) {
		int currentLevel = currentMap.episode * 10 + currentMap.map;
		switch (currentLevel) {
			case 60: case 61: case 62: case 63: case 64: case 78:
				my_snprintf( str, sizeof( str ), "iphone/header_ep7.tga" );
				break;
			case 65: case 66: case 67: case 68: case 69: case 79:
				my_snprintf( str, sizeof( str ), "iphone/header_ep8.tga" );
				break;
			case 70: case 71: case 72: case 73: case 74: case 75:
				my_snprintf( str, sizeof( str ), "iphone/header_ep9.tga" );
				break;
			case 76: case 77: case 80:
				my_snprintf( str, sizeof( str ), "iphone/header_ep10.tga" );
				break;
/*			case 77:
				my_snprintf( str, sizeof( str ), "iphone/header_ep10.tga" );
				break;
			case 80:
				my_snprintf( str, sizeof( str ), "iphone/header_ep10.tga" );
				break;
*/			default:
				sprintf( str, "  ");
				break;
		}
	}
	else
		my_snprintf( str, sizeof( str ), "iphone/header_ep11.tga" );
	//else
	//	my_snprintf( str, sizeof( str ), "iphone/button_epSOD.tga");
	
//	iphoneDrawPic( 0, 0, 384, 48, str );
//	iphoneDrawPic( -104, 0, 480, 48, str ); //gsh
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 48.0f), str ); //gsh

	// level
//	iphoneDrawNumber( 430, 0, currentMap.map + 1, 48, 48 );
//	iphoneDrawNumber( 430-48, 50, currentMap.map + 1, 48, 48 ); //gsh
	
	//display the appropriate mission number
	float missionWidth = 48.0f;
	float missionHeight = 48.0f;
	
	float missionX = 430.0f - missionWidth;
	float missionY = 50.0f;
	
	ScalePositionAndSize( &missionX, &missionY, &missionWidth, &missionHeight );
	
	if (currentMap.episode < 6)
		iphoneDrawNumber( missionX, missionY, currentMap.map + 1, missionWidth, missionHeight );
	else if (currentMap.episode < 10) {
		int currentLevel = currentMap.episode * 10 + currentMap.map;
		switch (currentLevel) {
			case 60: case 61: case 62: case 63: case 64:
				iphoneDrawNumber( missionX, missionY, currentLevel - 60 + 1, missionWidth, missionHeight );
				break;
			case 78:
				iphoneDrawNumber( missionX, missionY, 6, 48, 48 );
				break;
			case 65: case 66: case 67: case 68: case 69:
				iphoneDrawNumber( missionX, missionY, currentLevel - 65 + 1, missionWidth, missionHeight );
				break;
			case 79:
				iphoneDrawNumber( missionX, missionY, 6, 48, 48 );
				break;
			case 70: case 71: case 72: case 73: case 74: case 75:
				iphoneDrawNumber( missionX, missionY, currentLevel - 70 + 1, missionWidth, missionHeight );
				break;
			case 76:
				iphoneDrawNumber( missionX, missionY, 1, missionWidth, missionHeight );
				break;
			case 77:
				iphoneDrawNumber( missionX, missionY, 2, missionWidth, missionHeight );
				break;
			case 80:
				iphoneDrawNumber( missionX, missionY, 3, missionWidth, missionHeight );
				break;
			default:
				sprintf( str, "  ");
				break;
		}
	}
	else
		iphoneDrawNumber( missionX, missionY, currentMap.map + 1, missionWidth, missionHeight );
	

	// par / time
	float offset = 50.0f;
	DrawTime( 51, 63+offset, levelstate.fpartime * 60 );	// fpartime is in minutes
	DrawTime( 285, 63+offset, levelstate.time / 60 );	// levelstate.time is in tics
	if ( levelstate.time/60 <= levelstate.fpartime * 60 ) {
		iphoneDrawPicRect( MakeScaledRectFloat( 480.0f - 40.0f, 63.0f+offset, 32.0f, 32.0f ),
						   "iphone/partime.tga" );
	}
	
	// ratios
	int correction = 8;
	DrawRatio( 124+offset-1*correction, levelstate.killed_monsters, levelstate.total_monsters, "iphone/kills.tga" );
	DrawRatio( 189+offset-2*correction, levelstate.found_secrets, levelstate.total_secrets, "iphone/secrets.tga" );
	DrawRatio( 255+offset-3*correction, levelstate.found_treasure, levelstate.total_treasure, "iphone/treasure.tga" );
}

/*
 ==================
 iphoneIntermission
 
 ==================
 */
void iphoneIntermission() {
	int		nextLevel = 0;
	
	DrawIntermissionStats();

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
	if ( !TouchReleased( 0, 0, viddef.width, viddef.height ) ) {
		return;
	}
	
	menuState = IPM_GAME;
	
	PL_NextLevel( &Player );
	
	if( g_version->value == SPEAROFDESTINY && currentMap.episode >= 6 && currentMap.episode < 9)//added the episode check... gsh)
	{
		//added all this leveling stuff... gsh
		int currentLevel = currentMap.episode * 10 + currentMap.map;
		nextLevel = currentLevel + 1;
		switch (currentLevel) {
			case 78: nextLevel = 64; break;  //if tunnels secret was defeated goto tunnels boss
//			case 79: nextLevel = 69; break;  //if dungeons secret was defeated goto dungeons boss
			case 79: nextLevel = 72; break;  //if castle secret was defeated goto next castle map
			default: break;
		}
		if (Player.playstate == ex_secretlevel) {
			switch (currentLevel) {
				case 63: nextLevel = 78; break;  //if tunnels secret was earned goto tunnels secret
				//case 68: nextLevel = 79; break;  //if dungeons secret was earned goto dungeons secret
				case 71: nextLevel = 79; break;  //if castle secret was earned goto castle secret
				default: break;
			}
		}
		if (nextLevel >= 60 + 21)
		{
			// show ultimate victory
			menuState = IPM_VICTORY;
			Sound_StartBGTrack( "music/URAHERO.ogg", "music/URAHERO.ogg" );
			return;
		}
	}
	else if (currentMap.episode < 6)  //gsh added the check if it's the original wolf3d episodes
	{
		int		currentLevel = currentMap.episode * 10 + currentMap.map;
		
		if ( SysIPhoneGetContentVersion() == CONTENT_LITE ) {
			switch ( currentLevel ) {
				case 2:
					// go back to the episode select screen
					menuState = IPM_VICTORY;
					Sound_StartBGTrack( "music/URAHERO.ogg", "music/URAHERO.ogg" );
					return;
				default: nextLevel = currentLevel + 1; break;
			}		
		} else { // Content is Platinum
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
	}
	else  //they just defeated a custom map... gsh
	{
		// go back to the episode select screen
		//menuState = IPM_VICTORY;
		menuState = IPM_MAPS;
		Sound_StartBGTrack( "music/URAHERO.ogg", "music/URAHERO.ogg" );
		return;
	}
	
	iphoneStartMap( (nextLevel/10), (nextLevel%10), skill->value );
}

/*
 ==================
 iphoneVictory
 
 ==================
 */
void iphoneVictory() {
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ),
					   "iphone/victory_256.tga" );
	if ( !TouchReleased( 0, 0, viddef.width, viddef.height ) ) {
		return;
	}
	menuState = IPM_EPISODE;
	iphoneStartMainMenu();
//gsh	menuState = IPM_MAPSELECTOR;
}

/*
 ==================
 iphoneAutomap
 
 ==================
 */
float	mapOrigin[2];

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
						LoadWallTexture(mt->texnum);
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
				LoadWallTexture(mt->texnum);
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
	for( n = 0, sprt = levelData.sprites; n < levelData.numSprites; ++n, ++sprt ) {
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
	int halfWidth = viddef.width / 2;
	int halfHeight = viddef.height / 2;
	
	//draw the new background... gsh
	iphoneDrawPicRect( MakeScaledRectFloat( 0, 0, 480, 320 ), "iphone/map_background.tga");
	
	
	mapTile_t	*mt;
	float	px, py;
	float	angle, c, s;
	int		texnum;
	float	scale;
	
	// do touch ops before drawing for minimum latency
	
	scale = mapScale->value;
	if ( scale < 4 ) {	// ensure we don't div by 0 from a console cvar change
		scale = 4;
	}
	
	// drag for scrolling
	if ( numTouches == 1 ) {
		if ( numPrevTouches == 1 ) {
			mapOrigin[0] -= ( touches[0][0] - prevTouches[0][0] ) / scale;
			mapOrigin[1] += ( touches[0][1] - prevTouches[0][1] ) / scale;
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
			scale *= curDist / prevDist;
			if ( scale < 4 ) {
				scale = 4;
			}
			if ( scale > 64 ) {
				scale = 64;
			}
			Cvar_SetValue( mapScale->name, scale );
		}
		
	}
	
	// gsh - double tap for center on player
	static unsigned int timeTouchDown = 0;
	static int numTaps = 0;
	static unsigned int lastTapTime = 0;
	static int tapX = 0;
	static int tapY = 0;
	static int zoom = 0;
	static float TargetX = 0;
	static float TargetY = 0;
	static float TargetZoom = 0;
	static int prevTapX = 0;
	static int prevTapY = 0;
		
	//touch down
	if ( numTouches == 1 && numPrevTouches == 0) {
		timeTouchDown = Sys_Milliseconds();
		prevTapX = tapX;
		prevTapY = tapY;
		tapX = touches[0][0];
		tapY = touches[0][1];
	}
	//touch up
	if ( numTouches == 0  && numPrevTouches == 1 ) {
		unsigned int currentTime = Sys_Milliseconds();
		
		//check if time between last tap and current time is too long
		if (Sys_Milliseconds() - lastTapTime > 500 || zoom)
			numTaps = 0;

		//detect tap
		if ( currentTime - timeTouchDown  < 200 ) {

			//record tap time if first tap
			if (numTaps < 1)
				lastTapTime = Sys_Milliseconds();
			
			++numTaps;
			/*
			//center map on player
			if (numTaps >= 2) {
				numTaps = 0;
				zoom = 1;
				TargetX = Player.position.origin[0] / (float)TILEGLOBAL;
				TargetY = Player.position.origin[1] / (float)TILEGLOBAL;
				TargetZoom = scale;
			}*/
			
			//the taps need to be near each other to qualify as a double tap event
			int dx = abs(tapX - prevTapX);
			int dy = abs(tapY - prevTapY);
			int maxTapSeparation = 60;
			
			//safari style double tap zoom on location!
			if (numTaps >= 2 && dx < maxTapSeparation && dy < maxTapSeparation) {
				numTaps = 0;
				zoom = 1;
				//need to translate tap/screen coordinate to map coordinate
				
				int Xrelative2screenorigin = tapX - halfWidth;
				int Yrelative2screenorigin = halfHeight - tapY;
				float scaledX = Xrelative2screenorigin / scale;
				float scaledY = Yrelative2screenorigin / scale;
				float camSpaceX = scaledX + mapOrigin[0];
				float camSpaceY = scaledY + mapOrigin[1];
				
				//mapOrigin[0] = camSpaceX;
				//mapOrigin[1] = camSpaceY;
				
				TargetX = camSpaceX;
				TargetY = camSpaceY;
				
				if (scale < 32) { //zoom half way in
					//scale = 32;
					TargetZoom = 32;
				}
				else {
					//scale = 4;  //zoom completely out
					TargetZoom = 4;
				}
				
				Cvar_SetValue( mapScale->name, scale );
			}
		}
		else {
			//no tap... reset to 0
			numTaps = 0;
		}
	}
	
	if (zoom)
	{
		int invSpeed = 2;//3;
		float dist = TargetX - mapOrigin[0];
		mapOrigin[0] += dist/invSpeed;
		dist = TargetY - mapOrigin[1];
		mapOrigin[1] += dist/invSpeed;
		
		dist = TargetZoom - scale;
		scale += dist/(2*invSpeed);  //let's zoom a litle slower than we pan
		
		
		Com_Printf("current: %f\n", mapOrigin[0]);
		Com_Printf("target: %f\n", TargetX);
		//Com_Printf("dist: %f\n\n", dist);
		
		float tolerance = 0.5f;//3;
		if ( abs(mapOrigin[0] - TargetX) < tolerance && abs(mapOrigin[1] - TargetY) < tolerance && abs(scale - TargetZoom) < tolerance/2)
		{
			mapOrigin[0] = TargetX;
			mapOrigin[1] = TargetY;
			scale = TargetZoom;
			zoom = 0;
		}
		
		Cvar_SetValue( mapScale->name, scale );
	}

	// set up matrix for drawing in tile units
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	iphoneRotateForLandscape();
	pfglOrtho( mapOrigin[0]-halfWidth / scale, mapOrigin[0]+halfWidth / scale,
			  mapOrigin[1]-halfHeight / scale, mapOrigin[1]+halfHeight / scale, -99999, 99999 );

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
	iphoneRotateForLandscape();
	pfglOrtho( 0, viddef.width, viddef.height, 0, -99999, 99999 );
	if ( BackButton() ) {
		menuState = IPM_GAME;
	}
	
	// stats button for stats display
	//if ( iphoneDrawPicWithTouch( 64, 0, 64, 36, "iphone/stats.tga" ) ) {
	if ( iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 64.0f, 0.0f, 120.0f, 36.0f ), "iphone/stats_large.tga" ) ) {
		menuState = IPM_STATS;
	}
	//gsh
	if ( MenuButton() ) {
		menuState = IPM_MAIN;
	}
}

void iphoneStats() {
	DrawIntermissionStats();
		
	//gsh
	if ( BackButton() ) {
		menuState = IPM_AUTOMAP;
	}
	if ( MenuButton() ) {
		menuState = IPM_MAIN;
	}
}

#if SPEARSTOREKIT
//==========================
// iphoneDownloadSpearProgressBar
//gsh
//==========================
void iphoneDownloadSpearProgressBar()
{
	char str[80];
	float	scale = 40 / 32.0;
	int barHeight = 30;
		
	//calculate the percentage of data received
	unsigned int percentDownloaded = totalDownloaded * 100 / DownloadFileSize;
	
	int y = barHeight + 150 + 10;
	my_snprintf( str, sizeof(str), "Downloading: %i", dataAmount);
	iphoneDrawText( 100, y, 16, 16, str );
	y += 17*scale;
	my_snprintf( str, sizeof(str), "Percent Download: %i\%", percentDownloaded );
	iphoneDrawText( 100, y, 16, 16, str );
	y += 17*scale;
	my_snprintf( str, sizeof(str), "Total Downloaded: %i", totalDownloaded );
	iphoneDrawText( 100, y, 16, 16, str );
	y += 17*scale;
	my_snprintf(str, sizeof(str), "SOD will download and install itself.");  
	iphoneDrawText( 50, y, 16, 16, str );
	y += 17*scale;
	my_snprintf(str, sizeof(str), "You have not been charged at this time.");
	iphoneDrawText( 50, y, 16, 16, str );
	y += 17*scale;
	
	//draw "Spear of Destiny" at the top
	//iphoneDrawPic(90, 20, 256, 128, "iphone/spear_logo.tga");

	//draw the progress bar
	iphoneDrawPic( 40, 150, 400, barHeight, "iphone/menu_bar.tga" );
	
	//draw the progress
	colour4_t color = { 255, 0, 0, 100 };
	R_Draw_Blend( 40, 150, 4*percentDownloaded, barHeight, color );		
}
#endif

char urlbuffer[1024];

#if 0

//==========================
// iphoneDownloadMenu
//gsh
//==========================
void iphoneDownloadMenu()
{	
	char str[80];
	float	scale = 40 / 32.0;
	int barHeight = 30;
	
	//draw the background
	//iphoneDrawPic(0, 0, 480, 320, "iphone/download_background.tga");
	
	//draw the url
	iphoneCenterText(240, 120, urlbuffer);
	//iphoneDrawTextInBox(RectMake(0, 120, 16, 16), 480, urlbuffer, RectMake(0, 120, 480, 32));
	
	int y = barHeight + 150 + 10;
	my_snprintf( str, sizeof(str), "Packet Size: %i", userDataAmount);
	iphoneDrawText( 100, y, 16, 16, str );
	y += 17*scale;

	my_snprintf( str, sizeof(str), "Total Downloaded: %i", totalDownloaded );
	iphoneDrawText( 100, y, 16, 16, str );
	y += 17*scale;
	
	static int iterator = 5;
	static int pos = 230;
	
	//int width = 60;
	pos += iterator;
		
	if (pos > 480)
		pos = -60;
	
	//draw the barrel
	iphoneDrawPic(0, 150, 480, 34, "iphone/download_progress_bar.tga");
	
	//draw the bullet!
	iphoneDrawPic(pos, 150+34/2-22/2, 60, 22, "iphone/download_bullet.tga");
}

#endif

/*
//==========================================
//  iphoneUpdateTriviaDrags()
//  Updates the touches for the scrolling maps
 // gsh
//==========================================
 //TODO: delete this
void iphoneUpdateTriviaAndInstructionsDrags(int numEnteries, int x, int y, int w, int h, int spacing, int *dragPosition, int *dragVelocity)
{
	//Update drag/touch info
	//	int x = 0;
	//	int w = 480;// - skillSides - 64;
	
	if (TouchDown(x, y, w, h))
	{
		if (!numPrevTouches)
			prevTouches[0][0] = touches[0][0];
		else if (numTouches == 1)
			*dragVelocity = touches[0][0] - prevTouches[0][0];
	}
	
	
	
	
	//update the drags
	*dragPosition += *dragVelocity;
	
	//int rightBoundary = 240 - 2*width;//480/2 - (numPresets*width/2 + spacing);
	if (*dragPosition < -(numTrivia-1)*480-10)//270-10)//(numPresets*(width+spacing))/2 )
		*dragPosition = -(numTrivia-1)*480-10;//270-10;//(numPresets*(width+spacing))/2;
	else if (*dragPosition > x+10)//rightBoundary)
		*dragPosition = x+10;//rightBoundary;
	
	if (*dragPosition < -(numTrivia-1)*480 + x)//270)
		*dragVelocity = 2;
	else if (*dragPosition > x)
		*dragVelocity = -2;
	
	
	//retard the velocity
	if (*dragVelocity > 0)
		--*dragVelocity;
	else if (*dragVelocity < 0)
		++*dragVelocity;
	
}*/

//----------------------------------
// DrawBlendInBox
// gsh
//-----------------------------------
void DrawBlendInBox(int x, int y, int w, int h, colour4_t color, int boxX, int boxW)
{
	if ( x + w < boxX )
		return;
	else if ( x > boxX + boxW)
		return;
	
	if ( x < boxX ) {
		w = x + w - boxX;
		x = boxX;
	}
	if ( x + w > boxX + boxW )
		w = boxX + boxW - x;
		
	R_Draw_Blend( x, y, w, h, color );
}

//----------------------------------
// iphoneTriviaMenu
// gsh
//-----------------------------------
#if 0
// TODO: delete this implementation of iphoneTriviaMenu
void iphoneTriviaMenu()
{
	//int numTrivia = 15;
	
	static char triviaText[][1024] = {
		" ORIGINAL WOLF:\n\nThe original release of Wolfenstein 3D only contained 3 Episodes. Episodes 4, 5, and 6 were sold separately as \"The Nocturnal Missions\", and were intended to act as a prequel to the original trilogy of Episodes.  Subsequent releases of the game included all 6 Episodes together.",
		" BJ, THE MAN:\n\nWilliam Joseph Blazkowicz was born August 15, 1911, to Polish immigrants. Blazkowicz was a top spy for the Allied Forces, eventually receiving the Congressional Medal of Honor.",
		" BOX ART:\n\nKen Rieger was the talented illustrator who created the cover art for Spear of Destiny, Wolfenstein3D and Commander Keen.",
		" THE REAL SPEAR:\n\nThe Spear of Destiny, also known as The Spear or The Holy Lance, is believed to have been the weapon used to pierce the side of Jesus Christ at the Crucifixion.  The Spear soon became a symbol of God's favor and those who possessed it were believed to be invincible.",
		" THE SPEAR AND HITLER:\n\nHitler is believed to have acquired the Spear at the beginning of WWII.  Near the end of that war, it is rumored that General Patton discovered the Spear and that the downfall of Germany began that day.  It is fabled that the U.S. was destined to succeed Germany as the New World Power once ownership of the Spear changed hands.",
		" DEATH DIMENSION:\n\nIt is said that Hitler made a pact with the Angel of Death, promising the souls of his Nazi soldiers, to protect the Spear of Destiny.",
		" GOD MODE:\n\nIn the game, if you can access god mode, BJ's eyes will glow with eerie golden light.",
		" A PRINCE:\n\nRobert Prince, wrote the music for Commander Keen, Wolfenstein 3-D, and Doom.",
		" DELAYED DEVELOPMENT:\n\nIt is rumored that work on Spear of Destiny was hindered by the development team's time spent playing Fatal Fury (which Jay Wilbur called 'Fatal Productivity'), and Street Fighter II.",
		" A MATTER OF DEGREES:\n\nJohn Carmack is known for his innovations in 3D graphics and rocketry but what is less known is that this self-taught engineer has no degrees - he dropped out of school after one semester.",
		" THE CARMACK:\n\nJohn Carmack, one of the founders of id Software, preferred to work at a pizza joint and make games rather than go to college.  His favorite programming food is pizza and diet coke.",
		" DREAMING IN DIGITAL:\n\nDoom was reported to cause gamers to dream in pixels and lower productivity.",
		" KEEPING IT IN THE FAMILY:\n\nBJ is Commander Keen's grandfather.",
		" WHAT'S IN A NAME:\n\nThe original name for Doom was \"It's Green and Pissed\"",
		" GIBS:\n\nGibs, pronounced with a hard 'g', is sometimes confused as 'jibs' due to its pronunciation.  The term came from the imagery of flying chicken giblets, or gibs for short.",
		" WOLFENSTEIN 3D:\n\nThe game was originally designed to have more complex gameplay. Though dragging bodies, stealth kills and uniforms were already programmed, it was decided that they slowed down the pace of the game and were removed.",
		" ORIGINAL WOLF:\n\nThe original release of Wolfenstein 3D only contained 3 Episodes. Episodes 4, 5, and 6 were sold separately as \"The Nocturnal Missions\", and were intended to act as a prequel to the original trilogy of Episodes.  Subsequent releases of the game included all 6 Episodes together.",
	};
	
	//get number of trivia
	int numTrivia = sizeof(triviaText)/(sizeof(char) * 1024);
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	iphoneDrawPicRect( MakeScaledRectFloat( 240.0f-108*9/10.0f, 0.0f, 217.0f*9/10.0f, 50.0f*9/10.0f ), "iphone/header_trivia.tga");
	
	if (BackButton()) {
		menuState = IPM_MORE;
		return;
	}
	
	if (MenuButton()) {
		menuState = IPM_MAIN;
		return;
	}
	
	int startPosition = 85;
	
	//update the drags
	static int dragPosition = 85;
	static int dragVelocity = 0;
	//static int currentLocation = 0;
	//	int y = 30;
	
	int y = 20;
	int x = 0;
	//if (revLand->value)
	//	y = -20;
	
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 9.0f, y+60.0f+16, 64.0f, 64.0f ), "iphone/button_left.tga") ){//&& !dragVelocity) {
		dragVelocity += -25;//-18;
		
		if (dragVelocity < -50)
			dragVelocity = -50;
/*		++currentLocation;
		if (currentLocation >= numTrivia)
			currentLocation = 0;//numTrivia-1;
*/	}
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloaT( 411.0f, y+60.0f+16, 64.0f, 64.0f ), "iphone/button_right.tga") ){//&& !dragVelocity) {
		dragVelocity += 25;//18;
		if (dragVelocity > 50)
			dragVelocity = 50;
/*		--currentLocation;
		if (currentLocation < 0)
			currentLocation = numTrivia-1;//0;
*/	}
	
	int width = 305;
	int spacing = 100;//175;
	int size = 160;//180;//110;
	
	iphoneUpdateTriviaAndInstructionsDrags(numTrivia, startPosition, y+25, size*2, size - 53, 10, &dragPosition, &dragVelocity );  //might as well use the same drag functionality in controlmenu
	
	int minSpeed = 20;
	if ((dragVelocity < minSpeed && dragVelocity > 0) || (dragVelocity > -minSpeed && dragVelocity < 0))
	{
		//Com_Printf("(dragPosition-startPosition)\%480 = %i\n", (dragPosition-startPosition)%480);
		
		if (abs((dragPosition-startPosition)%(width+spacing)) < 25)
		{
			dragVelocity = 0;
			int tx = dragPosition - startPosition;
			int n = tx/(width+spacing);
			dragPosition = n*(width+spacing) + startPosition;
			
		}
		else
		{
			if (dragVelocity < 0)
				dragVelocity = -minSpeed;
			else
				dragVelocity = minSpeed;
		}
		//if ((dragPosition-startPosition)%480 == 0)
		//	dragVelocity = 0;
	}	
	/*
	if (dragVelocity == 0 )
	{
		int diff = dragPosition - startPosition;
		int n = diff/(width+spacing);
		int nodePosition = n*(width+spacing);
		
		if ( diff != nodePosition )
			dragVelocity++;
	}*/
	
	//int upperLimit = startPosition + (numTrivia-1)*480;
	int lowerLimit = startPosition - (numTrivia-1)*(width+spacing);
	
	
	
	
	
//	int x = 0;
	y = 50;
	int w = 305;
	int h = 250;
	colour4_t color = { 0, 0, 0, 255 };
	for (int i = 0; i < numTrivia; ++i)
	{/*
		if (i < currentLocation - 1)
			continue;
		if (i > currentLocation + 1)
			continue;*/
//		x = dragPosition + 480*i;
		/*
		if (i == 0 && currentLocation == 0 && dragPosition < -480)
			x = dragPosition + numTrivia*480;
		else if (i == (numTrivia - 1) && currentLocation == (numTrivia - 1) && dragPosition > -480)
			x = dragPosition - 480;
		*/
		/*
		if ( x > upperLimit )//startPosition + (numTrivia-1)*480 )
			x -= numTrivia*480;
		
		if ( x < lowerLimit )//startPosition - (numTrivia-1)*480 )
			x += numTrivia*480;
		*/
		if (dragPosition > startPosition )//+480)//upperLimit )//startPosition + numTrivia*480)
			dragPosition = lowerLimit;//startPosition;
		
		if (dragPosition < lowerLimit )//startPosition - numTrivia*480)
			dragPosition = startPosition;
		
		x = dragPosition + (width+spacing)*i;
		
		if (x > 480 || x + width < 0)
			continue;
		
		DrawBlendInBox( x, y, w, h, color, 71, 410-70);
		//iphoneDrawTextInBox(RectMake(x, y+16, 16, 16), 410-70-32-16, triviaText[i], RectMake(71, 0, 320, 410-70));
		iphoneDrawArialTextInBox(RectMake(x, y+16, 16, 16), 410-70-32-16, triviaText[i], RectMake(71, 0, 320, 410-70));
	}
}
#endif

#if 0
//This is a different implementation of the one above
//gsh
void iphoneTriviaMenu()
{	
	static int currentTrivia = 0;  //this represents which trivia item we are currently supposed to be on
	
	static char triviaText[][1024] = {
		" ORIGINAL WOLF:\n\nThe original release of Wolfenstein 3D only contained 3 Episodes. Episodes 4, 5, and 6 were sold separately as \"The Nocturnal Missions\", and were intended to act as a prequel to the original trilogy of Episodes.  Subsequent releases of the game included all 6 Episodes together.",
		" BJ, THE MAN:\n\nWilliam Joseph Blazkowicz was born August 15, 1911, to Polish immigrants. Blazkowicz was a top spy for the Allied Forces, eventually receiving the Congressional Medal of Honor.",
		" BOX ART:\n\nKen Rieger was the talented illustrator who created the cover art for Spear of Destiny, Wolfenstein3D and Commander Keen.",
		" THE REAL SPEAR:\n\nThe Spear of Destiny, also known as The Spear or The Holy Lance, is believed to have been the weapon used to pierce the side of Jesus Christ at the Crucifixion.  The Spear soon became a symbol of God's favor and those who possessed it were believed to be invincible.",
		" THE SPEAR AND HITLER:\n\nHitler is believed to have acquired the Spear at the beginning of WWII.  Near the end of that war, it is rumored that General Patton discovered the Spear and that the downfall of Germany began that day.  It is fabled that the U.S. was destined to succeed Germany as the New World Power once ownership of the Spear changed hands.",
		" DEATH DIMENSION:\n\nIt is said that Hitler made a pact with the Angel of Death, promising the souls of his Nazi soldiers, to protect the Spear of Destiny.",
		" GOD MODE:\n\nIn the game, if you can access god mode, BJ's eyes will glow with eerie golden light.",
		" A PRINCE:\n\nRobert Prince, wrote the music for Commander Keen, Wolfenstein 3-D, and Doom.",
		" DELAYED DEVELOPMENT:\n\nIt is rumored that work on Spear of Destiny was hindered by the development team's time spent playing Fatal Fury (which Jay Wilbur called 'Fatal Productivity'), and Street Fighter II.",
		" A MATTER OF DEGREES:\n\nJohn Carmack is known for his innovations in 3D graphics and rocketry but what is less known is that this self-taught engineer has no degrees - he dropped out of school after one semester.",
		" THE CARMACK:\n\nJohn Carmack, one of the founders of id Software, preferred to work at a pizza joint and make games rather than go to college.  His favorite programming food is pizza and diet coke.",
		" DREAMING IN DIGITAL:\n\nDoom was reported to cause gamers to dream in pixels and lower productivity.",
		" KEEPING IT IN THE FAMILY:\n\nBJ is Commander Keen's grandfather.",
		" WHAT'S IN A NAME:\n\nThe original name for Doom was \"It's Green and Pissed\"",
		" GIBS:\n\nGibs, pronounced with a hard 'g', is sometimes confused as 'jibs' due to its pronunciation.  The term came from the imagery of flying chicken giblets, or gibs for short.",
		" WOLFENSTEIN 3D:\n\nThe game was originally designed to have more complex gameplay. Though dragging bodies, stealth kills and uniforms were already programmed, it was decided that they slowed down the pace of the game and were removed.",
		" ORIGINAL WOLF:\n\nThe original release of Wolfenstein 3D only contained 3 Episodes. Episodes 4, 5, and 6 were sold separately as \"The Nocturnal Missions\", and were intended to act as a prequel to the original trilogy of Episodes.  Subsequent releases of the game included all 6 Episodes together.",
	};
	//get number of trivia
	int numTrivia = sizeof(triviaText)/(sizeof(char) * 1024);
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	iphoneDrawPicRect( MakeScaledRectFloat( 240.0f-108.0f*9/10.0f, 0.0f, 217.0f*9/10.0f, 50.0f*9/10.0f ), "iphone/header_trivia.tga");
	
	if (BackButton()) {
		menuState = IPM_MORE;
		iphoneStartMainMenu();
		return;
	}
	
	if (MenuButton()) {
		menuState = IPM_MAIN;
		return;
	}
	
	int startPosition = 85;
	
	//update the drags
	static int Position = 85;
	//static int Velocity = 0;
	//static int currentLocation = 0;
	//	int y = 30;
	
	int y = 20;
	int x = 0;
	
	static int moveRight = 0;
	
	//move right or left, depending on button pushed
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 9.0f, y+60.0f+16.0f, 64.0f, 64.0f ), "iphone/button_left.tga") ) {
		moveRight = 0;
		++currentTrivia;
		if (currentTrivia > numTrivia-1) {
			currentTrivia = 1;

		}
	}
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 411.0f, y+60.0f+16.0f, 64.0f, 64.0f ), "iphone/button_right.tga") ) {
		moveRight = 1;
		--currentTrivia;
		if (currentTrivia < 0) {
			currentTrivia = numTrivia-2;
		}
	}
	
	
	int width = 305;
	int spacing = 215;//175;

	int destinationPosition = startPosition - currentTrivia*(width+spacing);
	int nearestTrivia = -(Position - startPosition)/(width+spacing);
	
	//if we are not where we are supposed to be... get us there
	if (Position != destinationPosition)
	{
		Com_Printf("\n\nPosition: %i\n", Position);
		Com_Printf("nearestTrivia: %i\n", nearestTrivia);
		Com_Printf("currentTrivia: %i\n", currentTrivia);
		Com_Printf("destinationPosition: %i\n", destinationPosition);
		
		
		//now we must check for looping situations (where it would be faster to go backwards
	
		//this works great for traveling in a straight line without looping
		
		int oldPosition = Position;
		int deltaPos = destinationPosition - Position;
		/*
		if (deltaPos > 0)
			Position += 50;
		else
			Position -= 50;
		*/
		int moveDistance = 50;
		
		if ( moveRight )//!crossedOver )//currentTrivia - nearestTrivia >= 0)//abs(currentTrivia - nearestTrivia) > numTrivia/2)
		{
			//if (deltaPos < 0)
			//	moveDistance *= -1;
			
			Position += moveDistance;
					
			if (destinationPosition < Position && destinationPosition > oldPosition)
				Position = destinationPosition;
		}
		else //if (currentTrivia - nearestTrivia < 0)
		{
			
			//if (deltaPos > 0)
			//	moveDistance *= -1;
			
			Position -= moveDistance;
			
			if (destinationPosition > Position && destinationPosition < oldPosition)
				Position = destinationPosition;
			 
		}
		
		/*
		//check if the current position has crossed over the destination position
		if (destinationPosition < Position && destinationPosition > oldPosition)
			Position = destinationPosition;
		*/
		
		if ( abs(deltaPos) <= 100)
			Position = destinationPosition;
	}
		
	//int upperLimit = startPosition + (numTrivia-1)*480;
	int lowerLimit = startPosition - (numTrivia-1)*(width+spacing);
	
	
	
	
	
	//	int x = 0;
	// y = 50;
	int w = 305 * screenScale;
	int h = 250;
	colour4_t color = { 0, 0, 0, 255 };
	colour4_t colorWhite = { 255, 255, 255, 255 };
	colour4_t colorGrey = { 100, 100, 100, 255 };
	for (int i = 0; i < numTrivia; ++i)
	{
		if (Position > startPosition )//+480)//upperLimit )//startPosition + numTrivia*480)
			Position = lowerLimit;//startPosition;
		
		if (Position < lowerLimit )//startPosition - numTrivia*480)
			Position = startPosition;// - (width+spacing);
		
		float xFloat = Position + (width+spacing)*i;
		float yFloat = 50;
		
		if (xFloat > 480 || xFloat + width < 0)
			continue;
		
		ScalePosition( &xFloat, &yFloat );
		
		x = (int)xFloat;
		y = (int)yFloat;
		
		int boxWidth = (410-70) * screenScale;
		
		DrawBlendInBox( x-2, y-2, w+4, h+4, colorWhite, 71 * screenScale, boxWidth);
		DrawBlendInBox( x-1, y-1, w+2, h+2, colorGrey,  71 * screenScale, boxWidth);
		DrawBlendInBox( x,   y,   w,   h,   color,      71 * screenScale, boxWidth);
		//iphoneDrawTextInBox(RectMake(x, y+16, 16, 16), 410-70-32-16, triviaText[i], RectMake(71, 0, 320, 410-70));
		rect_t boxRect = RectMake(71, 0, 320, 410-70);
		ScaleRect( &boxRect );
		
		iphoneDrawArialTextInBox(RectMake(x, y+16, 16, 16), (410-70-32-16) * screenScale, triviaText[i], boxRect );
	}
}

//----------------------------------
// iphoneMoreMenu
// gsh
//-----------------------------------
void iphoneMoreMenu()
{
	wasDLInstructionsCalledFromEpisodeMenu = 0;
	
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	iphoneDrawPicRect( MakeScaledRectFloat( 240.0f-108*9/10.0f, 0.0f, 217.0f*9/10.0f, 50.0f*9/10.0f ), "iphone/header_more.tga");
	
	float scale =  40 / 32.0;
	
	if (BackButton()) {
		menuState = IPM_MAIN;
		iphoneStartMainMenu();
	}
	
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 240-64*scale, 120, 128*scale, 32*scale ), "iphone/button_trivia.tga")) {
		menuState = IPM_TRIVIA;
	}
	
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 240-64*scale, 170, 128*scale, 32*scale ), "iphone/button_downloads.tga")) {
		menuState = IPM_DOWNLOADINSTRUCTIONS;
	}
}

//----------------------------------
// iphoneDownloadInstructionsMenu
// gsh
//-----------------------------------
void iphoneDownloadInstructionsMenu()
{
	iphoneDrawPicRect( MakeScaledRectFloat( 0.0f, 0.0f, 480.0f, 320.0f ), "iphone/submenus_background_image.tga");
	iphoneDrawPicRect( MakeScaledRectFloat( 240.0f-108*9/10.0f, 0.0f, 217.0f*9/10.0f, 50.0f*9/10.0f ), "iphone/header_downloads.tga");
	
	if (BackButton())
	{
		menuState = IPM_MORE;
		if (wasDLInstructionsCalledFromEpisodeMenu)
			menuState = IPM_EPISODE;	
		
		return;
	}
	
	if (MenuButton()) {
		menuState = IPM_MAIN;
		return;
	}
	/*
	if ( iphoneDrawPicWithTouch( 64, 0, 64, 36, "iphone/button_web.tga" ) ) { 
		currentYesNoBox = ESNO_DOWNLOAD_INSTRUCTIONS;
		iphoneYesNoBox("Website", "This will navigate you to idsoftware.com/iphone/wolf3dInstructions.html.  Continue?");
	}*/
	
	char instructions[] = "\nWolfenstein Platinum allows you to download additional game levels from the Internet."
						  //"  For details, go to:\n\nhttp://www.idsoftware.com/iphone/\nwolf3dInstructions.html";
							"  For details, go to:\n\nhttp://www.idsoftware.com/wolfenstein\n"
							"-3d-classic-platinum/mapinstructions/";
	
	float x = 20;
	float y = 100;
	float w = 480-40;
	float h = 150;
	colour4_t color = { 0, 0, 0, 255 };
	colour4_t colorGrey = { 100, 100, 100, 255 };
	colour4_t colorWhite = { 255, 255, 255, 255 };
	
	x = ( viddef.width * 0.5f ) - ( w / 2.0f );
	float contentHeight = REFERENCE_HEIGHT - 45.0f;
	y = ( contentHeight * 0.5f ) - ( h / 2.0f );
	
	ScalePosition( NULL, &y );

	//draw a background, behind the text, with a white border
	R_Draw_Blend( x-2, y-2, w+4, h+4, colorWhite);
	R_Draw_Blend( x-1, y-1, w+2, h+2, colorGrey);
	R_Draw_Blend( x,   y,   w,   h,   color);

	//draw arial text
	rect_t boxRect = RectMake(10, 0, 480-40, 320);
	ScaleRect( &boxRect );
	
	iphoneDrawArialTextInBox(RectMake(x, y+16, 16, 16), 400, instructions, boxRect );
	
	//check for touch events on the text itself
	int	r = TouchReleased( x, y, w, h );
	if ( r ) {
		currentYesNoBox = YESNO_DOWNLOAD_INSTRUCTIONS;
		iphoneYesNoBox("Website", "This will navigate you to idsoftware.com/wolfenstein-3d-classic-platinum/mapinstructions/  Continue?");
	}
	if ( TouchDown( x, y, w, h ) ) {
		colour4_t colorLight = { 255, 255, 255, 64 };
		R_Draw_Blend( x, y, w, h, colorLight );
	}
}

#endif

//gsh
extern void iphoneSelectMapMenu();
#if SPEARSTOREKIT
extern void iphoneStoreKit();
#endif

void iphoneDrawMenus() {
//	iphoneDrawPic( 0, 0, 480, 320, "iphone/background_main_sepia.tga" );
	//iphoneDrawPicRect( MakeScaledRectFloat( 0, 0, 480, 320 ), "iphone/background_main_hued.tga" ); //gsh
	
	switch ( menuState ) {
		case IPM_MAIN: iphoneMainMenu(); break;
		case IPM_SKILL: /* iphoneSkillMenu(); */ break;
		case IPM_EPISODE: /* iphoneScrollingEpisodeMenu(); */ break;//gsh //iphoneEpisodeMenu(); break;
		case IPM_MAPS: /* iphoneScrollingMapMenu(); */ break; //gsh iphoneMapMenu(); break;
	//	case IPM_MAPSELECTOR: iphoneSelectMapMenu(); break;  //gsh
		case IPM_CONTROLS: /* iphoneControlMenu(); */ break;
		case IPM_INTERMISSION: iphoneIntermission(); break;
		case IPM_VICTORY: iphoneVictory(); break;
		case IPM_AUTOMAP: iphoneAutomap(); break;
		case IPM_STATS: iphoneStats(); break;
		case IPM_HUDEDIT: iphoneHudEditFrame(); break;//gsh HudEditFrame(); break;
		case IPM_DOWNLOADPROGRESS: /* iphoneDownloadMenu(); */ break; //gsh
#if SPEARSTOREKIT
		case IPM_STOREKIT: iphoneStoreKit(); break; //gsh
#endif
//		case IPM_LOADING: iphoneLoadingMenu(); //gsh
		case IPM_TRIVIA: /* iphoneTriviaMenu(); */ break;//gsh
		case IPM_MORE: /* iphoneMoreMenu(); */ break;//gsh
		case IPM_DOWNLOADINSTRUCTIONS: /* iphoneDownloadInstructionsMenu(); */ break; //gsh
		default: break;
	}
	
	// Draw letterbox bars. Some of the items in the level select menu spill beyond the scaled
	// viewport, this is a quick way to hide them.
	float contentHeight = REFERENCE_HEIGHT * screenScale;
	float boxWidth = viddef.width;
	float boxHeight = ( viddef.height - contentHeight ) / 2.0f;
	R_DrawBox( 0, 0, boxWidth, boxHeight, 0xFF000000 );
	// The lower box is adjusted to cover rows of pixels lost to floating-point truncation.
	R_DrawBox( 0, boxHeight + contentHeight - 1, boxWidth, boxHeight + 2, 0xFF000000 );
}


