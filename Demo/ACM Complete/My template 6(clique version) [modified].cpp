//Bismillahir Rahmanir Rahim
#pragma warning(disable:4786)
// #pragma comment(linker,"/STACK:514850416")
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <numeric>
#include <utility>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <functional>

#include <fstream>
#include <sstream>
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cmath>
#include <cassert>
#include <ctime>
#include "mcqd.h"
using namespace std;

#define MEMSET(dest,val) memset(dest,val,sizeof(dest))

const double pi = acos(-1.0);
const double eps = 1e-11;
const int inf = (1<<30);

int noA,noT,noG,noC;

struct Couple{
    string str;
    int seq;
};
struct Pattern{
    string str;
    int seq;
    bool found;
    vector<Couple> previousString;
    vector<Couple> currentString;
}pt;

struct Bucket{
    //Predecessor pr;
    vector<int> vi;
    vector<Pattern> vc;

}*target,*target2;

map<string,Bucket> mp;
map<string,Bucket>::iterator mit,mit2,currentmit;
vector<Pattern>::iterator vit,vit2,vit3;

void countATGC(char x);
void clearATGC();
string makeStringATGC();
bool checkNeighbour();
void computeMatching(string str);
void printList(int index);
int findMax();
int hammingDistance(string str1,string str2);
void reduction(int index,int amount);
void distribution(int index,int amount);
bool findClique(vector<Pattern>::iterator vpit);
void setFound(string str);


