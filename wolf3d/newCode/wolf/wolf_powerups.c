/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 2000-2001 by DarkOne the Hacker

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
 *	wolf_pushwalls.c:  Wolfenstein3-D power-up handler.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	This code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker.
 *
 */

#include "../wolfiphone.h"

typedef struct powerup_s
{
	int x, y;
	pow_t type;
	int sprite;
	struct powerup_s *prev, *next;

} powerup_t;

powerup_t *powerups = NULL;


int Pow_Texture[ pow_last ] =
{
	SPR_STAT_34,	// pow_gibs
	SPR_STAT_38,	// pow_gibs2
	SPR_STAT_6,		// pow_alpo
	SPR_STAT_25,	// pow_firstaid
	SPR_STAT_20,	// pow_key1
	SPR_STAT_21,	// pow_key2
// not used
	SPR_STAT_20,	// pow_key3
	SPR_STAT_20,	// pow_key4

	SPR_STAT_29,	// pow_cross
	SPR_STAT_30,	// pow_chalice
	SPR_STAT_31,	// pow_bible
	SPR_STAT_32,	// pow_crown
	SPR_STAT_26,	// pow_clip
	SPR_STAT_26,	// pow_clip2
	SPR_STAT_27,	// pow_machinegun
	SPR_STAT_28,	// pow_chaingun
	SPR_STAT_24,	// pow_food
	SPR_STAT_33,	// pow_fullheal
// spear
	SPR_STAT_49,	// pow_25clip
	SPR_STAT_51,	// pow_spear
};





