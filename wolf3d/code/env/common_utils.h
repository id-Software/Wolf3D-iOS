/*

	Copyright (C) 2004 Michael Liebscher <johnnycanuck@users.sourceforge.net>

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
 *	common_utils.h:   General Purpose Macros.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net> 
 *
 */

#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__



/* 
	Make sure the following macros are defined.
*/

#ifndef __FILE__

	#define __FILE__	""

#endif

#ifndef __LINE__

	#define __LINE__	0L

#endif



/*
	These function prefixes are used for figuring out which 
	functions are exported and which are not.
*/

#define PUBLIC			/* Accessible outside this module     */
#define PRIVATE static	/* Accessible only within this module */



#endif /* __COMMON_UTILS_H__ */

