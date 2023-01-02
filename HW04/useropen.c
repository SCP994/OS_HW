#include "useropen.h"

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

