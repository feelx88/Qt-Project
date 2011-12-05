#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>

class GLRenderer : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLRenderer(QWidget *parent = 0);

signals:

public slots:

};

#endif // GLRENDERER_H
