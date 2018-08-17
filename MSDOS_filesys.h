#ifndef MSDOS_FILESYS_H
#define MSDOS_FILESYS_H
void runMsdosFileSystem();
void showMenu();
void initFAT();
void ident_command();
void writeFile(char command_str2[], char command_str3[]);
void insertFile(char command_str2[], char command_str3[]);
void printFDT();
void printFAT();



#endif // !MSDOS_FILESYS_H