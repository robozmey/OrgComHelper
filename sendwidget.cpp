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
            tr("Выберите тело письма"), "",
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
    qInfo(text.toLocal8Bit().data());
    errorMessage.setInformativeText(text);
    errorMessage.setText("Ошибка!");
    errorMessage.exec();
}



void SendWidget::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
}





QString SendWidget::get_subject(bool &isNotOK)
{
    QString subject = ui->lineEdit_subject->text();
    if(subject == "")
    {
        showErrorMessage("Отсутствует тема письма!");
        isNotOK = true;
        return QString();
    }
    return subject;
}


SmtpInfo SendWidget::get_smtpInfo(bool &isNotOK)
{
    QString path_emailKey = ui->lineEdit_emailKey->text();
    if(!QFile(path_emailKey).exists())
    {
        showErrorMessage("Файл с ключами от почты указан неверно!");
        isNotOK = true;
        return SmtpInfo();
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
           isNotOK = true;
           return SmtpInfo();
       }
       email_server = in.readLine();
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 1, а не 4!");
           isNotOK = true;
           return SmtpInfo();
       }
       bool ok;
       port_server = in.readLine().toInt(&ok);
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 2, а не 4!");
           isNotOK = true;
           return SmtpInfo();
       }
       if(!ok){
           showErrorMessage("Формат файла с ключем неверен, порт должен являться числом!");
           isNotOK = true;
           return SmtpInfo();
       }
       email_login = in.readLine();
       if(in.atEnd()){
           showErrorMessage("Формат файла с ключем неверен, строк 3, а не 4!");
           isNotOK = true;
           return SmtpInfo();
       }
       email_password = in.readLine();
       File_emailKey.close();
    }
    else
    {
        showErrorMessage("Ошибка при чтении файла с ключами от почты!");
        isNotOK = true;
        return SmtpInfo();
    }
    qInfo("Email key getted successfuly:");
    qInfo("Server: %s", email_server.toLocal8Bit().data());
    qInfo("Port: %d", port_server);
    qInfo("Login: %s", email_login.toLocal8Bit().data());
    qInfo("Password: %s", email_password.toLocal8Bit().data());

    return SmtpInfo(email_server, port_server, email_login, email_password);
}


QStringList SendWidget::get_attachments(bool &isNotOK)
{
    QString path_attachmentsDir = ui->lineEdit_attachments->text();
    QDir attachmentsDir = QDir(path_attachmentsDir);
    if(!attachmentsDir.exists())
    {
        showErrorMessage("Папка с приложениями указана неверно!");
        isNotOK = true;
        return QStringList();
    }
    QFileInfoList attachments_file_list = attachmentsDir.entryInfoList(QDir::Files);
    QStringList attachments_list;
    for(auto attachment_file: attachments_file_list)
    {
        attachments_list.append(attachment_file.absoluteFilePath());
    }
    qInfo("Attachments(%d) getted successfuly", attachments_list.length());

    return attachments_list;
}


QFileInfoList SendWidget::get_personalAtteachments(bool &isNotOK)
{
    QString path_personalAttachmentsDir = ui->lineEdit_personalAttachments->text();
    QDir personalAttachmentsDir = QDir(path_personalAttachmentsDir);
    if(!personalAttachmentsDir.exists())
    {
        showErrorMessage("Папка с работами участников указана неверно!");
        isNotOK = true;
        return QFileInfoList();
    }
    QFileInfoList personalAttachments_list = personalAttachmentsDir.entryInfoList(QDir::Files);
    qInfo("Personal attachments(%d) getted successfuly", personalAttachments_list.length());

    return personalAttachments_list;
}


QString SendWidget::get_emailBodyText(bool &isNotOK)
{
    QString path_emailBody = ui->lineEdit_emailBody->text();
    QFile file_emailBody(path_emailBody);
    if(!file_emailBody.exists())
    {
        showErrorMessage("Файл с телом письма указан неверно!");
        isNotOK = true;
        return QString();
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
        isNotOK = true;
        return QString();
    }
    file_emailBody.close();
    qInfo("Email body getted successfuly");
    return emailBodyText;
}


