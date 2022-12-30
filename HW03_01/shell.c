#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	char buf[256];
	char* pre = "root@wy-virtual-machine:/home/wy/os_hw/HW3_1# ";
	static char* argv[] = {};
	pid_t pid = 0;
	while (true)
	{
		memset(buf, 0, 256);
		printf("%s", pre);
		fgets(buf, 256, stdin);
		if (!strcmp(buf, "cmd1\n"))
			if (!(pid = vfork()))
				execv("cmd1", argv);
			else
				waitpid(pid, 0, 0);
		else if (!strcmp(buf, "cmd2\n"))
			if (!(pid = vfork()))
				execv("cmd2", argv);
			else
				waitpid(pid, 0, 0);
		else if (!strcmp(buf, "cmd3\n"))
			if (!(pid = vfork()))
				execv("cmd3", argv);
			else
				waitpid(pid, 0, 0);
		else if (!strcmp(buf, "ls\n"))
			if (!(pid = vfork()))
				execv("/bin/ls", argv);
			else
				waitpid(pid, 0, 0);
		else if (!strcmp(buf, "exit\n"))
			break;
		else
			printf("Command not found.\n");
	}
	return 0;
}
