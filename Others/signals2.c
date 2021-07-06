#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
        kill(1,9);
        printf("Status inita:\n");
        system("ps -q 1 -eo stat");
}




