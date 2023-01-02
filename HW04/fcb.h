#pragma once
#include "fs.h"

fcb* get_fcb(int block);

fcb* find_fcb(int block, char* filename, char* exname, unsigned short attribute);

int empty_dir(int block);

void ls(int block);

