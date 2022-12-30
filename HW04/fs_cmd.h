#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"

int save();

int load();

void my_format();

int judge_dir(char* dirname);

void my_cd(char* dirname);

void my_mkdir(char* dirname);

void my_rmdir(char* dirname);

void my_ls();

char* get_name(char* filename);

char* get_ext(char* filename);

void my_create(char* filename);

void my_rm(char* filename);

int my_open(char* filename);

void my_close(int fd);

int my_write(int fd);

int do_write(int fd, char* text, int len, char wstyle);

int my_read(int fd);

int do_read(int fd, int len, char* text, int idx);

void my_exitsys();

void run();
