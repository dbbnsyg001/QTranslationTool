�����readExcelToHash()������ΪLinux��Windows�µİ汾

Windows�汾---ʹ��ODBC����Excel
��װ˵����1.��װ�����ļ����µ�AccessDatabaseEngine_X64.exe����Ӧƽ̨
	 2.�������ļ�config.ini�ŵ��ͳ���ͬһ��Ŀ¼
	 3.��������

Linux�汾-------ʹ��xlslib����Excel
��װ˵����1.�Ȱ�װlibiconv-dev������������ʾ���ܲ���������װ��������沽��
	 2.��װxlslib
	 3.�������ļ�config.ini�ŵ��ͳ���ͬһ��Ŀ¼
	 4.��������

Linux�����װ���裺
һ.libiconv-dev��
	1.����Դ��wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.13.1.tar.gz
	2.��ѹԴ�룬./configure -prefix xxxx    make    make install��װ
	3.����Ӧ��װ·����lib���������ӵ�/usr/local/lib
��.xlslib
	1.��ַ����Դ��https://sourceforge.net/projects/libxls/
	2.��ѹԴ�룬./configure -prefix xxxx    make    make install��װ
	3.��Ŀpro��������ӣ�LIBS += -L/usr/local/lib -lxlsreader
