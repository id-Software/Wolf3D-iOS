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

#include "../wolfiphone.h"

vec3_t vec3_origin = { 0, 0, 0 };


vec_t _VectorNormalize( vec3_t v );

vec_t (*pfVectorNormalize)( vec3_t v ) = _VectorNormalize;


/*
-----------------------------------------------------------------------------
 Function: _VectorNormalize -Normalize a 3D vector.

 Parameters: v -[in] 3D vector to normalize.

 Returns: Unit vector value.

 Notes: 
	For a given vector, the process of finding a unit vector which is
	parallel to it. This is done by dividing the given vector by its 
	magnitude.
-----------------------------------------------------------------------------
*/
PUBLIC vec_t _VectorNormalize( vec3_t v )
{
	float length, ilength;

	length = (float)pfSqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );

	if( length )
	{
		ilength = 1 / length;
		v[ 0 ] *= ilength;
		v[ 1 ] *= ilength;
		v[ 2 ] *= ilength;
	}
		
	return length;
}

/*
-----------------------------------------------------------------------------
 Function: ProjectPointOnPlane -Project a point onto a plane.

 Parameters: dst -[out] Destination Point on Plane.
			 p -[in] Point to project onto the plane.
			 normal -[in] A vector to specify the orientation of the plane.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal )
{
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom = 1.0f / DotProduct( normal, normal );

	d = DotProduct( normal, p ) * inv_denom;

	n[ 0 ] = normal[ 0 ] * inv_denom;
	n[ 1 ] = normal[ 1 ] * inv_denom;
	n[ 2 ] = normal[ 2 ] * inv_denom;

	dst[ 0 ] = p[ 0 ] - d * n[ 0 ];
	dst[ 1 ] = p[ 1 ] - d * n[ 1 ];
	dst[ 2 ] = p[ 2 ] - d * n[ 2 ];
}

/*
-----------------------------------------------------------------------------
 Function: PerpendicularVector	-
 
 Parameters:dst -[out] Perpendicular Vector.
			src -[in] Normalized vector. 
			

 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void PerpendicularVector( vec3_t dst, const vec3_t src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	/* find the smallest magnitude axially aligned vector */
	for( pos = 0, i = 0 ; i < 3 ; ++i )
	{
		if( fabs( src[ i ] ) < minelem )
		{
			pos = i;
			minelem = (float)fabs( src[ i ] );
		}
	}
	tempvec[ 0 ] = tempvec[ 1 ] = tempvec[ 2 ] = 0.0F;
	tempvec[ pos ] = 1.0F;

	/* project the point onto the plane defined by src */
	ProjectPointOnPlane( dst, tempvec, src );

	/* normalize the result */
	pfVectorNormalize( dst );
}

