/*

	Copyright (C) 2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	angle.h: Angle math routines.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"

/*
-----------------------------------------------------------------------------
 Function: angle_diff -Finds the difference between two angles.

 Parameters: angle1, angle2 -[in] Angles in Radians.

 Returns: 
		Returns the absolute difference between two angles, this will always
		be between 0 and 180 degrees.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL float angle_diff( float angle1, float angle2 )
{
	float d;

	if( angle1 > angle2 )
	{
		d = angle1 - angle2;
	}
	else
	{
		d = angle2 - angle1;
	}

	if( d > M_PI )
	{
		return 2 * M_PI - d;
	}
	else
	{
		return d;
	}
}

/*
-----------------------------------------------------------------------------
 Function: angle_wise -Clockwise distance between two angles.

 Parameters: angle1, angle2 -[in] Angles in Radians.

 Returns: 
		Returns the clockwise distance from angle2 to angle1, this may be 
		greater than 180 degrees.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL float angle_wise( float angle1, float angle2 )
{
	if( angle1 > angle2 )
	{
		return angle1 - angle2;
	}
	else
	{
		return angle1 + 2 * M_PI - angle2;
	}
}

/*
-----------------------------------------------------------------------------
 Function: interpolate_angle -Linear interpolate between angle A and B by
								fraction 'f'.

 Parameters: 

 Returns: 

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL float interpolate_angle( float from, float to, float fraction )
{
	float diff = angle_diff( from, to ) * fraction;

	if( angle_wise( to, from ) >= M_PI )
	{
		return from - diff;
	}
	else
	{
		return from + diff;
	}
}

/*
-----------------------------------------------------------------------------
 Function: normalize_angle -

 Parameters: 

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL float normalize_angle( float angle )
{
	while( angle < 0 )
	{
		angle += (2 * M_PI);
	}

	while( angle >= (2 * M_PI) )
	{
		angle -= (2 * M_PI);
	}

	return angle;
}

/*
-----------------------------------------------------------------------------
 Function: LerpAngle -Linear interpolate allowing for the Modulo 360 problem.

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL float LerpAngle( float from, float to, float frac )
{
	if( to - from > 180 ) 
	{
		to -= 360;
	}

	if( to - from < -180 ) 
	{
		to += 360;
	}
	
	return from + frac * (to - from);
}
