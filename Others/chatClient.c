#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int readwrite(int in,int out){
	char s[1024];
	int i=0;
	while(read(in,s+i,1)!=0){
		if(s[i]=='\n'){
			write(out,s,i+1);
			return i+1;
		}
		i++;
	}
	return 0;
}

int main(int argc, char** argv){
	struct sockaddr_in serv_addr;
	int port=1234,size,socketfd,max_sd;
	char buf[1024];
	fd_set readfds,master;
	if((socketfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("Socket failed\n");
		exit(1);
	}
	printf("Client created\n");
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	if(connect(socketfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		perror("Connection failed\n");
		exit(1);
	}
	printf("Connected to server\n");
	printf("Message format: [nick]:[message]\n");
	printf("Enter a login: \n");
	FD_ZERO(&master);
	FD_ZERO(&readfds);
	FD_SET(0,&master);
	FD_SET(socketfd,&master);
	max_sd=socketfd;
	while(1){
		readfds=master;
		if(select(max_sd+1,&readfds,NULL,NULL,NULL)==-1){
			perror("Select failed\n");
			exit(1);
		}
		if(FD_ISSET(0,&readfds)){
			size=read(0,buf,1024);
			if(size==0){
				close(0);
				close(socketfd);
				exit(1);
			}
			else{
				write(socketfd,buf,size);
			}
		}
		else if(FD_ISSET(socketfd,&readfds)){
			size=read(socketfd,buf,1024);
			if(size==0){
				close(0);
				close(socketfd);
				exit(1);
			}
			else{	
				write(1,buf,size);
			}
		}	
		
	}
	return 0;
}

