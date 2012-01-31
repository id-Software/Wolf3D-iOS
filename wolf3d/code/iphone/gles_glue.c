/*
 
 Copyright (C) 2009 Id Software, Inc.
 
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


#include "wolfiphone.h"

//int registration_sequence;

#include "iphone_qgl.h"


#ifdef QGL_LOG_GL_CALLS
unsigned int QGLLogGLCalls = 1;
FILE *QGLDebugFile(void) {
	return stdout;
}
#endif

void QGLCheckError(const char *message) {
	GLint err = qglGetError();
	if ( err != GL_NO_ERROR ) {
		printf( "GL ERROR %d from %s\n", err, message );
	}
}

unsigned int QGLBeginStarted = 0;




struct Vertex {
	float xyz[3];
	float st[2];
	GLubyte c[4];
};

#define MAX_VERTS 16384

typedef struct Vertex Vertex;
Vertex immediate[ MAX_VERTS ];
Vertex vab;
short quad_indexes[MAX_VERTS * 3 / 2 ];
int curr_vertex;
GLenum curr_prim;

void		InitImmediateModeGL() {
	for ( int i = 0; i < MAX_VERTS * 3 / 2; i+=6 ) {
		int q = i / 6 * 4;
		quad_indexes[ i + 0 ] = q + 0;
		quad_indexes[ i + 1 ] = q + 1;
		quad_indexes[ i + 2 ] = q + 2;
		
		quad_indexes[ i + 3 ] = q + 0;
		quad_indexes[ i + 4 ] = q + 2;
		quad_indexes[ i + 5 ] = q + 3;
	}
	
	qglVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].xyz );
	qglTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].st );
	qglColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex ), immediate[ 0 ].c );
	qglEnableClientState( GL_VERTEX_ARRAY );
	qglEnableClientState( GL_TEXTURE_COORD_ARRAY );
	qglEnableClientState( GL_COLOR_ARRAY );
}

void pfglBegin( GLenum prim ) {
	curr_vertex = 0;
	curr_prim = prim;
}

void pfglVertex3f( float x, float y, float z ) {
	assert( curr_vertex < MAX_VERTS );
	vab.xyz[ 0 ] = x;
	vab.xyz[ 1 ] = y;
	vab.xyz[ 2 ] = z;
	immediate[ curr_vertex ] = vab;
	curr_vertex++;
}
void pfglVertex2i( GLint x, GLint y ) {
	assert( curr_vertex < MAX_VERTS );
	vab.xyz[ 0 ] = (float)x;
	vab.xyz[ 1 ] = (float)y;
	vab.xyz[ 2 ] = 0.0f;
	immediate[ curr_vertex ] = vab;
	curr_vertex++;
}
void pfglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a ) {
	vab.c[ 0 ] = r;
	vab.c[ 1 ] = g;
	vab.c[ 2 ] = b;
	vab.c[ 3 ] = a;
}
void pfglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a ) {
	vab.c[ 0 ] = (GLubyte) ( r * 255 );
	vab.c[ 1 ] = (GLubyte) ( g * 255 );
	vab.c[ 2 ] = (GLubyte) ( b * 255 );
	vab.c[ 3 ] = (GLubyte) ( a * 255 );
}
void pfglTexCoord2i( GLint s, GLint t ) {
	vab.st[ 0 ] = (float)s;
	vab.st[ 1 ] = (float)t;
}
void pfglTexCoord2f( GLfloat s, GLfloat t ) {
	vab.st[ 0 ] = s;
	vab.st[ 1 ] = t;
}

void pfglEnd() {
	if ( curr_prim == GL_QUADS ) {
		qglDrawElements( GL_TRIANGLES, curr_vertex / 4 * 6, GL_UNSIGNED_SHORT, quad_indexes );
	} else {
		qglDrawArrays( curr_prim, 0, curr_vertex );
	}
	curr_vertex = 0;
	curr_prim = 0;
}

