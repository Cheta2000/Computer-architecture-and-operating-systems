#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

int main(int argc, char** argv){
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	char buf[1024];
	char logins[30][20];
	char* messTo;
	char* mess;
	int port=1234;
	int socketfd,new,sd,max_sd,len,size;
	bool exist=0;
	fd_set readfds;
	fd_set master;
	FD_ZERO(&readfds);
	FD_ZERO(&master);
	for(int i=0;i<20;i++){
		strcpy(logins[i],"[unknown]");
	}
	if((socketfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("Server failed\n");
		exit(1);
	}
	printf("Server created\n");
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);
	if(bind(socketfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		perror("Bind failed\n");
		exit(1);
	}
	printf("Binded\n");
	if(listen(socketfd,10)==-1){
		perror("Listen failed\n");
		exit(1);
	}
	printf("Listening...\n");
	FD_SET(socketfd,&master);
	max_sd=socketfd;
	while(1){
		readfds=master;
		if(select(max_sd+1,&readfds,NULL,NULL,NULL)==-1){
			perror("Select failed\n");
			exit(1);
		}
		printf("Selected\n");
		for(int i=0;i<=max_sd;i++){
			if(FD_ISSET(i,&readfds)){
				if(i==socketfd){
					len=sizeof(cli_addr);
					if((new=accept(socketfd,(struct sockaddr*)&cli_addr,&len))==-1){
						perror("Accept failed\n");
					}
					else{	
						printf("Accepted\n");
						FD_SET(new,&master);
						if(new>max_sd){
							max_sd=new;
						}
					printf("New connection from %s on socket %d\n",inet_ntoa(cli_addr.sin_addr), new);
					}
				}
				else{
					size=read(i,buf,1024);
					if(size<=0){
						if(size==0){
							printf("Client left\n");
							strcpy(logins[i],"[left]");
							for(int k=0;k<=max_sd;k++){
								if(FD_ISSET(k,&master)){
									if(k!=socketfd && k!=i){
										send(k,"Users: \n",8,0);
										for(int j=socketfd+1;j<=max_sd;j++){
											if(send(k,logins[j],strlen(logins[j]),0)==-1 || send(k,"\n",1,0)==-1){
												perror("Send failed\n");
											}
										}						
									}
								}
							}
						}
						else{
							perror("Recv failed\n");
						}
						close(i);
						FD_CLR(i,&master);
					}
					else if(strcmp(logins[i],"[unknown]")==0){
						strcpy(logins[i],strtok(buf,"\n"));
						printf("Login: %s\n",logins[i]);
						if(send(i,"Your login: ",13,0)==-1 || send(i,logins[i],strlen(logins[i]),0)==-1 || send(i,"\n",1,0)==-1){
							perror("Send failed\n");
						}
						for(int k=0;k<=max_sd;k++){
							if(FD_ISSET(k,&master)){
								if(k!=socketfd){
									send(k,"Users: \n",8,0);
									for(int j=socketfd+1;j<=max_sd;j++){
										if(send(k,logins[j],strlen(logins[j]),0)==-1 || send(k,"\n",1,0)==-1){
											perror("Send failed\n");
										}
									}					
								}
							}
						}		
					}
					else{
						messTo=strtok(buf,":");
						mess=strtok(NULL,"\n");
						exist=0;
						for(int j=socketfd+1;j<=max_sd;j++){
							if(FD_ISSET(j,&master)){
								if(strcmp(logins[j],messTo)==0){
									if(send(j,"Message from ",14,0)==-1 || send(j,logins[i],strlen(logins[i]),0)==-1 || send(j,": ",2,0)==-1 || send(j,mess,strlen(mess),0)==-1 || send(j,"\n",1,0)==-1){
										perror("Send failed\n");
									}
									exist=1;
								}
							}
						}
						if(exist==0){
							send(i,"*User does not exist*\n",23,0);
						}
					}
				}
			}
		}
	}
	return 0;
}
