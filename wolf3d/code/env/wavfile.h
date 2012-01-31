/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 1997-2001 Id Software, Inc.

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
 *	wav.h: Wav file loader.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	
 *	Date:	2004
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#ifndef __WAV_H__
#define __WAV_H__


// Structure used to describe a sound.
typedef struct 
{
	W32	sample_rate;	// Sample rate in Hz
	W32	channels;		// Number of Channels (0x01 = Mono, 0x02 = Stereo)
	W32	sample_size;	// Bytes per sample	
						//		1 = 8 bit Mono
						//		2 = 8 bit Stereo or 16 bit Mono
						//		4 = 16 bit Stereo
	W32	samples;

} soundInfo_t;


extern _boolean LoadWavInfo( const char *filename, W8 **wav, soundInfo_t *info );

#endif /* __WAV_H__ */
