#include "MSDOS_filesys.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <iomanip>
using namespace std;

typedef struct file
{
	string fileName;
	int firstBlock;
	int fileLength;
}FD;
FD user_FD[30];
int fileCount = 0;
//�����ļ������
typedef struct fat
{
	int location;
	int nextLocation;
}FAT;
FAT user_FAT[100];

int Select = 0;
char inputCommand[50];//�洢�����ļ���������
char command_Str1[10];//�洢�����е��ļ���������
char command_Str2[10];//�洢�����е��ļ�����
char command_Str3[5];//�洢�����е��ļ�����ֽڳ���

void runMsdosFileSystem() {
	showMenu();
	initFAT();
	while (true)
	{
		ident_command();
		switch (Select)
		{
		case 1:
			writeFile(command_Str2, command_Str3);
			printFDT();
			printFAT();
			break;
		case 2:
			insertFile(command_Str2, command_Str3);
			printFDT();
			printFAT();
			break;
		case 3:
			printFDT();
			break;
		case 4:
			printFAT();
			break;
		}
	}
}

void showMenu()
{
	cout << endl;
	cout << "  ----------------------------------------------------------" << endl;
	cout << " |                          Menu                            |" << endl;
	cout << "  ----------------------------------------------------------" << endl;
	cout << " | �ļ����� | " << "| ���        |" << " | ��ʽ                        |" << endl;
	cout << " |  write   | " << "|  ����д�ļ� |" << " |  write(�ļ������߼���¼��)  |" << endl;
	cout << " |  insert  | " << "|  ����д�ļ� |" << " |  insert(�ļ������߼���¼��) |" << endl;
	cout << " |  FDT     | " << "|  �ļ�Ŀ¼�� |" << " |  FDT()                      |" << endl;
	cout << " |  FAT     | " << "|  �ļ������ |" << " |  FAT()                      |" << endl;
	cout << "  ----------------------------------------------------------" << endl;
	cout << endl;

}

void initFAT() {
	for (int i = 0; i < 100; i++)
	{
		user_FAT[i].location = i;
	}
	user_FAT[0].nextLocation = -1;
	user_FAT[1].nextLocation = -1;
	for (int i = 2; i < 100; i++)
	{
		user_FAT[i].nextLocation = 0;
	}
}

void ident_command()
{
	for (int i = 0; i<sizeof(inputCommand); i++)
		inputCommand[i] = 0;
	cout << " ��������Ҫ���е��ļ�������";

	cin.getline(inputCommand, 50);

	//command_Str1�洢�����������
	for (int i = 0; i<sizeof(command_Str1); i++)
		command_Str1[i] = 0;
	int i;
	for (i = 0; inputCommand[i] != '('; i++)
		command_Str1[i] = inputCommand[i];

	//�ж����������
	if (strcmp(command_Str1, "write") == 0)
	{
		//command_Str2�洢�ļ���
		int j = 0;
		for (i = i + 1; inputCommand[i] != ','; i++)
		{
			command_Str2[j] = inputCommand[i];
			j++;
		}
		//command_Str3�洢�߼���¼��
		for (int i = 0; i<sizeof(command_Str3); i++)
			command_Str3[i] = 0;
		j = 0;
		for (i = i + 1; inputCommand[i] != ')'; i++)
		{
			command_Str3[j] = inputCommand[i];
			j++;
		}
		Select = 1;
	}

	else if (strcmp(command_Str1, "insert") == 0)
	{
		//command_Str2�洢�ļ���
		int j = 0;
		for (i = i + 1; inputCommand[i] != ','; i++)
		{
			command_Str2[j] = inputCommand[i];
			j++;
		}
		//command_Str3�洢�߼���¼��
		for (int i = 0; i<sizeof(command_Str3); i++)
			command_Str3[i] = 0;
		j = 0;
		for (i = i + 1; inputCommand[i] != ')'; i++)
		{
			command_Str3[j] = inputCommand[i];
			j++;
		}
		Select = 2;
	}

	else if (strcmp(command_Str1, "FDT") == 0)
	{
		Select = 3;
	}

	else if (strcmp(command_Str1, "FAT") == 0)
	{
		Select = 4;
	}
}

