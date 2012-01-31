/*

	Copyright (C) 2004 Michael Liebscher

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
 *	wolf_def.h:   Valid chunk numbers for Wolfenstein 3-D and Spear of Destiny.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by wolf_aud.c, wolf_gfx.c, wolf_map.c,
	wolf_pal.c, wolf_pm.c and wl6_name.c.

*/

#ifndef __WOLF_DEF_H__
#define __WOLF_DEF_H__

#include "../../../common/arch.h"


#define WL1_PAK		(1<<0)
#define WL6_PAK		(1<<1)
#define SDM_PAK		(1<<2)
#define SOD_PAK		(1<<3)
#define MAC_PAK		(1<<4)
#define THREEDO_PAK (1<<5)


#define WL1_FEXT    "*.WL1"
#define WL6_FEXT    "*.WL6"
#define SDM_FEXT    "*.SDM"
#define SOD_FEXT    "*.SOD"
#define MAC_FEXT    "*.MAC"



#define GFXWALLDIR		"walls"
#define GFXSPRITEDIR	"sprites"
#define SFXDIR			"sfx"

#define SODGFXSPRITEDIR	"sodsprites"
#define SODSFXDIR		"sodsfx"

#define MAPDIR			"maps"

#define LGFXDIR			"pics"

#define LSFXDIR			"lsfx"
#define SODLSFXDIR		"sodlsfx"

#define MUSICDIR		"music"





