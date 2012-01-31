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
 *	wav.c:   Handles wav files.
 *	
 *	Author:	Michael Liebscher	<johnnycanuck@users.sourceforge.net>	    
 *	Date:	2004   
 *
 */


#include <stdlib.h>
#include <stdio.h>

#include "../../../common/arch.h"

typedef struct
{
	W8  riff[ 4 ];
	W32 TotalLength;
	W8  wave[ 8 ];
	W32 LengthFORMAT;
	W16 DataType;
	W16 ChannelNumbers;
	W32 SampleRate;
	W32 BytesPerSecond;
	W16 BytesPerSample;
	W16 BitsPerSample;
	W8  data[ 4 ];
	W32 LengthData;

} wavheader_t;


/*
-----------------------------------------------------------------------------
 Function: write_wav() -Writes a wav file. 
 
 Parameters: filename -[in] Pointer to a null-terminated string that 
						specifies the name of the file to save as.
             data -[in] Pointer to audio data.
             size -[in] Length of audio data.
             channels -[in] Number of Channels (0x01 = Mono, 0x02 = Stereo).
             sample_rate -[in] Sample Rate in Hz
             sample_size -[in] Bytes Per Sample: 
                                1 = 8 bit Mono, 
                                2 = 8 bit Stereo or 16 bit Mono, 
                                4 = 16 bit Stereo
 
 Returns: Nothing.
 
 Notes: 

-----------------------------------------------------------------------------
*/
void 
write_wav( const char *filename, void *data, W32 size, 
            W16 channels, W32 sample_rate, 
            W16 sample_size  )
{
    wavheader_t header;
    FILE *handle;
    
    handle = fopen( filename, "wb" );
    if( handle == NULL )
    {
        printf( "unable to write wav file\n" );
        return;
    }
    
// RIFF Chunk        
	header.riff[ 0 ] = 'R';
	header.riff[ 1 ] = 'I';
	header.riff[ 2 ] = 'F';
	header.riff[ 3 ] = 'F';
    
	// Total Length Of Package To Follow
    header.TotalLength = size + 36;

// Format Chunk
	header.wave[ 0 ] = 'W';
	header.wave[ 1 ] = 'A';
	header.wave[ 2 ] = 'V';
	header.wave[ 3 ] = 'E';
	header.wave[ 4 ] = 'f';
	header.wave[ 5 ] = 'm';
	header.wave[ 6 ] = 't';
	header.wave[ 7 ] = ' ';
    
    header.LengthFORMAT = 0x10; // Length Of FORMAT Chunk (always 0x10)
    header.DataType  = 0x01; // Always 0x01
    
    // Channel Numbers (always 0x01 = Mono, 0x02 = Stereo)
    header.ChannelNumbers = channels;   
    
    
    header.SampleRate = sample_rate; // Sample Rate in Hz
    
    
    header.BytesPerSecond = sample_rate * sample_size * channels;
    
    
    header.BytesPerSample = sample_size; // Bytes Per Sample: 1 = 8 bit Mono, 
                        // 2 = 8 bit Stereo or 16 bit Mono, 
                        // 4 = 16 bit Stereo

    
    header.BitsPerSample =  sample_size * 8;
  
// DATA Chunk
    header.data[ 0 ] = 'd';
	header.data[ 1 ] = 'a';
	header.data[ 2 ] = 't';
	header.data[ 3 ] = 'a';

    // Length Of Data To Follow    
    header.LengthData = size;
    
    
    
    fwrite( &header, sizeof( W8 ), sizeof( wavheader_t ), handle );
    
    fwrite( data, sizeof( W8 ), size, handle );
    
    fclose( handle );
}
