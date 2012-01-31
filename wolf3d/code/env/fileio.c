/*

	Copyright (C) 2005 Michael Liebscher <johnnycanuck@users.sourceforge.net>

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

#include "../wolfiphone.h"

// if true, use mmap instead of alloc and read
//#define USE_MMAP

/*
-----------------------------------------------------------------------------
 Function: FS_GetLoadedFilePointer() -Get file pointer.
 
 Parameters: 
			filestream -[in] Target file handle.
			origin -[in] Pointer position
							SEEK_SET -Beginning of file.
							SEEK_CUR -Current position of file pointer.
							SEEK_END -End of file.
 
 Returns: File pointer on success, otherwise NULL.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC void *FS_GetLoadedFilePointer( filehandle_t *fhandle, W32 origin )
{
	switch( origin )
	{
		case SEEK_SET:
			return( (void *)fhandle->ptrStart );

		case SEEK_END:
			return( (void *)fhandle->ptrEnd );

		case SEEK_CUR:
			return( (void *)fhandle->ptrCurrent );
	}

	return NULL;
}

/*
-----------------------------------------------------------------------------
 Function: FS_GetFileSize() -Get the length of a file.
 
 Parameters: filestream -[in] Target file handle.
 
 Returns: The file length in bytes.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC SW32 FS_GetFileSize( filehandle_t *fhandle )
{
	return fhandle->filesize;
}

/*
-----------------------------------------------------------------------------
 Function: FS_FileSeek() -Moves the file pointer to a specified location.
 
 Parameters: 
			fhandle -[in] Pointer to filehandle_t structure.
			offset -[in] Number of bytes from origin
			origin -[in] Initial position
							SEEK_SET -Beginning of file.
							SEEK_CUR -Current position of file pointer.
							SEEK_END -End of file.
							
 Returns: If successful zero, otherwise a nonzero value.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC W32 FS_FileSeek( filehandle_t *fhandle, SW32 offset, W32 origin )
{
	switch( origin )
	{
		case SEEK_SET:
			if( offset < 0 ||
				offset > fhandle->filesize )
			{
				return 1;
			}
			
			fhandle->ptrCurrent = fhandle->ptrStart + offset;
			break;

		case SEEK_END:
			if( offset > 0 )
			{
				return 1;
			}

			/* Commenting this out to avoid a warning - note that this is probably not what was
			   intended!
			   
			// offset is negative 
			if( (fhandle->filesize + offset) < 0  )
			{
				return 1;
			}

			*/

			// offset is negative 
			fhandle->ptrCurrent = fhandle->ptrEnd + offset;
			break;

		case SEEK_CUR:
			if( offset < 0 )
			{
				// offset is negative
				if( ((fhandle->ptrCurrent - fhandle->ptrStart) + offset) < 0 )
				{
					return 1;
				}
			}

			if( offset > 0 )
			{
				if( offset > (fhandle->ptrEnd - fhandle->ptrCurrent) )
				{
					return 1;
				}
			}

			fhandle->ptrCurrent += offset;
			break;

		default:
			return 1;
	}

	return 0;
}

/*
-----------------------------------------------------------------------------
 Function: FS_FileTell() -Gets the current position of a file pointer.
 
 Parameters: fhandle -[in] Pointer to filehandle_t structure.
										
 Returns: If successful current file position, otherwise -1.
 
 Notes: 
-----------------------------------------------------------------------------
*/
PUBLIC SW32 FS_FileTell( filehandle_t *fhandle )
{
	return( fhandle->ptrCurrent - fhandle->ptrStart );
}

/*
-----------------------------------------------------------------------------
 Function: FS_CloseFile -Close file handle.
 
 Parameters: filestream -[in] Pointer to valid FILE structure.		 
 
 Returns: Nothing.
 
 Notes: Closes a file stream that was returned by FS_FOpenFile.

-----------------------------------------------------------------------------
*/
PUBLIC void FS_CloseFile( filehandle_t *fhandle )
{
	if( fhandle->filedata )
	{
#ifdef USE_MMAP
		if ( munmap( fhandle->filedata, fhandle->filesize ) != 0 ) {
			assert( 0 );
		}
#else
		free( fhandle->filedata );
#endif
		fhandle->filedata = NULL;
	}

	Z_Free( fhandle );
}


