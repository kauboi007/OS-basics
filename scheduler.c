#include<stdio.h>

int max(int a,int b){
    if(a>b) return a;
    return b;
}

struct process{
    int pid;
    int burst;
    int start;
    int finish;
    int turnover;
    int wait;
};

void init_process(int n,struct process*arr){
    for(int i=0;i<n;i++){
        struct process p;
        printf("enter pid:");
        scanf("%d",&p.pid);
        printf("enter burst:");
        scanf("%d",&p.burst);
        printf("enter start:");
        scanf("%d",&p.start);
        arr[i]=p;
    }
}

void fcfs(int n,struct process*arr){
    int time_elapsed=0;
    for(int i=0;i<n;i++){
        struct process *p=&arr[i];
        time_elapsed=max(time_elapsed,p->start);
        time_elapsed+=p->burst;
        p->finish=time_elapsed;
        p->turnover=p->finish-p->start;
        p->wait=p->turnover-p->burst;
    }

}

void print_results(int n,struct process*arr){
    int avg_time=0;
    for(int i=0;i<n;i++){
        printf("start :%d burst:%d end:%d pid:%d turnover:%d wait:%d",arr[i].start,arr[i].burst,arr[i].finish,arr[i].pid,arr[i].turnover,arr[i].wait);
        avg_time+=arr[i].wait;
    }   
    printf("average wait:%d",avg_time/n);
}