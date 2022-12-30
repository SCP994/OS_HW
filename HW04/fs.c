#include "fs.h"

extern fat* fat1;
extern int curdir;
extern useropen openfilelist[MAXOPENFILE];
extern char currentdir[80];
extern block_op blockop;
extern fcb_op fcbop;
extern unsigned char* myvhard;

extern unsigned char* hardp[BLOCKNUM];
extern useropen_op useropenop;


fcb* get_fcb(fcbps p)
{
	for (int i = 0; i < 25; ++i)
		if (p[i]->free == 0) return p[i];
	return NULL;
}

fcb* find_fcb(fcbps p, char* filename, char* exname, unsigned short attribute)
{
	for (int i = 0; i < 25; ++i)
		if (p[i]->free == 1)
		{
			if (attribute == 0 && (p[i]->attribute == 0 && strcmp(p[i]->filename, filename) == 0
				|| p[i]->attribute == 1 && strcmp(p[i]->filename, filename) == 0 && strlen(p[i]->exname) == 0))
				return p[i];
			if (attribute == 1 && (p[i]->attribute == 1 && strcmp(p[i]->filename, filename) == 0 && strcmp(p[i]->exname, exname) == 0
				|| p[i]->attribute == 0 && strcmp(p[i]->filename, filename) == 0 && strlen(exname) == 0))
				return p[i];
		}
	return NULL;
}

int empty_dir(fcbps p)
{
	for (int i = 0; i < 25; ++i)
		if (p[i]->free == 1 && strcmp(p[i]->filename, ".") != 0 && strcmp(p[i]->filename, "..") != 0)
			return 0;
	return 1;
}

unsigned short* get_blocks(int num)
{
	unsigned short* ret = (unsigned short*)malloc(sizeof(unsigned short) * num);
	int idx = 0;
	for (int i = 0; i < BLOCKNUM; ++i)
	{
		if (fat1->id[i] == FREE) ret[idx++] = i;
		if (idx == num) break;
	}
	if (idx < num) return NULL;
	return ret;
}

void clean_after(unsigned short block)
{
	while (fat1->id[block] != END)
	{
		unsigned short t = fat1->id[block];
		if (t == FREE) return;
		fat1->id[block] = FREE;
		block = t;
	}
	fat1->id[block] = FREE;
}

unsigned short get_parent_block_first()
{
	for (int i = 0; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 1 && openfilelist[i].attribute == 0 && strcmp(openfilelist[i].dir, currentdir) == 0)
			return openfilelist[i].first;
	return BLOCKNUM;
}

void save_to_openfiles(int idx, fcb* fcbp, char* dir)
{
	strcpy(openfilelist[idx].filename, fcbp->filename);
	strcpy(openfilelist[idx].exname, fcbp->exname);
	openfilelist[idx].attribute = fcbp->attribute;
	openfilelist[idx].first = fcbp->first;
	openfilelist[idx].length = fcbp->length;
	strcpy(openfilelist[idx].dir, dir);
	openfilelist[idx].count = 0;
	openfilelist[idx].fcbstate = 0;
	openfilelist[idx].topenfile = 1;
}


int get_openfiles_id()
{
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 0) return i;
	return -1;
}

int space_dir()
{
	int ret = 0;
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 0 || openfilelist[i].attribute == 0) ++ret;
	return ret;
}

void clean_dir()
{
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 1 && openfilelist[i].attribute == 0) openfilelist[i].topenfile = 0;
}

int space_file()
{
	int ret = 0;
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 0) ++ret;
	return ret;
}

int check_fd(int fd)
{
	if (fd > 0 && fd < MAXOPENFILE && openfilelist[fd].topenfile == 1 && openfilelist[fd].attribute == 1)
		return 0;
	return -1;
}

int close_file(int fd)
{
	if (fd > 0 && fd < MAXOPENFILE && openfilelist[fd].topenfile == 1 && openfilelist[fd].attribute == 1)
	{
		openfilelist[fd].topenfile = 0;
		return 0;
	}
	return -1;
}

