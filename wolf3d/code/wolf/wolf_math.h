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
 *	wolf_math.h:  Wolfenstein 3-D math routines.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>
 *
 *	Acknowledgement:
 *	This code was derived from NewWolf, and was originally
 *	written by DarkOne the Hacker. 
 * 
 *
 */

/*
	Notes:	
	This module is implemented by wolf_math.c

*/
#ifndef __WOLF_MATH_H__
#define __WOLF_MATH_H__


#define FLOATTILE  65536.0f


// Angle Direction Types & LUTs (Hard Coded! Please do not mess them)
typedef enum {q_first, q_second, q_third, q_fourth} quadrant;
typedef enum {dir4_east, dir4_north, dir4_west, dir4_south,	dir4_nodir}	dir4type;
typedef enum { dir8_east, dir8_northeast, dir8_north, dir8_northwest, dir8_west,
							dir8_southwest,	dir8_south,	dir8_southeast,	dir8_nodir} dir8type;

extern char dx4dir[5], dy4dir[5], dx8dir[9], dy8dir[9];
extern dir4type opposite4[5], dir4d[3][3];
extern dir8type opposite8[9], dir4to8[5], diagonal[9][9];
extern int dir8angle[9], dir4angle[5];
// ------------------------- * Vectors * -------------------------
// Vectors & angles for 3D-Space



typedef struct
{
	long origin[2];
	long angle;

} placeonplane_t;


// ------------------------- * Some Macroses * -------------------------


#define max_of_2(a, b) ((a)>(b)?(a):(b))



#define LABS(x) ((long)(x)>0?(x):-(x))


#define TILE2POS(a) (((a)<<TILESHIFT)+HALFTILE)
#define POS2TILE(a)  ((a)>>TILESHIFT)
#define POS2TILEf(a)  ((a)/FLOATTILE)
// ------------------------- * vvv FINE angles vvv * -------------------------
#define ASTEP			0.0078125f		// 1 FINE=x DEGREES
#define ASTEPRAD	0.000136354f	// 1 FINE=x RADIANS
#define ANG_1RAD	7333.8598			// 1 RADIAN=x FINES
#define ANG_0			0       //(int)((float)0/ASTEP)
#define ANG_1			128			//(int)((float)1/ASTEP)
#define ANG_6			768     //(int)((float)6/ASTEP)
#define ANG_15		1920		//(int)((float)15/ASTEP)
#define ANG_22_5	2880		//(int)((float)22.5/ASTEP)
#define ANG_30		3840    //(int)((float)30/ASTEP)
#define ANG_45		5760    //(int)((float)45/ASTEP)
#define ANG_67_5	8640		//(int)((float)67.5/ASTEP)
#define ANG_90		11520   //(int)((float)90/ASTEP)
#define ANG_112_5	14400		//(int)((float)112.5/ASTEP)
#define ANG_135		17280   //(int)((float)135/ASTEP)
#define ANG_157_5	20160		//(int)((float)157.5/ASTEP)
#define ANG_180		23040   //(int)((float)180/ASTEP)
#define ANG_202_5	25920		//(int)((float)202.5/ASTEP)
#define ANG_225		28800   //(int)((float)225/ASTEP)
#define ANG_247_5	31680		//(int)((float)247.5/ASTEP)
#define ANG_270		34560	  //(int)((float)270/ASTEP)
#define ANG_292_5	37440		//(int)((float)292.5/ASTEP)
#define ANG_315		40320	  //(int)((float)225/ASTEP)
#define ANG_337_5 43200   //(int)((float)337.5/ASTEP)
#define ANG_360		46080   //(int)((float)360/ASTEP)
// ------------------------- * ^^^ FINE angles ^^^ * -------------------------



#define FINE2RAD( a ) (((a) * M_PI ) / ANG_180)
#define RAD2FINE( a ) (((a) * ANG_180) / M_PI)
#define FINE2DEG( a ) ((float)(a) / ANG_1)	// !@# don't lose precision bits
#define FINE2DEGf( a ) ((a) / (float)ANG_1)
#define DEG2FINE( a ) ((a) * ANG_1)

extern double SinTable[], *CosTable, TanTable[ ANG_360 + 1 ];
extern int XnextTable[ ANG_360 + 1], YnextTable[ ANG_360 + 1 ];
extern int ColumnAngle[640]; // 

extern int G_Build_Tables(void);

#define TanDgr( x )		(tan( DEG2RAD( x ) ))
#define SinDgr( x )		(sin( DEG2RAD( x ) ))
#define CosDgr( x )		(cos( DEG2RAD( x ) ))

#define ArcTanDgr( x )	(RAD2DEG( atan( x ) ))
#define ArcSinDgr( x )	(RAD2DEG( asin( x ) ))
#define ArcCosDgr( x )	(RAD2DEG( acos( x ) ))



extern int NormalizeAngle( int angle );
extern int Point2LineDist( int x, int y, int a );
extern int LineLen2Point( int x, int y, int a );

extern quadrant GetQuadrant( float angle );
extern dir4type Get4dir( float angle );
extern dir8type Get8dir( float angle );

extern float TransformPoint( double Point1X, double Point1Y, double Point2X, double Point2Y );





#endif /* __WOLF_MATH_H__ */