typedef enum 
{
		// Lump Start
		WL1_H_BJPIC = 3,
		WL1_H_CASTLEPIC,                 // 4
		WL1_H_KEYBOARDPIC,               // 5
		WL1_H_CONTROLPIC,                // 6
		WL1_H_HEALPIC,                   // 7
		WL1_H_BGPIC,                     // 8
		WL1_H_GUNPIC,                    // 9
		WL1_H_KEYPIC,                    // 10
		WL1_H_BLAZEPIC,                  // 11
		WL1_H_WEAPON1234PIC,             // 12
		WL1_H_WOLFLOGOPIC,               // 13
		WL1_H_VISAPIC,					 // 14
		WL1_H_MCPIC,		             // 15
		WL1_H_IDLOGOPIC,                 // 16
		WL1_H_TOPWINDOWPIC,              // 17
		WL1_H_LEFTWINDOWPIC,             // 18
		WL1_H_RIGHTWINDOWPIC,            // 19		
		WL1_H_BOTTOMINFOPIC,			 // 20
		WL1_H_GAMEPIC,					 // 21
		// Lump Start
		WL1_C_OPTIONSPIC,                // 22
		WL1_C_CURSOR1PIC,                // 23
		WL1_C_CURSOR2PIC,                // 24
		WL1_C_NOTSELECTEDPIC,            // 25
		WL1_C_SELECTEDPIC,               // 26
		WL1_C_FXTITLEPIC,                // 27
		WL1_C_DIGITITLEPIC,              // 28
		WL1_C_MUSICTITLEPIC,             // 29
		WL1_C_MOUSELBACKPIC,             // 30
		WL1_C_BABYMODEPIC,               // 31
		WL1_C_EASYPIC,                   // 32
		WL1_C_NORMALPIC,                 // 33
		WL1_C_HARDPIC,                   // 34
		WL1_C_LOADSAVEDISKPIC,           // 35
		WL1_C_DISKLOADING1PIC,           // 36
		WL1_C_DISKLOADING2PIC,           // 37
		WL1_C_CONTROLPIC,                // 38
		WL1_C_CUSTOMIZEPIC,              // 39
		WL1_C_LOADGAMEPIC,               // 40
		WL1_C_SAVEGAMEPIC,               // 41
		WL1_C_EPISODE1PIC,               // 42
		WL1_C_EPISODE2PIC,               // 43
		WL1_C_EPISODE3PIC,               // 44
		WL1_C_EPISODE4PIC,               // 45
		WL1_C_EPISODE5PIC,               // 46
		WL1_C_EPISODE6PIC,               // 47
		WL1_C_CODEPIC,                   // 48
		WL1_C_TIMECODEPIC,				 // 49
		WL1_C_LEVELPIC,					 // 50
		WL1_C_NAMEPIC,					 // 51
		WL1_C_SCOREPIC,					 // 52 
		WL1_C_JOY1PIC,					 // 53
		WL1_C_JOY2PIC,					 // 54
		// Lump Start
		WL1_L_GUYPIC,                    // 55
		WL1_L_COLONPIC,                  // 56
		WL1_L_NUM0PIC,                   // 57
		WL1_L_NUM1PIC,                   // 58
		WL1_L_NUM2PIC,                   // 59
		WL1_L_NUM3PIC,                   // 60
		WL1_L_NUM4PIC,                   // 61
		WL1_L_NUM5PIC,                   // 62
		WL1_L_NUM6PIC,                   // 63
		WL1_L_NUM7PIC,                   // 64
		WL1_L_NUM8PIC,                   // 65
		WL1_L_NUM9PIC,                   // 66
		WL1_L_PERCENTPIC,                // 67
		WL1_L_APIC,                      // 68
		WL1_L_BPIC,                      // 69
		WL1_L_CPIC,                      // 70
		WL1_L_DPIC,                      // 71
		WL1_L_EPIC,                      // 72
		WL1_L_FPIC,                      // 73
		WL1_L_GPIC,                      // 74
		WL1_L_HPIC,                      // 75
		WL1_L_IPIC,                      // 76
		WL1_L_JPIC,                      // 77
		WL1_L_KPIC,                      // 78
		WL1_L_LPIC,                      // 79
		WL1_L_MPIC,                      // 80
		WL1_L_NPIC,                      // 81
		WL1_L_OPIC,                      // 82
		WL1_L_PPIC,                      // 83
		WL1_L_QPIC,                      // 84
		WL1_L_RPIC,                      // 85
		WL1_L_SPIC,                      // 86
		WL1_L_TPIC,                      // 87
		WL1_L_UPIC,                      // 88
		WL1_L_VPIC,                      // 89
		WL1_L_WPIC,                      // 90
		WL1_L_XPIC,                      // 91
		WL1_L_YPIC,                      // 92
		WL1_L_ZPIC,                      // 93
		WL1_L_EXPOINTPIC,				 // 94
		WL1_L_APOSTROPHEPIC,			 // 95
		WL1_L_GUY2PIC,                   // 96
		WL1_L_BJWINSPIC,                 // 97
		WL1_STATUSBARPIC,                // 98
		WL1_TITLEPIC,                    // 99
		WL1_PG13PIC,                     // 100
		WL1_CREDITSPIC,                  // 101
		WL1_HIGHSCORESPIC,               // 102
		// Lump Start
		WL1_KNIFEPIC,                    // 103
		WL1_GUNPIC,                      // 104
		WL1_MACHINEGUNPIC,               // 105
		WL1_GATLINGGUNPIC,               // 106
		WL1_NOKEYPIC,                    // 107
		WL1_GOLDKEYPIC,                  // 108
		WL1_SILVERKEYPIC,                // 109
		WL1_N_BLANKPIC,                  // 110
		WL1_N_0PIC,                      // 111
		WL1_N_1PIC,                      // 112
		WL1_N_2PIC,                      // 113
		WL1_N_3PIC,                      // 114
		WL1_N_4PIC,                      // 115
		WL1_N_5PIC,                      // 116
		WL1_N_6PIC,                      // 117
		WL1_N_7PIC,                      // 118
		WL1_N_8PIC,                      // 119
		WL1_N_9PIC,                      // 120
		WL1_FACE1APIC,                   // 121
		WL1_FACE1BPIC,                   // 122
		WL1_FACE1CPIC,                   // 123
		WL1_FACE2APIC,                   // 124
		WL1_FACE2BPIC,                   // 125
		WL1_FACE2CPIC,                   // 126
		WL1_FACE3APIC,                   // 127
		WL1_FACE3BPIC,                   // 128
		WL1_FACE3CPIC,                   // 129
		WL1_FACE4APIC,                   // 130
		WL1_FACE4BPIC,                   // 131
		WL1_FACE4CPIC,                   // 132
		WL1_FACE5APIC,                   // 133
		WL1_FACE5BPIC,                   // 134
		WL1_FACE5CPIC,                   // 135
		WL1_FACE6APIC,                   // 136
		WL1_FACE6BPIC,                   // 137
		WL1_FACE6CPIC,                   // 138
		WL1_FACE7APIC,                   // 139
		WL1_FACE7BPIC,                   // 140
		WL1_FACE7CPIC,                   // 141
		WL1_FACE8APIC,                   // 142
		WL1_GOTGATLINGPIC,               // 143
		WL1_MUTANTBJPIC,				 // 144
		WL1_PAUSEDPIC,                   // 145
		WL1_GETPSYCHEDPIC,               // 146

		WL1_ENUMEND

} wl1_graphicnums;

