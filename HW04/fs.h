#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 1024
#define SIZE 1024000
#define BLOCKNUM 1000
#define END 65535
#define FREE 0
#define MAXOPENFILE 10
#define FCBNUM 32

typedef struct FCB
{
	char filename[8];
	char exname[4];
	unsigned char attribute;
	unsigned short first;
	unsigned long length;
	char free;
} fcb;

typedef struct FCB_OP
{
	fcb* (*get)(int block);
	fcb* (*find)(int block, char*, char*, unsigned short);
	int (*empty)(int block);
	void (*ls)(int block);
} fcb_op;

typedef struct FAT
{
	unsigned short id[BLOCKNUM];
} fat;

typedef struct BLOCK0
{
	char information[200];
	unsigned short root;
	unsigned char* startblock;
} block0;

typedef struct BLOCK_OP
{
	unsigned short* (*get)(int num);
	void (*clean_after)(unsigned short block);
} block_op;

typedef struct USEROPEN
{
	char filename[8];
	char exname[4];
	unsigned char attribute;
	unsigned short first;
	unsigned long length;
	char dir[80];
	int count;
	char fcbstate;
	char topenfile;
} useropen;

typedef struct USEROPEN_OP
{
	void (*save)(int idx, fcb* p, char* dir);
	int (*get_id)();
	int (*space_dir)();
	void (*clean_dir)();
	int (*space_file)();
	int (*check_fd)(int fd);
	int (*close_file)(int fd);
	int (*find_file)(char* filename, char* exname, unsigned short attribute, char* dir);
	void (*close)();
} useropen_op;

unsigned char* myvhard;
useropen openfilelist[MAXOPENFILE];
int curdir;
char currentdir[80];

unsigned char* hardp[BLOCKNUM];
fat* fat1;

block_op blockop;
fcb_op fcbop;
useropen_op useropenop;

