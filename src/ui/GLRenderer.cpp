#include "GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

GLRenderer::GLRenderer(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLRenderer::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    node = new GLNode( glm::vec3( 0, 0, 0 ), glm::quat( glm::vec3( 0, 0, 0 ) ) );
    BMDImport::loadFromFile( node, "/home/feelx/devel/Qt-Project/untitled.bmd" );
}

void GLRenderer::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 45.f, (float)w / (float)h, 0.1f, 100.f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
}

void GLRenderer::paintGL()
{
    //Test code
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();

    gluLookAt( glm::sin( x ) * 10, 5, glm::cos( x ) * 10, 0, 0, 0, 0, 1, 0 );

    x += 0.001;

    node->update();
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
