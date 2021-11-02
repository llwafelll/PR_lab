#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid = getpid();
	printf("Running `myinfo` pid of this process is %d\n", pid);

	if (argc == 3)
		printf("Marcin Szram pid=%s, arg=%s\n", argv[1], argv[2]);

	else if (argc == 2)
		printf("Marcin Szram pid=%s\n", argv[1]);


	else if (argc == 1)
		printf("Marcin Szram [pid not provided]\n");

	else
		printf("Invalid number of arguments.\n");

	return 0;
}