//����д�ļ�
//1�������ļ�������ļ���������ô����
//2���Ƚ�д�볤�Ⱥ͵�ǰ����
//3�����д�볤�ȴ��ڵ��ڵ�ǰ�ĳ�����ô����׷��д
//4�����С����ô����ս����������һ���ִ��̿ռ���0
void writeFile(char command_Str2[], char command_Str3[]) {
	int reti_file;
	bool get_file = 0;
	for (reti_file = 0; reti_file < fileCount; reti_file++)
	{
		if (user_FD[reti_file].fileName == command_Str2)
		{
			get_file = 1;
			break;
		}
	}
	if (get_file == 0)
	{
		cout << " �ļ������ڣ��޷�д�ļ�!";
	}
	else
	{
		//��command_Str3���ʮ����
		int write_fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			write_fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		//�����д���ļ��Ⱦɵ��ļ���,��Ҫ������ĿյĿ�д��ȥ
		if (write_fileLength >= user_FD[reti_file].fileLength)
		{
			write_fileLength -= user_FD[reti_file].fileLength;
			//�ҵ�����ǰ�ļ������һ����
			int current_pt = user_FD[reti_file].firstBlock;
			int next_pt = user_FAT[current_pt].nextLocation;
			for (int x = user_FD[reti_file].fileLength; x > 1; x--)
			{
				current_pt = next_pt;
				next_pt = user_FAT[current_pt].nextLocation;
			}
			//�ҵ�write_fileLength���յĿ鲢����֮ǰ��β��׷�����µĿ�
			for (int i = write_fileLength; i > 0; i--)
			{
				//�����ǰ�������һ���飬��ô�������дΪ-1
				if (i == 1)
				{
					user_FAT[current_pt].nextLocation = -1;
					break;
				}
				//������һ���յĿ���Ϊ��ǰ�����һ����
				else
				{
					while (1)
					{
						next_pt = rand() % 100;
						if (user_FAT[next_pt].nextLocation == 0)
						{
							break;
						}
					}
					user_FAT[current_pt].nextLocation = next_pt;
					current_pt = next_pt;
				}
			}
			//����ٸ�FD�еĲ���
			user_FD[reti_file].fileLength += write_fileLength;
			cout << endl << " Write Succeed!" << endl << endl;
		}
		//�����д���ļ��Ⱦɵ��ļ�С,��Ҫ�ٵĿ��ڴ����
		else if (write_fileLength < user_FD[reti_file].fileLength)
		{
			int temp_count1 = user_FD[reti_file].fileLength - write_fileLength;//�ٶ��ٸ���
																			   //�ҵ���д���ļ������һ����
			int current_pt = user_FD[reti_file].firstBlock;
			int next_pt = user_FAT[current_pt].nextLocation;
			for (int x = write_fileLength; x > 1; x--)
			{
				current_pt = next_pt;
				next_pt = user_FAT[current_pt].nextLocation;
			}
			user_FAT[current_pt].nextLocation = -1;
			//�����������Ŀ鶼��0
			for (int y = temp_count1; y > 1; y--)
			{
				current_pt = next_pt;
				next_pt = user_FAT[current_pt].nextLocation;
				user_FAT[current_pt].nextLocation = 0;
			}
			user_FAT[next_pt].nextLocation = 0;
			user_FD[reti_file].fileLength -= temp_count1;
			cout << endl << " Write Succeed!" << endl << endl;
		}
	}
}

