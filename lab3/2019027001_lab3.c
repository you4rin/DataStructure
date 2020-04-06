#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAXLEN 100

typedef int DATATYPE;
typedef struct _node{
	DATATYPE data;
	struct _node* next;
}Node;

typedef struct{
	Node* Head;
}Stack;

void InitStack(Stack* s){ s->Head=NULL; }
int IsEmpty(Stack* s){ return s->Head?0:1; }

void Push(Stack* s,DATATYPE data){
	Node* newNode=(Node*)malloc(sizeof(Node));
	newNode->data=data;
	newNode->next=s->Head;
	s->Head=newNode;
}

DATATYPE Pop(Stack* s){
	DATATYPE tmpData;
	Node* tmp;
	if(IsEmpty(s)){
		printf("Stack is empty\n");
		exit(1);
	}
	tmpData=s->Head->data;
	tmp=s->Head;
	s->Head=s->Head->next;
	free(tmp);
	return tmpData;
}

DATATYPE Top(Stack* s){
	if(IsEmpty(s)){
		printf("Stack is empty\n");
		exit(1);
	}
	return s->Head->data;
}

int GetOperatorPriotity(char op){
	switch(op){
	case'*':case'/':return 5;
	case'+':case'-':return 3;
	case'(':return 1;
	default:return -1;
	}
}

int CompareOperatorPriority(char op1,char op2){
	if(GetOperatorPriotity(op1)>GetOperatorPriotity(op2))return 1;
	if(GetOperatorPriotity(op1)<GetOperatorPriotity(op2))return -1;
	return 0;
}

char* ConvertExp(char exp[]){
	Stack s;
	char* newExp;
	int len=strlen(exp),pos=0;
	if(len>MAXLEN){
		printf("Equation is too long\n");
		exit(1);
	}
	InitStack(&s);
	newExp=(char*)malloc(sizeof(char)*(len+1));
	for(int i=0;i<len;++i){
		if(exp[i]=='(')Push(&s,exp[i]);
		else if(exp[i]>47&&exp[i]<58)newExp[pos++]=exp[i];
		else if(exp[i]=='*'||exp[i]=='/'||exp[i]=='+'||exp[i]=='-'){
			while(!IsEmpty(&s)&&(CompareOperatorPriority(exp[i],Top(&s))<=0)){
				if(GetOperatorPriotity(Top(&s))!=1)newExp[pos++]=Pop(&s);
				else break;
			}
			Push(&s,exp[i]);
		}
		else if(exp[i]==')'){
			while(GetOperatorPriotity(Top(&s))!=1)newExp[pos++]=Pop(&s);
			Pop(&s);
		}
	}
	while(!IsEmpty(&s)){
		if(GetOperatorPriotity(Top(&s))!=1)newExp[pos++]=Pop(&s);
		else Pop(&s);
	}
	newExp[pos]='\0';
	return newExp;
}

int EvalExp(char exp[]){
	Stack s;
	int len=strlen(exp),lhs,rhs;
	InitStack(&s);
	for(int i=0;i<len;++i){
		if(exp[i]>47&&exp[i]<58)Push(&s,exp[i]-48);
		else{
			rhs=Pop(&s),lhs=Pop(&s);
			if(exp[i]=='*')Push(&s,lhs*rhs);
			else if(exp[i]=='/'){
				if(!rhs){
					printf("Can't divide by zero\n");
					exit(1);
				}
				Push(&s,lhs/rhs);
			}
			else if(exp[i]=='+')Push(&s,lhs+rhs);
			else if(exp[i]=='-')Push(&s,lhs-rhs);
		}
	}
	return Pop(&s);
}

int main(){
	char a[MAXLEN+1];
	char* b;
	strcpy(a,b=ConvertExp("1+2*3"));
	printf("%s = %d\n",a,EvalExp(a));
	free(b);
	strcpy(a,b=ConvertExp("(1+2)*3"));
	printf("%s = %d\n",a,EvalExp(a));
	free(b);
	strcpy(a,b=ConvertExp("((1-2)+3)*(5-2)"));
	printf("%s = %d\n",a,EvalExp(a));
	free(b);
}