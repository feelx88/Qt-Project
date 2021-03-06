#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../scene/GLNode.h"

#include "../ui/GLRenderer.h"

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
    mTimer = new QTimer( this );
    mTimer->setSingleShot( false );
    mTimer->setInterval( 0 );
    connect( mTimer, SIGNAL(timeout()), mRenderer, SLOT(updateGL()) );

    mRenderer->sMainWindow = this;
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
    GLNode::clearTextures();
    close();
}

void MainWindow::on_spielstarten_clicked()
{
    ui->stackedWidget->setCurrentIndex( 5 );
    mTimer->start();
    mRenderer->start();
    showFullScreen();
}

void MainWindow::on_pushButton_clicked()
{
    mRenderer->stop();
    mTimer->stop();
    showNormal();
    ui->stackedWidget->setCurrentIndex( 0 );
}

void MainWindow::lost()
{
    ui->stackedWidget->setCurrentIndex( 4 );
    showNormal();
}

void MainWindow::end()
{
    ui->stackedWidget->setCurrentIndex( 0 );
    mTimer->stop();
    mRenderer->stop();
    showNormal();
}
