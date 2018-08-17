#include "hier_filesys.h"
#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
using namespace std;

typedef struct uf              //�ļ���ṹ��  
{
	string fileName;
	int protectType;           //�ļ�������ʽ
	int current_length;         //�ļ���ǰ����  
	int maxlength;              //�ļ�����󳤶�  
	int location;               //�ļ���ַ
}UF;
UF user_UF[30];


typedef struct uof
{
	string fileName;
	string command;
	int read_pt;
	int write_pt;
	int current_length;
	int location;
	int state;
}UOF;
UOF user_UOF[4];

int file_count = 0;//��ǰ�ļ���Ŀ
int select = 0;//����ѡ�����
char input_command[50];//�洢�����ļ���������
char command_str1[10];//�洢�����е��ļ���������
char command_str2[10];//�洢�����е��ļ�����
char command_str3[5];//�洢�����е��ļ�����ֽڳ���
char command_str4[3];//�洢�����е��ļ�������ʽ
int current_location = 0;
int count_C = 0;  //���ڿ����ļ��Ĵ򿪸���

void runHierFileSystem() {
	while (1)
	{
		identify_command();
		switch (select)
		{
		case 1:
			fileCreate(command_str2, command_str3, command_str4);
			print_all_file();
			break;
		case 2:
			fileWrite(command_str2, command_str3);
			print_all_file();
			print_UOF();
			break;
		case 3:
			fileOpen(command_str2, command_str3);
			print_UOF();
			break;
		case 4:
			fileRead(command_str2, command_str3);
			print_UOF();
			break;
		case 5:
			fileClose(command_str2);
			print_UOF();
			break;
		case 6:
			fileDel(command_str2);
			print_all_file();
			break;
		}
	}
}

void identify_command()
{
	for (int i = 0; i<sizeof(input_command); i++)
		input_command[i] = 0;
	cout << "    ��������Ҫ���е��ļ�������";

	cin>>input_command;

	//command_str1�洢�����������
	for (int i = 0; i<sizeof(command_str1); i++)
		command_str1[i] = 0;
	int i;
	for (i = 0; input_command[i] != '('; i++)
		command_str1[i] = input_command[i];

	//�ж����������
	if (strcmp(command_str1, "create") == 0)
	{
		//command_str2�洢�ļ���
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3�洢�ļ�����ֽڳ���
		for (int i = 0; i<sizeof(command_str3); i++)
			command_str3[i] = 0;
		j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str3[j] = input_command[i];
			j++;
		}
		//command_str4�洢�ļ�������ʽ
		j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str4[j] = input_command[i];
			j++;
		}
		select = 1;
	}

	else if (strcmp(command_str1, "write") == 0)
	{
		//command_str2�洢�ļ�������
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3�洢Ҫд���ֽڸ���
		for (int i = 0; i<sizeof(command_str3); i++)
			command_str3[i] = 0;
		j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str3[j] = input_command[i];
			j++;
		}
		select = 2;
	}

	else if (strcmp(command_str1, "open") == 0)
	{
		//command_str2�洢�ļ�������
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3�洢������ʽ
		j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str3[j] = input_command[i];
			j++;
		}
		select = 3;
	}

	else if (strcmp(command_str1, "read") == 0)
	{
		//command_str2�洢�ļ�������
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3�洢Ҫ�����ֽڸ���
		for (int i = 0; i<sizeof(command_str3); i++)
			command_str3[i] = 0;
		j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str3[j] = input_command[i];
			j++;
		}
		select = 4;
	}

	else if (strcmp(command_str1, "close") == 0)
	{
		//command_str2�洢�ļ�������
		int j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		select = 5;
	}

	else if (strcmp(command_str1, "delete") == 0)
	{
		//command_str2�洢�ļ���
		int j = 0;
		for (i = i + 1; input_command[i] != ')'; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		select = 6;
	}

}

void fileCreate(char command_str2[], char command_str3[], char command_str4[])
{
	//�����ļ��������ݱ���
	(user_UF[file_count].fileName) = command_str2;

	//���ַ���command_str3ת��Ϊʮ����,���Ҹ�����󳤶ȵ����ݱ���  
	int maxlength = 0;
	int j = 1;
	for (int i = strlen(command_str3) - 1; i >= 0; i--)
	{
		maxlength += (command_str3[i] - 48)*j;
		j *= 10;
	}
	user_UF[file_count].maxlength = maxlength;

	//���ַ���command_str4ת��Ϊʮ���ƣ����Ҹ����ļ��������͵����ݱ���
	int protectType = 0;
	j = 1;
	for (int i = strlen(command_str4) - 1; i >= 0; i--)
	{
		protectType += (command_str4[i] - 48)*j;
		j *= 10;
	}
	user_UF[file_count].protectType = protectType;
	//���õ�ǰ�ļ�����Ϊ0
	user_UF[file_count].current_length = 0;
	//�����ļ�λ��Ϊ��ǰ�ڴ�β��λ��
	user_UF[file_count].location = current_location;
	//�ı��ڴ�β����λ��
	current_location += maxlength;
	//�ļ���+1
	file_count++;
	cout << endl << "    �ļ������ɹ�!" << endl;
}

void fileWrite(char command_str2[], char command_str3[])
{
	//��uf���ҵ���Ӧ���ļ��޸ĵ�ǰ�ļ�����
	int i;
	for (i = 0; i < file_count; i++)
	{
		if (user_UF[i].fileName == command_str2)
			break;
	}

	int writelenth = 0;
	int j = 1;
	for (int i = strlen(command_str3) - 1; i >= 0; i--)
	{
		writelenth += (command_str3[i] - 48)*j;
		j *= 10;
	}
	user_UF[i].current_length += writelenth;


	//��uof���ҵ���ǰ���ļ������޸�дָ��͵�ǰ�ļ�����
	int k;
	for (k = 0; k < 4; k++)
	{
		if (user_UOF[k].fileName == command_str2)
			break;
	}
	user_UOF[k].write_pt = writelenth;
	user_UOF[k].current_length = user_UF[i].current_length;
}


