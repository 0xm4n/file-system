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
	};//�����û��ṹ��
	user_account user;

	string username;
	char *password = new char[20];



	cout << "    �������û�����";
	cin >> username;

	ifstream fin;//���� ifstream ����
	fin.open("accout.txt", ios::in);

	while ( fin >> user.username >> user.password )
	{
		cout << user.username << " " << user.password << endl;
		if (user.username == username)
		{
			cout << "    ���û����ѱ�ע��"<<endl;regist();
		}
	}

	cout << "    ��������:";
	cin >> password;

	ofstream fout;
	fout.open("account.txt", ios_base::out | ios_base::app);
	fout << username << " " << password << endl;
	fout.close();
	cout << endl;
	cout << "    �����ɹ���" << endl;
	cout << endl;

}