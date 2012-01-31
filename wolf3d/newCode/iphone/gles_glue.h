

#ifndef __GLES_GLUE_H__
#define __GLES_GLUE_H__

#include "iphone_qgl.h"

typedef GLfloat GLdouble;

#define pfglEnable qglEnable
#define pfglDisable qglDisable
#define pfglActiveTextureARB qglActiveTexture
#define pfglGenTextures qglGenTextures
#define pfglDeleteTextures qglDeleteTextures
#define pfglDepthRange qglDepthRangef
#define pfglDepthFunc qglDepthFunc
#define pfglCullFace qglCullFace
#define pfglColor3f(r,g,b) pfglColor4f(r,g,b,1.0f)
#define pfglColor3ubv(c) pfglColor4ub( (c)[0], (c)[1], (c)[2], 255 )
#define pfglColor4ubv(c) pfglColor4ub( (c)[0], (c)[1], (c)[2], (c)[3] )
#define pfglBlendFunc qglBlendFunc
#define pfglClearColor qglClearColor
#define pfglClear qglClear
#define pfglDrawBuffer(buffer)  
#define pfglLineWidth qglLineWidth
#define pfglBindTexture qglBindTexture 
#define pfglTexParameteri qglTexParameteri
#define pfglTexParameterf qglTexParameterf
#define pfglTexImage2D qglTexImage2D
#define pfglFrustum qglFrustumf
#define pfglOrtho qglOrthof
#define pfglLoadIdentity qglLoadIdentity
#define pfglMatrixMode qglMatrixMode
#define pfglShadeModel qglShadeModel
#define pfglRotatef qglRotatef
#define pfglTranslatef qglTranslatef
#define pfglReadPixels qglReadPixels
#define pfglAlphaFunc qglAlphaFunc
#define pfglViewport qglViewport
#define pfglTexEnvi qglTexEnvi
#define pfglClientActiveTextureARB qglClientActiveTexture

#define pfglGetIntegerv qglGetIntegerv
#define pfglGetString qglGetString
#define pfglGetError qglGetError


#define GL_QUADS 888

/*
void		GLimp_BeginFrame();
void		GLimp_EndFrame( void );
_boolean	GLimp_Init( void *hinstance, void *hWnd );
void		GLimp_Shutdown( void );
int     	GLimp_SetMode( int *pwidth, int *pheight, int mode, _boolean fullscreen );
void		GLimp_AppActivate( _boolean active );
*/

#ifdef __cplusplus
extern "C" {
#endif
	
void pfglBegin( GLenum prim );
void pfglVertex3f( float x, float y, float z );
void pfglVertex2i( GLint x, GLint y );
void pfglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
void pfglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
void pfglTexCoord2i( GLint s, GLint t );
void pfglTexCoord2f( GLfloat s, GLfloat t );

void pfglEnd();

#ifdef __cplusplus
}
#endif
	
#endif