#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	int ID;
	char* name;
}Student;

typedef Student DATATYPE;
typedef int IDTYPE;

typedef struct node{
	DATATYPE data;
	struct node* next;
}Node;

typedef struct{
	Node* Head;
	Node* Cur;
	Node* Tail;
	int NumofData;
	int(*Comp)(DATATYPE data1,DATATYPE data2);
}LinkedList;

void InitList(LinkedList* list){
	list->Head=(Node*)malloc(sizeof(Node));
	list->Head->next=NULL;
	list->Comp=NULL;
	list->NumofData=0;
}

void HeadInsert(LinkedList* list,DATATYPE data){
	Node* tmp=(Node*)malloc(sizeof(Node));
	tmp->data.ID=data.ID;
	int len=strlen(data.name);
	tmp->data.name=(char*)malloc(sizeof(char)*(len+1));
	strcpy(tmp->data.name,data.name);

	tmp->next=list->Head->next;
	list->Head->next=tmp;

	++list->NumofData;
}

void SortInsert(LinkedList* list,DATATYPE data){
	Node* tmp=(Node*)malloc(sizeof(Node));
	Node* pos=list->Head;
	tmp->data.ID=data.ID;
	int len=strlen(data.name);
	tmp->data.name=(char*)malloc(sizeof(char)*(len+1));
	strcpy(tmp->data.name,data.name);

	while(pos->next!=NULL&&list->Comp(data,pos->next->data))pos=pos->next;

	tmp->next=pos->next;
	pos->next=tmp;

	++list->NumofData;
}

void Insert(LinkedList* list,DATATYPE data){
	if(list->Comp==NULL)HeadInsert(list,data);
	else SortInsert(list,data);
}

int PosHead(LinkedList* list,IDTYPE* ID){
	if(list->Head->next==NULL)return 0;

	list->Tail=list->Head;
	list->Cur=list->Head->next;

	*ID=list->Cur->data.ID;
	return 1;
}

int PosNext(LinkedList* list,IDTYPE* ID){
	if(list->Cur->next==NULL)return 0;

	list->Tail=list->Cur;
	list->Cur=list->Cur->next;

	*ID=list->Cur->data.ID;
	return 1;
}

void Remove(LinkedList* list){
	Node* tmp=list->Cur;

	list->Tail->next=list->Cur->next;
	list->Cur=list->Tail->next;

	free(tmp->data.name);
	free(tmp);
	--list->NumofData;
}

void Print(LinkedList* list){
	Node* pos=list->Head->next;
	printf("Num of datas %d\n",RetCount(list));
	while(pos!=NULL){
		printf("ID : %d, Name: %s\n",pos->data.ID,pos->data.name);
		pos=pos->next;
	}
}

int RetCount(LinkedList* list){return list->NumofData;}
int Compare(DATATYPE data1,DATATYPE data2){return data1.ID<data2.ID?0:1;}
void SetSortRule(LinkedList *list,int(*Comp)(DATATYPE data1,DATATYPE data2)){list->Comp=Comp;}

int main(){
	LinkedList list;
	Student data;
	int ID;

	InitList(&list);
	SetSortRule(&list,Compare);

	data.ID=2020200001,data.name="test person1";
	Insert(&list,data);
	data.ID=2020200765,data.name="test person2";
	Insert(&list,data);
	data.ID=2020300004,data.name="test person3";
	Insert(&list,data);
	data.ID=2020200628,data.name="test person4";
	Insert(&list,data);
	data.ID=2020200218,data.name="test person5";
	Insert(&list,data);

	Print(&list);
	printf("\n");

	int target=2020200628;
	printf("Delete student node ID = %d\n\n",target);

	if(PosHead(&list,&ID)){
		if(ID==target)Remove(&list);
		while(PosNext(&list,&ID))if(ID==target)Remove(&list);
	}

	Print(&list);
	return 0;
}