/*

	Copyright (C) 2004 Michael Liebscher

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
 *	Date:	2004   
 *
 *	Acknowledgement:
 *	This code was derived from Wolfenstein3-D, and was originally
 *	written by Id Software, Inc.
 *
 */

#ifndef __RANDOM_NUMBER_H__
#define __RANDOM_NUMBER_H__


#define random()	( (rand() & 0x7fff) / ((float)0x7fff) )
#define crandom()	( 2.0 * (random() - 0.5) )


extern void US_InitRndT( int randomize );
extern int US_RndT( void );





#endif /* __RANDOM_NUMBER_H__ */
