#include "useropen.h"

extern useropen openfilelist[MAXOPENFILE];

extern fcb_op fcbop;
extern useropen_op useropenop;

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
		if (openfilelist[fd].fcbstate == 1)
		{
			char dirname[80], buf[80];
			strcpy(dirname, openfilelist[fd].dir);
			unsigned short block = 5;
			int len = strlen(dirname);
			fcb* fcbp = NULL;
			for (int i = 0; i < len; ++i)
				for (int j = i + 1; j <= len; ++j)
					if (dirname[j] != '/' && dirname[j] != '\0')
						buf[j - i - 1] = dirname[j];
					else
					{
						buf[j - i - 1] = '\0';
						fcbp = fcbop.find(block, buf, NULL, 0);
						if (fcbp != NULL)
							block = fcbp->first;
						i = j - 1;
						break;
					}
			if (fcbp != NULL) block = fcbp->first;
			fcbp = fcbop.find(block, openfilelist[fd].filename, openfilelist[fd].exname, 1);
			fcbp->length = openfilelist[fd].length;
		}
		openfilelist[fd].topenfile = 0;
		return 0;
	}
	return -1;
}

int find_file(char* filename, char* exname, unsigned short attribute, char* dir)
{
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 1 && openfilelist[i].attribute == attribute && strcmp(openfilelist[i].filename, filename) == 0
			&& strcmp(openfilelist[i].exname, exname) == 0 && strcmp(openfilelist[i].dir, dir) == 0)
			return i;
	return -1;
}

void close()
{
	for (int i = 1; i < MAXOPENFILE; ++i)
		if (openfilelist[i].topenfile == 1 && openfilelist[i].attribute == 1 && openfilelist[i].fcbstate == 1)
			useropenop.close_file(i);
}

