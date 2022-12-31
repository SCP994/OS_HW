#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void child(const char* cmd, char* const argv[])
{
	pid_t pid = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork error: ");
		exit(-1);
	}
	if (pid == 0)
		execv(cmd, argv);
	if (pid > 0)
		waitpid(pid, NULL, 0);
}

int main()
{
	char buf[10];
	pid_t pid = 0;
	int sign = 0;

	while (1)
	{
		printf("virtual-shell# ");
		fgets(buf, 10, stdin);
		if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
		else sign = 1;

		if (strcmp(buf, "cmd1") == 0)
			child("cmd1", NULL);
		else if (strcmp(buf, "cmd2") == 0)
			child("cmd2", NULL);
		else if (strcmp(buf, "cmd3") == 0)
			child("cmd3", NULL);
		else if (strcmp(buf, "ls") == 0)
		{
			char* argv[] = { "ls", "-a", "-l", NULL };
			child("/usr/bin/ls", argv);
		}
		else if (strcmp(buf, "exit") == 0)
			break;
		else
			printf("No such command: %s\n", buf);

		if (sign == 1)
		{
			while (getchar() != '\n');
			sign = 0;
		}
	}
	return 0;
}

