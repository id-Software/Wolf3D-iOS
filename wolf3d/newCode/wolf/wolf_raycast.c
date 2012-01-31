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
 *	wolf_raycast.c:  Wolfenstein3-D ray-casting.
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


W8 tile_visible[ 64 ][ 64 ]; // can player see this tile?


/*
-----------------------------------------------------------------------------
 Function: R_RayCast() -Ray cast viewport. 
 
 Parameters: viewport -[in] Position of camera.
			 lvl -[in] Pointer to valid LevelData_t structure.
 
 Returns: Nothing.
 
 Notes: Marks all visible tiles in tile_visible[] array.

-----------------------------------------------------------------------------
*/
PUBLIC void R_RayCast( placeonplane_t viewport, LevelData_t *lvl )
{
	int n, x, y, angle, vx, vy;
	r_trace_t trace;

	memset( tile_visible, 0, sizeof( tile_visible ) ); // clear tile visible flags

// viewport tile coordinates
	x = viewport.origin[ 0 ];
	y = viewport.origin[ 1 ];
	angle = viewport.angle;

	vx = POS2TILE( viewport.origin[ 0 ] );
	vy = POS2TILE( viewport.origin[ 1 ] );

	trace.tile_vis = tile_visible;
	trace.flags = TRACE_SIGHT | TRACE_MARK_MAP;

//
// Ray casting
//

	// FIXME: control ray count and make angle init
	for( n = 0 ; n < 640 ; ++n ) 
	{
		trace.x = x;
		trace.y = y;
		trace.a = NormalizeAngle( angle + ColumnAngle[ n ] );

		R_Trace( &trace, lvl );
	}

//
// Rendering
//
	for( x = 0 ; x < 64; ++x )
	for( y = 0 ; y < 64; ++y )
	if( tile_visible[ x ][ y ] )
	{
		lvl->tileEverVisible[x][y] = 1;	// for automap
		if( lvl->tilemap[ x ][ y ] & DOOR_TILE )
		{
		/* door */
			if( lvl->Doors.DoorMap[ x ][ y ].action != dr_open )
			{
				_boolean backside = false;

				if( lvl->Doors.DoorMap[ x ][ y ].vertical )
				{
					if( x < vx ) 
						backside = true;
				}
				else
				{
					if( y < vy ) 
						backside = true;
				}

				R_Draw_Door( x, y, LOWERZCOORD, UPPERZCOORD, 
							 lvl->Doors.DoorMap[ x ][ y ].vertical,
							 backside, 
							 lvl->Doors.DoorMap[ x ][ y ].texture, 
							 Door_Opened( &lvl->Doors, x, y ) );
			}
		/* door sides */
			if( lvl->Doors.DoorMap[ x ][ y ].vertical )
			{
				if( y <= vy )
					R_Draw_Wall( (float)x, (float)(y-1), LOWERZCOORD, UPPERZCOORD, dir4_north, TEX_PLATE );

				if( y >= vy )
					R_Draw_Wall( (float)x, (float)(y+1), LOWERZCOORD, UPPERZCOORD, dir4_south, TEX_PLATE );

				if( x <= vx && lvl->tilemap[ x - 1 ][ y ] & WALL_TILE )
					R_Draw_Wall( (float)(x-1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_east, lvl->wall_tex_x[ x - 1 ][ y ] );
				
				if( x >= vx && lvl->tilemap[ x + 1 ][ y ] & WALL_TILE )
					R_Draw_Wall( (float)(x+1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_west, lvl->wall_tex_x[ x + 1 ][ y ] );
			}
			else
			{
				if( x <= vx )
					R_Draw_Wall((float)(x-1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_east, TEX_PLATE+1);

				if( x >= vx )
					R_Draw_Wall((float)(x+1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_west, TEX_PLATE+1);

				if( y <= vy && lvl->tilemap[ x ][ y - 1 ] & WALL_TILE )
					R_Draw_Wall( (float)x, (float)(y-1), LOWERZCOORD, UPPERZCOORD, dir4_north, lvl->wall_tex_y[x][y-1]);
				
				if( y >= vy && lvl->tilemap[ x ][ y + 1 ] & WALL_TILE )
					R_Draw_Wall( (float)x, (float)(y+1), LOWERZCOORD, UPPERZCOORD, dir4_south, lvl->wall_tex_y[x][y+1]);
			}
		}
		else
		{
			/* Push-Wall */
			if( (r_world->tilemap[ x ][ y ] & PUSHWALL_TILE) )
			{
				float dx, dy;

				dx = PWall.dx * PWall.PWpointsmoved / 128.0f;
				dy = PWall.dy * PWall.PWpointsmoved / 128.0f;

				if( PWall.x <= vx )
					R_Draw_Wall( (float)PWall.x + dx, (float)PWall.y + dy, LOWERZCOORD, UPPERZCOORD, dir4_east, PWall.tex_x );

				if( PWall.x >= vx )
					R_Draw_Wall( (float)PWall.x + dx, (float)PWall.y + dy, LOWERZCOORD, UPPERZCOORD, dir4_west, PWall.tex_x );

				if( PWall.y <= vy )
					R_Draw_Wall( (float)PWall.x + dx, (float)PWall.y + dy, LOWERZCOORD, UPPERZCOORD, dir4_north, PWall.tex_y );

				if( PWall.y >= vy )
					R_Draw_Wall( (float)PWall.x + dx, (float)PWall.y + dy, LOWERZCOORD, UPPERZCOORD, dir4_south, PWall.tex_y );

			}
			/* x-wall */
			if( x <= vx && r_world->tilemap[ x - 1 ][ y ] & WALL_TILE )
				R_Draw_Wall( (float)(x-1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_east, r_world->wall_tex_x[x-1][y]);
			
			if( x >= vx && r_world->tilemap[ x + 1 ][ y ] & WALL_TILE )
				R_Draw_Wall( (float)(x+1), (float)y, LOWERZCOORD, UPPERZCOORD, dir4_west, r_world->wall_tex_x[x+1][y]);
			
			/* y-wall */
			if( y <= vy && r_world->tilemap[ x ][ y - 1 ] & WALL_TILE )
				R_Draw_Wall( (float)x, (float)(y-1), LOWERZCOORD, UPPERZCOORD, dir4_north, r_world->wall_tex_y[x][y-1]);
			
			if( y >= vy && r_world->tilemap[ x ][ y + 1 ] & WALL_TILE )
				R_Draw_Wall( (float)x, (float)(y+1), LOWERZCOORD, UPPERZCOORD, dir4_south, r_world->wall_tex_y[x][y+1]);
		}


	}
}


int x_tile_step[ 4 ] = { 1, -1, -1,  1 };
int y_tile_step[ 4 ] = { 1,  1, -1, -1 };


/*
-----------------------------------------------------------------------------
 Function: R_TraceCheck() -Trace ray check. 
 
 Parameters: 
			 lvl -[in] Pointer to valid LevelData_t structure.
			 x, y -[in] In tiles.
 
 Returns: true to stop tracing, false otherwise.
 
 Notes: Tells ray casting if we hit a wall or door and to stop tracing.

-----------------------------------------------------------------------------
*/
PRIVATE _boolean R_TraceCheck( LevelData_t *lvl, int x, int y, int frac, int dfrac, _boolean vert, _boolean flip, r_trace_t *trace )
{
	if( lvl->tilemap[ x ][ y ] & WALL_TILE )
	{
		if( vert )
		{
			trace->x = (x << TILESHIFT) + (flip ? TILEGLOBAL : 0);
			trace->y = (y << TILESHIFT) + frac;
			trace->flags |= TRACE_HIT_VERT;
		}
		else
		{
			trace->x = (x << TILESHIFT) + frac;
			trace->y = (y << TILESHIFT) + (flip ? TILEGLOBAL : 0);
			trace->flags &= ~TRACE_HIT_VERT;
		}

		return true; // wall, stop tracing
	}

	if( trace->tile_vis )
	{
		trace->tile_vis[ x ][ y ] = true; // this tile is visible
	}


	if( lvl->tilemap[ x ][ y ] & DOOR_TILE &&
		lvl->Doors.DoorMap[ x ][ y ].action != dr_open )
	{
		frac += dfrac >> 1;
		if( POS2TILE( frac ) )
			return false;

		if( vert )
		{
			if( lvl->Doors.DoorMap[ x ][ y ].action != dr_closed &&
				(frac >> 10) > DOOR_FULLOPEN - Door_Opened( &lvl->Doors, x, y ) )
			{
				return false; // opened enough
			}

			trace->x = TILE2POS( x );
			trace->y = (y << TILESHIFT) + frac;
			trace->flags |= TRACE_HIT_VERT;
		}
		else
		{
			if( lvl->Doors.DoorMap[ x ][ y ].action != dr_closed &&
				(frac >> 10) < Door_Opened( &lvl->Doors, x, y ) )
			{
				return false; // opened enough
			}

			trace->y = TILE2POS( y );
			trace->x = (x << TILESHIFT) + frac;
			trace->flags &= ~TRACE_HIT_VERT;
		}
		trace->flags |= TRACE_HIT_DOOR;

		return true; // closed door, stop tracing
	}

	return false; // no intersection, go on!
}

/*
-----------------------------------------------------------------------------
 Function: R_Trace() -Trace ray. 
 
 Parameters: 
			 trace -[in] Pointer to valid r_trace_t structure.
			 lvl -[in] Pointer to valid LevelData_t structure.

 
 Returns: Nothing.
 
 Notes:

-----------------------------------------------------------------------------
*/
PUBLIC void R_Trace( r_trace_t *trace, LevelData_t *lvl )
{
	int xtilestep, ytilestep;
	int xstep, ystep;
	int xtile, ytile;
	int xintercept, yintercept;
	int YmapPos, XmapPos;
	quadrant q;

// Setup for ray casting
	q = GetQuadrant( FINE2RAD( trace->a ) );

	xtilestep = x_tile_step[ q ];
	ytilestep = y_tile_step[ q ];

	xtile = POS2TILE( trace->x ) + xtilestep;
	ytile = POS2TILE( trace->y ) + ytilestep;

	xstep = ytilestep * XnextTable[ trace->a ];
	ystep = xtilestep * YnextTable[ trace->a ];

	xintercept = (int)( ( ((ytilestep == -1 ? ytile+1 : ytile) << TILESHIFT) - trace->y ) / TanTable[ trace->a ]) + trace->x;
	yintercept = (int)( ( ((xtilestep == -1 ? xtile+1 : xtile) << TILESHIFT) - trace->x ) * TanTable[ trace->a ]) + trace->y;

	YmapPos = yintercept >> TILESHIFT; // toXray
	XmapPos = xintercept >> TILESHIFT; // toYray

	if( trace->tile_vis )
	{
		// this tile is visible
		trace->tile_vis[ POS2TILE( trace->x ) ][ POS2TILE( trace->y ) ] = true; 
	}
	
//
// Start of ray-casting
//
	while( 1 )
	{
//
// Vertical loop // an anologue for X-Ray
//
		while( ! (ytilestep == -1 && YmapPos <= ytile) && ! (ytilestep == 1 && YmapPos >= ytile) )
		{
			if( xtile < 0 || xtile >= 64 || YmapPos < 0 || YmapPos >= 64 )
			{
				return;
			}

			if( R_TraceCheck( lvl, xtile, YmapPos, yintercept % TILEGLOBAL, ystep, true, (_boolean)(xtilestep == -1), trace ) )
			{
				return;
			}

		// prepare for next step
			xtile += xtilestep;
			yintercept += ystep;
			YmapPos = yintercept >> TILESHIFT;
		}

//
// Horizontal loop // an anologue for Y-Ray
//
		while( ! (xtilestep == -1 && XmapPos <= xtile) && ! (xtilestep == 1 && XmapPos >= xtile) )
		{
			if( ytile < 0 || ytile >= 64 || XmapPos < 0 || XmapPos >= 64 )
			{
				return;
			}

			if( R_TraceCheck( lvl, XmapPos, ytile, xintercept % TILEGLOBAL, xstep, false, (_boolean)(ytilestep == -1), trace ) )
			{
				return;
			}

		// prepare for next step
			ytile += ytilestep;
			xintercept += xstep;
			XmapPos = xintercept >> TILESHIFT;
		}

	} // end of while( 1 )

}
