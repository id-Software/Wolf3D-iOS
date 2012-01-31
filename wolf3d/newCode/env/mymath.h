/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	mymath.h: Math routines.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#ifndef __MYMATH_H__
#define __MYMATH_H__



typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;


#define	PITCH	0	/* up / down */
#define	YAW		1	/* left / right */
#define	ROLL	2	/* fall over */


#ifndef M_PI

	#define M_PI	3.14159265358979323846f	// matches value in gcc v2 math.h

#endif



#ifndef ABS

	#define ABS( x )	( (x) < 0 ? -(x) : (x) )

#endif



#define	nanmask ( 255 << 23 )

#define	IS_NAN( x ) ( ( (*(int *) &x ) & nanmask ) == nanmask )



/* Use RINT() instead of rint() */
#ifdef __GNUC__

	#define RINT( x ) rint( x )

#else

	#define RINT( x ) floor( (x) + 0.5 )

#endif




extern void MathLib_Init( void );

extern int my_log2( int val );

extern float (*pfSqrt)( float x );

extern float CalcFov( float fov_x, float width, float height );



#endif /* __MYMATH_H__ */

