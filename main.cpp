#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "ChooseFileSys.h"
#include "HomePage.h"
#include "Login.h"
#include "Regist.h"
#include "hier_filesys.h"
#include "MSDOS_filesys.h"


int main()
{
	choose_fileSys();
	system("pause");
	return 0;
}

//²âÊÔ´úÂëÈçÏÂ
//create(fd1,30,1)
//create(fd2,20,3)
//create(fd3,50,2)
//open(fd1,write)
//open(fd2,write)
//open(fd3,write)
//write(fd1,25)
//write(fd2,20)
//write(fd3,15)
//read(fd1,20)
//close(fd1)
//delete(fd1)
