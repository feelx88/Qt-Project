#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    mRenderer( 0 )
{
    ui->setupUi(this);

    //Get Renderer from ui
    mRenderer = findChild<GLRenderer*>( "renderer" );

    //Set Timer for repainting the grpahics
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
