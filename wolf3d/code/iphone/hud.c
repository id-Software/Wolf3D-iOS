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

hud_t	huds;

void HudDraw();

void HudWrite();

void HudRead();

hudPic_t	*dragHud;
int			dragX, dragY;

void SetHudPic( hudPic_t *hp, const char *image ) {
	texture_t *gl;
	gl = TM_FindTexture( image, TT_Pic );
	assert( gl );
	hp->glTexNum = gl->texnum;
}

void SetHudSpot( hudPic_t *hp, int x, int y, int w, int h, int flags ) {
	hp->x = x;
	hp->y = y;
	hp->width = w;
	hp->height = h;
	hp->hudFlags = flags;
}

void HudSetTexnums() {
	SetHudPic( &huds.forwardStick, "iphone/up_down.tga" );
	SetHudPic( &huds.sideStick, "iphone/side_2_side.tga" );
	SetHudPic( &huds.turnStick, "iphone/diractional_03.tga" );
	SetHudPic( &huds.fire, "iphone/shoot.tga" );
	SetHudPic( &huds.menu, "iphone/menu.tga" );
	SetHudPic( &huds.map, "iphone/map.tga" );
	SetHudPic( &huds.ammo, "iphone/9.tga" );
	
#ifdef ALLOW_MAP_VIEW_HUD	
	// Special setup for mapView texture since it's a scratch texture.
	texture_t *gl = TM_AllocateTexture( "_mapView" );
	huds.mapView.glTexNum = gl->texnum;
	R_Bind( huds.mapView.glTexNum );
	pfglTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	pfglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	GLubyte *yellow = malloc( 64 * 4 );
	for( int i = 0; i < 64; i++ ) {
		yellow[ i * 4 + 0 ] = 255;
		yellow[ i * 4 + 1 ] = 255;
		yellow[ i * 4 + 2 ] = 0;
		yellow[ i * 4 + 3 ] = 128;
	}
	for( int i = 0; i < 64; i++ ) {
		pfglTexSubImage2D( GL_TEXTURE_2D, 0, 0, i, 64, 1, GL_RGBA, GL_UNSIGNED_BYTE, yellow );
	}
#endif	
}

void HudSetForScheme( int schemeNum ) {
	switch ( schemeNum ) {
		default:
		case 0:
			SetHudSpot( &huds.forwardStick, 0, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.sideStick, 0, 320-100, 100, 100, HF_DISABLED );
			SetHudSpot( &huds.turnStick, 0, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.fire, 480-80, 320-80, 80, 80, 0 );
			SetHudSpot( &huds.menu, 480-64, 0, 64, 32, 0 );
			SetHudSpot( &huds.map, 0, 0, 64, 32, 0 );
			SetHudSpot( &huds.ammo, 480-80, 320-100-44, 80, 44, 0 );
			break;
		case 1:
			SetHudSpot( &huds.forwardStick, 480-100, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.sideStick, 0, 320-100, 100, 100, HF_DISABLED );
			SetHudSpot( &huds.turnStick, 480-100, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.fire, 0, 320-80, 80, 80, 0 );
			SetHudSpot( &huds.menu, 480-64, 0, 64, 32, 0 );
			SetHudSpot( &huds.map, 0, 0, 64, 32, 0 );
			SetHudSpot( &huds.ammo, 480-80, 320-100-44, 80, 44, 0 );
			break;
		case 2:
			SetHudSpot( &huds.forwardStick, 0, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.sideStick, 0, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.turnStick, 480-100, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.fire, 480-80, 0, 80, 80, 0 );
			SetHudSpot( &huds.menu, 0, 32, 64, 32, 0 );
			SetHudSpot( &huds.map, 0, 0, 64, 32, 0 );
			SetHudSpot( &huds.ammo, 480-80, 80, 80, 44, 0 );
			break;
		case 3:
			SetHudSpot( &huds.forwardStick, 480-100, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.sideStick, 480-100, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.turnStick, 0, 320-100, 100, 100, 0 );
			SetHudSpot( &huds.fire, 480-80, 0, 80, 80, 0 );
			SetHudSpot( &huds.menu, 0, 32, 64, 32, 0 );
			SetHudSpot( &huds.map, 0, 0, 64, 32, 0 );
			SetHudSpot( &huds.ammo, 480-80, 80, 80, 44, 0 );
			break;
	}
	
#ifdef ALLOW_MAP_VIEW_HUD	
	SetHudSpot( &huds.mapView, 240-64, 0, 128, 128, 0 );
	huds.mapView.hudFlags |= HF_DISABLED;
#endif	
}