void fileOpen(char command_str2[], char command_str3[])
{
	if (count_C <= 3)
	{
		//��һ���յ�λ��
		int j;
		for (j = 0; j < 4; j++)
		{
			if (user_UOF[j].fileName == "NULL")
				break;
		}

		//��UF���ҵ���Ҫ�򿪵��ļ�����Ϣ
		int i;
		for (i = 0; i < file_count; i++)
		{
			if (user_UF[i].fileName == command_str2)
				break;
		}

		//����Ӧ����Ϣ���뵽uof��
		user_UOF[j].fileName = command_str2;
		user_UOF[j].command = command_str3;
		user_UOF[j].read_pt = 1;
		user_UOF[j].write_pt = user_UF[i].current_length;
		user_UOF[j].location = user_UF[i].location;
		count_C++;
	}
	else
	{
		cout << endl << "    UOF�������޷����ļ�������" << endl;
	}
}

void fileRead(char command_str2[], char command_str3[])
{
	//��UOF���ҵ���Ҫ��ȡ���ļ�
	int i;
	for (i = 0; i < 4; i++)
	{
		if (user_UOF[i].fileName == command_str2)
			break;
	}
	//���ַ���command_str3ת��Ϊʮ����,���Ҹ��Ƶ�uof��ָ����  
	int read_char = 0;
	int j = 1;
	for (int i = strlen(command_str3) - 1; i >= 0; i--)
	{
		read_char += (command_str3[i] - 48)*j;
		j *= 10;
	}
	//�޸�UOF�еĶ�ָ��
	user_UOF[i].read_pt = read_char;
}

void fileClose(char command_str2[])
{
	//��UOF���ҵ���Ҫ�رյ��ļ�
	int i;
	for (i = 0; i < 4; i++)
	{
		if (user_UOF[i].fileName == command_str2)
			break;
	}

	//���UOF�е���Ϣ
	user_UOF[i].fileName = "NULL";
	user_UOF[i].command = "NULL";
	user_UOF[i].read_pt = 0;
	user_UOF[i].write_pt = 0;
	user_UOF[i].current_length = 0;
	user_UOF[i].location = 0;
	count_C--;
}

void fileDel(char command_str2[])
{
	int i;
	for (i = 0; i < file_count; i++)
	{
		if (user_UF[i].fileName == command_str2)
			break;
	}
	user_UF[i].fileName = "NULL";
	user_UF[i].protectType = 0;
	user_UF[i].current_length = 0;
	user_UF[i].maxlength = 0;
}


void init_UOF()
{
	for (int i = 0; i < 4; i++)
	{
		user_UOF[i] = { "NULL","NULL",0,0,0,0,0 };
	}

}

void print_UOF()
{
	cout << endl;
	cout << "     --------------------------------------------------------------------------" << endl;
	cout << "    |                          �û���ǰ���ļ��б�                            |" << endl;
	cout << "     --------------------------------------------------------------------------" << endl;
	cout << "    | �ļ��� |" << " | ������ʽ |" << " | ��ָ�� |" << " | дָ�� |" << " | ��ǰ���� |" << " | λ�� |" << " | ״̬ |" << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << "    |  ";
		cout << setw(6) << setiosflags(ios::left) << user_UOF[i].fileName;
		cout << "| |  ";
		cout << setw(8) << setiosflags(ios::left) << user_UOF[i].command;
		cout << "| |  ";
		cout << setw(6) << setiosflags(ios::left) << user_UOF[i].read_pt;
		cout << "| |  ";
		cout << setw(6) << setiosflags(ios::left) << user_UOF[i].write_pt;
		cout << "| |  ";
		cout << setw(8) << setiosflags(ios::left) << user_UOF[i].current_length;
		cout << "| |  ";
		cout << setw(4) << setiosflags(ios::left) << user_UOF[i].location;
		cout << "| |  ";
		cout << setw(4) << setiosflags(ios::left) << user_UOF[i].state;
		cout << "|" << endl;

	}
	cout << "     --------------------------------------------------------------------------" << endl;
	cout << endl;
}


//��ӡĿ¼�е������ļ�
void print_all_file()
{
	cout << endl;
	cout << "     -------------------------------------------------------------------------" << endl;
	cout << "    |                                �û��ļ�Ŀ¼                             |" << endl;
	cout << "     -------------------------------------------------------------------------" << endl;
	cout << "    | �ļ��� | " << "| �ļ�������ʽ | " << "| �ļ���ǰ���� | " << "| �ļ���󳤶� | " << "| �ļ���ַ  |" << endl;
	for (int i = 0; i < file_count; i++)
	{
		if (user_UF[i].fileName == "NULL")
		{
			continue;
		}
		else
		{
			cout << "    |  ";
			cout << setw(6) << setiosflags(ios::left) << user_UF[i].fileName;
			cout << "| |  ";
			cout << setw(12) << setiosflags(ios::left) << user_UF[i].protectType;
			cout << "| |  ";
			cout << setw(12) << setiosflags(ios::left) << user_UF[i].current_length;
			cout << "| |  ";
			cout << setw(12) << setiosflags(ios::left) << user_UF[i].maxlength;
			cout << "| |  ";
			cout << setw(8) << setiosflags(ios::left) << user_UF[i].location;
			cout << " |" << endl;
		}
	}
	cout << "     -------------------------------------------------------------------------" << endl << endl;
}