#include<stdio.h>

enum syscalls_numbers{
    SYS_READ = 0,
    SYS_WRITE = 1,
    SYS_EXIT = 60,
    SYS_OPEN = 2 , 
    SYS_CLOSE = 3
};  
// kernel stores the PID,name,arg count and description of all syscalls in a struct
struct syscalls{
    int number;
    char *name;
    int arg_count;
    char *description;

};

int trace_mode=1;

void dispatch_syscalls(int syscall_number,long args[]){
    if(!(args[0]<0||args[0]>1023||args[1]<=0)){
        if(syscall_number==SYS_WRITE){
            if(trace_mode==1) printf("[TRACE] ");
            printf("write(%ld,buf,%ld)\n",args[0],args[1]);
        }
        else if(syscall_number==SYS_EXIT){
            if(trace_mode==1) printf("[TRACE] ");
            printf("program exited with code: %ld\n",args[0]);
        }
        else if(syscall_number==SYS_CLOSE){
            if(trace_mode==1) printf("[TRACE] ");
            printf("closing file : %ld\n",args[0]);
        }
        else if(syscall_number==SYS_OPEN){
            if(trace_mode==1) printf("[TRACE] ");
            printf("opening file : %ld\n",args[0]);
        }
        else{
            if(trace_mode==1) printf("[TRACE] ");
            printf("syscall %d handled\n",syscall_number);
        }
    }
    else{
        printf("[KERNEL] Error : Invalid args");
        return;
    }
}

int main(){
    long arg0[]={1,2,3};
    long arg1[]={1};
    long arg2[]={1};
    long arg3[]={0};
    dispatch_syscalls(SYS_OPEN,arg0);
    dispatch_syscalls(SYS_WRITE,arg1);
    dispatch_syscalls(SYS_CLOSE,arg2);
    dispatch_syscalls(SYS_EXIT,arg3);
    return 0;
}