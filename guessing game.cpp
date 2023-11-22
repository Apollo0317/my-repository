#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
void guess(void);
void trick(int);
int main(){
guess();
return 0;    
}
void guess(void ){
char arr[10];    
int m;//存放猜的数；
int i=1;//存放猜的次数；
int number;
int t=1;
int r;
char c;
printf("--------------round%d\n",t++);    
while (1){
srand((unsigned)time(NULL));
r=rand()%100+1;    
i=1;    
trick(r);
printf("规定次数上限\n");
scanf("%d",&number);    
printf("猜个数吧\n");
scanf("%d",&m);
if(number==1){
   if(m==r){
    printf("amazing man, right answer\n");}
    else{
    printf("猜错了，情理之中\n");
    }
}
else{
    while(m!=r&&i<number){
        i++;
            if((r-m)>=10){
            printf("猜太小啦");
            }
            else if((r-m)>0){
            printf("有点小了");    
            }
            else if((m-r)>=10){
            printf("猜太大啦");    
            }
            else{
            printf("有点大了");    
            }
        printf(" 还有%d次机会\n",number-i+1);    
        scanf("%d",&m);
    }
    if(r==m){
printf("you got it! it only take %d times\n",i);}
    else{
printf("bad luck bro.\n");      
}

}//zone
printf("have another round?\n");
while(getchar()!='\n'){
continue;} 
c=getchar();
if(c=='n'){
printf("ok,see you\n");    
break;
}
else if(c=='y'){
printf("ok");    
}
else{
printf("I don't understand\n");
while(getchar()!='\n'){
continue;}
while((c=getchar())!='y'&&c!='n'){
while(getchar()!='\n'){
continue;}
printf("I don't understand\n");
}
if(c=='n'){
printf("ok,see you\n");    
break;}
else
printf("ok");
}   
//zone
while(getchar()!='\n'){
continue;}
printf("--------------round%d\n",t++);
}

}//fun1
void trick(int r){
int c;
printf("输入任意数字以继续\n");
scanf("%d",&c);  
if(c==666){
printf("作弊模式已激活，答案是%d\n",r);    
}
}//fun2




