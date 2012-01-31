/*

	Copyright (C) 2004 Michael Liebscher
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
 *	wolf_powerups.h:  Wolfenstein3-D power-up handler.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 */

/*
	Notes:	
	This module is implemented by wolf_powerups.c

*/

#ifndef __WOLF_POWERUPS_H__
#define __WOLF_POWERUPS_H__

typedef enum
{
//please provide description
	pow_gibs,			//  1% if <=10%; SLURPIESND
	pow_gibs2,			//  1% if <=10%; SLURPIESND
	pow_alpo,			//  4% if <100%; HEALTH1SND
	pow_firstaid,		// 25% if <100%; HEALTH2SND
	pow_key1,			// gold key;		GETKEYSND
	pow_key2,			// silver key;	GETKEYSND
	pow_key3,			// not used
	pow_key4,			// not used
	pow_cross,			//  100pts; BONUS1SND
	pow_chalice,		//  500pts; BONUS2SND
	pow_bible,			// 1000pts; BONUS3SND
	pow_crown,			// 5000pts; BONUS4SND
	pow_clip,			// 8bul if <99bul; GETAMMOSND
	pow_clip2,			// 4bul if <99bul; GETAMMOSND
	pow_machinegun,		// machine gun; GETMACHINESND
	pow_chaingun,		// gatling gun; GETGATLINGSND
	pow_food,			// 10% if <100%; HEALTH1SND
	pow_fullheal,		// 99%, 25bul; BONUS1UPSND
	pow_25clip,			// 25bul if <99bul; GETAMMOBOXSND
	pow_spear,			// spear of destiny!

	pow_last
// add new types <!only!> here (after last)

} pow_t;

typedef struct powerup_s
{
	int x, y;
	pow_t type;
	int sprite;
} powerup_t;

extern void Powerup_Reset( void );
extern void Powerup_Spawn( int x, int y, int type );
extern void Powerup_PickUp( int x, int y );


#endif /* __WOLF_POWERUPS_H__ */

