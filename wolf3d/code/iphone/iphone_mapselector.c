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
 
 
#include "../wolfiphone.h"
#include <dirent.h>

const char levelNames[][29] = {
"E1 level 1",
"E1 level 2",
"E1 level 3",
"E1 level 4",
"E1 level 5",
"E1 level 6",
"E1 level 7",
"E1 level 8",
"E1 Boss   ",
"E1 Secret ",
"E2 level 1",
"E2 level 2",
"E2 level 3",
"E2 level 4",
"E2 level 5",
"E2 level 6",
"E2 level 7",
"E2 level 8",
"E2 Boss   ",
"E2 Secret ",
"E3 level 1",
"E3 level 2",
"E3 level 3",
"E3 level 4",
"E3 level 5",
"E3 level 6",
"E3 level 7",
"E3 level 8",
"E3 Boss   ",
"E3 Secret ",
"E4 level 1",
"E4 level 2",
"E4 level 3",
"E4 level 4",
"E4 level 5",
"E4 level 6",
"E4 level 7",
"E4 level 8",
"E4 Boss   ",
"E4 Secret ",
"E5 level 1",
"E5 level 2",
"E5 level 3",
"E5 level 4",
"E5 level 5",
"E5 level 6",
"E5 level 7",
"E5 level 8",
"E5 Boss   ",
"E5 Secret ",
"E5 level 1",
"E6 level 2",
"E6 level 3",
"E6 level 4",
"E6 level 5",
"E6 level 6",
"E6 level 7",
"E6 level 8",
"E6 Boss   ",
"E6 Secret ",
"Floor 1:  Tunnels 1     ",
"Floor 2:  Tunnels 2     ",
"Floor 3:  Tunnels 3     ",
"Floor 4:  Tunnels 4     ",
"Floor 5:  Tunnels Boss  ",
"Floor 6:  Dungeons 1    ",
"Floor 7:  Dungeons 2    ",
"Floor 8:  Dungeons 3    ",
"Floor 9:  Dungeons 4    ",
"Floor 10: Dungeons Boss ",
"Floor 11: Castle 1      ",
"Floor 12: Castle 2      ",
"Floor 13: Castle 3      ",
"Floor 14: Castle 4      ",
"Floor 15: Castle 5      ",
"Floor 16: Castle Boss   ",
"Floor 17: Ramparts      ",
"Floor 18: Death Knight  ",
"Floor 19: Secret 1      ",
"Floor 20: Secret 2      ",
"Floor 21: Angel of Death",
};

int dragVelocity = 0;   //velocity for the scrolling maps
int dragPosition = 32;   //position for the scrolling maps

extern int BackButton();
#ifdef SPEARSTOREKIT
extern void GetSpearOfDestiny( int x, int y );
#endif
extern int iphoneDrawPicWithTouchAndColor( int x, int y, int w, int h, const char *pic, colour4_t color );
extern void iphoneStartUserMap( int episodeNum, int mapNum, int skillLevel, char *mapName );


extern void GetMoreLevels( int x, int y );

//==========================================
//  iphoneUpdateDrags()
//  Updates the touches for the scrolling maps
//==========================================
void iphoneUpdateDrags(int numMaps, int skillSides, int height, int spacing, int numUserMaps)
{
	//Update drag/touch info
	int x = 64;
	int w = 480 - skillSides - 64;
	int y = 0;
	int h = 320;
	
	if (TouchDown(x, y, w, h))
	{
		if (!numPrevTouches)
			prevTouches[0][1] = touches[0][1];
		else if (numTouches == 1)
			dragVelocity = prevTouches[0][1] - touches[0][1];
	}
	
	//boundary check the drags
	int num = numMaps+1+6;
	if (g_version->value == SPEAROFDESTINY)
		num = numMaps + 7;
	
	num += numUserMaps + 1;
	
	if (dragPosition < 320 - (height+spacing)*(num) - 20)
		dragPosition = 320 - (height+spacing)*(num) - 20;
	else if (dragPosition > 40)
		dragPosition = 40;
	
	if (dragPosition < 320 - (height+spacing)*(num))
		dragVelocity = -1;
	else if (dragPosition > 32)
		dragVelocity = 1;
	
	//update the drags
	dragPosition -= dragVelocity;
	
	dragVelocity /= screenScale;
	
	//retard the velocity
	if (dragVelocity > 0)
		--dragVelocity;
	else if (dragVelocity < 0)
		++dragVelocity;
	
}

