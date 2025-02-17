#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}

void find(char *path, char *tar)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }


    switch(st.type){
    case T_FILE:
        if(strcmp(fmtname(path),tar)==0){
            printf("%s\n",path);
        }
        close(fd);  // close很重要
        return; 

    case T_DIR:
        if(strcmp(fmtname(path),tar)==0){
            printf("%s\n",path);
        }
        
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/'; // 补充路径'/'
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;// 字符串结束符


            if(strcmp(de.name,".")==0 || strcmp(de.name, "..")==0){
                continue;
            }

            find(buf,tar);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc < 3){
        fprintf(2,"Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}
