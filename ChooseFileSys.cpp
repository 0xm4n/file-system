#include "ChooseFileSys.h"
#include "HomePage.h"
#include "MSDOS_filesys.h"
#include <iostream>
using namespace std;

void choose_fileSys() {
	int temp;//ʶ���û�����Ĳ���
	cout << endl;
	cout << "     =================================================" << endl;
	cout << "    ||          ��ӭʹ��ģ���ļ�ϵͳ                 ||" << endl;
	cout << "     =================================================" << endl;
	cout << "    ||       ����Ŀ¼�ļ�ϵͳ      |      ����1      || " << endl;
	cout << "    ||-----------------------------|-----------------|| " << endl;
	cout << "    ||       MS-DOS�ļ�ϵͳ        |      ����2      || " << endl;
	cout << "     =================================================" << endl;
	cout << endl << "    ��������Ҫʹ�õ��ļ�ϵͳ��";
	cin >> temp;
	cout << endl;
	if (temp == 1)
	{
		showHome();
	}
	else if (temp == 2)
	{
		runMsdosFileSystem();
	}
	else
	{
		cout << "    ���������������룡";
		choose_fileSys();
	}
}