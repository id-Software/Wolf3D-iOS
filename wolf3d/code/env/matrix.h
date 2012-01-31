/*

	Copyright (C) 2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>

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
 *	matrix.h: Matrix math routines.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"

typedef float mat3_t[ 9 ];	// 3x3 matrix
typedef float mat4_t[ 16 ];	// 4x4 matrix

extern void Matrix3x3Multiply( mat3_t in1, mat3_t in2, mat3_t out ) ;
extern void MatrixIdentity( mat4_t matrix );
extern void MatrixInvert( mat4_t in, mat4_t out );
extern void VectorMatrixMultiply( vec3_t vecIn, mat4_t m, vec3_t vecOut );
extern void VectorMatrix3x3Multiply( vec3_t vecIn, mat4_t m, vec3_t vecOut );


#endif /* __MATRIX_H__ */
