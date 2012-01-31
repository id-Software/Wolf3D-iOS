/*

	Copyright (C) 2004 Michael Liebscher

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
/*
void ReplaceText(char* original, char* insert, int nInsertAt)
{
	int sizeInsert = sizeof(insert);
	int sizeOriginal = sizeof(original); //should be 1024
	
	for (int i = 0; i < sizeInsert; i++)
	{
		original[nInsertAt + i] = insert[i];
	}
	
	
	
}*/

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Client_PrepRefresh( const char *r_mapname )
{
//	char mapname[ 32 ];
	char mapname[ 64 ]; //gsh, decided to allow longer map names

	if( ! r_mapname || ! *r_mapname )
	{
		return;
	}

	if( g_version->value == SPEAROFDESTINY && currentMap.episode >= 6 && currentMap.episode < 10)//added the episode check... gsh)
	{
		spritelocation = SODSPRITESDIRNAME;
	}
	else
	{
		spritelocation = WL6SPRITESDIRNAME;
	}

	my_strlcpy( mapname, r_mapname, sizeof( mapname ) );	// skip "maps/"

	// !@# fix crash bug if you type something short...
	if ( strlen( mapname ) > 4 && !strcmp( mapname + strlen( mapname ) - 4, ".map" ) ) {
		mapname[ strlen( mapname ) - 4 ] = '\0';		// cut off ".map"
	}

	// register models, pics, and skins	
	R_BeginRegistration( mapname );

	if( r_world == NULL )
	{
		return;
	}


	Com_Printf( "Map: %s\n", r_world->mapName ); 
	
	Level_ScanInfoPlane( r_world ); // Spawn items/guards

	Com_Printf( "Spawning Entities\n" );
	PL_Spawn( r_world->pSpawn, r_world ); // Spawn Player

	Com_Printf( "Caching Textures and Sounds\n" );
	Level_PrecacheTextures_Sound( r_world );
	
	// clear any lines of console text
	Con_ClearNotify();
	
	//gsh
	//this is a hack... to save space on the download... we've removed the music
	//so instead we're going to replace the SOD music with wolf3d music here
	//however, we don't have to worry about that now that we package the SOD music
	//with the binary
	/*
	if (currentMap.episode >= 6)   //if it's the SOD levels
	{
		char *source;
		switch (currentMap.episode * 10 + currentMap.map)
		{
			case 60:	source = "ZEROHOUR.ogg";	break;
			case 61:	source = "CORNER.ogg";		break;
			case 62:	source = "DUNGEON.ogg";		break;
			case 63:	source = "ENDLEVEL.ogg";	break;
			case 64:	source = "FUNKYOU.ogg";		break;
			case 65:	source = "HEADACHE.ogg";	break;
			case 66:	source = "HITLWLTZ.ogg";	break;
			case 67:	source = "INTROCW3.ogg";	break;
			case 68:	source = "NAZI_NOR.ogg";	break;
			case 69:	source = "NAZI_OMI.ogg";	break;
			case 70:	source = "NAZI_RAP.ogg";	break;
			case 71:	source = "PACMAN.ogg";		break;
			case 72:	source = "POW.ogg";			break;
			case 73:	source = "PREGNANT.ogg";	break;
			case 74:	source = "ROSTER.ogg";		break;
			case 75:	source = "SALUTE.ogg";		break;
			case 76:	source = "SEARCHN.ogg";		break;
			case 77:	source = "SUSPENSE.ogg";	break;
			case 78:	source = "TWELFTH.ogg";		break;
			case 79:	source = "URAHERO.ogg";		break;
			case 80:	source = "ULTIMATE.ogg";	break;  
			default:
				source = "CORNER.ogg";
		}
		strcpy(levelData.musicName + 6, source);  //the '6' is to get us past the "music/" part of musicName
		levelData.musicName[6 + strlen(source)] = '\0';
	}*/
	
	//Com_Printf("Starting Music Track: %s\n", levelData.musicName);
	//Sound_StartBGTrack( levelData.musicName, levelData.musicName );

	Player.playstate = ex_playing;	
}

int tics;




