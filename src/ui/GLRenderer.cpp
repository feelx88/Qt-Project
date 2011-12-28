#include "GLRenderer.h"

#include <GL/glu.h>

#include <QKeyEvent>

#include "../game/Game.h"
#include "../scene/Node.h"

Node* GLRenderer::sRootNode = 0;

GLRenderer::GLRenderer(QWidget *parent)
    : QGLWidget(parent), mGame( new Game() ), oldNSec( 0 )
{
    sRootNode = new Node( 0 );
    setAutoBufferSwap( true );
}

void GLRenderer::keyPressEvent( QKeyEvent *evt )
{
    mGame->processKeyEvents( evt, true );
}

void GLRenderer::keyReleaseEvent(QKeyEvent *evt)
{
    mGame->processKeyEvents( evt, false );
}

void GLRenderer::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    mGame->init();
}

void GLRenderer::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 45.f, (float)w / (float)h, 0.1f, 500.f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void GLRenderer::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();

    mGame->run();

    sRootNode->update();
}

/* Nice effect :D, at setinterval(10)
    x++; //int
    glRotatef((float)(x)/100.f, 0, 0, 1);
    glBegin(GL_TRIANGLES);
        glColor3f(1,0,x%2);
        glVertex3f(0,0,1);
        glColor3f(0,1,0);
        glVertex3f(1,0,1);
        glColor3f(0,0,1);
        glVertex3f(0,1,0);
    glEnd();
*/
