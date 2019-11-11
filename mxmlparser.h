#ifndef MXMLPARSER_H
#define MXMLPARSER_H
//解决乱码
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QtXml>
#include<QString>
#include <QFile>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QHash>
#include<QVector>
#if defined(Q_OS_LINUX)
#include<xls.h>
#endif
class ExcelParser : public QObject
{
public:
    explicit ExcelParser(QObject *p);
    /*界面两个按钮主函数--自定义转化和一键转化*/
    bool excelToTs(QString type);
    bool excelAllToTs(QString ts_dir_path);

    /*获取ts目录或者路径，获取excel文件的路径*/
    QString getTsDir() const;
    void setTsDir(const QString ts_dir);
    QString getTsPath() const;
    void setTsPath(const QString ts_path);
    QString getExcelPath() const;
    void setExcelPath(const QString excel_path);
    QString readConfigType(QString file_name,QString mode);
private:

    bool readExcelToHash(QString mode);//把Excel的Key读到QHash中
    bool updateTs();//更新ts文件主函数
    QString findValue(QString key);//寻找QHash中key对应的value

    bool openTsFile(QString file_path);//把xml结构写到doc内存中
    void writeDoc(QDomNode&tran_node);//把节点信息更新到doc中
    bool closeTsFile(QString file_path);//把dox内存的信息写到文件

    void writeFailData(QString fail_file_path);
    QDomDocument doc_;
    QString tsdir_;
    QString tspath_;
    QString excelpath_;
    QHash<QString,QString> alldata_; /*存放excel文件的key和value*/
    QVector<QString>lack_;/*存放没有匹配的ts文件的key*/
    QString tempvalue_;/*保存临时写入doc的value*/
};

#endif // MXMLPARSER_H
