#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCKSIZE 1024
#define SIZE 1024000
#define BLOCKNUM 1000
#define END 65535
#define FREE 0
#define MAXOPENFILE 10

typedef struct FCB
{
	char filename[8];
	char exname[4];
	unsigned char attribute;
	unsigned short first;
	unsigned long length;
	char free;
} fcb;

typedef fcb(*fcbps)[25];

typedef struct FCB_OP
{
	fcb* (*get)(fcbps p);
	fcb* (*find)(fcbps p, char* filename, char* exname, unsigned short attribute);
	int (*empty)(fcbps p);
} fcb_op;

fcb* get_fcb(fcbps p);

fcb* find_fcb(fcbps p, char* filename, char* exname, unsigned short attribute);

int empty_dir(fcbps p);



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
	//unsigned short (*get_parent_block)();
} block_op;

unsigned short* get_blocks(int num);

void clean_after(unsigned short block);

unsigned short get_parent_block_first();



typedef struct USEROPEN
{
	char filename[8];
	char exname[3];
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
} useropen_op;

void save_to_openfiles(int idx, fcb* p, char* dir);

int get_openfiles_id();

int space_dir();

void clean_dir();

int space_file();

int check_fd(int fd);

int close_file(int fd);

