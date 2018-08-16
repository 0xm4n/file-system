#include "HomePage.h"
#include "Login.h"
#include "Regist.h"
#include <iostream>
using namespace std;

void showHome()
{
	int temp;//识别用户输入的操作
	cout << endl;
	cout << "     =====================================" << endl;
	cout << "    ||     欢迎使用二级目录文件系统      ||" << endl;
	cout << "     =====================================" << endl;
	cout << "    ||       登录      |      输入1      || " << endl;
	cout << "    ||-----------------|-----------------|| " << endl;
	cout << "    ||       注册      |      输入2      || " << endl;
	cout << "     =====================================" << endl;
	cout << endl << "    请输入需要进行的操作：";
	cin >> temp;
	cout << endl;
	if (temp == 1)
	{
		login();
	}
	else if (temp == 2)
	{
		regist();
	}
	else
	{
		cout << "    输入出错请从新输入！";
		showHome();
	}
}
