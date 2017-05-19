#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void serialReceived();
    void on_actionConnectButtonAction_triggered();
    void redrawFrame();
    void on_actionTableCellAction_triggered();

    void on_actionDrawEraseToggleAction_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
