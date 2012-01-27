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

    ui->stackedWidget->setCurrentIndex(0);

    //Get Renderer from ui
    mRenderer = findChild<GLRenderer*>( "renderer" );

    //Set Timer for repainting the graphics
    timer = new QTimer( this );
    timer->setSingleShot( false );
    timer->setInterval( 0 );
    connect( timer, SIGNAL(timeout()), mRenderer, SLOT(updateGL()) );

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

void MainWindow::on_neuesspiel_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//Zurueck-Button bei Fenster Neues Spiel
void MainWindow::on_zurueck_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_optionen_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_credits_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//Zurueck-Button bei Fenster credits
void MainWindow::on_zurueck_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Zurueck-Button bei Fenster optionen
void MainWindow::on_zurueck_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_spielverlassen_clicked()
{
    close();
}
