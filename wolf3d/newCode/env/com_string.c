/*

	Copyright (C) 2004-2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>
	Copyright (C) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
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
 *	com_string.c: Common string functions done in a portable manner.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>
 *
 *	Acknowledgement:
 *	Portion of this code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 *  Portion of this code was derived from code that was originally
 *  written by Todd C. Miller.
 *
 */
 
#include "../wolfiphone.h"





/*
-----------------------------------------------------------------------------
 Function: my_strlcpy -Copies a specified number of characters from a 
						source string into a buffer. 

 Parameters: dest -[in/out] Pointer to a buffer into which the function 
						copies characters
			 source -[in] Pointer to a NUL-terminated string from which 
						the function copies characters. 
			 nMaxLength -[in] Specifies the number of bytes to be copied 
						from the string pointed to by source into the
						buffer pointed to by dest.
 
 Returns: Returns strlen( source );	if retval >= nMaxLength, truncation 
			occurred.
 
 Notes: 
		At most nMaxLength-1 characters will be copied. Always NUL- 
		terminates (unless nMaxLength == 0). 
-----------------------------------------------------------------------------
*/
PUBLIC size_t my_strlcpy( char *dest, const char *source, size_t nMaxLength )
{
	char *d = dest;
	const char *s = source;
	size_t n = nMaxLength;

	/* Copy as many bytes as will fit */
	if( n != 0 && --n != 0 )
	{
		do
		{
			if( (*d++ = *s++) == 0 )
			{
				break;
			}

		} while( --n != 0 );
	}

	/* Not enough room in dest, add NUL and traverse rest of source */
	if( n == 0 ) 
	{
		if( nMaxLength != 0 )
		{
			*d = '\0';		/* NUL-terminate dest */
		}

		while( *s++ )
		{
			;
		}
	}

	return( s - source - 1 );	/* count does not include NUL */
}

/*
-----------------------------------------------------------------------------
 Function: my_strlcat -Appends one string to another. 

 Parameters: dest -[in/out] Pointer to a NUL-terminated string. The buffer 
					must be large enough to contain both strings or else
					truncation will occur.
			 source -[in] Pointer to a NUL-terminated string from which 
						the function copies characters. 
			 nMaxLength -[in] full size of dest, not space left.
 
 Returns: Returns strlen( source ) + MIN( nMaxLength, strlen( initial dest ) ).
		  If retval >= nMaxLength, truncation occurred.
 
 Notes: 
		At most nMaxLength-1 characters will be copied. Always NUL- 
		terminates (unless nMaxLength <= strlen( dest ) ).		  
-----------------------------------------------------------------------------
*/
PUBLIC size_t my_strlcat( char *dest, const char *source, size_t nMaxLength )
{
	char *d = dest;
	const char *s = source;
	size_t n = nMaxLength;
	size_t dlen;

	/* Find the end of dest and adjust bytes left but don't go past end */
	while( n-- != 0 && *d != '\0' )
	{
		d++;
	}

	dlen = d - dest;
	n = nMaxLength - dlen;

	/* No room left to append string */
	if( n == 0 )
	{
		return( dlen + strlen( s ) );
	}

	while( *s != '\0' )
	{
		if( n != 1 ) 
		{
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';	/* NUL-terminate string */

	return( dlen + (s - source) );	/* count does not include NUL */
	
}


/*
-----------------------------------------------------------------------------
 Function: my_strnicmp -Compare characters of two strings without regard to case.

 Parameters: string1, string2 -[in] NUL-terminated strings to compare. 
			 count -[in] Number of characters to compare.
 
 Returns:	 0 string1 identical to string2, -1 otherwise.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC int my_strnicmp( const char *string1, const char *string2, size_t count )
{
	char c1, c2;

	if( ! string1 || ! *string1 || 
	    ! string2 || ! *string2 ) 
	{ 
		return -1; 
	}
	
	do
	{
		c1 = *string1++;
		c2 = *string2++;

		if( ! count-- )
		{
			return 0;		/* strings are equal until end point */
		}
		
		if( c1 != c2 )
		{		
			if( TOUPPER( c1 ) != TOUPPER( c2 ) ) /* Uppercase compare */
			{
				return -1;	/* strings are not equal */
			}
		}

	} while( c1 );
	
	return 0;		/* strings are equal */
}

/*
-----------------------------------------------------------------------------
 Function: my_stricmp -Perform an uppercase comparison of strings.
 
 Parameters: string1, string2 -[in] NUL-terminated strings to compare.
 
 Returns:	0 string1 identical to string2, -1 otherwise. 
 
 Notes:	
		Calls my_strnicmp, where count is 99999 
-----------------------------------------------------------------------------
*/
PUBLIC int my_stricmp( const char *string1, const char *string2 )
{
	return my_strnicmp( string1, string2, 99999 );
}

/*
-----------------------------------------------------------------------------
 Function: my_snprintf -Write formatted data to a string.
 
 Parameters: dest -[out] Storage location for output.
			 size -[in] Maximum number of characters to store.
			 format -[in] Format-control string.
			 ... -[in] Optional arguments.
 
 Returns:	Nothing. 
 
 Notes: If the format string is longer than 32768 truncation will occur.
		Also, if format is longer than dest truncation will occur.
-----------------------------------------------------------------------------
*/
PUBLIC void my_snprintf( char *dest, size_t size, const char *format, ... )
{
	va_list	argptr;
	char	bigbuffer[ 0x8000 ];

	va_start( argptr, format );
	(void)vsnprintf( bigbuffer, sizeof( bigbuffer ), format, argptr );
	va_end( argptr );
	
	bigbuffer[ sizeof( bigbuffer ) - 1 ] = '\0';
	
	my_strlcpy( dest, bigbuffer, size );
}