/*
-----------------------------------------------------------------------------
 Function: RotatePointAroundVector -Rotate a point around a vector.

 Parameters: dst -[out] Point after rotation.
			 dir -[in] vector.
			 point -[in] Point.
			 degrees -[in] Degrees of rotation.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees ) 
{
	mat3_t	m;
	mat3_t	im;
	mat3_t	zrot;
	mat3_t	tmpmat;
	mat3_t	rot;
	vec3_t  vr, vup, vf;
	float	rad;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector( vr, dir );
	vectorCrossProduct( vr, vf, vup );

	m[0] = vr[0];
	m[3] = vr[1];
	m[6] = vr[2];

	m[1] = vup[0];
	m[4] = vup[1];
	m[7] = vup[2];

	m[2] = vf[0];
	m[5] = vf[1];
	m[8] = vf[2];

	memcpy( im, m, sizeof( im ) );

	im[1] = m[3];
	im[2] = m[6];
	im[3] = m[1];
	im[5] = m[7];
	im[6] = m[2];
	im[7] = m[5];

	memset( zrot, 0, sizeof( zrot ) );
	zrot[0] = zrot[4] = zrot[8] = 1.0F;

	rad = DEG2RAD( degrees );
	zrot[0] = (float)cos( rad );
	zrot[1] = (float)sin( rad );
	zrot[3] = (float)-sin( rad );
	zrot[4] = (float)cos( rad );

	Matrix3x3Multiply( m, zrot, tmpmat );
	Matrix3x3Multiply( tmpmat, im, rot );

	dst[0] = rot[0] * point[0] + rot[1] * point[1] + rot[2] * point[2];
	dst[1] = rot[3] * point[0] + rot[4] * point[1] + rot[5] * point[2];
	dst[2] = rot[6] * point[0] + rot[7] * point[1] + rot[8] * point[2];	
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC float RadiusFromBounds( const vec3_t mins, const vec3_t maxs )
{
	int		i;
	vec3_t	corner;
	float a, b;

	for( i = 0; i < 3; ++i ) 
	{
		a = (float)fabs( mins[i] );
		b = (float)fabs( maxs[i] );
		corner[i] = a > b ? a : b;
	}

	return vectorLength( corner );
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void AddPointToBounds( vec3_t v, vec3_t mins, vec3_t maxs )
{
	if ( v[0] < mins[0] ) 
	{
		mins[0] = v[0];
	}
	if ( v[0] > maxs[0]) 
	{
		maxs[0] = v[0];
	}

	if ( v[1] < mins[1] ) 
	{
		mins[1] = v[1];
	}
	if ( v[1] > maxs[1]) 
	{
		maxs[1] = v[1];
	}

	if ( v[2] < mins[2] ) 
	{
		mins[2] = v[2];
	}
	if ( v[2] > maxs[2]) 
	{
		maxs[2] = v[2];
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
PUBLIC void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up )
{
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * ( M_PI*2 / 360 );
	sy = (float)sin( angle );
	cy = (float)cos( angle );
	angle = angles[PITCH] * ( M_PI*2 / 360 );
	sp = (float)sin( angle );
	cp = (float)cos( angle );
	angle = angles[ROLL] * ( M_PI*2 / 360 );
	sr = (float)sin( angle );
	cr = (float)cos( angle );

	if( forward )
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}

	if( right )
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}

	if( up )
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

/*
-----------------------------------------------------------------------------
 Function: vectorCompare -Compares two vectors for equality.

 Parameters: v1, v2 -[in] 3d vectors to compare.

 Returns:	1 if they are equal, otherwise 0.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL int vectorCompare( const vec3_t v1, const vec3_t v2 )
{
	if( v1[ 0 ] != v2[ 0 ] || 
		v1[ 1 ] != v2[ 1 ] || 
		v1[ 2 ] != v2[ 2 ] )
	{
		return 0;
	}
			
	return 1;
}

/*
-----------------------------------------------------------------------------
 Function: vectorLength -Get the length of a vector.

 Parameters: v -[in] 3D vector to get the length of.

 Returns:	The length of the vector.

 Notes: 
		Since the square of length is a sum of squares, and squares 
        (of real numbers) are always positive, length is always positive.
        The only time the length of a 3D vector is zero is when the vector 
        is the zero vector.
-----------------------------------------------------------------------------
*/
INLINECALL vec_t vectorLength( const vec3_t v )
{
	return (vec_t)pfSqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ] );
}