typedef enum 
{
		// Lump Start
		H_BJPIC = 3,                          
		H_CASTLEPIC,                         // 4
		H_BLAZEPIC,                          // 5
		H_TOPWINDOWPIC,                      // 6
		H_LEFTWINDOWPIC,                     // 7
		H_RIGHTWINDOWPIC,                    // 8
		H_BOTTOMINFOPIC,                     // 9
		// Lump Start
		C_OPTIONSPIC,                        // 10
		C_CURSOR1PIC,                        // 11
		C_CURSOR2PIC,                        // 12
		C_NOTSELECTEDPIC,                    // 13
		C_SELECTEDPIC,                       // 14
		C_FXTITLEPIC,                        // 15
		C_DIGITITLEPIC,                      // 16
		C_MUSICTITLEPIC,                     // 17
		C_MOUSELBACKPIC,                     // 18
		C_BABYMODEPIC,                       // 19
		C_EASYPIC,                           // 20
		C_NORMALPIC,                         // 21
		C_HARDPIC,                           // 22
		C_LOADSAVEDISKPIC,                   // 23
		C_DISKLOADING1PIC,                   // 24
		C_DISKLOADING2PIC,                   // 25
		C_CONTROLPIC,                        // 26
		C_CUSTOMIZEPIC,                      // 27
		C_LOADGAMEPIC,                       // 28
		C_SAVEGAMEPIC,                       // 29
		C_EPISODE1PIC,                       // 30
		C_EPISODE2PIC,                       // 31
		C_EPISODE3PIC,                       // 32
		C_EPISODE4PIC,                       // 33
		C_EPISODE5PIC,                       // 34
		C_EPISODE6PIC,                       // 35
		C_CODEPIC,                           // 36
		C_TIMECODEPIC,                       // 37
		C_LEVELPIC,                          // 38
		C_NAMEPIC,                           // 39
		C_SCOREPIC,                          // 40
		C_JOY1PIC,                           // 41
		C_JOY2PIC,                           // 42
		// Lump Start
		L_GUYPIC,                            // 43
		L_COLONPIC,                          // 44
		L_NUM0PIC,                           // 45
		L_NUM1PIC,                           // 46
		L_NUM2PIC,                           // 47
		L_NUM3PIC,                           // 48
		L_NUM4PIC,                           // 49
		L_NUM5PIC,                           // 50
		L_NUM6PIC,                           // 51
		L_NUM7PIC,                           // 52
		L_NUM8PIC,                           // 53
		L_NUM9PIC,                           // 54
		L_PERCENTPIC,                        // 55
		L_APIC,                              // 56
		L_BPIC,                              // 57
		L_CPIC,                              // 58
		L_DPIC,                              // 59
		L_EPIC,                              // 60
		L_FPIC,                              // 61
		L_GPIC,                              // 62
		L_HPIC,                              // 63
		L_IPIC,                              // 64
		L_JPIC,                              // 65
		L_KPIC,                              // 66
		L_LPIC,                              // 67
		L_MPIC,                              // 68
		L_NPIC,                              // 69
		L_OPIC,                              // 70
		L_PPIC,                              // 71
		L_QPIC,                              // 72
		L_RPIC,                              // 73
		L_SPIC,                              // 74
		L_TPIC,                              // 75
		L_UPIC,                              // 76
		L_VPIC,                              // 77
		L_WPIC,                              // 78
		L_XPIC,                              // 79
		L_YPIC,                              // 80
		L_ZPIC,                              // 81
		L_EXPOINTPIC,                        // 82
		L_APOSTROPHEPIC,                     // 83
		L_GUY2PIC,                           // 84
		L_BJWINSPIC,                         // 85
		STATUSBARPIC,                        // 86
		TITLEPIC,                            // 87
		PG13PIC,                             // 88
		CREDITSPIC,                          // 89
		HIGHSCORESPIC,                       // 90
		// Lump Start
		KNIFEPIC,                            // 91
		GUNPIC,                              // 92
		MACHINEGUNPIC,                       // 93
		GATLINGGUNPIC,                       // 94
		NOKEYPIC,                            // 95
		GOLDKEYPIC,                          // 96
		SILVERKEYPIC,                        // 97
		N_BLANKPIC,                          // 98
		N_0PIC,                              // 99
		N_1PIC,                              // 100
		N_2PIC,                              // 101
		N_3PIC,                              // 102
		N_4PIC,                              // 103
		N_5PIC,                              // 104
		N_6PIC,                              // 105
		N_7PIC,                              // 106
		N_8PIC,                              // 107
		N_9PIC,                              // 108
		FACE1APIC,                           // 109
		FACE1BPIC,                           // 110
		FACE1CPIC,                           // 111
		FACE2APIC,                           // 112
		FACE2BPIC,                           // 113
		FACE2CPIC,                           // 114
		FACE3APIC,                           // 115
		FACE3BPIC,                           // 116
		FACE3CPIC,                           // 117
		FACE4APIC,                           // 118
		FACE4BPIC,                           // 119
		FACE4CPIC,                           // 120
		FACE5APIC,                           // 121
		FACE5BPIC,                           // 122
		FACE5CPIC,                           // 123
		FACE6APIC,                           // 124
		FACE6BPIC,                           // 125
		FACE6CPIC,                           // 126
		FACE7APIC,                           // 127
		FACE7BPIC,                           // 128
		FACE7CPIC,                           // 129
		FACE8APIC,                           // 130
		GOTGATLINGPIC,                       // 131
		MUTANTBJPIC,                         // 132
		PAUSEDPIC,                           // 133
		GETPSYCHEDPIC,                       // 134

		ORDERSCREEN=136,
		ERRORSCREEN,                         // 137
		T_HELPART,                           // 138
		T_DEMO0,                             // 139
		T_DEMO1,                             // 140
		T_DEMO2,                             // 141
		T_DEMO3,                             // 142
		T_ENDART1,                           // 143
		T_ENDART2,                           // 144
		T_ENDART3,                           // 145
		T_ENDART4,                           // 146
		T_ENDART5,                           // 147
		T_ENDART6,                           // 148
		ENUMEND

} graphicnums;


