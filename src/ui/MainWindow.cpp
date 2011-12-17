#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTimer>

#include "../ui/GLRenderer.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    mRenderer( 0 )
{
    ui->setupUi(this);

    //Get Renderer from ui
    mRenderer = findChild<GLRenderer*>( "renderer" );

    //Set Timer for repainting the graphics
    QTimer *timer = new QTimer( this );
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
}

void MainWindow::keyReleaseEvent(QKeyEvent *evt)
{
    if( mRenderer )
        mRenderer->keyReleaseEvent( evt );
}
