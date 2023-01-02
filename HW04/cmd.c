#include "cmd.h"

void function_init()
{
	blockop.get = &get_blocks;
	blockop.clean_after = &clean_after;

	fcbop.get = &get_fcb;
	fcbop.find = &find_fcb;
	fcbop.empty = &empty_dir;
	fcbop.ls = &ls;

	useropenop.save = &save_to_openfiles;
}

void my_format()
{
	function_init();
	myvhard = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	int error = load();

	for (int i = 0; i < BLOCKNUM; ++i)
		hardp[i] = &myvhard[i * BLOCKSIZE];

	block0* block = (block0*)hardp[0];
	fat1 = (fat*)hardp[1];
	fcb* fcbp1 = (fcb*)hardp[5], * fcbp2 = (fcb*)hardp[6];

	if (error != 0)
	{
		strcpy(block->information, "Block Size: 1024 B, Block Number: 1000.");
		block->root = 5;
		block->startblock = hardp[7];

		for (int i = 0; i < 5; ++i)
			fat1->id[i] = END;
		fat1->id[5] = 6;
		fat1->id[6] = END;
		for (int i = 7; i < BLOCKNUM; ++i)
			fat1->id[i] = FREE;

		for (int i = 0; i < FCBNUM; ++i)
		{
			fcbp1[i].free = 0;
			fcbp2[i].free = 0;
		}

		strcpy(fcbp1[0].filename, ".");
		strcpy(fcbp1[0].exname, "");
		fcbp1[0].attribute = 0;
		fcbp1[0].first = 5;
		fcbp1[0].length = 2048;
		fcbp1[0].free = 1;
		strcpy(fcbp1[1].filename, "..");
		strcpy(fcbp1[1].exname, "");
		fcbp1[1].attribute = 0;
		fcbp1[1].first = 5;
		fcbp1[1].length = 2048;
		fcbp1[1].free = 1;
	}

	for (int i = 0; i < MAXOPENFILE; ++i)
		openfilelist[i].topenfile = 0;

	curdir = 0;
	strcpy(currentdir, "/");
	useropenop.save(0, &fcbp1[0], currentdir);
}

void my_mkdir(char* dirname)
{
	fcb* fcbp = fcbop.find(openfilelist[curdir].first, dirname, NULL, 0);
	if (fcbp != NULL)
	{
		printf("Duplicated directory.\n");
		return;
	}
	fcbp = fcbop.get(openfilelist[curdir].first);
	if (fcbp == NULL)
	{
		printf("No space left in the parent directory.\n");
		return;
	}
	unsigned short* blocks = blockop.get(2);
	if (blocks == NULL)
	{
		printf("No space left on the disk.\n");
		return;
	}
	unsigned short block1 = blocks[0], block2 = blocks[1];
	free(blocks);
	blocks = NULL;
	fat1->id[block1] = block2;
	fat1->id[block2] = END;

	strcpy(fcbp->filename, dirname);
	strcpy(fcbp->exname, "");
	fcbp->attribute = 0;
	fcbp->first = block1;
	fcbp->length = 2048;
	fcbp->free = 1;

	fcb* fcbp1 = (fcb*)hardp[block1], * fcbp2 = (fcb*)hardp[block2];
	for (int i = 0; i < FCBNUM; ++i)
	{
		fcbp1[i].free = 0;
		fcbp2[i].free = 0;
	}

	strcpy(fcbp1[0].filename, ".");
	strcpy(fcbp1[0].exname, "");
	fcbp1[0].attribute = 0;
	fcbp1[0].first = block1;
	fcbp1[0].length = 2048;
	fcbp1[0].free = 1;
	strcpy(fcbp1[1].filename, "..");
	strcpy(fcbp1[1].exname, "");
	fcbp1[1].attribute = 1;
	fcbp1[1].first = openfilelist[curdir].first;
	fcbp1[1].length = 2048;
	fcbp1[1].free = 1;
}

void my_rmdir(char* dirname)
{
	fcb* fcbp = fcbop.find(openfilelist[curdir].first, dirname, NULL, 0);
	if (fcbp == NULL)
	{
		printf("No such directory.\n");
		return;
	}
	if (!fcbop.empty(fcbp->first))
	{
		printf("The directory is not empty.\n");
		return;
	}

	unsigned short block1 = fcbp->first, block2 = fat1->id[block1];
	fcbp->free = 0;
	fcbp = NULL;
	fat1->id[block1] = FREE;
	fat1->id[block2] = FREE;
}

