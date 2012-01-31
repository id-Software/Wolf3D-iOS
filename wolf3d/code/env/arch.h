/*

	Copyright (C) 2004 Michael Liebscher <johnnycanuck@users.sourceforge.net>
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
 *	arch.h:	System dependant #defines and macros.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	 
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#ifndef __ARCH_H__
#define __ARCH_H__



// Define BUILDSTRING and CPUSTRING based on platform
#ifdef _WIN32

        #ifdef _DEBUG

                #define BUILDSTRING "Win32 DEBUG"

        #else

                #define BUILDSTRING "Win32 RELEASE"

        #endif


        #ifdef _M_IX86

                #define CPUSTRING       "x86"

        #elif defined _M_ALPHA

                #define CPUSTRING       "AXP"
                
        #else
        
                #define CPUSTRING       "Unknown CPU"

        #endif


#elif defined __linux__

        #define BUILDSTRING "Linux"

        #ifdef __i386__

                #define CPUSTRING "i386"

        #elif defined __alpha__

                #define CPUSTRING "AXP"

        #else

                #define CPUSTRING "Unknown CPU"

        #endif


#elif defined __FreeBSD__
        
        #define BUILDSTRING "FreeBSD"
        
        #ifdef __i386__

                #define CPUSTRING "i386"

        #else

                #define CPUSTRING "Unknown CPU"

        #endif


#elif defined __sun__

        #define BUILDSTRING "Solaris"

        #ifdef __i386__

                #define CPUSTRING "i386"

        #else

                #define CPUSTRING "sparc"

        #endif


#elif defined MACOS

        #define BUILDSTRING "MAC"

        #ifdef __powerpc__

                #define CPUSTRING "PowerPC"

        #else

                #define CPUSTRING "Unknown CPU"

        #endif


#else

        #define BUILDSTRING "Unknown OS"
        #define CPUSTRING "Unknown CPU"

#endif /* if WIN32 else __linux__ else __FreeBSD__ else __sun__ else MACOS */





/*
	correct numeric types:  W8, SW8, W16, SW16, W32, SW32, W64, SW64
	correct misc types:	void, float, _boolean
 
	s       -signed
	XX      -Number of bits
 
*/
#if( __GNUC__ || __WATCOMC__ || _MSC_VER )
        
        
        typedef unsigned char   W8,		*PW8;
        typedef signed char		SW8,    *PSW8;
        typedef unsigned short  W16,    *PW16;
        typedef signed short    SW16,   *PSW16;
        typedef unsigned long   W32,    *PW32;
        typedef signed long		SW32,   *PSW32;
        
        

        #if( __GNUC__ )

                typedef unsigned long long	W64,    *PW64;
                typedef long long			SW64,   *PSW64;

        #elif( _MSC_VER || __WATCOMC__ )

                typedef unsigned __int64    W64,	*PW64;
                typedef __int64				SW64,   *PSW64;

        #else

                #error "please define W64"

        #endif

#else

        #error "Unknown compiler, please define basic types"

#endif


/* Define NULL pointer value */
#ifndef NULL

        #ifdef __cplusplus
                #define NULL    0
        #else
                #define NULL    ((void *)0)
        #endif

#endif /* NULL */



/* Define INLINECALL keyword */
#ifndef INLINECALL

	#if defined(__cplusplus) || defined(__GNUC__) 

		#define INLINECALL inline	

	#elif defined(_WIN32) && !defined(__WATCOMC__)

		#define INLINECALL __inline	

	#else

		#define INLINECALL	/* Not supported */

	#endif

#endif /* INLINECALL */




typedef W8	colour3_t[ 3 ]; // RGB
typedef W8	colour4_t[ 4 ]; // RGBA
typedef W32 COLOURVAL, *PCOLOURVAL;  // Represents a 32-bit colour value.

#ifdef _WIN32

    #define vsnprintf _vsnprintf

#endif 


typedef W8 _boolean; 

#define false 0
#define true 1

//enum {  false = 0, 
//        true = 1 };




#define ShortSwap( x )  ( ( (((W16) (x)) & 0x00FF) << 8 ) | ( (((W16) (x))& 0xFF00) >> 8) )
#define LongSwap( x )   ( ( ((W32) (x)) & 0xFF000000) >> 24 ) | ( ((( (W32) (x) ) & 0xFF0000) >> 8)  ) | ( ((( (W32) (x) ) & 0xFF00) << 8 ) ) | ( (( (W32) (x) ) & 0xFF) << 24 )



#if defined( IPHONE) || defined(__i386__) || defined(_M_IX86)  // Little endian        

        #define BigShort( x )           ShortSwap( x )
        #define LittleShort( x )        ( x )

        #define BigLong( x )            LongSwap( x )
        #define LittleLong( x )         ( x )

        #define BigFloat( x )           FloatSwap( x )
        #define LittleFloat( x )        ( x )


#else // Big endian

        #define BigShort( x )           ( x )
        #define LittleShort( x )        ShortSwap( x )

        #define BigLong( x )            ( x )
        #define LittleLong( x )         LongSwap( x )

        #define BigFloat( x )           ( x )
        #define LittleFloat( x )        FloatSwap( x )

#endif


#endif /* __ARCH_H__ */
