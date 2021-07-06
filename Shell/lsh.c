#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int bgflag;
int pipeflag;
int rdrflag;
int id;
char olds[50];
char news[50];

void init(){
	system("clear");
	printf("*******************\nWelcome in my shell\n");
}

void cd_command(char **args){
	for(int i=0;i<50;i++){
		olds[i]=news[i];
	}
	getcwd(news,sizeof(news));
	if(args[1]!=NULL){
		if(strcmp(args[1],"..")==0){
			chdir("..");
		}
		else if(strcmp(args[1],"-")==0){
			chdir(olds);
		}
		else{
			chdir(args[1]);
		}
	}
	else{
		chdir("/home/mateusz");
	}
}

struct pipe_commands{
	char ** comm;
};

char* read_line(){
	char* line=malloc(sizeof(char)*1024);
	int position=0,counter=0;
	char c;
	c=getchar();
	if(c==EOF){
		exit(0);
	}
	while(c==' '){
		c=getchar();
	}
	while(c!='\n'){
		if(c=='&'){
			bgflag=1;
			break;
		}
		else{
			if(c=='|'){
				pipeflag=1;
			}
			else if(c=='>'){
                        	rdrflag=1;
                	}
               		else if(c=='<'){
                        	rdrflag=2;
                	}
                	else if(c=='2'){
			        c=getchar();
                        	if(c=='>'){
                                	rdrflag=3;
                        	}
                       		else{
                                	counter=counter+1;
                                	line[position]='2';
                                	position++;
                            	}
                	}
			counter++;
                        line[position]=c;
                        position++;
			if(c!='\n'){
				c=getchar();
			}
		}
		if(c==EOF){
			exit(0);
		}
	}
	if(counter==0){
		return "";
	}
	else{
		return line;
	}
}

char** take_args(char* line){
	char* token;
	char** args=malloc(sizeof(char*)*1024);
	int position=0;
	token=strtok(line," ");
	while(token!=NULL){
		args[position]=token;
		position++;
		token=strtok(NULL," ");
	}
	args[position]=NULL;
	return args;
}

struct pipe_commands* pipe_take_args(char* line){
	char* token;
	char** args=malloc(sizeof(char*)*1024);
	struct pipe_commands* commands=malloc(sizeof(struct pipe_commands)*1024);
	int argsposition=0,commandsposition=0;
	token=strtok(line," ");
	while(token!=NULL){
		if(strcmp(token,"|")==0){
			args[argsposition]=NULL;
			commands[commandsposition].comm=args;
			args=malloc(sizeof(char*)*1024);
			commandsposition++;
			argsposition=0;
		}
		else{
			args[argsposition]=token;
			argsposition++;
		}
		token=strtok(NULL," ");
	}
	args[argsposition]=NULL;
	commands[commandsposition].comm=args;
	return commands;
}

char** redirection_take_args(char* line,char** place){
	char* token;
        char** args=malloc(sizeof(char*)*1024);
        int position=0;
        token=strtok(line," ");
        while(token!=NULL){
                if(strcmp(token,"<")==0 || strcmp(token,">")==0 || strcmp(token,"2>")==0){
			token=strtok(NULL," ");
	           	*place=token;
			break;
		}
                else{
                        args[position]=token;
                        position++;
                }
                token=strtok(NULL," ");
        }
        args[position]=NULL;
        return args;
}


void execute(char** args){
	int status;
	id=fork();
	if(id<0){
		perror("Fail");
		return;
	}
	if(id==0){
		if(execvp(args[0],args)<0){
			write(2,"Unknown command\n",strlen("Unknown command\n"));
		}
		exit(1);
	}
	else{
		if(bgflag==0){
			waitpid(id,&status,0);
		}
	}
}

void pipe_execute(struct pipe_commands* commands){
	int fd[2];
	int status;
	int out=0;
	int counter=0;
	while ((commands[counter].comm)!=NULL) {
		pipe(fd);
		id=fork();
		if(id<0){
			perror("Fail");
			return;
		}
		if(id==0){
			dup2(out, 0);
			if((commands[counter+1].comm)!=NULL){
				dup2(fd[1],1);
			}
			close(fd[0]);
			execvp(commands[counter].comm[0],commands[counter].comm);
			exit(1);
		}
		else{
			if(bgflag==0){
				waitpid(id,&status,0);
			}
			close(fd[1]);
			out=fd[0];
			counter++;
		}
	}
}

void redirection_execute(char** args, char* place){
	int status;
	id=fork();
	int fd1;
	if(id<0){
		perror("Fail");
		return;
	}
	if(id==0){
		if(rdrflag==1){
			fd1=open(place,O_WRONLY|O_CREAT,0644);
			dup2(fd1,1);
			close(fd1);
		}
		if(rdrflag==2){
			int fd2=open(place,O_RDONLY);
			dup2(fd2,0);
			close(fd2);
		}
		if(rdrflag==3){
			int fd3=open(place,O_WRONLY|O_CREAT,0644);
			dup2(fd3,2);
			close(fd3);
		}
		if(execvp(args[0],args)<0){
			write(2,"Unknown command\n",strlen("Unknown command\n"));
		}
	}
	else{
		if(bgflag==0){
			waitpid(id,&status,0);
		}
	}
}

void shell(){
	char* line;
	while(1){
		id=0;
		rdrflag=0;
		bgflag=0;
		pipeflag=0;
		printf(">>>");
		line=read_line();
		if(strcmp(line,"exit")==0){
			break;
		}
		if(!(strcmp(line,"")==0)){
			if(pipeflag==1){
				struct pipe_commands* commands;
				commands=pipe_take_args(line);
				pipe_execute(commands);
				free(commands);
			}
			else if(rdrflag>0){
				char** args;
				char* place;
				args=redirection_take_args(line,&place);
				redirection_execute(args,place);
				free(args);
			}
			else{
				char** args;
				args=take_args(line);
				if(strcmp(args[0],"cd")==0){
					cd_command(args);
				}
				else{
					execute(args);
				}
				free(args);
			}
		}
		if(strcmp(line,"")!=0){
			free(line);
		}
	}
}

void handle_sigint(int sig){
	signal(SIGINT, handle_sigint);
       	if(id!=0){
        	kill(id,SIGINT);
		printf("Stopped\n");
	}
}


int main(){
	signal(SIGINT,handle_sigint);
	signal(SIGCHLD,SIG_IGN);
	init();
	shell();
	return 0;
}

