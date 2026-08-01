
int _start(){
    const char arr[]="hello world\n";
    asm volatile(
        "mov $1 , %%rax\n\t"
        "mov $1 , %%rdi\n\t"
        "lea %0 , %%rsi\n\t"
        "mov $14 ,%%rdx\n\t"
        "syscall\n\t"
        :
        :"m"(arr)
        :"%rax","%rdi","%rsi","%rdx"

    );
    asm  volatile(
        "mov $60 , %%rax\n\t"
        "mov $0 , %%rdi\n\t"
        "syscall\n\t"
        :::"rdx","rdi"
    );
    
}