#include<stdio.h>
#define HEAP_LEN 100
typedef char DATATYPE;
typedef struct Heap{
	int num;
	DATATYPE arr[HEAP_LEN];
}Heap;

void HeapInit(Heap* h){h->num=0;}
int IsEmpty(Heap* h){return !h->num;}

int RetParent(int idx){return idx/2;}
int RetLeft(int idx){return idx*2;}
int RetRight(int idx){return idx*2+1;}

int RetHigh(Heap* h,int idx){
	if(RetLeft(idx)>h->num)return 0;
	if(RetLeft(idx)==h->num||h->arr[RetRight(idx)]<=h->arr[RetLeft(idx)])return RetLeft(idx);
	return RetRight(idx);
}

void HeapInsert(Heap* h,DATATYPE data){
	int idx=++h->num;
	while(idx!=1){
		if(data<=h->arr[RetParent(idx)])break;
		h->arr[idx]=h->arr[RetParent(idx)];
		idx=RetParent(idx);
	}
	h->arr[idx]=data;
}

DATATYPE HeapDelete(Heap* h){
	DATATYPE data=h->arr[1],last=h->arr[h->num];
	int pidx=1,cidx;
	while(cidx=RetHigh(h,pidx)){
		if(last>=h->arr[cidx])break;
		h->arr[pidx]=h->arr[cidx];
		pidx=cidx;
	}
	h->arr[pidx]=last;
	--h->num;
	return data;
}

void PrintAll(Heap* h){for(int i=1;i<=h->num;++i)printf("%d : %c\n",i,h->arr[i]);}

int main(){
	Heap h;
	HeapInit(&h);
	HeapInsert(&h,'T');HeapInsert(&h,'H');HeapInsert(&h,'I');HeapInsert(&h,'S');HeapInsert(&h,'I');
	HeapInsert(&h,'S');HeapInsert(&h,'H');HeapInsert(&h,'E');HeapInsert(&h,'A');HeapInsert(&h,'P');

	PrintAll(&h);
	printf("\n%c\n\n",HeapDelete(&h));
	PrintAll(&h);
	printf("\n");
	while(!IsEmpty(&h))printf("%c\n",HeapDelete(&h));
}