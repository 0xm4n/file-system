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

	cout << "    �û�����";
	cin>>temp_username;
	cout << "    ���룺";
	cin >> temp_password;

	ifstream fin;
	fin.open("account.txt", ios_base::in);

	if (fin.fail())
	{
		cout << "    �޷���ȡ��̨csv�ļ�����¼ʧ�ܣ�";
	}


	int flag = 0;
	while (fin >> login_user.username >> login_user.password)
	{
		if (login_user.username == temp_username && login_user.password == temp_password)
		{
			cout << endl;
			cout << "    ��½�ɹ�!" << endl<<endl;
			flag = 1;//��¼�ɹ�
			show_menu();
			break;
		}
	}
	if (flag == 0)//��¼ʧ��
	{
		cout << endl;
		cout << "    �û������������" << endl;
		login();
	}
}