/*
-----------------------------------------------------------------------------
 Function: my_CopyString -Allocate a duplicate copy of a string, and return
							duplicate.
 
 Parameters: in -[in] String to duplicate.
			 
 
 Returns:	
		Pointer to duplicate string. Caller is responsible for freeing 
		memory with Z_Free. 
 
 Notes: 		  
-----------------------------------------------------------------------------
*/
PUBLIC char *my_CopyString( const char *in )
{
	char *out;
	
	out = Z_Malloc( strlen( in ) + 1 );
	my_strlcpy( out, in, strlen( in ) + 1 );
	
	return out;
}

/*
-----------------------------------------------------------------------------
 Function: my_strhash -Create a hash id from string.
 
 Parameters: string -[in] NUL-terminated string.
			 
 
 Returns:	Hash id. 
 
 Notes: 		  
-----------------------------------------------------------------------------
*/
PUBLIC W32 my_strhash( const char *string )
{
	W32 hash = *string;
                                                                                
	if( hash )
	{
		for( string += 1; *string != '\0'; ++string )
		{
			hash = (hash << 5) - hash + *string;
		}
	}
                                                                                
	return hash;
}


/*
-----------------------------------------------------------------------------
 Function: my_strupr -Convert a string to uppercase.

 Parameters: string -[in/out] NUL-terminated string to capitalize.
 
 Returns: 
		This functions returns a pointer to the converted string. Because 
		the modification is done in place, the pointer returned is the same
		as the pointer passed as the input argument. No return value is 
		reserved to indicate an error.

 Notes: 
	  
-----------------------------------------------------------------------------
*/
PUBLIC char *my_strupr( char *string )
{
	char *ptr;

	if( ! string || ! *string )
	{
		return string;
	}

	ptr = string;

	do
	{
		*ptr = TOUPPER( *ptr );		

	} while( *ptr++ );


	return string;
}

/*
-----------------------------------------------------------------------------
 Function: my_strlwr -Convert a string to lowercase. 

 Parameters: string -[in/out] NUL-terminated string to convert to lowercase.
 
 Returns: 
		This functions returns a pointer to the converted string. Because 
		the modification is done in place, the pointer returned is the same
		as the pointer passed as the input argument. No return value is 
		reserved to indicate an error.

 Notes: 
	  
-----------------------------------------------------------------------------
*/
PUBLIC char *my_strlwr( char *string )
{
	char *ptr;

	if( ! string || ! *string )
	{
		return string; 
	}

	ptr = string;

	do
	{
		*ptr = TOLOWER( *ptr );		

	} while( *ptr++ );


	return string;
}

/*
-----------------------------------------------------------------------------
 Function: StringToInteger -Convert string to integer.
 
 Parameters: string -[in] NUL-terminated string to be converted.
			 error -[out] Error code. See header.							
			 
 Returns: An integer value.
 
 Notes: 		  
-----------------------------------------------------------------------------
*/
PUBLIC SW32 StringToInteger( const char *string, W32 *error )
{
	const char *ptr = string;
	SW32 temp;
	SW32 number = 0;
	W32 errortag = 0;
	_boolean bNegative = false;

	if( ! string || ! *string )
	{	
		*error = SCE_NULL_VALUE;
		
		return 0;
	}

	if( *ptr == '-' )
	{
		bNegative = true;

		ptr++;
	}	

	while( *ptr && ISNUMERIC( *ptr ) )
	{
		temp = number;
		number = (number * 10) + *ptr - '0';

		if( number < temp )
		{
			errortag &= SCE_BUFFER_OVERFLOW;
		}

		ptr++;
	}

	if( *ptr )
	{
		errortag &= SCE_NON_NUMERIC;
	}


	if( bNegative )
	{
		number = -number;
	}
	

	*error = errortag;

	return number;
}

/*
-----------------------------------------------------------------------------
 Function: StringToFloat -Convert string to float.
 
 Parameters: string -[in] NUL-terminated string to be converted.
			 error -[out] Error code. See header.							
			 
 Returns: A float value.
 
 Notes: 		  
-----------------------------------------------------------------------------
*/
PUBLIC double StringToFloat( const char *string, W32 *error )
{
	const char *ptr = string;
	double number = 0;
	SW32 exponent = 0;
	W32 expError;
	_boolean bNegative = false;

	*error = 0;

	if( ! string || ! *string )
	{	
		*error &= SCE_NULL_VALUE;
		
		return 0;
	}

	if( *ptr == '-' )
	{
		bNegative = true;

		ptr++;
	}
	else if( *ptr == '+' )
	{
		ptr++;
	}

	while( *ptr && ISNUMERIC( *ptr ) )
	{
		number = (number * 10) + (double)(*ptr - '0');

		ptr++;
	}

	if( *ptr == '.' )
	{
		ptr++;

		while( *ptr && ISNUMERIC( *ptr ) )
		{
			number = (number * 10) + (double)(*ptr - '0');
			exponent--;

			ptr++;
		}
	}

	if( TOLOWER( *ptr ) == 'e' )
	{		
		ptr++;

		exponent += StringToInteger( ptr, &expError );
	}

	if( bNegative )
	{
		number = -number;
	}

	if( expError )
	{
		*error |= expError;
	}

	return (number * pow( 10, exponent ));
}
