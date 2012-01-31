/*

	Copyright (C) 2004-2005 Michael Liebscher

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
 *	wolf_map.c:	Decode Wolfenstein 3-D Map data.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wolf_def.h"

#include "../string/com_string.h"
#include "../loaders/tga.h"
#include "../filesys/file.h"
#include "../../../common/arch.h"
#include "../memory/memory.h"
#include "../../../common/common_utils.h"


#define MAPHEADNAME		"MAPHEAD"
#define MAPNAME			"GAMEMAPS"



PRIVATE FILE *maphandle;


PRIVATE W32	headeroffsets[ 100 ];
PRIVATE W32	TotalMaps;
PRIVATE W16	RLEWtag;

PRIVATE W16 gameversion;


PRIVATE const W32 vgaCeilingWL6[] =
{
	0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0xbfbf,
	0x4e4e,0x4e4e,0x4e4e,0x1d1d,0x8d8d,0x4e4e,0x1d1d,0x2d2d,0x1d1d,0x8d8d,
	0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x1d1d,0x2d2d,0xdddd,0x1d1d,0x1d1d,0x9898,

	0x1d1d,0x9d9d,0x2d2d,0xdddd,0xdddd,0x9d9d,0x2d2d,0x4d4d,0x1d1d,0xdddd,
	0x7d7d,0x1d1d,0x2d2d,0x2d2d,0xdddd,0xd7d7,0x1d1d,0x1d1d,0x1d1d,0x2d2d,
	0x1d1d,0x1d1d,0x1d1d,0x1d1d,0xdddd,0xdddd,0x7d7d,0xdddd,0xdddd,0xdddd
};

PRIVATE const W32 vgaCeilingSOD[] =
{
	0x6f6f,0x4f4f,0x1d1d,0xdede,0xdfdf,0x2e2e,0x7f7f,0x9e9e,0xaeae,0x7f7f,
	0x1d1d,0xdede,0xdfdf,0xdede,0xdfdf,0xdede,0xe1e1,0xdcdc,0x2e2e,0x1d1d,0xdcdc
};

PRIVATE W32 WL6_songs[] =
{
//
// Episode One
//
	GETTHEM_MUS,
	SEARCHN_MUS,
	POW_MUS,
	SUSPENSE_MUS,
	GETTHEM_MUS,
	SEARCHN_MUS,
	POW_MUS,
	SUSPENSE_MUS,

	WARMARCH_MUS,	// Boss level
	CORNER_MUS,		// Secret level

//
// Episode Two
//
	NAZI_OMI_MUS,
	PREGNANT_MUS,
	GOINGAFT_MUS,
	HEADACHE_MUS,
	NAZI_OMI_MUS,
	PREGNANT_MUS,
	HEADACHE_MUS,
	GOINGAFT_MUS,

	WARMARCH_MUS,	// Boss level
	DUNGEON_MUS,	// Secret level

//
// Episode Three
//
	INTROCW3_MUS,
	NAZI_RAP_MUS,
	TWELFTH_MUS,
	ZEROHOUR_MUS,
	INTROCW3_MUS,
	NAZI_RAP_MUS,
	TWELFTH_MUS,
	ZEROHOUR_MUS,

	ULTIMATE_MUS,	// Boss level
	PACMAN_MUS,		// Secret level

//
// Episode Four
//
	GETTHEM_MUS,
	SEARCHN_MUS,
	POW_MUS,
	SUSPENSE_MUS,
	GETTHEM_MUS,
	SEARCHN_MUS,
	POW_MUS,
	SUSPENSE_MUS,

	WARMARCH_MUS,	// Boss level
	CORNER_MUS,		// Secret level

//
// Episode Five
//
	NAZI_OMI_MUS,
	PREGNANT_MUS,
	GOINGAFT_MUS,
	HEADACHE_MUS,
	NAZI_OMI_MUS,
	PREGNANT_MUS,
	HEADACHE_MUS,
	GOINGAFT_MUS,

	WARMARCH_MUS,	// Boss level
	DUNGEON_MUS,	// Secret level

//
// Episode Six
//
	INTROCW3_MUS,
	NAZI_RAP_MUS,
	TWELFTH_MUS,
	ZEROHOUR_MUS,
	INTROCW3_MUS,
	NAZI_RAP_MUS,
	TWELFTH_MUS,
	ZEROHOUR_MUS,

	ULTIMATE_MUS,	// Boss level
	FUNKYOU_MUS		// Secret level
};
 

PRIVATE W32 SOD_songs[] =
{	
	SOD_XTIPTOE_MUS,
	SOD_XFUNKIE_MUS,
	SOD_XDEATH_MUS,
	SOD_XGETYOU_MUS,	// DON'T KNOW
	SOD_ULTIMATE_MUS,	// Trans Grosse

	SOD_DUNGEON_MUS,
	SOD_GOINGAFT_MUS,
	SOD_POW_MUS,
	SOD_TWELFTH_MUS,
	SOD_ULTIMATE_MUS,	// Barnacle Wilhelm BOSS

	SOD_NAZI_OMI_MUS,
	SOD_GETTHEM_MUS,
	SOD_SUSPENSE_MUS,
	SOD_SEARCHN_MUS,
	SOD_ZEROHOUR_MUS,
	SOD_ULTIMATE_MUS,	// Super Mutant BOSS

	SOD_XPUTIT_MUS,
	SOD_ULTIMATE_MUS,	// Death Knight BOSS

	SOD_XJAZNAZI_MUS,	// Secret level
	SOD_XFUNKIE_MUS,	// Secret level (DON'T KNOW)

	SOD_XEVIL_MUS		// Angel of Death BOSS

};



typedef struct
{
	float time;
	char timestr[ 6 ];

} times;


PRIVATE times parTimesWL6[] =
{
	 //
	 // Episode One Par Times
	 //
	 { 1.5,	"01:30" },
	 { 2,	"02:00" },
	 { 2,	"02:00" },
	 { 3.5,	"03:30" },
	 { 3,	"03:00" },
	 { 3,	"03:00" },
	 { 2.5,	"02:30" },
	 { 2.5,	"02:30" },
	 { 0,	"??:??" },	// Boss level
	 { 0,	"??:??" },	// Secret level

	 //
	 // Episode Two Par Times
	 //
	 { 1.5,	"01:30" },
	 { 3.5,	"03:30" },
	 { 3,	"03:00" },
	 { 2,	"02:00" },
	 { 4,	"04:00" },
	 { 6,	"06:00" },
	 { 1,	"01:00" },
	 { 3,	"03:00" },
	 { 0,	"??:??" },
	 { 0,	"??:??" },

	 //
	 // Episode Three Par Times
	 //
	 { 1.5,	"01:30" },
	 { 1.5,	"01:30" },
	 { 2.5,	"02:30" },
	 { 2.5,	"02:30" },
	 { 3.5,	"03:30" },
	 { 2.5,	"02:30" },
	 { 2,	"02:00" },
	 { 6,	"06:00" },
	 { 0,	"??:??" },
	 { 0,	"??:??" },

	 //
	 // Episode Four Par Times
	 //
	 { 2,	"02:00" },
	 { 2,	"02:00" },
	 { 1.5,	"01:30" },
	 { 1,	"01:00" },
	 { 4.5,	"04:30" },
	 { 3.5,	"03:30" },
	 { 2,	"02:00" },
	 { 4.5,	"04:30" },
	 { 0,	"??:??" },
	 { 0,	"??:??" },

	 //
	 // Episode Five Par Times
	 //
	 { 2.5,	"02:30" },
	 { 1.5,	"01:30" },
	 { 2.5,	"02:30" },
	 { 2.5,	"02:30" },
	 { 4,	"04:00" },
	 { 3,	"03:00" },
	 { 4.5,	"04:30" },
	 { 3.5,	"03:30" },
	 { 0,	"??:??" },
	 { 0,	"??:??" },

	 //
	 // Episode Six Par Times
	 //
	 { 6.5,	"06:30" },
	 { 4,	"04:00" },
	 { 4.5,	"04:30" },
	 { 6,	"06:00" },
	 { 5,	"05:00" },
	 { 5.5,	"05:30" },
	 { 5.5,	"05:30" },
	 { 8.5,	"08:30" },
	 { 0,	"??:??" },
	 { 0,	"??:??" }
};

PRIVATE times parTimesSOD[] =	
{ 
	 //
	 // SPEAR OF DESTINY TIMES
	 //
	 { 1.5,	"01:30" },
	 { 3.5,	"03:30" },
	 { 2.75,"02:45" },
	 { 3.5,	"03:30" },
	 { 0,	"??:??" },	// Boss 1
	 { 4.5,	"04:30" },
	 { 3.25,"03:15" },
	 { 2.75,"02:45" },
	 { 4.75,"04:45" },
	 { 0,	"??:??" },	// Boss 2
	 { 6.5,	"06:30" },
	 { 4.5,	"04:30" },
	 { 2.75,"02:45" },
	 { 4.5,	"04:30" },
	 { 6,	"06:00" },
	 { 0,	"??:??" },	// Boss 3
	 { 6,	"06:00" },
	 { 0,	"??:??" },	// Boss 4
	 { 0,	"??:??" },	// Secret level 1
	 { 0,	"??:??" },	// Secret level 2

};


/*
-----------------------------------------------------------------------------
 Function: CAL_SetupMapFile -Setup map files for decoding.
 
 Parameters: extension -[in] file extension for map data files.
 
 Returns: Non-zero on success, zero otherwise.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE W8 CAL_SetupMapFile( const char *extension )
{
	FILE *handle;
	SW32 length;
	char fname[ 13 ];
	
	
//
// load maphead.xxx (offsets and tileinfo for map file)
//
	cs_strlcpy( fname, MAPHEADNAME, sizeof( fname ) );
	cs_strlcat( fname, extension, sizeof( fname ) );

	handle = fopen( cs_strupr( fname ), "rb" );
	if( handle == NULL )
	{
		handle = fopen( cs_strlwr( fname ), "rb" );
		if( handle == NULL )
		{
			printf( "Could not open file (%s) for read!\n", fname );
			return 0;
		}
	}

	length = FS_FileLength( handle );
	

	fread( &RLEWtag, 2, 1, handle );
	
	for( TotalMaps = 0 ; TotalMaps < length ; ++TotalMaps )
	{
		fread( &headeroffsets[ TotalMaps ], 4, 1, handle );		
		if( ! headeroffsets[ TotalMaps ] )
		{
			break;
		}
	}


	fclose( handle );



	cs_strlcpy( fname, MAPNAME, sizeof( fname ) );
	cs_strlcat( fname, extension, sizeof( fname ) );
	
	maphandle = fopen( cs_strupr( fname ), "rb");
	if( NULL == maphandle )
	{
		maphandle = fopen( cs_strlwr( fname ), "rb");
		if( NULL == maphandle )
		{
			return 0;
		}
	}


	return 1;
}

/*
-----------------------------------------------------------------------------
 Function: CAL_ShutdownMapFile -Shutdown map file.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE void CAL_ShutdownMapFile( void )
{
	if( maphandle )
	{
		fclose( maphandle );
	}
}



/*
-----------------------------------------------------------------------------
 Function: CA_CacheMap -Cache and save map data.
 
 Parameters: 
			ChunkOffset -[in] Chunk offset.
			Chunklength -[in] Length of chunk.
			filename -[in] File name to save map as.
			index -[in] File name index number.

 Returns: Non-zero on success, otherwise zero.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PRIVATE W8 CA_CacheMap( W32 ChunkOffset, W32 Chunklength, const char *filename, W32 index )
{
	W32 offset[ 3 ];
	W32 offsetin[ 3 ];
	W32 temp;
	W16 length[ 3 ];
	W8 sig[ 5 ];
	W16 w, h;	
	W8 *data;
	W32 ceiling;
	W32 floor;
	FILE *fout;
	float ftime;
	char *stime;
	char name[ 32 ];
	char musicName[ 64 ];
	extern char gamepal[];
	SW32 jmp;

	if( gameversion == SOD_PAK )
	{
		temp = (vgaCeilingSOD[ index ] & 0xff) * 3;
		ceiling = ( (gamepal[ temp ] << 2)  << 16 ) | ( (gamepal[ temp+1 ] << 2 ) << 8) | (gamepal[ temp+2 ]<<2);
	
		temp = 0x19 * 3;
		floor = ( (gamepal[ temp ] << 2)  << 16 ) | ( (gamepal[ temp+1 ] << 2 ) << 8) | (gamepal[ temp+2 ]<<2);

		ftime = parTimesSOD[ index ].time;
		stime = parTimesSOD[ index ].timestr;

		cs_snprintf( musicName, sizeof( musicName ), "music/%s.ogg", GetMusicFileName_SOD( SOD_songs[ index ] ) );
	}
	else
	{
		temp = (vgaCeilingWL6[ index ] & 0xff) * 3;
		ceiling = ( (gamepal[ temp ] << 2)  << 16 ) | ( (gamepal[ temp+1 ] << 2 ) << 8) | (gamepal[ temp+2 ]<<2);
	
		temp = 0x19 * 3;
		floor = ( (gamepal[ temp ] << 2)  << 16 ) | ( (gamepal[ temp+1 ] << 2 ) << 8) | (gamepal[ temp+2 ]<<2);

		ftime = parTimesWL6[ index ].time;
		stime = parTimesWL6[ index ].timestr;

		cs_snprintf( musicName, sizeof( musicName ), "music/%s.ogg", GetMusicFileName_WL6( WL6_songs[ index ] ) );
	}

	fout = fopen( filename, "wb");
	if( NULL == fout )
	{
		return 0;
	}
	


	fseek( maphandle, ChunkOffset, SEEK_SET );


	fread( &offsetin, sizeof( W32 ), 3, maphandle );
	fread( &length, sizeof( W16 ), 3, maphandle );
	
	fread( &w, sizeof( W16 ), 1, maphandle );
	fread( &h, sizeof( W16 ), 1, maphandle );

	

	fread( name, sizeof( W8 ), 16, maphandle );	
	fread( sig, sizeof( W8 ), 4, maphandle );
		
//	
// Output header
//

	// Map file header signature
	fwrite( sig, sizeof( W8 ), 4, fout );	

	// RLE Word tag
	fwrite( &RLEWtag, sizeof( W16 ), 1, fout );	

	// Max Width
	fwrite( &w, sizeof( W16 ), 1, fout );
	
	// Max Height
	fwrite( &h, sizeof( W16 ), 1, fout );	

	// Ceiling Colour
	fwrite( &ceiling, sizeof( W32 ), 1, fout );		

	// Floor Colour
	fwrite( &floor, sizeof( W32 ), 1, fout );	

	// Length of layers
	temp = length[ 0 ];
	fwrite( &temp, sizeof( W16 ), 1, fout );	// Length One
	temp = length[ 1 ];
	fwrite( &temp, sizeof( W16 ), 1, fout );	// Length Two	
	temp = length[ 2 ];
	fwrite( &temp, sizeof( W16 ), 1, fout );	// Length Three

	jmp = ftell( fout );

	temp = 0;
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset One
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset Two	
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset Three
	
	
	// Map name length	
	temp = strlen( name );
	fwrite( &temp, sizeof( W16 ), 1, fout );

	// Music name length	
	temp = strlen( musicName );
	fwrite( &temp, sizeof( W16 ), 1, fout );		

	// Par time Float
	fwrite( &ftime, sizeof( float ), 1, fout );	

	// Par time string
	fwrite( stime, sizeof( W8 ), 5 , fout );	

	// Map name
	fwrite( name, sizeof( W8 ), strlen( name ), fout );	

	// Music file name
	fwrite( musicName, sizeof( W8 ), strlen( musicName ), fout );	
	

	


	data = MM_MALLOC( length[ 0 ] );
	if( data == NULL )
	{
		return 0;
	}

	offset[ 0 ] = ftell( fout );

	fseek( maphandle, offsetin[ 0 ], SEEK_SET );
	fread( data, 1, length[ 0 ], maphandle );

	fwrite( data, 1, length[ 0 ], fout );


	data = MM_REALLOC( data, length[ 1 ] );
	if( data == NULL )
	{
		return 0;
	}

	offset[ 1 ] = ftell( fout );

	fseek( maphandle, offsetin[ 1 ], SEEK_SET );
	fread( data, 1, length[ 1 ], maphandle );

	fwrite( data, 1, length[ 1 ], fout );




	data = MM_REALLOC( data, length[ 2 ] );
	if( data == NULL )
	{
		return 0;
	}

	offset[ 2 ] = ftell( fout );

	fseek( maphandle, offsetin[ 2 ], SEEK_SET );
	fread( data, 1, length[ 2 ], maphandle );

	fwrite( data, 1, length[ 2 ], fout );	

	MM_FREE( data );


	fseek( fout, jmp, SEEK_SET );

	temp = offset[ 0 ];
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset One
	
	temp = offset[ 1 ];
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset Two	
	
	temp = offset[ 2 ];
	fwrite( &temp, sizeof( W32 ), 1, fout );	// Offset Three

	
	fclose( fout );
	
	
	return 1;
}


/*
-----------------------------------------------------------------------------
 Function: MapRipper -Re-encode map data.
 
 Parameters: extension -[in] file extension for map data files.
 
 Returns: Nothing.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC _boolean MapRipper( const char *fextension, W16 version )
{
	W32 i;
	char fname[ 32 ];

	gameversion = version;

//
// Setup
//

	if( 0 == FS_Mkdir( MAPDIR ) )
	{
		printf( "[%s] Could not create directory (%s)!\n", "wolf_map.c", MAPDIR );
		
		return false;
	}


	if( ! CAL_SetupMapFile( fextension ) )
	{
		CAL_ShutdownMapFile();
		
		return false;
	}

//
// Decode Map data
//

	printf( "Decoding Map Data...\n" );

	for( i = 0 ; i < TotalMaps ; ++i )
	{
		cs_snprintf( fname, sizeof( fname ), "%s/%c%.2d.map", MAPDIR, TOLOWER( fextension[ 1 ] ), i );

		CA_CacheMap( headeroffsets[ i ], 
						headeroffsets[ i + 1 ] - headeroffsets[ i ], 
						fname, i );

	}

//
// Shutdown
//

	CAL_ShutdownMapFile();

	return true;
}
