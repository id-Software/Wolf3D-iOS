/*

	Copyright (C) 2005 Michael Liebscher
	Copyright 2000-2002, Michael Smith <msmith@xiph.org>
	(c) Kenneth Arnold <kcarnold@yahoo.com>
	(c) Monty <monty@xiph.org>

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
 *	vorbisenc_inter.c:   Simple Ogg Vorbis interface.
 *	
 *	Author:	Michael Liebscher <johnnycanuck@users.sourceforge.net>	    
 *	Date:	2005   
 *
 *	Acknowledgement:
 *	Portions from oggenc, Copyright 2000-2002, Michael Smith <msmith@xiph.org>
 *	Vorbize, (c) Kenneth Arnold <kcarnold@yahoo.com>
 *	and libvorbis examples, (c) Monty <monty@xiph.org>
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vorbis/vorbisenc.h>

#include "../../common/arch.h"
#include "../../common/common_utils.h"

#define READSIZE 1024


PRIVATE W32 channels;
PRIVATE W32 samplesize;

PRIVATE W8 *ptrCurrent;
PRIVATE W8 *ptrEnd;

long read_samples( float **buffer, int samples )
{
	int sampbyte = samplesize / 8;
	SW8 *buf;
	long bytes_read;
	int i,j;
	long realsamples;

	buf = ptrCurrent;

	if( (samples * sampbyte * channels) > (ptrEnd - ptrCurrent) )
	{
		bytes_read = ptrEnd - ptrCurrent;
		ptrCurrent = ptrEnd;

		if( bytes_read == 0 )
		{
			return 0;
		}
	}
	else
	{
		bytes_read = samples * sampbyte * channels;
		ptrCurrent += samples * sampbyte * channels;
	}



	realsamples = bytes_read / (sampbyte * channels);


	for( i = 0 ; i < realsamples ; ++i )
	{
		for( j = 0 ; j < channels ; ++j )
		{
			buffer[j][i] = ((buf[ i * 2 * channels + 2 * j + 1 ] << 8) |
							(buf[ i * 2 * channels + 2 * j ] & 0xFF)) / 32768.0f;
		}
	}
		

	return realsamples;
}

int vorbis_encode( const char *filename, void *data, W32 size, W32 in_channels, W32 in_samplesize,
			   W32 rate, W32 quality, W32 max_bitrate, W32 min_bitrate  )
{
	FILE *fp;
	ogg_stream_state os;
	ogg_page 		 og;
	ogg_packet 		 op;

	vorbis_dsp_state	vd;
	vorbis_block		vb;
	vorbis_info			vi;

	ogg_packet header_main;
	ogg_packet header_comments;
	ogg_packet header_codebooks;
	int result;
	unsigned int serialno = 0;

	vorbis_comment comments;

	int ret = 0;
	int eos;
	W32 samplesdone = 0;
	W32 packetsdone = 0;
	W32 bytes_written = 0;



	fp = fopen( filename, "wb" );
	if( fp == NULL )
	{
		return 0;
	}

	memset( &comments, 0, sizeof( comments ) );

	channels = in_channels;
	samplesize = in_samplesize;
	ptrCurrent = (PW8)data;
	ptrEnd = (PW8)data + size;


	vorbis_info_init( &vi );

	if( vorbis_encode_setup_vbr( &vi, channels, rate, quality ) )
	{
		fprintf( stderr, "Mode initialisation failed: invalid parameters for quality\n" );
		vorbis_info_clear( &vi );
		
		return 1;
	}

	/* do we have optional hard quality restrictions? */
	if( max_bitrate > 0 || min_bitrate > 0 )
	{
		struct ovectl_ratemanage_arg ai;

		vorbis_encode_ctl( &vi, OV_ECTL_RATEMANAGE_GET, &ai );

		ai.bitrate_hard_min = min_bitrate;
		ai.bitrate_hard_max = max_bitrate;
		ai.management_active = 1;

		vorbis_encode_ctl( &vi, OV_ECTL_RATEMANAGE_SET, &ai );
	}

	/* Turn off management entirely (if it was turned on). */
	vorbis_encode_ctl( &vi, OV_ECTL_RATEMANAGE_SET, NULL );


	vorbis_encode_setup_init( &vi );

	vorbis_analysis_init( &vd, &vi );
	vorbis_block_init( &vd, &vb );

	ogg_stream_init( &os, serialno );

	/* Now, build the three header packets and send through to the stream 
	   output stage (but defer actual file output until the main encode loop) */


	/* Build the packets */
	ret = vorbis_analysis_headerout( &vd, &comments,
			&header_main, &header_comments, &header_codebooks );

	/* And stream them out */
	ogg_stream_packetin( &os, &header_main );
	ogg_stream_packetin( &os, &header_comments );
	ogg_stream_packetin( &os, &header_codebooks );

	while( (result = ogg_stream_flush( &os, &og )) )
	{
		if( ! result )
		{
			break;
		}

		ret = fwrite( og.header, 1, og.header_len, fp );
		ret += fwrite( og.body, 1, og.body_len, fp );

		if(ret != og.header_len + og.body_len)
		{
			printf( "Failed writing header to output stream\n") ;
			ret = 1;

			goto cleanup; /* Bail and try to clean up stuff */
		}
	}


	eos = 0;

	/* Main encode loop - continue until end of file */
	while( ! eos )
	{
		float **buffer = vorbis_analysis_buffer( &vd, READSIZE );
		long samples_read = read_samples( buffer, READSIZE );

		if( samples_read == 0 )
		{
			/* Tell the library that we wrote 0 bytes - signalling the end */
			vorbis_analysis_wrote( &vd, 0 );
		}
		else
		{
			samplesdone += samples_read;

			/* Call progress update every 40 pages */
			if( packetsdone >= 40 )
			{
				packetsdone = 0;

				// progress bar here
			}

			/* Tell the library how many samples (per channel) we wrote 
			   into the supplied buffer */
			vorbis_analysis_wrote( &vd, samples_read );
		}

		/* While we can get enough data from the library to analyse, one
		   block at a time... */
		while( vorbis_analysis_blockout( &vd, &vb ) == 1 )
		{

			/* Do the main analysis, creating a packet */
			vorbis_analysis( &vb, NULL );
			vorbis_bitrate_addblock( &vb );

			while( vorbis_bitrate_flushpacket( &vd, &op ) ) 
			{
				/* Add packet to bitstream */
				ogg_stream_packetin( &os, &op );
				packetsdone++;

				/* If we've gone over a page boundary, we can do actual output,
				   so do so (for however many pages are available) */

				while( ! eos )
				{
					int result = ogg_stream_pageout( &os, &og );
					if( ! result )
					{
						break;
					}

					ret = fwrite( og.header, 1, og.header_len, fp );
					ret += fwrite( og.body, 1, og.body_len, fp );

					if(ret != og.header_len + og.body_len)
					{
						printf( "Failed writing data to output stream\n" );
						ret = 1;

						goto cleanup; /* Bail */
					}
					else
					{
						bytes_written += ret; 
					}
	
					if( ogg_page_eos( &og ) )
					{
						eos = 1;
					}
				}
			}
		}
	}


cleanup:

	fclose( fp );

	ogg_stream_clear( &os );

	vorbis_block_clear( &vb );
	vorbis_dsp_clear( &vd );
	vorbis_info_clear( &vi );

	return 0;
}
