#include<stdio.h>
#include<stdlib.h>
#define MAX 10
void bubble_sort(int* arr,int size){
    for(int i=0;i<size-1;++i){
        for(int j=1;j<size-i;++j){
            if(arr[j-1]>arr[j]){
                int temp=arr[j-1];
                arr[j-1]=arr[j];
                arr[j]=temp;
            }
        }
    }
}
int main(int argc,char** argv){
    if(argc<2)return 1;
    int* arr=(int*)malloc(sizeof(int)*MAX);
    int size=0;
    FILE* fp=fopen(argv[1],"r");
    while(fscanf(fp,"%d",&arr[size])!=EOF)++size;
    fclose(fp);
    bubble_sort(arr,size);
    fp=fopen("OutputData.txt","w");
    for(int i=0;i<size;++i)fprintf(fp,"%d ",arr[i]);
    fclose(fp);
    free(arr);
}
