#include "GLRenderer.h"

GLRenderer::GLRenderer(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLRenderer::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
}

void GLRenderer::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void GLRenderer::paintGL()
{
    //Test code
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(0,0,1);
        glColor3f(0,1,0);
        glVertex3f(1,0,1);
        glColor3f(0,0,1);
        glVertex3f(0,1,0);
    glEnd();
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
