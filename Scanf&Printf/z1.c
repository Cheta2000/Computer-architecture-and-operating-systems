#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


char* numSys(int number,int base){
	static char sign[]="0123456789ABCDEF";
	char* score=malloc(sizeof(char)*50);
	int position=0;
	char* rvrscore=malloc(sizeof(char)*50);
	do{
		score[position]=sign[number%base];
		position++;
		number=number/base;
	}while(number!=0);
	for(int i=strlen(score)-1;i>=0;i--){
		rvrscore[strlen(score)-1-i]=score[i];
	}
	rvrscore[position]='\0';
	return rvrscore;
}

void myscanf(char* str, ...){
	va_list vl;
	char* p=(char*)&str+sizeof(str);
	int i=0,j=0;
	int* res=&i;
	char c;
	char buff[1024] = {0};
	char *out_loc;
	char** sres;
	while(c!='\n'){ 
		if(read(0,&c,1)){
			buff[i]=c;
			i++;
		}
	}
	va_start(vl,str);
	i=0;
	while(str && str[i]){
		if(str[i]=='%'){
			i++;
			switch (str[i]){
				case 's':
					out_loc=(char*)va_arg(vl,char*);	
					strcpy(out_loc,&buff[j]);
					j=j+strlen(&buff[j]);
					break;
				case 'd': 
					*(int*)va_arg(vl,int*)=strtol(&buff[j],&out_loc,10);
					j+=out_loc -&buff[j];
					break;
				case 'x': 
					*(int*)va_arg(vl, int*)=strtol(&buff[j],&out_loc,16);
					j+=out_loc -&buff[j];
					break;
				case 'b':
					*(int*)va_arg(vl,int*)=strtol(&buff[j],&out_loc,2);
					j=j+out_loc-&buff[j];
					break;
			}
		} 
		else{
			buff[j]=str[i];
			j++;
		}
		i++;
	}
	va_end(vl);
}






void myprintf(char* format, ...){
	char *p=(char*)( &format + sizeof(format));
	char c=format[0];
	int i=1;
	int iArg;
	char* sArg=malloc(sizeof(char)*1024);
	while(c!='\0'){
		while(c!='%' && c!='\0'){
			write(1,&c,1);
			c=format[i];
			i++;
		}
		if(c=='%'){
			c=format[i];
			i++;
			switch(c){
				case 's':
					sArg=*((char**)p);
					p=p+sizeof(char*);
					write(1,sArg,strlen(sArg));
					break;
				case 'd':
					iArg=*((int*)p);
					p=p+sizeof(int*);
					if(iArg<0){
						write(1,"-",1);
               	                  	        iArg=-iArg;
                          	      }
					sArg=numSys(iArg,10);
					write(1,sArg,strlen(sArg));
					break;
				case 'x':
					iArg=*((int*)p);
					p=p+sizeof(int*);
					if(iArg<0){
						write(1,"-",1);
						iArg=-iArg;
					}
		                        sArg=numSys(iArg,16);
					write(1,sArg,strlen(sArg));
					break;
				case 'b':
					iArg=*((int*)p);
					p=p+sizeof(int*);
					if(iArg<0){
		                                write(1,"-",1);
		                                iArg=-iArg;
		                        }
		                        sArg=numSys(iArg,2);
					write(1,sArg,strlen(sArg));
					break;
				default:
					write(1,"%",1);
					write(1,&c,1);
				}
			c=format[i];
			i++;
		}
		else{
			write(1,&c,1);
		}
	}
	p=NULL;
}



int main(){
	int a=23;
	int b=178;
	int bin;
	char* p="check";
	char st[20];
	myprintf("Wynik to: %s %d %b %x\n",p,a,b,a);
	myprintf("Podaj dec hex bin str\n");
	myscanf("%d %x %b %s",&a,&b,&bin,&st);
	myprintf("Wynik 2 to: %d %d %d %s",a,b,bin,st);
	return 0;
}

