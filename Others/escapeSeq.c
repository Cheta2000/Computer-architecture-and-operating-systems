#include <stdio.h>

int main(){
	for(int i=30;i<=37;i++){
		printf("\e[0;%dmHello world!\e[00m\n",i);
	}
}