void SnapSticks( hudPic_t *test, const hudPic_t *to ) {
	if ( test->x < to->x + to->width && test->x + test->width > to->x
		&& test->y < to->y + to->height && test->y + test->height > to->y ) {
		test->x = to->x;
		test->y = to->y;
	}
}

/*
 ==================
 HudEditFrame
 
 ==================
 */
void HudEditFrame() {
	int		w;
	int		x;
	colour3_t gray = { 32, 32, 32 };
	
	iphoneSetNotifyText( "Drag the controls" );
	
	if ( numTouches == 0 && numPrevTouches == 1 && dragHud ) {
		// if it was released near the center, make it inactive
		int x = prevTouches[0][0];
		int y = prevTouches[0][1];
		if ( x > 120 && x < 360 && y > 80 && y < 240 ) {
			dragHud->hudFlags |= HF_DISABLED;			
		} else {
			// magnet pull a matchable axis if it is close enough
			if ( dragHud == &huds.forwardStick ) {
				SnapSticks( &huds.sideStick, dragHud );
				SnapSticks( &huds.turnStick, dragHud );				
			} 
			if ( dragHud == &huds.sideStick ) {
				SnapSticks( &huds.forwardStick, dragHud );
			} 
			if ( dragHud == &huds.turnStick ) {
				SnapSticks( &huds.forwardStick, dragHud );
			} 
		}
		Sound_StartLocalSound( "iphone/baction_01.wav" );
		dragHud = NULL;
	}
	
	if ( numTouches == 1 && numPrevTouches == 0 ) {
		// identify the hud being touched for drag
		int x = touches[0][0];
		int y = touches[0][1];
		dragHud = NULL;
		for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
			if ( x >= hud->x && x - hud->x < hud->width && 
				y >= hud->y && y - hud->y < hud->height ) {
				dragHud = hud;
				dragX = dragHud->x - x;
				dragY = dragHud->y - y;
				Sound_StartLocalSound( "iphone/bdown_01.wav" );
				dragHud->hudFlags &= ~HF_DISABLED;
				break;
			}
		}
	}
	
	if ( numTouches == 1 && numPrevTouches == 1 && dragHud ) {
		// adjust the position of the dragHud
		dragHud->x = touches[0][0] + dragX;
		dragHud->y = touches[0][1] + dragY;
		if ( dragHud->x < 0 ) {
			dragHud->x = 0;
		}
		if ( dragHud->x > 480 - dragHud->width ) {
			dragHud->x = 480 - dragHud->width;
		}
		if ( dragHud->y < 0 ) {
			dragHud->y = 0;
		}
		if ( dragHud->y > 320 - dragHud->height ) {
			dragHud->y = 320 - dragHud->height;
		}
	}
	
	// layout the disabled items in the center
	w = 0;
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
		if ( hud->hudFlags & HF_DISABLED ) {
			w += hud->width;
		}
	}
	x = 240 - w / 2;
	
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
		if ( hud->hudFlags & HF_DISABLED ) {
			hud->x = x;
			hud->y = 160-hud->height/2;
			x += hud->width;
		 }
	 }

	// decide where the menu button, map button, and ammo will draw
	
	// solid background color and some UI elements for context
	R_Draw_Fill( 0, 0, 480, 320, gray );	
	
	iphoneDrawFace();
	iphoneDrawNotifyText();
	
	// draw the active items at their current locations
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
		if ( hud->hudFlags & HF_DISABLED ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( hud == &huds.ammo ) {
			iphoneDrawNumber( huds.ammo.x + huds.ammo.width / 2, huds.ammo.y, 99, 48, 48 );
		} else {
			iphoneDrawPicNum( hud->x, hud->y, hud->width, hud->height, hud->glTexNum );
		}
		pfglColor3f( 1, 1, 1 );
	}
	
	// draw the done button
	if ( iphoneDrawPicWithTouch( 240-32, 320-80-32, 64, 32, "iphone/button_back.tga" ) ) {
		menuState = IPM_CONTROLS;
		iphoneSetNotifyText( "" );
	}
 }

