/*

	Copyright (C) 2004-2005 Michael Liebscher
	Copyright (C) 2000-2002 by DarkOne the Hacker

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
 *	wolf_level.c:  Wolfenstein3-D Level management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"


statinfo_t static_wl6[]=
{
	{false, -1},			// puddle          spr1v
	{ true, -1},			// Green Barrel    "
	{ true, -1},			// Table/chairs    "
	{ true, -1},			// Floor lamp      "
	{false, -1},			// Chandelier      "
	{ true, -1},			// Hanged man      "
	{false, pow_alpo},		// Bad food        "
	{ true, -1},			// Red pillar      "
	{ true, -1},			// Tree            spr2v
	{false, -1},			// Skeleton flat   "
	{ true, -1},			// Sink            " (SOD:gibs)
	{ true, -1},			// Potted plant    "
	{ true, -1},			// Urn             "
	{ true, -1},			// Bare table      "
	{false, -1},			// Ceiling light   "
	{false, -1},			// Kitchen stuff   "
	{ true, -1},			// suit of armor   spr3v
	{ true, -1},			// Hanging cage    "
	{ true, -1},			// SkeletoninCage  "
	{false, -1},			// Skeleton relax  "
	{false, pow_key1},		// Key 1           "
	{false, pow_key2},		// Key 2           "
	{ true, -1},			// stuff				(SOD:gibs)
	{false, -1},			// stuff
	{false, pow_food},		// Good food       spr4v
	{false, pow_firstaid},	// First aid       "
	{false, pow_clip},		// Clip            "
	{false, pow_machinegun},// Machine gun     "
	{false, pow_chaingun},	// Gatling gun     "
	{false, pow_cross},		// Cross           "
	{false, pow_chalice},	// Chalice         "
	{false, pow_bible},		// Bible           "
	{false, pow_crown},		// crown           spr5v
	{false, pow_fullheal},	// one up          "
	{false, pow_gibs},		// gibs            "
	{ true, -1},			// barrel          "
	{ true, -1},			// well            "
	{ true, -1},			// Empty well      "
	{false, pow_gibs},		// Gibs 2          "
	{ true, -1},			// flag				"
	{ true, -1},			// Call Apogee		spr7v
	{false, -1},			// junk            "
	{false, -1},			// junk 		   "
	{false, -1},			// junk            "
	{false, -1},			// pots            "
	{ true, -1},			// stove           " (SOD:gibs)
	{ true, -1},			// spears          " (SOD:gibs)
	{false, -1},			// vines		   "
};


statinfo_t static_sod[] =
{
	{false, -1},			// puddle          spr1v
	{ true, -1},			// Green Barrel    "
	{ true, -1},			// Table/chairs    "
	{ true, -1},			// Floor lamp      "
	{false, -1},			// Chandelier      "
	{ true, -1},			// Hanged man      "
	{false, pow_alpo},		// Bad food        "
	{ true, -1},			// Red pillar      "
	{ true, -1},			// Tree            spr2v
	{false, -1},			// Skeleton flat   "
	{ true, -1},			// Sink            " (SOD:gibs)
	{ true, -1},			// Potted plant    "
	{ true, -1},			// Urn             "
	{ true, -1},			// Bare table      "
	{false, -1},			// Ceiling light   "
	{ true, -1},			// Gibs!
	{ true, -1},			// suit of armor   spr3v
	{ true, -1},			// Hanging cage    "
	{ true, -1},			// SkeletoninCage  "
	{false, -1},			// Skeleton relax  "
	{false, pow_key1},		// Key 1           "
	{false, pow_key2},		// Key 2           "
	{ true, -1},			// stuff				(SOD:gibs)
	{false, -1},			// stuff
	{false, pow_food},		// Good food       spr4v
	{false, pow_firstaid},	// First aid       "
	{false, pow_clip},		// Clip            "
	{false, pow_machinegun},// Machine gun     "
	{false, pow_chaingun},	// Gatling gun     "
	{false, pow_cross},		// Cross           "
	{false, pow_chalice},	// Chalice         "
	{false, pow_bible},		// Bible           "
	{false, pow_crown},		// crown           spr5v
	{false, pow_fullheal},	// one up          "
	{false, pow_gibs},		// gibs            "
	{ true, -1},			// barrel          "
	{ true, -1},			// well            "
	{ true, -1},			// Empty well      "
	{false, pow_gibs},		// Gibs 2          "
	{ true, -1},			// flag			   "
	{false, -1},			// Red light
	{false, -1},			// junk            "
	{false, -1},			// junk 		   "
	{false, -1},			// junk            "
	{ true, -1},			// Gibs!
	{ true, -1},			// stove           " (SOD:gibs)
	{ true, -1},			// spears          " (SOD:gibs)
	{false, -1},			// vines		   "
	{ true, -1},			// marble pillar
	{false, pow_25clip},	// bonus 25 clip
	{ true, -1},			// truck
	{false, pow_spear},		// SPEAR OF DESTINY!
};

statinfo_t *statinfo = static_wl6;
int num_statics = sizeof( static_wl6 ) / sizeof( static_wl6[ 0 ] );

texture_t *wallTextures[1000];
texture_t *spriteTextures[1000];

PRIVATE W16	cachedGuard = 0;
PRIVATE W16	cachedOfficer = 0;
PRIVATE W16	cachedSS = 0;
PRIVATE W16	cachedDog = 0;
PRIVATE W16	cachedMutant = 0;

PRIVATE int progress_bar = 0;

LevelData_t levelData;


void CacheTextures( W16 start, W16 end )
{
	W16 i;
	static char texname[ 64 ];

	for( i = start ; i <= end ; ++i )
	{
		if ( !spriteTextures[i] ) {
			my_snprintf( texname, sizeof( texname ), "%s/%.3d.tga", spritelocation, i );
			spriteTextures[i] = TM_FindTexture( texname, TT_Sprite );
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: Level_ScanInfoPlane -Spawn all actors and mark down special places.
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Level_ScanInfoPlane( LevelData_t *lvl )
{
	int x, y;
	W16	tile;
	
	cachedGuard = 0;
	cachedOfficer = 0;
	cachedSS = 0;
	cachedDog = 0;
	cachedMutant = 0;
	progress_bar = 0;

	for( y = 0 ; y < 64; ++y )
	{
		for( x = 0 ; x < 64 ; ++x )
		{
			tile = lvl->Plane2[ (63 - y) * 64 + x ];
			if( ! tile ) 
			{
				continue;
			}

			switch( tile )
			{
//
// guard
//
			case 180:
			case 181:
			case 182:
			case 183:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 144:
			case 145:
			case 146:
			case 147:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 108:
			case 109:
			case 110:
			case 111:
				if( ! cachedGuard )
				{
					CacheTextures( SPR_GRD_S_1, SPR_GRD_SHOOT3 );
					cachedGuard = 1;
				}
				SpawnStand( en_guard, x, y, tile - 108, lvl );
				break;

			case 184:
			case 185:
			case 186:
			case 187:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 148:
			case 149:
			case 150:
			case 151:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 112:
			case 113:
			case 114:
			case 115:
				if( ! cachedGuard )
				{
					CacheTextures( SPR_GRD_S_1, SPR_GRD_SHOOT3 );
					cachedGuard = 1;
				}
				SpawnPatrol( en_guard, x, y,tile - 112 );
				break;

			case 124:
				SpawnDeadGuard( en_guard, x, y );
				break;
//
// officer
//
			case 188:
			case 189:
			case 190:
			case 191:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 152:
			case 153:
			case 154:
			case 155:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 116:
			case 117:
			case 118:
			case 119:
				if( ! cachedOfficer )
				{
					CacheTextures( SPR_OFC_S_1, SPR_OFC_SHOOT3 );
					cachedOfficer = 1;
				}
				SpawnStand( en_officer, x, y, tile - 116, lvl );
				break;


			case 192:
			case 193:
			case 194:
			case 195:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 156:
			case 157:
			case 158:
			case 159:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 120:
			case 121:
			case 122:
			case 123:
				if( ! cachedOfficer )
				{
					CacheTextures( SPR_OFC_S_1, SPR_OFC_SHOOT3 );
					cachedOfficer = 1;
				}
				SpawnPatrol( en_officer, x, y, tile - 120 );
				break;
//
// SS
//
			case 198:
			case 199:
			case 200:
			case 201:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 162:
			case 163:
			case 164:
			case 165:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 126:
			case 127:
			case 128:
			case 129:
				if( ! cachedSS )
				{
					CacheTextures( SPR_SS_S_1, SPR_SS_SHOOT3 );
					cachedSS = 1;
				}
				SpawnStand( en_ss, x, y, tile - 126, lvl );
				break;

			case 202:
			case 203:
			case 204:
			case 205:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 166:
			case 167:
			case 168:
			case 169:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 130:
			case 131:
			case 132:
			case 133:
				if( ! cachedSS )
				{
					CacheTextures( SPR_SS_S_1, SPR_SS_SHOOT3 );
					cachedSS = 1;
				}
				SpawnPatrol( en_ss, x, y, tile - 130 );
				break;
//
// dogs
//
			case 206:
			case 207:
			case 208:
			case 209:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 170:
			case 171:
			case 172:
			case 173:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 134:
			case 135:
			case 136:
			case 137:
				if( ! cachedDog )
				{
					CacheTextures( SPR_DOG_W1_1, SPR_DOG_JUMP3 );
					cachedDog = 1;
				}
				SpawnStand( en_dog, x, y, tile - 134, lvl );
				break;

			case 210:
			case 211:
			case 212:
			case 213:
				if( skill->value < gd_hard )
					break;
				tile -= 36;
			case 174:
			case 175:
			case 176:
			case 177:
				if( skill->value < gd_medium )
					break;
				tile -= 36;
			case 138:
			case 139:
			case 140:
			case 141:
				if( ! cachedDog )
				{
					CacheTextures( SPR_DOG_W1_1, SPR_DOG_JUMP3 );
					cachedDog = 1;
				}
				SpawnPatrol( en_dog, x, y, tile - 138 );
				break;
// bosses
			case 214:
				CacheTextures( SPR_BOSS_W1, SPR_BOSS_DIE3 );
				SpawnBoss( en_boss, x, y );
				break;

			case 197:
				CacheTextures( SPR_GRETEL_W1, SPR_GRETEL_DIE3 );
				SpawnBoss( en_gretel, x, y );
				break;

			case 215:
				CacheTextures( SPR_GIFT_W1, SPR_GIFT_DEAD );
				SpawnBoss( en_gift, x, y );
				break;

			case 179:
				CacheTextures( SPR_FAT_W1, SPR_FAT_DEAD );
				SpawnBoss( en_fat, x, y );
				break;

			case 196:
				CacheTextures( SPR_SCHABB_W1, SPR_HYPO4 );
				SpawnBoss( en_schabbs, x, y );
				break;

			case 160:
				CacheTextures( SPR_FAKE_W1, SPR_FAKE_DEAD );
				SpawnBoss( en_fake, x, y );
				break;

			case 178:
				CacheTextures( SPR_MECHA_W1, SPR_HITLER_DIE7 );
				SpawnBoss( en_mecha, x, y );
				break;
//
// Spear
//
			case 106:
				CacheTextures( SPR_SPECTRE_W1, SPR_SPECTRE_F4 );
				SpawnBoss( en_spectre, x, y );
				break;

			case 107:
				CacheTextures( SPR_ANGEL_W1, SPR_ANGEL_DEAD );
				SpawnBoss( en_angel, x, y );
				break;

			case 125:
				CacheTextures( SPR_TRANS_W1, SPR_TRANS_DIE3 );
				SpawnBoss( en_trans, x, y );
				break;

			case 142:
				CacheTextures( SPR_UBER_W1, SPR_UBER_DEAD );
				SpawnBoss( en_uber, x, y );
				break;

			case 143:
				CacheTextures( SPR_WILL_W1, SPR_WILL_DEAD );
				SpawnBoss( en_will, x, y );
				break;

			case 161:
				CacheTextures( SPR_DEATH_W1, SPR_DEATH_DEAD );
				SpawnBoss( en_death, x, y );
				break;
//
// mutants
//
			case 252:
			case 253:
			case 254:
			case 255:
				if( skill->value < gd_hard )
					break;
				tile -= 18;
			case 234:
			case 235:
			case 236:
			case 237:
				if( skill->value < gd_medium )
					break;
				tile -= 18;
			case 216:
			case 217:
			case 218:
			case 219:
				if( ! cachedMutant )
				{
					CacheTextures( SPR_MUT_S_1, SPR_MUT_SHOOT4 );
					cachedMutant = 1;
				}
				SpawnStand( en_mutant, x, y, tile - 216, lvl );
				break;

			case 256:
			case 257:
			case 258:
			case 259:
				if (skill->value<gd_hard)
					break;
				tile -= 18;
			case 238:
			case 239:
			case 240:
			case 241:
				if( skill->value < gd_medium )
					break;
				tile -= 18;
			case 220:
			case 221:
			case 222:
			case 223:
				if( ! cachedMutant )
				{
					CacheTextures( SPR_MUT_S_1, SPR_MUT_SHOOT4 );
					cachedMutant = 1;
				}
				SpawnPatrol( en_mutant, x, y, tile - 220 );
				break;

//
// ghosts
//
			case 224:
				CacheTextures( SPR_BLINKY_W1, SPR_BLINKY_W2 );
				SpawnGhosts( en_blinky, x, y );
				break;

			case 225:
				CacheTextures( SPR_PINKY_W1, SPR_PINKY_W2 );
				SpawnGhosts( en_clyde, x, y );
				break;

			case 226:
				CacheTextures( SPR_CLYDE_W1, SPR_CLYDE_W2 );
				SpawnGhosts( en_pinky, x, y );
				break;

			case 227:
				CacheTextures( SPR_INKY_W1, SPR_INKY_W2 );
				SpawnGhosts( en_inky, x, y );
				break;
			}
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Lvl_SpawnStatic( LevelData_t *lvl, int type, int x, int y )
{
	int spr_id;

	if( statinfo[ type ].powerup == -1 )
	{
		if( statinfo[ type ].block )	// blocking static
		{
			lvl->tilemap[ x ][ y ] |= BLOCK_TILE;
		}
		else						// dressing static
		{
			lvl->tilemap[ x ][ y ] |= DRESS_TILE;
		}

		spr_id = Sprite_GetNewSprite();
		if( spr_id == -1 )
		{
			return;
		}

		Sprite_SetPos( spr_id, TILE2POS( x ), TILE2POS( y ), 0 );
		Sprite_SetTex( spr_id, 0, SPR_STAT_0 + type );
	}
	else
	{
		Powerup_Spawn( x, y, statinfo[ type ].powerup );
		if( statinfo[ type ].powerup == pow_cross || 
			statinfo[ type ].powerup == pow_chalice ||
			statinfo[ type ].powerup == pow_bible ||
			statinfo[ type ].powerup == pow_crown ||
			statinfo[ type ].powerup == pow_fullheal )
		{
				levelstate.total_treasure++; // FIXME: move this to Powerup_Spawn Function!
		}
	}
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Lvl_SpawnObj( LevelData_t *lvl, int type, int x, int y )
{
	if( type >= 23 && type < 23 + num_statics )
	{// static object
		Lvl_SpawnStatic( lvl, type - 23, x, y);
		return;
	}

	switch( type )
	{
		case 0x13: // start N
			lvl->pSpawn.origin[ 0 ] = TILE2POS( x );
			lvl->pSpawn.origin[ 1 ] = TILE2POS( y );
			lvl->pSpawn.angle = ANG_90;
			break;

		case 0x14: // start E
			lvl->pSpawn.origin[ 0 ] = TILE2POS( x );
			lvl->pSpawn.origin[ 1 ] = TILE2POS( y );
			lvl->pSpawn.angle = ANG_0;
			break;

		case 0x15: // start S
			lvl->pSpawn.origin[ 0 ] = TILE2POS( x );
			lvl->pSpawn.origin[ 1 ] = TILE2POS( y );
			lvl->pSpawn.angle = ANG_270;
			break;

		case 0x16: // start W
			lvl->pSpawn.origin[ 0 ] = TILE2POS( x );
			lvl->pSpawn.origin[ 1 ] = TILE2POS( y );
			lvl->pSpawn.angle = ANG_180;
			break;

		case 0x5a: // turn E
			lvl->tilemap[ x ][ y ] |= TILE_IS_E_TURN;//FIXME!
			break;

		case 0x5b: // turn NE
			lvl->tilemap[ x ][ y ] |= TILE_IS_NE_TURN;//FIXME!
			break;

		case 0x5c: // turn N
			lvl->tilemap[ x ][ y ] |= TILE_IS_N_TURN;//FIXME!
			break;

		case 0x5d: // turn NW
			lvl->tilemap[ x ][ y ] |= TILE_IS_NW_TURN;//FIXME!
			break;

		case 0x5e: // turn W
			lvl->tilemap[ x ][ y ] |= TILE_IS_W_TURN;//FIXME!
			break;

		case 0x5f: // turn SW
			lvl->tilemap[ x ][ y ] |= TILE_IS_SW_TURN;//FIXME!
			break;

		case 0x60: // turn S
			lvl->tilemap[ x ][ y ] |= TILE_IS_S_TURN;//FIXME!
			break;

		case 0x61: // turn SE
			lvl->tilemap[ x ][ y ] |= TILE_IS_SE_TURN;//FIXME!
			break;

		case 0x62: // pushwall modifier
			lvl->tilemap[ x ][ y ] |= SECRET_TILE;
			levelstate.total_secrets++;
			break;

		case 0x63: // Victory trigger
			lvl->tilemap[ x ][ y ] |= EXIT_TILE;
			break;
		// spawn guards

	} // end of switch( type )

}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
		length -[in] The length of the EXPANDED data.

 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Lvl_CarmackExpand( W16 *source, W16 *dest, W16 length )
{
	#define NEARTAG	0xA7
	#define FARTAG	0xA8


	W32	chhigh, offset;
	W16	*copyptr, *outptr;
	W8	*inptr;
	W16 ch, count;

	length /= 2;

	inptr = (W8 *)source;
	outptr = dest;

	while( length )
	{
		ch = *(W16 *)inptr;
		inptr += 2;
		chhigh = ch >> 8;
		if( chhigh == NEARTAG )
		{
			count = ch & 0xff;
			if( ! count )
			{	// have to insert a word containing the tag byte
				ch |= *inptr++;
				*outptr++ = (W16)ch;
				length--;
			}
			else
			{
				offset = *inptr++;
				copyptr = outptr - offset;
				length -= count;
				while( count-- )
				{
					*outptr++ = *copyptr++;
				}
			}
		}
		else if( chhigh == FARTAG )
		{
			count = ch & 0xff;
			if( ! count )
			{				// have to insert a word containing the tag byte
				ch |= *inptr++;
				*outptr++ = ch;
				length--;
			}
			else
			{
				offset = *(W16 *)inptr;
				inptr += 2;
				copyptr = dest + offset;
				length -= count;
				while( count-- )
				{
					*outptr++ = *copyptr++;
				}
			}
		}
		else
		{
			*outptr++ = ch;
			length--;
		}
	}
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
		length -[in] Is EXPANDED length

 Returns:
	
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Lvl_RLEWexpand( W16 *source, W16 *dest, 
				   long length, unsigned rlewtag )
{
  unsigned value,count,i;
  W16 *end;
 


//
// expand it
//
	end = dest + (length >> 1);

	do
	{
		value = *source++;
		if( value != rlewtag )
		{
			//
			// uncompressed
			//
			*dest++ = value;
		}
		else
		{
			//
			// compressed string
			//
			count = *source++;
			value = *source++;

			for( i = 1 ; i <= count ; ++i )
			{
				*dest++ = value;
			}
		}

	} while( dest < end );

}


#define MAPHEADER_SIZE	49
#define MAP_SIGNATURE	0x21444921


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC LevelData_t *Level_LoadMap( const char *levelname )
{
	W16 rle;
	W32  offset[ 3 ];
	W16 length[ 3 ];	
	W16 w, h;
	W32	signature;
	W16 *buffer, expanded;	
	W8	*data;
	W32 ceiling, floor;
	LevelData_t *newMap;
	filehandle_t *fhandle;
	W16 mapNameLength;
	char *mapName;
	W16 musicNameLength;
	char *musicName;
	SW32 filesize;
	
	int x, y0, y, layer1, layer2, layer3;



	if( g_version->value == SPEAROFDESTINY  && currentMap.episode >= 6 && currentMap.episode < 9)//added the episode check... gsh)
	{
		statinfo = static_sod;
		num_statics = sizeof( static_sod ) / sizeof( static_sod[ 0 ] );
	}
	else
	{
		statinfo = static_wl6;
		num_statics = sizeof( static_wl6 ) / sizeof( static_wl6[ 0 ] );
	}



	newMap = &levelData;
	memset( newMap, 0, sizeof( LevelData_t ) );

		
    fhandle = FS_OpenFile( levelname, 0 );
	if( ! fhandle )
	{
		char errBuffer[1024];
		sprintf( errBuffer, "levelname: %s, could not be found", levelname);
		iphoneMessageBox("map filename", errBuffer);
		Com_Printf( "Could not load map (%s)\n", levelname );
		return NULL;
	}
	
	filesize = FS_GetFileSize( fhandle );
	if( filesize < MAPHEADER_SIZE )
	{
		iphoneMessageBox("map header size", "header size of the map is wrong");
		Com_Printf("Map file size is smaller than mapheader size\n");
		return NULL;
	}

		
//
// Process map header
//
	FS_ReadFile( &signature, 1, 4, fhandle );
	if( signature != MAP_SIGNATURE )
	{
		iphoneMessageBox("map signature", "signature of the map file is invalid");
		Com_Printf("File signature does not match MAP_SIGNATURE\n");
		return NULL;
	}

	
	
	FS_ReadFile( &rle, 2, 1, fhandle );

	FS_ReadFile( &w, 2, 1, fhandle );
	FS_ReadFile( &h, 2, 1, fhandle );

	FS_ReadFile( &ceiling, 4, 1, fhandle );
	FS_ReadFile( &floor, 4, 1, fhandle );
	

	FS_ReadFile( &length, 2, 3, fhandle );
	FS_ReadFile( &offset, 4, 3, fhandle );
	
	
	FS_ReadFile( &mapNameLength, 1, 2, fhandle );
	FS_ReadFile( &musicNameLength, 1, 2, fhandle );
	
	FS_ReadFile( &levelstate.fpartime, sizeof( float ), 1, fhandle );

	FS_ReadFile( levelstate.spartime, sizeof( W8 ), 5, fhandle );
	levelstate.spartime[ 5 ] = '\0';

	
	if( filesize < (MAPHEADER_SIZE + mapNameLength + musicNameLength +
			length[ 0 ] + length[ 1 ] + length[ 2 ]) )
	{
		iphoneMessageBox("map filesize", "filesize is less than MAPHEADER_SIZE + mapNameLength + musicNameLength + etc");
		Com_Printf("filesize is less than MAPHEADER_SIZE + mapNameLength + musicNameLength + etc\n");
		return NULL;	
	}
	
	mapName = Z_Malloc( mapNameLength + 1 );
	musicName = Z_Malloc( musicNameLength + 1 );


	FS_ReadFile( mapName, 1, mapNameLength, fhandle );
	mapName[ mapNameLength ] = '\0';	

	
	FS_ReadFile( musicName, 1, musicNameLength, fhandle );
	musicName[ musicNameLength ] = '\0';
	

	if( filesize < (MAPHEADER_SIZE + mapNameLength + musicNameLength) )
	{
		iphoneMessageBox("map filesize", "filesize is less than MAPHEADER_SIZE + mapNameLength + musicNameLength");
		Com_Printf("filesize is less than MAPHEADER_SIZE + mapNameLength + musicNameLength\n");
		return NULL;	
	}
	
//
// Plane1  -Walls
// 	
	data = MM_MALLOC( length[ 0 ] );

	FS_FileSeek( fhandle, offset[ 0 ], SEEK_SET );
	FS_ReadFile( data, 1, length[ 0 ], fhandle );
	

	expanded = *((unsigned short *)data);
	buffer = MM_MALLOC( expanded );

	Lvl_CarmackExpand( (unsigned short *)data+1, buffer, expanded );
	Lvl_RLEWexpand( buffer+1, newMap->Plane1, 64*64*2, rle );
	
	MM_FREE( buffer );	
	MM_FREE( data );

//
// Plane2 -Objects
// 
    data = MM_MALLOC( length[ 1 ] );

	FS_FileSeek( fhandle, offset[ 1 ], SEEK_SET );
	FS_ReadFile( data, 1, length[ 1 ], fhandle );
	

	expanded = *((PW16)data);
	buffer = MM_MALLOC( expanded );

	Lvl_CarmackExpand( (PW16)data+1, buffer, expanded );
	Lvl_RLEWexpand( buffer+1, newMap->Plane2, 64*64*2, rle );
	
	MM_FREE( buffer );	
	MM_FREE( data );

//
// Plane3 -Other
// 
    data = MM_MALLOC( length[ 2 ] );

	FS_FileSeek( fhandle, offset[ 2 ], SEEK_SET );
	FS_ReadFile( data, 1, length[ 2 ], fhandle );
	

	expanded = *((PW16)data);
	buffer = MM_MALLOC( expanded );

	Lvl_CarmackExpand( (PW16)data+1, buffer, expanded );
	Lvl_RLEWexpand( buffer+1, newMap->Plane3, 64*64*2, rle ); 
 
    MM_FREE( buffer );	
	MM_FREE( data );

	
	FS_CloseFile( fhandle );
		
	// HUGE HACK to take out the pushwall maze that occasionally
	// gets players stuck in level E4M2 without actually touching
	// a map editor...
	if ( !strcmp( levelname, "maps/w31.map" ) ) {
		for ( x = 22 ; x <= 32 ; x++ ) {
				for ( y0 = 30 ; y0 <= 32 ; y0++ ) {
				newMap->Plane1[ y0 * 64 + x ] = newMap->Plane1[ 30*64+21 ];
				newMap->Plane2[ y0 * 64 + x ] = newMap->Plane2[ 30*64+21 ];
				newMap->Plane3[ y0 * 64 + x ] = newMap->Plane3[ 30*64+21 ];
			}
		}
	}
	
	
	for( y0 = 0 ; y0 < 64 ; ++y0 )
	for( x = 0 ; x < 64 ; ++x )
	{
		y = 63 - y0;
		layer1 = newMap->Plane1[ y0 * 64 + x ];
		layer2 = newMap->Plane2[ y0 * 64 + x ];
		layer3 = newMap->Plane3[ y0 * 64 + x ];

// if server, process obj layer!
		if( layer2 ) 
		{
			Lvl_SpawnObj( newMap, layer2, x, y );
		}

// Map data layer
		if( layer1 == 0 )
		{
			newMap->areas[ x ][ y ] = -3; // unknown area
		}
		else if( layer1 < 0x6a ) // solid map object
		{
			if( (layer1 >= 0x5A && layer1 <= 0x5F) || 
				layer1 == 0x64 || layer1 == 0x65 ) // door
			{
				newMap->tilemap[ x ][ y ] |= DOOR_TILE;
				Door_SpawnDoor( &newMap->Doors, x, y, layer1 );
				newMap->areas[ x ][ y ] = -2; // door area
			}
			else
			{
				newMap->tilemap[ x ][ y ] |= WALL_TILE;

				newMap->wall_tex_x[ x ][ y ] = (layer1-1) * 2 + 1;
				newMap->wall_tex_y[ x ][ y ] = (layer1-1) * 2;
				newMap->areas[ x ][ y ] = -1; // wall area
				
				if( layer1 == 0x15 ) // elevator
				{
					newMap->tilemap[ x ][ y ] |= ELEVATOR_TILE;
				}
			}
		}
		else if( layer1 == 0x6a ) // Ambush floor tile
		{
			newMap->tilemap[ x ][ y ] |= AMBUSH_TILE;
			newMap->areas[ x ][ y ] = -3; // unknown area
		}
		else if( layer1 >= FIRSTAREA &&
			layer1 < (FIRSTAREA + NUMAREAS) ) // area
		{
			if( layer1 == FIRSTAREA ) // secret level
			{
				newMap->tilemap[ x ][ y ] |= SECRETLEVEL_TILE;
			}
			
			newMap->areas[ x ][ y ] = layer1 - FIRSTAREA;// spawn area
		}
		else
		{
			newMap->areas[ x ][ y ] = -3; // unknown area
		}
// End of the map data layer
	}
	
	// JDC: try to replace all the unknown areas with an adjacent area, to
	// avoid the silent attack / no damage problem when you get an ambush
	// guard stuck on their original tile
	for ( x = 1 ; x < 63 ; x++ ) {
		for ( y = 1 ; y < 63 ; y++ ) {
			if ( newMap->areas[x][y] != -3 ) {
				continue;
			}
			if ( newMap->areas[x-1][y] >= 0 ) {
				newMap->areas[x][y] = newMap->areas[x-1][y];
			} else if ( newMap->areas[x+1][y] >= 0 ) {
				newMap->areas[x][y] = newMap->areas[x+1][y];
			} else if ( newMap->areas[x][y-1] >= 0 ) {
				newMap->areas[x][y] = newMap->areas[x][y-1];
			} else if ( newMap->areas[x+1][y+1] >= 0 ) {
				newMap->areas[x][y] = newMap->areas[x][y+1];
			}
		}
	}
	
	Door_SetAreas( &newMap->Doors, newMap->areas );

	my_strlcpy( levelstate.level_name, mapName, sizeof( levelstate.level_name ) );
	
	strcpy( newMap->mapName, mapName );
	strcpy( newMap->musicName, musicName );

	newMap->ceilingColour[ 0 ] = (W8)((ceiling >> 16) & 0xFF);
	newMap->ceilingColour[ 1 ] = (W8)((ceiling >> 8) & 0xFF);
	newMap->ceilingColour[ 2 ] = (W8)((ceiling ) & 0xFF);
	newMap->floorColour[ 0 ] = (W8)((floor >> 16) & 0xFF);
	newMap->floorColour[ 1 ] = (W8)((floor >> 8) & 0xFF);
	newMap->floorColour[ 2 ] = (W8)((floor ) & 0xFF);

	return newMap;
}

/*
 -----------------------------------------------------------------------------
 Function:		Level_VerifyMap
 
 Parameters:	level file name
 
 Returns:		0 if invalid map, 1 otherwise
 
 Notes: 
 
 -----------------------------------------------------------------------------
 */
