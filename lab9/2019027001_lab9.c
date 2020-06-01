#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define NODE_DEGREE 2
#define MAX_CHILDS (NODE_DEGREE*2)
#define MAX_KEYS (MAX_CHILDS-1)

#define SPACE (4*MAX_KEYS)

typedef struct BTreeNode{
	int keys[MAX_KEYS];
	struct BTreeNode* childs[MAX_CHILDS];
	int keyIndex;
	int leaf;
} BTreeNode;

typedef struct{
	struct BTreeNode* root;
	int degree;
} BTree;

BTreeNode* CreateBTreeNode(){
	BTreeNode* temp=(BTreeNode*)malloc(sizeof(BTreeNode));

	for(int i=0;i<MAX_KEYS;++i)temp->keys[i]=0;
	for(int i=0;i<MAX_CHILDS;++i)temp->childs[i]=NULL;

	temp->keyIndex=0;
	temp->leaf=TRUE;

	return temp;
}
BTree *CreateBTree(){
	BTree* tree=(BTree*)malloc(sizeof(BTree));
	BTreeNode* temp=CreateBTreeNode();

	tree->degree=NODE_DEGREE;
	tree->root=temp;

	return tree;
}

void Search(BTreeNode* node,int key){
	for(int i=0;i<node->keyIndex;++i){
		if(node->keys[i]==key){
			printf("key %d exist in tree\n",key);
			break;
		}
		else if(node->keys[i]>key){
			if(node->leaf)printf("key %d not exist in tree\n",key);
			else Search(node->childs[i],key);
			break;
		}
		if(i==node->keyIndex-1){
			if(node->leaf)printf("key %d not exist in tree\n",key);
			else Search(node->childs[node->keyIndex],key);
		}
	}
}

void SplitChild(BTreeNode* parent,BTreeNode* child,int index){
	BTreeNode* temp=CreateBTreeNode();

	temp->leaf=child->leaf;
	temp->keyIndex=NODE_DEGREE-1;

	for(int i=0;i<NODE_DEGREE-1;++i)temp->keys[i]=child->keys[NODE_DEGREE+i];

	if(!child->leaf)for(int i=0;i<NODE_DEGREE;++i)temp->childs[i]=child->childs[NODE_DEGREE+i];
	child->keyIndex=NODE_DEGREE-1;

	for(int i=parent->keyIndex;i>index-1;--i)parent->childs[i+i]=parent->childs[i];
	parent->childs[index]=temp;

	for(int i=parent->keyIndex;i>index-1;--i)parent->keys[i]=parent->keys[i-1];
	parent->keys[index-1]=child->keys[NODE_DEGREE-1];

	parent->keyIndex++;
}

void InsertNonFull(BTreeNode* node,int key){
	int index=node->keyIndex;

	if(node->leaf){
		while(index>0&&node->keys[index-1]>key){
			node->keys[index]=node->keys[index-1];
			index--;
		}
		node->keys[index]=key;
		node->keyIndex++;
	}
	else{
		while(index>0&&node->keys[index-1]>key)index--;

		if(node->childs[index]->keyIndex==MAX_KEYS){
			SplitChild(node,node->childs[index],index+1);
			if(node->keys[index]<key)index++;
		}
		InsertNonFull(node->childs[index],key);
	}
}
void Insert(BTree* tree,int key){
	if(tree->root->keyIndex==MAX_KEYS){
		BTreeNode* temp=CreateBTreeNode();

		temp->leaf=FALSE;
		temp->keyIndex=0;
		temp->childs[0]=tree->root;

		SplitChild(temp,tree->root,1);

		InsertNonFull(temp,key);

		tree->root=temp;
	}
	else InsertNonFull(tree->root,key);
}

void Merge(BTreeNode* node,int index){
	BTreeNode* child=node->childs[index];
	BTreeNode* sibling=node->childs[index+1];

	child->keys[NODE_DEGREE-1]=node->keys[index];
	for(int i=0;i<sibling->keyIndex;++i)child->keys[i+NODE_DEGREE]=sibling->keys[i];
	if(!child->leaf)for(int i=0;i<sibling->keyIndex+1;++i)child->childs[i+NODE_DEGREE]=sibling->childs[i];

	for(int i=index+1;i<node->keyIndex;++i)node->keys[i-1]=node->keys[i];
	for(int i=index+2;i<node->keyIndex+1;++i)node->childs[i-1]=node->childs[i];

	child->keyIndex+=sibling->keyIndex+1;
	node->keyIndex--;
}

void BorrowFromPrev(BTreeNode* node,int index){
	BTreeNode* child=node->childs[index];
	BTreeNode* sibling=node->childs[index-1];

	for(int i=child->keyIndex-1;i>-1;--i)child->keys[i+1]=child->keys[i];

	if(!child->leaf)for(int i=child->keyIndex;i>-1;--i)child->childs[i+1]=child->childs[i];
	child->keys[0]=node->keys[index-1];

	if(!child->leaf)child->childs[0]=sibling->childs[sibling->keyIndex];
	node->keys[index-1]=sibling->keys[sibling->keyIndex-1];

	child->keyIndex++;
	sibling->keyIndex--;
}

