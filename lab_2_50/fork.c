#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pomiar_czasu.h>

int main(char argc, char *argv[]){
  
	int pid, wynik, i;
	char arg[10];
	char pids[10];
	pid_t parent_pid = getpid();
    
	pid = fork(); 			
	sprintf(pids, "%d", parent_pid);

	if (argc <= 1) sprintf(arg, "");
	if (argc == 2) sprintf(arg, "%s", argv[1]);

    
    if(pid==0){ 			

		char *args[] = {"myinfo", pids, arg, NULL};
		wynik = execv("./myinfo", args);
		if(wynik==-1)
			printf("Proces potomny nie wykonal programu\n");

		exit(0);
      
	} else
		wait(NULL);
  
}

  