/*
-----------------------------------------------------------------------------
 Function: CrossProduct -Calulates the cross product of two vectors.

 Parameters: v1, v2 -[in] 3D vectors.
		      cross -[out] The vector cross product.

 Returns:	Nothing

 Notes: 
		The vector cross product takes two vector operands to produce a 
        vector result. The result, like all geometric vectors, has two 
        properties: length and orientation.

	    To find a vector perpendicular to a particular plane, compute the 
        cross product of two vectors in that plane. But there are two 
        directions perpendicular to the plane. Which one does the cross 
        product give you? That is determined by the right hand rule.

	    The cross product of two vectors is perpendicular to both; the right
        hand rule picks the one out of two possible perpendicular directions.

	    Computing Cross Product from Column Matrices:
		   u × v  = ( uj vk - uk vj , uk vi - ui vk , ui vj - uj vi )T
-----------------------------------------------------------------------------
*/
PUBLIC void vectorCrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross )
{
	cross[ 0 ] = v1[ 1 ] * v2[ 2 ] - v1[ 2 ] * v2[ 1 ];		// X
	cross[ 1 ] = v1[ 2 ] * v2[ 0 ] - v1[ 0 ] * v2[ 2 ];		// Y
	cross[ 2 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];		// Z
}


#if defined(__i386__) && defined(_MSC_VER)

// Taken from an article written by Michael Abrash that originally appeared in
// Dr. Dobb's Journal.
PUBLIC void vectorCrossProduct_asm( const vec3_t v1, const vec3_t v2, vec3_t cross )
{
	__asm
	{
		mov     eax, cross
		mov     ecx, v2
		mov     edx, v1

		;optimized cross product; 22 cycles
		fld dword ptr   [ecx+4]     ;starts & ends on cycle 0
		fmul dword ptr  [edx+8]     ;starts on cycle 1       
		fld dword ptr   [ecx+8]     ;starts & ends on cycle 2
		fmul dword ptr  [edx+0]     ;starts on cycle 3       
		fld dword ptr   [ecx+0]     ;starts & ends on cycle 4
		fmul dword ptr  [edx+4]     ;starts on cycle 5       
		fld dword ptr   [ecx+8]     ;starts & ends on cycle 6
		fmul dword ptr  [edx+4]     ;starts on cycle 7       
		fld dword ptr   [ecx+0]     ;starts & ends on cycle 8
		fmul dword ptr  [edx+8]     ;starts on cycle 9         
		fld dword ptr   [ecx+4]     ;starts & ends on cycle 10 
		fmul dword ptr  [edx+0]     ;starts on cycle 11        
		fxch            st(2)       ;no cost                   
		fsubrp          st(5),st(0) ;starts on cycle 12        
		fsubrp          st(3),st(0) ;starts on cycle 13        
		fsubrp          st(1),st(0) ;starts on cycle 14        
		fxch            st(2)       ;no cost, stalls for cycle 15
		fstp dword ptr  [eax+0]     ;starts on cycle 16, ends on cycle 17  
		fstp dword ptr  [eax+4]     ;starts on cycle 18, ends on cycle 19
		fstp dword ptr  [eax+8]     ;starts on cycle 20, ends on cycle 21
	}
}

#endif /* __i386__ */



/*
-----------------------------------------------------------------------------
 Function: _DotProduct -Calculates the dot product.
 
 Parameters: v1, v2 -[in] 3D vectors to compute dot product.

 Returns:	the dot product

 Notes: 
		Dot product, which takes two vectors as operands and produces a real
        number as its output. Sometimes the dot product is called the inner 
        product or the scalar product.
		The dot product is:
			a · b = a1b1 + a2b2 + a3b3
-----------------------------------------------------------------------------
*/
PUBLIC vec_t _vectorDotProduct( const vec3_t v1, const vec3_t v2 )
{
	return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
}


#if defined(__i386__) && defined(_MSC_VER)

// Taken from an article written by Michael Abrash that originally appeared in
// Dr. Dobb's Journal.
PUBLIC vec_t _vectorDotProduct_asm( const vec3_t v1, const vec3_t v2 )
{
	float dotret;

	__asm
	{
		mov     eax, v2
		mov     ecx, v1
		
		;optimized dot product; 15 cycles
		fld dword ptr   [eax+0]     ;starts & ends on cycle 0
		fmul dword ptr  [ecx+0]     ;starts on cycle 1
		fld dword ptr   [eax+4]     ;starts & ends on cycle 2
		fmul dword ptr  [ecx+4]     ;starts on cycle 3
		fld dword ptr   [eax+8]     ;starts & ends on cycle 4
		fmul dword ptr  [ecx+8]     ;starts on cycle 5
		fxch            st(1)       ;no cost
		faddp           st(2),st(0) ;starts on cycle 6, stalls for cycles 7-8
		faddp           st(1),st(0) ;starts on cycle 9, stalls for cycles 10-12
		fstp dword ptr  [dotret]    ;starts on cycle 13, ends on cycle 14
	}

    return dotret;
}