void BorrowFromNext(BTreeNode* node,int index){
	BTreeNode* child=node->childs[index];
	BTreeNode* sibling=node->childs[index+1];

	child->keys[child->keyIndex]=node->keys[index];

	if(!child->leaf)child->childs[child->keyIndex+1]=sibling->childs[0];
	node->keys[index]=sibling->keys[0];

	for(int i=1;i<sibling->keyIndex;++i)sibling->keys[i-1]=sibling->keys[i];

	if(!sibling->leaf)for(int i=1;i<sibling->keyIndex;++i)sibling->childs[i-1]=sibling->childs[i];

	child->keyIndex++;
	sibling->keyIndex--;
}

void Fill(BTreeNode* node,int index){
	if(index&&node->childs[index-1]->keyIndex>NODE_DEGREE-1)BorrowFromPrev(node,index);
	else if(index!=node->keyIndex&&node->childs[index+1]->keyIndex>NODE_DEGREE-1)BorrowFromNext(node,index);
	else index!=node->keyIndex?Merge(node,index):Merge(node,index-1);
}


void RemoveFromLeaf(BTreeNode* node,int index){
	for(int i=index+1;i<node->keyIndex;++i)node->keys[i-1]=node->keys[i];
	node->keyIndex--;
}
void RemoveFromNonLeaf(BTreeNode* node,int index){
	int key=node->keys[index];
	void RemoveNode(BTreeNode* node,int key);

	if(node->childs[index]->keyIndex>NODE_DEGREE-1){
		int pred=RetPredecessor(node,index);
		node->keys[index]=pred;
		RemoveNode(node->childs[index],pred);
	}
	else if(node->childs[index+1]->keyIndex>NODE_DEGREE-1){
		int succ=RetSuccessor(node,index);
		node->keys[index]=succ;
		RemoveNode(node->childs[index+1],succ);
	}
	else{
		Merge(node,index);
		RemoveNode(node->childs[index],key);
	}
}

void RemoveNode(BTreeNode* node,int key){
	int index=FindKey(node,key);

	if(index<node->keyIndex&&node->keys[index]==key)node->leaf?RemoveFromLeaf(node,index):RemoveFromNonLeaf(node,index);
	else{
		if(node->leaf){
			printf("key %d is not exist in the tree\n",key);
			return;
		}

		int flag=node->keyIndex==index?TRUE:FALSE;

		if(node->childs[index]->keyIndex<NODE_DEGREE)Fill(node,index);

		(flag&&(index>node->keyIndex))?RemoveNode(node->childs[index-1],key):RemoveNode(node->childs[index],key);
	}
}

void Remove(BTree* tree,int key){
	RemoveNode(tree->root,key);
	if(!tree->root->keyIndex)tree->root=(tree->root->leaf)?CreateBTreeNode():tree->root->childs[0];
}

int FindKey(BTreeNode* node,int key){
	int index=0;
	while(index<node->keyIndex&&node->keys[index]<key)index++;
	return index;
}

int RetPredecessor(BTreeNode* node,int index){
	BTreeNode* curr=node->childs[index];
	while(!curr->leaf)curr=curr->childs[curr->keyIndex];
	return curr->keys[curr->keyIndex-1];
}
int RetSuccessor(BTreeNode* node,int index){
	BTreeNode* curr=node->childs[index+1];
	while(!curr->leaf)curr=curr->childs[0];
	return curr->keys[0];
}

void PrintTree(BTreeNode* node,int level,int blank){
	if(!node->keyIndex){
		printf("tree not exist\n");
		return;
	}

	if(blank){
		for(int i=0;i<level;++i){
			for(int j=0;j<SPACE;++j)printf(" ");
			printf("|");
		}
	}

	if(node->keyIndex>0){
		for(int i=0;i<(SPACE-4*node->keyIndex);++i)printf(" ");
		printf("[");
		for(int i=0;i<node->keyIndex-1;++i)printf("%2d, ",node->keys[i]);
		printf("%2d]|",node->keys[node->keyIndex-1]);
	}
	else printf("[ ]");

	if(node->leaf){
		printf("\n");
		return;
	}
	else for(int i=0;i<node->keyIndex+1;++i)i?PrintTree(node->childs[i],level+1,TRUE):PrintTree(node->childs[i],level+1,FALSE);
}

int main(int argc,char *argv[]){
	int test[]={1,7,2,11,4,8,13,10,5,19,3,6,9,18,23,12,14,20,21,16,26,27,22,24,25};

	BTree* tree=CreateBTree();

	for(int i=0;i<sizeof(test)/sizeof(int);++i){
		printf("Insert : %d\n",test[i]);
		Insert(tree,test[i]);
		PrintTree(tree->root,0,FALSE);
		printf("\n");
	}

	for(int i= 0;i<3*(SPACE+1);++i)printf("-");
	printf("\n");

	int testdel[]={14,3,12,27,1,10,11,25,5,9,23,20,8,4,24,1};

	for(int i=0;i<sizeof(testdel)/sizeof(int);++i){
		printf("Remove : %d\n",testdel[i]);
		Remove(tree,testdel[i]);
		PrintTree(tree->root,0,FALSE);
		printf("\n");
	}

	for(int i=0;i<3*(SPACE+1);++i)printf("-");
	printf("\n");

	int target=2;
	printf("Search %d\n",target);
	Search(tree->root,target);
	printf("\n");

	target=26;
	printf("Search %d\n",target);
	Search(tree->root,target);
	printf("\n");

	target=18;
	printf("Search %d\n",target);
	Search(tree->root,target);
	printf("\n");

	target=14;
	printf("Search %d\n",target);
	Search(tree->root,target);
	printf("\n");

	return 0;
}