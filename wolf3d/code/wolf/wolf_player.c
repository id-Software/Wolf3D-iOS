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
 *	wolf_player.c:  Wolfenstein3-D player management.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 *
 */

#include "../wolfiphone.h"

player_t Player; // player struct (pos, health etc...)


#define PLAYERSIZE	MINDIST	// player radius



struct atkinf
{
	char tics, attack, frame; // attack is 1 for gun, 2 for knife

} attackinfo[ 4 ][ 14 ] = // 4 guns, 14 frames max for every gun!
{
	{ {6,0,1},{6,2,2},{6,0,3},{6,-1,0} },
	{ {6,0,1},{6,1,2},{6,0,3},{6,-1,0} },
	{ {6,0,1},{6,1,2},{6,3,3},{6,-1,0} },
	{ {6,0,1},{6,1,2},{6,4,3},{6,-1,0} },
};


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: true if player can change weapons, otherwise false.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE _boolean PL_ChangeWeapon( player_t *self, int weapon )
{
	unsigned itemflag;
	
	itemflag = ITEM_WEAPON_1 << weapon;

	if( self->ammo[ AMMO_BULLETS ] == 0 && weapon != WEAPON_KNIFE )
	{
//		Com_Printf("Not enough ammo.\n");
		return false;
	}

	if( ! (self->items & itemflag) ) 
	{
//		Com_Printf( "No weapon.\n" );
		return false;
	}


	self->weapon = 
	self->pendingweapon = weapon;

	self->attackframe = 
	self->attackcount = 
	self->weaponframe = 0;

	return true;
}