typedef enum 
{
		// Lump Start
		SDM_C_BACKDROPPIC = 3,
		SDM_C_MOUSELBACKPIC,                     // 4
		SDM_C_CURSOR1PIC,                        // 5
		SDM_C_CURSOR2PIC,                        // 6
		SDM_C_NOTSELECTEDPIC,                    // 7
		SDM_C_SELECTEDPIC,                       // 8
		// Lump Start
		SDM_C_CUSTOMIZEPIC,                      // 9
		SDM_C_JOY1PIC,                           // 10
		SDM_C_JOY2PIC,                           // 11
		SDM_C_MOUSEPIC,                          // 12
		SDM_C_JOYSTICKPIC,                       // 13
		SDM_C_KEYBOARDPIC,                       // 14
		SDM_C_CONTROLPIC,                        // 15
		// Lump Start
		SDM_C_OPTIONSPIC,                        // 16
		// Lump Start
		SDM_C_FXTITLEPIC,                        // 17
		SDM_C_DIGITITLEPIC,                      // 18
		SDM_C_MUSICTITLEPIC,                     // 19
		// Lump Start
		SDM_C_HOWTOUGHPIC,                       // 20
		SDM_C_BABYMODEPIC,                       // 21
		SDM_C_EASYPIC,                           // 22
		SDM_C_NORMALPIC,                         // 23
		SDM_C_HARDPIC,                           // 24
		// Lump Start
		SDM_C_DISKLOADING1PIC,                   // 25
		SDM_C_DISKLOADING2PIC,                   // 26
		SDM_C_LOADGAMEPIC,                       // 27
		SDM_C_SAVEGAMEPIC,                       // 28
		// Lump Start
		SDM_HIGHSCORESPIC,                       // 29
		SDM_C_WONSPEARPIC,                       // 30
		// Lump Start
		SDM_L_GUYPIC,                            // 31
		SDM_L_COLONPIC,                          // 32
		SDM_L_NUM0PIC,                           // 33
		SDM_L_NUM1PIC,                           // 34
		SDM_L_NUM2PIC,                           // 35
		SDM_L_NUM3PIC,                           // 36
		SDM_L_NUM4PIC,                           // 37
		SDM_L_NUM5PIC,                           // 38
		SDM_L_NUM6PIC,                           // 39
		SDM_L_NUM7PIC,                           // 40
		SDM_L_NUM8PIC,                           // 41
		SDM_L_NUM9PIC,                           // 42
		SDM_L_PERCENTPIC,                        // 43
		SDM_L_APIC,                              // 44
		SDM_L_BPIC,                              // 45
		SDM_L_CPIC,                              // 46
		SDM_L_DPIC,                              // 47
		SDM_L_EPIC,                              // 48
		SDM_L_FPIC,                              // 49
		SDM_L_GPIC,                              // 50
		SDM_L_HPIC,                              // 51
		SDM_L_IPIC,                              // 52
		SDM_L_JPIC,                              // 53
		SDM_L_KPIC,                              // 54
		SDM_L_LPIC,                              // 55
		SDM_L_MPIC,                              // 56
		SDM_L_NPIC,                              // 57
		SDM_L_OPIC,                              // 58
		SDM_L_PPIC,                              // 59
		SDM_L_QPIC,                              // 60
		SDM_L_RPIC,                              // 61
		SDM_L_SPIC,                              // 62
		SDM_L_TPIC,                              // 63
		SDM_L_UPIC,                              // 64
		SDM_L_VPIC,                              // 65
		SDM_L_WPIC,                              // 66
		SDM_L_XPIC,                              // 67
		SDM_L_YPIC,                              // 68
		SDM_L_ZPIC,                              // 69
		SDM_L_EXPOINTPIC,                        // 70
		SDM_L_APOSTROPHEPIC,                     // 71
		SDM_L_GUY2PIC,                           // 72
		SDM_L_BJWINSPIC,                         // 73
		// Lump Start
		SDM_TITLE1PIC,                           // 74
		SDM_TITLE2PIC,                           // 75
		SDM_STATUSBARPIC,                        // 76
		SDM_PG13PIC,                             // 77
		SDM_CREDITSPIC,                          // 78
		// Lump Start
		SDM_KNIFEPIC,                            // 79
		SDM_GUNPIC,                              // 80
		SDM_MACHINEGUNPIC,                       // 81
		SDM_GATLINGGUNPIC,                       // 82
		SDM_NOKEYPIC,                            // 83
		SDM_GOLDKEYPIC,                          // 84
		SDM_SILVERKEYPIC,                        // 85
		SDM_N_BLANKPIC,                          // 86
		SDM_N_0PIC,                              // 87
		SDM_N_1PIC,                              // 88
		SDM_N_2PIC,                              // 89
		SDM_N_3PIC,                              // 90
		SDM_N_4PIC,                              // 91
		SDM_N_5PIC,                              // 92
		SDM_N_6PIC,                              // 93
		SDM_N_7PIC,                              // 94
		SDM_N_8PIC,                              // 95
		SDM_N_9PIC,                              // 96
		SDM_FACE1APIC,                           // 97
		SDM_FACE1BPIC,                           // 98
		SDM_FACE1CPIC,                           // 99
		SDM_FACE2APIC,                           // 100
		SDM_FACE2BPIC,                           // 101
		SDM_FACE2CPIC,                           // 102
		SDM_FACE3APIC,                           // 103
		SDM_FACE3BPIC,                           // 104
		SDM_FACE3CPIC,                           // 105
		SDM_FACE4APIC,                           // 106
		SDM_FACE4BPIC,                           // 107
		SDM_FACE4CPIC,                           // 108
		SDM_FACE5APIC,                           // 109
		SDM_FACE5BPIC,                           // 110
		SDM_FACE5CPIC,                           // 111
		SDM_FACE6APIC,                           // 112
		SDM_FACE6BPIC,                           // 113
		SDM_FACE6CPIC,                           // 114
		SDM_FACE7APIC,                           // 115
		SDM_FACE7BPIC,                           // 116
		SDM_FACE7CPIC,                           // 117
		SDM_FACE8APIC,                           // 118
		SDM_GOTGATLINGPIC,                       // 119
		SDM_GODMODEFACE1PIC,                     // 120
		SDM_GODMODEFACE2PIC,                     // 121
		SDM_GODMODEFACE3PIC,                     // 122
		SDM_BJWAITING1PIC,                       // 123
		SDM_BJWAITING2PIC,                       // 124
		SDM_BJOUCHPIC,                           // 125
		SDM_PAUSEDPIC,                           // 126
		SDM_GETPSYCHEDPIC,                       // 127



		SDM_ORDERSCREEN = 129,
		SDM_ERRORSCREEN,                         // 130
		SDM_TITLEPALETTE,                        // 131
		SDM_T_DEMO0,                             // 132
		
		SDM_ENUMEND

} sdm_graphicnums;