//gsh
/*
int boundsLeft = 86;//70;
int boundsRight = 386;//480-72;
int boundsAbove = 80-15;//97;
int boundsUnder = 285-15;//320-15;
*/
//------------------
// ConvertFromImageToScreen
// This converts a point placed in the screen of the iphone image
// to the screen coordinates of the actual iphone
// gsh
//-------------------
void ConvertFromImageToScreen(int *x, int *y, rect_t boundsRect)
{
	int boundsLeft = boundsRect.x;
	int boundsRight = boundsRect.x + boundsRect.width;
	int boundsAbove = boundsRect.y;
	int boundsUnder = boundsRect.y + boundsRect.height;
	
	int valX = *x;//prevTouches[0][0];
	int valY = *y;//prevTouches[0][1];
	
	//int boundsLeft = 64;
	//int boundsRight = 480-64;
	int width = boundsRight - boundsLeft;
	
	//int boundsAbove = 100;
	//int boundsUnder = 320-10;
	int height = boundsUnder - boundsAbove;
	
	valX -= boundsLeft;
	valX = 480*valX/width;
	
	valY -= boundsAbove;
	valY = 320*valY/height;
	
	*x = valX;
	*y = valY;
}

//------------------
// ConvertFromScreenToImage
// This converts a point placed on the screen of the iphone
// to the coordinates of the screen of the iphone image
// gsh
//-------------------
void ConvertFromScreenToImage(int *x, int *y, rect_t boundsRect)
{
	int boundsLeft = boundsRect.x;
	int boundsRight = boundsRect.x + boundsRect.width;
	int boundsAbove = boundsRect.y;
	int boundsUnder = boundsRect.y + boundsRect.height;
	
	int valX = *x;//prevTouches[0][0];
	int valY = *y;//prevTouches[0][1];
	
	//int boundsLeft = 64;
	//int boundsRight = 480-64;
	int width = boundsRight - boundsLeft;
	
	//int boundsAbove = 100;
	//int boundsUnder = 320-10;
	int height = boundsUnder - boundsAbove;
	
	valX = width*valX/480;
	valX += boundsLeft;
	
	valY = height*valY/320;
	valY += boundsAbove;
	
	*x = valX;
	*y = valY;
}

//------------------
// ScaleFromScreenToImage
// This converts to the correct scale to draw with
// gsh
//-------------------
void ScaleFromScreenToImage(int *width, int *height, rect_t boundsRect)
{
	int boundsLeft = boundsRect.x;
	int boundsRight = boundsRect.x + boundsRect.width;
	int boundsAbove = boundsRect.y;
	int boundsUnder = boundsRect.y + boundsRect.height;
	
	int w = *width;
	int h = *height;
	
	//int boundsLeft = 64;
	//int boundsRight = 480-64;
	int iW = boundsRight - boundsLeft;
	
	//int boundsAbove = 100;
	//int boundsUnder = 320-10;
	int iH = boundsUnder - boundsAbove;
	
	w = w*iW/480;
	h = h*iH/320;
	
	*width = w;
	*height = h;
}


