#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int sig){
	printf("\nObsluzono sygnal: %d\n",sig);
}

int main(){
	for(int i=1;i<=63;i++){
		if(signal(i,sig_handler)==SIG_ERR){	
			printf("Nie mozna przechwycic sygnalu: %d\n",i);
		}
	}
 	return 0;
}
