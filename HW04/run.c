#include "cmd.h"

int main(void)
{
	printf("%ld\n", sizeof(fcb));
	my_format();

	my_mkdir("a");

	my_mkdir("b");

	my_ls();

	my_rmdir("b");

	my_ls();

	my_exitsys();

}

