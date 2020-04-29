#include<stdio.h>
#include<stdlib.h>
#define COUNT 12

typedef int DATATYPE;
typedef struct _node{
	DATATYPE data;
	struct _node* left;
	struct _node* right;
}Node;

Node* MakeNode(void){
	Node* node=(Node*)malloc(sizeof(Node));
	node->left=NULL;
	node->right=NULL;
	return node;
}

DATATYPE RetData(Node* node){return node->data;}
void SaveData(Node* node,DATATYPE data){node->data=data;}
Node* RetLeft(Node* node){return node->left;}
Node* RetRight(Node* node){return node->right;}

void MakeLeft(Node* parent,Node* child){
	if(parent->left)free(parent->left);
	parent->left=child;
}

void MakeRight(Node* parent,Node* child){
	if(parent->right)free(parent->right);
	parent->right=child;
}

Node* RemoveLeft(Node* node){
	Node* temp=NULL;
	if(node){
		temp=node->left;
		node->left=NULL;
	}
	return temp;
}

Node* RemoveRight(Node* node){
	Node* temp=NULL;
	if(node){
		temp=node->right;
		node->right=NULL;
	}
	return temp;
}

void ChangeLeft(Node* parent,Node* child){parent->left=child;}
void ChangeRight(Node* parent,Node* child){parent->right=child;}

void PreOrder(Node* node){
	if(node==NULL)return;
	printf("%d ",node->data);
	PreOrder(node->left);
	PreOrder(node->right);
}

void InOrder(Node* node){
	if(node==NULL)return;
	InOrder(node->left);
	printf("%d ",node->data);
	InOrder(node->right);
}

void PostOrder(Node* node){
	if(node==NULL)return;
	PostOrder(node->left);
	PostOrder(node->right);
	printf("%d ",node->data);
}

void MakeBST(Node** node){*node=NULL;}

int RetHeight(Node* node){
	int left,right;
	if(node==NULL)return 0;
	left=RetHeight(RetLeft(node)),right=RetHeight(RetRight(node));
	return left>right?left+1:right+1;
}

int RetDiff(Node* node){
	int left,right;
	if(node==NULL)return 0;
	left=RetHeight(RetLeft(node)),right=RetHeight(RetRight(node));
	return left-right;
}

Node* RotateLL(Node* node){
	Node *parent=node,*child=RetLeft(parent);
	ChangeLeft(parent,RetRight(child));
	ChangeRight(child,parent);
	return child;
}

Node* RotateRR(Node* node){
	Node *parent=node,*child=RetRight(parent);
	ChangeRight(parent,RetLeft(child));
	ChangeLeft(child,parent);
	return child;
}

Node* RotateRL(Node* node){
	Node *parent=node,*child=RetRight(parent);
	ChangeRight(parent,RotateLL(child));
	return RotateRR(parent);
}

Node* RotateLR(Node* node){
	Node *parent=node,*child=RetLeft(parent);
	ChangeLeft(parent,RotateRR(child));
	return RotateLL(parent);
}

Node* Rebalance(Node** root){
	int diff=RetDiff(*root);
	if(diff>1){
		if(RetDiff(RetLeft(*root))>0){
			printf("Rotate LL\n");
			*root=RotateLL(*root);
		}
		else{
			printf("Rotate LR\n");
			*root=RotateLR(*root);
		}
	}
	if(diff<-1){
		if(RetDiff(RetRight(*root))<0){
			printf("Rotate RR\n");
			*root=RotateRR(*root);
		}
		else{
			printf("Rotate RL\n");
			*root=RotateRL(*root);
		}
	}
	return *root;
}

Node* InsertBST(Node** root,DATATYPE data){
	if(*root==NULL){
		*root=MakeNode();
		SaveData(*root,data);
	}
	else if(data!=RetData(*root)){
		data<RetData(*root)?InsertBST(&((*root)->left),data):InsertBST(&((*root)->right),data);
		*root=Rebalance(root);
	}
	else return NULL;
	return *root;
}

Node* SearchBST(Node* node,DATATYPE target){
	Node* cur=node;
	DATATYPE data;
	while(cur){
		data=RetData(cur);
		if(target==data)return cur;
		else if(target<data)cur=RetLeft(cur);
		else cur=RetRight(cur);
	}
	return NULL;
}

Node* RemoveBST(Node** root,DATATYPE target){
	Node *newNode=MakeNode(),*parent=newNode,*cur=*root,*targetNode;
	ChangeRight(newNode,*root);
	while(cur&&RetData(cur)!=target){
		parent=cur;
		if(target<RetData(cur))cur=RetLeft(cur);
		else cur=RetRight(cur);
	}
	if(cur==NULL)return NULL;
	targetNode=cur;
	if(RetLeft(targetNode)==NULL&&RetRight(targetNode)==NULL){
		if(RetLeft(parent)==targetNode)RemoveLeft(parent);
		else RemoveRight(parent);
	}
	else if(RetLeft(targetNode)==NULL||RetRight(targetNode)==NULL){
		Node* child;
		if(RetLeft(targetNode)!=NULL)child=RetLeft(targetNode);
		else child=RetRight(targetNode);
		if(RetLeft(parent)==targetNode)ChangeLeft(parent,child);
		else ChangeRight(parent,child);
	}
	else{
		Node* minNode=RetRight(targetNode);
		Node* minParent=targetNode;
		DATATYPE backup;
		while(RetLeft(minNode)){
			minParent=minNode;
			minNode=RetLeft(minNode);
		}
		backup=RetData(targetNode);
		SaveData(targetNode,RetData(minNode));
		if(RetLeft(minParent)==minNode)ChangeLeft(minParent,RetRight(minNode));
		else ChangeRight(minParent,RetRight(minNode));
		targetNode=minNode;
		SaveData(targetNode,backup);
		if(RetRight(newNode)!=*root)*root=RetRight(newNode);
		free(newNode);
		return targetNode;
	}
}

void Print2D(Node* root,int space){
	if(root==NULL)return;
	space+=COUNT;
	Print2D(root->right,space);
	printf("\n");
	for(int i=COUNT;i<space;++i)printf(" ");
	printf("%d\n",root->data);
	Print2D(root->left,space);
}

void PrintAll(Node* node){
	//PreOrder(node);
	//InOrder(node);
	//PostOrder(node);
	Print2D(node,0);
}

int main(){
	Node* root;
	MakeBST(&root);
	InsertBST(&root,7);InsertBST(&root,6);InsertBST(&root,3);InsertBST(&root,4);
	InsertBST(&root,5);InsertBST(&root,9);InsertBST(&root,8);InsertBST(&root,1);
	InsertBST(&root,2);
	PrintAll(root);
}