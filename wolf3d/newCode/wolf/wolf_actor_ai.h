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
 *	wolf_actor_ai.h:  Wolfenstein3-D entity management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 *	Portion of this code was derived from Wolfenstein3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

/*
	Notes:	
	This module is implemented by wolf_actor_ai.c.

*/

#ifndef __WOLF_ACTOR_AI_H__
#define __WOLF_ACTOR_AI_H__

#include "wolf_actors.h"

extern void A_DeathScream( entity_t *self );
extern void A_FirstSighting( entity_t *self );
extern void A_DamageActor( entity_t *self, int damage );

// hitler
extern void A_MechaSound( entity_t *self );
extern void A_Slurpie( entity_t *self );
extern void A_HitlerMorph( entity_t *self );

// angel
extern void A_Breathing( entity_t *self );
extern void A_StartAttack( entity_t *self );
extern void A_Relaunch( entity_t *self );
extern void A_Victory( entity_t *self );

// ghost
extern void A_Dormant( entity_t *self );

extern void A_StartDeathCam( entity_t *self );

// missiles
extern void T_Projectile( entity_t *self );
extern void A_Smoke( entity_t *self );


#endif /* __WOLF_ACTOR_AI_H__ */
