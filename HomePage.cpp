#include "HomePage.h"
#include "Login.h"
#include "Regist.h"
#include <iostream>
using namespace std;

void showHome()
{
	int temp;//ʶ���û�����Ĳ���
	cout << endl;
	cout << "     =====================================" << endl;
	cout << "    ||     ��ӭʹ�ö���Ŀ¼�ļ�ϵͳ      ||" << endl;
	cout << "     =====================================" << endl;
	cout << "    ||       ��¼      |      ����1      || " << endl;
	cout << "    ||-----------------|-----------------|| " << endl;
	cout << "    ||       ע��      |      ����2      || " << endl;
	cout << "     =====================================" << endl;
	cout << endl << "    ��������Ҫ���еĲ�����";
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
		cout << "    ���������������룡";
		showHome();
	}
}