int N,T,L,D,currentArr[4],originalArr[4],distributionValue;
int light=0;
int main()
{
    //L=atoi(argv[1]);
    //D=atoi(argv[2]);
    printf("Provide l and d value:\n");
    scanf("%d %d",&L, &D);
    //L=15;
    //D=4;
    stringstream ss,ss2;
    ss<< L;
    ss2<< D;
    string filename= "input_l"+ ss.str() + "_d"+ss2.str()+"_0.txt";
    time_t tstart,tend;
	freopen(filename.c_str(),"r",stdin);
	freopen("out.txt","w",stdout);

    int i,totalChar,k,j,seq;
    char ch,cArr[1000],cStr[40],x,seqStr[40];
    string pattern,strATGC;

    seq=0;
    //scanf("%d %d %d %d",&N,&T,&L,&D);

    tstart=time(0);

    while(gets(seqStr)){
        i=0;
        while(scanf("%c",&ch)!=EOF && ch!='>'){
            if(isalpha(ch)){
                cArr[i++]=toupper(ch);
            }
        }
        totalChar=i;

        for(i=0;i<(totalChar-L+1);i++){
            noA=noT=noG=noC=0;
            k=0;

            for(j=i;j<i+L;j++){
                //countATGC(cArr[j]);
                x=cArr[j];
                if(x=='A')noA++;
                else if(x=='T')noT++;
                else if(x=='G')noG++;
                else noC++;

                cStr[k++]=cArr[j];
            }
            cStr[k]='\0';
            strATGC=makeStringATGC();
            pattern=cStr;
            vector<int> vi;
            vi.push_back(noA);vi.push_back(noT);vi.push_back(noG);vi.push_back(noC);

            pt.str=pattern;
            pt.seq=seq;
            pt.found=0;
            mp[strATGC].vc.push_back(pt);
            mp[strATGC].vi=vi;

            /*if(i && !((i+L)%N)){
                i+=L-1;
                seq++;
            }*/

        }
        seq++;
    }
    T=seq;

    //cout<<"map size="<<mp.size()<<endl;
    //int limit=mp.size()/2;

    int cnt2=0;
	for(currentmit=mp.begin();currentmit!=mp.end();currentmit++){
        //printf("on map no-%d\n",cnt2);cout<<currentmit->first<<endl;
        //printf("this map has vector of size=%d\n",currentmit->second.vc.size());

        if(checkNeighbour())
            break;
        cnt2++;
        //if(cnt2==limit)
            //break;
 	}

    tend=time(0);
    printf("total time required : %lf",difftime(tend,tstart));

	return 0;
}
void countATGC(char x){

}
string makeStringATGC(){
    stringstream ss;
    ss<<noA<<noT<<noG<<noC;
    string str=ss.str();
    return str;
}
bool checkNeighbour(){
    string str=currentmit->first;
    vector <int> vi= currentmit->second.vi;
    //if(str=="2132")
        //puts("found");

    int i,index,prev,curr,j;
    for(i=0;i<4;i++){
        originalArr[i]=currentArr[i]=vi[i];
    }

    int n=D*2;
    //double tstart,tstop,tstop2;
    //tstart = (double)clock()/CLOCKS_PER_SEC;
    for(int i=n;i>0;i--){
        distributionValue=i;
        reduction(3,i);
    }
    //tstop = (double)clock()/CLOCKS_PER_SEC;
    //printf("travarsing took time=%lf\n",tstop-tstart);

    for(vit3=currentmit->second.vc.begin();vit3!=currentmit->second.vc.end();vit3++){
        if(!(vit3->found)){
            //if(vit3->str=="CGGGCCCCCCCGGCCCGCTGG")
               // printf("desired string found:");
            if(findClique(vit3))
                return 1;
        }
    }

    currentmit->second.vc.clear();

    //tstop2 = (double)clock()/CLOCKS_PER_SEC;
    //printf("clique finding took time=%lf\n",tstop2-tstop);

    //findClique();

    return 0;

}
void reduction(int index,int amount){

    int reduce,remaining;

    for(reduce=amount;reduce>=0;reduce--){

        if((currentArr[index]-reduce)<0)
            continue;
        currentArr[index]-=reduce;
        remaining=amount-reduce;

        if(!remaining)
            distribution(index,distributionValue);
        else{
			if((index-1)>0)
				reduction(index-1,remaining);
		}
        currentArr[index]+=reduce;
    }
}
void leftDistribution(int index,int amount){
    int eat,remaining,flag=0;
    for(eat=amount;eat>=0;eat--){
        currentArr[index]+=eat;
        remaining=amount-eat;
        if(!remaining){
            stringstream ss;
            ss<<currentArr[0]<<currentArr[1]<<currentArr[2]<<currentArr[3];
            string newString=ss.str();
            computeMatching(newString);
        }
        else{
            if((index-1)>=0)
                leftDistribution(index-1,remaining);
			else
				flag=1;

        }
        currentArr[index]-=eat;

		if(flag==1)
			return;
    }
}
bool rightDistribution(int index,int amount,int mainIndex,int leftAmount){
  int eat,remaining;

        if(currentArr[index]<originalArr[index]){
            if(index+1<4)
                return rightDistribution(index+1,amount,mainIndex,leftAmount);

            return 0;
        }

        for(eat=amount;eat>=0;eat--){
            currentArr[index]+=eat;
            remaining=amount-eat;
            if(!remaining){
                 leftDistribution(mainIndex-1,leftAmount);
            }
            else{
                if(index+1<4){
                    rightDistribution(index+1,remaining,mainIndex,leftAmount) ;                       //leftDistribution(mainIndex-1,amount);
                }
            }
            currentArr[index]-=eat;

        }
        return 1;


}
void distribution(int index,int amount){
    int eat;

    if(index+1<4){
        for(eat=amount-1;eat>=0;eat--){
            if(!rightDistribution(index+1,eat,index,amount-eat)){
                 leftDistribution(index-1,amount);
                 break;
            }
        }
    }
    else
        leftDistribution(index-1,amount);
}
void computeMatching(string str){
    Bucket *target;
    Couple cp;

    if(mp.find(str)!=mp.end()){
        target=&mp[str];
        for(vit=currentmit->second.vc.begin();vit!=currentmit->second.vc.end();vit++){

            if(!(vit->found)){
                for(vit2=target->vc.begin();vit2!=target->vc.end();vit2++){
                    if(!(vit2->found) && (vit->seq!=vit2->seq) && hammingDistance(vit->str,vit2->str)<(D*2+1)){

                        cp.str=vit2->str;cp.seq=vit2->seq;
                        vit->currentString.push_back(cp);

                        cp.str=vit->str;cp.seq=vit->seq;
                        vit2->previousString.push_back(cp);
                    }
                }
            }
        }
    }
}

