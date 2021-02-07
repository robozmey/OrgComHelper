#ifndef SENDWIDGET_H
#define SENDWIDGET_H

#include <QtCore>
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
#include <QThread>

//#include "smtp.h"
#include "util.h"
#include "src/SmtpMime"

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
    QString get_subject(bool &isNotOK);
    QStringList get_attachments(bool &isNotOK);
    QFileInfoList get_personalAtteachments(bool &isNotOK);
    QString get_emailBodyText(bool &isNotOK);
    QVector<Participant> get_participant_email_list(bool &isNotOK);
    QStringList get_participant_text_splited(bool &isNotOK);
    std::tuple<QVector<Participant>, QVector<QStringList>, QVector<QStringList>> get_mergedInfoToSend(QStringList participant_text_splited, QFileInfoList personalAttachments_list, QVector<Participant> participant_email_list, bool &isNotOK);
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