/*
-----------------------------------------------------------------------------
 Function: FS_OpenFile -Open file from the file system.
 
-----------------------------------------------------------------------------
*/
PUBLIC filehandle_t *FS_OpenFile( const char *filename, W32 FlagsAndAttributes )
{
	char			netpath[ MAX_OSPATH ];
	filehandle_t	*hFile;
	const char		*pathBase;
	struct stat s;
	int		fd;
	

//
//	Check for the file in the directory tree
//
	if ( FlagsAndAttributes & FA_FILE_IPHONE_DOC_DIR ) {
		extern char iphoneDocDirectory[1024];
		pathBase = iphoneDocDirectory;
		my_snprintf( netpath, sizeof( netpath ), "%s/%s", pathBase, filename );
	} else {
		pathBase = FS_Gamedir();
		my_snprintf( netpath, sizeof( netpath ), "%s/%s", pathBase, filename );
	}

	// high performance file mapping path, avoiding stdio
	fd = open( netpath, O_RDONLY );
	if ( fd == -1 ) {
		//if it couldn't be found in that path then check again in the document directory
		//gsh
		//pathBase = FS_ForceGamedir(); 
		extern char iphoneDocDirectory[1024];
		pathBase = iphoneDocDirectory;
		my_snprintf( netpath, sizeof( netpath ), "%s/%s", pathBase, filename );
		fd = open( netpath, O_RDONLY );
		if ( fd == -1 ) {  
			return NULL;
		}
	}
	fstat( fd, &s );
	
	hFile = Z_Malloc( sizeof( filehandle_t ) );
	memset( hFile, 0, sizeof( filehandle_t ) );
	
	hFile->filesize = s.st_size;
#ifdef USE_MMAP	
	hFile->filedata = mmap( NULL, hFile->filesize, PROT_READ, MAP_FILE|MAP_PRIVATE, fd, 0 );
	if ( (int)hFile->filedata == -1 ) {
		Com_Printf( "mmap failed: %s\n", strerror( errno ) );
		assert( 0 );
	}
#else
	hFile->filedata = malloc( hFile->filesize );
	read( fd, hFile->filedata, hFile->filesize );
#endif
	hFile->ptrStart =  hFile->ptrCurrent = (PW8)hFile->filedata;
	hFile->ptrEnd =  (PW8)hFile->filedata + hFile->filesize;
	hFile->bLoaded = true;
	// mmap doesn't require the file to stay open
	close( fd );
	return hFile;
}


/*
-----------------------------------------------------------------------------
 Function: FS_ReadFile -Reads data from a stream.
 
 Parameters: buffer -[in/out] Storage location for data.
			 size -[in] Item size in bytes.
			 count -[in] Maximum number of items to be read.
			 fhandle -[in] Pointer to valid filehandle_t structure.
 
 Returns: On success number of full items actually read, otherwise -1.
 
 Notes:

-----------------------------------------------------------------------------
*/
PUBLIC SW32 FS_ReadFile( void *buffer, W32 size, W32 count, filehandle_t *fhandle )
{		
	W8	*buf = (PW8)buffer;
	W32 i;

	if( (size * count) > (fhandle->ptrEnd - fhandle->ptrCurrent) )
	{
		SW32 read;

		read = (fhandle->ptrEnd - fhandle->ptrCurrent);

		for( i = 0 ; i < (fhandle->ptrEnd - fhandle->ptrCurrent) ; ++i )
		{
			buf[ i ] = fhandle->ptrCurrent[ i ];
		}

		fhandle->ptrCurrent = fhandle->ptrEnd;
		
		return( read );
	}
	else
	{
		for( i = 0 ; i < (size * count) ; ++i, fhandle->ptrCurrent++ )
		{
			buf[ i ] = *fhandle->ptrCurrent;
		}
	
		return( (size * count) / size );
	}

	/* should never get here */
	return -1;
}
