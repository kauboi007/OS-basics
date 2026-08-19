#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

struct process{
    int pid;
    int arrtime;
    int burst;
    int completed;
    int wait;
    int tat;
    int remaining;
};

struct helper{
    int n;
    struct process*arr;
    double res;
};

int max(int a,int b){
    if(a>b) return a;
    return b;
}

int min(int a,int b){
    if(a<b) return a;
    return b;	
}

double calc(int n,struct process*p){
    int avg=0;
    for(int i=0;i<n;i++){
        avg+=p[i].wait;
    }
    return (double)avg/n;
}


void init_process(int n,struct process*arr){
    for(int i=0;i<n;i++){
        struct process p;
        printf("enter pid:");
        scanf("%d",&p.pid);
        printf("enter burst:");
        scanf("%d",&p.burst);
        printf("enter start:");
        scanf("%d",&p.arrtime);
        p.remaining=p.burst;
        p.completed=0;
        p.tat=0;
        p.wait=0;
        arr[i]=p;
    }
}

int cmp(void*a,void*b){
    return (((struct process*)a)->burst)-(((struct process*)b)->burst);
}

int cmp_arrival(const void *a, const void *b) {
    const struct process *p1 = a;
    const struct process *p2 = b;
    return p1->arrtime - p2->arrtime;
}

double fcfs(int n,struct process*arr){
    qsort(arr,n,sizeof(struct process),cmp_arrival);
    int time_elapsed=0;
    for(int i=0;i<n;i++){
        struct process *p=&arr[i];
        time_elapsed=max(time_elapsed,p->arrtime);
        time_elapsed+=p->burst;
        p->completed=time_elapsed;
        p->tat=p->completed-p->arrtime;
        p->wait=p->tat-p->burst;
    }
    return calc(n,arr);
}

void* fcfsh(void*arg){
    struct helper*h=(struct helper*)arg;
    int n=h->n;
    struct process*arr=h->arr;
    (h->res)=fcfs(n,arr);
    pthread_exit(&(h->res));
}

double rr(int n,struct process*arr){
    int done=0;
    int tq=4;
    int time_elapsed=0;
    while(done!=n){
        for(int i=0;i<n;i++){
            struct process*p=&arr[i];
            if(p->remaining==0) continue;
            time_elapsed=max(time_elapsed,p->arrtime);
            int run=min(tq,p->remaining);
            p->remaining-=run;
            time_elapsed+=run;
            if(p->remaining==0){
                done+=1;
                p->completed=time_elapsed;
                p->tat=p->completed-p->arrtime;
                p->wait=p->tat-p->burst;
            }
        }
    }
    
    return calc(n,arr);
}

void* rrh(void*arg){
    struct helper*h=(struct helper*)arg;
    int n=h->n;
    struct process*arr=h->arr;
    (h->res)=rr(n,arr);
    pthread_exit(&(h->res));
}

double sjf(int n, struct process* arr) {
    int completed_count = 0;
    int time_elapsed = 0;
    
    for(int i=0; i<n; i++) arr[i].completed = 0;

    while (completed_count < n) {
        int idx = -1;
        int min_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (arr[i].arrtime <= time_elapsed && !arr[i].completed) {
                if (arr[i].burst < min_burst) {
                    min_burst = arr[i].burst;
                    idx = i;
                }
                else if (arr[i].burst == min_burst) {
                    if (arr[i].arrtime < arr[idx].arrtime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            time_elapsed += arr[idx].burst;
            arr[idx].completed = 1;
            arr[idx].tat = time_elapsed - arr[idx].arrtime;
            arr[idx].wait = arr[idx].tat - arr[idx].burst;
            completed_count++;
        } else {
            time_elapsed++;
        }
    }
    return calc(n, arr);
}

void* sjfh(void*arg){
    struct helper*h=(struct helper*)arg;
    int n=h->n;
    struct process*arr=h->arr;
    (h->res)=sjf(n,arr);
    pthread_exit(&(h->res));
}

int main(){
    printf("enter no of proccesses:");
    int n;
    scanf("%d",&n);
    struct process arr[n];
    init_process(n,arr);
    struct process*arr_fcfs=malloc(n*sizeof(struct process));
    struct process*arr_rr=malloc(n*sizeof(struct process));
    struct process*arr_sjf=malloc(n*sizeof(struct process));

    memcpy(arr_fcfs,arr,n*sizeof(struct process));
    memcpy(arr_rr,arr,n*sizeof(struct process));
    memcpy(arr_sjf,arr,n*sizeof(struct process));
    struct helper h_fcfs = { .n = n, .arr = arr_fcfs, .res = 0.0 };
    struct helper h_rr   = { .n = n, .arr = arr_rr,   .res = 0.0 };
    struct helper h_sjf  = { .n = n, .arr = arr_sjf,  .res = 0.0 };

    pthread_t t_fcfs,t_rr,t_sjf;

    pthread_create(&t_fcfs,NULL,fcfsh,&h_fcfs);
    pthread_create(&t_rr,NULL,rrh,&h_rr);
    pthread_create(&t_sjf,NULL,sjfh,&h_sjf);

    void*sfcfs;
    void*srr;
    void*ssjf;

    
    pthread_join(t_fcfs,&sfcfs);
    pthread_join(t_rr,&srr);
    pthread_join(t_sjf,&ssjf);

    printf("FCFS : %.2f\n",*(double*)sfcfs);
    printf("RR : %.2f\n",*(double*)srr);
    printf("SJF : %.2f\n",*(double*)ssjf);

    char best_algo[10];
    double min_avg = res_fcfs;
    strcpy(best_algo, "FCFS");

    if (res_sjf < min_avg) {
        min_avg = res_sjf;
        strcpy(best_algo, "SJF");
    }
    if (res_rr < min_avg) {
        min_avg = res_rr;
        strcpy(best_algo, "RR");
    }

    printf("\nAlgo with minimum average waiting time is : %s (%.2f)\n", best_algo, min_avg);

    free(arr_fcfs);
    free(arr_rr);
    free(arr_sjf);

    return 0;
}

