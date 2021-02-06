#ifndef RENAMEWIDGET_H
#define RENAMEWIDGET_H

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

#include "util.h"

namespace Ui {
    class RenameWidget;
}

class RenameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RenameWidget(QWidget *parent = 0);
    ~RenameWidget();

signals:
    void mainWindow();

private slots:
    void on_radioButton_1Typ_clicked();
    void on_radioButton_2Typ_clicked();
    void on_radioButton_without_clicked();
    void on_radioButton_other_clicked();
    void on_pushButton_upload_clicked();
    void on_pushButton_rename_clicked();
    void on_pushButton_protocol_clicked();
    void on_pushButton_namelessDir_clicked();
    void on_pushButton_distDir_clicked();

    void showErrorMessage(QString);

private:
    Ui::RenameWidget *ui;
};

#endif // RENAMEWIDGET_H
