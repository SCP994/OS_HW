#pragma once
#include "fs.h"

void save_to_openfiles(int idx, fcb* p, char* dir);

int get_openfiles_id();

int space_dir();

void clean_dir();

int space_file();

int check_fd(int fd);

int close_file(int fd);

