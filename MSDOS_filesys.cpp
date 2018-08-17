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
//定义文件分配表
typedef struct fat
{
	int location;
	int nextLocation;
}FAT;
FAT user_FAT[100];

int Select = 0;
char inputCommand[50];//存储输入文件操作命令
char command_Str1[10];//存储命令中的文件操作类型
char command_Str2[10];//存储命令中的文件描述
char command_Str3[5];//存储命令中的文件最大字节长度

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
	cout << " | 文件操作 | " << "| 简介        |" << " | 格式                        |" << endl;
	cout << " |  write   | " << "|  覆盖写文件 |" << " |  write(文件名，逻辑记录号)  |" << endl;
	cout << " |  insert  | " << "|  插入写文件 |" << " |  insert(文件名，逻辑记录号) |" << endl;
	cout << " |  FDT     | " << "|  文件目录表 |" << " |  FDT()                      |" << endl;
	cout << " |  FAT     | " << "|  文件分配表 |" << " |  FAT()                      |" << endl;
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
	cout << " 请输入需要进行的文件操作：";

	cin.getline(inputCommand, 50);

	//command_Str1存储命令操作类型
	for (int i = 0; i<sizeof(command_Str1); i++)
		command_Str1[i] = 0;
	int i;
	for (i = 0; inputCommand[i] != '('; i++)
		command_Str1[i] = inputCommand[i];

	//判断命令的类型
	if (strcmp(command_Str1, "write") == 0)
	{
		//command_Str2存储文件名
		int j = 0;
		for (i = i + 1; inputCommand[i] != ','; i++)
		{
			command_Str2[j] = inputCommand[i];
			j++;
		}
		//command_Str3存储逻辑记录号
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
		//command_Str2存储文件名
		int j = 0;
		for (i = i + 1; inputCommand[i] != ','; i++)
		{
			command_Str2[j] = inputCommand[i];
			j++;
		}
		//command_Str3存储逻辑记录号
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

//覆盖写文件
//1、检索文件，如果文件不存在那么报错
//2、比较写入长度和当前长度
//3、如果写入长度大于等于当前的长度那么就是追加写
//4、如果小于那么就清空将多出来的那一部分磁盘空间变成0
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
		cout << " 文件不存在，无法写文件!";
	}
	else
	{
		//将command_Str3变成十进制
		int write_fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			write_fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		//如果新写的文件比旧的文件大,需要多出来的空的块写进去
		if (write_fileLength >= user_FD[reti_file].fileLength)
		{
			write_fileLength -= user_FD[reti_file].fileLength;
			//找到插入前文件的最后一个块
			int current_pt = user_FD[reti_file].firstBlock;
			int next_pt = user_FAT[current_pt].nextLocation;
			for (int x = user_FD[reti_file].fileLength; x > 1; x--)
			{
				current_pt = next_pt;
				next_pt = user_FAT[current_pt].nextLocation;
			}
			//找到write_fileLength个空的块并且在之前的尾部追加上新的块
			for (int i = write_fileLength; i > 0; i--)
			{
				//如果当前块是最后一个块，那么将这个块写为-1
				if (i == 1)
				{
					user_FAT[current_pt].nextLocation = -1;
					break;
				}
				//否则找一个空的块作为当前块的下一个块
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
			//最后再改FD中的参数
			user_FD[reti_file].fileLength += write_fileLength;
			cout << endl << " Write Succeed!" << endl << endl;
		}
		//如果新写的文件比旧的文件小,需要少的块内存清空
		else if (write_fileLength < user_FD[reti_file].fileLength)
		{
			int temp_count1 = user_FD[reti_file].fileLength - write_fileLength;//少多少个块
																			   //找到新写的文件的最后一个块
			int current_pt = user_FD[reti_file].firstBlock;
			int next_pt = user_FAT[current_pt].nextLocation;
			for (int x = write_fileLength; x > 1; x--)
			{
				current_pt = next_pt;
				next_pt = user_FAT[current_pt].nextLocation;
			}
			user_FAT[current_pt].nextLocation = -1;
			//将后面多出来的块都清0
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

//新创建文件或插入在文件最后面
//分两种情况第一种是文件不在FDT中，此时新建这个文件，第二种情况是文件在FDT中此时直接续写在最后一个块中
void insertFile(char command_Str2[], char command_Str3[]) {

	//从第1个文件开始检索是否存在这个文件
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
		//将command_Str3变成十进制
		int insert_fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			insert_fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		//找到插入前文件的最后一个块
		int current_pt = user_FD[retrieval].firstBlock;
		int next_pt = user_FAT[current_pt].nextLocation;
		for (int x = user_FD[retrieval].fileLength; x > 1; x--)
		{
			current_pt = next_pt;
			next_pt = user_FAT[current_pt].nextLocation;
		}
		//找到insert_fileLength个空的块并且在之前的尾部追加上新的块
		for (int i = insert_fileLength; i > 0; i--)
		{
			//如果当前块是最后一个块，那么将这个块写为-1
			if (i == 1)
			{
				user_FAT[current_pt].nextLocation = -1;
				break;
			}
			//否则找一个空的块作为当前块的下一个块
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
		//最后再改FD中的参数
		user_FD[retrieval].fileLength += insert_fileLength;
		cout << endl << " Insert Succeed!" << endl << endl;
	}
	else
	{
		//将文件名写入FDT
		user_FD[fileCount].fileName = command_Str2;
		//将文件长度写入FDT
		int fileLength = 0;
		int j = 1;
		for (int i = strlen(command_Str3) - 1; i >= 0; i--)
		{
			fileLength += (command_Str3[i] - 48)*j;
			j *= 10;
		}
		user_FD[fileCount].fileLength = fileLength;
		//在FAT中找到一个空的块作为新文件的起始块
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
		//改写FAT表
		for (int i = fileLength; i > 0; i--)
		{
			//如果当前块是最后一个块，那么将这个块写为-1
			if (i == 1)
			{
				user_FAT[currentBlock].nextLocation = -1;
				break;
			}
			//否则找一个空的块作为当前块的下一个块
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
	cout << " |            文件目录表              |" << endl;
	cout << "  ------------------------------------" << endl;
	cout << " | 文件名 | " << "| 起始块号 | " << "| 文件长度   | " << endl;
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