#include "sendwidget.h"
#include "ui_sendwidget.h"

SendWidget::SendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendWidget)
{
    ui->setupUi(this);

    QSslSocket::supportsSsl();
}

SendWidget::~SendWidget()
{
    delete ui;
}

void SendWidget::on_pushButton_emailBase_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
            tr("Выберите файл с адресами электронных почт участников"), "",
            tr("Текстовый файл (*.txt)"));

    ui->lineEdit_emailBase->setText(path);
}

void SendWidget::on_pushButton_attachments_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
            tr("Выберите папку с общими приложениями к письму"), "");

    ui->lineEdit_attachments->setText(path);
}

void SendWidget::on_pushButton_personalAttachments_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
            tr("Выберите папку с работами участников"), "");

    ui->lineEdit_personalAttachments->setText(path);
}

void SendWidget::on_pushButton_emailBody_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
            tr("Выберите протокол"), "",
            tr("Веб-страница (*.htm *.html)"));

    ui->lineEdit_emailBody->setText(path);
}


void SendWidget::on_pushButton_emailKey_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
            tr("Выберите файл с логином и пароль от электонной почты"), "",
            tr("Текстовый файл (*.txt)"));

    ui->lineEdit_emailKey->setText(path);
}


void SendWidget::showErrorMessage(QString text)
{
    QMessageBox errorMessage(this);
    errorMessage.setInformativeText(text);
    errorMessage.setText("Ошибка!");
    errorMessage.exec();
}



void SendWidget::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
}



