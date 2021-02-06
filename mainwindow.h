#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renamewidget.h"
#include "sendwidget.h"

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
    void on_pushButton_renamer_clicked();
    void on_pushButton_sender_clicked();

private:
    Ui::MainWindow *ui;
    // second and third windows
    RenameWidget *renamerWindow;
    SendWidget *senderWindow;
};

#endif // MAINWINDOW_H