QVector<Participant> SendWidget::get_participant_email_list(bool &isNotOK)
{
    QString path_emailBase = ui->lineEdit_emailBase->text();
    if(!QFile(path_emailBase).exists())
    {
        showErrorMessage("Файл с адресами почт участников указан неверно!");
        isNotOK = true;
        return  QVector<Participant>();
    }
    QVector<Participant> participant_email_list;
    QFile file_emailBase(path_emailBase);
    QString tmp = "";
    if(file_emailBase.open(QIODevice::ReadOnly))
    {
        QTextStream stream_emailBase(&file_emailBase);
        while(!stream_emailBase.atEnd())
        {
            QString line = remove_yo(QString::fromUtf8(stream_emailBase.readLine().toLocal8Bit()));
            tmp += line + "\n";
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
        isNotOK = true;
        return  QVector<Participant>();
    }
    file_emailBase.close();
    qInfo("Participant emails(%d) getted successfuly", participant_email_list.length());
    return participant_email_list;
}

QStringList SendWidget::get_participant_text_splited(bool &isNotOK)
{
    QString participants_text = remove_yo(ui->textEdit_participants->toPlainText());
    if(participants_text == "")
    {
        showErrorMessage("Отсутствует участники олимпиады!");
        isNotOK = true;
        return QStringList();
    }
    QStringList participant_text_splited = participants_text.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    qInfo("Participant names(%d) getted successfuly", participant_text_splited.length());

    return participant_text_splited;
}

std::tuple<QVector<Participant>, QVector<QStringList>, QVector<QStringList>> SendWidget::get_mergedInfoToSend(QStringList participant_text_splited, QFileInfoList personalAttachments_list, QVector<Participant> participant_email_list, bool &isNotOK)
{
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
            isNotOK = true;
            return std::make_tuple(participant_list, personalAttrachments_byPaticipant_list, emails_byPaticipant_list);
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
            isNotOK = true;
            return std::make_tuple(participant_list, personalAttrachments_byPaticipant_list, emails_byPaticipant_list);
        }

        participant_list.append(Participant(splited_fio.at(0), splited_fio.at(1)));

        QSet<QString> emails_list;

        for(auto participant_with_email: participant_email_list)
        {

            if(splited_fio[0] == participant_with_email.lastName && splited_fio[1] == participant_with_email.firstName)
            {
                emails_list.insert(participant_with_email.email);
                qInfo("Email will send to %s", participant_with_email.email.toLocal8Bit().data());
            }
        }
        emails_byPaticipant_list.append(emails_list.toList());
    }
    qInfo("ParticipantInfo merged successfuly");

    return std::make_tuple(participant_list, personalAttrachments_byPaticipant_list, emails_byPaticipant_list);
}

void SendWidget::send_emails(QVector<Participant> participant_list, QVector<QStringList> emails_byPaticipant_list, QStringList attachments_list, QVector<QStringList> personalAttrachments_byPaticipant_list, SmtpInfo smtpInfo, QString subject, QString emailBodyText)
{
    QProgressDialog progressDialog(this);
    progressDialog.setLabelText("Отправка");
    progressDialog.setMinimum(0);
    progressDialog.setMaximum(participant_list.size());
    progressDialog.show();

    for(int participant_index = 0; participant_index < participant_list.size(); participant_index++)
    {
        progressDialog.setValue(participant_index);

        qInfo() << "Sending emails for "<< participant_index << participant_list[participant_index].lastName.toLatin1() << participant_list[participant_index].firstName.toLatin1();

        for(auto email: emails_byPaticipant_list[participant_index])
        {

            qInfo("Sending email to %s", email.toLocal8Bit().data());



            // Create a MimeMessage

            MimeMessage message;

            EmailAddress sender(smtpInfo.login, smtpInfo.login);
            message.setSender(&sender);

            EmailAddress to(email, email);
            message.addRecipient(&to);

            message.setSubject(subject);

            // Add some text
            MimeHtml html;
            html.setHtml(emailBodyText.toLocal8Bit());
            message.addPart(&html);

            for(auto filename: attachments_list + personalAttrachments_byPaticipant_list[participant_index])
            {
                message.addPart(new MimeAttachment(new QFile(filename)));
            } //*/

            // Now we create the attachment object


            SmtpClient smtp(smtpInfo.server, smtpInfo.port, SmtpClient::SslConnection);

            smtp.setUser(smtpInfo.login);
            smtp.setPassword(smtpInfo.password);

            if (!smtp.connectToHost()) {
                showErrorMessage("Failed to connect to host!");
                progressDialog.deleteLater();
                return;
            }

            //qInfo("Succesfully connected to smtp server");

            if (!smtp.login()) {
                showErrorMessage("Failed to login!");
                progressDialog.deleteLater();
                return;
            }

            //qInfo("Succesfully logged in to smtp server");

            if (!smtp.sendMail(message)) {
               // showErrorMessage("Failed to send mail!");
               // return;
              // QThread::msleep(100);
            }

            //qInfo("Succesfully sended email");



            smtp.quit();

            /*
            Smtp* smtp = new Smtp(email_login, email_password, email_server, port_server);
            connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
            QStringList files = attachments_list + personalAttrachments_byPaticipant_list[participant_index];
            smtp->sendMail(email_login, email, subject, emailBodyText, files );
            while(smtp->state != 0){
                qInfo("%d", smtp->state);
                QThread::msleep(1000);
            }
            //*/
        }

    }


    progressDialog.setValue(participant_list.length());

    progressDialog.deleteLater();

    qInfo("All emails sended successfuly");
}

void SendWidget::on_pushButton_startSend_clicked()
{
    bool isNotOK = false;

    QString subject = get_subject(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    QStringList attachments_list = get_attachments(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    QFileInfoList personalAttachments_list = get_personalAtteachments(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    SmtpInfo smtpInfo = get_smtpInfo(isNotOK);

    ///////////////////////////////////////////////////////////////////////
    QString emailBodyText = get_emailBodyText(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    QVector<Participant> participant_email_list = get_participant_email_list(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    QStringList participant_text_splited = get_participant_text_splited(isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    QVector<Participant> participant_list;
    QVector<QStringList> personalAttrachments_byPaticipant_list;
    QVector<QStringList> emails_byPaticipant_list;
    std::tie(participant_list, personalAttrachments_byPaticipant_list, emails_byPaticipant_list) = get_mergedInfoToSend(participant_text_splited, personalAttachments_list, participant_email_list, isNotOK);
    if(isNotOK) return;

    ///////////////////////////////////////////////////////////////////////
    send_emails(participant_list, emails_byPaticipant_list,attachments_list, personalAttrachments_byPaticipant_list, smtpInfo, subject, emailBodyText);
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
