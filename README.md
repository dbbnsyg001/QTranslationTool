程序的readExcelToHash()函数分为Linux和Windows下的版本

Windows版本---使用ODBC连接Excel
安装说明：1.安装驱动文件夹下的AccessDatabaseEngine_X64.exe（对应平台
	 2.将配置文件config.ini放到和程序同一级目录
	 3.程序运行

Linux版本-------使用xlslib连接Excel
安装说明：1.先安装libiconv-dev，否则中文显示可能不正常。安装步骤见下面步骤
	 2.安装xlslib
	 3.将配置文件config.ini放到和程序同一级目录
	 4.程序运行

Linux组件安装步骤：
一.libiconv-dev：
	1.下载源码wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.13.1.tar.gz
	2.解压源码，./configure -prefix xxxx    make    make install安装
	3.将对应安装路径的lib创建软链接到/usr/local/lib
二.xlslib
	1.网址下载源码https://sourceforge.net/projects/libxls/
	2.解压源码，./configure -prefix xxxx    make    make install安装
	3.项目pro配置中添加，LIBS += -L/usr/local/lib -lxlsreader
