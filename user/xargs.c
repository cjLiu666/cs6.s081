#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#define MAXLEN 1024

int main(int argc, char* argv[]) {
    char* params[MAXARG];
    int pIndex = 0;
    char otherParam[MAXLEN];

    for (int i = 1; i < argc; i++) {
        params[pIndex++] = argv[i];
    }

    
    while (1) {
        int curPIndex = pIndex;
        int index = 0, startIndex = 0;
        char c;
        int n;

        // 获取stdin中的参数
        while (((n = read(0, &c, 1)) != 0)) {
            if (c == ' ' || c == '\n') {
                otherParam[index++] = 0;
                params[curPIndex++] = &otherParam[startIndex];
                startIndex = index;
                if (c == '\n') {
                    params[curPIndex++] = 0;
                    break;
                } 
            } else {
                otherParam[index++] = c;
            }
        }

        if (0 == n) {
            exit(0);
        }
        

        int pid = fork();
        if (pid == 0) {
            // 子进程
            exec(params[0], params);
        } else {
            // 父进程
            wait((int*)0);
        }
    }   
    exit(0);
}