void my_ls()
{
	fcbop.ls(openfilelist[curdir].first);
}

void my_exitsys()
{
	save();
	free(myvhard);
	myvhard = NULL;
}

char* trim(char* str)
{
	int len = strlen(str), start = 0, end = 0;
	char* buf = (char*)malloc(sizeof(char) * (len + 1));
	char* ret = (char*)malloc(sizeof(char) * (len + 1));
	strcpy(buf, str);

	for (int i = 0; i < len; ++i)
		if (buf[i] != ' ' && buf[i] != '\t')
		{
			start = i;
			break;
		}

	for (int i = len - 1; i >= 0; --i)
		if (buf[i] != ' ' && buf[i] != '\t')
		{
			end = i + 1;
			break;
		}
	buf[end] = '\0';

	strcpy(ret, &buf[start]);
	free(buf);
	return ret;
}

int check_str(char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
		if (str[i] == ' ' || str[i] == '\t') return -1;
	return 0;
}

void shell()
{
	my_format();

	char buf1[100], buf2[100], cmd[100];
	int sign = 0, len = 0;

	while (1)
	{
		printf("filesystem-shell%s# ", currentdir);
		fgets(buf1, 100, stdin);
		if (buf1[strlen(buf1) - 1] == '\n') buf1[strlen(buf1) - 1] = '\0';
		else sign = 1;
		if (strlen(buf1) == 0) continue;
		char* buf1_trim = trim(buf1);
		len = strlen(buf1_trim);
		if (len == 0)
		{
			free(buf1_trim);
			continue;
		}
		if (strcmp(buf1_trim, "exit") == 0)
		{
			free(buf1_trim);
			break;
		}
		else if (strcmp(buf1_trim, "ls") == 0)
			my_ls();
		else
		{
			for (int i = 0; i < len; ++i)
				if (buf1_trim[i] == ' ' || buf1_trim[i] == '\t')
				{
					buf1_trim[i] = '\0';
					break;
				}
			strcpy(cmd, buf1_trim);
			strcpy(buf2, &buf1_trim[strlen(buf1_trim) + 1]);
			char* buf2_trim = trim(buf2);

			if (strcmp(cmd, "mkdir") == 0 && strlen(buf2_trim) > 0)
				my_mkdir(buf2_trim);
			else if (strcmp(cmd, "rmdir") == 0 && strlen(buf2_trim) > 0)
				my_rmdir(buf2_trim);
			else
				printf("No such command: %s\n", buf1);

			free(buf2_trim);
		}
		if (sign == 1)
		{
			while (getchar() != '\n');
			sign = 0;
		}
		free(buf1_trim);
	}
	my_exitsys();
}