typedef enum 
{
		// Lump Start
		SOD_C_BACKDROPPIC = 3,
		SOD_C_MOUSELBACKPIC,                     // 4
		SOD_C_CURSOR1PIC,                        // 5
		SOD_C_CURSOR2PIC,                        // 6
		SOD_C_NOTSELECTEDPIC,                    // 7
		SOD_C_SELECTEDPIC,                       // 8
		// Lump Start
		SOD_C_CUSTOMIZEPIC,                      // 9
		SOD_C_JOY1PIC,                           // 10
		SOD_C_JOY2PIC,                           // 11
		SOD_C_MOUSEPIC,                          // 12
		SOD_C_JOYSTICKPIC,                       // 13
		SOD_C_KEYBOARDPIC,                       // 14
		SOD_C_CONTROLPIC,                        // 15
		// Lump Start
		SOD_C_OPTIONSPIC,                        // 16
		// Lump Start
		SOD_C_FXTITLEPIC,                        // 17
		SOD_C_DIGITITLEPIC,                      // 18
		SOD_C_MUSICTITLEPIC,                     // 19
		// Lump Start
		SOD_C_HOWTOUGHPIC,                       // 20
		SOD_C_BABYMODEPIC,                       // 21
		SOD_C_EASYPIC,                           // 22
		SOD_C_NORMALPIC,                         // 23
		SOD_C_HARDPIC,                           // 24
		// Lump Start
		SOD_C_DISKLOADING1PIC,                   // 25
		SOD_C_DISKLOADING2PIC,                   // 26
		SOD_C_LOADGAMEPIC,                       // 27
		SOD_C_SAVEGAMEPIC,                       // 28
		// Lump Start
		SOD_HIGHSCORESPIC,                       // 29
		SOD_C_WONSPEARPIC,                       // 30
		// Lump Start
		SOD_BJCOLLAPSE1PIC,                      // 31
		SOD_BJCOLLAPSE2PIC,                      // 32
		SOD_BJCOLLAPSE3PIC,                      // 33
		SOD_BJCOLLAPSE4PIC,                      // 34
		SOD_ENDPICPIC,                           // 35
		// Lump Start
		SOD_L_GUYPIC,                            // 36
		SOD_L_COLONPIC,                          // 37
		SOD_L_NUM0PIC,                           // 38
		SOD_L_NUM1PIC,                           // 39
		SOD_L_NUM2PIC,                           // 40
		SOD_L_NUM3PIC,                           // 41
		SOD_L_NUM4PIC,                           // 42
		SOD_L_NUM5PIC,                           // 43
		SOD_L_NUM6PIC,                           // 44
		SOD_L_NUM7PIC,                           // 45
		SOD_L_NUM8PIC,                           // 46
		SOD_L_NUM9PIC,                           // 47
		SOD_L_PERCENTPIC,                        // 48
		SOD_L_APIC,                              // 49
		SOD_L_BPIC,                              // 50
		SOD_L_CPIC,                              // 51
		SOD_L_DPIC,                              // 52
		SOD_L_EPIC,                              // 53
		SOD_L_FPIC,                              // 54
		SOD_L_GPIC,                              // 55
		SOD_L_HPIC,                              // 56
		SOD_L_IPIC,                              // 57
		SOD_L_JPIC,                              // 58
		SOD_L_KPIC,                              // 59
		SOD_L_LPIC,                              // 60
		SOD_L_MPIC,                              // 61
		SOD_L_NPIC,                              // 62
		SOD_L_OPIC,                              // 63
		SOD_L_PPIC,                              // 64
		SOD_L_QPIC,                              // 65
		SOD_L_RPIC,                              // 66
		SOD_L_SPIC,                              // 67
		SOD_L_TPIC,                              // 68
		SOD_L_UPIC,                              // 69
		SOD_L_VPIC,                              // 70
		SOD_L_WPIC,                              // 71
		SOD_L_XPIC,                              // 72
		SOD_L_YPIC,                              // 73
		SOD_L_ZPIC,                              // 74
		SOD_L_EXPOINTPIC,                        // 75
		SOD_L_APOSTROPHEPIC,                     // 76
		SOD_L_GUY2PIC,                           // 77
		SOD_L_BJWINSPIC,                         // 78
		// Lump Start
		SOD_TITLE1PIC,                           // 79
		SOD_TITLE2PIC,                           // 80
		// Lump Start
		SOD_ENDSCREEN11PIC,                      // 81
		// Lump Start
		SOD_ENDSCREEN12PIC,                      // 82
		SOD_ENDSCREEN3PIC,                       // 83
		SOD_ENDSCREEN4PIC,                       // 84
		SOD_ENDSCREEN5PIC,                       // 85
		SOD_ENDSCREEN6PIC,                       // 86
		SOD_ENDSCREEN7PIC,                       // 87
		SOD_ENDSCREEN8PIC,                       // 88
		SOD_ENDSCREEN9PIC,                       // 89
		SOD_STATUSBARPIC,                        // 90
		SOD_PG13PIC,                             // 91
		SOD_CREDITSPIC,                          // 92
		// Lump Start
		SOD_IDGUYS1PIC,                          // 93
		SOD_IDGUYS2PIC,                          // 94
		// Lump Start
		SOD_COPYPROTTOPPIC,                      // 95
		SOD_COPYPROTBOXPIC,                      // 96
		SOD_BOSSPIC1PIC,                         // 97
		SOD_BOSSPIC2PIC,                         // 98
		SOD_BOSSPIC3PIC,                         // 99
		SOD_BOSSPIC4PIC,                         // 100
		// Lump Start
		SOD_KNIFEPIC,                            // 101
		SOD_GUNPIC,                              // 102
		SOD_MACHINEGUNPIC,                       // 103
		SOD_GATLINGGUNPIC,                       // 104
		SOD_NOKEYPIC,                            // 105
		SOD_GOLDKEYPIC,                          // 106
		SOD_SILVERKEYPIC,                        // 107
		SOD_N_BLANKPIC,                          // 108
		SOD_N_0PIC,                              // 109
		SOD_N_1PIC,                              // 110
		SOD_N_2PIC,                              // 111
		SOD_N_3PIC,                              // 112
		SOD_N_4PIC,                              // 113
		SOD_N_5PIC,                              // 114
		SOD_N_6PIC,                              // 115
		SOD_N_7PIC,                              // 116
		SOD_N_8PIC,                              // 117
		SOD_N_9PIC,                              // 118
		SOD_FACE1APIC,                           // 119
		SOD_FACE1BPIC,                           // 120
		SOD_FACE1CPIC,                           // 121
		SOD_FACE2APIC,                           // 122
		SOD_FACE2BPIC,                           // 123
		SOD_FACE2CPIC,                           // 124
		SOD_FACE3APIC,                           // 125
		SOD_FACE3BPIC,                           // 126
		SOD_FACE3CPIC,                           // 127
		SOD_FACE4APIC,                           // 128
		SOD_FACE4BPIC,                           // 129
		SOD_FACE4CPIC,                           // 130
		SOD_FACE5APIC,                           // 131
		SOD_FACE5BPIC,                           // 132
		SOD_FACE5CPIC,                           // 133
		SOD_FACE6APIC,                           // 134
		SOD_FACE6BPIC,                           // 135
		SOD_FACE6CPIC,                           // 136
		SOD_FACE7APIC,                           // 137
		SOD_FACE7BPIC,                           // 138
		SOD_FACE7CPIC,                           // 139
		SOD_FACE8APIC,                           // 140
		SOD_GOTGATLINGPIC,                       // 141
		SOD_GODMODEFACE1PIC,                     // 142
		SOD_GODMODEFACE2PIC,                     // 143
		SOD_GODMODEFACE3PIC,                     // 144
		SOD_BJWAITING1PIC,                       // 145
		SOD_BJWAITING2PIC,                       // 146
		SOD_BJOUCHPIC,                           // 147
		SOD_PAUSEDPIC,                           // 148
		SOD_GETPSYCHEDPIC,                       // 149

		SOD_ORDERSCREEN = 151,
		SOD_ERRORSCREEN,                         // 152
		SOD_TITLEPALETTE,                        // 153
		SOD_END1PALETTE,                         // 154
		SOD_END2PALETTE,                         // 155
		SOD_END3PALETTE,                         // 156
		SOD_END4PALETTE,                         // 157
		SOD_END5PALETTE,                         // 158
		SOD_END6PALETTE,                         // 159
		SOD_END7PALETTE,                         // 160
		SOD_END8PALETTE,                         // 161
		SOD_END9PALETTE,                         // 162
		SOD_IDGUYSPALETTE,                       // 163
		SOD_T_DEMO0,                             // 164
		SOD_T_DEMO1,                             // 165
		SOD_T_DEMO2,                             // 166
		SOD_T_DEMO3,                             // 167
		SOD_T_ENDART1,                           // 168
		SOD_ENUMEND

} sod_graphicnums;


