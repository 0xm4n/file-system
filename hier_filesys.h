#ifndef HIER_FILESYS_H
#define HIER_FILESYS_H
void runHierFileSystem();
void identify_command();
void fileCreate(char command_str2[], char command_str3[], char command_str4[]);
void fileWrite(char command_str2[], char command_str3[]);
void fileOpen(char command_str2[], char command_str3[]);
void fileRead(char command_str2[], char command_str3[]);
void fileClose(char command_str2[]);
void fileDel(char command_str2[]);
void init_UOF();
void print_UOF();
void print_all_file();
#endif // !HIER_FILESYS_H