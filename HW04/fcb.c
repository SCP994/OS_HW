#include "fcb.h"

fcb* get_fcb(int block)
{
	fcb* fcbp = NULL;
	while (1)
	{
		if (block == FREE || block == END) break;
		fcbp = (fcb*)hardp[block];
		for (int i = 0; i < FCBNUM; ++i)
			if (fcbp[i].free == 0) return &fcbp[i];
		block = fat1->id[block];
	}
	return NULL;
}

fcb* find_fcb(int block, char* filename, char* exname, unsigned short attribute)
{
	fcb* fcbp = NULL;
	while (1)
	{
		if (block == FREE || block == END) break;
		fcbp = (fcb*)hardp[block];
		for (int i = 0; i < FCBNUM; ++i)
			if (fcbp[i].free == 1)
			{
				if (attribute == 0 && (fcbp[i].attribute == 0 && strcmp(fcbp[i].filename, filename) == 0
					|| fcbp[i].attribute == 1 && strcmp(fcbp[i].filename, filename) == 0 && strlen(fcbp[i].exname) == 0))
					return &fcbp[i];
				if (attribute == 1 && (fcbp[i].attribute == 1 && strcmp(fcbp[i].filename, filename) == 0 && strcmp(fcbp[i].exname, exname) == 0
					|| fcbp[i].attribute == 0 && strcmp(fcbp[i].filename, filename) == 0 && strlen(exname) == 0))
					return &fcbp[i];
			}
		block = fat1->id[block];
	}
	return NULL;
}

int empty_dir(int block)
{
	fcb* fcbp = NULL;
	while (1)
	{
		if (block == FREE || block == END) break;
		fcbp = (fcb*)hardp[block];
		for (int i = 0; i < FCBNUM; ++i)
			if (fcbp[i].free == 1 && strcmp(fcbp[i].filename, ".") != 0 && strcmp(fcbp[i].filename, "..") != 0)
				return 0;
		block = fat1->id[block];
	}
	return 1;
}

void ls(int block)
{
	int sign = 0;
	fcb* fcbp = NULL;
	while (1)
	{
		if (block == FREE || block == END)
		{
			if (sign) printf("\n");
			break;
		}
		fcbp = (fcb*)hardp[block];
		for (int i = 0; i < FCBNUM; ++i)
			if (fcbp[i].free == 1)
			{
				if (fcbp[i].attribute == 0 && strcmp(fcbp[i].filename, ".") != 0 && strcmp(fcbp[i].filename, "..") != 0)
				{
					printf("%s\t", fcbp[i].filename);
					sign = 1;
				}
				if (fcbp[i].attribute == 1)
				{
					printf("%s%s\t", fcbp[i].filename, fcbp[i].exname);
					sign = 1;
				}
			}
		block = fat1->id[block];
	}
}

