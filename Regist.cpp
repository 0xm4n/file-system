#include "Regist.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void regist()
{
	struct user_account
	{
		string username;
		string password;
	};//定义用户结构体
	user_account user;

	string username;
	char *password = new char[20];



	cout << "    请输入用户名：";
	cin >> username;

	ifstream fin;//声明 ifstream 对象
	fin.open("accout.txt", ios::in);

	while ( fin >> user.username >> user.password )
	{
		cout << user.username << " " << user.password << endl;
		if (user.username == username)
		{
			cout << "    该用户名已被注册"<<endl;regist();
		}
	}

	cout << "    输入密码:";
	cin >> password;

	ofstream fout;
	fout.open("account.txt", ios_base::out | ios_base::app);
	fout << username << " " << password << endl;
	fout.close();
	cout << endl;
	cout << "    创建成功！" << endl;
	cout << endl;

}