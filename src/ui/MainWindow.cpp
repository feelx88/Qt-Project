#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTimer>

#include "../ui/GLRenderer.h"

QTimer* timer;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    mRenderer( 0 )
{
    ui->setupUi(this);

    //Get Renderer from ui
    mRenderer = findChild<GLRenderer*>( "renderer" );

    //Set Timer for repainting the graphics
    timer = new QTimer( this );
    timer->setSingleShot( false );
    timer->setInterval( 0 );
    connect( timer, SIGNAL(timeout()), mRenderer, SLOT(repaint()) );

    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *evt)
{
    if( mRenderer )
        mRenderer->keyPressEvent( evt );

    if( evt->key() == Qt::Key_Tab )
        timer->setInterval( 10 );
    if( evt->key() == Qt::Key_Q )
        timer->setInterval( 100 );
    if( evt->key() == Qt::Key_W )
        timer->setInterval( 0 );
}

void MainWindow::keyReleaseEvent(QKeyEvent *evt)
{
    if( mRenderer )
        mRenderer->keyReleaseEvent( evt );
}
