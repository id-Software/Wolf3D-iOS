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

/*
mac wolf wall mapping:
cp 000.5551 ../../../base/walls/000.5551
cp 001.5551 ../../../base/walls/010.5551
cp 002.5551 ../../../base/walls/004.5551
cp 003.5551 ../../../base/walls/006.5551
cp 004.5551 ../../../base/walls/022.5551
cp 005.5551 ../../../base/walls/018.5551
cp 006.5551 ../../../base/walls/020.5551
cp 007.5551 ../../../base/walls/044.5551
cp 008.5551 ../../../base/walls/014.5551
cp 009.5551 ../../../base/walls/008.5551
cp 010.5551 ../../../base/walls/032.5551
cp 011.5551 ../../../base/walls/038.5551
cp 014.5551 ../../../base/walls/040.5551
cp 015.5551 ../../../base/walls/041.5551
cp 016.5551 ../../../base/walls/030.5551
cp 017.5551 ../../../base/walls/028.5551
cp 018.5551 ../../../base/walls/026.5551
cp 019.5551 ../../../base/walls/046.5551
cp 020.5551 ../../../base/walls/050.5551
cp 021.5551 ../../../base/walls/078.5551
cp 022.5551 ../../../base/walls/066.5551
cp 025.5551 ../../../base/walls/056.5551
cp 026.5551 ../../../base/walls/058.5551
cp 027.5551 ../../../base/walls/086.5551
cp 028.5551 ../../../base/walls/090.5551
cp 029.5551 ../../../base/walls/068.5551
cp 030.5551 ../../../base/walls/126.5551
cp 031.5551 ../../../base/walls/132.5551
cp 032.5551 ../../../base/walls/133.5551
cp 033.5551 ../../../base/walls/130.5551
cp 034.5551 ../../../base/walls/128.5551

 // SOD? cp 012.5551 ../../../base/walls/000.5551
 // SOD? cp 013.5551 ../../../base/walls/000.5551
 // SOD? cp 023.5551 ../../../base/walls/000.5551
 // SOD? cp 024.5551 ../../../base/walls/000.5551
 
 All odd numbers except 31, 41, 43, and 133 can be just darkened down
 

cp 000.tga ../../newWalls/000.tga
cp 001.tga ../../newWalls/010.tga
cp 002.tga ../../newWalls/004.tga
cp 003.tga ../../newWalls/006.tga
cp 004.tga ../../newWalls/022.tga
cp 005.tga ../../newWalls/018.tga
cp 006.tga ../../newWalls/020.tga
cp 007.tga ../../newWalls/044.tga
cp 008.tga ../../newWalls/014.tga
cp 009.tga ../../newWalls/008.tga
cp 010.tga ../../newWalls/032.tga
cp 011.tga ../../newWalls/038.tga
cp 014.tga ../../newWalls/040.tga
cp 015.tga ../../newWalls/041.tga
cp 016.tga ../../newWalls/030.tga
cp 017.tga ../../newWalls/028.tga
cp 018.tga ../../newWalls/026.tga
cp 019.tga ../../newWalls/046.tga
cp 020.tga ../../newWalls/050.tga
cp 021.tga ../../newWalls/078.tga
cp 022.tga ../../newWalls/066.tga
cp 025.tga ../../newWalls/056.tga
cp 026.tga ../../newWalls/058.tga
cp 027.tga ../../newWalls/086.tga
cp 028.tga ../../newWalls/090.tga
cp 029.tga ../../newWalls/068.tga
cp 030.tga ../../newWalls/126.tga
cp 031.tga ../../newWalls/132.tga
cp 032.tga ../../newWalls/133.tga
cp 033.tga ../../newWalls/130.tga
cp 034.tga ../../newWalls/128.tga
 
 mac wolf weapon mapping:
cp weapon00.5551 ../../../base/sprites/514.5551
cp weapon01.5551 ../../../base/sprites/515.5551
cp weapon02.5551 ../../../base/sprites/516.5551
cp weapon03.5551 ../../../base/sprites/517.5551
cp weapon04.5551 ../../../base/sprites/518.5551
cp weapon05.5551 ../../../base/sprites/519.5551
cp weapon06.5551 ../../../base/sprites/520.5551
cp weapon07.5551 ../../../base/sprites/521.5551
cp weapon08.5551 ../../../base/sprites/522.5551
cp weapon09.5551 ../../../base/sprites/523.5551
cp weapon10.5551 ../../../base/sprites/524.5551
cp weapon11.5551 ../../../base/sprites/525.5551
cp weapon12.5551 ../../../base/sprites/526.5551
cp weapon13.5551 ../../../base/sprites/527.5551
cp weapon14.5551 ../../../base/sprites/528.5551
cp weapon15.5551 ../../../base/sprites/529.5551
 
 
mac wolf sprite mapping

brown shirt corpse
cp 039.5551 ~/dev/iphone/wolf3d/base/sprites/099.5551
 
brown plant
cp 136.5551 ~/dev/iphone/wolf3d/base/sprites/013.5551
 
 */
#include <pthread.h> //gsh
#include "../wolfiphone.h"
#include "arialGlyphRects.h" //gsh... cass made a nice fontimage helper.  We might as well use it.

currentMap_t currentMap;

vec3_t	vnull = { 0, 0, 0 };

int		iphoneFrameNum;
int		intermissionTriggerFrame;
int		slowAIFrame;

//gsh
int returnButtonFrameNum = 0;

// console mode
int consoleActive;

// The size of UI elements will be scaled by this value in order to support different
// resolutions, such as Retina displays and the iPad.
float screenScale = 1.0f;

// the native iPhone code should set the following each frame:
int	numTouches;
int	touches[5][2];	// [0] = x, [1] = y in landscape mode, raster order with y = 0 at top
int isTouchMoving = 0; //gsh

// On Retina displays, touch coordinates are still reported at "low resolution". Since the game
// compares touch coordinates directly to the on-screen positions of buttons, we will need to
// scale the touches on Retina displays.
int touchCoordinateScale = 1;

float deviceScale = 1.0f;

deviceOrientation_t deviceOrientation = ORIENTATION_LANDSCAPE_LEFT;

float	tilt;		// -1.0 to 1.0
float	tiltPitch;

