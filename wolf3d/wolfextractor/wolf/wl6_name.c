/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>

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
 *	wl6_name.c:   Convert chunk number to string name.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#include <stdlib.h>

#include "wolf_def.h"
#include "../../../common/arch.h"
#include "../../../common/common_utils.h"



/*
-----------------------------------------------------------------------------
 Function: GetLumpFileName_WL1() -Returns lump name string.
 
 Parameters: chunk -[in] Chunk value to get string name for.
 
 Returns: NULL on error, otherwise string name.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC char *GetLumpFileName_WL1( W32 chunk )
{
    switch( chunk )
    {
		case WL1_H_BJPIC: return "H_BJPIC";
        case WL1_H_CASTLEPIC: return "H_CASTLEPIC";
		case WL1_H_KEYBOARDPIC: return "H_KEYBOARDPIC";
		case WL1_H_CONTROLPIC: return "H_CONTROLPIC";
		case WL1_H_HEALPIC: return "H_HEALPIC";
		case WL1_H_BGPIC: return "H_BGPIC";
		case WL1_H_GUNPIC: return "H_GUNPIC";
		case WL1_H_KEYPIC: return "H_KEYPIC";
		case WL1_H_BLAZEPIC: return "H_BLAZEPIC";
		case WL1_H_WEAPON1234PIC: return "H_WEAPON1234PIC";
		case WL1_H_WOLFLOGOPIC: return "H_WOLFLOGOPIC";
		case WL1_H_VISAPIC: return "H_VISAPIC";
		case WL1_H_MCPIC: return "H_MCPIC";
		case WL1_H_IDLOGOPIC: return "H_IDLOGOPIC";
		case WL1_H_TOPWINDOWPIC: return "H_TOPWINDOWPI";
		case WL1_H_LEFTWINDOWPIC: return "H_LEFTWINDOWPIC";
		case WL1_H_RIGHTWINDOWPIC: return "H_RIGHTWINDOWPIC";
		case WL1_H_BOTTOMINFOPIC: return "H_BOTTOMINFOPIC";
		case WL1_H_GAMEPIC: return "H_GAMEPIC";

		case WL1_C_OPTIONSPIC: return "C_OPTIONSPIC";
		case WL1_C_CURSOR1PIC: return "C_CURSOR0PIC";
		case WL1_C_CURSOR2PIC: return "C_CURSOR1PIC";
		case WL1_C_NOTSELECTEDPIC: return "C_NOTSELECTEDPIC";
		case WL1_C_SELECTEDPIC: return "C_SELECTEDPIC";
		case WL1_C_FXTITLEPIC: return "C_FXTITLEPIC";
		case WL1_C_DIGITITLEPIC: return "C_DIGITITLEPIC";
		case WL1_C_MUSICTITLEPIC: return "C_MUSICTITLEPIC";
		case WL1_C_MOUSELBACKPIC: return "C_MOUSELBACKPIC";
		case WL1_C_BABYMODEPIC: return "C_SKILL1PIC";
		case WL1_C_EASYPIC: return "C_SKILL2PIC";
		case WL1_C_NORMALPIC: return "C_SKILL3PIC";
		case WL1_C_HARDPIC: return "C_SKILL4PIC";
		case WL1_C_LOADSAVEDISKPIC: return "C_LOADSAVEDISKPIC";
		case WL1_C_DISKLOADING1PIC: return "C_DISKLOADING0PIC";
		case WL1_C_DISKLOADING2PIC: return "C_DISKLOADING1PIC";
		case WL1_C_CONTROLPIC: return "C_CONTROLPIC";
		case WL1_C_CUSTOMIZEPIC: return "C_CUSTOMIZEPIC";
		case WL1_C_LOADGAMEPIC: return "C_LOADGAMEPIC";
		case WL1_C_SAVEGAMEPIC: return "C_SAVEGAMEPIC";
		case WL1_C_EPISODE1PIC: return "C_EPISODE1PIC";
		case WL1_C_EPISODE2PIC: return "C_EPISODE2PIC";
		case WL1_C_EPISODE3PIC: return "C_EPISODE3PIC";
		case WL1_C_EPISODE4PIC: return "C_EPISODE4PIC";
		case WL1_C_EPISODE5PIC: return "C_EPISODE5PIC";
		case WL1_C_EPISODE6PIC: return "C_EPISODE6PIC";
		case WL1_C_CODEPIC: return "C_CODEPIC";		
		case WL1_C_TIMECODEPIC: return "C_TIMECODEPIC";
		case WL1_C_LEVELPIC: return "C_LEVELPIC";
		case WL1_C_NAMEPIC: return "C_NAMEPIC";
		case WL1_C_SCOREPIC: return "C_SCOREPIC";
		case WL1_C_JOY1PIC: return "C_JOY1PIC";
		case WL1_C_JOY2PIC: return "C_JOY2PIC";

		case WL1_L_GUYPIC: return "L_GUY0PIC";
		case WL1_L_COLONPIC: 
		case WL1_L_NUM0PIC: 
		case WL1_L_NUM1PIC: 
		case WL1_L_NUM2PIC: 
		case WL1_L_NUM3PIC: 
		case WL1_L_NUM4PIC: 
		case WL1_L_NUM5PIC: 
		case WL1_L_NUM6PIC: 
		case WL1_L_NUM7PIC: 
		case WL1_L_NUM8PIC: 
		case WL1_L_NUM9PIC: 
		case WL1_L_PERCENTPIC: 
		case WL1_L_APIC:
		case WL1_L_BPIC:
		case WL1_L_CPIC:
		case WL1_L_DPIC:
		case WL1_L_EPIC:
		case WL1_L_FPIC:
		case WL1_L_GPIC:
		case WL1_L_HPIC:
		case WL1_L_IPIC:
		case WL1_L_JPIC:
		case WL1_L_KPIC:
		case WL1_L_LPIC:
		case WL1_L_MPIC:
		case WL1_L_NPIC:
		case WL1_L_OPIC:
		case WL1_L_PPIC:
		case WL1_L_QPIC:
		case WL1_L_RPIC:
		case WL1_L_SPIC:
		case WL1_L_TPIC:
		case WL1_L_UPIC:
		case WL1_L_VPIC:
		case WL1_L_WPIC:
		case WL1_L_XPIC:
		case WL1_L_YPIC:
		case WL1_L_ZPIC: 
		case WL1_L_EXPOINTPIC: 
		case WL1_L_APOSTROPHEPIC: return "L_FONTPIC";
		case WL1_L_GUY2PIC: return "L_GUY1PIC";
		case WL1_L_BJWINSPIC: return "L_BJWINSPIC";
		case WL1_STATUSBARPIC: return "STATUSBARPIC";
		case WL1_TITLEPIC: return "TITLEPIC";
		case WL1_PG13PIC: return "PC13PIC";
		case WL1_CREDITSPIC: return "CREDITSPIC";
		case WL1_HIGHSCORESPIC: return "HIGHSCORESPIC";

		case WL1_KNIFEPIC: return "KNIFEPIC";
		case WL1_GUNPIC: return "GUNPIC";
		case WL1_MACHINEGUNPIC: return "MACHINEGUNPIC";
		case WL1_GATLINGGUNPIC: return "GATLINGGUNPIC";
		case WL1_NOKEYPIC: return "NOKEYPIC";
		case WL1_GOLDKEYPIC: return "GOLDKEYPIC";
		case WL1_SILVERKEYPIC: return "SILVERKEYPIC";
		case WL1_N_BLANKPIC: return "N_BLANKPIC";
		case WL1_N_0PIC:
		case WL1_N_1PIC:
		case WL1_N_2PIC:
		case WL1_N_3PIC:
		case WL1_N_4PIC:
		case WL1_N_5PIC:
		case WL1_N_6PIC:
		case WL1_N_7PIC:
		case WL1_N_8PIC:
		case WL1_N_9PIC: return "N_NUMPIC";
		case WL1_FACE1APIC: return "FACE1APIC";
		case WL1_FACE1BPIC: return "FACE1BPIC";
		case WL1_FACE1CPIC: return "FACE1CPIC";
		case WL1_FACE2APIC: return "FACE2APIC";
		case WL1_FACE2BPIC: return "FACE2BPIC";
		case WL1_FACE2CPIC: return "FACE2CPIC";
		case WL1_FACE3APIC: return "FACE3APIC";
		case WL1_FACE3BPIC: return "FACE3BPIC";
		case WL1_FACE3CPIC: return "FACE3CPIC";
		case WL1_FACE4APIC: return "FACE4APIC";
		case WL1_FACE4BPIC: return "FACE4BPIC";
		case WL1_FACE4CPIC: return "FACE4CPIC";
		case WL1_FACE5APIC: return "FACE5APIC";
		case WL1_FACE5BPIC: return "FACE5BPIC";
		case WL1_FACE5CPIC: return "FACE5CPIC";
		case WL1_FACE6APIC: return "FACE6APIC";
		case WL1_FACE6BPIC: return "FACE6BPIC";
		case WL1_FACE6CPIC: return "FACE6CPIC";
		case WL1_FACE7APIC: return "FACE7APIC";
		case WL1_FACE7BPIC: return "FACE7BPIC";
		case WL1_FACE7CPIC: return "FACE7CPIC";
		case WL1_FACE8APIC: return "FACE8APIC";
		case WL1_GOTGATLINGPIC: return "GOTGATLINGPIC";
		case WL1_MUTANTBJPIC: return "MUTANTBJPIC";
		case WL1_PAUSEDPIC: return "PAUSEDPIC";
		case WL1_GETPSYCHEDPIC: return "GETPSYCHEDPIC";		

		default: return NULL;
    } // End switch chunk
}

/*
-----------------------------------------------------------------------------
 Function: GetLumpFileName_WL6() -Returns lump name string.
 
 Parameters: chunk -[in] Chunk value to get string name for.
 
 Returns: NULL on error, otherwise string name.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC char *GetLumpFileName_WL6( W32 chunk )
{
    switch( chunk )
    {
        case H_BJPIC: return "H_BJPIC";
        case H_CASTLEPIC: return "H_CASTLEPIC";
		case H_BLAZEPIC: return "H_BLAZEPIC";
		case H_TOPWINDOWPIC: return "H_TOPWINDOWPIC";
		case H_LEFTWINDOWPIC: return "H_LEFTWINDOWPIC";
		case H_RIGHTWINDOWPIC: return "H_RIGHTWINDOWPIC";
		case H_BOTTOMINFOPIC: return "H_BOTTOMINFOPIC";
		case C_OPTIONSPIC: return "C_OPTIONSPIC";
		case C_CURSOR1PIC: return "C_CURSOR0PIC";
		case C_CURSOR2PIC: return "C_CURSOR1PIC";
		case C_NOTSELECTEDPIC: return "C_NOTSELECTEDPIC";
		case C_SELECTEDPIC: return "C_SELECTEDPIC";
		case C_FXTITLEPIC: return "C_FXTITLEPIC";
		case C_DIGITITLEPIC: return "C_DIGITITLEPIC";
		case C_MUSICTITLEPIC: return "C_MUSICTITLEPIC";
		case C_MOUSELBACKPIC: return "C_MOUSELBACKPIC";
		case C_BABYMODEPIC: return "C_SKILL1PIC";
		case C_EASYPIC: return "C_SKILL2PIC";
		case C_NORMALPIC: return "C_SKILL3PIC";
		case C_HARDPIC: return "C_SKILL4PIC";
		case C_LOADSAVEDISKPIC: return "C_LOADSAVEDISKPIC";
		case C_DISKLOADING1PIC: return "C_DISKLOADING0PIC";
		case C_DISKLOADING2PIC: return "C_DISKLOADING1PIC";
		case C_CONTROLPIC: return "C_CONTROLPIC";
		case C_CUSTOMIZEPIC: return "C_CUSTOMIZEPIC";
		case C_LOADGAMEPIC: return "C_LOADGAMEPIC";
		case C_SAVEGAMEPIC: return "C_SAVEGAMEPIC";
		case C_EPISODE1PIC: return "C_EPISODE1PIC";
		case C_EPISODE2PIC: return "C_EPISODE2PIC";
		case C_EPISODE3PIC: return "C_EPISODE3PIC";
		case C_EPISODE4PIC: return "C_EPISODE4PIC";
		case C_EPISODE5PIC: return "C_EPISODE5PIC";
		case C_EPISODE6PIC: return "C_EPISODE6PIC";
		case C_CODEPIC: return "C_CODEPIC";
		case C_TIMECODEPIC: return "C_TIMECODEPIC";
		case C_LEVELPIC: return "C_LEVELPIC";
		case C_NAMEPIC: return "C_NAMEPIC";
		case C_SCOREPIC: return "C_SCOREPIC";
		case C_JOY1PIC: return "C_JOY1PIC";
		case C_JOY2PIC: return "C_JOY2PIC";
		
		case L_GUYPIC: return "L_GUY0PIC";
		case L_COLONPIC: 
		case L_NUM0PIC: 
		case L_NUM1PIC: 
		case L_NUM2PIC: 
		case L_NUM3PIC: 
		case L_NUM4PIC: 
		case L_NUM5PIC: 
		case L_NUM6PIC: 
		case L_NUM7PIC: 
		case L_NUM8PIC: 
		case L_NUM9PIC: 
		case L_PERCENTPIC: 
		case L_APIC:
		case L_BPIC:
		case L_CPIC:
		case L_DPIC:
		case L_EPIC:
		case L_FPIC:
		case L_GPIC:
		case L_HPIC:
		case L_IPIC:
		case L_JPIC:
		case L_KPIC:
		case L_LPIC:
		case L_MPIC:
		case L_NPIC:
		case L_OPIC:
		case L_PPIC:
		case L_QPIC:
		case L_RPIC:
		case L_SPIC:
		case L_TPIC:
		case L_UPIC:
		case L_VPIC:
		case L_WPIC:
		case L_XPIC:
		case L_YPIC:
		case L_ZPIC: 
		case L_EXPOINTPIC: 
		case L_APOSTROPHEPIC: return "L_FONTPIC";
		case L_GUY2PIC: return "L_GUY1PIC";
		case L_BJWINSPIC: return "L_BJWINSPIC";
		case STATUSBARPIC: return "STATUSBARPIC";
		case TITLEPIC: return "TITLEPIC";
		case PG13PIC: return "PC13PIC";
		case CREDITSPIC: return "CREDITSPIC";
		case HIGHSCORESPIC: return "HIGHSCORESPIC";
		case KNIFEPIC: return "KNIFEPIC";
		case GUNPIC: return "GUNPIC";
		case MACHINEGUNPIC: return "MACHINEGUNPIC";
		case GATLINGGUNPIC: return "GATLINGGUNPIC";
		case NOKEYPIC: return "NOKEYPIC";
		case GOLDKEYPIC: return "GOLDKEYPIC";
		case SILVERKEYPIC: return "SILVERKEYPIC";
		case N_BLANKPIC: return "N_BLANKPIC";
		case N_0PIC:
		case N_1PIC:
		case N_2PIC:
		case N_3PIC:
		case N_4PIC:
		case N_5PIC:
		case N_6PIC:
		case N_7PIC:
		case N_8PIC:
		case N_9PIC: return "N_NUMPIC";
		case FACE1APIC: return "FACE1APIC";
		case FACE1BPIC: return "FACE1BPIC";
		case FACE1CPIC: return "FACE1CPIC";
		case FACE2APIC: return "FACE2APIC";
		case FACE2BPIC: return "FACE2BPIC";
		case FACE2CPIC: return "FACE2CPIC";
		case FACE3APIC: return "FACE3APIC";
		case FACE3BPIC: return "FACE3BPIC";
		case FACE3CPIC: return "FACE3CPIC";
		case FACE4APIC: return "FACE4APIC";
		case FACE4BPIC: return "FACE4BPIC";
		case FACE4CPIC: return "FACE4CPIC";
		case FACE5APIC: return "FACE5APIC";
		case FACE5BPIC: return "FACE5BPIC";
		case FACE5CPIC: return "FACE5CPIC";
		case FACE6APIC: return "FACE6APIC";
		case FACE6BPIC: return "FACE6BPIC";
		case FACE6CPIC: return "FACE6CPIC";
		case FACE7APIC: return "FACE7APIC";
		case FACE7BPIC: return "FACE7BPIC";
		case FACE7CPIC: return "FACE7CPIC";
		case FACE8APIC: return "FACE8APIC";
		case GOTGATLINGPIC: return "GOTGATLINGPIC";
		case MUTANTBJPIC: return "MUTANTBJPIC";
		case PAUSEDPIC: return "PAUSEDPIC";
		case GETPSYCHEDPIC: return "GETPSYCHEDPIC";
		
        default: return NULL;
    } // End switch chunk
}


/*
-----------------------------------------------------------------------------
 Function: GetLumpFileName_SDM() -Returns lump name string.
 
 Parameters: chunk -[in] Chunk value to get string name for.
 
 Returns: NULL on error, otherwise string name.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC char *GetLumpFileName_SDM( W32 chunk )
{
    switch( chunk )
    {
        case SDM_C_BACKDROPPIC: return "C_BACKDROPPIC";
		case SDM_C_MOUSELBACKPIC: return "SC_MOUSELBACKPIC";
		case SDM_C_CURSOR1PIC: return "SC_CURSOR0PIC";
		case SDM_C_CURSOR2PIC: return "SC_CURSOR1PIC";
		case SDM_C_NOTSELECTEDPIC: return "SC_NOTSELECTEDPIC";
		case SDM_C_SELECTEDPIC: return "SC_SELECTEDPIC";

		case SDM_C_CUSTOMIZEPIC: return "SC_CUSTOMIZEPIC";
		case SDM_C_JOY1PIC: return "C_JOY1PIC";
		case SDM_C_JOY2PIC: return "C_JOY2PIC";
		case SDM_C_MOUSEPIC: return "C_MOUSEPIC";
		case SDM_C_JOYSTICKPIC: return "C_JOYSTICKPIC";
		case SDM_C_KEYBOARDPIC: return "C_KEYBOARDPIC";
		case SDM_C_CONTROLPIC: return "SC_CONTROLPIC";

		case SDM_C_OPTIONSPIC: return "SC_OPTIONSPIC";

		case SDM_C_FXTITLEPIC: return "SC_FXTITLEPIC";
		case SDM_C_DIGITITLEPIC: return "SC_DIGITITLEPIC";
		case SDM_C_MUSICTITLEPIC: return "SC_MUSICTITLEPIC";

		case SDM_C_HOWTOUGHPIC: return "C_HOWTOUGHPIC";
		case SDM_C_BABYMODEPIC: return "SC_SKILL1PIC";
		case SDM_C_EASYPIC: return "SC_SKILL2PIC";
		case SDM_C_NORMALPIC: return "SC_SKILL3PIC";
		case SDM_C_HARDPIC: return "SC_SKILL4PIC";

		case SDM_C_DISKLOADING1PIC: return "C_DISKLOADING0PIC";
		case SDM_C_DISKLOADING2PIC: return "C_DISKLOADING1PIC";
		case SDM_C_LOADGAMEPIC: return "SC_LOADGAMEPIC";
		case SDM_C_SAVEGAMEPIC: return "SC_SAVEGAMEPIC";

		case SDM_HIGHSCORESPIC: return "SHIGHSCORESPIC";
		case SDM_C_WONSPEARPIC: return "C_WONSPEARPIC";

		case SDM_L_GUYPIC: return "L_GUY0PIC";
		case SDM_L_COLONPIC:
		case SDM_L_NUM0PIC: 
		case SDM_L_NUM1PIC: 
		case SDM_L_NUM2PIC: 
		case SDM_L_NUM3PIC: 
		case SDM_L_NUM4PIC: 
		case SDM_L_NUM5PIC: 
		case SDM_L_NUM6PIC: 
		case SDM_L_NUM7PIC: 
		case SDM_L_NUM8PIC: 
		case SDM_L_NUM9PIC:
		case SDM_L_PERCENTPIC:
		case SDM_L_APIC:
		case SDM_L_BPIC:
		case SDM_L_CPIC:
		case SDM_L_DPIC:
		case SDM_L_EPIC:
		case SDM_L_FPIC:
		case SDM_L_GPIC:
		case SDM_L_HPIC:
		case SDM_L_IPIC:
		case SDM_L_JPIC:
		case SDM_L_KPIC:
		case SDM_L_LPIC:
		case SDM_L_MPIC:
		case SDM_L_NPIC:
		case SDM_L_OPIC:
		case SDM_L_PPIC:
		case SDM_L_QPIC:
		case SDM_L_RPIC:
		case SDM_L_SPIC:
		case SDM_L_TPIC:
		case SDM_L_UPIC:
		case SDM_L_VPIC:
		case SDM_L_WPIC:
		case SDM_L_XPIC:
		case SDM_L_YPIC:
		case SDM_L_ZPIC: 
		case SDM_L_EXPOINTPIC:
		case SDM_L_APOSTROPHEPIC: return "L_FONTPIC";
		case SDM_L_GUY2PIC: return "L_GUY1PIC";
		case SDM_L_BJWINSPIC: return "L_BJWINSPIC";

		case SDM_TITLE1PIC: 
		case SDM_TITLE2PIC: return "STITLEPIC";		
		case SDM_STATUSBARPIC: return "STATUSBARPIC";
		case SDM_PG13PIC: return "PC13PIC";
		case SDM_CREDITSPIC: return "SCREDITSPIC";

		case SDM_KNIFEPIC: return "KNIFEPIC";
		case SDM_GUNPIC: return "GUNPIC";
		case SDM_MACHINEGUNPIC: return "MACHINEGUNPIC";
		case SDM_GATLINGGUNPIC: return "GATLINGGUNPIC";
		case SDM_NOKEYPIC: return "NOKEYPIC";
		case SDM_GOLDKEYPIC: return "GOLDKEYPIC";
		case SDM_SILVERKEYPIC: return "SILVERKEYPIC";
		case SDM_N_BLANKPIC: return "N_BLANKPIC";
		case SDM_N_0PIC:
		case SDM_N_1PIC:
		case SDM_N_2PIC:
		case SDM_N_3PIC:
		case SDM_N_4PIC:
		case SDM_N_5PIC:
		case SDM_N_6PIC:
		case SDM_N_7PIC:
		case SDM_N_8PIC:
		case SDM_N_9PIC: return "N_NUMPIC";
		case SDM_FACE1APIC: return "FACE1APIC";
		case SDM_FACE1BPIC: return "FACE1BPIC";
		case SDM_FACE1CPIC: return "FACE1CPIC";
		case SDM_FACE2APIC: return "FACE2APIC";
		case SDM_FACE2BPIC: return "FACE2BPIC";
		case SDM_FACE2CPIC: return "FACE2CPIC";
		case SDM_FACE3APIC: return "FACE3APIC";
		case SDM_FACE3BPIC: return "FACE3BPIC";
		case SDM_FACE3CPIC: return "FACE3CPIC";
		case SDM_FACE4APIC: return "FACE4APIC";
		case SDM_FACE4BPIC: return "FACE4BPIC";
		case SDM_FACE4CPIC: return "FACE4CPIC";
		case SDM_FACE5APIC: return "FACE5APIC";
		case SDM_FACE5BPIC: return "FACE5BPIC";
		case SDM_FACE5CPIC: return "FACE5CPIC";
		case SDM_FACE6APIC: return "FACE6APIC";
		case SDM_FACE6BPIC: return "FACE6BPIC";
		case SDM_FACE6CPIC: return "FACE6CPIC";
		case SDM_FACE7APIC: return "FACE7APIC";
		case SDM_FACE7BPIC: return "FACE7BPIC";
		case SDM_FACE7CPIC: return "FACE7CPIC";
		case SDM_FACE8APIC: return "FACE8APIC";
		case SDM_GOTGATLINGPIC: return "GOTGATLINGPIC";
		case SDM_GODMODEFACE1PIC: return "GODMODEFACE0PIC";
		case SDM_GODMODEFACE2PIC: return "GODMODEFACE1PIC";
		case SDM_GODMODEFACE3PIC: return "GODMODEFACE2PIC";
		case SDM_BJWAITING1PIC: return "BJWAITING0PIC";
		case SDM_BJWAITING2PIC: return "BJWAITING1PIC";
		case SDM_BJOUCHPIC: return "BJOUCHPIC";
		case SDM_PAUSEDPIC: return "PAUSEDPIC";
		case SDM_GETPSYCHEDPIC: return "GETPSYCHEDPIC";
		
        default: return NULL;
    
	} // End switch chunk
}

/*
-----------------------------------------------------------------------------
 Function: GetLumpFileName_SOD() -Returns lump name string.
 
 Parameters: chunk -[in] Chunk value to get string name for.
 
 Returns: NULL on error, otherwise string name.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC char *GetLumpFileName_SOD( W32 chunk )
{
    switch( chunk )
    {
        case SOD_C_BACKDROPPIC: return "C_BACKDROPPIC";
		case SOD_C_MOUSELBACKPIC: return "SC_MOUSELBACKPIC";
		case SOD_C_CURSOR1PIC: return "SC_CURSOR0PIC";
		case SOD_C_CURSOR2PIC: return "SC_CURSOR1PIC";
		case SOD_C_NOTSELECTEDPIC: return "SC_NOTSELECTEDPIC";
		case SOD_C_SELECTEDPIC: return "SC_SELECTEDPIC";
		case SOD_C_CUSTOMIZEPIC: return "SC_CUSTOMIZEPIC";
		case SOD_C_JOY1PIC: return "C_JOY1PIC";
		case SOD_C_JOY2PIC: return "C_JOY2PIC";
		case SOD_C_MOUSEPIC: return "C_MOUSEPIC";
		case SOD_C_JOYSTICKPIC: return "C_JOYSTICKPIC";
		case SOD_C_KEYBOARDPIC: return "C_KEYBOARDPIC";
		case SOD_C_CONTROLPIC: return "SC_CONTROLPIC";
		case SOD_C_OPTIONSPIC: return "SC_OPTIONSPIC";
		case SOD_C_FXTITLEPIC: return "SC_FXTITLEPIC";
		case SOD_C_DIGITITLEPIC: return "SC_DIGITITLEPIC";
		case SOD_C_MUSICTITLEPIC: return "SC_MUSICTITLEPIC";
		case SOD_C_HOWTOUGHPIC: return "C_HOWTOUGHPIC";
		case SOD_C_BABYMODEPIC: return "SC_SKILL1PIC";
		case SOD_C_EASYPIC: return "SC_SKILL2PIC";
		case SOD_C_NORMALPIC: return "SC_SKILL3PIC";
		case SOD_C_HARDPIC: return "SC_SKILL4PIC";
		case SOD_C_DISKLOADING1PIC: return "C_DISKLOADING0PIC";
		case SOD_C_DISKLOADING2PIC: return "C_DISKLOADING1PIC";
		case SOD_C_LOADGAMEPIC: return "SC_LOADGAMEPIC";
		case SOD_C_SAVEGAMEPIC: return "SC_SAVEGAMEPIC";
		case SOD_HIGHSCORESPIC: return "SHIGHSCORESPIC";
		case SOD_C_WONSPEARPIC: return "C_WONSPEARPIC";
		case SOD_BJCOLLAPSE1PIC: return "BJCOLLAPSE1PIC";
		case SOD_BJCOLLAPSE2PIC: return "BJCOLLAPSE2PIC";
		case SOD_BJCOLLAPSE3PIC: return "BJCOLLAPSE3PIC";
		case SOD_BJCOLLAPSE4PIC: return "BJCOLLAPSE4PIC";
		case SOD_ENDPICPIC: return "ENDPICPIC";
		case SOD_L_GUYPIC: return "L_GUY0PIC";
		case SOD_L_COLONPIC:
		case SOD_L_NUM0PIC: 
		case SOD_L_NUM1PIC: 
		case SOD_L_NUM2PIC: 
		case SOD_L_NUM3PIC: 
		case SOD_L_NUM4PIC: 
		case SOD_L_NUM5PIC: 
		case SOD_L_NUM6PIC: 
		case SOD_L_NUM7PIC: 
		case SOD_L_NUM8PIC: 
		case SOD_L_NUM9PIC:
		case SOD_L_PERCENTPIC:
		case SOD_L_APIC:
		case SOD_L_BPIC:
		case SOD_L_CPIC:
		case SOD_L_DPIC:
		case SOD_L_EPIC:
		case SOD_L_FPIC:
		case SOD_L_GPIC:
		case SOD_L_HPIC:
		case SOD_L_IPIC:
		case SOD_L_JPIC:
		case SOD_L_KPIC:
		case SOD_L_LPIC:
		case SOD_L_MPIC:
		case SOD_L_NPIC:
		case SOD_L_OPIC:
		case SOD_L_PPIC:
		case SOD_L_QPIC:
		case SOD_L_RPIC:
		case SOD_L_SPIC:
		case SOD_L_TPIC:
		case SOD_L_UPIC:
		case SOD_L_VPIC:
		case SOD_L_WPIC:
		case SOD_L_XPIC:
		case SOD_L_YPIC:
		case SOD_L_ZPIC: 
		case SOD_L_EXPOINTPIC:
		case SOD_L_APOSTROPHEPIC: return "L_FONTPIC";
		case SOD_L_GUY2PIC: return "L_GUY1PIC";
		case SOD_L_BJWINSPIC: return "L_BJWINSPIC";
		case SOD_TITLE1PIC: 
		case SOD_TITLE2PIC: return "STITLEPIC";
		case SOD_ENDSCREEN11PIC: return "ENDSCREEN11PIC";
		case SOD_ENDSCREEN12PIC: return "ENDSCREEN12PIC";
		case SOD_ENDSCREEN3PIC: return "ENDSCREEN3PIC";
		case SOD_ENDSCREEN4PIC: return "ENDSCREEN4PIC";
		case SOD_ENDSCREEN5PIC: return "ENDSCREEN5PIC";
		case SOD_ENDSCREEN6PIC: return "ENDSCREEN6PIC";
		case SOD_ENDSCREEN7PIC: return "ENDSCREEN7PIC";
		case SOD_ENDSCREEN8PIC: return "ENDSCREEN8PIC";
		case SOD_ENDSCREEN9PIC: return "ENDSCREEN9PIC";
		case SOD_STATUSBARPIC: return "STATUSBARPIC";
		case SOD_PG13PIC: return "PC13PIC";
		case SOD_CREDITSPIC: return "SCREDITSPIC";
		case SOD_IDGUYS1PIC: 
		case SOD_IDGUYS2PIC: return "IDGUYSPIC";
		case SOD_COPYPROTTOPPIC: return "COPYPROTTOPPIC";
		case SOD_COPYPROTBOXPIC: return "COPYPROTBOXPIC";
		case SOD_BOSSPIC1PIC: return "BOSSPIC1PIC";
		case SOD_BOSSPIC2PIC: return "BOSSPIC2PIC";
		case SOD_BOSSPIC3PIC: return "BOSSPIC3PIC";
		case SOD_BOSSPIC4PIC: return "BOSSPIC4PIC";
		case SOD_KNIFEPIC: return "KNIFEPIC";
		case SOD_GUNPIC: return "GUNPIC";
		case SOD_MACHINEGUNPIC: return "MACHINEGUNPIC";
		case SOD_GATLINGGUNPIC: return "GATLINGGUNPIC";
		case SOD_NOKEYPIC: return "NOKEYPIC";
		case SOD_GOLDKEYPIC: return "GOLDKEYPIC";
		case SOD_SILVERKEYPIC: return "SILVERKEYPIC";
		case SOD_N_BLANKPIC: return "N_BLANKPIC";
		case SOD_N_0PIC:
		case SOD_N_1PIC:
		case SOD_N_2PIC:
		case SOD_N_3PIC:
		case SOD_N_4PIC:
		case SOD_N_5PIC:
		case SOD_N_6PIC:
		case SOD_N_7PIC:
		case SOD_N_8PIC:
		case SOD_N_9PIC: return "N_NUMPIC";
		case SOD_FACE1APIC: return "FACE1APIC";
		case SOD_FACE1BPIC: return "FACE1BPIC";
		case SOD_FACE1CPIC: return "FACE1CPIC";
		case SOD_FACE2APIC: return "FACE2APIC";
		case SOD_FACE2BPIC: return "FACE2BPIC";
		case SOD_FACE2CPIC: return "FACE2CPIC";
		case SOD_FACE3APIC: return "FACE3APIC";
		case SOD_FACE3BPIC: return "FACE3BPIC";
		case SOD_FACE3CPIC: return "FACE3CPIC";
		case SOD_FACE4APIC: return "FACE4APIC";
		case SOD_FACE4BPIC: return "FACE4BPIC";
		case SOD_FACE4CPIC: return "FACE4CPIC";
		case SOD_FACE5APIC: return "FACE5APIC";
		case SOD_FACE5BPIC: return "FACE5BPIC";
		case SOD_FACE5CPIC: return "FACE5CPIC";
		case SOD_FACE6APIC: return "FACE6APIC";
		case SOD_FACE6BPIC: return "FACE6BPIC";
		case SOD_FACE6CPIC: return "FACE6CPIC";
		case SOD_FACE7APIC: return "FACE7APIC";
		case SOD_FACE7BPIC: return "FACE7BPIC";
		case SOD_FACE7CPIC: return "FACE7CPIC";
		case SOD_FACE8APIC: return "FACE8APIC";
		case SOD_GOTGATLINGPIC: return "GOTGATLINGPIC";
		case SOD_GODMODEFACE1PIC: return "GODMODEFACE0PIC";
		case SOD_GODMODEFACE2PIC: return "GODMODEFACE1PIC";
		case SOD_GODMODEFACE3PIC: return "GODMODEFACE2PIC";
		case SOD_BJWAITING1PIC: return "BJWAITING0PIC";
		case SOD_BJWAITING2PIC: return "BJWAITING1PIC";
		case SOD_BJOUCHPIC: return "BJOUCHPIC";
		case SOD_PAUSEDPIC: return "PAUSEDPIC";
		case SOD_GETPSYCHEDPIC: return "GETPSYCHEDPIC";
		
        default: return NULL;
    
	} // End switch chunk

}





PUBLIC char *GetMusicFileName_WL6( W32 chunk )
{
    switch( chunk )
    {
		case CORNER_MUS: return "CORNER";
		case DUNGEON_MUS: return "DUNGEON";
		case WARMARCH_MUS: return "WARMARCH";
		case GETTHEM_MUS: return "GETTHEM";
		case HEADACHE_MUS: return "HEADACHE";
		case HITLWLTZ_MUS: return "HITLWLTZ";
		case INTROCW3_MUS: return "INTROCW3";
		case NAZI_NOR_MUS: return "NAZI_NOR";
		case NAZI_OMI_MUS: return "NAZI_OMI";
		case POW_MUS: return "POW";
		case SALUTE_MUS: return "SALUTE";
		case SEARCHN_MUS: return "SEARCHN";
		case SUSPENSE_MUS: return "SUSPENSE";
		case VICTORS_MUS: return "VICTORS";
		case WONDERIN_MUS: return "WONDERIN";
		case FUNKYOU_MUS: return "FUNKYOU";
		case ENDLEVEL_MUS: return "ENDLEVEL";
		case GOINGAFT_MUS: return "GOINGAFT";
		case PREGNANT_MUS: return "PREGNANT";
		case ULTIMATE_MUS: return "ULTIMATE";
		case NAZI_RAP_MUS: return "NAZI_RAP";
		case ZEROHOUR_MUS: return "ZEROHOUR";
		case TWELFTH_MUS: return "TWELFTH";
		case ROSTER_MUS: return "ROSTER";
		case URAHERO_MUS: return "URAHERO";
		case VICMARCH_MUS: return "VICMARCH";
		case PACMAN_MUS: return "PACMAN";

        default: return NULL;
    
	} // End switch chunk

}

PUBLIC char *GetMusicFileName_SOD( W32 chunk )
{
    switch( chunk )
    {
		case SOD_XFUNKIE_MUS: return "XFUNKIE";
		case SOD_DUNGEON_MUS: return "DUNGEON";
		case SOD_XDEATH_MUS: return "XDEATH";
		case SOD_GETTHEM_MUS: return "GETTHEM";
		case SOD_XTIPTOE_MUS: return "XTIPTOE";
		case SOD_GOINGAFT_MUS: return "GOINGAFT";
		case SOD_URAHERO_MUS: return "URAHERO";
		case SOD_XTHEEND_MUS: return "XTHEEND";
		case SOD_NAZI_OMI_MUS: return "NAZI_OMI";
		case SOD_POW_MUS: return "POW";
		case SOD_TWELFTH_MUS: return "TWELFTH";
		case SOD_SEARCHN_MUS: return "SEARCHN";
		case SOD_SUSPENSE_MUS: return "SUSPENSE";
		case SOD_ZEROHOUR_MUS: return "ZEROHOUR";
		case SOD_WONDERIN_MUS: return "WONDERIN";
		case SOD_ULTIMATE_MUS: return "ULTIMATE";
		case SOD_ENDLEVEL_MUS: return "ENDLEVEL";
		case SOD_XEVIL_MUS: return "XEVIL";
		case SOD_XJAZNAZI_MUS: return "XJAZNAZI";
		case SOD_COPYPRO_MUS: return "COPYPRO";
		case SOD_XAWARD_MUS: return "XAWARD";
		case SOD_XPUTIT_MUS: return "XPUTIT";
		case SOD_XGETYOU_MUS: return "XGETYOU";
		case SOD_XTOWER2_MUS: return "XTOWER2";

        default: return NULL;
    
	} // End switch chunk

}