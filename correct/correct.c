#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 50

int arr[MAX][MAX]={0};

int min_(int,int,int,int);
int check(char*,char*); //words,dic

int main(int argc,char* argv[]){


FILE* words=fopen("words.txt","r");
FILE* dic=fopen("vocabulary1.txt","r");
FILE* newb=fopen("words_new.txt","w");
long pos=0;
int rank=10,temp=0,flag=1,x=0;
char ch;
char w[MAX],d[MAX];
const char div[2]=" ";
char* token=NULL;
while(fgets(w,MAX,words)!=NULL){
    token=strtok(w,div);
    while(token!=NULL){
        rank=10;
        fseek(dic,0,SEEK_SET);
        while(fgets(d,MAX,dic)!=NULL){
            if(d[0]=='\n'||d[0]=='\0'){
                continue;
            }
            else if((temp=check(token,d))==0){
                flag=0;
                break;
            }
            else if(rank>temp){
                rank=temp;
                x=strlen(d);
                pos=ftell(dic);
                flag=1;
            }

        }
    if(flag==1){
    fseek(dic,pos-x-2,SEEK_SET);
    fgets(d,MAX,dic);
    d[x]='\0';
    //fseek(newb,-1,SEEK_CUR);
    fputs(d,newb);
    }
    if(flag==0){
        fputs(token,newb);
    }
    fputc(' ',newb);
    token=strtok(NULL,div);
    }
    fputc('\n',newb);
}


    return 0;
}

int check(char* words,char* dic){
    int lw=strlen(words),ld=strlen(dic);
    dic[ld-1]='\0';
    if(words[lw-1]=='\n'){
        words[lw-1]='\0';
        lw--;
    }
    for(int i=0;i<=lw;i++){/////////
        for(int j=0;j<=ld-1;j++){
            if(i==0){
                arr[i][j]=j;
            }
            else if(j==0){
                arr[i][j]=i;
            }
            else if(words[i-1]==dic[j-1]){
                arr[i][j]=min_(arr[i-1][j]+1,arr[i][j-1]+1,arr[i-1][j-1],15);
            }
            else{
                arr[i][j]=min_(arr[i-1][j]+1,arr[i][j-1]+1,arr[i-1][j-1]+1,15);
            }
        }
    }

return arr[lw][ld-1];
}

int min_(int a,int b,int c,int d){
    int m1,m2,m;
if(a>b){
    m1=b;
}
else{
    m1=a;
}
if(c>d){
    m2=d;
}
else{
    m2=c;
}
if(m1>m2){
    return m2;
}
else{
    return m1;
}

}