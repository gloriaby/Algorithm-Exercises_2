#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 200
//#define N 8

struct LinkedNode{
	int no;
	int weight;
	struct LinkedNode* next;
};

struct LinkedNode** get2Dlist(FILE* fp);
void free2Dlist(struct LinkedNode** list);




int main(){
	char* filename="dijkstraData.txt";
//	char* filename="testcase1.txt";
	FILE* fp=fopen(filename,"r");
	struct LinkedNode** AJList=get2Dlist(fp);
	fclose(fp);

	int X[N]={0};
	int X_size=0;//number of non zero values in array X: number of vertexes in set X
	int A[N]={0};//shortest paths array

	X[0]=1;//source vertex:1
	X_size=1;
	//A[0]=0;
	
	int v,w;
	int w_old=0;//if w_old == w: no node is added to X in this iteration
	struct LinkedNode* temp;
	int min;
	while(X_size<N){
		v=1;
		min=INT_MAX;
		while(v<=N){
			if(X[v-1]==0){//v is not in X
				v++;
				continue;
			}
			temp=*(AJList+v-1);
//			printf("check no %d\n",temp->no);/////////////////
			while(temp!=NULL){
//				printf("---temp->no:%d\t",temp->no);/////////////
				if((X[temp->no-1]==0)&&(temp->weight+A[v-1] < min)){
					w=temp->no;
					min=temp->weight+A[v-1];
//					printf("\nupdate min=%d with no %d and edge %d\n",min, temp->no, temp->weight);/////////////
				}
				temp=temp->next;
			}
			v++;
		}
//		printf("found  w:%d\n",w);/////////////////////
		X_size++;//if no (v,w) is found: no path between two specific nodes, X_size still ++
		if(w!=w_old){
			A[w-1]=min;
			X[w-1]=1;
//			printf("add node w:%d to X; X[w-1]=%d; A[w-1]=%d\n",w,X[w-1],A[w-1]);
		}

		w_old=w;
	}

	int j;
	printf("\nprint shortest paths from source node 1 to all other nodes:\n");
	for(j=0;j<N;j++){
		if(X[j]==0){//no path between node 1 to node j+1
			A[j]=1000000;
		}
		printf("A[%d]=%d\n",j,A[j]);
	}
	printf("\n");

/*
	int j;
	for(j=1;j<=N;j++){
		if((j==7)||(j==37)||(j==59)||(j==82)||(j==99)||(j==115)||(j==133)||(j==165)||(j==188)||(j==197)){
			printf("%d,",A[j-1]);
		}
	}
*/
	free2Dlist(AJList);

	return 0;	
}

struct LinkedNode** get2Dlist(FILE* fp){
	if(fp==NULL){
		fprintf(stderr,"Can not open file.\n");
		exit(1);
	}

	struct LinkedNode** list=malloc(sizeof(struct LinkedNode*)*N);

	//if do not initialize all pointers to be NULL, when access *(list+i),
	//may get error from valgrind:Conditional jump or move depends on uninitialised value(s)
	int i;
	for(i=0;i<N;i++){
		*(list+i)=NULL;
	}

	struct LinkedNode* temp=NULL;

	char *line=NULL;

	char *data;//used to set position in the line
	int offset;

	size_t len=0;
	int no, weight;
	i=0;
	while(getline(&line, &len, fp)!=-1){
//		printf("%s",line);
		data=line;
		if(sscanf(data, "%d\t%n", &no, &offset)!=EOF){//the first entry is the node number
			data+=offset;
//			printf("no:%d\n",no);/////////////////////

			while(sscanf(data, "%d,%d\t%n", &no, &weight,&offset)!=EOF){
				data+=offset;
//				printf("%d,%d\t",no,weight);///////////////////
				struct LinkedNode* newnodep=malloc(sizeof(struct LinkedNode));
				newnodep->no=no;
				newnodep->weight=weight;
				newnodep->next=NULL;
				if(*(list+i)==NULL){
					*(list+i)=newnodep;
				}
				else{
					temp->next=newnodep;
				}

				temp=newnodep;
			}
//			printf("\n");//////////////////////
		}
		i++;
	}
	free(line);
	return list;
}


void free2Dlist(struct LinkedNode** list){
	int i;
	struct LinkedNode* temp1=NULL;
	struct LinkedNode* temp2=NULL;
	for(i=0;i<N;i++){
		temp1=*(list+i);
		while(temp1!=NULL){
			temp2=temp1->next;
			free(temp1);
			temp1=temp2;
		}
	}
	free(list);
}
