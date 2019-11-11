#include "mxmlparser.h"

ExcelParser::ExcelParser(QObject *p):QObject(p)
{

}

bool ExcelParser::excelToTs(QString type)
{
    //数据库读数据到QHash
   if(!readExcelToHash(type))return false;
    //    for(auto iter=alldata_.begin();iter!=alldata_.end();++iter)
    //    {
    //        qDebug()<<iter.key()<<"value="<<iter.value();
    //    }
    //QHsh写到ts文件
    if(!updateTs())return false;
    return true;
}

bool ExcelParser::excelAllToTs(QString ts_dir_path)
{
    QDir dir(ts_dir_path);
    QStringList nameFilters;
    nameFilters << "*.ts";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    for(int i=0;i<files.count();++i)
    {
        QString type=readConfigType(files.at(i),"languageType");
        //qDebug()<<"type="<<type<<"excelPath="<<getExcelPath();
        setTsPath(ts_dir_path+"/"+files.at(i));
        qDebug()<<"当前路径:"<<getTsPath();
        if(type==" ")
        {
            continue;
        }
        if(!excelToTs(type))
        {
            qDebug()<<type<<"转化失败,请检查配置文件或代码";
            continue;
        }
        qDebug()<<type<<"转化成功";
    }
    return  true;
}

QString ExcelParser::getTsDir() const
{
    return tsdir_;
}

void ExcelParser::setTsDir(const QString ts_dir)
{
    this->tsdir_=ts_dir;
}
QString ExcelParser::getTsPath() const
{
    return tspath_;
}

void ExcelParser::setTsPath(const QString ts_path)
{
    this->tspath_ = ts_path;
}

QString ExcelParser::getExcelPath() const
{
    return excelpath_;
}

void ExcelParser::setExcelPath(const QString excel_path)
{
    this->excelpath_ = excel_path;
}

QString ExcelParser::readConfigType(QString file_name,QString mode)
{
    QString type=" ";
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat,this);
    //统一utf-8防止中文乱码
    config->setIniCodec("UTF-8");
    if(config==nullptr)
    {
        qDebug()<<"file not exist!";
        return type;
    }
    type=config->value(mode+"/"+file_name).toString();
    //qDebug()<<type;
    return type;
}

#if defined(Q_OS_WIN32)
bool ExcelParser::readExcelToHash(QString type)
{
    //把上一次读取的全部清空
    alldata_.clear();
    lack_.clear();
    QString key,value;
    key="Key";
    value=type;
    QSqlDatabase db;
    //防止只能打开一次，old connection removed.
    if(!QSqlDatabase::contains("excelexport"))
    {
        db=QSqlDatabase::addDatabase("QODBC", "excelexport");
    }
    else {
        db=QSqlDatabase::addDatabase("QODBC", "excelexport2");
    }

    if (!db.isValid()) {
        qDebug() << "database is not valid!";
        return false;
    }
    QString dsn=QString("Driver={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};Readonly=TRUE;DBQ=%1").arg(this->getExcelPath());
    db.setDatabaseName(dsn);
    if (!db.open()) {
        qDebug()<<"failed in db.open()|"<< db.lastError().text();//关键
        return false;
    }
    //查询开始
    QSqlQuery query(db);
    QString command="select "+key+",["+value+"] from [Sheet0$]";
    qDebug()<<command;
    if(!query.exec(command))
    {
        qDebug()<<"failed in query.exec()|"<<query.lastError().text();//关键
        return false;
    }
    // 存放到QHash中
    while (query.next())
    {
        QString tempKey = query.value(0).toString();//读出来中文乱码出现处,已经解决，是sql语句未识别问题
        QString tempValue = query.value(1).toString();
        alldata_.insert(tempKey,tempValue);
    }
    //关闭
    query.clear();
    db.close();
    return true;
}
#endif