#define MAX_TILT_HISTORY 64
float	tiltHistory[MAX_TILT_HISTORY][4];
int		tiltHistoryNum;

// so we can detect button releases
int	numPrevTouches;
int prevTouches[5][2];

texture_t *numberPics[10];

//gsh
texture_t *arialFontTexture;

// Profiling information
CFAbsoluteTime soundTime = 0.0;
CFAbsoluteTime menuTime = 0.0;

char *mugshotnames[ NUM_MUGSHOTS ] =
{
"iphone/FACE1APIC.tga",
"iphone/FACE1BPIC.tga",
"iphone/FACE1CPIC.tga",

"iphone/FACE2APIC.tga",
"iphone/FACE2BPIC.tga",
"iphone/FACE2CPIC.tga",

"iphone/FACE3APIC.tga",
"iphone/FACE3BPIC.tga",
"iphone/FACE3CPIC.tga",

"iphone/FACE4APIC.tga",
"iphone/FACE4BPIC.tga",
"iphone/FACE4CPIC.tga",

"iphone/FACE5APIC.tga",
"iphone/FACE5BPIC.tga",
"iphone/FACE5CPIC.tga",

"iphone/FACE6APIC.tga",
"iphone/FACE6BPIC.tga",
"iphone/FACE6CPIC.tga",

"iphone/FACE7APIC.tga",
"iphone/FACE7BPIC.tga",
"iphone/FACE7CPIC.tga",

"iphone/FACE8APIC.tga",
"iphone/GOTGATLINGPIC.tga",	

"iphone/GODMODEFACE0PIC.tga",
"iphone/GODMODEFACE1PIC.tga",
"iphone/GODMODEFACE2PIC.tga",
};

int damageflash;
int bonusFrameNum;
int attackDirTime[2];

logTime_t	loggedTimes[MAX_LOGGED_TIMES];	// indexed by iphoneFrameNum


// Some HUD element constants
#define HUD_FACE_WIDTH		64.0f
#define HUD_FACE_HEIGHT		80.0f

#define HUD_WEAPON_WIDTH	128.0f
#define HUD_WEAPON_HEIGHT	128.0f

float faceWidth = HUD_FACE_WIDTH;
float faceHeight = HUD_FACE_HEIGHT;

float weaponWidth = HUD_WEAPON_WIDTH;
float weaponHeight = HUD_WEAPON_HEIGHT;

/*
 ==================
 iphoneActivateConsole
 
 ==================
 */
void iphoneActivateConsole() {
	extern float scr_conlines;
	scr_conlines = 0.5f;
	consoleActive = 1;
	
	// draw a frame immediately, because the UIKit takes
	// a long time to bring up the keyboard
	iphoneFrame();
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
 iphoneExecuteConsoleCommand
 
 ==================
 */
void iphoneExecuteCommandLine() {
	char buf[256];
	strcpy( buf, SysIPhoneGetConsoleTextField() );
	
	Com_Printf( "%s\n", buf );
	Cmd_ExecuteString( buf );
	SysIPhoneSetConsoleTextField( "" );
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
		
		pfglTexCoord2f( fcol, frow+myfont->hFrac );
		pfglVertex2i( x, y+scale );	
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow+myfont->hFrac );	
		pfglVertex2i( x+scale, y+scale );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow );					
		pfglVertex2i( x+scale, y );
	}
	
	pfglEnd();
	
	return l * step;
}

//gsh
void iphoneCenterTextWithColor(int x, int y, const char *str, colour4_t color ) {
	
	pfglColor4f(color[0], color[1], color[2], color[3]);
	
	iphoneCenterText(x, y, str);
	
	pfglColor4f(1, 1, 1, 1);
}
/*
 ==================
 iphoneDrawArialText
 left justified arial text
 gsh
 Returns the width in pixels
 ==================
 */
int iphoneDrawArialText( int x, int y, float scale, const char *str ) {
	scale *= screenScale;
	
	/*
	int l = strlen( str );
	int	i;
	font_t *myfont = myfonts[0];
	//	int		scale;
	int		step = 10;
	
	//	scale = 16;
	int left_margin = x;
	 */
	texture_t *gl;
	
	gl = TM_FindTexture( "iphone/arialImageLAL_white-alpha.tga", TT_Pic );
	if( ! gl ) {
		Com_Printf( "Can't find pic: %s\n", "iphone/arialImageLAL_white-alpha.tga" );
		return 0;
	}
	
	R_Bind( gl->texnum );
	
		
//	float	fx = x;
//	float	fy = y;
	
//	int scale = 1;//16;
//	float scale = 0.9f;
	
	pfglBegin( GL_QUADS );
	
	while ( *str ) {
		int i = *str;
		if ( i >= ' ' && i < 128 ) {
			GlyphRect *glyph = &glyphRects[i-32];
			
			// the glyphRects don't include the shadow outline
			float	x0 = ( glyph->x0 - 2 ) / 256.0;
			float	y0 = ( glyph->y0 - 2 ) / 256.0;
			float	x1 = ( glyph->x1 + 3 ) / 256.0;
			float	y1 = ( glyph->y1 + 3 ) / 256.0;
			
			
			float	width = ( x1 - x0 ) * 256 * scale;
			float	height = ( y1 - y0 ) * 256 * scale;
			
//			float	xoff = ( glyph->xoff - 1 ) * scale;
			float	yoff = ( glyph->yoff - 1 ) * scale;
			
			if (i == 'l' || i == 'I' || i == 'h' || i == 'i')
				yoff += 1;
			
			pfglTexCoord2f( x0, y0 );
//			pfglVertex2f( fx + xoff, fy + yoff );
			pfglVertex2i( x, y + yoff);
			
			pfglTexCoord2f( x0, y1);
//			pfglVertex2f( fx + xoff, fy + yoff + height );
			pfglVertex2i( x, y+height + yoff);	
						
			pfglTexCoord2f( x1, y1);
//			pfglVertex2f( fx + xoff + width, fy + yoff + height );
			pfglVertex2i( x+width, y+height + yoff );
			
			pfglTexCoord2f( x1, y0 );
//			pfglVertex2f( fx + xoff + width, fy + yoff );
			pfglVertex2i( x+width, y + yoff);
			
			// with our default texture, the difference is negligable
//			fx += glyph->xadvance * scale;
			x += glyph->xadvance * scale;
			//			fx += ceil(glyph->xadvance);	// with the outline, ceil is probably the right thing
		}
		str++;
	}
	
	pfglEnd();
	
	return x;
}
/*
 ==================
 iphoneCenterArialText
 center justified arial text
 gsh
 Returns the width in pixels
 ==================
 */
