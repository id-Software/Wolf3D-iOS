/*

	Copyright (C) 2004 Michael Liebscher
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
 *	math.c:	Math routines.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *	Date:	2004  
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"






////////////////////////////
//
//	Square Root
//
////////////////////////////

/*
-----------------------------------------------------------------------------
 Function: _sqrtf -Calculates the square root.

 Parameters: x -[in] Nonnegative floating-point value

 Returns: The square-root of x.

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC float _sqrtf( float x )
{
	return (float)sqrt( x );
}


/*
-----------------------------------------------------------------------------
 Function: _3DNow_Sqrt -Calculates the square root.

 Parameters: x -[in] Nonnegative floating-point value

 Returns: The square-root of x.

 Notes: 15-Bit Precision
-----------------------------------------------------------------------------
*/
#if __i386__

PUBLIC float _3DNow_Sqrt( float x )
{
	float	root = 0.f;

#if( _MSC_VER || __WATCOMC__ )

	__asm
	{
		femms
		movd		mm0, x
		pfrsqrt		mm1, mm0
		punpckldq	mm0, mm0
		pfmul		mm0, mm1
		movd		root, mm0
		femms
	}

#endif

	return root;
}

/*
-----------------------------------------------------------------------------
 Function: _SSE_Sqrt -Calculates the square root.

 Parameters: x -[in] Nonnegative floating-point value

 Returns: The square-root of x.

 Notes: 
-----------------------------------------------------------------------------
*/
float _SSE_Sqrt( float x )
{
	float	root = 0.f;

#if( _MSC_VER || __WATCOMC__ )

	__asm
	{
		sqrtss		xmm0, x
		movss		root, xmm0
	}


#endif

	return root;
}

#endif /* __i386__ */

////////////////////////////
//
//	End Square Root
//
////////////////////////////


float (*pfSqrt)( float x )  = _sqrtf;




/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int my_log2( int val )
{
	int answer = 0;
	while( ( val >>= 1 ) != 0 ) 
	{
		answer++;
	}
	return answer;
}

/*
-----------------------------------------------------------------------------
 Function: CalcFov	-Calculate the field of view.
 
 Parameters:fov_x -[in] Must be within 1 and 179 degrees.
			width -[in] Width of viewing area.
			height -[in] Height of viewing area.

 Returns: The field of view in degrees.
 
 Notes:

-----------------------------------------------------------------------------
*/
PUBLIC float CalcFov( float fov_x, float width, float height )
{
	if( fov_x < 1 || fov_x > 179 )
	{
		Com_Error( ERR_DROP, "Bad fov: %f", fov_x );
	}

	return (float)RAD2DEG( atan( height / ( width / tan( fov_x / 360 * M_PI ) ) ) ) * 2;
}

/*
-----------------------------------------------------------------------------
 Function: MathLib_Init	-Initialize optimized math routines.
 
 Parameters: Nothing.

 Returns: Nothing.
 
 Notes:

-----------------------------------------------------------------------------
*/
PUBLIC void MathLib_Init( void )
{
	Com_Printf( "Initializing Math Module\n" );
	
#if 0//__i386__

	if( main_cpu_s.b3DNow )
	{
//		pfSqrt = _3DNow_Sqrt;

		Com_Printf( "...using 3DNow!\n" );
	}
		
	if( main_cpu_s.bSSE )
	{
//		pfSqrt = _SSE_Sqrt;

		Com_Printf( "...using SSE\n" );
	}

#endif
}