typedef enum 
{
		CORNER_MUS,              // 0
		DUNGEON_MUS,             // 1
		WARMARCH_MUS,            // 2
		GETTHEM_MUS,             // 3
		HEADACHE_MUS,            // 4
		HITLWLTZ_MUS,            // 5
		INTROCW3_MUS,            // 6
		NAZI_NOR_MUS,            // 7
		NAZI_OMI_MUS,            // 8
		POW_MUS,                 // 9
		SALUTE_MUS,              // 10
		SEARCHN_MUS,             // 11
		SUSPENSE_MUS,            // 12
		VICTORS_MUS,             // 13
		WONDERIN_MUS,            // 14
		FUNKYOU_MUS,             // 15
		ENDLEVEL_MUS,            // 16
		GOINGAFT_MUS,            // 17
		PREGNANT_MUS,            // 18
		ULTIMATE_MUS,            // 19
		NAZI_RAP_MUS,            // 20
		ZEROHOUR_MUS,            // 21
		TWELFTH_MUS,             // 22
		ROSTER_MUS,              // 23
		URAHERO_MUS,             // 24
		VICMARCH_MUS,            // 25
		PACMAN_MUS,              // 26
		LASTMUSIC

} wl6_musicnames;

typedef enum 
{
		SOD_XFUNKIE_MUS,             // 0
		SOD_DUNGEON_MUS,             // 1
		SOD_XDEATH_MUS,              // 2
		SOD_GETTHEM_MUS,             // 3
		SOD_XTIPTOE_MUS,             // 4
		SOD_GOINGAFT_MUS,            // 5
		SOD_URAHERO_MUS,             // 6
		SOD_XTHEEND_MUS,             // 7
		SOD_NAZI_OMI_MUS,            // 8
		SOD_POW_MUS,                 // 9
		SOD_TWELFTH_MUS,             // 10
		SOD_SEARCHN_MUS,             // 11
		SOD_SUSPENSE_MUS,            // 12
		SOD_ZEROHOUR_MUS,            // 13
		SOD_WONDERIN_MUS,            // 14
		SOD_ULTIMATE_MUS,            // 15
		SOD_ENDLEVEL_MUS,            // 16
		SOD_XEVIL_MUS,               // 17
		SOD_XJAZNAZI_MUS,            // 18
		SOD_COPYPRO_MUS,             // 19
		SOD_XAWARD_MUS,              // 20
		SOD_XPUTIT_MUS,              // 21
		SOD_XGETYOU_MUS,             // 22
		SOD_XTOWER2_MUS,             // 23
		SOD_LASTMUSIC

} sod_musicnames;


