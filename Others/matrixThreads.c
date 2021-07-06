#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

int** m1;
int** m2;
int** score;
int s,col=0;
pthread_mutex_t lock;

void* count(void* x){
	pthread_mutex_lock(&lock);
	int set=col;
	col++;
	pthread_mutex_unlock(&lock);
	int sum=0;
	for(int i=0;i<s;i++){
		for(int j=0;j<s;j++){
			sum=sum+m1[set][j]*m2[j][i];
		}
		score[set][i]=sum;
		sum=0;
	}

	return NULL;
}


int main(){
	int w,check=0;
	printf("Podaj wielkosc macierzy i ilosc watkow: ");
	scanf("%d %d",&s,&w);
	assert(w>0);
	pthread_t tids[w];
	m1=malloc(s*sizeof(int*));
	m2=malloc(s*sizeof(int*));
	score=malloc(s*sizeof(int*));
	for(int i=0;i<s;i++){
		m1[i]=malloc(s*sizeof(int));
		m2[i]=malloc(s*sizeof(int));
		score[i]=malloc(s*sizeof(int));
	}
	srand(time(NULL));
	for(int i=0;i<s;i++){
		for(int j=0;j<s;j++){
			m1[i][j]=rand()%2;
			m2[i][j]=rand()%2;
		}
	}
	printf("Macierz 1: \n");
	for(int i=0;i<s;i++){
		for(int j=0;j<s;j++){
			printf("%d ",m1[i][j]);
		}
		printf("\n");
	}
	printf("Macierz 2: \n");
        for(int i=0;i<s;i++){
                for(int j=0;j<s;j++){
                        printf("%d ",m2[i][j]);
                }
                printf("\n");
        }
	pthread_mutex_init(&lock,NULL);
	for(int i=0;i<w;i++){
		if(check<s){
			pthread_create(tids+i,NULL,count,NULL);
			check++;
		}
	}
	while(check<s){
		for(int i=0;i<w;i++){
			if(check<s){
				pthread_join(tids[i],NULL);
				pthread_create(tids+i,NULL,count,NULL);
				check++;
			}
		}
	}
	for(int i=0;i<w;i++){
		if(w<=s){
			pthread_join(tids[i],NULL);
		}
	}
	printf("Wynik: \n");
        for(int i=0;i<s;i++){
                for(int j=0;j<s;j++){
                        printf("%d ",score[i][j]);
                }
                printf("\n");
        }
	pthread_mutex_destroy(&lock);
	pthread_exit(NULL);
}