//==========================================
//  iphoneDrawRewards()
//  Draws the rewards on each level
//==========================================
int forceDrawRewards = 0;
void iphoneDrawRewards(int m, int s, int x, int y)
{
	int ch = currentMap.mapFlags[s][m];
	// bit1 = attempted
	// bit2 = completed
	// bit3 = 100% kills
	// bit4 = 100% secrets
	// bit5 = 100% treasure
	
	//ch = 4+8+16+32; //used for testing proper placement

	x += 80;
	int startX = x;

	//x += 66;//375;
	y -= 27;//10;

	int width = 40;//22;
	int height = 40;//22;
	int spacing = -12;
	// draw award shadows
	iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/kills_shadow.tga" );		x += width + spacing;
	iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/secrets_shadow.tga" );		x += width + spacing;
	iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/treasure_shadow.tga" );	x += width + spacing;
	iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/partime_shadow.tga" );	//x += width + 5;
	
	x = startX;
	
	// draw awards
	if ( (ch & MF_KILLS) || forceDrawRewards) {
		iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/kills.tga" );		
	}
	x += width + spacing;
	if ( (ch & MF_SECRETS) || forceDrawRewards) {
		iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/secrets.tga" );
	}
	x += width + spacing;
	if ( (ch & MF_TREASURE) || forceDrawRewards) {
		iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/treasure.tga" );
	}
	x += width + spacing;
	if ( (ch & MF_TIME) || forceDrawRewards) {
		iphoneDrawPicRect( MakeScaledRectFloat( x,y+23, width, height ), "iphone/partime.tga" );
	}
}

//==========================================
//  iphoneDrawUserMaps()
//  This function scans through the custom maps
//  and displays the ones on screen
//==========================================
int iphoneDrawUserMaps(int Yoffset, int height, int spacing, int skillValue)
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
		int y = Yoffset;
		if (g_version->value != SPEAROFDESTINY)  //make space for the purchase SOD button
			y += height + spacing + spacing;
		
		//draw the custom map spacing
		y += height + spacing;
		iphoneDrawPic( 0, y, 480-80, height, "iphone/button_epSOD.tga");
		
			
		while ( ( ep = readdir (dp) ) )
		{
			++numMaps;
			if (numMaps < 3)  //skip the '.' and '..'
				continue;
			
			y += height + spacing;
			Com_Printf("value y:  %i\n", y);

			//TODO: draw the names!
			if ( y >= -height && y < 320 )
			{
			
				//draw maps and touch
				int alpha = 128;
				//colour4_t colorSecret = { 80, 0, 0, alpha };
				colour4_t colorNoTry = { 0, 0, 0, alpha };
			
				if ( iphoneDrawPicWithTouchAndColor( 0, y, 480-80, height, "iphone/menu_bar.tga", colorNoTry) && dragVelocity == 0 )//&& isSecretAvailable )
				{
					//reset the player to new
					PL_NewGame( &Player );
				
					//set episode and map numbers and load map
					int e = 9; //9 will be used to show a custom map
					Cvar_SetValue( episode->name, e );
					iphonePreloadBeforePlay();				//This prevents crashes when loading SOD maps
					iphoneStartUserMap( e, numMaps - 3, skillValue, ep->d_name );
				}
				
				iphoneDrawText( 100, y + 16, 16, 16, ep->d_name );
				//iphoneCenterText( 160, y + 16, ep->d_name );
				
				//draw the rewards
				iphoneDrawRewards(numMaps-2+90, skillValue, 0, y);
			}
		}
		closedir (dp);
		
		--numMaps;
		--numMaps;
	}
	
	Com_Printf("numMaps: %i\n", numMaps);
	
	return numMaps;
}

