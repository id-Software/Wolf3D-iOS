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
 *	cvar.c: Dynamic variable tracking.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>
 *
 *	Acknowledgement:
 *	This code was derived from Quake II, and was originally
 *	written by Id Software, Inc.
 *
 */

#include "../wolfiphone.h"


cvar_t	*cvar_vars;


/*
-----------------------------------------------------------------------------
 Function: Cvar_InfoValidate -String can not have / " ;
 
 Parameters: string -[in] String to validate.
 
 Returns: 1 if string is valid, otherwise 0;
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE _boolean Cvar_InfoValidate( const char *string )
{
	if( strstr( string, "\\" ) )
	{
		return false;
	}

	if( strstr( string, "\"" ) )
	{
		return false;
	}

	if( strstr( string, ";" ) )
	{
		return false;
	}

	return true;
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_FindVar -Return cvar;
 
 Parameters: var_name -[in] Name of cvar to lookup.
 
 Returns: NULL if cvar not found, otherwise returns the cvar.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE cvar_t *Cvar_FindVar( const char *var_name )
{
	cvar_t	*var;
	W32 hashid;

	hashid = my_strhash( var_name );
	
	for( var = cvar_vars ; var ; var = var->next )
	{
		if( hashid == var->id )
		{
			return var;
		}
	}

	return NULL;
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_VariableValue -Get value of cvar.
 
 Parameters: var_name -[in] Name of cvar to get value.
 
 Returns: 0 if not found, other the value of the cvar.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC float Cvar_VariableValue( const char *var_name )
{
	cvar_t	*var;
	
	var = Cvar_FindVar( var_name );	
	if( ! var )
	{
		return 0;
	}

	return (float)atof( var->string );
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_VariableString -Get cvar variable as string.
 
 Parameters: var_name -[in] Name of cvar to get value.
 
 Returns: Blank string on error, otherwise value string.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cvar_VariableString( const char *var_name )
{
	cvar_t *var;
	
	var = Cvar_FindVar( var_name );
	if( ! var )
	{
		return "";
	}

	return var->string;
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_CompleteVariable -Complete cvar string name.
 
 Parameters: partial -[in] Partial name of string to look up.
 
 Returns: NULL if partial string not found, otherwise the complete
			string name.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cvar_CompleteVariable( const char *partial )
{
	cvar_t	*cvar;
	size_t	len;
	W32 hashid;
	
	len = strlen( partial );
	
	if( ! len )
	{
		return NULL;
	}	

//		
// Check exact match.
//
	hashid = my_strhash( partial );

	for( cvar = cvar_vars ; cvar ; cvar = cvar->next )
	{
		if( hashid == cvar->id )
		{
			return cvar->name;
		}
	}

//
// Check partial match.
//
	for( cvar = cvar_vars ; cvar ; cvar = cvar->next )
	{
		if( ! strncmp( partial, cvar->name, len ) )
		{
			return cvar->name;
		}
	}

	return NULL;
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_Get -Get cvar structure.
 
 Parameters:
			var_name -[in] the name of the cvar variable.
			var_value -[in] string value of the cvar variable.
			flags -[in] see CVARFlags for more information.
 
 Returns: NULL on error, otherwise valid pointer to cvar_t structure.
 
 Notes: 
	If the variable already exists, the value will not be set and
	the flags will be or'ed.
-----------------------------------------------------------------------------
*/
PUBLIC cvar_t *Cvar_Get( const char *var_name, const char *var_value, CVARFlags flags )
{
	cvar_t	*var;
	
	if( flags & (CVAR_USERINFO | CVAR_SERVERINFO) )
	{
		if( ! Cvar_InfoValidate( var_name ) )
		{
			Com_Printf( "invalid info cvar name\n" );
			return NULL;
		}
	}

	var = Cvar_FindVar( var_name );
	if( var )
	{
		var->flags |= flags;
		return var;
	}

	if( ! var_value )
	{
		return NULL;
	}

	if( flags & (CVAR_USERINFO | CVAR_SERVERINFO) )
	{
		if( ! Cvar_InfoValidate( var_value ) )
		{
			Com_Printf( "invalid info cvar value\n" );
			return NULL;
		}
	}

	var = Z_Malloc( sizeof( *var ) );
	var->name = my_CopyString( var_name );
	var->string = my_CopyString( var_value );
	var->id = my_strhash( var_name );
	var->modified = true;
	var->value = (float)atof( var->string );

	// link the variable in
	var->next = cvar_vars;
	cvar_vars = var;

	var->flags = flags;

	return var;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE cvar_t *Cvar_Set2( const char *var_name, const char *value, _boolean force )
{
	cvar_t	*var;

	var = Cvar_FindVar( var_name );
	if( ! var )
	{	// create it
		return Cvar_Get( var_name, value, CVAR_INIT );
	}

	if( var->flags & (CVAR_USERINFO | CVAR_SERVERINFO) )
	{
		if( ! Cvar_InfoValidate( value ) )
		{
			Com_Printf( "invalid info cvar value\n" );
			return var;
		}
	}

	if( ! force )
	{
		if( var->flags & CVAR_NOSET )
		{
			Com_Printf( "%s is write protected.\n", var_name );
			return var;
		}

		if( var->flags & CVAR_LATCH )
		{
			if( var->latched_string )
			{
				if( strcmp( value, var->latched_string ) == 0 )
				{
					return var;
				}

				Z_Free( var->latched_string );
			}
			else
			{
				if( strcmp( value, var->string ) == 0 )
				{
					return var;
				}
			}
			return var;
		}
	}
	else
	{
		if( var->latched_string )
		{
			Z_Free( var->latched_string );
			var->latched_string = NULL;
		}
	}

	if( ! strcmp( value, var->string ) )
	{
		return var;		// not changed
	}

	var->modified = true;

	if( var->flags & CVAR_USERINFO )
	{
		userinfo_modified = true;	// transmit at next oportunity
	}
	
	Z_Free( var->string );	// free the old value string
	
	var->string = my_CopyString( value );
	var->value = (float)atof( var->string );

	return var;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC cvar_t *Cvar_ForceSet( const char *var_name, const char *value )
{
	return Cvar_Set2( var_name, value, true );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC cvar_t *Cvar_Set( const char *var_name, const char *value )
{
	return Cvar_Set2( var_name, value, false );
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC cvar_t *Cvar_FullSet( const char *var_name, const char *value, CVARFlags flags )
{
	cvar_t	*var;
	
	var = Cvar_FindVar( var_name );
	if( ! var )
	{	// create it
		return Cvar_Get( var_name, value, flags );
	}

	var->modified = true;

	if( var->flags & CVAR_USERINFO )
	{
		userinfo_modified = true;	// transmit at next oportunity
	}
	
	Z_Free( var->string );	// free the old value string
	
	var->string = my_CopyString( value );
	var->value = (float)atof( var->string );
	var->flags = flags;

	return var;
}

/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cvar_SetValue( const char *var_name, float value )
{
	char	val[ 32 ];

	if( value == (int)value )
	{
		my_snprintf( val, sizeof( val ), "%i", (int)value );
	}
	else
	{
		my_snprintf( val, sizeof( val ), "%f", value );
	}
	
	Cvar_Set( var_name, val );
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_GetLatchedVars -Any variables with latched values will now 
								be updated
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cvar_GetLatchedVars( void )
{
	cvar_t	*var;

	for( var = cvar_vars ; var ; var = var->next )
	{
		if( ! var->latched_string )
		{
			continue;
		}

		Z_Free( var->string );
		var->string = var->latched_string;
		var->latched_string = NULL;
		var->value = (float)atof(var->string);
	}
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_Command -Handles variable inspection and changing from 
						the console.
 
 Parameters: Nothing.
 
 Returns: false if variable not found, otherwise true.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC _boolean Cvar_Command( void )
{
	cvar_t	*v;

// check variables
	v = Cvar_FindVar( Cmd_Argv( 0 ) );
	if( ! v )
	{
		return false;
	}
		
// perform a variable print or set
	if( Cmd_Argc() == 1 )
	{
		Com_Printf( "\"%s\" is \"%s\"\n", v->name, v->string );
		return true;
	}

	Cvar_Set( v->name, Cmd_Argv( 1 ) );
	return true;
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_Set_f -Allows setting and defining of arbitrary cvars from console.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cvar_Set_f( void )
{
	int		c;
	int		flags;

	c = Cmd_Argc();
	if( c != 3 && c != 4 )
	{
		Com_Printf( "usage: set <variable> <value> [u / s]\n" );
		return;
	}

	if( c == 4 )
	{
		if( ! strcmp( Cmd_Argv( 3 ), "u" ) )
		{
			flags = CVAR_USERINFO;
		}
		else if( ! strcmp( Cmd_Argv( 3 ), "s" ) )
		{
			flags = CVAR_SERVERINFO;
		}
		else
		{
			Com_Printf( "flags can only be 'u' or 's'\n" );
			return;
		}
		Cvar_FullSet( Cmd_Argv( 1 ), Cmd_Argv( 2 ), flags );
	}
	else
	{
		Cvar_Set( Cmd_Argv( 1 ), Cmd_Argv( 2 ) );
	}
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_WriteVariables -Appends lines containing "set variable value"
								for all variables with the archive flag set
								to true.
 
 Parameters:
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC void Cvar_WriteVariables( const char *path )
{
	cvar_t	*var;
	char	buffer[1024];
	FILE	*f;

	f = fopen( path, "a" );
	for( var = cvar_vars ; var ; var = var->next )
	{
		if( var->flags & CVAR_ARCHIVE )
		{
			my_snprintf( buffer, sizeof( buffer ), "set %s \"%s\"\n", var->name, var->string );
			fprintf( f, "%s", buffer );
		}
	}
	fclose( f );
}

/*
-----------------------------------------------------------------------------
 Function: Cvar_List_f -Print all cvars to the console.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE void Cvar_List_f( void )
{
	cvar_t	*var;
	int		i;

	i = 0;
	for( var = cvar_vars ; var ; var = var->next, ++i )
	{
		if( var->flags & CVAR_ARCHIVE )
		{
			Com_Printf ("*");
		}
		else
		{
			Com_Printf (" ");
		}


		if( var->flags & CVAR_USERINFO )
		{
			Com_Printf ("U");
		}
		else
		{
			Com_Printf (" ");
		}


		if (var->flags & CVAR_SERVERINFO)
		{
			Com_Printf ("S");
		}
		else
		{
			Com_Printf (" ");
		}


		if( var->flags & CVAR_NOSET )
		{
			Com_Printf ("-");
		}
		else if (var->flags & CVAR_LATCH)
		{
			Com_Printf ("L");
		}
		else
		{
			Com_Printf (" ");
		}
		
		Com_Printf (" %s \"%s\"\n", var->name, var->string);
	}

	Com_Printf ("%i cvars\n", i);
}


PUBLIC _boolean userinfo_modified;


/*
-----------------------------------------------------------------------------
 Function: 
 
 Parameters:
 
 Returns:
 
 Notes: 

-----------------------------------------------------------------------------
*/
PRIVATE char *Cvar_BitInfo( int bit )
{
	return NULL;
/*	static char	info[ MAX_INFO_STRING ];
	cvar_t	*var;

	info[0] = 0;

	for (var = cvar_vars ; var ; var = var->next)
	{
		if (var->flags & bit)
			Info_SetValueForKey (info, var->name, var->string);
	}
	return info;
*/
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_Userinfo -Get CVAR_USERINFO cvars in a string.
 
 Parameters: Nothing.
 
 Returns: An info string containing all the CVAR_USERINFO cvars.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cvar_Userinfo( void )
{
	return Cvar_BitInfo( CVAR_USERINFO );
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_Serverinfo -Get CVAR_SERVERINFO cvars in a string.
 
 Parameters: Nothing.
 
 Returns: An info string containing all the CVAR_SERVERINFO cvars.
 
 Notes: 

-----------------------------------------------------------------------------
*/
PUBLIC char *Cvar_Serverinfo( void )
{
	return Cvar_BitInfo( CVAR_SERVERINFO );
}


/*
-----------------------------------------------------------------------------
 Function: Cvar_Init -Initialize cvar console functions.
 
 Parameters: Nothing.
 
 Returns: Nothing.
 
 Notes: 
 
	This is done so that we can read in archived cvars from cfg files.
	Also to list all the cvar variables.
-----------------------------------------------------------------------------
*/
PUBLIC void Cvar_Init( void )
{
	Cmd_AddCommand( "set", Cvar_Set_f ); 
	Cmd_AddCommand( "listCvars", Cvar_List_f );
}

