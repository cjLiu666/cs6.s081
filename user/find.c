#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


//以下代码基本来自ls.c
char* fmtname(char *path) {
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    return p;
}


void find(char* dir, char* file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    switch (st.type) {
        // 递归出口
        case T_FILE:
            if (strcmp(fmtname(dir), file) == 0) {
                printf("%s\n", dir);
            }
            break;
        case T_DIR:
            if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: dir too long\n");
                break;
            }
            strcpy(buf, dir);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                
                // 对目录下面的每个文件进行递归
                if (strcmp(fmtname(buf),  ".") != 0 && strcmp(fmtname(buf), "..") != 0) {
                    find(buf, file);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find dir file\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}