//int judge_dir(char* dirname)
//{
//	if (strcmp(dirname, "/") == 0) return 1;
//	unsigned short block1_parent = 5, block2_parent = 6;
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//	char buf[80];
//	int len = strlen(dirname);
//	for (int i = 0; i < len; ++i)
//		for (int j = i + 1; j <= len; ++j)
//			if (dirname[j] != '/' && dirname[j] != '\0')
//				buf[j - i - 1] = dirname[j];
//			else
//			{
//				buf[j - i - 1] = '\0';
//				block1_parent = BLOCKNUM;
//				fcb* fcbp = fcbop.find(fcbps1_parent, buf, NULL, 0);
//				if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, buf, NULL, 0);
//				if (fcbp == NULL)
//					return 0;
//				block1_parent = fcbp->first;
//				block2_parent = fat1->id[block1_parent];
//				fcbps1_parent = (fcbps)hardp[block1_parent];
//				fcbps2_parent = (fcbps)hardp[block2_parent];
//				i = j - 1;
//				break;
//			}
//	return 1;
//}
//
//void my_cd(char* dirname)
//{
//	char dir1[80], dir2[80], temp[80];
//	int count = 0;
//
//	if (dirname[0] != '/')
//	{
//		strcpy(dir1, currentdir);
//		if (strcmp(dir1, "/") != 0)
//			strcat(dir1, "/");
//		strcat(dir1, dirname);
//	}
//	//printf("dir: %s\n", dir1);
//
//	strcpy(dir2, "/");
//	int len = strlen(dir1);
//	for (int i = 0; i < len; ++i)
//		for (int j = i + 1; j <= len; ++j)
//			if (dir1[j] != '/' && dir1[j] != '\0')
//				temp[j - i - 1] = dir1[j];
//			else
//			{
//				temp[j - i - 1] = '\0';
//				if (strcmp(temp, ".") != 0)
//					if (strcmp(temp, "..") != 0)
//					{
//						if (strcmp(dir2, "/") != 0)
//							strcat(dir2, "/");
//						strcat(dir2, temp);
//						++count;
//					}
//					else if (strcmp(dir2, "/") != 0)
//					{
//						for (int k = strlen(dir2) - 1; k >= 0; --k)
//							if (dir2[k] == '/')
//							{
//								dir2[k] = '\0';
//								--count;
//								break;
//							}
//						if (strcmp(dir2, "") == 0)
//							strcat(dir2, "/");
//					}
//				i = j - 1;
//				break;
//			}
//	//printf("dir2: %s\n", dir2);
//	if (judge_dir(dir2) == 0)
//	{
//		printf("No such directory.\n");
//		return;
//	}
//
//	int space = useropenop.space_dir();
//	if (count > space)
//	{
//		printf("Open too many files.\n");
//		return;
//	}
//	useropenop.clean_dir();
//
//	unsigned short block1_parent = 5, block2_parent = 6;
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//
//	strcpy(currentdir, dir2);
//	if (strcmp(dir2, "/") == 0)
//		return;
//
//	strcpy(dir1, "/");
//	len = strlen(dir2);
//	for (int i = 0; i < len; ++i)
//		for (int j = i + 1; j <= len; ++j)
//			if (dir2[j] != '/' && dir2[j] != '\0')
//				temp[j - i - 1] = dir2[j];
//			else
//			{
//				temp[j - i - 1] = '\0';
//				fcb* fcbp = fcbop.find(fcbps1_parent, temp, NULL, 0);
//				if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, temp, NULL, 0);
//				block1_parent = fcbp->first;
//				block2_parent = fat1->id[block1_parent];
//
//				int idx = useropenop.get_id();
//				if (strcmp(dir1, "/") != 0)
//					strcat(dir1, "/");
//				strcat(dir1, temp);
//				useropenop.save(idx, fcbp, dir1);
//				curdir = idx;
//				fcbps1_parent = (fcbps)hardp[block1_parent];
//				fcbps2_parent = (fcbps)hardp[block2_parent];
//				i = j - 1;
//				break;
//			}
//}
//

//

//

