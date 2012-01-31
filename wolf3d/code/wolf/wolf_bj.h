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
 *	wolf_bj.h:  Wolfenstein3-D bj code .
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
	This module is implemented by wolf_bj.c

*/

#ifndef __WOLF_BJ_H__
#define __WOLF_BJ_H__



extern void SpawnBJVictory( void );
extern void T_BJRun( entity_t *Guard );
extern void T_BJJump( entity_t *Guard );
extern void T_BJYell( entity_t *Guard );
extern void T_BJDone( entity_t *Guard );





#endif /* __WOLF_BJ_H__ */