int iphoneCenterArialText( int x, int y, float scale, const char *str )
{
	const char *strcopy = str;
	float width = 0;
	while ( *str )
	{
		int i = *str;
		if ( i >= ' ' && i < 128 ) {
			
			GlyphRect *glyph = &glyphRects[i-32];
			width += glyph->xadvance * scale;
		}
		++str;
	}
	
	float scaledX = x - width / 2;
	float scaledY = y;
	ScalePosition( &scaledX, &scaledY );
	
	return iphoneDrawArialText( scaledX, scaledY, scale, strcopy );
}
/*
 ==================
 iphoneDrawArialTextInBox
 center justified arial text
 gsh
 Returns the width in pixels
 ==================
 */
int iphoneDrawArialTextInBox( rect_t paragraph, int lineLength, const char *str, rect_t boxRect ) {
	
	int l = strlen( str );
	int	i;
	
	if (paragraph.x > boxRect.x + boxRect.width)
		return 0;
	if (paragraph.x + lineLength < boxRect.x)
		return 0;
	
	int x = paragraph.x;
	int y = paragraph.y;
//	Com_Printf("y value: %i\n", y);
//	int width = paragraph.width;   //font width
//	int height = paragraph.height; //font height
	
	const int left_margin = x+5;
	int		step = 10;
	
	texture_t *gl;
	
	gl = TM_FindTexture( "iphone/arialImageLAL_white-alpha.tga", TT_Pic );
	if( ! gl ) {
		Com_Printf( "Can't find pic: %s\n", "iphone/arialImageLAL_white-alpha.tga" );
		return 0;
	}
	
	R_Bind( gl->texnum );
	pfglBegin( GL_QUADS );
	
	int lengthOfNextWord = 0;
	float scale = 0.65f;//0.9f;
	
	for ( i = 0 ; i < l ; i++/*, x += step*/ ) { 
		int m = str[i];
		GlyphRect *glyph = &glyphRects[m-32];
		
		float	x0 = ( glyph->x0 - 2 ) / 256.0;
		float	y0 = ( glyph->y0 - 2 ) / 256.0;
		float	x1 = ( glyph->x1 + 3 ) / 256.0;
		float	y1 = ( glyph->y1 + 3 ) / 256.0;
		
		float	width = ( x1 - x0 ) * 256 * scale;
		float	height = ( y1 - y0 ) * 256 * scale;
		
		//			float	xoff = ( glyph->xoff - 1 ) * scale;
		float	yoff = ( glyph->yoff - 1 ) * scale;
		
		
		//int	row, col; 
		//float frow, fcol;
		int	num = str[i];
		
		//check when to new-line
		if ( num == ' ' ) {
			float w = 0;
			int n = i + 1;
			while ( str[n] != ' ' && str[n] != '\0' && str[n] != '\n') {
				//++n;
				int m = str[n];
				GlyphRect *glyph2 = &glyphRects[m-32];
				w += glyph2->xadvance * scale;
				++n;
			}
			lengthOfNextWord = n - i - 1;
//			Com_Printf("length of word: %i\n", n - i - 1);
//			Com_Printf("length of word pixels: %f\n",w);
			
			if ( w + x > lineLength + left_margin ) {
				y += 30*scale;
				x = left_margin;
			}
			else
				x += 10*scale;
			//whil
			//x += 10*scale;
			continue;
		}
		if (num == '\n') {
			y += 30*scale;
			x = left_margin;
			continue;
		}/*
		if (x + glyph->xadvance * scale > left_margin + lineLength) {
			y += 30*scale;
			x = left_margin;// + width;
		}*/
		
		//check rendering boundaries
		if (x < boxRect.x+10) {
			x += glyph->xadvance * scale + 1;
			continue;
		}
		if (x + glyph->xadvance * scale > boxRect.x + boxRect.width) {
			x += glyph->xadvance * scale + 1;
			continue;
		}
		
		if (y > boxRect.y + boxRect.height)
			break;
				
		pfglTexCoord2f( x0, y0 );
		pfglVertex2i( x, y + yoff);
		
		pfglTexCoord2f( x0, y1);
		pfglVertex2i( x, y+height + yoff);
		
		pfglTexCoord2f( x1, y1);
		pfglVertex2i( x+width, y+height + yoff );
		
		pfglTexCoord2f( x1, y0 );
		pfglVertex2i( x+width, y + yoff);
		
		x += glyph->xadvance * scale + 1;
		
	}
	
	pfglEnd();
	
	return l * step;
}



int iphoneDrawText( int x, int y, int width, int height, const char *str ) {
	int l = strlen( str );
	int	i;
	font_t *myfont = myfonts[0];
//	int		scale;
	int		step = 10;
	
//	scale = 16;
	int left_margin = x;
	
	R_Bind( myfont->texfont->texnum );
	//R_Bind(arialFontTexture->texnum );
	pfglBegin( GL_QUADS );
	
	for ( i = 0 ; i < l ; i++, x += step ) { 
		int	row, col; 
		float frow, fcol;
		int	num = str[i];
		
		if ( num == ' ' ) {
			continue;
		}
		if (num == '\n') {
			y += height;
			x = left_margin;
		}
		
		row = (num >> 4) - 2;
		col = num & 15;
		
		frow = row * myfont->hFrac;
		fcol = col * myfont->wFrac;
		
		pfglTexCoord2f( fcol, frow );							
		pfglVertex2i( x, y );
		
		pfglTexCoord2f( fcol, frow+myfont->hFrac );
		pfglVertex2i( x, y+height );	
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow+myfont->hFrac );	
		pfglVertex2i( x+width, y+height );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow );					
		pfglVertex2i( x+width, y );
				
	}
	
	pfglEnd();
	
	return l * step;
}

/*
 ==================
 iphoneDrawTextWithColor
 gsh
 ==================
 */