/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE powerup_t *Pow_Remove( powerup_t *powerup )
{
	powerup_t *next;

	if( powerup == NULL ) 
		return NULL;

	if( powerup->prev ) 
		powerup->prev->next = powerup->next;

	if( powerup->next ) 
		powerup->next->prev = powerup->prev;

	next = powerup->next;
	if( powerups == powerup ) 
		powerups = next; //fuck!

	MM_FREE( powerup );

	return next;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE powerup_t *Pow_AddNew( void )
{
	powerup_t *newp;
	
	newp = MM_MALLOC( sizeof( powerup_t ) );
	newp->prev = NULL;
	newp->next = powerups;

	if( powerups )
	{
		powerups->prev = newp;
	}

	powerups = newp;

	return newp;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Powerup_Reset( void )
{
	powerup_t *powerup = powerups;

	while( powerup )
	{
		powerup = Pow_Remove( powerup );
	}
	
	powerups = NULL;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: 1 if powerup is picked up, otherwise 0.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE int Pow_Give( pow_t type )
{
	static const char *keynames[] = { "Gold", "Silver", "?", "?" };

	switch( type )
	{
//
// Keys
//
		case pow_key1:
		case pow_key2:
		case pow_key3:
		case pow_key4:
			type -= pow_key1;
			PL_GiveKey( &Player, type );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/012.wav" ), 1, ATTN_NORM, 0 );
			iphoneSetNotifyText( "%s key\n", keynames[ type ] );
			break;
//
// Treasure
//
		case pow_cross:
			PL_GiveHealth( &Player, 1, 150 );	// iphone -- trasure acts as health crumbs
			PL_GivePoints( &Player, 100 );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/035.wav" ), 1, ATTN_NORM, 0 );
			if ( ++levelstate.found_treasure == levelstate.total_treasure ) {
				iphoneSetNotifyText( "You found the last treasure!" );
			}
			break;

		case pow_chalice:
			PL_GiveHealth( &Player, 1, 150 );	// iphone -- trasure acts as health crumbs
			PL_GivePoints( &Player, 500 );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/036.wav" ), 1, ATTN_NORM, 0 );
			if ( ++levelstate.found_treasure == levelstate.total_treasure ) {
				iphoneSetNotifyText( "You found the last treasure!" );
			}
			break;

		case pow_bible:
			PL_GiveHealth( &Player, 1, 150 );	// iphone -- trasure acts as health crumbs
			PL_GivePoints( &Player, 1000 );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/037.wav" ), 1, ATTN_NORM, 0 );
			if ( ++levelstate.found_treasure == levelstate.total_treasure ) {
				iphoneSetNotifyText( "You found the last treasure!" );
			}
			break;

		case pow_crown:
			PL_GiveHealth( &Player, 1, 150 );	// iphone -- trasure acts as health crumbs
			PL_GivePoints( &Player, 5000 );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/045.wav" ), 1, ATTN_NORM, 0 );
			if ( ++levelstate.found_treasure == levelstate.total_treasure ) {
				iphoneSetNotifyText( "You found the last treasure!" );
			}
			break;

//
// Health
//
		case pow_gibs:
			if( ! PL_GiveHealth( &Player, 1, 11 ) )
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/061.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_alpo:
			if( ! PL_GiveHealth( &Player, 4, 0 ) )
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/033.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_food:
			if( ! PL_GiveHealth( &Player, 10, 0 ) ) 
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/033.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_firstaid:
			if( ! PL_GiveHealth( &Player, 25, 0 ) )
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/034.wav" ), 1, ATTN_NORM, 0 );
			break;

//
// Weapon & Ammo
//
		case pow_clip:
			if( ! PL_GiveAmmo( &Player, AMMO_BULLETS, 8 ) ) 
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/031.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_clip2:
			if( ! PL_GiveAmmo( &Player, AMMO_BULLETS, 4 ) ) 
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/031.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_25clip:
			if( ! PL_GiveAmmo( &Player, AMMO_BULLETS, 25 ) ) 
			{
				return 0;
			}
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/064.wav" ), 1, ATTN_NORM, 0 );
			break;

		case pow_machinegun:
			PL_GiveWeapon( &Player, WEAPON_AUTO );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/030.wav" ), 1, ATTN_NORM, 0 );
			iphoneSetNotifyText( "Machinegun" );
			break;

		case pow_chaingun:
			PL_GiveWeapon( &Player, WEAPON_CHAIN );
			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "lsfx/038.wav" ), 1, ATTN_NORM, 0 );
			iphoneSetNotifyText( "Chaingun" );

			Player.facecount = -100;
			Player.face_gotgun = true;
			break;

//
// Artifacts
//
		case pow_fullheal:
			PL_GiveHealth( &Player, 999, 0 );
			PL_GiveAmmo( &Player, AMMO_BULLETS, 25 );
			PL_GiveLife( &Player );
			if ( ++levelstate.found_treasure == levelstate.total_treasure ) {
				iphoneSetNotifyText( "You found the last treasure!" );
			} else {
				iphoneSetNotifyText( "Full Heal" );
			}
			// no extra lives on iPhone			Com_Printf( "Extra life!\n" );
			break;

		case pow_spear:
			{
			char szTextMsg[ 256 ];

			Sound_StartSound( NULL, 0, CHAN_ITEM, Sound_RegisterSound( "sodsfx/109.wav" ), 1, ATTN_NORM, 0 );
			iphoneSetNotifyText( "Spear of Destiny" );

			my_snprintf( szTextMsg, sizeof( szTextMsg ), 
				"loading ; map s%.2d.map\n", 20 );
			Cbuf_AddText( szTextMsg );
			}
			break;

		default:
			Com_DPrintf( "Warning: Unknown item type: %d\n", type );
			break;
	}

	iphoneStartBonusFlash();
	
	return 1;
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: x, y -[in] In are in TILES.
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Powerup_Spawn( int x, int y, int type, LevelData_t *lvl )
{
	powerup_t *newp;

	lvl->tilemap[ x ][ y ] |= POWERUP_TILE;
	newp = Pow_AddNew();
	newp->sprite = Sprite_GetNewSprite();
	Sprite_SetPos( newp->sprite, TILE2POS( newp->x = x ), TILE2POS( newp->y = y ), 0 );
	newp->type = type;
	Sprite_SetTex( newp->sprite, -1, Pow_Texture[ type ] );
	lvl->tilemap[ x ][ y ] |= POWERUP_TILE;
// good place to update total treasure count!
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:	x, y -[in] In are in TILES.
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Powerup_PickUp( int x, int y )
{
	powerup_t *pow;
	_boolean p_left = false, p_pick = false;

	for( pow = powerups ; pow ; pow = pow->next )
	{
check_again:
		if( pow->x == x && pow->y == y)
		{// got a powerup here
			if( Pow_Give( pow->type ) ) //FIXME script
			{// picked up this stuff, remove it!
				p_pick = true;
				Sprite_RemoveSprite( pow->sprite );
				pow = Pow_Remove( pow );
				if( pow )
					goto check_again;
				else
					break;
			}
			else
			{// player do not need it, so may be next time!
				p_left = true;
			}
		}
	}

	if( p_left )
	{
		r_world->tilemap[ x ][ y ] |= POWERUP_TILE;
	}
	else
	{
		r_world->tilemap[ x ][ y ] &= ~POWERUP_TILE;
	}
}

