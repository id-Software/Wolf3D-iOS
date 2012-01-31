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
 *	wolf_act_stat.h:  Wolfenstein3-D entity management.
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


*/

#ifndef __WOLF_ACT_STAT_H__
#define __WOLF_ACT_STAT_H__

#include "wolf_bj.h"
#include "wolf_actor_ai.h"
#include "wolf_ai_com.h"
#include "wolf_sprites.h"

#define ST_INFO_NULL { 0, SPR_DEMO, 0, NULL, NULL, st_dead }

/*
	1-if object can be rotated, 0 if one sprite for every direction
	base object's state texture if rotation is on facing player
	after how man frames change state to .next_state
	what to do every frame
	what to do once per state
	next state
*/

stateinfo objstate[ NUMENEMIES ][ NUMSTATES ] =
{
	// en_guard,
	{
		{ 1, SPR_GRD_S_1,	 0, T_Stand, NULL, st_stand }, // st_stand,

		{ 1, SPR_GRD_W1_1,	20, T_Path,	NULL, st_path1s }, // st_path1,
		{ 1, SPR_GRD_W1_1,  5,	NULL,	NULL, st_path2  }, // st_path1s,
		{ 1, SPR_GRD_W2_1,	15,	T_Path,	NULL, st_path3  }, // st_path2,
		{ 1, SPR_GRD_W3_1,	20, T_Path,	NULL, st_path3s }, // st_path3,
		{ 1, SPR_GRD_W3_1,	5,	NULL,	NULL, st_path4  }, // st_path3s,
		{ 1, SPR_GRD_W4_1,	15, T_Path,	NULL, st_path1  }, // st_path4,

		{ 0, SPR_GRD_PAIN_1,	10, NULL,	NULL, st_chase1},// st_pain,
		{ 0, SPR_GRD_PAIN_2,	10, NULL,	NULL, st_chase1},// st_pain1,
	
		{ 0, SPR_GRD_SHOOT1,	20, NULL,	NULL,	st_shoot2},// st_shoot1,
		{ 0, SPR_GRD_SHOOT2,	20, NULL,	T_Shoot,st_shoot3},// st_shoot2,
		{ 0, SPR_GRD_SHOOT3,	20, NULL,	NULL,	st_chase1},// st_shoot3,

		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot4,
		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot5,
		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot6,
		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot7,
		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot8,
		{ 0, SPR_DEMO,	0, NULL,	NULL, st_chase1 }, // st_shoot9,

		{ 1, SPR_GRD_W1_1, 10, T_Chase,	NULL, st_chase1s }, // st_chase1,
		{ 1, SPR_GRD_W1_1,  3, NULL,	NULL, st_chase2  }, // st_chase1s,
		{ 1, SPR_GRD_W2_1,  8, T_Chase,	NULL, st_chase3  }, // st_chase2,
		{ 1, SPR_GRD_W3_1, 10, T_Chase,	NULL, st_chase3s }, // st_chase3,
		{ 1, SPR_GRD_W3_1,  3, NULL,	NULL, st_chase4  }, // st_chase3s,
		{ 1, SPR_GRD_W4_1,  8, T_Chase,	NULL, st_chase1  }, // st_chase4,

		{ 0, SPR_GRD_DIE_1, 15, NULL, A_DeathScream,st_die2 }, // st_die1,
		{ 0, SPR_GRD_DIE_2, 15, NULL, NULL,			st_die3 }, // st_die2,
		{ 0, SPR_GRD_DIE_3, 15, NULL, NULL,			st_dead }, // st_die3,

		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die4,
		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die5,
		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die6,
		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die7,
		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die8,
		{ 0, SPR_DEMO,	0, NULL,	 NULL, st_dead }, // st_die9,

		{ 0, SPR_GRD_DEAD,	0, NULL,	 NULL, st_dead } // st_dead
	},
	// en_officer,
	{
		{1, SPR_OFC_S_1,	 0, T_Stand, NULL, st_stand}, // st_stand,

		{1, SPR_OFC_W1_1,	20, T_Path,	 NULL, st_path1s},// st_path1,
		{1, SPR_OFC_W1_1,  5, NULL	,  NULL, st_path2}, // st_path1s,
		{1, SPR_OFC_W2_1, 15, T_Path,  NULL, st_path3}, // st_path2,
		{1, SPR_OFC_W3_1,	20, T_Path,	 NULL, st_path3s},// st_path3,
		{1, SPR_OFC_W3_1,	 5, NULL	,	 NULL, st_path4}, // st_path3s,
		{1, SPR_OFC_W4_1,	15, T_Path,	 NULL, st_path1}, // st_path4,

		{0, SPR_OFC_PAIN_1,	10, NULL,	 NULL, st_chase1},// st_pain,
		{0, SPR_OFC_PAIN_2,	10, NULL,	 NULL, st_chase1},// st_pain1,
	
		{0, SPR_OFC_SHOOT1,	 6, NULL,	 NULL, st_shoot2},// st_shoot1,
		{0, SPR_OFC_SHOOT2,	20, NULL,	 T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_OFC_SHOOT3,	10, NULL,	 NULL, st_chase1},// st_shoot3,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot4,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{1, SPR_OFC_W1_1, 10, T_Chase, NULL, st_chase1s},// st_chase1,
		{1, SPR_OFC_W1_1,  3, NULL	 , NULL, st_chase2}, // st_chase1s,
		{1, SPR_OFC_W2_1,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{1, SPR_OFC_W3_1, 10, T_Chase, NULL, st_chase3s},// st_chase3,
		{1, SPR_OFC_W3_1,  3, NULL	 , NULL, st_chase4}, // st_chase3s,
		{1, SPR_OFC_W4_1,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_OFC_DIE_1, 11, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_OFC_DIE_2, 11, NULL, NULL,			st_die3},// st_die2,
		{0, SPR_OFC_DIE_3, 11, NULL, NULL,			st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die9,

		{0, SPR_OFC_DEAD,	0, NULL,	 NULL, st_dead} // st_dead
	},
	// en_ss,
	{
		{1, SPR_SS_S_1,	 0, T_Stand, NULL, st_stand}, // st_stand,

		{1, SPR_SS_W1_1,	20, T_Path,	 NULL, st_path1s},// st_path1,
		{1, SPR_SS_W1_1,  5, NULL	,  NULL, st_path2}, // st_path1s,
		{1, SPR_SS_W2_1, 15, T_Path,  NULL, st_path3}, // st_path2,
		{1, SPR_SS_W3_1,	20, T_Path,	 NULL, st_path3s},// st_path3,
		{1, SPR_SS_W3_1,	 5, NULL	,	 NULL, st_path4}, // st_path3s,
		{1, SPR_SS_W4_1,	15, T_Path,	 NULL, st_path1}, // st_path4,

		{0, SPR_SS_PAIN_1,	10, NULL,	 NULL, st_chase1},// st_pain,
		{0, SPR_SS_PAIN_2,	10, NULL,	 NULL, st_chase1},// st_pain1,
	
		{0, SPR_SS_SHOOT1, 20, NULL, NULL,		st_shoot2},// st_shoot1,
		{0, SPR_SS_SHOOT2, 20, NULL, T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_SS_SHOOT3, 10, NULL, NULL,		st_shoot4},// st_shoot3,
		{0, SPR_SS_SHOOT2, 10, NULL, T_Shoot, st_shoot5},// st_shoot4,
		{0, SPR_SS_SHOOT3, 10, NULL, NULL,		st_shoot6},// st_shoot5,
		{0, SPR_SS_SHOOT2, 10, NULL, T_Shoot, st_shoot7},// st_shoot6,
		{0, SPR_SS_SHOOT3, 10, NULL, NULL,		st_shoot8},// st_shoot7,
		{0, SPR_SS_SHOOT2, 10, NULL, T_Shoot, st_shoot9},// st_shoot8,
		{0, SPR_SS_SHOOT3, 10, NULL, NULL,		st_chase1},// st_shoot9,

		{1, SPR_SS_W1_1, 10, T_Chase, NULL, st_chase1s},// st_chase1,
		{1, SPR_SS_W1_1,  3, NULL	 , NULL, st_chase2}, // st_chase1s,
		{1, SPR_SS_W2_1,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{1, SPR_SS_W3_1, 10, T_Chase, NULL, st_chase3s},// st_chase3,
		{1, SPR_SS_W3_1,  3, NULL	 , NULL, st_chase4},	 // st_chase3s,
		{1, SPR_SS_W4_1,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_SS_DIE_1, 15, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_SS_DIE_2, 15, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_SS_DIE_3, 15, NULL, NULL,					 st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die9,

		{0, SPR_SS_DEAD,	0, NULL,	 NULL, st_dead} // st_dead
	},
	// en_dog,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{1, SPR_DOG_W1_1,	20, T_Path,	 NULL, st_path1s},// st_path1,
		{1, SPR_DOG_W1_1,  5, NULL	,  NULL, st_path2}, // st_path1s,
		{1, SPR_DOG_W2_1, 15, T_Path,  NULL, st_path3}, // st_path2,
		{1, SPR_DOG_W3_1,	20, T_Path,	 NULL, st_path3s},// st_path3,
		{1, SPR_DOG_W3_1,	 5, NULL	,	 NULL, st_path4}, // st_path3s,
		{1, SPR_DOG_W4_1,	15, T_Path,	 NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_DOG_JUMP1, 10, NULL,	 NULL,	 st_shoot2},// st_shoot1,
		{0, SPR_DOG_JUMP2, 10, NULL,	 T_Bite, st_shoot3},// st_shoot2,
		{0, SPR_DOG_JUMP3, 10, NULL,	 NULL,   st_shoot4},// st_shoot3,
		{0, SPR_DOG_JUMP1, 10, NULL,	 NULL,	 st_shoot5},// st_shoot4,
		{0, SPR_DOG_W1_1,	 10, NULL,	 NULL,	 st_chase1},// st_shoot5,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{1, SPR_DOG_W1_1, 10, T_DogChase, NULL, st_chase1s},// st_chase1,
		{1, SPR_DOG_W1_1,  3, NULL			, NULL, st_chase2}, // st_chase1s,
		{1, SPR_DOG_W2_1,  8, T_DogChase, NULL, st_chase3}, // st_chase2,
		{1, SPR_DOG_W3_1, 10, T_DogChase, NULL, st_chase3s},// st_chase3,
		{1, SPR_DOG_W3_1,  3, NULL			, NULL, st_chase4},	 // st_chase3s,
		{1, SPR_DOG_W4_1,  8, T_DogChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_DOG_DIE_1, 15, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_DOG_DIE_2, 15, NULL, NULL,					st_die3},// st_die2,
		{0, SPR_DOG_DIE_3, 15, NULL, NULL,					st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_DOG_DEAD,	0, NULL, NULL, st_dead} // st_dead
	},
	// en_boss,
	{
		{0, SPR_BOSS_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_BOSS_SHOOT1, 30, NULL, NULL,		st_shoot2},// st_shoot1,
		{0, SPR_BOSS_SHOOT2, 10, NULL, T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_BOSS_SHOOT3, 10, NULL, T_Shoot, st_shoot4},// st_shoot3,
		{0, SPR_BOSS_SHOOT2, 10, NULL, T_Shoot,	st_shoot5},// st_shoot4,
		{0, SPR_BOSS_SHOOT3, 10, NULL, T_Shoot,	st_shoot6},// st_shoot5,
		{0, SPR_BOSS_SHOOT2, 10, NULL, T_Shoot, st_shoot7},// st_shoot6,
		{0, SPR_BOSS_SHOOT3, 10, NULL, T_Shoot, st_shoot8},// st_shoot7,
		{0, SPR_BOSS_SHOOT1, 10, NULL, NULL,		st_chase1},// st_shoot8,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{0, SPR_BOSS_W1, 10, T_Chase, NULL, st_chase1s},// st_chase1,
		{0, SPR_BOSS_W1,  3, NULL		, NULL, st_chase2}, // st_chase1s,
		{0, SPR_BOSS_W2,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{0, SPR_BOSS_W3, 10, T_Chase, NULL, st_chase3s},// st_chase3,
		{0, SPR_BOSS_W3,  3, NULL		, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_BOSS_W4,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_BOSS_DIE1, 15, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_BOSS_DIE2, 15, NULL, NULL,					st_die3},// st_die2,
		{0, SPR_BOSS_DIE3, 15, NULL, NULL,					st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_BOSS_DEAD, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_schabbs,
	{
		{0, SPR_SCHABB_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_SCHABB_SHOOT1, 30, NULL, NULL,     st_shoot2},// st_shoot1,
		{0, SPR_SCHABB_SHOOT2, 10, NULL, T_Launch, st_chase1},// st_shoot2,
		
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot3,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},//  st_shoot4,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{0, SPR_SCHABB_W1, 10, T_BossChase, NULL, st_chase1s},// st_chase1,
		{0, SPR_SCHABB_W1,  3, NULL		    , NULL, st_chase2}, // st_chase1s,
		{0, SPR_SCHABB_W2,  8, T_BossChase, NULL, st_chase3}, // st_chase2,
		{0, SPR_SCHABB_W3, 10, T_BossChase, NULL, st_chase3s},// st_chase3,
		{0, SPR_SCHABB_W3,  3, NULL		    , NULL, st_chase4},	 // st_chase3s,
		{0, SPR_SCHABB_W4,  8, T_BossChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_SCHABB_W1,		10, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_SCHABB_W1,		10, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_SCHABB_DIE1,	10, NULL, NULL,					 st_die4},// st_die3,
		{0, SPR_SCHABB_DIE2,	10, NULL, NULL,					 st_die5},// st_die4,
		{0, SPR_SCHABB_DIE3,	10, NULL, NULL,					 st_dead},// st_die5,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_SCHABB_DEAD, 0, NULL, A_StartDeathCam, st_dead} // st_dead
	},
	// en_fake,
	{
		{0, SPR_FAKE_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_FAKE_SHOOT, 8, NULL, T_Launch, st_shoot2},// st_shoot1,
		{0, SPR_FAKE_SHOOT, 8, NULL, T_Launch, st_shoot3},// st_shoot2,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot4},// st_shoot3,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot5},// st_shoot4,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot6},// st_shoot4,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot7},// st_shoot4,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot8},// st_shoot4,
		{0, SPR_FAKE_SHOOT,	8, NULL, T_Launch, st_shoot9},// st_shoot4,
		{0, SPR_FAKE_SHOOT,	8, NULL, NULL,			 st_chase1},// st_shoot4,

		{0, SPR_FAKE_W1, 10, T_Fake, NULL, st_chase1s},// st_chase1,
		{0, SPR_FAKE_W1,  3, NULL  , NULL, st_chase2}, // st_chase1s,
		{0, SPR_FAKE_W2,  8, T_Fake, NULL, st_chase3}, // st_chase2,
		{0, SPR_FAKE_W3, 10, T_Fake, NULL, st_chase3s},// st_chase3,
		{0, SPR_FAKE_W3,  3, NULL  , NULL, st_chase4}, // st_chase3s,
		{0, SPR_FAKE_W4,  8, T_Fake, NULL, st_chase1}, // st_chase4,

		{0, SPR_FAKE_DIE1, 10, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_FAKE_DIE2, 10, NULL, NULL,					st_die3},// st_die2,
		{0, SPR_FAKE_DIE3, 10, NULL, NULL,					st_die4},// st_die3,
		{0, SPR_FAKE_DIE4, 10, NULL, NULL,					st_die5},// st_die4,
		{0, SPR_FAKE_DIE5, 10, NULL, NULL,					st_dead},// st_die5,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_FAKE_DEAD, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_hitler, (mecha)
	{
		{0, SPR_MECHA_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_MECHA_SHOOT1, 30, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_MECHA_SHOOT2, 10, NULL, T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_MECHA_SHOOT3, 10, NULL, T_Shoot, st_shoot4},// st_shoot3,
		{0, SPR_MECHA_SHOOT2, 10, NULL, T_Shoot, st_shoot5},// st_shoot4,
		{0, SPR_MECHA_SHOOT3, 10, NULL, T_Shoot, st_shoot6},// st_shoot5,
		{0, SPR_MECHA_SHOOT2, 10, NULL, T_Shoot, st_chase1},// st_shoot6,

		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_MECHA_W1, 10, T_Chase, A_MechaSound, st_chase1s},// st_chase1,
		{0, SPR_MECHA_W1,  6, NULL	 , NULL, st_chase2}, // st_chase1s,
		{0, SPR_MECHA_W2,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{0, SPR_MECHA_W3, 10, T_Chase, A_MechaSound, st_chase3s},// st_chase3,
		{0, SPR_MECHA_W3,  6, NULL	 , NULL, st_chase4},	 // st_chase3s,
		{0, SPR_MECHA_W4,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_MECHA_DIE1, 10, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_MECHA_DIE2, 10, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_MECHA_DIE3, 10, NULL, A_HitlerMorph, st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_MECHA_DEAD, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_hitler,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_HITLER_SHOOT1, 30, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_HITLER_SHOOT2, 10, NULL, T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_HITLER_SHOOT3, 10, NULL, T_Shoot, st_shoot4},// st_shoot3,
		{0, SPR_HITLER_SHOOT2, 10, NULL, T_Shoot, st_shoot5},// st_shoot4,
		{0, SPR_HITLER_SHOOT3, 10, NULL, T_Shoot, st_shoot6},// st_shoot5,
		{0, SPR_HITLER_SHOOT2, 10, NULL, T_Shoot, st_chase1},// st_shoot6,

		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_HITLER_W1, 6, T_Chase, NULL, st_chase1s}, // st_chase1,
		{0, SPR_HITLER_W1, 4, NULL	 , NULL, st_chase2},  // st_chase1s,
		{0, SPR_HITLER_W2, 2, T_Chase, NULL, st_chase3},  // st_chase2,
		{0, SPR_HITLER_W3, 6, T_Chase, NULL, st_chase3s}, // st_chase3,
		{0, SPR_HITLER_W3, 4, NULL		, NULL, st_chase4}, // st_chase3s,
		{0, SPR_HITLER_W4, 2, T_Chase, NULL, st_chase1},  // st_chase4,

		{0, SPR_HITLER_W1,    1, NULL, A_DeathScream,  st_die2},// st_die1,
		{0, SPR_HITLER_W1,	 10, NULL, NULL, st_die3},// st_die2,
		{0, SPR_HITLER_DIE1, 10, NULL, NULL, st_dead},// st_die3,
		{0, SPR_HITLER_DIE2, 10, NULL, NULL, st_dead},// st_die4,
		{0, SPR_HITLER_DIE3, 10, NULL, NULL, st_dead},// st_die5,
		{0, SPR_HITLER_DIE4, 10, NULL, NULL, st_dead},// st_die6,
		{0, SPR_HITLER_DIE5, 10, NULL, NULL, st_dead},// st_die7,
		{0, SPR_HITLER_DIE6, 10, NULL, NULL, st_dead},// st_die8,
		{0, SPR_HITLER_DIE7, 10, NULL, NULL, st_dead},// st_die9,

		{0, SPR_HITLER_DEAD, 0, NULL, A_StartDeathCam, st_dead} // st_dead
	},
	// en_mutant,
	{
		{1, SPR_MUT_S_1,	 0, T_Stand, NULL, st_stand}, // st_stand,

		{1, SPR_MUT_W1_1,	20, T_Path,	 NULL, st_path1s},// st_path1,
		{1, SPR_MUT_W1_1,  5, NULL	,  NULL, st_path2}, // st_path1s,
		{1, SPR_MUT_W2_1, 15, T_Path,  NULL, st_path3}, // st_path2,
		{1, SPR_MUT_W3_1,	20, T_Path,	 NULL, st_path3s},// st_path3,
		{1, SPR_MUT_W3_1,	 5, NULL	,	 NULL, st_path4}, // st_path3s,
		{1, SPR_MUT_W4_1,	15, T_Path,	 NULL, st_path1}, // st_path4,

		{0, SPR_MUT_PAIN_1,	10, NULL,	 NULL, st_chase1},// st_pain,
		{0, SPR_MUT_PAIN_2,	10, NULL,	 NULL, st_chase1},// st_pain1,
	
		{0, SPR_MUT_SHOOT1,	 6, NULL, T_Shoot, st_shoot2}, // st_shoot1,
		{0, SPR_MUT_SHOOT2,	20, NULL, NULL,		 st_shoot3}, // st_shoot2,
		{0, SPR_MUT_SHOOT3,	10, NULL, T_Shoot, st_shoot4}, // st_shoot3,
		{0, SPR_MUT_SHOOT4,	20, NULL, NULL,		 st_chase1}, // st_shoot4,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{1, SPR_MUT_W1_1, 10, T_Chase, NULL, st_chase1s},// st_chase1,
		{1, SPR_MUT_W1_1,  3, NULL	 , NULL, st_chase2}, // st_chase1s,
		{1, SPR_MUT_W2_1,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{1, SPR_MUT_W3_1, 10, T_Chase, NULL, st_chase3s},// st_chase3,
		{1, SPR_MUT_W3_1,  3, NULL	 , NULL, st_chase4},	 // st_chase3s,
		{1, SPR_MUT_W4_1,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_MUT_DIE_1, 7, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_MUT_DIE_2, 7, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_MUT_DIE_3, 7, NULL, NULL,					 st_die4},// st_die3,
		{0, SPR_MUT_DIE_4, 7, NULL, NULL,					 st_dead},// st_die4,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_dead},// st_die9,

		{0, SPR_MUT_DEAD,	0, NULL,	 NULL, st_dead} // st_dead
	},
	// en_blinky,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot3},// st_shoot2,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot4},// st_shoot3,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot5},// st_shoot4,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot6},// st_shoot5,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_BLINKY_W1, 10, T_Ghosts, NULL, st_chase2},// st_chase1,
		{0, SPR_DEMO,  0, NULL, NULL, st_chase2},			// st_chase1s,
		{0, SPR_BLINKY_W2, 10, T_Ghosts, NULL, st_chase1},// st_chase2,

		{0, SPR_DEMO, 0, NULL, NULL, st_chase3s},// st_chase3,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1}, // st_chase4,

		{0, SPR_DEMO, 10, NULL, NULL, st_die2},// st_die1,
		{0, SPR_DEMO, 10, NULL, NULL, st_die3},// st_die2,
		{0, SPR_DEMO, 10, NULL, NULL, st_dead},// st_die3,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_DEMO, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_clyde,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot3},// st_shoot2,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot4},// st_shoot3,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot5},// st_shoot4,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot6},// st_shoot5,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_CLYDE_W1, 10, T_Ghosts, NULL, st_chase2},// st_chase1,
		{0, SPR_DEMO,  0, NULL, NULL, st_chase2},			// st_chase1s,
		{0, SPR_CLYDE_W2, 10, T_Ghosts, NULL, st_chase1},// st_chase2,

		{0, SPR_DEMO, 0, NULL, NULL, st_chase3s},// st_chase3,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1}, // st_chase4,

		{0, SPR_DEMO, 10, NULL, NULL, st_die2},// st_die1,
		{0, SPR_DEMO, 10, NULL, NULL, st_die3},// st_die2,
		{0, SPR_DEMO, 10, NULL, NULL, st_dead},// st_die3,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_DEMO, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_pinky,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot3},// st_shoot2,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot4},// st_shoot3,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot5},// st_shoot4,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot6},// st_shoot5,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_PINKY_W1, 10, T_Ghosts, NULL, st_chase2},// st_chase1,
		{0, SPR_DEMO,  0, NULL, NULL, st_chase2},			// st_chase1s,
		{0, SPR_PINKY_W2, 10, T_Ghosts, NULL, st_chase1},// st_chase2,

		{0, SPR_DEMO, 0, NULL, NULL, st_chase3s},// st_chase3,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1}, // st_chase4,

		{0, SPR_DEMO, 10, NULL, NULL, st_die2},// st_die1,
		{0, SPR_DEMO, 10, NULL, NULL, st_die3},// st_die2,
		{0, SPR_DEMO, 10, NULL, NULL, st_dead},// st_die3,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_DEMO, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_inky,
	{
		{0, SPR_DEMO,	0, NULL, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot2},// st_shoot1,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot3},// st_shoot2,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot4},// st_shoot3,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot5},// st_shoot4,
		{0, SPR_DEMO, 0, NULL, NULL, st_shoot6},// st_shoot5,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot8},// st_shoot7,
		{0, SPR_DEMO,	0, NULL, NULL, st_shoot9},// st_shoot8,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_shoot9,

		{0, SPR_INKY_W1, 10, T_Ghosts, NULL, st_chase2},// st_chase1,
		{0, SPR_DEMO,  0, NULL, NULL, st_chase2},			// st_chase1s,
		{0, SPR_INKY_W2, 10, T_Ghosts, NULL, st_chase1},// st_chase2,

		{0, SPR_DEMO, 0, NULL, NULL, st_chase3s},// st_chase3,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_chase1}, // st_chase4,

		{0, SPR_DEMO, 10, NULL, NULL, st_die2},// st_die1,
		{0, SPR_DEMO, 10, NULL, NULL, st_die3},// st_die2,
		{0, SPR_DEMO, 10, NULL, NULL, st_dead},// st_die3,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_DEMO, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_gretel,
	{
		{0, SPR_GRETEL_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_GRETEL_SHOOT1, 30, NULL, NULL,		st_shoot2},// st_shoot1,
		{0, SPR_GRETEL_SHOOT2, 10, NULL, T_Shoot, st_shoot3},// st_shoot2,
		{0, SPR_GRETEL_SHOOT3, 10, NULL, T_Shoot, st_shoot4},// st_shoot3,
		{0, SPR_GRETEL_SHOOT2, 10, NULL, T_Shoot,	st_shoot5},// st_shoot4,
		{0, SPR_GRETEL_SHOOT3, 10, NULL, T_Shoot,	st_shoot6},// st_shoot5,
		{0, SPR_GRETEL_SHOOT2, 10, NULL, T_Shoot, st_shoot7},// st_shoot6,
		{0, SPR_GRETEL_SHOOT3, 10, NULL, T_Shoot, st_shoot8},// st_shoot7,
		{0, SPR_GRETEL_SHOOT1, 10, NULL, NULL,		st_chase1},// st_shoot8,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{0, SPR_GRETEL_W1, 10, T_Chase, NULL, st_chase1s},// st_chase1,
		{0, SPR_GRETEL_W1,  3, NULL		, NULL, st_chase2}, // st_chase1s,
		{0, SPR_GRETEL_W2,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{0, SPR_GRETEL_W3, 10, T_Chase, NULL, st_chase3s},// st_chase3,
		{0, SPR_GRETEL_W3,  3, NULL		, NULL, st_chase4},	 // st_chase3s,
		{0, SPR_GRETEL_W4,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_GRETEL_DIE1, 15, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_GRETEL_DIE2, 15, NULL, NULL,					st_die3},// st_die2,
		{0, SPR_GRETEL_DIE3, 15, NULL, NULL,					st_dead},// st_die3,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die4,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die5,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_GRETEL_DEAD, 0, NULL, NULL, st_dead} // st_dead
	},
	// en_gift,
	{
		{0, SPR_GIFT_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_GIFT_SHOOT1, 30, NULL, NULL,     st_shoot2},// st_shoot1,
		{0, SPR_GIFT_SHOOT2, 10, NULL, T_Launch, st_chase1},// st_shoot2,
		
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot3,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},//  st_shoot4,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot5,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot6,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{0, SPR_GIFT_W1, 10, T_BossChase, NULL, st_chase1s},// st_chase1,
		{0, SPR_GIFT_W1,  3, NULL		    , NULL, st_chase2}, // st_chase1s,
		{0, SPR_GIFT_W2,  8, T_BossChase, NULL, st_chase3}, // st_chase2,
		{0, SPR_GIFT_W3, 10, T_BossChase, NULL, st_chase3s},// st_chase3,
		{0, SPR_GIFT_W3,  3, NULL		    , NULL, st_chase4},	 // st_chase3s,
		{0, SPR_GIFT_W4,  8, T_BossChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_GIFT_W1,	 10, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_GIFT_W1,	 10, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_GIFT_DIE1, 10, NULL, NULL,					 st_die4},// st_die3,
		{0, SPR_GIFT_DIE2, 10, NULL, NULL,					 st_die5},// st_die4,
		{0, SPR_GIFT_DIE3, 10, NULL, NULL,					 st_dead},// st_die5,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_GIFT_DEAD, 0, NULL, A_StartDeathCam, st_dead} // st_dead
	},
	// en_fat,
	{
		{0, SPR_FAT_W1,	0, T_Stand, NULL, st_stand}, // st_stand,

		{0, SPR_DEMO, 0, NULL, NULL, st_path1s},// st_path1,
		{0, SPR_DEMO, 0, NULL, NULL, st_path2}, // st_path1s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3}, // st_path2,
		{0, SPR_DEMO, 0, NULL, NULL, st_path3s},// st_path3,
		{0, SPR_DEMO, 0, NULL, NULL, st_path4}, // st_path3s,
		{0, SPR_DEMO, 0, NULL, NULL, st_path1}, // st_path4,

		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain,
		{0, SPR_DEMO,	0, NULL, NULL, st_chase1},// st_pain1,
	
		{0, SPR_FAT_SHOOT1, 30, NULL, NULL,				 st_shoot2},// st_shoot1,
		{0, SPR_FAT_SHOOT2, 10, NULL, T_Launch, st_shoot3},// st_shoot2,
		{0, SPR_FAT_SHOOT3,	10, NULL,	T_Shoot, st_shoot4},// st_shoot3,
		{0, SPR_FAT_SHOOT4,	10, NULL,	T_Shoot, st_shoot5},//  st_shoot4,
		{0, SPR_FAT_SHOOT3,	10, NULL,	T_Shoot, st_shoot6},// st_shoot5,
		{0, SPR_FAT_SHOOT4,	10, NULL,	T_Shoot, st_chase1},// st_shoot6,

		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot7,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot8,
		{0, SPR_DEMO,	0, NULL,	 NULL, st_chase1},// st_shoot9,

		{0, SPR_FAT_W1, 10, T_BossChase, NULL, st_chase1s},// st_chase1,
		{0, SPR_FAT_W1,  3, NULL       , NULL, st_chase2}, // st_chase1s,
		{0, SPR_FAT_W2,  8, T_BossChase, NULL, st_chase3}, // st_chase2,
		{0, SPR_FAT_W3, 10, T_BossChase, NULL, st_chase3s},// st_chase3,
		{0, SPR_FAT_W3,  3, NULL       , NULL, st_chase4},	 // st_chase3s,
		{0, SPR_FAT_W4,  8, T_BossChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_FAT_W1,		10, NULL, A_DeathScream, st_die2},// st_die1,
		{0, SPR_FAT_W1,		10, NULL, NULL,					 st_die3},// st_die2,
		{0, SPR_FAT_DIE1, 10, NULL, NULL,					 st_die4},// st_die3,
		{0, SPR_FAT_DIE2,	10, NULL, NULL,					 st_die5},// st_die4,
		{0, SPR_FAT_DIE3,	10, NULL, NULL,					 st_dead},// st_die5,

		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die6,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die7,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die8,
		{0, SPR_DEMO,	0, NULL, NULL, st_dead},// st_die9,

		{0, SPR_FAT_DEAD, 0, NULL, A_StartDeathCam, st_dead} // st_dead
	},
// --- Projectiles
	// en_needle,
	{
		ST_INFO_NULL, // st_stand,

		{0, SPR_HYPO1, 6, T_Projectile, NULL, st_path2}, // st_path1,
		ST_INFO_NULL, // st_path1s,
		{0, SPR_HYPO2, 6, T_Projectile, NULL, st_path3}, // st_path2,
		{0, SPR_HYPO3, 6, T_Projectile, NULL, st_path4}, // st_path3,
		ST_INFO_NULL, // st_path3s,
		{0, SPR_HYPO4, 6, T_Projectile, NULL, st_path1}, // st_path4,

		ST_INFO_NULL,// st_pain,
		ST_INFO_NULL,// st_pain1,
	
		ST_INFO_NULL,// st_shoot1,
		ST_INFO_NULL,// st_shoot2,
		ST_INFO_NULL,// st_shoot3,
		ST_INFO_NULL,//  st_shoot4,
		ST_INFO_NULL,// st_shoot5,
		ST_INFO_NULL,// st_shoot6,

		ST_INFO_NULL,// st_shoot7,
		ST_INFO_NULL,// st_shoot8,
		ST_INFO_NULL,// st_shoot9,

		ST_INFO_NULL,// st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL,// st_chase3,
		ST_INFO_NULL,	 // st_chase3s,
		ST_INFO_NULL, // st_chase4,

		ST_INFO_NULL, // st_die1,
		ST_INFO_NULL, // st_die2,
		ST_INFO_NULL, // st_die3,
		ST_INFO_NULL,// st_die4,
		ST_INFO_NULL,// st_die5,

		ST_INFO_NULL,// st_die6,
		ST_INFO_NULL,// st_die7,
		ST_INFO_NULL,// st_die8,
		ST_INFO_NULL,// st_die9,

		ST_INFO_NULL // st_dead
	},
	// en_fire,
	{
		ST_INFO_NULL, // st_stand,

		{0, SPR_FIRE1, 6, NULL, T_Projectile, st_path2}, // st_path1,
		ST_INFO_NULL, // st_path1s,
		{0, SPR_FIRE2, 6, NULL, T_Projectile, st_path1}, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL,// st_pain,
		ST_INFO_NULL,// st_pain1,
	
		ST_INFO_NULL,// st_shoot1,
		ST_INFO_NULL,// st_shoot2,
		ST_INFO_NULL,// st_shoot3,
		ST_INFO_NULL,//  st_shoot4,
		ST_INFO_NULL,// st_shoot5,
		ST_INFO_NULL,// st_shoot6,

		ST_INFO_NULL,// st_shoot7,
		ST_INFO_NULL,// st_shoot8,
		ST_INFO_NULL,// st_shoot9,

		ST_INFO_NULL,// st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL,// st_chase3,
		ST_INFO_NULL,	 // st_chase3s,
		ST_INFO_NULL, // st_chase4,

		ST_INFO_NULL, // st_die1,
		ST_INFO_NULL, // st_die2,
		ST_INFO_NULL, // st_die3,
		ST_INFO_NULL,// st_die4,
		ST_INFO_NULL,// st_die5,

		ST_INFO_NULL,// st_die6,
		ST_INFO_NULL,// st_die7,
		ST_INFO_NULL,// st_die8,
		ST_INFO_NULL,// st_die9,

		ST_INFO_NULL // st_dead
	},
	// en_rocket,
	{
		{1, SPR_ROCKET_1, 3, T_Projectile, A_Smoke, st_stand}, // st_stand,

		ST_INFO_NULL,// st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL,// st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL,// st_pain,
		ST_INFO_NULL,// st_pain1,
	
		ST_INFO_NULL,// st_shoot1,
		ST_INFO_NULL,// st_shoot2,
		ST_INFO_NULL,// st_shoot3,
		ST_INFO_NULL,//  st_shoot4,
		ST_INFO_NULL,// st_shoot5,
		ST_INFO_NULL,// st_shoot6,

		ST_INFO_NULL,// st_shoot7,
		ST_INFO_NULL,// st_shoot8,
		ST_INFO_NULL,// st_shoot9,

		ST_INFO_NULL,// st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL,// st_chase3,
		ST_INFO_NULL,	 // st_chase3s,
		ST_INFO_NULL, // st_chase4,

		{0, SPR_BOOM_1, 6, NULL, NULL, st_die2}, // st_die1,
		{0, SPR_BOOM_2, 6, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_BOOM_3, 6, NULL, NULL, st_remove}, // st_die3,
		ST_INFO_NULL,// st_die4,
		ST_INFO_NULL,// st_die5,

		ST_INFO_NULL,// st_die6,
		ST_INFO_NULL,// st_die7,
		ST_INFO_NULL,// st_die8,
		ST_INFO_NULL,// st_die9,

		ST_INFO_NULL // st_dead
	},
	// en_smoke,
	{
		ST_INFO_NULL, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		ST_INFO_NULL, // st_shoot1,
		ST_INFO_NULL, // st_shoot2,
		ST_INFO_NULL, // st_shoot3,
		ST_INFO_NULL, // st_shoot4,
		ST_INFO_NULL, // st_shoot5,
		ST_INFO_NULL, // st_shoot6,

		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		ST_INFO_NULL, // st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL, // st_chase3,
		ST_INFO_NULL,	// st_chase3s,
		ST_INFO_NULL, // st_chase4,

		{0, SPR_SMOKE_1, 3, NULL, NULL, st_die2}, // st_die1,
		{0, SPR_SMOKE_2, 3, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_SMOKE_3, 3, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_SMOKE_4, 3, NULL, NULL, st_remove}, // st_die4,
		ST_INFO_NULL, // st_die5,

		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		ST_INFO_NULL  // st_dead
	},
	// en_bj,
	{
		ST_INFO_NULL, // st_stand,

		{0, SPR_BJ_W1, 12, T_BJRun, NULL, st_path1s}, // st_path1,
		{0, SPR_BJ_W1,  3, NULL,	  NULL, st_path2}, // st_path1s,
		{0, SPR_BJ_W2,  8, T_BJRun, NULL, st_path3}, // st_path2,
		{0, SPR_BJ_W3, 12, T_BJRun, NULL, st_path3s}, // st_path3,
		{0, SPR_BJ_W3,  3, NULL,	  NULL, st_path4}, // st_path3s,
		{0, SPR_BJ_W4,  8, T_BJRun, NULL, st_path1}, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		{0, SPR_BJ_JUMP1, 14, T_BJJump, NULL, st_shoot2}, // st_shoot1,
		{0, SPR_BJ_JUMP2, 14, T_BJJump, T_BJYell, st_shoot3}, // st_shoot2,
		{0, SPR_BJ_JUMP3, 14, T_BJJump, NULL, st_shoot4}, // st_shoot3,
		{0, SPR_BJ_JUMP4,300, NULL, T_BJDone, st_shoot4}, // st_shoot4,
		ST_INFO_NULL, // st_shoot5,
		ST_INFO_NULL, // st_shoot6,

		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		ST_INFO_NULL, // st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL, // st_chase3,
		ST_INFO_NULL,	// st_chase3s,
		ST_INFO_NULL, // st_chase4,

		ST_INFO_NULL, // st_die1,
		ST_INFO_NULL, // st_die2,
		ST_INFO_NULL, // st_die3,
		ST_INFO_NULL, // st_die4,
		ST_INFO_NULL, // st_die5,

		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		ST_INFO_NULL  // st_dead
	},

// --- Spear of destiny!
	// en_spark,
	{
		ST_INFO_NULL, // st_stand,

		{0, SPR_SPARK1, 6, T_Projectile, NULL, st_path2}, // st_path1,
		ST_INFO_NULL, // st_path1s,
		{0, SPR_SPARK2, 6, T_Projectile, NULL, st_path3}, // st_path2,
		{0, SPR_SPARK3, 6, T_Projectile, NULL, st_path4}, // st_path3,
		ST_INFO_NULL, // st_path3s,
		{0, SPR_SPARK4, 6, T_Projectile, NULL, st_path1}, // st_path4,

		ST_INFO_NULL,// st_pain,
		ST_INFO_NULL,// st_pain1,
	
		ST_INFO_NULL,// st_shoot1,
		ST_INFO_NULL,// st_shoot2,
		ST_INFO_NULL,// st_shoot3,
		ST_INFO_NULL,//  st_shoot4,
		ST_INFO_NULL,// st_shoot5,
		ST_INFO_NULL,// st_shoot6,

		ST_INFO_NULL,// st_shoot7,
		ST_INFO_NULL,// st_shoot8,
		ST_INFO_NULL,// st_shoot9,

		ST_INFO_NULL,// st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL,// st_chase3,
		ST_INFO_NULL,	 // st_chase3s,
		ST_INFO_NULL, // st_chase4,

		ST_INFO_NULL, // st_die1,
		ST_INFO_NULL, // st_die2,
		ST_INFO_NULL, // st_die3,
		ST_INFO_NULL,// st_die4,
		ST_INFO_NULL,// st_die5,

		ST_INFO_NULL,// st_die6,
		ST_INFO_NULL,// st_die7,
		ST_INFO_NULL,// st_die8,
		ST_INFO_NULL,// st_die9,

		ST_INFO_NULL // st_dead
	},
	// en_hrocket,
	{
		{1, SPR_HROCKET_1, 3, T_Projectile, A_Smoke, st_stand}, // st_stand,

		ST_INFO_NULL,// st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL,// st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL,// st_pain,
		ST_INFO_NULL,// st_pain1,
	
		ST_INFO_NULL,// st_shoot1,
		ST_INFO_NULL,// st_shoot2,
		ST_INFO_NULL,// st_shoot3,
		ST_INFO_NULL,//  st_shoot4,
		ST_INFO_NULL,// st_shoot5,
		ST_INFO_NULL,// st_shoot6,

		ST_INFO_NULL,// st_shoot7,
		ST_INFO_NULL,// st_shoot8,
		ST_INFO_NULL,// st_shoot9,

		ST_INFO_NULL,// st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL,// st_chase3,
		ST_INFO_NULL,	 // st_chase3s,
		ST_INFO_NULL, // st_chase4,

		{0, SPR_HBOOM_1, 6, NULL, NULL, st_die2}, // st_die1,
		{0, SPR_HBOOM_2, 6, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_HBOOM_3, 6, NULL, NULL, st_remove}, // st_die3,
		ST_INFO_NULL,// st_die4,
		ST_INFO_NULL,// st_die5,

		ST_INFO_NULL,// st_die6,
		ST_INFO_NULL,// st_die7,
		ST_INFO_NULL,// st_die8,
		ST_INFO_NULL,// st_die9,

		ST_INFO_NULL // st_dead
	},
	// en_hsmoke,
	{
		ST_INFO_NULL, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		ST_INFO_NULL, // st_shoot1,
		ST_INFO_NULL, // st_shoot2,
		ST_INFO_NULL, // st_shoot3,
		ST_INFO_NULL, // st_shoot4,
		ST_INFO_NULL, // st_shoot5,
		ST_INFO_NULL, // st_shoot6,

		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		ST_INFO_NULL, // st_chase1,
		ST_INFO_NULL, // st_chase1s,
		ST_INFO_NULL, // st_chase2,
		ST_INFO_NULL, // st_chase3,
		ST_INFO_NULL,	// st_chase3s,
		ST_INFO_NULL, // st_chase4,

		{0, SPR_HSMOKE_1, 3, NULL, NULL, st_die2}, // st_die1,
		{0, SPR_HSMOKE_2, 3, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_HSMOKE_3, 3, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_HSMOKE_4, 3, NULL, NULL, st_remove}, // st_die4,
		ST_INFO_NULL, // st_die5,

		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		ST_INFO_NULL  // st_dead
	},
	// en_spectre,
	{
		ST_INFO_NULL, // st_stand,

		{0, SPR_SPECTRE_W1, 10, T_Stand, NULL, st_path2}, // st_path1,
		ST_INFO_NULL, // st_path1s,
		{0, SPR_SPECTRE_W2, 10, T_Stand, NULL, st_path3}, // st_path2,
		{0, SPR_SPECTRE_W3, 10, T_Stand, NULL, st_path4}, // st_path3,
		ST_INFO_NULL, // st_path3s,
		{0, SPR_SPECTRE_W4, 10, T_Stand, NULL, st_path1}, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		ST_INFO_NULL, // st_shoot1,
		ST_INFO_NULL, // st_shoot2,
		ST_INFO_NULL, // st_shoot3,
		ST_INFO_NULL, // st_shoot4,
		ST_INFO_NULL, // st_shoot5,
		ST_INFO_NULL, // st_shoot6,

		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_SPECTRE_W1, 10, T_Ghosts, NULL, st_chase2}, // st_chase1,
		ST_INFO_NULL, // st_chase1s,
		{0, SPR_SPECTRE_W2, 10, T_Ghosts, NULL, st_chase3}, // st_chase2,
		{0, SPR_SPECTRE_W3, 10, T_Ghosts, NULL, st_chase4}, // st_chase3,
		ST_INFO_NULL,	// st_chase3s,
		{0, SPR_SPECTRE_W4, 10, T_Ghosts, NULL, st_chase1}, // st_chase4,

		{0, SPR_SPECTRE_F1, 10, NULL, NULL, st_die2}, // st_die1,
		{0, SPR_SPECTRE_F2, 10, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_SPECTRE_F3, 10, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_SPECTRE_F4, 300, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_SPECTRE_F4, 10, NULL, A_Dormant, st_die5}, // st_die5,

		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		ST_INFO_NULL  // st_dead
	},
	// en_angel,
	{
		{0, SPR_ANGEL_W1, 0, T_Stand, NULL, st_stand}, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		{0, SPR_ANGEL_TIRED1, 40, NULL,	A_Breathing, st_pain1}, // st_pain,
		{0, SPR_ANGEL_TIRED2, 40, NULL,	NULL, st_shoot4}, // st_pain1,
	
		{0, SPR_ANGEL_SHOOT1, 10, NULL,	A_StartAttack, st_shoot2}, // st_shoot1,
		{0, SPR_ANGEL_SHOOT2, 20, NULL,	T_Launch, st_shoot3}, // st_shoot2,
		{0, SPR_ANGEL_SHOOT1, 10, NULL,	A_Relaunch, st_shoot2}, // st_shoot3,

		{0, SPR_ANGEL_TIRED1, 40, NULL,	A_Breathing, st_shoot5}, // st_shoot4,
		{0, SPR_ANGEL_TIRED2, 40, NULL,	NULL, st_shoot6}, // st_shoot5,
		{0, SPR_ANGEL_TIRED1, 40, NULL,	A_Breathing, st_shoot7}, // st_shoot6,
		{0, SPR_ANGEL_TIRED2, 40, NULL,	NULL, st_shoot8}, // st_shoot7,
		{0, SPR_ANGEL_TIRED1, 40, NULL,	A_Breathing, st_chase1}, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_ANGEL_W1, 10, T_BossChase,	NULL, st_chase1s}, // st_chase1,
		{0, SPR_ANGEL_W1,  3, NULL,		NULL, st_chase2}, // st_chase1s,
		{0, SPR_ANGEL_W2,  8, T_BossChase,	NULL, st_chase3}, // st_chase2,
		{0, SPR_ANGEL_W3, 10, T_BossChase,	NULL, st_chase3s}, // st_chase3,
		{0, SPR_ANGEL_W3,  3, NULL,		NULL, st_chase4},	// st_chase3s,
		{0, SPR_ANGEL_W4,  8, T_BossChase,	NULL, st_chase1}, // st_chase4,

		{0, SPR_ANGEL_W1, 1, NULL, A_DeathScream, st_die2}, // st_die1,
		{0, SPR_ANGEL_W1, 1, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_ANGEL_DIE1, 10, NULL, A_Slurpie, st_die4}, // st_die3,
		{0, SPR_ANGEL_DIE2, 10, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_ANGEL_DIE3, 10, NULL, NULL, st_die6}, // st_die5,
		{0, SPR_ANGEL_DIE4, 10, NULL, NULL, st_die7}, // st_die6,
		{0, SPR_ANGEL_DIE5, 10, NULL, NULL, st_die8}, // st_die7,
		{0, SPR_ANGEL_DIE6, 10, NULL, NULL, st_die9}, // st_die8,
		{0, SPR_ANGEL_DIE7, 10, NULL, NULL, st_dead}, // st_die9,

		{0, SPR_ANGEL_DEAD, 130, NULL, A_Victory, st_dead}  // st_dead
	},
	// en_trans,
	{
		{0, SPR_TRANS_W1, 0, T_Stand, NULL, st_stand}, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		{0, SPR_TRANS_SHOOT1, 30, NULL,	NULL, st_shoot2}, // st_shoot1,
		{0, SPR_TRANS_SHOOT2, 10, NULL,	T_Shoot, st_shoot3}, // st_shoot2,
		{0, SPR_TRANS_SHOOT3, 10, NULL,	T_Shoot, st_shoot4}, // st_shoot3,
		{0, SPR_TRANS_SHOOT2, 10, NULL,	T_Shoot, st_shoot5}, // st_shoot4,
		{0, SPR_TRANS_SHOOT3, 10, NULL,	T_Shoot, st_shoot6}, // st_shoot5,
		{0, SPR_TRANS_SHOOT2, 10, NULL,	T_Shoot, st_shoot7}, // st_shoot6,
		{0, SPR_TRANS_SHOOT3, 10, NULL,	T_Shoot, st_shoot8}, // st_shoot7,
		{0, SPR_TRANS_SHOOT1, 10, NULL,	NULL, st_chase1}, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_TRANS_W1, 10, T_Chase, NULL, st_chase1s}, // st_chase1,
		{0, SPR_TRANS_W1,  3, NULL,		 NULL, st_chase2}, // st_chase1s,
		{0, SPR_TRANS_W2,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{0, SPR_TRANS_W3, 10, T_Chase, NULL, st_chase3s}, // st_chase3,
		{0, SPR_TRANS_W3,  3, NULL,		 NULL, st_chase4},	// st_chase3s,
		{0, SPR_TRANS_W4,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_TRANS_W1, 1, NULL, A_DeathScream, st_die2}, // st_die1,
		{0, SPR_TRANS_W1, 1, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_TRANS_DIE1, 15, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_TRANS_DIE2, 15, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_TRANS_DIE3, 15, NULL, NULL, st_dead}, // st_die5,
		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		{0, SPR_TRANS_DEAD, 0, NULL, NULL, st_dead}  // st_dead
	},
	// en_uber,
	{
		{0, SPR_UBER_W1, 0, T_Stand, NULL, st_stand}, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		{0, SPR_UBER_SHOOT1, 30, NULL, NULL, st_shoot2}, // st_shoot1,
		{0, SPR_UBER_SHOOT2, 12, NULL, T_UShoot, st_shoot3}, // st_shoot2,
		{0, SPR_UBER_SHOOT3, 12, NULL, T_UShoot, st_shoot4}, // st_shoot3,
		{0, SPR_UBER_SHOOT4, 12, NULL, T_UShoot, st_shoot5}, // st_shoot4,
		{0, SPR_UBER_SHOOT3, 12, NULL, T_UShoot, st_shoot6}, // st_shoot5,
		{0, SPR_UBER_SHOOT2, 12, NULL, T_UShoot, st_shoot7}, // st_shoot6,
		{0, SPR_UBER_SHOOT1, 12, NULL, NULL, st_chase1}, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_UBER_W1, 10, T_Chase, NULL, st_chase1s}, // st_chase1,
		{0, SPR_UBER_W1,  3, NULL,		 NULL, st_chase2}, // st_chase1s,
		{0, SPR_UBER_W2,  8, T_Chase, NULL, st_chase3}, // st_chase2,
		{0, SPR_UBER_W3, 10, T_Chase, NULL, st_chase3s}, // st_chase3,
		{0, SPR_UBER_W3,  3, NULL,		 NULL, st_chase4},	// st_chase3s,
		{0, SPR_UBER_W4,  8, T_Chase, NULL, st_chase1}, // st_chase4,

		{0, SPR_UBER_W1, 1, NULL, A_DeathScream, st_die2}, // st_die1,
		{0, SPR_UBER_W1, 1, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_UBER_DIE1, 15, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_UBER_DIE2, 15, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_UBER_DIE3, 15, NULL, NULL, st_die6}, // st_die5,
		{0, SPR_UBER_DIE4, 15, NULL, NULL, st_dead}, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		{0, SPR_UBER_DEAD, 0, NULL, NULL, st_dead}  // st_dead
	},
	// en_will,
	{
		{0, SPR_WILL_W1, 0, T_Stand, NULL, st_stand}, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		{0, SPR_WILL_SHOOT1, 30, NULL, NULL, st_shoot2}, // st_shoot1,
		{0, SPR_WILL_SHOOT2, 10, NULL, T_Launch, st_shoot3}, // st_shoot2,
		{0, SPR_WILL_SHOOT3, 10, NULL, T_Shoot, st_shoot4}, // st_shoot3,
		{0, SPR_WILL_SHOOT4, 10, NULL, T_Shoot, st_shoot5}, // st_shoot4,
		{0, SPR_WILL_SHOOT3, 10, NULL, T_Shoot, st_shoot6}, // st_shoot5,
		{0, SPR_WILL_SHOOT4, 10, NULL, T_Shoot, st_chase1}, // st_shoot6,
		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_WILL_W1, 10, T_BossChase, NULL, st_chase1s}, // st_chase1,
		{0, SPR_WILL_W1,  3, NULL,	 NULL, st_chase2}, // st_chase1s,
		{0, SPR_WILL_W2,  8, T_BossChase, NULL, st_chase3}, // st_chase2,
		{0, SPR_WILL_W3, 10, T_BossChase, NULL, st_chase3s}, // st_chase3,
		{0, SPR_WILL_W3,  3, NULL,	 NULL, st_chase4},	// st_chase3s,
		{0, SPR_WILL_W4,  8, T_BossChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_WILL_W1, 1, NULL, A_DeathScream, st_die2}, // st_die1,
		{0, SPR_WILL_W1, 10, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_WILL_DIE1, 10, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_WILL_DIE2, 10, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_WILL_DIE3, 10, NULL, NULL, st_dead}, // st_die5,
		ST_INFO_NULL, // st_die6,
		ST_INFO_NULL, // st_die7,
		ST_INFO_NULL, // st_die8,
		ST_INFO_NULL, // st_die9,

		{0, SPR_WILL_DEAD, 20, NULL, NULL, st_dead}  // st_dead
	},
	// en_death
	{
		{0, SPR_DEATH_W1, 0, T_Stand, NULL, st_stand}, // st_stand,

		ST_INFO_NULL, // st_path1,
		ST_INFO_NULL, // st_path1s,
		ST_INFO_NULL, // st_path2,
		ST_INFO_NULL, // st_path3,
		ST_INFO_NULL, // st_path3s,
		ST_INFO_NULL, // st_path4,

		ST_INFO_NULL, // st_pain,
		ST_INFO_NULL, // st_pain1,
	
		{0, SPR_DEATH_SHOOT1, 30, NULL, NULL, st_shoot2}, // st_shoot1,
		{0, SPR_DEATH_SHOOT2, 10, NULL, T_Launch, st_shoot3}, // st_shoot2,
		{0, SPR_DEATH_SHOOT4, 10, NULL, T_Shoot, st_shoot4}, // st_shoot3,
		{0, SPR_DEATH_SHOOT3, 10, NULL, T_Launch, st_shoot5}, // st_shoot4,
		{0, SPR_DEATH_SHOOT4, 10, NULL, T_Shoot, st_chase1}, // st_shoot5,
		ST_INFO_NULL, // st_shoot6,
		ST_INFO_NULL, // st_shoot7,
		ST_INFO_NULL, // st_shoot8,
		ST_INFO_NULL, // st_shoot9,

		{0, SPR_DEATH_W1, 10, T_BossChase, NULL, st_chase1s}, // st_chase1,
		{0, SPR_DEATH_W1,  3, NULL,	  NULL, st_chase2}, // st_chase1s,
		{0, SPR_DEATH_W2,  8, T_BossChase, NULL, st_chase3}, // st_chase2,
		{0, SPR_DEATH_W3, 10, T_BossChase, NULL, st_chase3s}, // st_chase3,
		{0, SPR_DEATH_W3,  3, NULL,	  NULL, st_chase4},	// st_chase3s,
		{0, SPR_DEATH_W4,  8, T_BossChase, NULL, st_chase1}, // st_chase4,

		{0, SPR_DEATH_W1, 1, NULL, A_DeathScream, st_die2}, // st_die1,
		{0, SPR_DEATH_W1, 10, NULL, NULL, st_die3}, // st_die2,
		{0, SPR_DEATH_DIE1, 10, NULL, NULL, st_die4}, // st_die3,
		{0, SPR_DEATH_DIE2, 10, NULL, NULL, st_die5}, // st_die4,
		{0, SPR_DEATH_DIE3, 10, NULL, NULL, st_die6}, // st_die5,
		{0, SPR_DEATH_DIE4, 10, NULL, NULL, st_die7}, // st_die6,
		{0, SPR_DEATH_DIE5, 10, NULL, NULL, st_die7}, // st_die7,
		{0, SPR_DEATH_DIE6, 10, NULL, NULL, st_die7}, // st_die8,
		ST_INFO_NULL, // st_die9,

		{0, SPR_DEATH_DEAD, 0, NULL, NULL, st_dead}  // st_dead
	}

};

int	starthitpoints[ 4 ][ NUMENEMIES ] =
//
// BABY MODE
//
{
 {25,	// guards
	50,	// officer
	100,	// SS
	1,	// dogs
	850,	// Hans
	850,	// Schabbs
	200,	// fake hitler
	800,	// mecha hitler
	500,  // hitler
	45,	// mutants
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts

	850,	// Gretel
	850,	// Gift
	850,	// Fat

// --- Projectiles
	0,		// en_needle,
	0,		// en_fire,
	0,		// en_rocket,
	0,		// en_smoke,
	100,	// en_bj,
// --- Spear of destiny!
	0,		// en_spark,
	0,		// en_hrocket,
	0,		// en_hsmoke,

	5,		// en_spectre,
	1450,	// en_angel,
	850,	// en_trans,
	1050,	// en_uber,
	950,	// en_will,
	1250	// en_death
	},
	 //
	 // DON'T HURT ME MODE
	 //
 {25,	// guards
	50,	// officer
	100,	// SS
	1,	// dogs
	950,	// Hans
	950,	// Schabbs
	300,	// fake hitler
	950,	// mecha hitler
	700,	// hitler
	55,	// mutants
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts

	950,	// Gretel
	950,	// Gift
	950,	// Fat

// --- Projectiles
	0,		// en_needle,
	0,		// en_fire,
	0,		// en_rocket,
	0,		// en_smoke,
	100,	// en_bj,
// --- Spear of destiny!
	0,		// en_spark,
	0,		// en_hrocket,
	0,		// en_hsmoke,

	10,		// en_spectre,
	1550,	// en_angel,
	950,	// en_trans,
	1150,	// en_uber,
	1050,	// en_will,
	1350	// en_death
	},
	 //
	 // BRING 'EM ON MODE
	 //
 {25,	// guards
	50,	// officer
	100,	// SS
	1,	// dogs

	1050,	// Hans
	1550,	// Schabbs
	400,	// fake hitler
	1050,	// mecha hitler
	800,	// hitler

	55,	// mutants
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts

	1050,	// Gretel
	1050,	// Gift
	1050,	// Fat

// --- Projectiles
	0,		// en_needle,
	0,		// en_fire,
	0,		// en_rocket,
	0,		// en_smoke,
	100,	// en_bj,
// --- Spear of destiny!
	0,		// en_spark,
	0,		// en_hrocket,
	0,		// en_hsmoke,

	15,	// en_spectre,
	1650,	// en_angel,
	1050,	// en_trans,
	1250,	// en_uber,
	1150,	// en_will,
	1450	// en_death
	},
	 //
	 // DEATH INCARNATE MODE
	 //
 {25,	// guards
	50,	// officer
	100,	// SS
	1,	// dogs

	1200,	// Hans
	2400,	// Schabbs
	500,	// fake hitler
	1200,	// mecha hitler
	900,	// hitler

	65,	// mutants
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts
	25,	// ghosts

	1200,	// Gretel
	1200,	// Gift
	1200,	// Fat

// --- Projectiles
	0,		// en_needle,
	0,		// en_fire,
	0,		// en_rocket,
	0,		// en_smoke,
	100,	// en_bj,
// --- Spear of destiny!
	0,		// en_spark,
	0,		// en_hrocket,
	0,		// en_hsmoke,

	25,	// en_spectre,
	2000,	// en_angel,
	1200,	// en_trans,
	1400,	// en_uber,
	1300,	// en_will,
	1600	// en_death
	}
};


#endif /* __WOLF_ACT_STAT_H__ */

