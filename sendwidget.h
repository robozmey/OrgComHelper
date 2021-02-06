#ifndef SENDWIDGET_H
#define SENDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <string>
#include <QDir>
#include <QFile>
#include <QErrorMessage>
#include <QProgressDialog>

#include "smtp.h"
#include "util.h"

namespace Ui {
    class SendWidget;
}

class SendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SendWidget(QWidget *parent = 0);
    ~SendWidget();

signals:
    void mainWindow();

private slots:
    void on_pushButton_emailBase_clicked();
    void on_pushButton_attachments_clicked();
    void on_pushButton_personalAttachments_clicked();
    void on_pushButton_emailBody_clicked();
    void on_pushButton_emailKey_clicked();
    void on_pushButton_startSend_clicked();
    void showErrorMessage(QString);
    void mailSent(QString);

private:
    Ui::SendWidget *ui;
};



#endif // SENDWIDGET_H
