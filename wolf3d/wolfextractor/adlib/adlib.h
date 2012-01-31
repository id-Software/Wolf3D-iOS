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

/*
 *	adlib.h:	Interface to adlib hardware.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein 3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

 /*
	Notes:

	This module is implemented by adlib.c 
 */

#ifndef __ADLIB_H__
#define __ADLIB_H__


#include "../../../common/arch.h"


#define MAX_WAV_SIZE	100000


typedef	struct
{
    W8   mChar, cChar,
         mScale, cScale,
         mAttack, cAttack,
         mSus, cSus,
         mWave, cWave,
         nConn,

    // These are only for Muse - these bytes are really unused
         voice,
         mode,
         unused[ 3 ];

} Instrument;

typedef	struct
{
    char length[4];
	char priority[2];

} SoundCommon;

typedef	struct
{
    SoundCommon	common;
    Instrument	inst;
    W8   block;
    W8   data[ 1 ];

} AdLibSound;


extern W8 ADLIB_Init();
extern void ADLIB_Shutdown();

extern W8 ADLIB_DecodeSound( AdLibSound *sound, W8 *buffer, W32 *length );



#endif /* __ADLIB_H__ */

