#include "block.h"

extern fat* fat1;

unsigned short* get_blocks(int num)
{
	unsigned short* ret = (unsigned short*)malloc(sizeof(unsigned short) * num);
	int idx = 0;
	for (int i = 0; i < BLOCKNUM; ++i)
	{
		if (fat1->id[i] == FREE) ret[idx++] = i;
		if (idx == num) break;
	}
	if (idx < num)
	{
		free(ret);
		ret = NULL;
	}
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

