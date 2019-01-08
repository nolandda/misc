#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int run_command(char* const * cmd) {

  pid_t   childpid = 0;
  int child_status = 0;
  pid_t tpid = 0;
  int rv = 0;
  
  if((childpid = fork()) == -1)
  {
    fprintf(stderr, "ERROR: command fork() failed [%s]\n", strerror(errno));
    exit(-1);
  }

  if(childpid == 0)
  {
    rv = execvp(cmd[0], cmd);
    if(rv < 0) {
      fprintf(stderr, "ERROR: execvp() failed [%s]\n", strerror(errno));
    }
    fprintf(stderr, "ERROR: CANNOT HAPPEN\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    // Wait for child to complete
    do {
      tpid = wait(&child_status);
    } while(tpid != childpid);
  }

  return child_status;
}



int run_command_and_pipe_input(char* const * cmd,
			       const char* inputstr) {

  int pipefd[2];
  pid_t childpid;
  int rv = 0;
  int child_status = 0;
  pid_t tpid = 0;
  rv = pipe(pipefd);
  if( rv < 0 ) {
    fprintf(stderr, "ERROR: command pipe() failed [%s]\n", strerror(errno));
    exit(-1);
  }
  
  if((childpid = fork()) == -1)
  {
    fprintf(stderr, "ERROR: command fork() failed [%s]\n", strerror(errno));
    exit(-1);
  }

  if(childpid == 0)
  {
    rv = dup2(STDIN_FILENO, pipefd[0]);         //replacing stdin with pipe read
    if(rv < 0) {
      fprintf(stderr, "ERROR: dup2() failed [%s]\n", strerror(errno));
      exit(-1);
    }    
    close(pipefd[1]);       //closing pipe write
    close(pipefd[0]);

    rv = execvp(cmd[0], cmd);
    if(rv < 0) {
      fprintf(stderr, "ERROR: execvp() failed [%s]\n", strerror(errno));
    }
    fprintf(stderr, "ERROR: CANNOT HAPPEN\n");
    exit(-1);
  }
  else
  {
    // Parent process closes up input side of pipe 
    close(pipefd[0]);

    // Send inputstr through the output side of pipe 
    write(pipefd[1], inputstr, (strlen(inputstr)+1));

    do {
      tpid = wait(&child_status);
    } while(tpid != childpid);
  }

  close(pipefd[0]);
  close(pipefd[1]);

  return child_status;
}