void iphoneDrawTextWithColor( rect_t rect, const char *str, float colors[4] ) {

	pfglColor4f(colors[0], colors[1], colors[2], colors[3]);
	iphoneDrawText(rect.x, rect.y, rect.width, rect.height, str);
	pfglColor4f(1, 1, 1, 1);
}

/*
 ==================
 iphoneDrawMapName
 gsh
 ==================
 */
void iphoneDrawMapName( rect_t rect, const char *str ) {
	
	
	rect.y += 25;
	rect.x += 110;//80;
	
	rectFloat_t rectFloat = MakeScaledRectFloat( rect.x, rect.y, rect.width, rect.height );
	/*
	float colors[4] =  { 0, 0, 0, 1 };
	iphoneDrawTextWithColor(RectMake(rect.x+1, rect.y+1, rect.width, rect.height), str, colors);
	iphoneDrawTextWithColor(RectMake(rect.x+2, rect.y+2, rect.width, rect.height), str, colors);
	
	colors[0] = 225.0f/255;
	colors[1] = 166.0f/255;
	iphoneDrawTextWithColor(rect, str, colors);
	*/
	
	pfglColor4f(0, 0, 0, 1);
	iphoneDrawArialText(rectFloat.x + 1, rectFloat.y + 1, 0.9f, str);
	iphoneDrawArialText(rectFloat.x + 2, rectFloat.y + 2, 0.9f, str);
	pfglColor4f(225.0f/255, 166.0f/255, 0, 1);
	pfglColor4f(225.0f/255, 242.0f/255, 0, 1);
	iphoneDrawArialText(rectFloat.x, rectFloat.y, 0.9f, str);
	pfglColor4f(1, 1, 1, 1);
}

/*
 ==================
 iphoneDrawTextInBox
 gsh
 Returns the width in pixels
 ==================
 */
int iphoneDrawTextInBox( rect_t paragraph, int lineLength, const char *str, rect_t boxRect ) {
	int l = strlen( str );
	int	i;
	font_t *myfont = myfonts[0];
	
	int x = paragraph.x;
	int y = paragraph.y;
	int width = paragraph.width;   //font width
	int height = paragraph.height; //font height
	
	int left_margin = x;
	int		step = 10;
	
	R_Bind( myfont->texfont->texnum );
	pfglBegin( GL_QUADS );
	
	int lengthOfNextWord = 0;
	
	for ( i = 0 ; i < l ; i++, x += step ) { 
		int	row, col; 
		float frow, fcol;
		int	num = str[i];
		
		//check when to new-line
		if ( num == ' ' ) {
			int n = i+1;
			while (str[n] != ' ' && str[n] != '\0' && str[n] != '\n') { 
				++n;
			}
			lengthOfNextWord = n - i;// - 1;
			if (x + lengthOfNextWord*step > left_margin + lineLength) {
				y += height;
				x = left_margin;
			}
			continue;
		}
		if (num == '\n') {
			y += height;
			x = left_margin;
			continue;
		}
		if (x + width > left_margin + lineLength) {
			y += height;
			x = left_margin + width;
		}
		
		//check rendering boundaries
		if (x < boxRect.x)
			continue;
		if (x + width > boxRect.x + boxRect.width)
			continue;
		if (y < boxRect.y)
			continue;
		if (y + height > boxRect.y + boxRect.height)
			continue;
		
		row = (num >> 4) - 2;
		col = num & 15;
		
		frow = row * myfont->hFrac;
		fcol = col * myfont->wFrac;
		
		pfglTexCoord2f( fcol, frow );							
		pfglVertex2i( x, y );
		
		pfglTexCoord2f( fcol, frow+myfont->hFrac );
		pfglVertex2i( x, y+height );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow+myfont->hFrac );	
		pfglVertex2i( x+width, y+height );
		
		pfglTexCoord2f( fcol+myfont->wFrac, frow );					
		pfglVertex2i( x+width, y );
	}
	
	pfglEnd();
	
	return l * step;
}

/*
 ==================
 ScaleToScreen
 
 Scales a value for the current resolution.
 ==================
 */
void ScaleToScreen( int * value ) {
	if ( value ) {
		*value *= screenScale;
	}
}

/*
 ==================
 ScalePosition
 
 Scales an x, y position for the current resolution.
 ==================
 */
void ScalePosition( float * x, float * y ) {
	if ( x ) {
		*x *= screenScale;
		*x += ( viddef.width - ( REFERENCE_WIDTH * screenScale ) ) / 2.0f; 
	}
	
	if ( y ) {
		*y *= screenScale;
		*y += ( viddef.height - ( REFERENCE_HEIGHT * screenScale ) ) / 2.0f;
	}
}

/*
 ==================
 ScalePositionInt
 
 Scales an x, y position for the current resolution.
 ==================
 */
void ScalePositionInt( int * x, int * y ) {
	if ( x ) {
		*x *= screenScale;
		*x += ( viddef.width - ( REFERENCE_WIDTH * screenScale ) ) / 2.0f; 
	}
	
	if ( y ) {
		*y *= screenScale;
		*y += ( viddef.height - ( REFERENCE_HEIGHT * screenScale ) ) / 2.0f;
	}	
}

/*
 ==================
 ScalePositionAndSize
 
 Scales an x, y, width, and height set for the current resolution.
 ==================
 */
void ScalePositionAndSize( float * x, float * y, float * w, float * h ) {	
	ScalePosition( x, y );
	
	if ( w ) {
		*w *= screenScale;
	}
	
	if ( h ) {
		*h *= screenScale;
	}
}

/*
 ==================
 ScalePositionAndSizeInt
 
 Scales an x, y, width, and height set for the current resolution.
 ==================
 */
void ScalePositionAndSizeInt( int * x, int * y, int * w, int * h ) {	
	ScalePositionInt( x, y );
	
	if ( w ) {
		*w *= screenScale;
	}
	
	if ( h ) {
		*h *= screenScale;
	}
}

/*
 ==================
 ScaleRect
 
 Scales a rect for the current resolution.
 ==================
 */
void ScaleRect( rect_t * rect ) {
	if ( rect ) {
		ScalePositionInt( &(rect->x), &(rect->y) );
		rect->width *= screenScale;
		rect->height *= screenScale;
	}
}


/*
 ==================
 ScaleRectFloat
 
 Scales a rect for the current resolution.
 ==================
 */
