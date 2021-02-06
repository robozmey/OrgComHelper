#include "renamewidget.h"
#include "ui_renamewidget.h"

RenameWidget::RenameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenameWidget)
{
    ui->setupUi(this);
    ui->radioButton_without->setChecked(true);

}

RenameWidget::~RenameWidget()
{
    delete ui;
}

void RenameWidget::on_pushButton_upload_clicked()
{
    this->close();
    emit mainWindow();
}


void RenameWidget::on_pushButton_protocol_clicked()
{
    QString path_protocol = QFileDialog::getOpenFileName(this,
            tr("Выберите протокол"), "",
            tr("Книга Microsoft Excel (*.xlsx)"));

    ui->lineEdit_protocol->setText(path_protocol);
}

void RenameWidget::on_pushButton_namelessDir_clicked()
{
    QString path_nameless = QFileDialog::getExistingDirectory(this,
            tr("Выберите папку с безымянными работами"), "");

    ui->lineEdit_namelessDir->setText(path_nameless);
}

void RenameWidget::on_pushButton_distDir_clicked()
{
    QString path_dist = QFileDialog::getExistingDirectory(this,
            tr("Выберите папку для переименованных работ"), "");

    ui->lineEdit_distDir->setText(path_dist);
}



void RenameWidget::showErrorMessage(QString text)
{
    QMessageBox errorMessage(this);
    errorMessage.setInformativeText(text);
    errorMessage.setText("Ошибка!");
    errorMessage.exec();
}



void RenameWidget::on_pushButton_rename_clicked()
{
    QString participants_text = remove_yo(ui->textEdit_participant->toPlainText());
    QString grades_text = remove_yo(ui->textEdit_grades->toPlainText());

    QStringList participants_text_splited = participants_text.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    qInfo("Participant names(%d) getted successfuly", participants_text_splited.length());

    QStringList grades_list = grades_text.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    qInfo("Participant grades(%d) getted successfuly", grades_list.length());

    if(participants_text_splited.length() != grades_list.length())
    {
        showErrorMessage("Количество учеников не соответствует количеству их классов!");
        return;
    }


    QVector<Participant> participant_list;
    for(int index_of_participant = 0; index_of_participant < participants_text_splited.length(); index_of_participant++)
    {
        QString participant_fio = participants_text_splited[index_of_participant];
        QString grade = grades_list[index_of_participant];

        QStringList splited_fio = participant_fio.split(QRegExp("[ \t]"),QString::SkipEmptyParts);

        if(splited_fio.length() < 2)
        {
            showErrorMessage("Неправильно указано имя одного или нескольких участников!");
            return;
        }

        participant_list.append(Participant(splited_fio.at(0), splited_fio.at(1), grade));
    }
    qInfo("ParticipantInfo merged successfuly");

    QString suffix = "";
    if(ui->radioButton_1Typ->isChecked())
    {
        suffix = "_1тур";
    }
    else if(ui->radioButton_2Typ->isChecked())
    {
        suffix = "_2тур";
    }
    else if(ui->radioButton_other->isChecked())
    {
        suffix = "_" + ui->lineEdit_suffix->text();
    }
    qInfo("Suffix(%s) getted successfuly", suffix.toLocal8Bit().data());

    QList<QString> newFileNames;
    for(auto participant: participant_list)
    {
        newFileNames.append(participant.lastName + "_" + participant.firstName + "_" + participant.grade + "кл" + suffix + ".pdf");
    }
    qInfo("New names(%d) generated successfuly", newFileNames.length());

    QString dirName_namelessDir = ui->lineEdit_namelessDir->text();
    QString dirName_distDir = ui->lineEdit_distDir->text();

    QDir namelessDir = QDir(dirName_namelessDir);
    if(!namelessDir.exists())
    {
        showErrorMessage("Папка с безымянными работами указана неверно!");
        return;
    }

    QDir distDir = QDir(dirName_distDir);
    if(!distDir.exists())
    {
        showErrorMessage("Папка для переименованных работ указана неверно!");
        return;
    }

    QFileInfoList fileName_list = namelessDir.entryInfoList(QDir::Files);
    if(participants_text_splited.length() != fileName_list.length())
    {
        showErrorMessage("Количество файлов не соответствует количеству учеников!");
        return;
    }
    qInfo("Nameless files(%d) getted successfuly", fileName_list.length());

    QProgressDialog progressDialog(this);
    progressDialog.setLabelText("Переименовывание");
    progressDialog.setMinimum(0);
    progressDialog.setMaximum(newFileNames.length());
    progressDialog.show();

    for(int index_of_file = 0; index_of_file < newFileNames.length(); index_of_file++)
    {
        progressDialog.setValue(index_of_file);
        QString oldFileName = fileName_list[index_of_file].absoluteFilePath();
        QString newFileName = dirName_distDir + "/" + newFileNames[index_of_file];

        qInfo("%s -> %s", oldFileName.toLocal8Bit().data(), newFileName.toLocal8Bit().data());

        if (QFile::exists(newFileName))
        {
            QFile::remove(newFileName);
        }

        if(!QFile::copy(oldFileName, newFileName))
        {
            showErrorMessage("При переименовывании произоошла ошибка!");
        }
    }
    progressDialog.setValue(newFileNames.length());
    qInfo("All files renamed successfuly");

}

