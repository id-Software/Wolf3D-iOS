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
 *	wolf_ai_com.h:  Wolfenstein3-D entity management.
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
	This module is implemented by wolf_ai_com.c

*/

#ifndef __WOLF_AI_COM_H__
#define __WOLF_AI_COM_H__

#include "wolf_actors.h"

// common AI functions

extern void T_Stand( entity_t *self );
extern void T_Path( entity_t *self );
extern void T_Ghosts( entity_t *self );
extern void T_Chase( entity_t *self );
extern void T_Bite( entity_t *self );
extern void T_DogChase( entity_t *self );
extern void T_BossChase( entity_t *self );
extern void T_Fake( entity_t *self );

extern void T_Shoot( entity_t *self );
extern void T_UShoot( entity_t *self );
extern void T_Launch( entity_t *self );


#endif /* __WOLF_AI_COM_H__ */

