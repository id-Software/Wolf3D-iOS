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

#ifndef __ANGLE_H__
#define __ANGLE_H__


#define DEG2RAD( a ) ( (a) * 0.01745329251994329576f ) // a * M_PI / 180.0f
#define RAD2DEG( a ) ( (int)((a) / 0.01745329251994329576f) ) // a * 180.0f / M_PI 

#define	ANGLE2SHORT( x )	( (int)((x) * 65536 / 360) & 65535 )
#define	SHORT2ANGLE( x )	( (x) * (360.0 / 65536) )



extern float angle_diff( float angle1, float angle2 );
extern float angle_wise( float angle1, float angle2 );
extern float interpolate_angle( float from, float to, float fraction );
extern float normalize_angle( float angle );
extern float LerpAngle( float from, float to, float frac );



#endif /* __ANGLE_H__ */