//�´����ļ���������ļ������
//�����������һ�����ļ�����FDT�У���ʱ�½�����ļ����ڶ���������ļ���FDT�д�ʱֱ����д�����һ������
void insertFile(char command_Str2[], char command_Str3[]) {

	//�ӵ�1���ļ���ʼ�����Ƿ��������ļ�
	int retrieval;
	bool in_FD = 0;
	for (retrieval = 0; retrieval < fileCount; retrieval++)
	{
		if (user_FD[retrieval].fileName == command_Str2)
		{
			in_FD = 1;
			break;
		}
	}
	if (in_FD == 1)
	{
		//��command_Str3���ʮ����
		int insert_fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			insert_fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		//�ҵ�����ǰ�ļ������һ����
		int current_pt = user_FD[retrieval].firstBlock;
		int next_pt = user_FAT[current_pt].nextLocation;
		for (int x = user_FD[retrieval].fileLength; x > 1; x--)
		{
			current_pt = next_pt;
			next_pt = user_FAT[current_pt].nextLocation;
		}
		//�ҵ�insert_fileLength���յĿ鲢����֮ǰ��β��׷�����µĿ�
		for (int i = insert_fileLength; i > 0; i--)
		{
			//�����ǰ�������һ���飬��ô�������дΪ-1
			if (i == 1)
			{
				user_FAT[current_pt].nextLocation = -1;
				break;
			}
			//������һ���յĿ���Ϊ��ǰ�����һ����
			else
			{
				while (1)
				{
					next_pt = rand() % 100;
					if (user_FAT[next_pt].nextLocation == 0)
					{
						break;
					}
				}
				user_FAT[current_pt].nextLocation = next_pt;
				current_pt = next_pt;
			}
		}
		//����ٸ�FD�еĲ���
		user_FD[retrieval].fileLength += insert_fileLength;
		cout << endl << " Insert Succeed!" << endl << endl;
	}
	else
	{
		//���ļ���д��FDT
		user_FD[fileCount].fileName = command_Str2;
		//���ļ�����д��FDT
		int fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		user_FD[fileCount].fileLength = fileLength;
		//��FAT���ҵ�һ���յĿ���Ϊ���ļ�����ʼ��
		srand((unsigned)time(NULL));
		int firstBlock = 0;
		while (1)
		{
			firstBlock = rand() % 100;
			if (user_FAT[firstBlock].nextLocation == 0)
			{
				break;
			}
		}
		user_FD[fileCount].firstBlock = firstBlock;
		int currentBlock = firstBlock;
		int nextBlock = 0;
		//��дFAT��
		for (int i = fileLength; i > 0; i--)
		{
			//�����ǰ�������һ���飬��ô�������дΪ-1
			if (i == 1)
			{
				user_FAT[currentBlock].nextLocation = -1;
				break;
			}
			//������һ���յĿ���Ϊ��ǰ�����һ����
			else
			{
				while (1)
				{
					nextBlock = rand() % 100;
					if (user_FAT[nextBlock].nextLocation == 0)
					{
						break;
					}
				}
				user_FAT[currentBlock].nextLocation = nextBlock;
				currentBlock = nextBlock;
			}
		}
		fileCount++;
		cout << endl << " Create Succeed!" << endl << endl;
	}

}

void printFDT()
{
	cout << endl;
	cout << "  ------------------------------------" << endl;
	cout << " |            �ļ�Ŀ¼��              |" << endl;
	cout << "  ------------------------------------" << endl;
	cout << " | �ļ��� | " << "| ��ʼ��� | " << "| �ļ�����   | " << endl;
	for (int i = 0; i < fileCount; i++)
	{
		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FD[i].fileName;
		cout << "| |  ";
		cout << setw(8) << setiosflags(ios::left) << user_FD[i].firstBlock;
		cout << "| |  ";
		cout << setw(10) << setiosflags(ios::left) << user_FD[i].fileLength;
		cout << "|" << endl;
	}
	cout << "  ------------------------------------" << endl << endl;
}

void printFAT() {
	cout << endl;
	cout << "  -----------------------------------------------------------------------------------" << endl;
	cout << " |                              FAT                                                  |" << endl;
	cout << "  -----------------------------------------------------------------------------------" << endl;
	cout << " | ADRESS " << " NEXT  | " << "| ADRESS " << " NEXT | " << "| ADRESS " << " NEXT | " << "| ADRESS " << " NEXT | " << "| ADRESS " << " NEXT | " << endl;
	for (int i = 0; i < 20; i++)
	{
		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FAT[5 * i].location;
		cout << "  ";
		cout << setw(4) << setiosflags(ios::left) << user_FAT[5 * i].nextLocation;
		cout << " |";

		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FAT[5 * i + 1].location;
		cout << "  ";
		cout << setw(4) << setiosflags(ios::left) << user_FAT[5 * i + 1].nextLocation;
		cout << "|";

		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FAT[5 * i + 2].location;
		cout << "  ";
		cout << setw(4) << setiosflags(ios::left) << user_FAT[5 * i + 2].nextLocation;
		cout << "|";

		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FAT[5 * i + 3].location;
		cout << "  ";
		cout << setw(4) << setiosflags(ios::left) << user_FAT[5 * i + 3].nextLocation;
		cout << "|";

		cout << " |  ";
		cout << setw(6) << setiosflags(ios::left) << user_FAT[5 * i + 4].location;
		cout << "  ";
		cout << setw(4) << setiosflags(ios::left) << user_FAT[5 * i + 4].nextLocation;
		cout << "|" << endl;
	}
	cout << "  -----------------------------------------------------------------------------------" << endl << endl;

}