/////////////////////////////////////////////////////////////////////
//
//	WL1
//
/////////////////////////////////////////////////////////////////////

//
// Data LUMPs
//
#define	WL1_README_LUMP_START		3
#define WL1_README_LUMP_END			25

#define WL1_CONTROLS_LUMP_START		26
#define WL1_CONTROLS_LUMP_END		52

#define WL1_LEVELEND_LUMP_START		53
#define WL1_LEVELEND_LUMP_END		93

#define WL1_LATCHPICS_LUMP_START	99
#define WL1_LATCHPICS_LUMP_END		141



/////////////////////////////////////////////////////////////////////
//
//	WL6
//
/////////////////////////////////////////////////////////////////////

//
// Data LUMPs
//
#define WL6_README_LUMP_START		3
#define WL6_README_LUMP_END			9

#define WL6_CONTROLS_LUMP_START		10
#define WL6_CONTROLS_LUMP_END		42

#define WL6_LEVELEND_LUMP_START		43
#define WL6_LEVELEND_LUMP_END		85

#define WL6_LATCHPICS_LUMP_START	91
#define WL6_LATCHPICS_LUMP_END		134


//
// Amount of each data item
//
#define NUMFONT      2
#define NUMFONTM     0
#define NUMPICM      0
#define NUMSPRITES   0
#define NUMTILE8     72
#define NUMTILE8M    0
#define NUMTILE16    0
#define NUMTILE16M   0
#define NUMTILE32    0
#define NUMTILE32M   0
#define NUMEXTERNS   13
//
// File offsets for data items
//
#define STRUCTPIC    0

