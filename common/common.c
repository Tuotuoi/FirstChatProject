/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月28日 星期六 20时21分27秒
 ************************************************************************/

#include "head.h"
#include "common.h"
char *get_value(char *path, char *key) {
    FILE *fp = NULL;
    char *sub = NULL;
    ssize_t nrd;
    char *line = NULL;
    extern char conf_ans[50];
    size_t linecap = 0;
    if(path == NULL || key == NULL) {
        fprintf(stderr,"Error in argument!\n");
        return NULL;
    }
    if((fp = fopen(path,"r")) == NULL) {
        perror("fopen");
        return NULL;
    }
    while((nrd = getline(&line,&linecap,fp)) != -1) {
        if((sub = strstr(line, key)) == NULL) {
            continue;
        } else {
            if (line[strlen(key)] == '=') {
                strncpy(conf_ans, sub + strlen(key) + 1,nrd - strlen(key) - 2);
                *(conf_ans + nrd - strlen(key) - 2) = '\0';
                break;
            }
        }
    }
    free(line);
    free(fp);
    if(sub == NULL) {
        return NULL;
    }
    return conf_ans;
}

void make_noblock_iocntl(int fd) {
    unsigned long ul = 1;
    ioctl(fd,FIONBIO,&ul);
}
void make_block_iocntl(int fd) {
    unsigned long ul = 0;
    ioctl(fd,FIONBIO,&ul);
}
void make_nonblock(int fd) {
    int flag;
    if (flag = fcntl (fd, F_GETFL, 0) < 0) {
        perror("fcntl");
        return ;
    }

    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) < 0) {
        perror("fcntl");
        return ;
    }
}

void make_block(int fd) {
    int flag;
    if (flag = fcntl (fd, F_GETFL, 0) < 0) {
        perror("fcntl");
        return ;
    }
    flag &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) < 0) {
        perror("fcntl");
        return ;
    }
}
