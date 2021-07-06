#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int counter=0;

void handler(int sig){
	counter++;
}

int main(){
	int status;
	if(fork()==0){
		for(int i=0;i<100;i++){
			kill(getppid(),SIGUSR1);
		}
		printf("Dziecko wyslalo 100 sygnalow do rodzica i wychodzi\n");
		exit(1);
	}
	else{
		signal(SIGUSR1,handler);
		wait(&status);
		printf("Rodzic otrzymal %d sygnalow\n",counter);
	}
}
