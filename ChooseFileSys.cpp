#include "ChooseFileSys.h"
#include "HomePage.h"
#include "MSDOS_filesys.h"
#include <iostream>
using namespace std;

void choose_fileSys() {
	int temp;//识别用户输入的操作
	cout << endl;
	cout << "     =================================================" << endl;
	cout << "    ||          欢迎使用模拟文件系统                 ||" << endl;
	cout << "     =================================================" << endl;
	cout << "    ||       二级目录文件系统      |      输入1      || " << endl;
	cout << "    ||-----------------------------|-----------------|| " << endl;
	cout << "    ||       MS-DOS文件系统        |      输入2      || " << endl;
	cout << "     =================================================" << endl;
	cout << endl << "    请输入需要使用的文件系统：";
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
		cout << "    输入出错请从新输入！";
		choose_fileSys();
	}
}