int hammingDistance(string str1,string str2){

    int count=0;
    for(int i=0;i<L;i++){
        if(str1[i]!=str2[i])
            count++;
    }
    return count;
}
bool findClique(vector<Pattern>::iterator vpit){
    //time_t tstart,tend;
    //tstart=time(0);
    //double tstart,tstop,ttime;
    //tstart = (double)clock()/CLOCKS_PER_SEC;
     int i,j;
    vector<Couple> nodeList= vpit->previousString;
    nodeList.insert(nodeList.begin(),vpit->currentString.begin(),vpit->currentString.end());

      int size=nodeList.size();

      //printf("nodelist size %d\n",size);
      if(size<=(T-2))
        return 0;
      bool **conn = new bool*[size];
      int *qmax,qsize;

      for (int i=0; i < size; i++) {
        conn[i] = new bool[size];
        memset(conn[i], 0, size * sizeof(bool));
      }

    int cnt=0;
      for(int i=0;i<size-1;i++){
        for(int j=i+1;j<size;j++){
            if(nodeList[i].seq!=nodeList[j].seq && (hammingDistance(nodeList[i].str,nodeList[j].str)<(D*2+1))){
                conn[i][j] = true;
                conn[j][i] = true;
                cnt++;
            }
        }
      }
        //printf("number of edges %d",cnt);

    Maxclique md(conn, size, 0.025);
    md.mcqdyn(qmax, qsize);

    //printf("clique finished");
    //if(vpit->str=="CGGGCCCCCCCGGCCCGCTGG")
        //printf("qsize for target= %d\n",qsize);
    vector<string> vcstr;
    int profile[4][30];
    char motif[30];
    string motifstr;


    if(qsize>(T-2)){

        //printf("entered for");
        //cout<<vpit->str<<endl;

        for(i=0;i<4;i++){
            for(j=0;j<L;j++)
                profile[i][j]=0;
        }
       // cout<<vpit->str<<endl;
        vcstr.push_back(vpit->str);
        for(i=0;i<qsize;i++){
            //cout<<nodeList[qmax[i]].str<<endl;
            //setFound(nodeList[qmax[i]].str);
            vcstr.push_back(nodeList[qmax[i]].str);
        }

        for(i=0;i<vcstr.size();i++){
            for(j=0;j<L;j++){
                if(vcstr[i][j]=='A')profile[0][j]++;
                else if(vcstr[i][j]=='T')profile[1][j]++;
                else if(vcstr[i][j]=='G')profile[2][j]++;
                else if(vcstr[i][j]=='C')profile[3][j]++;
            }
        }

        int node;
        for(i=0;i<L;i++){
            int maxi=-1;
            for(j=0;j<4;j++){
                if(profile[j][i]>maxi){
                    node=j;
                    maxi=profile[j][i];
                }
            }
            if(node==0)motif[i]='A';
            else if(node==1)motif[i]='T';
            else if(node==2)motif[i]='G';
            else motif[i]='C';
        }

        motif[L]='\0';
        motifstr= string(motif);
        //cout<<"temporary motif="<<motifstr<<endl;

        int flag=0;
        int failed=0;
        for(i=0;i<vcstr.size();i++){
                //cout<<"candidate key "<<vcstr[i]<<endl;
            int h=hammingDistance(vcstr[i],motifstr);
            if(h>D){
                flag=1;
                failed++;
                //printf("failed\n");
                //break;
            }
        }
        if((vcstr.size()-failed)>=T){
            cout<<"motif="<<motifstr<<endl;
            for(i=0;i<vcstr.size();i++)
                setFound(vcstr[i]);

            return 1;
        }

        //puts("============");

    }
    return 0;
    //tend=time(0);
    //printf("clique took time=%lf",difftime(tend,tstart));
    //tstop = (double)clock()/CLOCKS_PER_SEC;
    //ttime=tstop-tstart;
    //printf("clique took time=%lf\n",ttime);
}
void setFound(string str){
    noA=noT=noG=noC=0;
    char x;
    for(int i=0;i<str.size();i++){
        x=str[i];
        if(x=='A')noA++;
        else if(x=='T')noT++;
        else if(x=='G')noG++;
        else noC++;
    }
    string strATGC=makeStringATGC();

    target2=&mp[strATGC];

    vector<Pattern>::iterator vit;
    for(vit=target2->vc.begin();vit!=target2->vc.end();vit++){
        if(str==vit->str)
            vit->found=1;
    }


}
/*
|=================================|
|COMMENT FREOPEN BEFORE SUBMITTING|
|=================================|

if u cant find algorithm:
=========================
1.do the simulation of testcase.
2.try alternate solutions.

if u cant match test case:
==========================
1.uncomment freopen and debug on same testcase.

if u get wa:
===========
1.check if input output format is correct.
2.read the problem again see is there any gotchas.
3.read your code line by line ,check for any problem
4.uncomment freopen and check for several testcases.
5. you may start with boundery value (1 or neg and N)
*/
