#include "save_load.h"

int save()
{
	FILE* fp = fopen("saved", "wb+");
	fwrite((char*)myvhard, sizeof(char), SIZE, fp);
	fclose(fp);
	return 0;
}

int load()
{
	FILE* fp = fopen("saved", "rb");
	if (fp == NULL)
		return -1;
	fread((char*)myvhard, sizeof(char), SIZE, fp);
	fclose(fp);
	return 0;
}

