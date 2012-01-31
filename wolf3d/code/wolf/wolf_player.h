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
 *	wolf_player.h:  Wolfenstein3-D player management.
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
	This module is implemented by wolf_player.c

*/

#ifndef __WOLF_PLAYER_H__
#define __WOLF_PLAYER_H__

#define ITEM_KEY_1 1
#define ITEM_KEY_2 2
#define ITEM_KEY_3 4
#define ITEM_KEY_4 8
#define ITEM_WEAPON_1 16
#define ITEM_WEAPON_2 32
#define ITEM_WEAPON_3 64
#define ITEM_WEAPON_4 128
#define ITEM_WEAPON_5 256
#define ITEM_WEAPON_6 512
#define ITEM_WEAPON_7 1024
#define ITEM_WEAPON_8 2048
#define ITEM_BACKPACK (1<<12) // doubles carrying capacity
#define ITEM_AUGMENT	(1<<13) // adds 50 to maximum health
#define ITEM_UNIFORM  (1<<14) // allows you to pass guards
#define ITEM_AUTOMAP  (1<<15)	// shows unknown map ares in other color (as in DooM)
#define ITEM_FREE			(1<<16)	// - unused -


enum weapon_e
{
	WEAPON_KNIFE,
	WEAPON_PISTOL,
	WEAPON_AUTO,
	WEAPON_CHAIN,

	WEAPON_TYPES
};

enum key_e
{
	KEY_GOLD,
	KEY_SILVER,
	KEY_FREE1,
	KEY_FREE2,

	KEY_TYPES
};

enum ammo_e
{
	AMMO_BULLETS,

	AMMO_TYPES
};



// flags
#define PL_FLAG_REUSE		1 // use button pressed
#define PL_FLAG_ATTCK		2 // attacking
// debug (cheat codes) flags
#define FL_GODMODE			(1<<4)
#define FL_NOTARGET	(1<<6)


typedef	enum state_e
{
	ex_notingame,
	ex_playing,
	ex_dead,
	ex_secretlevel,
	ex_victory,
	ex_complete
/*
	ex_stillplaying,
	ex_completed,
	ex_died,
	ex_warped,
	ex_resetgame,
	ex_loadedgame,
	ex_victorious,
	ex_abort,
	ex_demodone,
	ex_secretlevel
*/
} state_t;

// ------------------------- * types * -------------------------
// Player structure: Holds all info about player
typedef struct player_s
{
	usercmd_t	cmd;		// movement / action command
	
	placeonplane_t position; // player position
	int movx, movy, speed;
	int tilex, tiley;

// stats
	int health, lives, frags;
	int armor;	// there are 2 types. The better one is indicated by high bit set
	int ammo[AMMO_TYPES];
	int old_score, score, next_extra;
	unsigned items; // (keys, weapon)
	int weapon, pendingweapon;
	int previousweapon; //gsh
// additional info
	int attackframe, attackcount, weaponframe; // attack info
	unsigned flags;
	int areanumber;

	_boolean madenoise;	// FIXME: move to flags?
	entity_t *LastAttacker;
	int faceframe, facecount;	// bj's face in the HUD // FIXME decide something!
	_boolean face_gotgun, face_ouch;
	state_t playstate; // fixme: move to gamestate

} player_t;

extern player_t Player;




extern void PL_Spawn( placeonplane_t location, LevelData_t *lvl );

extern void PL_Process( player_t *self, LevelData_t *lvl );

extern void PL_Damage( player_t *self, entity_t *attacker, int points );
extern _boolean PL_GiveHealth( player_t *self, int points, int max );
extern _boolean PL_GiveAmmo( player_t *self, int type, int ammo );
extern void PL_GiveWeapon( player_t *self, int weapon );
extern void PL_GivePoints( player_t *self, W32 points );
extern void PL_GiveKey( player_t *self, int key );

extern void PL_NewGame( player_t *self );
extern void PL_NextLevel( player_t *self );
extern _boolean PL_Reborn( player_t *self );

extern void PL_Init( void );
extern void PL_Reset( void );


extern void fire_hit( player_t *self );
extern void fire_lead( player_t *self );


#endif /* __WOLF_PLAYER_H__ */