#if defined(Q_OS_LINUX)
bool ExcelParser::readExcelToHash(QString type)
{
    //把上一次读取的全部清空
    alldata_.clear();
    lack_.clear();
    int col=readConfigType(type,"excelRow").toInt();
    qDebug()<<"col->"<<col;
    xls::xlsWorkBook *pWb=nullptr;
    xls::xlsWorkSheet *pWs;
    xls::xlsCell * key=nullptr;
    xls::xlsCell * value=nullptr;
    int row;
    qDebug()<<getTsPath();
    qDebug()<<getExcelPath();
    std::string path= getExcelPath().toStdString();
    pWb = xls::xls_open(path.c_str(),"UTF-8");//这里如果打不开 ，在路径正确的同时，将文件另存一下，一定要保存为xls文件
    if (nullptr==pWb){
        fprintf(stderr, "File not found!\n");
        return false;
    }
    pWs = xls::xls_getWorkSheet(pWb, 0);//pWs指向第二个Sheet
    xls::xls_parseWorkSheet(pWs);

    for (row = 1; row < pWs->rows.lastrow; ++row)
    {
        key=xls::xls_cell(pWs, static_cast<xls::WORD>(row), static_cast<xls::WORD>(0));
        value = xls::xls_cell(pWs, static_cast<xls::WORD>(row), static_cast<xls::WORD>(col));
        // 判断单元格及内容是否为空,查阅资料后必须if判断，如果不判断可能会出现问题
        if (key && key->str&&value&&value->str) {
            //static_cast<char*>报错
            alldata_.insert(reinterpret_cast<char*>(key->str),reinterpret_cast<char*>(value->str));
        }
    }

    xls:: xls_close_WS(pWs);
    xls:: xls_close_WB(pWb);
    return true;
}
#endif
bool ExcelParser::updateTs()
{
    //清除上一次遗留的树结构
    doc_.clear();
    //打开文件
    if(!openTsFile(getTsPath()))
    {
        return false;
    }
    //修改保存xml
    QDomElement root = doc_.documentElement();
    QString value;
    QDomNode n = root.firstChild();
    //从根节点开始往下遍历
    while ( !n.isNull() )
    {
        QDomNodeList list = n.toElement().childNodes();
        for(int a=0; a<list.count(); a++) //遍历context列表
        {
            QDomNode node = list.at(a);
            if(node.isElement()&&node.nodeName() == "message" )
            {
                QDomNodeList tranlist=node.childNodes();
                for(int i=0;i<tranlist.count();i++)//遍历message列表
                {
                    QDomNode trannode = tranlist.at(i);
                    //写到doc中
                    writeDoc(trannode);
                }
            }
        }
        n = n.nextSibling();
    }
    //结束，并且把doc的内容写回文件中
    if(!closeTsFile(getTsPath()))
    {
        return false;
    }
    return true;
}

QString ExcelParser::findValue(QString key)
{
    QString value="";
    QHash<QString, QString>::const_iterator iter=alldata_.find(key);
    if(iter!=alldata_.end())
    {
        value=iter.value();
    }
    else
    {
        lack_.push_back(key);
    }
    return value;
}

bool ExcelParser::openTsFile(QString file_path)
{
    QFile file(file_path);

    if( !file.open( QFile::ReadOnly | QFile::Text  ) )
    {
        qDebug() << "failed in openTsFile()->file.open():" << file_path;
        return false;
    }
    //setContent时，默认只由空白组成的文本节点被剥离，<flag></flag>-><flag/>
    if( !doc_.setContent(&file))
    {
        qDebug() <<"failed in doc.setContent():"<< file_path;
        file.close();
        return false;
    }
    file.close();
    return true;
}

void  ExcelParser::writeDoc(QDomNode&tran_node)
{
    if(tran_node.isElement()&&tran_node.nodeName() == "source" )
    {
        //        qDebug()<<trannode.firstChild().nodeValue();重开函数value要放全局
        tempvalue_=findValue(tran_node.firstChild().nodeValue());
    }
    if(tran_node.isElement()&&tran_node.nodeName() == "translation")
    {
        if(tran_node.hasChildNodes())
        {
            //  trannode.firstChild().setNodeValue(value);
            QDomNode oldnode = tran_node.firstChild();
            tran_node.firstChild().setNodeValue(tempvalue_);
            QDomNode newnode = tran_node.firstChild();
            tran_node.replaceChild(newnode,oldnode);
        }
        else
        {
            //            QDomElement cfg = doc.createElement(" ");
            //            cfg.setNodeValue(value);
            //            trannode.appendChild(cfg);
            QDomText text;
            text = doc_.createTextNode(tempvalue_);
            tran_node.appendChild(text);
            text.clear();
        }
    }
}
bool  ExcelParser::closeTsFile(QString file_path)
{
    QFile filexml(file_path);
    if( !filexml.open( QFile::WriteOnly) ){
        qDebug("failed in closeTsFile()->file.open()\n");
        return false;
    }
    QTextStream ts(&filexml);
    doc_.save(ts, 4);
    filexml.close();

    //把lack中未匹配的输出到txt文件中
    QString failtxt=file_path+".txt";
    QFile failfile(failtxt);

    failfile.open(QIODevice::ReadWrite|QIODevice::Truncate);
    for(int i=0;i<lack_.size();++i)
    {
        //qDebug()<<lack_.at(i).toUtf8();
        failfile.write(lack_.at(i).toUtf8()+"\n");
    }
    failfile.close();
    return true;
}
