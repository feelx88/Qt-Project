#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>

class GLNode;

class GLRenderer : public QGLWidget
{
    Q_OBJECT
public:
    //Default Qt contructor
    explicit GLRenderer(QWidget *parent = 0);

protected:
    //Initialize the drawing surface
    void initializeGL();

    //Set the correct Viewport on resize
    void resizeGL(int w, int h);

    //Here everything gets drawn
    void paintGL();

    GLNode *node;

    float x;

};

#endif // GLRENDERER_H
