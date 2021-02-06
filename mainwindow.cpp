#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize the second window
    renamerWindow = new RenameWidget();
    // connected to the slot start the main window on the button in the second window
    connect(renamerWindow, &RenameWidget::mainWindow, this, &MainWindow::show);

    // Initialize the third window
    senderWindow = new SendWidget();
    // connected to the slot start the main window on the button in the third window
    connect(senderWindow, &SendWidget::mainWindow, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_renamer_clicked()
{
    renamerWindow->show();  // Show a second window
    this->close();    // Close the main window
}

void MainWindow::on_pushButton_sender_clicked()
{
    senderWindow->show();  // Show a third window
    this->close();    // Close the main window
}
