/*

	Copyright (C) 2004 Michael Liebscher
	Copyright (C) 2000 by DarkOne the Hacker

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
 *	wolf_math.c:  Wolfenstein 3-D math routines.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *	Date:	2004 
 *
 *	Acknowledgement:
 *	This code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 * 
 *
 */

#include "../wolfiphone.h"

#define XRES			640
#define YRES			480

// ------------------------- * LUTs * -------------------------
double	SinTable[ ANG_360 + ANG_90 + 1 ], 
		*CosTable = SinTable + ANG_90,
		TanTable[ ANG_360 + 1 ];

int XnextTable[ ANG_360 + 1 ],
	YnextTable[ ANG_360 + 1 ];

int ColumnAngle[ 640 ]; // ViewAngle=PlayerAngle+ColumnAngle[curcolumn]; /in fines/

char dx4dir[5]={1, 0, -1,  0, 0};  // dx & dy based on direction
char dy4dir[5]={0, 1,  0, -1, 0};
char dx8dir[9]={1, 1, 0, -1, -1, -1,  0,  1, 0};  // dx & dy based on direction
char dy8dir[9]={0, 1, 1,  1,  0, -1, -1, -1, 0};
dir4type opposite4[5]={2, 3, 0, 1, 4};
dir8type opposite8[9]={4, 5, 6, 7, 0, 1, 2, 3, 8};
dir8type dir4to8[5]={0, 2, 4, 6, 8};
dir8type diagonal[9][9]=
{
/* east */	{dir8_nodir, dir8_nodir, dir8_northeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southeast, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* north */ {dir8_northeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_northwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* west */  {dir8_nodir, dir8_nodir, dir8_northwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southwest, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* south */ {dir8_southeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir}
};
// dir of delta tooks dx{-1|0|1}+1 & dy{-1|0|1}+1 and give direction
dir4type dir4d[3][3]={{dir4_nodir, dir4_west , dir4_nodir},
											{dir4_south, dir4_nodir, dir4_north},
											{dir4_nodir, dir4_east , dir4_nodir}};
int dir8angle[9]={ANG_0, ANG_45, ANG_90, ANG_135, ANG_180, ANG_225, ANG_270, ANG_315, ANG_0};
int dir4angle[5]={ANG_0, ANG_90, ANG_180, ANG_270, ANG_0};

/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int G_Build_Tables( void )
{
	double angle, tanfov2, tanval, value;
	int n;

	for( n = 0 ; n <= ANG_90 ; ++n )
	{
		angle = FINE2RAD( n );

		value = sin( angle );
		SinTable[ n ] = SinTable[ ANG_180 - n ] = SinTable[ n + ANG_360 ] = value;
		SinTable[ ANG_180 + n ] = SinTable[ ANG_360 - n ] = -value;
	}

	for( n = 0 ; n <= ANG_360 ; ++n )
	{
		angle = FINE2RAD( n ); //angle is in radians, n is in FINEs

		if( n == ANG_90 || n == ANG_270 )
		{
			TanTable[ n ] = tan( FINE2RAD( n - 0.5 ) );	// infinity
			YnextTable[ n ] = (int)(FLOATTILE * tan( FINE2RAD( n - 0.5 ) )); // infinity
		}
		else
		{
			TanTable[ n ] = tan( angle );
			YnextTable[ n ] = (int)(FLOATTILE * tan( angle ));
		}

		if( n == ANG_0 || n == ANG_360 )
			XnextTable[ n ] = (int)(FLOATTILE / tan( FINE2RAD( n + 0.5 ) )); // infinity
		else if( n == ANG_180 )
			XnextTable[ n ] = (int)(FLOATTILE / tan(FINE2RAD( n - 0.5 ) )); // -infinity
		else if( n == ANG_90 || n == ANG_270 )
			XnextTable[ n ] = 0;
		else
			XnextTable[ n ] = (int)(FLOATTILE / tan( angle ));
	}

	tanfov2 = TanDgr( CalcFov( 75, XRES, YRES) / 2.0 ) * ((float)XRES / (float)YRES );
	for( n = 0 ; n < XRES ; ++n )
	{
		tanval = tanfov2 * (-1.0 + 2.0 * (double)n / (double)(XRES-1) );
		ColumnAngle[ n ] = (int)RAD2FINE( atan( tanval ) );
	}

	US_InitRndT( 1 ); // random number generators


	return 1;
}



/*
-----------------------------------------------------------------------------
 Function: NormalizeAngle -clips angle to [0..360] bounds.

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int NormalizeAngle( int alpha )
{
	if( alpha > ANG_360 ) 
		alpha %= ANG_360;
	
	if(alpha<ANG_0)
		alpha = ANG_360 - (-alpha) % ANG_360;

	return alpha;
}


/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC quadrant GetQuadrant( float angle )
{
	angle = normalize_angle( angle );

	if( angle < M_PI / 2 ) 
	{
		return q_first;
	}
	else if( angle < M_PI ) 
	{
		return q_second;
	}
	else if( angle < 3 * M_PI / 2 )  
	{
		return q_third;
	}
	else 
	{
		return q_fourth;
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
PUBLIC dir4type Get4dir( float angle )
{
	angle = normalize_angle( angle + M_PI / 4 );

	if( angle < M_PI / 2 ) 
	{
		return dir4_east;
	}
	else if( angle < M_PI ) 
	{
		return dir4_north;
	}
	else if( angle < 3 * M_PI / 2 ) 
	{
		return dir4_west;
	}
	else 
	{
		return dir4_south;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Get8dir -Get 8 point direction.

 Parameters: angle -[in] Radian angle.

 Returns: Directional point.

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC dir8type Get8dir( float angle )
{
	angle = normalize_angle( angle + M_PI / 12 );

	if( angle <= (M_PI / 4) )
	{
		return dir8_east;
	}
	else if( angle < (M_PI / 2) ) 
	{
		return dir8_northeast;
	}
	else if( angle <= (3 * M_PI / 4) )
	{
		return dir8_north;
	}
	else if( angle < M_PI ) 
	{
		return dir8_northwest;
	}
	else if( angle <= (5 * M_PI / 4) )	
	{
		return dir8_west;
	}
	else if( angle < (3 * M_PI / 2) )
	{
		return dir8_southwest;
	}
	else if( angle <= (7 * M_PI / 4) )
	{
		return dir8_south;
	}
	else
	{
		return dir8_southeast;
	}
}

/*
-----------------------------------------------------------------------------
 Function: Point2LineDist -calculates distance between a point (x, y) and
							a line.

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int Point2LineDist( int x, int y, int a )
{
	return ABS( (int)(x * SinTable[ a ] - y * CosTable[ a ]) );
}


/*
-----------------------------------------------------------------------------
 Function: LineLen2Point -Calculates line length to the point nearest to (poin)

 Parameters:

 Returns:

 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int LineLen2Point( int x, int y, int a )
{
	return (int)(x * CosTable[ a ] + y * SinTable[ a ] );
}


/*
-----------------------------------------------------------------------------
 Function: 

 Parameters:

 Returns:

 Notes: 

  Function returns angle in radians
            point2 = {x,y}
			  / |
		    /   |	
 		  /     |
		/a______|----------> x
	point1 = {x, y}
-----------------------------------------------------------------------------
*/
PUBLIC float TransformPoint( double Point1X, double Point1Y, double Point2X, double Point2Y )
{
	float angle;

	angle = atan2( Point1Y - Point2Y, Point1X - Point2X );
	
	return normalize_angle( angle );
}

