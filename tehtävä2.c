#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define die(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0);
#define BUFSIZE 80

int main(int argc, char * * argv)
{
	int i,pid; 
	char buffer[BUFSIZE];
	char command[256];
	int newpipe[2];
	command[0]='\0';
	/* concatenate arguments to one long string */
	for (i=1; i<argc; i++) {
		strcat(command,argv[i]);
		strcat(command," ");
	}
	if(pipe(newpipe) < 0){
		exit(1);
	}
	
	/* run the command and read its output via pipe */
	pid = fork();
    if(pid == 0){
        dup2(newpipe[1],STDIN_FILENO);
		close(newpipe[1]);
		close(newpipe[0]);
		if(execvp(argv[1], argv+1) == -1) {
            perror("execvp");
            exit(1);
        }
    }
    else{
		close(newpipe[0]);
		close(newpipe[1]);
        read(newpipe[0],buffer,BUFSIZE);
    }	
	return 0;
}