/*
 ==================
 iphoneHudEditFrame
 Same as HudEditFrame only different
 gsh
 ==================
 */
void iphoneHudEditFrame() {
	int		w;
	int		x;
	colour3_t gray = { 32, 32, 32 };
	
	int adjustY = 10;
	
	rect_t boundsRect = RectMake(86, 80-adjustY, 300, 205);
	
	
	
	//iphoneSetNotifyText( "Drag the controls" );
	
	if ( numTouches == 0 && numPrevTouches == 1 && dragHud ) {
		// if it was released near the center, make it inactive
		int x = prevTouches[0][0];
		int y = prevTouches[0][1];
		
		//convert x, y coordinates from iphone_image to screen coordinates
		ConvertFromImageToScreen( &x, &y, boundsRect);
		//ConvertFromScreenToImage( &x, &y);
		
		if ( x > 120 && x < 360 && y > 80 && y < 240 ) {
			dragHud->hudFlags |= HF_DISABLED;			
		} else {
			// magnet pull a matchable axis if it is close enough
			if ( dragHud == &huds.forwardStick ) {
				SnapSticks( &huds.sideStick, dragHud );
				SnapSticks( &huds.turnStick, dragHud );				
			} 
			if ( dragHud == &huds.sideStick ) {
				SnapSticks( &huds.forwardStick, dragHud );
			} 
			if ( dragHud == &huds.turnStick ) {
				SnapSticks( &huds.forwardStick, dragHud );
			} 
		}
		Sound_StartLocalSound( "iphone/baction_01.wav" );
		dragHud = NULL;
	}
	
	if ( numTouches == 1 && numPrevTouches == 0 ) {
		// identify the hud being touched for drag
		int x = touches[0][0];
		int y = touches[0][1];
		
		//convert x, y coordinates from iphone_image to screen coordinates
		ConvertFromImageToScreen( &x, &y, boundsRect);
//		ConvertFromScreenToImage( &x, &y);
		
		dragHud = NULL;
		for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
			if ( x >= hud->x && x - hud->x < hud->width && 
				y >= hud->y && y - hud->y < hud->height ) {
				dragHud = hud;
				//int tempDragX = dragHud->x;
				//int tempDragY = dragHud->y;
				//ConvertFromImageToScreen(&, <#int * y#>)
				dragX = dragHud->x - x;
				dragY = dragHud->y - y;
				Sound_StartLocalSound( "iphone/bdown_01.wav" );
				dragHud->hudFlags &= ~HF_DISABLED;
				break;
			}
		}
	}
	
	if ( numTouches == 1 && numPrevTouches == 1 && dragHud ) {
		// adjust the position of the dragHud
		int x = touches[0][0];
		int y = touches[0][1];
		ConvertFromImageToScreen(&x, &y, boundsRect);
		
		dragHud->x = x + dragX;
		dragHud->y = y + dragY;
		if ( dragHud->x < 0 ) {
			dragHud->x = 0;
		}
		if ( dragHud->x > 480 - dragHud->width ) {
			dragHud->x = 480 - dragHud->width;
		}
		if ( dragHud->y < 0 ) {
			dragHud->y = 0;
		}
		if ( dragHud->y > 320 - dragHud->height ) {
			dragHud->y = 320 - dragHud->height;
		}
	}
	
	// layout the disabled items in the center
	w = 0;
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
		if ( hud->hudFlags & HF_DISABLED ) {
			w += hud->width;
		}
	}
	x = 240 - w / 2;
	
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {
		if ( hud->hudFlags & HF_DISABLED ) {
			hud->x = x;
			hud->y = 160-hud->height/2;
			x += hud->width;
		}
	}
	
	// decide where the menu button, map button, and ammo will draw
	
	// solid background color and some UI elements for context
	R_Draw_Fill( 0, 0, 480, 320, gray );	
	
	iphoneDrawPic(0, 0, 480, 320, "iphone/submenus_background_image.tga");
	//iphoneDrawPic(240-108, 0, 217, 50, "iphone/header_advanced.tga");
	iphoneDrawPic(240-108*9/10, 0, 217*9/10, 50*9/10, "iphone/header_advanced.tga");
	
	//gsh
	iphoneDrawPic(10, 60-adjustY, 462, 250, "iphone/iphone_image.tga");