/*
-----------------------------------------------------------------------------
 Function: Called if player pressed USE button
 
 Parameters:
 
 Returns: returns true if player used something
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE _boolean PL_Use( player_t *self, LevelData_t *lvl )
{
	int x, y, dir;

	dir = Get4dir( FINE2RAD( self->position.angle ) );
	x = self->tilex + dx4dir[ dir ];
	y = self->tiley + dy4dir[ dir ];

	if( lvl->tilemap[ x ][ y ] & DOOR_TILE )
	{
		return Door_TryUse( &lvl->Doors.DoorMap[ x ][ y ], Player.items );
	}

	if( lvl->tilemap[ x ][ y ] & SECRET_TILE )
	{
		return PushWall_Push( x, y, dir );
	}

	if( lvl->tilemap[ x ][ y ] & ELEVATOR_TILE )
	{
		int newtex;
		
		switch( dir )
		{
			case dir4_east:
			case dir4_west:
				newtex = lvl->wall_tex_x[ x ][ y ] += 2;
				break;
				
			case dir4_north:
			case dir4_south:
				return false; // don't allow to press elevator rails
		}
		
		if( lvl->tilemap[ self->tilex ][ self->tiley ] & SECRETLEVEL_TILE )
		{
			self->playstate = ex_secretlevel;
		}
		else
		{
			self->playstate = ex_complete;
		}
		Sound_StartSound( NULL, 0, CHAN_BODY, Sound_RegisterSound( "lsfx/040.wav" ), 1, ATTN_NORM, 0 );
		
		iphoneStartIntermission( 0 );
		
		return true;
	}

	//Sound_StartSound( NULL, 0, CHAN_BODY, Sound_RegisterSound( "lsfx/020.wav" ), 1, ATTN_NORM, 0 );	
	return false;
}



#define STOPSPEED 0x0D00
#define FRICTION  0.25f
#define MAXMOVE 	(MINDIST*2-1)


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: returns true if move ok
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE _boolean PL_TryMove( player_t *self, LevelData_t *lvl )
{
	int xl, yl, xh, yh, x, y;
	int d, n;

	xl = POS2TILE( Player.position.origin[ 0 ] - PLAYERSIZE );
	yl = POS2TILE( Player.position.origin[ 1 ] - PLAYERSIZE );
	xh = POS2TILE( Player.position.origin[ 0 ] + PLAYERSIZE );
	yh = POS2TILE( Player.position.origin[ 1 ] + PLAYERSIZE );

	// Cheching for solid walls:
	for( y = yl ; y <= yh ; ++y )
		for( x = xl ; x <= xh ; ++x )
	{
		if( lvl->tilemap[ x ][ y ] & SOLID_TILE ) 
			return 0;

		if( lvl->tilemap[ x ][ y ] & DOOR_TILE && 
		   Door_Opened( &lvl->Doors, x, y) != DOOR_FULLOPEN ) {
			// iphone hack to allow player to move halfway into door tiles
			// if the player bounds doesn't cross the middle of the tile, let the move continue			
			if ( abs( Player.position.origin[0] - TILE2POS( x ) ) <= 0x9000
				&& abs( Player.position.origin[1] - TILE2POS( y ) ) <= 0x9000 ) {
				return 0;
			}
		}
	}

// check for actors
	for( n = 0 ; n < NumGuards ; ++n )
	{
		if( Guards[ n ].state >= st_die1 ) 
			continue;

		d = self->position.origin[ 0 ] - Guards[ n ].x;

		if( d < -MINACTORDIST || d > MINACTORDIST )
			continue;

		d = self->position.origin[ 1 ] - Guards[ n ].y;

		if( d < -MINACTORDIST || d > MINACTORDIST)
			continue;

		return false;
	}

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PL_ClipMove( player_t *self, int xmove, int ymove )
{
	int basex, basey;

	basex = self->position.origin[ 0 ];
	basey = self->position.origin[ 1 ];

	self->position.origin[ 0 ] += xmove;
	self->position.origin[ 1 ] += ymove;
	if( PL_TryMove( self, r_world ) )
	{
		return; // we moved as we wanted
	}

	//Sound_StartSound( NULL, 0, CHAN_BODY, Sound_RegisterSound( "lsfx/000.wav" ), 1, ATTN_NORM, 0 );	

	if( xmove )	// don't bother if we don't move x!
	{
		self->position.origin[ 0 ] = basex + xmove;
		self->position.origin[ 1 ] = basey;
		if( PL_TryMove( self, r_world ) ) 
		{
			return; // May be we'll move only X direction?
		}
	}
	if( ymove )	// don't bother if we don't move y!
	{
		self->position.origin[ 0 ] = basex;
		self->position.origin[ 1 ] = basey + ymove;
		if( PL_TryMove( self, r_world ) ) 
		{
			return; // May be we'll move only Y direction?
		}
	}

// movement blocked; we must stay on one place... :(
	self->position.origin[ 0 ] = basex;
	self->position.origin[ 1 ] = basey;
}


/*
-----------------------------------------------------------------------------
 Function: Changes player's angle and position
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PL_ControlMovement( player_t *self, LevelData_t *lvl )
{
	int angle, speed;

// rotation
	angle = self->position.angle;

//	if(cmd->forwardmove || cmd->sidemove)
		self->movx = self->movy = 0; // clear accumulated movement

	if( Player.cmd.forwardmove )
	{
		speed = tics * Player.cmd.forwardmove;
		self->movx+=(int)(speed * CosTable[ angle ] );
		self->movy+=(int)(speed * SinTable[ angle ] );
	}
	if( Player.cmd.sidemove )
	{
		speed = tics * Player.cmd.sidemove;
		self->movx += (int)( speed * SinTable[ angle ] );
		self->movy -= (int)( speed * CosTable[ angle ] );
	}

	if( ! self->movx && ! self->movy ) 
		return;
	
#ifdef SPEAR

	funnyticount = 0; // ZERO FUNNY COUNTER IF MOVED! // FIXME!

#endif
	self->speed = self->movx + self->movy;

// bound movement
	if( self->movx > MAXMOVE ) 
		self->movx = MAXMOVE;
	else if( self->movx < -MAXMOVE ) 
		self->movx = -MAXMOVE;

	if( self->movy > MAXMOVE ) 
		self->movy = MAXMOVE;
	else if( self->movy < -MAXMOVE ) 
		self->movy = -MAXMOVE;

// move player and clip movement to walls (check for no-clip mode here)
	PL_ClipMove( self, self->movx, self->movy );
	self->tilex = POS2TILE( self->position.origin[ 0 ] );
	self->tiley = POS2TILE( self->position.origin[ 1 ] );

	// pick up items easier -- any tile you touch, instead of
	// just the midpoint tile
	{
		int	x, y;
		
		for ( x = -1 ; x <= 1 ; x+= 2 ) {
			int	tilex = POS2TILE( self->position.origin[0] + x * PLAYERSIZE );
			for ( y = -1 ; y <= 1 ; y+= 2 ) {
				int	tiley = POS2TILE( self->position.origin[1] + y * PLAYERSIZE );
				Powerup_PickUp( tilex, tiley );
			}
		}
	}
//	Powerup_PickUp( self->tilex, self->tiley );

// Checking for area change, ambush tiles and doors will have negative values
	if( lvl->areas[ self->tilex ][ self->tiley ] >= 0 &&
		lvl->areas[ self->tilex ][ self->tiley ] != Player.areanumber )
	{
		Player.areanumber = lvl->areas[ self->tilex ][ self->tiley ];
		assert( Player.areanumber >= 0 && Player.areanumber < NUMAREAS );
		Areas_ConnectAreas( Player.areanumber );
	}

	if( lvl->tilemap[ self->tilex ][ self->tiley ] & EXIT_TILE )
	{
		iphoneStartIntermission( 0 );
	}
	

}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PL_PlayerAttack( player_t *self, _boolean re_attack )
{
	struct atkinf *cur;

	self->attackcount -= tics;
	while( self->attackcount <= 0 )
	{
		cur = &attackinfo[ self->weapon ][ self->attackframe ];
		switch( cur->attack )
		{
			case -1:
				self->flags &= ~PL_FLAG_ATTCK;
				if( ! self->ammo[ AMMO_BULLETS ] )
				{
					self->weapon = WEAPON_KNIFE;
				}
				else if( self->weapon != self->pendingweapon )
				{
					self->weapon = self->pendingweapon;
				}
				self->attackframe = self->weaponframe = 0;
				return;

			case 4:
				if( ! self->ammo[ AMMO_BULLETS ] )
				{
					break;
				}

				if( re_attack ) 
				{
					self->attackframe -= 2;
				}

			case 1:
				if( ! self->ammo[ AMMO_BULLETS ] ) // can only happen with chain gun
				{
					self->attackframe++;
					break;
				}
				fire_lead( self );
				self->ammo[ AMMO_BULLETS ]--;
				break;

			case 2:
				fire_hit( self );
				break;

			case 3:
				if(self->ammo[AMMO_BULLETS] && re_attack)
					self->attackframe-=2;
				break;
		}

		self->attackcount += cur->tics;
		self->attackframe++;
		self->weaponframe = attackinfo[ self->weapon ][ self->attackframe ].frame;
	}

}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_Process( player_t *self, LevelData_t *lvl )
{	
	int n;

	self->madenoise = false;

	PL_ControlMovement( self, lvl );

	if( self->flags & PL_FLAG_ATTCK )
	{
		PL_PlayerAttack( self, Player.cmd.buttons & BUTTON_ATTACK );
	}
	else
	{
		if( Player.cmd.buttons & BUTTON_USE )
		{
			if(!(self->flags & PL_FLAG_REUSE) && PL_Use( self, lvl ) )
			{
				self->flags|=PL_FLAG_REUSE;
			}
		}
		else
		{
			self->flags &= ~PL_FLAG_REUSE;
		}

		if( Player.cmd.buttons & BUTTON_ATTACK )
		{
			self->flags |= PL_FLAG_ATTCK;
			
			//gsh
			if (self->previousweapon != WEAPON_KNIFE && self->previousweapon)
			{
				//self->weapon = self->previousweapon;
				PL_ChangeWeapon(self, self->previousweapon);
			}
			
			self->attackframe = 0;
			self->attackcount = attackinfo[ self->weapon ][ 0 ].tics;
			self->weaponframe = attackinfo[ self->weapon ][ 0 ].frame;
		}
		else if ( Player.cmd.buttons & BUTTON_ALTERNATE_ATTACK )   //gsh
		{
			self->flags |= PL_FLAG_ATTCK;
			
//			PL_ChangeWeapon(self, WEAPON_KNIFE);
			if (self->weapon != WEAPON_KNIFE)
			{
				self->previousweapon = self->weapon;
				self->weapon = WEAPON_KNIFE;
			}
			
			self->attackframe = 0;
			self->attackcount = attackinfo[ self->weapon ][ 0 ].tics;
			self->weaponframe = attackinfo[ self->weapon ][ 0 ].frame;
		}
		else if ( Player.cmd.buttons & BUTTON_CHANGE_WEAPON ) {
			self->pendingweapon=self->weapon;
			for( n = 0 ; n < 4; ++n )
			{
				if( ++self->weapon > WEAPON_CHAIN ) 
				{
					self->weapon = WEAPON_KNIFE;
				}
				
				if( PL_ChangeWeapon( self, self->weapon ) ) 
				{
					break;
				}
			}
			self->weapon = self->pendingweapon;
		}
		
	}
}



/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_Reset(void)
{
	memset( &Player, 0, sizeof( Player ) );
	Player.playstate = ex_notingame;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_Spawn( placeonplane_t location, LevelData_t *lvl )
{
	Player.position = location;
	Player.tilex = POS2TILE( location.origin[ 0 ] );
	Player.tiley = POS2TILE( location.origin[ 1 ] );
	Player.areanumber = lvl->areas[ Player.tilex ][ Player.tiley ];
	assert( Player.areanumber >= 0 && Player.areanumber < NUMAREAS );
	if( Player.areanumber < 0 ) 
	{
		Player.areanumber = 36;
	}

	Areas_ConnectAreas( Player.areanumber );
	
	//gsh
	iphoneSetLevelNotifyText();
#if 0
	char str[128];
	//sprintf( str, "Entering level E%iM%i", currentMap.episode + 1, currentMap.map + 1 );
	//gsh
	if (currentMap.episode < 6)
		sprintf( str, "Entering level E%iM%i", currentMap.episode+1, currentMap.map+1 );
	else if (currentMap.episode < 10) {
		int currentLevel = currentMap.episode * 10 + currentMap.map;
		switch (currentLevel) {
			case 60: case 61: case 62: case 63: case 64:
				sprintf( str, "Entering Tunnels %i", currentLevel-60+1);
				break;
			case 78:
				sprintf( str, "Entering Tunnels %i", 6);
				break;				
			case 65: case 66: case 67: case 68: case 69:
				sprintf( str, "Entering Dungeons %i", currentLevel-65+1);
				break;
			case 79:
				sprintf( str, "Entering Dungeons %i", 6);
				break;				
			case 70: case 71: case 72: case 73: case 74: case 75:
				sprintf( str, "Entering Castle");
				break;
			case 76:
				sprintf( str, "Entering Ramparts");
				break;
			case 77:
				sprintf( str, "Entering Death Knight");
				break;
			case 80:
				sprintf( str, "Entering Dungeon Dimension");
				break;
			default:
				sprintf( str, "  ");
				break;
		}
	}
	else 
		sprintf( str, "Entering level custom %i", /*currentMap.episode+1,*/ currentMap.map+1 );
	
	iphoneSetNotifyText( str );
