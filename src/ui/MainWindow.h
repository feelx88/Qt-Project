#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "src/ui/GLRenderer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //Main renderer
    GLRenderer *mRenderer;
};

#endif // MAINWINDOW_H
