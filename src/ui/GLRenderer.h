#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QtOpenGL/QGLWidget>

class QKeyEvent;
class QEvent;

class Node;
class Game;

class GLRenderer : public QGLWidget
{
    Q_OBJECT
public:
    //Default Qt contructor
    explicit GLRenderer(QWidget *parent = 0);

    static Node* getRootNode()
    {
        return sRootNode;
    }

    void keyPressEvent( QKeyEvent *evt );
    void keyReleaseEvent( QKeyEvent *evt );

protected:
    //Initialize the drawing surface
    void initializeGL();

    //Set the correct Viewport on resize
    void resizeGL(int w, int h);

    //Here everything gets drawn
    void paintGL();

    static Node* sRootNode;

    Game *mGame;

    int oldNSec;
};

#endif // GLRENDERER_H
