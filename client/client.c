/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 18时16分13秒
 ************************************************************************/

#include "../common/common.h"
#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/color.h"

char *conf = "./client.conf";

int sockfd;
void logout(int signalnum) {
    close(sockfd);
    exit(3);
    printf("revc logout\n");
}

int main() {
    int port;
    struct Msg msg;
    struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    char ip[20] = {0};
    port = atoi(get_value(conf,"SERVER_PORT"));
    strcpy(ip,get_value(conf,"SERVER_IP"));
    printf("ip = %s, port = %d\n",ip,port);

    if((sockfd = socket_connect(ip,port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    strcpy(msg.from,get_value(conf,"MY_NAME"));
    printf("%s\n",msg.from);
    msg.flag = 2;
    if(chat_send(msg,sockfd) < 0) {
        return 2;
    }
    struct RecvMsg rmsg = chat_recv(sockfd);
    if(rmsg.retval < 0) {
        fprintf(stderr,"Error!\n");
        return 1;
    }
    printf(GREEN"server"NONE": %s\n",rmsg.msg.message);
    if(rmsg.msg.flag == 3) {
        close(sockfd);
    }
    pid_t pid;
    if((pid = fork()) < 0) {
        perror("fork");
    }
    if(pid == 0) {
    signal(SIGINT,logout);
        system("clear");
        char c;
        while(c != EOF) {
            printf(L_PINK"please Input Message :"NONE"\n");
            scanf("%[^\n]s",msg.message);
            c = getchar();
            for(int i = 0; i < size.ws_col - strlen(msg.message) - 1; i++) {
                printf(" ");
            }
            printf(GREEN"%s :"NONE,msg.from);
            printf(L_GREEN"%s"NONE,msg.message);
            chat_send(msg,sockfd);
            memset(msg.message,0,sizeof(msg.message));
            //system("clear");
        } 
        close(sockfd);
    }
    else {
        wait(NULL);
        close(sockfd);
    }
    return 0;
}