//==========================================
//  iphoneSelectMapMenu()
//  A doom classic style map selector
//==========================================
void iphoneSelectMapMenu()
{
	int		s;			//skill
	char	str[64];
	
	// highlight the current skill selection
	int skillValue = (int)skill->value;
	if (skillValue < 0)
		skillValue = 0;
	if (skillValue > 3)
		skillValue = 3;
	
	int skillSides = 80;
	
	int	e = 0;		//episode
	
	int m, map;			//mission/map
	int height = 48;
	int spacing = 10;
	int y = 0;
	
	char strMission[80];
	strMission[79] = '\0';
	
	int		numMaps = 60;
	if (g_version->value == SPEAROFDESTINY)
		numMaps = 60 + 21;
	
	iphoneDrawPic(0, 0, 480, 320, "iphone/submenus_background_image.tga");
	
	//check for BackButton touches
	if ( BackButton() ) {
		menuState = IPM_MAIN;
		return;
	}
	
	//we want to use the menu selector bar
	my_snprintf( str, sizeof( str ), "iphone/menu_bar.tga" );
	
	//iterate through the maps... drawing only the visible maps
	for (m = 0; m < numMaps; m++)
	{
		
		//Draws the spacing between each episode
		e = m/10;
		if (e < 6)
		{
			y = dragPosition + ((height + spacing) * e*11);// + ((height + spacing) * (e+1));
			if ( y >= -height && y < 320 )
			{
				my_snprintf( str, sizeof( str ), "iphone/button_ep%i.tga", e+1 );
				iphoneDrawPic( 0, y, 480-80, height, str);
			}
				
			y = dragPosition + ((height + spacing) * m) + ((height + spacing) * (e+1));
		}
		else
		{
			y = dragPosition + ((height + spacing) * 6*11);
			if ( y >= -height && y < 320 )
			{
				my_snprintf( str, sizeof( str ), "iphone/button_epSOD.tga");
				iphoneDrawPic( 0, y, 480-80, height, str);
			}
			y = dragPosition + ((height + spacing) * m) + ((height + spacing) * 7);
		}
		
		//we want to use the menu selector bar
		my_snprintf( str, sizeof( str ), "iphone/menu_bar.tga" );
		
		//only draw the maps that are currently viewable
		if ( y >= -height && y < 320 )
		{
			e = m/10;
			map = m%10;
			
			//color maps
			int ch = currentMap.mapFlags[skillValue][m];
			
			int alpha = 128;
			colour4_t colorSecret = { 80, 0, 0, alpha };
			colour4_t colorNoTry = { 0, 0, 0, alpha };
			colour4_t colorTried = { 80, 80, 0, alpha };
			colour4_t colorCompleted = { 0, 80, 0, alpha };
			colour4_t colorBlink = { 80, 80, 80, alpha };
			
			unsigned char *color = colorNoTry;
			bool isSecretAvailable = true;
			
			if ( map == 9  && !( ch & MF_TRIED ) && e < 6)  //if it's the 9th and not SOD
			{
				color = colorSecret;
				isSecretAvailable = false;
			}
			else if ( (m == 79 || m == 78) && !(ch & MF_TRIED) ) //if it's the secret SOD levels
			{
				color = colorSecret;
				isSecretAvailable = false;
			} else if ( ch & MF_COMPLETED ) {
				color = colorCompleted;
			} else if ( ch & MF_TRIED ) {
				color = colorTried;
			} else {
				color = colorNoTry;
			}
			
			//blink the level you're currently on
			if ( ( iphoneFrameNum & 8 ) && map == currentMap.map && e == currentMap.episode )
				color = colorBlink;
			
			//draw maps and touch
			if ( iphoneDrawPicWithTouchAndColor( 0, y, 480-80, height, str, color) && dragVelocity == 0 && isSecretAvailable )
			{
				//reset the player to new
				PL_NewGame( &Player );
				
				//set episode and map numbers and load map
				Cvar_SetValue( episode->name, e );
				iphonePreloadBeforePlay();				//This prevents crashes when loading SOD maps
				iphoneStartMap( e, map, skillValue );
			}
			
			//draw the rewards
			iphoneDrawRewards(m, skillValue, 0, y);
			
			//draw the episode and map over the selection bar
			my_snprintf( strMission, sizeof( strMission ), "%s", levelNames[m] );
			iphoneDrawText( 100, y + 16, 16, 16, strMission );
//			iphoneCenterText( 160, y + 16, strMission );
		}
	}
	
	int numUserMaps = 0;
	
#if 0
	//buy more episodes button
	if (g_version->value != SPEAROFDESTINY) 
		GetSpearOfDestiny( 0, dragPosition + (height + spacing) * (m+6) );
#endif
	//TODO: Draw user maps
	numUserMaps = iphoneDrawUserMaps(y, height, spacing, skillValue);	
	
	//Update the scrolling drags
	iphoneUpdateDrags(numMaps, skillSides, height, spacing, numUserMaps);

	
	//Draw/choose the skills on the right side of the screen
	for ( s = 0 ; s < 4 ; s++ )
	{
		my_snprintf( str, sizeof( str ), "iphone/button_skill%i.tga", s+1 );
		
		if ( s != (int)skill->value ) {
			pfglColor3f( 0.5, 0.5, 0.5 );
		}
		if ( iphoneDrawPicWithTouch( 480 - skillSides, skillSides*s, skillSides, skillSides, str ) ) {	
			Cvar_SetValue( skill->name, s );
			skillValue = s;
			break;
		}
		pfglColor3f( 1, 1, 1 );
	}
	

	
	//Draw a bar that covers empty space
	iphoneDrawPic(0, 0, 480-80, 32, "iphone/menu_bar.tga");
	
	//levels header
	iphoneDrawPic(240-64, 0, 128, 32, "iphone/header_levels.tga");
	
	BackButton();  //this is kinda cheap...
	//we want the back button to render on top
	//but we also want it to check for touches before the maps
	//so we call this function twice
	//(at the beginning for touches; the end for rendering)
}
