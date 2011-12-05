#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>

class GLRenderer : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLRenderer(QWidget *parent = 0);

protected:
    //Initialize the drawing surface
    void initializeGL();

    //Set the correct
    void resizeGL(int w, int h);
    void paintGL();

    float x;

};

#endif // GLRENDERER_H
