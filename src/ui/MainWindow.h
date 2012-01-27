#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "src/ui/GLRenderer.h"

#include <QKeyEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent( QKeyEvent *evt );
    void keyReleaseEvent( QKeyEvent *evt );

private slots:
    void on_neuesspiel_2_clicked();

    void on_zurueck_clicked();

    void on_optionen_clicked();

    void on_credits_clicked();

    void on_zurueck_2_clicked();

    void on_zurueck_3_clicked();

    void on_spielverlassen_clicked();

private:
    Ui::MainWindow *ui;

    //Main renderer
    GLRenderer *mRenderer;
};

#endif // MAINWINDOW_H
