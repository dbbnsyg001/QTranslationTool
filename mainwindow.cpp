#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parser_=new ExcelParser(this);
    this->setWindowTitle("execl转化ts---翻译工具");
    languagetype_=parser_->readConfigType(ui->languageComboBox->currentText(),"comBoxLanguageType");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openTsDirButton_clicked()
{
    //文件夹路径
    QString tsDirPath = QFileDialog::getExistingDirectory(this, "选择ts目录","./");
    if (tsDirPath.isEmpty())
    {
        qDebug()<<"目录为空";
        return;
    }
    else
    {
        parser_->setTsDir(tsDirPath);
        ui->tsDirLineEdit->setText(parser_->getTsDir());
    }
}

void MainWindow::on_openTsFileButton_clicked()
{
    parser_->setTsPath(QFileDialog::getOpenFileName(this, "打开TS文件", "./","TS(*.ts)"));
    ui->tsLineEdit->setText(parser_->getTsPath());
}

void MainWindow::on_openExcelFileButton_clicked()
{
    parser_->setExcelPath(QFileDialog::getOpenFileName(this, "打开EXCEL文件","./","EXCEL(*.xls)"));
    ui->excelLineEdit->setText(parser_->getExcelPath());
}

void MainWindow::on_customTransButton_clicked()
{
    if(ui->excelLineEdit->text().isEmpty()||ui->tsLineEdit->text().isEmpty())
    {
        ui->statusLabel->setText("自定义路径转化失败,路径为空");
        return;
    }
    ui->statusLabel->setText("自定义路径转化开始");
    if(!parser_->excelToTs(this->languagetype_))
    {
        ui->statusLabel->setText("自定义路径转化失败");
        return;
    }
    ui->statusLabel->setText("自定义路径转化成功");
}

void MainWindow::on_allTransButton_clicked()
{
    if(ui->excelLineEdit->text().isEmpty()||ui->tsDirLineEdit->text().isEmpty())
    {
        ui->statusLabel->setText("文件夹转化失败,路径为空");
        return;
    }
    ui->statusLabel->setText("文件夹转化开始");
    if(!parser_->excelAllToTs(parser_->getTsDir()))
    {
        ui->statusLabel->setText("文件夹转化失败");
        return;
    }
    ui->statusLabel->setText("文件夹转化成功");
}

void MainWindow::on_languageComboBox_currentIndexChanged(const QString &arg)
{
    languagetype_=parser_->readConfigType(arg,"comBoxLanguageType");
}

