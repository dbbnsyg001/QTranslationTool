#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mxmlparser.h"
//指定UTF-8解决中文乱码
#pragma execution_character_set("utf-8")
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

private slots:
    //选择ts目录按钮 槽函数
    void on_openTsDirButton_clicked();
    //打开ts文件按钮 槽函数
    void on_openTsFileButton_clicked();
    //打开execl文件按钮 槽函数
    void on_openExcelFileButton_clicked();
    //自定路径转化->翻译 槽函数
    void on_customTransButton_clicked();
    //默认路径转化->翻译 槽函数
    void on_allTransButton_clicked();
    //下拉框变化->翻译语言 槽函数
    void on_languageComboBox_currentIndexChanged(const QString &arg);

private:
    Ui::MainWindow *ui;
    ExcelParser *parser_;
    QString languagetype_;
};

#endif // MAINWINDOW_H