#define STARTFONT    1
#define STARTFONTM   3
#define STARTPICS    3
#define STARTPICM    135
#define STARTSPRITES 135
#define STARTTILE8   135
#define STARTTILE8M  136
#define STARTTILE16  136
#define STARTTILE16M 136
#define STARTTILE32  136
#define STARTTILE32M 136
#define STARTEXTERNS 136



/////////////////////////////////////////////////////////////////////
//
//	Spear of Destiny
//
/////////////////////////////////////////////////////////////////////


//
// Amount of each data item
//
#define NUMCHUNKS       169
#define NUMPICS         147
#define SOD_NUMEXTERNS  18
//
// File offsets for data items
//

#define SOD_STARTDIFF   15

////    End SOD






extern _boolean LumpExtractor( const char *fextension, W32 limit, W16 version );
extern _boolean PExtractor( const char *extension, W16 version );
extern _boolean AudioRipper( const char *fextension, W32 start, W32 end, W16 version );

extern _boolean MapRipper( const char *fextension, W16 version );



extern char *GetMusicFileName_WL6( W32 chunk );
extern char *GetMusicFileName_SOD( W32 chunk );


extern char *GetLumpFileName_WL1( W32 chunk );
extern char *GetLumpFileName_WL6( W32 chunk );
extern char *GetLumpFileName_SDM( W32 chunk );
extern char *GetLumpFileName_SOD( W32 chunk );


#endif /* __WOLF_DEF_H__ */

