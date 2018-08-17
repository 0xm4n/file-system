#include "hier_filesys.h"
#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
using namespace std;

typedef struct uf              //文件块结构体  
{
	string fileName;
	int protectType;           //文件保护方式
	int current_length;         //文件当前长度  
	int maxlength;              //文件的最大长度  
	int location;               //文件地址
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

int file_count = 0;//当前文件数目
int select = 0;//功能选择参数
char input_command[50];//存储输入文件操作命令
char command_str1[10];//存储命令中的文件操作类型
char command_str2[10];//存储命令中的文件描述
char command_str3[5];//存储命令中的文件最大字节长度
char command_str4[3];//存储命令中的文件保护方式
int current_location = 0;
int count_C = 0;  //用于控制文件的打开个数

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
	cout << "    请输入需要进行的文件操作：";

	cin>>input_command;

	//command_str1存储命令操作类型
	for (int i = 0; i<sizeof(command_str1); i++)
		command_str1[i] = 0;
	int i;
	for (i = 0; input_command[i] != '('; i++)
		command_str1[i] = input_command[i];

	//判断命令的类型
	if (strcmp(command_str1, "create") == 0)
	{
		//command_str2存储文件名
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3存储文件最大字节长度
		for (int i = 0; i<sizeof(command_str3); i++)
			command_str3[i] = 0;
		j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str3[j] = input_command[i];
			j++;
		}
		//command_str4存储文件保护方式
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
		//command_str2存储文件描述符
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3存储要写的字节个数
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
		//command_str2存储文件描述符
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3存储操作方式
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
		//command_str2存储文件描述符
		int j = 0;
		for (i = i + 1; input_command[i] != ','; i++)
		{
			command_str2[j] = input_command[i];
			j++;
		}
		//command_str3存储要读的字节个数
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
		//command_str2存储文件描述符
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
		//command_str2存储文件名
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
	//复制文件名到数据表中
	(user_UF[file_count].fileName) = command_str2;

	//把字符串command_str3转换为十进制,并且复制最大长度到数据表中  
	int maxlength = 0;
	int j = 1;
	for (int i = strlen(command_str3) - 1; i >= 0; i--)
	{
		maxlength += (command_str3[i] - 48)*j;
		j *= 10;
	}
	user_UF[file_count].maxlength = maxlength;

	//把字符串command_str4转换为十进制，并且复制文件保护类型到数据表中
	int protectType = 0;
	j = 1;
	for (int i = strlen(command_str4) - 1; i >= 0; i--)
	{
		protectType += (command_str4[i] - 48)*j;
		j *= 10;
	}
	user_UF[file_count].protectType = protectType;
	//设置当前文件长度为0
	user_UF[file_count].current_length = 0;
	//设置文件位置为当前内存尾部位置
	user_UF[file_count].location = current_location;
	//改变内存尾部的位置
	current_location += maxlength;
	//文件数+1
	file_count++;
	cout << endl << "    文件创建成功!" << endl;
}

void fileWrite(char command_str2[], char command_str3[])
{
	//在uf中找到对应的文件修改当前文件长度
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


	//在uof中找到当前的文件并且修改写指针和当前文件长度
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
		//找一个空的位置
		int j;
		for (j = 0; j < 4; j++)
		{
			if (user_UOF[j].fileName == "NULL")
				break;
		}

		//在UF中找到需要打开的文件的信息
		int i;
		for (i = 0; i < file_count; i++)
		{
			if (user_UF[i].fileName == command_str2)
				break;
		}

		//将相应的信息插入到uof中
		user_UOF[j].fileName = command_str2;
		user_UOF[j].command = command_str3;
		user_UOF[j].read_pt = 1;
		user_UOF[j].write_pt = user_UF[i].current_length;
		user_UOF[j].location = user_UF[i].location;
		count_C++;
	}
	else
	{
		cout << endl << "    UOF已满，无法打开文件！！！" << endl;
	}
}

void fileRead(char command_str2[], char command_str3[])
{
	//在UOF中找到需要读取的文件
	int i;
	for (i = 0; i < 4; i++)
	{
		if (user_UOF[i].fileName == command_str2)
			break;
	}
	//把字符串command_str3转换为十进制,并且复制到uof读指针中  
	int read_char = 0;
	int j = 1;
	for (int i = strlen(command_str3) - 1; i >= 0; i--)
	{
		read_char += (command_str3[i] - 48)*j;
		j *= 10;
	}
	//修改UOF中的读指针
	user_UOF[i].read_pt = read_char;
}

void fileClose(char command_str2[])
{
	//在UOF中找到需要关闭的文件
	int i;
	for (i = 0; i < 4; i++)
	{
		if (user_UOF[i].fileName == command_str2)
			break;
	}

	//清空UOF中的信息
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
	cout << "    |                          用户当前打开文件列表                            |" << endl;
	cout << "     --------------------------------------------------------------------------" << endl;
	cout << "    | 文件名 |" << " | 操作方式 |" << " | 读指针 |" << " | 写指针 |" << " | 当前长度 |" << " | 位置 |" << " | 状态 |" << endl;
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


//打印目录中的所有文件
void print_all_file()
{
	cout << endl;
	cout << "     -------------------------------------------------------------------------" << endl;
	cout << "    |                                用户文件目录                             |" << endl;
	cout << "     -------------------------------------------------------------------------" << endl;
	cout << "    | 文件名 | " << "| 文件保护方式 | " << "| 文件当前长度 | " << "| 文件最大长度 | " << "| 文件地址  |" << endl;
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