#endif /* __i386__ */



/*
-----------------------------------------------------------------------------
 Function: _vectorSubtract -Vector Difference.

 Parameters: veca, vecb -[in] 3D vectors.
			 out -[out] The vector difference of vectors A and B.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL void _vectorSubtract( const vec3_t veca, const vec3_t vecb, vec3_t out )
{
	out[ 0 ] = veca[ 0 ] - vecb[ 0 ];
	out[ 1 ] = veca[ 1 ] - vecb[ 1 ];
	out[ 2 ] = veca[ 2 ] - vecb[ 2 ];
}

/*
-----------------------------------------------------------------------------
 Function: _vectorAdd -Vector addition.

 Parameters: veca, vecb -[in] 3D vectors.
			 out -[out] The vector sum of vectors A and B

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL void _vectorAdd( const vec3_t veca, const vec3_t vecb, vec3_t out )
{
	out[ 0 ] = veca[ 0 ] + vecb[ 0 ];
	out[ 1 ] = veca[ 1 ] + vecb[ 1 ];
	out[ 2 ] = veca[ 2 ] + vecb[ 2 ];
}

/*
-----------------------------------------------------------------------------
 Function: _vectorCopy -Copy a vector.

 Parameters: in -[in] Source vector.
			 out -[out] Destination vector.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL void _vectorCopy( const vec3_t in, vec3_t out )
{
	out[ 0 ] = in[ 0 ];
	out[ 1 ] = in[ 1 ];
	out[ 2 ] = in[ 2 ];
}

/*
-----------------------------------------------------------------------------
 Function: _vectorScale -Scale a vector.

 Parameters: in -[in] Source vector.
			 scale -[in] Scale vector.
			 out -[out] Destination vector.

 Returns: Nothing.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL void _vectorScale( const vec3_t in, const vec_t scale, vec3_t out )
{
	out[ 0 ] = in[ 0 ] * scale;
	out[ 1 ] = in[ 1 ] * scale;
	out[ 2 ] = in[ 2 ] * scale;
}

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL void _vectorMA( const vec3_t veca, float scale, const vec3_t vecb, vec3_t vecc )
{
	vecc[ 0 ] = veca[ 0 ] + scale * vecb[ 0 ];
	vecc[ 1 ] = veca[ 1 ] + scale * vecb[ 1 ];
	vecc[ 2 ] = veca[ 2 ] + scale * vecb[ 2 ];
}






/////////////////////////////////////////////////////////////////////
//
//	2D Vector routines
//
/////////////////////////////////////////////////////////////////////


/*
-----------------------------------------------------------------------------
 Function: vector2DCompare -Compares two vectors for equality.

 Parameters: v1, v2 -[in] 2d vectors to compare.

 Returns:	1 if they are equal, otherwise 0.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL W32 vector2DCompare( const vec2_t v1, const vec2_t v2 )
{
	if( v1[ 0 ] != v2[ 0 ] || v1[ 1 ] != v2[ 1 ] )
	{
		return 0;
	}
			
	return 1;
}

/*
-----------------------------------------------------------------------------
 Function: vector2DLength -Get the length of a vector.

 Parameters: v -[in] 2D vector to get the length of.

 Returns:	The length of the vector.

 Notes: 
-----------------------------------------------------------------------------
*/
INLINECALL vec_t vector2DLength( const vec2_t v )
{
	return (vec_t)pfSqrt( v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] );
}