void SendWidget::on_pushButton_startSend_clicked()
{
    QString subject = ui->lineEdit_subject->text();
    if(subject == "")
    {
        showErrorMessage("Отсутствует тема письма!");
        return;
    }

    ///////////////////////////////////////////////////////////////////////
    QString path_attachmentsDir = ui->lineEdit_attachments->text();
    QDir attachmentsDir = QDir(path_attachmentsDir);
    if(!attachmentsDir.exists())
    {
        showErrorMessage("Папка с приложениями указана неверно!");
        return;
    }
    QFileInfoList attachments_file_list = attachmentsDir.entryInfoList(QDir::Files);
    QStringList attachments_list;
    for(auto attachment_file: attachments_file_list)
    {
        attachments_list.append(attachment_file.absoluteFilePath());
    }
    qInfo("Attachments(%d) getted successfuly", attachments_list.length());

    ///////////////////////////////////////////////////////////////////////
    QString path_personalAttachmentsDir = ui->lineEdit_personalAttachments->text();
    QDir personalAttachmentsDir = QDir(path_personalAttachmentsDir);
    if(!attachmentsDir.exists())
    {
        showErrorMessage("Папка с работами участников указана неверно!");
        return;
    }
    QFileInfoList personalAttachments_list = personalAttachmentsDir.entryInfoList(QDir::Files);
    qInfo("Personal attachments(%d) getted successfuly", personalAttachments_list.length());

    ///////////////////////////////////////////////////////////////////////
    QString path_emailKey = ui->lineEdit_emailKey->text();
    if(!QFile(path_emailKey).exists())
    {
        showErrorMessage("Файл с ключами от почты указан неверно!");
        return;
    }
    QFile File_emailKey(path_emailKey);
    QString email_server;
    int port_server;
    QString email_login;
    QString email_password;
    if (File_emailKey.open(QIODevice::ReadOnly))
    {
       QTextStream in(&File_emailKey);
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 0, а не 4!");
           return;
       }
       email_server = in.readLine();
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 1, а не 4!");
           return;
       }
       bool ok;
       port_server = in.readLine().toInt(&ok);
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 2, а не 4!");
           return;
       }
       if(!ok){
           showErrorMessage("Формат файла с ключем неверен, порт должен являться числом!");
           return;
       }
       email_login = in.readLine();
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 3, а не 4!");
           return;
       }
       email_password = in.readLine();
       File_emailKey.close();
    }
    else
    {
        showErrorMessage("Ошибка при чтении файла с ключами от почты!");
        return;
    }
    qInfo("Email key getted successfuly:");
    qInfo("Server: %s", email_server.toLocal8Bit().data());
    qInfo("Port: %d", port_server);
    qInfo("Login: %s", email_login.toLocal8Bit().data());
    qInfo("Password: %s", email_password.toLocal8Bit().data());

    ///////////////////////////////////////////////////////////////////////
    QString path_emailBody = ui->lineEdit_emailBody->text();
    QFile file_emailBody(path_emailBody);
    if(!file_emailBody.exists())
    {
        showErrorMessage("Файл с телом письма указан неверно!");
        return;
    }
    QString emailBodyText = "";
    if(file_emailBody.open(QIODevice::ReadOnly))
    {
        QTextStream stream_emailBody(&file_emailBody);
        emailBodyText = stream_emailBody.readAll();
    }
    else
    {
        showErrorMessage("Ошибка при чтении файла с телом письма!");
        return;
    }
    file_emailBody.close();
    qInfo("Email body getted successfuly");

    ///////////////////////////////////////////////////////////////////////
    QString path_emailBase = ui->lineEdit_emailBase->text();
    if(!QFile(path_emailBody).exists())
    {
        showErrorMessage("Файл с адресами почт участников указан неверно!");
        return;
    }
    QVector<Participant> participant_email_list;
    QFile file_emailBase(path_emailBase);
    if(file_emailBase.open(QIODevice::ReadOnly))
    {
        QTextStream stream_emailBase(&file_emailBase);
        while(!stream_emailBase.atEnd())
        {
            QString line = remove_yo(stream_emailBase.readLine());
            QStringList splited_line = line.split(QRegExp("[ \t]"),QString::SkipEmptyParts);
            if(splited_line.length() < 3)
            {
                continue;
            }
            participant_email_list.append(Participant(splited_line[1], splited_line[2], "0", splited_line[0]));

        }
    }
    else
    {
        showErrorMessage("Ошибка при чтении файла с адресами почт участников!");
        return;
    }
    file_emailBase.close();
    qInfo("Participant emails(%d) getted successfuly", participant_email_list.length());

    ///////////////////////////////////////////////////////////////////////
    QString participants_text = remove_yo(ui->textEdit_participants->toPlainText());
    if(participants_text == "")
    {
        showErrorMessage("Отсутствует участники олимпиады!");
        return;
    }
    QStringList participant_text_splited = participants_text.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    qInfo("Participant names(%d) getted successfuly", participant_text_splited.length());

    ///////////////////////////////////////////////////////////////////////
    QVector<Participant> participant_list;
    QVector<QStringList> personalAttrachments_byPaticipant_list;
    QVector<QStringList> emails_byPaticipant_list;
    for(int index_of_participant = 0; index_of_participant < participant_text_splited.length(); index_of_participant++)
    {
        QString participant_fio = participant_text_splited[index_of_participant];

        QStringList splited_fio = participant_fio.split(QRegExp("[ \t]"),QString::SkipEmptyParts);

        if(splited_fio.length() < 2)
        {
            showErrorMessage("Неправильно указано имя одного или нескольких участников!");
            return;
        }

        QStringList personalAttachments_forPaticipant;

        for(auto file: personalAttachments_list)
        {
            if(QRegExp("^" + splited_fio.at(0) + "_" + splited_fio.at(1) + "*").indexIn(file.fileName()) != -1)
            {
                personalAttachments_forPaticipant.append(file.absoluteFilePath());
                qInfo("%s", file.fileName().toLocal8Bit().data());
                qInfo(("^" + splited_fio.at(0) + "_" + splited_fio.at(1) + "*").toLocal8Bit().data());
                qInfo("%d", QRegExp("^" + splited_fio.at(0) + "_" + splited_fio.at(1) + "*").indexIn(file.fileName()));
            }
        }

        personalAttrachments_byPaticipant_list.append(personalAttachments_forPaticipant);

        if(personalAttachments_forPaticipant.size() == 0)
        {
            showErrorMessage("У одного или нескольких участников нет работы в папке с работами!");
            return;
        }

        participant_list.append(Participant(splited_fio.at(0), splited_fio.at(1)));

        QSet<QString> emails_list;

        for(auto participant_with_email: participant_email_list)
        {

            if(splited_fio[0] == participant_with_email.lastName && splited_fio[1] == participant_with_email.firstName)
            {
                emails_list.insert(participant_with_email.email);
                qInfo("Email Sending to %s", participant_with_email.email.toLocal8Bit().data());
            }
        }
        emails_byPaticipant_list.append(emails_list.toList());
    }
    qInfo("ParticipantInfo merged successfuly");

    ///////////////////////////////////////////////////////////////////////
    QProgressDialog progressDialog(this);
    progressDialog.setLabelText("Отправка");
    progressDialog.setMinimum(0);
    progressDialog.setMaximum(participant_list.size());
    progressDialog.show();
    for(int participant_index = 0; participant_index < participant_list.size(); participant_index++)
    {
        progressDialog.setValue(participant_index);

        qInfo("Sending to %s", participant_list[participant_index].lastName.toLocal8Bit().data());

        for(auto email: emails_byPaticipant_list[participant_index])
        {
            qInfo("Sending email to %s", email.toLocal8Bit().data());
            Smtp* smtp = new Smtp(email_login, email_password, email_server, port_server);
            connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
            QStringList files = attachments_list + personalAttrachments_byPaticipant_list[participant_index];
            smtp->sendMail(email_login, email, subject, emailBodyText, files );

        }

    }
    progressDialog.setValue(participant_list.length());
    qInfo("All emails sended successfuly");

    ui->lineEdit_protocol->setText(subject);
}

/*
void SendWidget::on_pushButton_upload_clicked()
{
    this->close();
    emit mainWindow();
}


void SendWidget::on_pushButton_protocol_clicked()
{
    QString path_protocol = QFileDialog::getOpenFileName(this,
            tr("Выберите протокол"), "",
            tr("Книга Microsoft Excel (*.xlsx)"));

    ui->lineEdit_protocol->setText(path_protocol);
}
*/
