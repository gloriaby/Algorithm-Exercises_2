//suppost the number of vertex (not the number of lines in file) is already known (N=875714)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 875714
//define N 9
struct LinkedNode{
	int val;
	struct LinkedNode* next;
};

struct Stack{
	int stack[N];
	int top;
};

struct LinkedNode** getAjlist(FILE* fp, int reversed);
void printAjList(struct LinkedNode** ajlist);
void freeAjList(struct LinkedNode** ajlist);
void DFS_Loop(struct LinkedNode** ajlist, int* leader, int* finishtime);
void DFS_nonrecursion(struct LinkedNode** ajlist, int i, int* leader, int* finishtime, int* temparray);
void push(int i);
int pop();

int t;//for recording the finishing time in the 1st DFS-Loop
int t_max;//for recording the finishing time in the 1st DFS-Loop
int s;//for recording the leader in the 2nd DFS-Loop
struct Stack S={.top=-1};//stack is used in DFS non-recursive method

int main(){
	char* filename="SCC.txt";
//	char* filename="test1.txt";
	char* mode="r";
	int i;

	FILE* fp=fopen(filename, mode);
	struct LinkedNode** ajlist_r=getAjlist(fp, 1);//get the adjacency list (reversed Graph)
	fclose(fp);
    
//1st DSF-Loop on reveresed graph
	int leader[N]={0};
	int finishtime[N]={0};
	int temparray[N]={0};
	DFS_Loop(ajlist_r, leader, finishtime);//run DSF-Loop on reversed graph ajlist_r


//2nd DSF-Loop on original graph(with new node name)
	fp=fopen(filename, mode);
	struct LinkedNode** ajlist=getAjlist(fp,0);

	//modify the node name(number) based on finishing time of the reversed graph	
	i=0;
	while(i<N){
		if(*(ajlist+i)!=NULL){
			struct LinkedNode* p=*(ajlist+i);
			p->val=finishtime[p->val-1];
			p=p->next;
			while(p!=NULL){
				p->val=finishtime[p->val-1];
				p=p->next;
			}
		}

		i++;
	}


	//reset leader[] and fishihtime[] (all zeros)
	for(i=0;i<N;i++){
		leader[i]=0;
		finishtime[i]=0;
	}

	DFS_Loop(ajlist, leader, finishtime);//run DSF-Loop on graph with new node name


//find the number of nodes in each SCC (has the same leader in each SCC)
	int count[N]={0};
	for(i=0;i<N;i++){
		count[leader[i]-1]++;
	}

	//output the record into another file in order to sort the result
	FILE* r=fopen("countleader.txt", "w");
	for(i=0;i<N;i++){
		if(count[i]!=0){
			fprintf(r, "%d nodes in SCC with leader %d\n", count[i], i+1);
		}
	}
	fclose(r);	

	freeAjList(ajlist);
	freeAjList(ajlist_r);
	fclose(fp);
	return 0;

}


struct LinkedNode** getAjlist(FILE* fp, int reversed){
	char head[10], tail[10];///////////
	struct LinkedNode** ajlist=malloc(sizeof(struct LinkedNode*)*N);

	//initialize all N pointers to be NULL
	int j;
	for(j=0;j<N;j++){
		*(ajlist+j)=NULL;
	}
	
	if(fp==NULL){
		fprintf(stderr, "Can not open file.\n");
		exit(1);
	}

