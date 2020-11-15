#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("Usage: %s\n", argv[0]);
		return 1;
	}
	
	pid_t pid = fork();
	if (pid > 0)
		execvp(argv[1], argv + 1);
	else if (pid == 0)
		wait((int *) 0);
	else
		perror("fork failed");
		return 1;
}


