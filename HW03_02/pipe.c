#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void pipe_test()
{
	int filedes[2], pid[3] = { 0, 0, 0 }, error = 0, n = 0, status;
	char buf[4096 * 16];

	if (error = pipe(filedes))
	{
		perror("pipe error");
		exit(-1);
	}

	for (int i = 0; i < 3; ++i)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("fork error");
			exit(-1);
		}
		else if (pid[i] == 0)
		{
			memset(pid, '\0', 12);
			pid[i] = -(i + 1);
			close(filedes[0]);
			break;
		}
	}
	if (pid[0] > 0)
		close(filedes[1]);

	if (pid[0] > 0)
	{
		sleep(5);
		n = read(filedes[0], buf, 4096 * 16);
		printf("read %d byte(s) from pipe\n", n);
		n = read(filedes[0], buf, 100);
		printf("read %d byte(s) from pipe\n", n);
		printf("wait 5 seconds to read\n");
		n = read(filedes[0], buf, 100);
		printf("read %d byte(s) from pipe\n", n);
		close(filedes[0]);
	}

	if (pid[0] == -1)
	{
		memset(buf, '1', 4096 * 16);
		n = write(filedes[1], buf, strlen(buf));
		printf("write %d byte(s) to pipe\n", n);
		printf("wait 5 seconds to write\n");
		n = write(filedes[1], "1", 1);
		printf("write %d byte(s) to pipe\n", n);
		close(filedes[1]);
		exit(0);
	}

	if (pid[1] == -2)
	{
		sleep(10);
		memset(buf, '1', 4096 * 16);
		n = write(filedes[1], buf, 99);
		printf("write %d byte(s) to pipe\n", n);
		close(filedes[1]);
		exit(0);
	}

	if (pid[2] == -3)
	{
		sleep(15);
		n = write(filedes[1], "1", 1);
		printf("write %d byte(s) to pipe\n", n);
		close(filedes[1]);
		exit(0);
	}

	wait(NULL);
	wait(NULL);
	wait(&status);
	if (WIFEXITED(status))
		printf("child exit with %d\n", WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		printf("child killed by %d\n", WTERMSIG(status));

	exit(0);
}

int main()
{
	pipe_test();

	return 0;
}