	if(reversed==0){//get the original graph (suppose ascending order in the 1st column of file SCC.txt)
		int lastv=0;//record last vertext that meet
		struct LinkedNode* end=NULL;//record last node of current line

 		int i=-1;
		while(fscanf(fp, "%s %s\n", tail, head)!=EOF){
			if(atoi(tail)!=lastv){//haven't built this vertex yet
				i++;
				*(ajlist+i)=malloc(sizeof(struct LinkedNode));
				(*(ajlist+i))->val=atoi(tail);
				(*(ajlist+i))->next=malloc(sizeof(struct LinkedNode));
				(*(ajlist+i))->next->val=atoi(head);
				(*(ajlist+i))->next->next=NULL;
				end=(*(ajlist+i))->next;
				lastv=atoi(tail);
			}
			else{//already met this vertex before, add it to the current linked list
				end->next=malloc(sizeof(struct LinkedNode));
				end->next->val=atoi(head);
				end->next->next=NULL;
				end=end->next;
			}
		}
	}
	else{//get the reversed graph		
		while(fscanf(fp, "%s %s\n", head, tail)!=EOF){
			if( *(ajlist+atoi(tail)-1) ==NULL){//havn't built this vertex yet
				*(ajlist+atoi(tail)-1)=malloc(sizeof(struct LinkedNode));
				(*(ajlist+atoi(tail)-1))->val=atoi(tail);
				(*(ajlist+atoi(tail)-1))->next=malloc(sizeof(struct LinkedNode));
				(*(ajlist+atoi(tail)-1))->next->val=atoi(head);
				(*(ajlist+atoi(tail)-1))->next->next=NULL;
			}
			else{//already met this vertex before, add it to the corresponding linked list
				struct LinkedNode* end=*(ajlist+atoi(tail)-1);
				while(end->next!=NULL){
					end=end->next;
				}
				end->next=malloc(sizeof(struct LinkedNode));
				end->next->val=atoi(head);
				end->next->next=NULL;
			}
		}
	}
	return ajlist;
}

void printAjList(struct LinkedNode** ajlist){
	//print the adjacent list for testing
	int i=0;
	while(i<N){
		struct LinkedNode* p=*(ajlist+i);
		if(p!=NULL){
			printf("%d:", p->val);
			p=p->next;
			while(p!=NULL){
				printf("%d->",p->val);
				p=p->next;
			}
			printf("\n");
		}
		else{
			printf("null\n");
		}
			i++;
	}
}


void freeAjList(struct LinkedNode** ajlist){
	//free the adjancy list(2D)
	int i=0;
	while(i<N){
		struct LinkedNode* q=*(ajlist+i);
		struct LinkedNode* x;
		while(q!=NULL){
			x=q->next;
			free(q);
			q=x;
		}
		i++;	
	}
	free(ajlist);
}

void DFS_Loop(struct LinkedNode** ajlist, int* leader, int* finishtime){
	t_max=0;//how many nodes have been explored during each for loop

	int i,j;
	for(i=N;i>=1;i--){
		if(leader[i-1]==0){//not yet explored
			s=i;
			t=-1;
			int temparray[N]={0};//mark the node explored this time (with leader s)
			DFS_nonrecursion(ajlist,i,leader, finishtime, temparray);
			t_max+=(t+1);


			//modified finishtime[], get the real finishtime for each node
			for(j=1;j<=N;j++){
				if(temparray[j-1]!=0){
					finishtime[j-1]=t_max-finishtime[j-1];
				}
			}
		}
	}

}


void DFS_nonrecursion(struct LinkedNode** ajlist, int i, int* leader, int* finishtime, int* temparray){
	push(i);
	int v,k;
	struct LinkedNode* p;
	while(S.top>=0){
		v=pop();
		if(leader[v-1]==0){//not explored
			t++;
			finishtime[v-1]=t;
			temparray[v-1]=1;//marked explored at this time
			leader[v-1]=s;

			k=0;
			while(k<N){
				p=*(ajlist+k);
				if((p!=NULL)&&(p->val==v)){
					p=p->next;
					while(p!=NULL){
						if(leader[p->val-1]==0){
							push(p->val);
						}
						p=p->next;
					}
					break;
				}
				k++;
			}
		}
	}
}
	
void push(int i){
	if(S.top==N-1){
		printf("stack is already full, can not push anything\n");
		exit(-1);
	}
	else{
		S.top++;
		S.stack[S.top]=i;
	}
}

int pop(){
	int x=-1;
	if(S.top<0){
		printf("stack is already empty, can not pop\n");
		exit(-1);
	}
	else{
		x=S.stack[S.top];
		S.stack[S.top]=0;
		S.top--;
	}
	return x;
}
