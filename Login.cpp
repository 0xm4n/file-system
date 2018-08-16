#include "Login.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void login()
{
	struct user_account
	{
		string username;
		string password;
	};
	user_account login_user;

	string temp_username;
	char *temp_password = new char[20];

	cout << "    用户名：";
	cin>>temp_username;
	cout << "    密码：";
	cin >> temp_password;

	ifstream fin;
	fin.open("account.txt", ios_base::in);

	if (fin.fail())
	{
		cout << "    无法读取后台csv文件，登录失败！";
	}


	int flag = 0;
	while (fin >> login_user.username >> login_user.password)
	{
		if (login_user.username == temp_username && login_user.password == temp_password)
		{
			cout << endl;
			cout << "    登陆成功!" << endl<<endl;
			flag = 1;//登录成功
			show_menu();
			break;
		}
	}
	if (flag == 0)//登录失败
	{
		cout << endl;
		cout << "    用户名或密码错误" << endl;
		login();
	}
}