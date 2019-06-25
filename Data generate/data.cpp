#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
    char motif[30][30],arr[4],randVal[30],motifOriginal[30];
    int L,D,randPosition[30],i,j,seqPosition[30];
    freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);

    scanf("%s %d %d",motifOriginal,&L,&D);

    arr[0]='A';arr[1]='T';arr[2]='G';arr[3]='C';
    for(i=1;i<=20;i++){
        strcpy(motif[i],motifOriginal);
        for(j=0;j<D;j++){
            randPosition[j]=rand()%L;
            randVal[j]=arr[rand()%4];
            motif[i][randPosition[j]]=randVal[j];
        }
        seqPosition[i]=rand()%(600-L);
        //printf("%s %d\n",motif[i],seqPosition[i]);
    }
    for(i=1;i<=20;i++){
        printf(">sequence %d\n",i);

        for(j=0;j<600;j++){
            if(j==seqPosition[i])
            {
                printf("%s",motif[i]);
                j=j+L-1;
            }
            else{
                printf("%c",arr[rand()%4]);
            }
        }
        puts("");
    }
}