PUBLIC int Level_VerifyMap( const char *levelname )
{
	W16 rle;
	W32  offset[ 3 ];
	W16 length[ 3 ];	
	W16 w, h;
	W32	signature;
	W32 ceiling, floor;
	filehandle_t *fhandle;
	W16 mapNameLength;
	char *mapName = NULL;
	W16 musicNameLength;
	char *musicName = NULL;
	SW32 filesize;
	int value = 1;
	
	
	fhandle = FS_OpenFile( levelname, FA_FILE_IPHONE_DOC_DIR );
	if( ! fhandle )
	{
		value = 0;
		goto cleanup;
	}
	
	filesize = FS_GetFileSize( fhandle );
	if( filesize < MAPHEADER_SIZE )
	{
		value = 0;
		goto cleanup;
	}
	
	FS_ReadFile( &signature, 1, 4, fhandle );
	if( signature != MAP_SIGNATURE )
	{
		value = 0;
		goto cleanup;
	}
	
	FS_ReadFile( &rle, 2, 1, fhandle );
	
	FS_ReadFile( &w, 2, 1, fhandle );
	FS_ReadFile( &h, 2, 1, fhandle );
	
	FS_ReadFile( &ceiling, 4, 1, fhandle );
	FS_ReadFile( &floor, 4, 1, fhandle );
	
	
	FS_ReadFile( &length, 2, 3, fhandle );
	FS_ReadFile( &offset, 4, 3, fhandle );
	
	
	FS_ReadFile( &mapNameLength, 1, 2, fhandle );
	FS_ReadFile( &musicNameLength, 1, 2, fhandle );
	
	FS_ReadFile( &levelstate.fpartime, sizeof( float ), 1, fhandle );
	
	FS_ReadFile( levelstate.spartime, sizeof( W8 ), 5, fhandle );
	levelstate.spartime[ 5 ] = '\0';
	
	
	if( filesize < (MAPHEADER_SIZE + mapNameLength + musicNameLength +
					length[ 0 ] + length[ 1 ] + length[ 2 ]) )
	{
		value = 0;
		goto cleanup;
	}
	
	mapName = Z_Malloc( mapNameLength + 1 );
	musicName = Z_Malloc( musicNameLength + 1 );
	
	
	FS_ReadFile( mapName, 1, mapNameLength, fhandle );
	mapName[ mapNameLength ] = '\0';	
	
	
	FS_ReadFile( musicName, 1, musicNameLength, fhandle );
	musicName[ musicNameLength ] = '\0';
	
	
	if( filesize < (MAPHEADER_SIZE + mapNameLength + musicNameLength) )
	{
		value = 0;
		goto cleanup;
	}
	
cleanup:
	FS_CloseFile(fhandle);
	if (mapName) {
		Z_Free(mapName);
	}
	if (musicName) {
		Z_Free(musicName);
	}
	return value;
}