//
//char* get_name(char* filename)
//{
//	char* name = (char*)malloc(sizeof(char) * 8);
//	memset(name, '\0', 8);
//	int idx = -1;
//	for (int i = strlen(filename) - 1; i >= 0; --i)
//		if (filename[i] == '.')
//		{
//			idx = i;
//			break;
//		}
//	if (idx == -1 || strlen(filename) - idx - 1 > 3 || idx == 0)
//		strncpy(name, filename, 7);
//	else
//	{
//		if (idx > 7) idx = 7;
//		for (int i = 0; i < idx; ++i)
//			name[i] = filename[i];
//	}
//	return name;
//}
//
//char* get_ext(char* filename)
//{
//	char* ext = (char*)malloc(sizeof(char) * 4);
//	memset(ext, '\0', 4);
//	int idx = -1;
//	for (int i = strlen(filename) - 1; i >= 0; --i)
//		if (filename[i] == '.')
//		{
//			idx = i;
//			break;
//		}
//	if (idx == -1 || strlen(filename) - idx - 1 > 3 || idx == 0)
//		return ext;
//	else
//		for (int i = idx + 1; i < strlen(filename); ++i)
//			ext[i - idx - 1] = filename[i];
//	return ext;
//}
//
//void my_create(char* filename)
//{
//	char* name = get_name(filename), * exname = get_ext(filename);
//
//	unsigned short block1_parent = openfilelist[curdir].first, block2_parent = fat1->id[block1_parent];
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//	fcb* fcbp = fcbop.find(fcbps1_parent, name, exname, 1);
//	if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, name, exname, 1);
//	if (fcbp != NULL)
//	{
//		printf("File exists.\n");
//		return;
//	}
//
//	unsigned short* ret = blockop.get(1);
//	if (ret == NULL)
//	{
//		printf("No space left.\n");
//		return;
//	}
//	unsigned short block1 = ret[0];
//	free(ret);
//	ret = NULL;
//
//	fcbp = fcbop.get(fcbps1_parent);
//	if (fcbp == NULL) fcbp = fcbop.get(fcbps2_parent);
//	if (fcbp == NULL)
//	{
//		printf("Enough files for the parent directory.\n");
//		return;
//	}
//	fat1->id[block1] = END;
//
//	strcpy(fcbp->filename, name);
//	strcpy(fcbp->exname, exname);
//	fcbp->attribute = 1;
//	fcbp->first = block1;
//	fcbp->length = 0;
//	fcbp->free = 1;
//}
//
//void my_rm(char* filename)
//{
//	char* name = get_name(filename), * exname = get_ext(filename);
//
//	unsigned short block1_parent = openfilelist[curdir].first, block2_parent = fat1->id[block1_parent];
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//	fcb* fcbp = fcbop.find(fcbps1_parent, name, exname, 1);
//	if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, name, exname, 1);
//	if (fcbp == NULL)
//	{
//		printf("File doesn't exist.\n");
//		return;
//	}
//
//	unsigned short block1 = fcbp->first;
//	while (fat1->id[block1] != END)
//	{
//		unsigned short t = fat1->id[block1];
//		fat1->id[block1] = FREE;
//		block1 = t;
//	}
//	fat1->id[block1] = FREE;
//
//	fcbp->free = 0;
//}
//
//int my_open(char* filename)
//{
//	char* name = get_name(filename), * exname = get_ext(filename);
//
//	unsigned short block1_parent = openfilelist[curdir].first, block2_parent = fat1->id[block1_parent];
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//	fcb* fcbp = fcbop.find(fcbps1_parent, name, exname, 1);
//	if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, name, exname, 1);
//	if (fcbp == NULL)
//	{
//		printf("File doesn't exist.\n");
//		return -1;
//	}
//
//	int idx = useropenop.get_id();
//	if (idx == -1)
//	{
//		printf("Open too many files.\n");
//		return -1;
//	}
//	useropenop.save(idx, fcbp, currentdir);
//	printf("fd: %d\n", idx);
//	return idx;
//}
//
//void my_close(int fd)
//{
//	int error = useropenop.check_fd(fd);
//	if (error != 0)
//	{
//		printf("No such file opened.\n");
//		return;
//	}
//
//	unsigned short block1_parent = openfilelist[curdir].first, block2_parent = fat1->id[block1_parent];
//	fcbps fcbps1_parent = (fcbps)hardp[block1_parent], fcbps2_parent = (fcbps)hardp[block2_parent];
//	fcb* fcbp = fcbop.find(fcbps1_parent, openfilelist[fd].filename, openfilelist[fd].exname, 1);
//	if (fcbp == NULL) fcbp = fcbop.find(fcbps2_parent, openfilelist[fd].filename, openfilelist[fd].exname, 1);
//	if (fcbp == NULL)
//	{
//		printf("File doesn't exist.\n");
//		return;
//	}
//	fcbp->length = openfilelist[fd].length;
//
//	error = useropenop.close_file(fd);
//	if (error != 0)
//	{
//		printf("No such file opened. %d\n", error);
//		return;
//	}
//	printf("success\n");
//}
//
//int my_write(int fd)
//{
//	int error = useropenop.check_fd(fd);
//	if (error != 0)
//	{
//		printf("No such file opened.\n");
//		return -1;
//	}
//
//	char buf[1024];
//	printf("Enter: ");
//	fgets(buf, 1024, stdin);
//	printf("%s\n", buf);
//	printf("Choose write style(1, 2, 3): ");
//	char style = 0;
//	scanf("%c", &style);
//	printf("Style: %c\n", style);
//	do_write(fd, buf, strlen(buf), style);
//}
//
//int do_write(int fd, char* text, int len, char wstyle)
//{
//	unsigned short block1 = openfilelist[fd].first;
//
//	if (wstyle == '1')
//	{
//		char* wp = (char*)hardp[block1];
//		strcpy(wp, text);
//
//		if (fat1->id[block1] != END)
//			blockop.clean_after(fat1->id[block1]);
//		fat1->id[block1] = END;
//
//		openfilelist[fd].length = len;
//		openfilelist[fd].count = len;
//		openfilelist[fd].fcbstate = 1;
//		printf("%d,%d\n", fd, openfilelist[fd].length);
//		return 0;
//	}
//
//	if (wstyle == '2' || wstyle == '3')
//	{
//		if (wstyle == 3) openfilelist[fd].count = openfilelist[fd].length;
//		int num = openfilelist[fd].count / (BLOCKSIZE - 1);
//		while (num--)
//			block1 = fat1->id[block1];
//
//		if (fat1->id[block1] != END)
//			blockop.clean_after(fat1->id[block1]);
//
//		char* wp = (char*)hardp[block1];
//		int wp_idx = openfilelist[fd].count % (BLOCKSIZE - 1);
//		if (wp_idx + len <= BLOCKSIZE)
//		{
//			for (int i = wp_idx; i < wp_idx + len; ++i)
//				wp[i] = text[i - wp_idx];
//			openfilelist[fd].length = openfilelist[fd].count + len;
//			openfilelist[fd].count = openfilelist[fd].length;
//			openfilelist[fd].fcbstate = 1;
//			return 0;
//		}
//		else
//		{
//			for (int i = wp_idx; i < BLOCKSIZE; ++i)
//				wp[i] = text[i - wp_idx];
//			fat1->id[block1] = blockop.get(1)[0];
//			block1 = fat1->id[block1];
//			fat1->id[block1] = END;
//
//			wp = (char*)hardp[block1];
//			for (int i = BLOCKSIZE - wp_idx; i < len; ++i)
//				wp[i - BLOCKSIZE + wp_idx] = text[i];
//
//			openfilelist[fd].length = openfilelist[fd].count + len;
//			openfilelist[fd].count = openfilelist[fd].length;
//			openfilelist[fd].fcbstate = 1;
//			return 0;
//		}
//	}
//	return -1;
//}
//
//int my_read(int fd)
//{
//	int idx = 0, len = 0;
//	printf("Enter read position: ");
//	scanf("%d", &idx);
//	printf("Enter length to read: ");
//	scanf("%d", &len);
//
//	int error = useropenop.check_fd(fd);
//	if (error != 0)
//	{
//		printf("No such file opened.\n");
//		return -1;
//	}
//
//	if (len > BLOCKSIZE || len < 1)
//	{
//		printf("Param len error.\n");
//		return -1;
//	}
//
//	char* buf = (char*)malloc(sizeof(char) * len);
//	error = do_read(fd, len, buf, idx);
//	if (error != 0)
//	{
//		printf("Read error. %d\n", error);
//		free(buf);
//		return error;
//	}
//	printf("%s\n", buf);
//	free(buf);
//	return 0;
//}
//
//int do_read(int fd, int len, char* text, int idx)
//{
//	openfilelist[fd].count = idx;
//	if (openfilelist[fd].count + len > openfilelist[fd].length)
//	{
//		printf("Error: %d,%d,%d,%d\n", fd, openfilelist[fd].count, len, openfilelist[fd].length);
//		return -1;
//	}
//
//	unsigned short block1 = openfilelist[fd].first;
//
//	int num = openfilelist[fd].count / (BLOCKSIZE - 1);
//	while (num--)
//		block1 = fat1->id[block1];
//
//	char* wp = (char*)hardp[block1];
//
//	if (BLOCKSIZE - openfilelist[fd].count >= len)
//		for (int i = openfilelist[fd].count; i < openfilelist[fd].count + len; ++i)
//			text[i - openfilelist[fd].count] = wp[i];
//	else
//	{
//		for (int i = openfilelist[fd].count; i < BLOCKSIZE; ++i)
//			text[i - openfilelist[fd].count] = wp[i];
//		block1 = fat1->id[block1];
//		wp = (char*)hardp[block1];
//		for (int i = 0; i < len - (BLOCKSIZE - openfilelist[fd].count); ++i)
//			text[i + BLOCKSIZE - openfilelist[fd].count] = wp[i];
//	}
//	return 0;
//}
//

//