//	iphoneDrawPic(0, 320-240, 480, 240, "iphone/iphone_image.tga");

	
//	iphoneDrawFace();
//	iphoneDrawNotifyText();
	
	
	// draw the active items at their current locations
	for ( hudPic_t *hud = (hudPic_t *)&huds ; hud != (hudPic_t *)(&huds+1) ; hud++ ) {  //nice foreach loop!
		if ( hud->hudFlags & HF_DISABLED ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( hud == &huds.ammo ) {
			int x = huds.ammo.x + huds.ammo.width / 2;
			int y = huds.ammo.y;
			int w = 48;
			int h = 48;
			
			ConvertFromScreenToImage(&x, &y, boundsRect);
			ScaleFromScreenToImage(&w, &h, boundsRect);
			
			iphoneDrawNumber( x, y, 99, w, h );
		} else {
			int x = hud->x;
			int y = hud->y;
			int w = hud->width;
			int h = hud->height;
			
			ConvertFromScreenToImage(&x, &y, boundsRect);
			ScaleFromScreenToImage(&w, &h, boundsRect);
			
			iphoneDrawPicNum( x, y, w, h, hud->glTexNum );
		}
		pfglColor3f( 1, 1, 1 );
	}
	
	
	
	
	
	// draw the back button
	if ( iphoneDrawPicWithTouch( 0, 0, 64, 36, "iphone/button_back.tga" ) ) {
		menuState = IPM_CONTROLS;
		iphoneSetNotifyText( "" );
	}
	
	//x = 64;
	//if (revLand->value)
		x = 480-64;
	// draw the menu button
	if ( iphoneDrawPicWithTouch( x, 0, 64, 36, "iphone/menu.tga" ) ) {
	//if ( iphoneDrawPicWithTouch( 64, 0, 64, 36, "iphone/button_menu_yellow.tga" ) ) {
		menuState = IPM_MAIN;
		iphoneSetNotifyText( "" );
	}
	
	/*
	//------------------------------------
	//Draw volume up/down settings... gsh
	int width = 64*9/10;
	int height = 36;//48*9/10;

	int y = 0;
	y = 0;
	x = 480-width;
	if (revLand->value){
		y = 320-height;
		x = 0;
	}
	
	 //gsh
	if ((int)volumeFireUpSetting->value) {
		if (iphoneDrawPicWithTouch( x, y, width, height, "iphone/button_pistol.tga" ))
			Cvar_Set("volumeFireUpSetting", "0");
	}
	else {
		if (iphoneDrawPicWithTouch( x, y, width, height, "iphone/button_knife.tga" ))
			Cvar_Set("volumeFireUpSetting", "1");
	}
		
	
	 x = 480-width-width-5;
	 if (revLand->value)
		 x = width+5;
	 
	if ((int)volumeFireDownSetting->value) {
		if (iphoneDrawPicWithTouch( x, y, width, height, "iphone/button_pistol.tga" ))
			Cvar_Set("volumeFireDownSetting", "0");
	}
	else {
		if (iphoneDrawPicWithTouch( x, y, width, height, "iphone/button_knife.tga" ))
			Cvar_Set("volumeFireDownSetting", "1");
	}
	//-----------------------------------
	*/
	//draw the instructions
	//if (revLand->value)
	//	iphoneDrawText(x+width+20, 300, 16, 16, "Drag the controls to customize");
	//else
//		iphoneCenterText(240, 300, "Drag each element of the controls to customize");
	iphoneCenterArialText(245, 315, 0.8f, "Drag each element of the controls to customize");
}

