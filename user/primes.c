#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int* p) {
    close(p[1]);
    int num;
    int n = read(p[0], &num, sizeof(int));
    if (0 == n) {
        exit(0);
    }

    int child[2];
    pipe(child);
    int pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        exit(0);
    } else if(pid > 0) {
        // 父进程
        close(child[0]);
        int prime = num;
        printf("prime %d\n", prime);
        while (read(p[0], &num, sizeof(int) != 0)) {
            if (num % prime != 0) {
                write(child[1], &num, sizeof(int));
            }
        }
        close(p[0]);
        close(child[1]);
        wait((int*)0);
    } else {
        // 子进程
        sieve(child);
    }
    
    
}
int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        exit(0);
    } else if (pid > 0) {
        // 父进程
        close(p[0]);
        for (int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait((int*)0);
    } else{
        // 子进程
        sieve(p);
    }
    exit(0);
}