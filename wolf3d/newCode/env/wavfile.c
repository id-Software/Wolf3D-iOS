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
 *	wav.c: Wav file loader.
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


PRIVATE W8 *iff_pdata;
PRIVATE W8 *iff_end;
PRIVATE W8 *iff_last_chunk;
PRIVATE W8 *iff_data;
PRIVATE int	iff_chunk_len;



PRIVATE short Wav_GetLittleShort( void )
{
	short val = 0;

	val = *iff_pdata;
	val += (*(iff_pdata + 1) << 8);

	iff_pdata += 2;

	return val;
}


PRIVATE int Wav_GetLittleLong( void )
{
	int val = 0;

	val =  *iff_pdata;
	val += (*(iff_pdata + 1) << 8);
	val += (*(iff_pdata + 2) << 16);
	val += (*(iff_pdata + 3) << 24);

	iff_pdata += 4;

	return val;
}


PRIVATE void Wav_FindNextChunk( const char *name )
{
	while( 1 )
	{
		iff_pdata = iff_last_chunk;

		if( iff_pdata >= iff_end )
		{
			// Didn't find the chunk
			iff_pdata = NULL;
			return;
		}
		
		iff_pdata += 4;
		iff_chunk_len = Wav_GetLittleLong();
		if( iff_chunk_len < 0 )
		{
			iff_pdata = NULL;
			return;
		}

		iff_pdata -= 8;
		iff_last_chunk = iff_pdata + 8 + ((iff_chunk_len + 1) & ~1);
		if( ! my_strnicmp((const char *)iff_pdata, name, 4) )
		{
			return;
		}
	}
}


PRIVATE void Wav_FindChunk( const char *name )
{
	iff_last_chunk = iff_data;

	Wav_FindNextChunk( name );
}

PRIVATE void DumpChunks( void )
{
	char str[ 5 ];
	
	str[ 4 ] = 0;
	iff_pdata = iff_data;
	do
	{
		memcpy( str, iff_pdata, 4 );
		iff_pdata += 4;
		iff_chunk_len = Wav_GetLittleLong();
		Com_Printf( "0x%x : %s (%d)\n", (int)(iff_pdata - 4), str, iff_chunk_len );
		iff_pdata += (iff_chunk_len + 1) & ~1;

	} while( iff_pdata < iff_end );

}


/*
-----------------------------------------------------------------------------
 Function: LoadWavInfo -Load wav file.
 
 Parameters: filename -[in] Name of wav file to load.
			 wav -[out] wav data.
			 info -[out] wav sound info.
 
 Returns: True if file loaded, otherwise false.
 
 Notes: Caller is responsible for freeing wav data by calling Z_Free.

-----------------------------------------------------------------------------
*/
PUBLIC _boolean LoadWavInfo( const char *filename, W8 **wav, soundInfo_t *info )
{
	filehandle_t *hFile;
	W8	*data;
	W32	wavlength;

	hFile = FS_OpenFile( filename, 0 );
	if( ! hFile )
	{
		return false;
	}

	data = (PW8)FS_GetLoadedFilePointer( hFile, SEEK_SET );
	wavlength = FS_GetFileSize( hFile );

	iff_data = data;
	iff_end = data + wavlength;

	// look for RIFF signature
	Wav_FindChunk( "RIFF" );
	if( ! (iff_pdata && ! my_strnicmp( (const char *)iff_pdata + 8, "WAVE", 4 ) ) )
	{
		Com_DPrintf( "[LoadWavInfo]: Missing RIFF/WAVE chunks (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	// Get "fmt " chunk
	iff_data = iff_pdata + 12;

	Wav_FindChunk( "fmt " );
	if( ! iff_pdata )
	{
		Com_DPrintf( "[LoadWavInfo]: Missing fmt chunk (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	iff_pdata += 8;

	if( Wav_GetLittleShort() != 1 )
	{
		Com_DPrintf( "[LoadWavInfo]: Microsoft PCM format only (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	info->channels = Wav_GetLittleShort();
	info->sample_rate = Wav_GetLittleLong();

	iff_pdata += 4;

	info->sample_size = Wav_GetLittleShort(); // Bytes Per Sample

	if (info->sample_size != 1 && info->sample_size != 2)
	{
		Com_DPrintf( "[LoadWavInfo]: only 8 and 16 bit WAV files supported (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	iff_pdata += 2;


	// Find data chunk
	Wav_FindChunk( "data" );
	if( ! iff_pdata )
	{
		Com_DPrintf( "[LoadWavInfo]: missing 'data' chunk (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	iff_pdata += 4;
	info->samples = Wav_GetLittleLong() / info->sample_size;

	if( info->samples <= 0 )
	{
		Com_DPrintf( "[LoadWavInfo]: file with 0 samples (%s)\n", filename );
		FS_CloseFile( hFile );

		return false;
	}

	// Load the data
	*wav = Z_Malloc( info->samples * info->sample_size );
	memcpy( *wav, data + (iff_pdata - data), info->samples * info->sample_size );

	FS_CloseFile( hFile );

	return true;
}

