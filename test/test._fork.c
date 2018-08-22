#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("%d\n",getpid());
    int pid = fork();
    if (pid == 0) { // 
        printf("zi %d\n",getpid());
    } 
    else{
        
        printf("%d fu %d\n",pid,getpid());
    }
    return 0;
}