void ScaleRectFloat( rectFloat_t * rect ) {
	if ( rect ) {
		ScalePositionAndSize( &rect->x, &rect->y, &rect->width, &rect->height );
	}
}

/*
 ==================
 MakeScaledRectFloat
 
 Produces a scaled rect for the current resolution.
 ==================
 */
rectFloat_t MakeScaledRectFloat( float x, float y, float width, float height ) {
	rectFloat_t rect = { x, y, width, height };
	ScaleRectFloat( &rect );
	return rect;
}

/*
 ==================
 TouchDown
 
 Checks all touches against a square
 ==================
 */
int	TouchDown( int x, int y, int w, int h ) {
	//ScalePositionAndSize( &x, &y, &w, &h );
	
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
	//ScalePositionAndSize( &x, &y, &w, &h );
	
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
		if ( downNow && !isTouchMoving ) {
			Sound_StartLocalSound( "iphone/bdown_01.wav" );
		}
		// wasn't down the previous frame
		return 0;
	}
	
	if ( downNow ) {
		// still down
		return 0;
	}
	
	if ( numTouches == numPrevTouches && !isTouchMoving ) {
		// finger dragged off
		Sound_StartLocalSound( "iphone/baborted_01.wav" );
		return 0;
	}
	
	if ( !isTouchMoving ) {  //gsh, added the if !isTouchMoving check
		// released
		Sound_StartLocalSound( "iphone/baction_01.wav" );
	}
	return 1;
}

/*
 ==================
 iphoneSet2D
 
 ==================
 */