/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Level_PrecacheTextures_Sound( LevelData_t *lvl )
{
	int x, y;
	char texname[ 32 ];	

	

	for( x = 0 ; x < 64 ; ++x )
	for( y = 0 ; y < 64 ; ++y )
	{
		if( lvl->tilemap[ x ][ y ] & WALL_TILE )
		{
			LoadWallTexture( lvl->wall_tex_x[ x ][ y ] );
			LoadWallTexture( lvl->wall_tex_y[ x ][ y ] );
		}

		if( lvl->tilemap[ x ][ y ] & POWERUP_TILE )
		{
			int	tex = lvl->tilemap[ x ][ y ] & POWERUP_TILE;
			my_snprintf( texname, sizeof( texname ), "%s/%.3d.tga", spritelocation, tex );
			spriteTextures[tex] = TM_FindTexture( texname, TT_Wall );
		}		
	}

	// Doors
	for( x = TEX_DOOR; x < TEX_DLOCK+2 ; ++x )
	{
		LoadWallTexture( x );
	}


	// Items
	CacheTextures( 26, 36 );	

	// Weapon frames
	CacheTextures( SPR_KNIFEREADY, SPR_CHAINATK4 );
	
	// in case the last wall load was a dim copy
	pfglColor3f( 1, 1, 1 );
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: true if a straight line between 2 points is unobstructed,
			otherwise false.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean Level_CheckLine( SW32 x1, SW32 y1, SW32 x2, SW32 y2, LevelData_t *lvl )
{
	SW32 xt1, yt1, xt2, yt2; /* tile positions */
	SW32 x, y;				/* current point in !tiles! */
	SW32 xdist, ydist;
	SW32 xstep, ystep; /* Step value for each whole xy */
	
	SW32 deltafrac; /* current point in !1/256 of tile! */

	SW32 Frac;		/* Fractional xy stepper */

	SW32 partial;	/* how much to move in our direction to border */
	SW32 intercept;	/* Temp for door code */



	#define	FRACBITS	8		/* Number of bits of fraction */


// get start & end tiles
	xt1 = x1 >> TILESHIFT; 
	yt1 = y1 >> TILESHIFT;
	
	xt2 = x2 >> TILESHIFT; 
	yt2 = y2 >> TILESHIFT;

	xdist = ABS( xt2 - xt1 ); // X distance in tiles
	ydist = ABS( yt2 - yt1 ); // Y distance in tiles

// 1/256 tile precision (TILESHIFT is 16)
	x1 >>= FRACBITS; y1 >>= FRACBITS;
	x2 >>= FRACBITS; y2 >>= FRACBITS;


	if( xdist ) // always positive check only for 0
	{
		if( xt2 > xt1 )
		{
			partial = 256 - (x1 & 0xff);
			xstep = 1;
		}
		else
		{
			partial = x1 & 0xff;
			xstep = -1;
		}

		deltafrac = ABS( x2 - x1 );
		ystep = ((y2 - y1) << FRACBITS) / deltafrac;
		Frac = y1 + ((ystep * partial) >> FRACBITS);

		x = xt1 + xstep;
		xt2 += xstep;
		do
		{
			y = Frac >> FRACBITS;
			Frac += ystep;

			
			assert( x >= 0 && x < 64 && y >= 0 && y < 64 );
			if( lvl->tilemap[ x ][ y ] & WALL_TILE )
			{
				return false; // Wall is in path quitting!
			}
				
			if( lvl->tilemap[ x ][ y ] & DOOR_TILE )
			{// door, see if the door is open enough
				if( lvl->Doors.DoorMap[ x ][ y ].action != dr_open )
				{
					if( lvl->Doors.DoorMap[ x ][ y ].action == dr_closed )
					{
						return false;
					}
					// checking vertical doors in action: ->_I_
					intercept = ((Frac - ystep / 2) & 0xFF) >> 4; // 1/64 of tile
					if( intercept < (63 - lvl->Doors.DoorMap[ x ][ y ].ticcount) )
					{
						return false;
					}
				}
			}
			x += xstep;

		} while( x != xt2 );
	}

	if( ydist ) // always positive check only for 0
	{
		if( yt2 > yt1 )
		{
			partial = 256 - (y1 & 0xff);
			ystep = 1;
		}
		else
		{
			partial = y1 & 0xff;
			ystep = -1;
		}

		deltafrac = ABS( y2 - y1 );
		xstep = ((x2 - x1) << FRACBITS) / deltafrac;
		Frac = x1 + ((xstep * partial) >> FRACBITS);

		y = yt1 + ystep;
		yt2 += ystep;
		do
		{
			x = Frac >> FRACBITS;
			Frac += xstep;

			assert( x >= 0 && x < 64 && y >= 0 && y < 64 );
			if( lvl->tilemap[ x ][ y ] & WALL_TILE )
			{
				return false; // Wall is in path quitting!
			}
				
			if( lvl->tilemap[ x ][ y ] & DOOR_TILE )
			{// door, see if the door is open enough
				if( lvl->Doors.DoorMap[ x ][ y ].action != dr_open )
				{
					if( lvl->Doors.DoorMap[ x ][ y ].action == dr_closed )
					{
						return false;
					}
					// checking vertical doors in action: ->_I_
					intercept = ((Frac - xstep / 2) & 0xFF) >> 4; // 1/64 of tile
					if( intercept < lvl->Doors.DoorMap[ x ][ y ].ticcount )
					{
						return false;
					}
				}
			}
			y += ystep;

		} while( y != yt2 );
	}

	return true;
}
