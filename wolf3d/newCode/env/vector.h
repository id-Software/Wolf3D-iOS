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
 *	vector.h: 2D and 3D vector math routines.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef float vec_t;
typedef vec_t vec2_t[ 2 ];
typedef vec_t vec3_t[ 3 ];
typedef vec_t vec4_t[ 4 ];
typedef vec_t vec5_t[ 5 ];



extern vec3_t vec3_origin;


/////////////////////////////////////////////////////////////////////
//
//	3D Vector routines
//
/////////////////////////////////////////////////////////////////////

#define vectorClear( a )		( (a)[ 0 ] = (a)[ 1 ] = (a)[ 2 ] = 0 )
#define vectorNegate( a, b )	( (b)[ 0 ] = (-a)[ 0 ], (b)[ 1 ] = (-a)[ 1 ], (b)[ 2 ] = (-a)[ 2 ] )
#define vectorSet( v, x, y, z )	( (v)[ 0 ] = ( x ), (v)[ 1 ] = ( y ), (v)[ 2 ] = ( z ) )
#define vectorInverse( a )		( (a)[ 0 ] = (-a)[ 0 ], (a)[ 1 ] = (-a)[ 1 ], (a)[ 2 ] = (-a)[ 2 ] )

#if	1

	#define DotProduct( x, y )			( (x)[ 0 ] * (y)[ 0 ] + (x)[ 1 ] * (y)[ 1 ] + (x)[ 2 ] * (y)[ 2 ] )
	#define vectorSubtract( a, b, c )	( (c)[ 0 ] = (a)[ 0 ] - (b)[ 0 ], (c)[ 1 ] = (a)[ 1 ] - (b)[ 1 ], (c)[ 2 ] = (a)[ 2 ] - (b)[ 2 ] )
	#define vectorAdd( a, b, c )		( (c)[ 0 ] = (a)[ 0 ] + (b)[ 0 ], (c)[ 1 ] = (a)[ 1 ] + (b)[ 1 ], (c)[ 2 ] = (a)[ 2 ] + (b)[ 2 ] )
	#define vectorCopy( a, b )			( (b)[ 0 ] = (a)[ 0 ], (b)[ 1 ] = (a)[ 1 ], (b)[ 2 ] = (a)[ 2 ] )
	#define	vectorScale( v, s, o )		( (o)[ 0 ] = (v)[ 0 ] * (s),(o)[ 1 ] = (v)[ 1 ] * (s), (o)[ 2 ] = (v)[ 2 ] * (s) )
	#define	vectorMA( v, s, b, o )		( (o)[ 0 ] = (v)[ 0 ] + (b)[ 0 ]*(s),(o)[ 1 ] = (v)[ 1 ] + (b)[ 1 ] * (s),(o)[ 2 ] = (v)[ 2 ] + (b)[ 2 ] * (s) )

#else

	/* just in case you don't want to use the macros */
	#define DotProduct( x, y )			_vectorDotProduct( x, y )
	#define vectorSubtract( a, b, c )	_vectorSubtract( a, b, c )
	#define vectorAdd( a, b, c )		_vectorAdd( a, b, c )
	#define vectorCopy( a, b )			_vectorCopy( a, b )
	#define	vectorScale( v, s, o )		_vectorScale( v, s, o )
	#define	vectorMA( v, s, b, o )		_vectorMA( v, s, b, o )

#endif


extern void vectorCrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross );


extern int vectorCompare( const vec3_t v1, const vec3_t v2 );
extern vec_t vectorLength( const vec3_t v );
extern vec_t (*pfVectorNormalize)( vec3_t vec ); 

extern void angleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up );


extern void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees );
extern void PerpendicularVector( vec3_t dst, const vec3_t src );
extern void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal );


extern void AddPointToBounds( vec3_t v, vec3_t mins, vec3_t maxs );
extern float RadiusFromBounds( const vec3_t mins, const vec3_t maxs );



/////////////////////////////////////////////////////////////////////
//
//	2D Vector routines
//
/////////////////////////////////////////////////////////////////////

#define vector2DClear( a )		( (a)[ 0 ] = (a)[ 1 ] = 0 )
#define vector2DNegate( a, b )	( (b)[ 0 ] = (-a)[ 0 ], (b)[ 1 ] = (-a)[ 1 ] )
#define vector2DSet( v, x, y )	( (v)[ 0 ] = ( x ), (v)[ 1 ] = ( y ) )
#define vector2DInverse( a )	( (a)[ 0 ] = (-a)[ 0 ], (a)[ 1 ] = (-a)[ 1 ] )

#define vector2DPerpDot( a, b )		( (a)[ 0 ] * (b)[ 1 ] - (a)[ 1 ] * (b)[ 0 ] )
#define vector2DDotProduct( x, y )	( (x)[ 0 ] * (y)[ 0 ] + (x)[ 1 ] * (y)[ 1 ] )
#define vector2DSubtract( a, b, c )	( (c)[ 0 ] = (a)[ 0 ] - (b)[ 0 ], (c)[ 1 ] = (a)[ 1 ] - (b)[ 1 ] )
#define vector2DAdd( a, b, c )		( (c)[ 0 ] = (a)[ 0 ] + (b)[ 0 ], (c)[ 1 ] = (a)[ 1 ] + (b)[ 1 ] )
#define vector2DCopy( a, b )		( (b)[ 0 ] = (a)[ 0 ], (b)[ 1 ] = (a)[ 1 ] )
#define	vector2DScale( v, s, o )	( (o)[ 0 ] = (v)[ 0 ] * (s), (o)[ 1 ] = (v)[ 1 ] * (s) )
#define	vector2DMA( v, s, b, o )	( (o)[ 0 ] = (v)[ 0 ] + (b)[ 0 ]*(s), (o)[ 1 ] = (v)[ 1 ] + (b)[ 1 ] * (s) )


extern W32 vector2DCompare( const vec2_t v1, const vec2_t v2 );
extern vec_t vector2DLength( const vec2_t v );


#endif /* __VECTOR_H__ */