void iphoneSet2D( void ) {
	pfglMatrixMode( GL_MODELVIEW );
    pfglLoadIdentity();
	pfglDisable( GL_DEPTH_TEST );
	
	static bool doBlending = true;
	if ( doBlending ) {
		pfglEnable( GL_BLEND );
	} else {
		pfglDisable( GL_BLEND );
	}
	
	pfglColor4f( 1,1,1,1 );
	
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	iphoneRotateForLandscape();
	pfglOrtho( 0, viddef.width, viddef.height, 0, -99999, 99999 );
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
float	AxisHit( hudPic_t *hud ) {
	int x = hud->x;
	int y = hud->y;
	int width = hud->width;
	int height = hud->width;
	
	//ScalePositionAndSize( &x, &y, &width, &height );
	
	float activeFraction = 0.8f;
	int	i;
	int isXaxis = ( hud != &huds.forwardStick );
	
	if ( hud->hudFlags & HF_DISABLED ) {
		return 0;
	}
	
	// allow the touches to go outside the indicated bounds with clamping
	x += width / 2;
	y += height / 2;
	width *= 2;
	activeFraction *= 0.5f;
	
	// if the axis is not paired with another, leave it skinnier along the axis
	if ( hud == &huds.sideStick && huds.forwardStick.x == hud->x && huds.forwardStick.y == hud->y ) {
		height *= 2;
	}
	if ( hud == &huds.turnStick && huds.forwardStick.x == hud->x && huds.forwardStick.y == hud->y ) {
		height *= 2;
	}
	if ( hud == &huds.forwardStick ) {
		// don't bother checking for a skinny forwardStick
		height *= 2;
	}

	x -= width / 2;
	y -= height / 2;
	
	for ( i = 0 ; i < numTouches ; i++ ) {
		if ( touches[i][0] >= x && touches[i][1] >= y
			&& touches[i][0] < x + width && touches[i][1] < y + height ) {
				float	f;
				if ( isXaxis ) {
					f = (float)( touches[i][0] - x ) / width * 2.0f - 1.0f;
				} else {
					f = (float)( touches[i][1] - y ) / height * 2.0f - 1.0f;
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

void iphoneTiltEvent( float *tilts ) {
	int		i;
	int		j;
	int		c;
	float	sum[3];
	static float prevTime;

	if ( deviceOrientation == ORIENTATION_LANDSCAPE_RIGHT ) {
		tilts[1] = -tilts[1];
		tilts[0] = -tilts[0];
	}

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

void ShowTime() {
	int	i;
	if ( !showTime->value ) {
		return;
	}
	colour4_t frameColor = { 0, 0, 255, 255 };
	colour4_t swapColor = { 255, 0, 0, 255 };
	colour4_t activeColor = { 0, 255, 0, 255 };
	
	for ( i = 1 ; i < 30 ; i++ ) {
		logTime_t *lt = &loggedTimes[(iphoneFrameNum - i ) & (MAX_LOGGED_TIMES-1)];
		logTime_t *ltp = &loggedTimes[(iphoneFrameNum - i - 1 ) & (MAX_LOGGED_TIMES-1)];
		int	frameTime = lt->enterFrame - ltp->enterFrame;
		int	swapTime = lt->afterSwap - lt->beforeSwap;
		int	activeTime = lt->beforeSwap - lt->enterFrame;
		R_Draw_Fill( 0, i * 4, frameTime * 8, 2, frameColor );
		R_Draw_Fill( frameTime * 8 - swapTime * 8, i * 4, swapTime * 8, 2, swapColor );
		R_Draw_Fill( 0, i * 4, activeTime * 8, 2, activeColor );
	}
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
	
	stickTurnValue = ( stickTurnBase->value + stickTurnScale->value * sensitivity->value );
	stickMoveValue = stickMoveBase->value + stickMoveScale->value * sensitivity->value;
	
	usercmd_t *cmd = &Player.cmd;
	memset( cmd, 0, sizeof( *cmd ) );
		   
	if ( TouchDown( huds.fire.x, huds.fire.y, huds.fire.width, huds.fire.height ) ) {
		cmd->buttons |= BUTTON_ATTACK;
	}
	if ( tiltFire->value > 0 && tiltPitch < tiltFire->value ) {
		cmd->buttons |= BUTTON_ATTACK;
	}
	
#ifdef VOLUMHACK
	//gsh... attempting a left/right click attack
	if ( volumeFireUp->value ) {
		
		if ((int)volumeFireUpSetting->value)
			cmd->buttons |= BUTTON_ATTACK;
		else
			cmd->buttons |= BUTTON_ALTERNATE_ATTACK;
		
		Cvar_Set("volumeFireUp", "0");
	}
	else if ( volumeFireDown->value ) {
		
		if ((int)volumeFireDownSetting->value)
			cmd->buttons |= BUTTON_ATTACK;
		else
			cmd->buttons |= BUTTON_ALTERNATE_ATTACK;
		
		Cvar_Set("volumeFireDown", "0");
	}
#endif

	int weaponSwitchWidth = weaponWidth * 0.625f;   

	// tapping the weapon issues the nextWeapon impulse
	if ( TouchReleased( (viddef.width / 2) - ( weaponSwitchWidth / 2 ),
						viddef.height - faceHeight - ( weaponHeight / 2 ),
						weaponSwitchWidth,
						weaponHeight / 2 ) ) {
		cmd->buttons |= BUTTON_CHANGE_WEAPON;
	}
	
	float forwardAxisHit = AxisHit( &huds.forwardStick );
	float sideAxisHit = AxisHit( &huds.sideStick );
	float turnAxisHit = AxisHit( &huds.turnStick );
	
	static bool printSticks = false;
	
	if ( printSticks ) {
		printf( "Forward: %.4f \nSide: %.4f\nTurn: %.4f\n", forwardAxisHit, sideAxisHit, turnAxisHit );
	}
	
	cmd->forwardmove = -stickMoveValue * forwardAxisHit;
	cmd->sidemove = stickMoveValue * sideAxisHit;
	Player.position.angle += -stickTurnValue * turnAxisHit * tics;

	// accelerometer tilting
	cmd->sidemove += tiltMove->value * DeadBandAdjust( tilt, tiltDeadBand->value );
	
	
	// Multiply by 0.5 and tics because the original calculation was for 30Hz.
	// The multiplication allows this calculation to scale with different framerates while
	// maintaining the same behavior at 30Hz.
	Player.position.angle -= tiltTurn->value * DeadBandAdjust( tilt, tiltDeadBand->value ) * 0.5f * tics;
	
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
//gsh TODO: change hud alphas
//TODO: make this cvar setable and adjustable in settings menu
//float alphaValueForHudControls = 1;//0.5f;
void iphoneHighlightPicNumWhenTouched( int x, int y, int w, int h, int glTexNum ) {
	if ( TouchDown( x, y, w, h ) ) {
		pfglColor4f(1,1,1,1);
	} else {
		pfglColor4f(1,1,1,hudAlpha->value);//0.5);
	}
	iphoneDrawPicNum( x, y, w, h, glTexNum );
	pfglColor4f(1,1,1,1);
}

/*
 ==================
 iphoneDrawWeapon
 
 ==================
 */
void iphoneDrawWeapon( void ) {
	char name[ 32 ];
	texture_t *tex;	
	float w = 128 * screenScale;
	float h = 128 * screenScale;
	float x = (viddef.width - w ) / 2.0f;
	float y = viddef.height - ( HUD_FACE_HEIGHT * screenScale ) - h;
	int	frame;
	
	weaponWidth = w;
	weaponHeight = h;
	
	if ( gunFrame->value ) {
		// screenshots look better with the muzzle flash
		frame = Player.weapon * 4 + gunFrame->value + SPR_KNIFEREADY;
	} else {
		frame = Player.weapon * 4 + Player.weaponframe + SPR_KNIFEREADY;
	}
	
	my_snprintf( name, sizeof( name ), "%s/%d.tga", spritelocation, frame);
	tex = TM_FindTexture( name, TT_Pic );

	R_Bind( tex->texnum );

	pfglBegin( GL_TRIANGLE_STRIP );
	
	pfglTexCoord2f( 0.01f, 0.01f );	pfglVertex2f( x, y );
	pfglTexCoord2f( 0.01f, 0.99f );	pfglVertex2f( x, y + h );
	pfglTexCoord2f( 0.99f, 0.01f );	pfglVertex2f( x + w, y );
	pfglTexCoord2f( 0.99f, 0.99f );	pfglVertex2f( x + w, y + h );
	
	pfglEnd();
}

/*
 ==================
 iphoneDrawNumber
 
 x is the midpoint, y is the top
 ==================
 */
void iphoneDrawNumber( int x, int y, int number, int charWidth, int charHeight ) {	
	//ScalePositionAndSize( &x, &y, &charWidth, &charHeight );
	
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
		pfglBegin( GL_TRIANGLE_STRIP );
		
		pfglTexCoord2f( 0, 0 );	pfglVertex2i( x, y );
		pfglTexCoord2f( 0, 1 );	pfglVertex2i( x, y+charHeight );
		pfglTexCoord2f( 1, 0 );	pfglVertex2i( x+charWidth, y );
		pfglTexCoord2f( 1, 1 );	pfglVertex2i( x+charWidth, y+charHeight );
		
		pfglEnd();
		x += charStep;
	}
}


/*
 ==================
 iphoneDrawFace
 
 ==================
 */
void iphoneDrawFace() {
	int i;
	float w = HUD_FACE_WIDTH * screenScale;
	float h = HUD_FACE_HEIGHT * screenScale;
	const char *pic;
	const int halfScreenWidth = viddef.width / 2.0f;

	faceWidth = 128.0f * screenScale;
	float halfFaceWidth = faceWidth / 2.0f;
	faceHeight = 80.0f * screenScale;

	float keyY = 72.0f * screenScale;
	float keyWidth = 32.0f * screenScale;
	float keyHeight = 64.0f * screenScale;
	
	float damageWidth = 40.0f * screenScale;
	
	// solid background
	iphoneDrawPicFloat( halfScreenWidth - halfFaceWidth, viddef.height - faceHeight, faceWidth, faceHeight, "iphone/status_hud.tga" );

	// keys on the side
	if( Player.items & ITEM_KEY_1 ) {
		iphoneDrawPicFloat( halfScreenWidth - halfFaceWidth, viddef.height - keyY, keyWidth, keyHeight, "iphone/gold_key.tga" );
	}
	
	if( Player.items & ITEM_KEY_2 ) {
		iphoneDrawPicFloat( halfScreenWidth + halfFaceWidth - keyWidth, viddef.height - keyY, keyWidth, keyHeight, "iphone/silver_key.tga" );
	}
	
	
	// decide on the face
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
		if( Player.face_gotgun )
		{
			pic = mugshotnames[ 22 ];
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
			
			//gsh
			if ((Player.flags & FL_GODMODE))
				pic = mugshotnames[ 23+Player.faceframe ];
		}
	}
	else
	{
		pic = mugshotnames[ 21 ];
	}

	iphoneDrawPicFloat( halfScreenWidth - w / 2.0f, (float)(viddef.height) - faceHeight, w, h, pic );
		
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
			iphoneDrawPicFloat( halfScreenWidth - halfFaceWidth,
								(float)(viddef.height) - faceHeight,
								damageWidth, faceHeight, "iphone/L_damage.tga" );
		} else {
			iphoneDrawPicFloat( halfScreenWidth + halfFaceWidth - damageWidth,
								(float)viddef.height - faceHeight,
								damageWidth, faceHeight, "iphone/R_damage.tga" );
		}
		pfglColor4f( 1, 1, 1, 1 );
	}
	
	float healthWidth = 16 * screenScale;
	float healthHeight = 16 * screenScale;
	
	// draw the tiny health numbers on top of the face
	iphoneDrawNumber( halfScreenWidth, viddef.height - healthHeight, Player.health, healthWidth, healthHeight );
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
	
	--notifyFrameNum; //gsh
	
//	pfglColor4f( 1, 1, 1, f );
	//iphoneCenterText( 240, 5, notifyText );
//	iphoneDrawArialText( 200, 20, 0.7f, notifyText );  //gsh
	pfglColor4f( 0, 0, 0, f );
	iphoneCenterArialText( 240+1, 20+1, 0.8f, notifyText); //gsh
	iphoneCenterArialText( 240+2, 20+2, 0.8f, notifyText); //gsh
	iphoneCenterArialText( 240+3, 20+3, 0.8f, notifyText); //gsh
	pfglColor4f( 1, 1, 1, f );
	iphoneCenterArialText( 240, 20, 0.8f, notifyText); //gsh
	pfglColor4f( 1, 1, 1, 1 );
}

/*
 ==================
 iphoneDrawReturnButton
 
 Displays a button that allows the player to return to the map screen.
 But it only displays for a few seconds.
 ==================
 */
void iphoneDrawReturnButton() {
	
	if (returnButtonFrameNum <= 0)
		return;
		
	// display for three seconds, then fade over 0.3
	float f = iphoneFrameNum - returnButtonFrameNum - 80;
	if ( f < 0 ) {
		f = 1.0;
	} else {
		f = 1.0 - f * 0.1f;
		if ( f < 0 ) {
			returnButtonFrameNum = 0;
			return;
		}
	}
	
	//always be semi-transparent
	if ( f > 0.5f )
		f = 0.5f;

	pfglColor4f( 1, 1, 1, f );
	if (iphoneDrawPicRectWithTouch( MakeScaledRectFloat( 240.0f-32.0f, 32.0f, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT ),
									"iphone/button_back.tga")) {//int x, int y, int w, int h, const char *pic )) {
		menuState = IPM_MAPS;
		returnButtonFrameNum = 0;
		
		//if it's a spear map, it needs special attention
		if (currentMap.episode > 5 && currentMap.episode < 10)
		{
			//get the level number
			int levelNum = currentMap.episode*10 + currentMap.map;
			
			if (levelNum == 78 || (levelNum >= 60 && levelNum < 65)) {
				episode->value = 6;
			}
			else if (levelNum == 79 || (levelNum >= 65 && levelNum < 70)) {
				episode->value = 7;
			}
			else if (levelNum >= 70 && levelNum < 76) {
				episode->value = 8;
			}
			else if (levelNum == 76 || levelNum == 77 || levelNum == 80) {
				episode->value = 9;
			}
		}
		
		iphoneStartPreviousMenu();
	}
	pfglColor4f( 1, 1, 1, 1 );
	
	--returnButtonFrameNum;
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
//gsh... note to self:  this is where the controls are drawn
void iphoneDrawHudControl( hudPic_t *hud ) {
	if ( hud->hudFlags & HF_DISABLED ) {
		return;
	}
	iphoneHighlightPicNumWhenTouched( hud->x, hud->y, hud->width, hud->height, hud->glTexNum );
}
//gsh... note to self:  this is where menu/map buttons are drawn
int iphoneDrawHudButton( hudPic_t *hud ) {
	if ( hud->hudFlags & HF_DISABLED ) {
		return 0;
	}
	iphoneDrawHudControl( hud );
	return TouchReleased( hud->x, hud->y, hud->width, hud->height );
}

void iphoneUpdateMapView() {
#ifdef ALLOW_MAP_VIEW_HUD		
	if ( huds.mapView.hudFlags & HF_DISABLED ) {
		return;
	}
	R_Bind( huds.mapView.glTexNum );
	int playerx = Player.position.origin[0] / (float)TILEGLOBAL;
	int playery = Player.position.origin[1] / (float)TILEGLOBAL;
	
	for ( int j = 0; j < 16; j++ ) {
		int y = ( ( iphoneFrameNum << 4 )  + j ) & 63;
		int disty = playery - y;
		GLubyte row[ 64 * 4 ];
		for( int x = 0 ; x < 64; ++x ) {
			int distx = playerx - x;
			if ( distx >= -1 && distx <= 1 && disty == 0 ) {
				row[ x * 4 + 0 ] = 255;
				row[ x * 4 + 1 ] = 255;
				row[ x * 4 + 2 ] = 255;
				row[ x * 4 + 3 ] = 255;
			} else if ( disty >= -1 && disty <= 1 && distx == 0 ) {
				row[ x * 4 + 0 ] = 255;
				row[ x * 4 + 1 ] = 255;
				row[ x * 4 + 2 ] = 255;
				row[ x * 4 + 3 ] = 255;
			} else if ( r_world->tilemap[ x ][ y ] & ( WALL_TILE | PUSHWALL_TILE ) || ! r_world->tileEverVisible[ x ][ y ] ) {
				row[ x * 4 + 0 ] = 0;
				row[ x * 4 + 1 ] = 0;
				row[ x * 4 + 2 ] = 0;
				row[ x * 4 + 3 ] = 0;
			} else {
				row[ x * 4 + 0 ] = 0;
				row[ x * 4 + 1 ] = 0;
				row[ x * 4 + 2 ] = 0;
				row[ x * 4 + 3 ] = 255;
			}
		}
		pfglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 63 - y, 64, 1, GL_RGBA, GL_UNSIGNED_BYTE, row );
	}
#endif	
}

void iphoneDrawMapView() {
#ifdef ALLOW_MAP_VIEW_HUD		
	iphoneDrawHudControl( &huds.mapView );
#endif
}

/*
==================
iphoneFrame

==================
*/
void iphoneFrame() {
	// clear depth buffer
	qglDepthMask( GL_TRUE );
	pfglClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	
	unsigned char blendColor[4] = { 0, 0, 0, 0 };
	
	iphoneFrameNum++;
	loggedTimes[iphoneFrameNum&(MAX_LOGGED_TIMES-1)].enterFrame = Sys_Milliseconds();

	// check for delayed intermission trigger after boss kill
	if ( intermissionTriggerFrame > 0 && iphoneFrameNum >= intermissionTriggerFrame ) {
		iphoneStartIntermission( 0 );
	}
	
	iphoneUpdateMapView();
	
	// toggle / scroll down the console
	Client_Screen_RunConsole();
	
	// fixed frame timing, assume we go 60Hz
	tics = DEFAULT_FRAME_INTERVAL;		// wolf's global rate counter
	
	CFAbsoluteTime soundStartTime = CFAbsoluteTimeGetCurrent();
	Sound_Update( vnull, vnull, vnull, vnull );
	CFAbsoluteTime soundEndTime = CFAbsoluteTimeGetCurrent();
	soundTime = soundEndTime - soundStartTime;
	
	//printf( "Sound_Update took %0.8f seconds.\n", soundTime );
	
	if ( consoleActive ) {	
		iphoneSet2D();	

		Client_Screen_DrawConsole();	
		
		iphoneSavePrevTouches();
		//SysIPhoneSwapBuffers();
		return;
	}
	if ( menuState != IPM_GAME ) {
		iphoneSet2D();
		
		CFAbsoluteTime menuStartTime = CFAbsoluteTimeGetCurrent();
		iphoneDrawMenus( vnull, vnull, vnull, vnull );
		CFAbsoluteTime menuEndTime = CFAbsoluteTimeGetCurrent();
		menuTime = menuEndTime - menuStartTime;
	
		//printf( "iphoneDrawMenus took %0.8f seconds.\n", menuTime );
	
		iphoneSavePrevTouches();
		return;
	}

	//------------------
	// normal gameplay
	//------------------
	
	//this is a hack for "Floor 18, Part II: Death's Door"
	//there's no gold key to leave the first room
	//so we give it to the player here... gsh
	if (currentMap.episode == 8 && !(Player.items & ITEM_KEY_1))
		Player.items |= ITEM_KEY_1;

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
		Door_ProcessDoors_e( &r_world->Doors, tics );
		
		levelstate.time += tics;
	}

	// fill the floor and ceiling
	pfglDisable( GL_BLEND );
	pfglDisable( GL_DEPTH_TEST );
	pfglMatrixMode( GL_MODELVIEW );
    pfglLoadIdentity();
	pfglMatrixMode( GL_PROJECTION );
    pfglLoadIdentity();
	iphoneRotateForLandscape();
	pfglOrtho( 0, viddef.width, viddef.height, 0, -99999, 99999 );
	R_Draw_Fill( 0, 0, viddef.width, viddef.height >> 1, r_world->ceilingColour );
	R_Draw_Fill( 0, viddef.height >> 1, viddef.width, viddef.height, r_world->floorColour );
	
	// setup to draw the 3D world 
	float cur_x_fov, cur_y_fov; // x & y field of view (in degrees)
	float ratio; // viewport width/height
	ratio = (float) viddef.width / (float)viddef.height; // FIXME: move somewhere
	cur_x_fov = 75;
	cur_y_fov = CalcFov( cur_x_fov, (float)viddef.width, (float)viddef.height );
	
	pfglLoadIdentity();
	iphoneRotateForLandscape();
	MYgluPerspective( cur_y_fov - 2.0f, ratio, 0.2f, 64.0f );	// tweak fov in to avoid edge tile clips
	pfglMatrixMode( GL_MODELVIEW );
	pfglLoadIdentity();
	
	pfglRotatef( (float)(90 - FINE2DEG( Player.position.angle )), 0, 1, 0 );
	pfglTranslatef( -Player.position.origin[ 0 ] / FLOATTILE, 0, Player.position.origin[ 1 ] / FLOATTILE );
	
	pfglEnable( GL_DEPTH_TEST );
	
	// find and draw all the walls
	R_RayCast( Player.position, r_world );

	// blend in the sprites
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
			if (currentMap.episode >=9) //gsh
				iphoneStartUserMap( currentMap.episode, currentMap.map, currentMap.skill, NULL );
			else
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
	
	//gsh
	iphoneDrawReturnButton();

	iphoneDrawMapView();

	iphoneDrawFace();	

	iphoneDrawNumber( huds.ammo.x + huds.ammo.width / 2, huds.ammo.y, Player.ammo[AMMO_BULLETS], 48, 48 );
	
	if ( hideControls->value != 1 ) {
		iphoneDrawHudControl( &huds.forwardStick );
		iphoneDrawHudControl( &huds.sideStick );
		iphoneDrawHudControl( &huds.turnStick );
		iphoneDrawHudControl( &huds.fire );
	}
	
	if ( iphoneDrawHudButton( &huds.menu ) ) {
		menuState = IPM_MAIN;
		iphoneStartMainMenu();
	}
	if ( iphoneDrawHudButton( &huds.map ) ) {
		iphoneOpenAutomap();
	}

	Client_Screen_DrawConsole();	

	ShowTilt();		// debug tool
	ShowTime();		// debug tool
	
	iphoneSavePrevTouches();
	
	//SysIPhoneSwapBuffers();	// do the swapbuffers
	
}

void iphoneDrawLoading()
{
	Com_Printf("Draw Loading!\n");
//	unsigned char blendColor[4];
	
	iphoneSet2D();
	
	//draw stuff here
	iphoneDrawText(100, 100, 16, 16, "Drawing Loading!");//, <#int y#>, <#int width#>, <#int height#>, <#const char * str#>)
	
	//SysIPhoneSwapBuffers();	// do the swapbuffers	
}