#endif
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_Give_f( void )
{
	Com_Printf( "Giving stuff.\n" );
	PL_GiveHealth( &Player, 999, 0 );
	PL_GiveAmmo( &Player, AMMO_BULLETS, 99 );
	PL_GiveWeapon( &Player, WEAPON_AUTO );
	PL_GiveWeapon( &Player, WEAPON_CHAIN );
	PL_GiveKey( &Player, KEY_GOLD );
	PL_GiveKey( &Player, KEY_SILVER );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cmd_God_f( void )
{
	Player.flags ^= FL_GODMODE;

	Com_Printf( "God mode %s\n", Player.flags & FL_GODMODE ? "ON":"OFF" );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void PL_notarget_f( void )
{
	Player.flags ^= FL_NOTARGET;
	Com_Printf( "Notarget mode %s\n", Player.flags & FL_NOTARGET ? "ON":"OFF" );
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_Init(void)
{
	PL_Reset();
	PL_NewGame( &Player );

	Cmd_AddCommand( "god", Cmd_God_f );
	Cmd_AddCommand( "notarget", PL_notarget_f );
	Cmd_AddCommand( "give", Cmd_Give_f );
}

// ------------------------- * environment interraction * -------------------------
#define EXTRAPOINTS 40000	// points for an extra life


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_Damage( player_t *self, entity_t *attacker, int points )
{
	
	if( self->playstate == ex_dead )
	{
		return;
	}

	self->LastAttacker = attacker;

	if( skill->value == gd_baby )
	{
		points >>= 2;
	}

	// vibe the phone
	SysIPhoneVibrate();

	// note the direction of the last hit for the directional blends
	{
		int		dx = attacker->x - self->position.origin[0];
		int		dy = attacker->y - self->position.origin[1];
		
		// probably won't ever have damage from self, but check anyway
		if ( dx != 0 || dy != 0 ) {
			float angle = atan2f( dy, dx );
			float playerAngle = self->position.angle * 360.0f / (float)ANG_360;
			float deltaAngle;
			angle = angle * 180.0f / M_PI;
			if ( angle < 0 ) {
				angle = 360 + angle;
			}
			deltaAngle = angle - playerAngle;
			if ( deltaAngle > 180 ) {
				deltaAngle = deltaAngle - 360;
			}
			if ( deltaAngle < -180 ) {
				deltaAngle = 360 + deltaAngle;
			}
//			Com_Printf( "damage: player angle: %4.0f shotAngle: %4.0f deltaAngle:%4.0f\n", playerAngle, angle, deltaAngle );
			if ( deltaAngle > 40 ) {
				iphoneSetAttackDirection( 1 );
			} else if ( deltaAngle < -40 ) {
				iphoneSetAttackDirection( -1 );
			}
		}
	}

	// do everything else but subtract health in god mode, to ease
	// testing of damage feedback
	if( !(self->flags & FL_GODMODE) )
	{
		self->health -= points;
	}
	
	if( self->health <= 0 )
	{
		// dead
		self->health = 0;
		self->playstate = ex_dead;

		Sound_StartSound( NULL, 0, CHAN_BODY, Sound_RegisterSound( "lsfx/009.wav" ), 1, ATTN_NORM, 0 );
	}

	// red screen flash
	iphoneStartDamageFlash( points );
	
	// stop the happy grin face if shot before it times out
	Player.face_gotgun = false;

	// make BJ's eyes bulge on huge hits
	if( points > 30 && Player.health != 0 )
	{
		Player.face_ouch = true;
		Player.facecount = 0;
	}
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: returns true if player needs this health.
 
 Notes: 
	gives player some HP
	max can be:
	 0 - natural player's health limit (100 or 150 with augment)
	>0 - indicates the limit
-----------------------------------------------------------------------------
*/
PUBLIC _boolean PL_GiveHealth( player_t *self, int points, int max )
{
	if( max == 0 )
	{
		max = (self->items & ITEM_AUGMENT) ? 150 : 100;
	}

	if( self->health >= max )
	{
		return false; // doesn't need this health
	}

	self->health += points;

	if( self->health > max )
	{
		self->health = max;
	}

	Player.face_gotgun = false;

	return true; // took it
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns: returns true if player needs this ammo
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean PL_GiveAmmo( player_t *self, int type, int ammo )
{
	int max_ammo[ AMMO_TYPES ] = { 99 };
	int max;

	max = max_ammo[ type ];
	if( self->items & ITEM_BACKPACK )
	{
		max *= 2;
	}

	if( self->ammo[ type ] >= max )
	{
		return false; // don't need
	}

	if( ! self->ammo[ type ] && ! self->attackframe ) // knife was out
	{
		self->weapon = self->pendingweapon;
	}

	self->ammo[ type ] += ammo;
	if( self->ammo[ type ] > max )
	{
		self->ammo[ type ] = max;
	}

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_GiveWeapon( player_t *self, int weapon )
{
	unsigned itemflag;

	PL_GiveAmmo( self, AMMO_BULLETS, 6 ); // give some ammo with a weapon

	itemflag = ITEM_WEAPON_1 << weapon;
	if( self->items & itemflag )
	{
		return; // player owns this weapon
	}
	else
	{
		self->items |= itemflag;
		if ( self->weapon < weapon ) {	// don't switch if already using better weapon
			self->weapon = self->pendingweapon = weapon;
		}
	}
}


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_GivePoints( player_t *self, W32 points )
{
#if 0		// no score on iphone
	self->score += points;
	while( self->score >= self->next_extra )
	{
		self->next_extra += EXTRAPOINTS;
		PL_GiveLife( self );
	}	
#endif
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_GiveKey( player_t *self, int key )
{
	self->items |= ITEM_KEY_1 << key;
}


/*
-----------------------------------------------------------------------------
 Function:  Set up player for the new game
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_NewGame( player_t *self )
{
	memset( self, 0, sizeof( player_t ) );

	self->health = 100;
	self->ammo[ AMMO_BULLETS ] = 16;	// JDC: changed for iphone 8;
	self->lives = 3;

	self->previousweapon = WEAPON_KNIFE; //gsh
	self->weapon = self->pendingweapon = WEAPON_PISTOL;
	self->items = ITEM_WEAPON_1 | ITEM_WEAPON_2;
	self->next_extra = EXTRAPOINTS;
}

/*
-----------------------------------------------------------------------------
 Function: Set up player for level transition
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PL_NextLevel( player_t *self )
{
	self->old_score = self->score;
	self->attackcount = self->attackframe = self->weaponframe = 0;
	self->flags = 0;

	self->items &= ~(ITEM_KEY_1 | ITEM_KEY_2 | ITEM_KEY_3 | ITEM_KEY_4);
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters: self -[in] Player to respawn in game world.
 
 Returns: returns false if no lives left
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean PL_Reborn( player_t *self )
{
#if 0		// removed game over from iphone version
	if( --self->lives < 1 )
	{
		return false;
	}
#endif

	self->health = 100;
	self->ammo[ AMMO_BULLETS ] = 16;	// JDC: changed for iphone 8;
	self->score = self->old_score;
	self->attackcount = 0;
	self->attackframe = 0;
	self->weaponframe = 0;
	self->flags = 0;

	self->previousweapon = WEAPON_KNIFE; //gsh
	self->weapon = self->pendingweapon = WEAPON_PISTOL;
	self->items = ITEM_WEAPON_1 | ITEM_WEAPON_2;

	self->playstate = ex_playing;

	return true;
}
