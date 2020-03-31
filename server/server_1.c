/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 16时13分48秒
 ************************************************************************/
 #include "../common/common.h"
 #include "../common/tcp_server.h"
 #include "../common/chatroom.h"
 #include "../common/color.h"

 #define MAX_CLIENT 512
 char recvmemory[MAX_CLIENT][1024];
 struct User {
    char name[20];
     int online;
     pthread_t tid;
     int fd;
 };
struct User *client;

int find_sub() {
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(!client[i].online) {
            return i;
        }
        return -1;
    }
}

bool check_online(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if(client[i].online && !strcmp(name,client[i].name)) {
            return true;
        }
    }
    return false;
}

void *work(void *arg) {
//    printf(YELLOW"client login!\n");
    int *sub = (int *)arg;
    int client_fd = client[*sub].fd;
    struct RecvMsg rmsg;
    printf(GREEN"Login "NONE ": %s\n",rmsg.msg.from);
    while(1) {

        rmsg = chat_recv(client_fd);
        if(rmsg.retval < 0) {
            printf(PINK"Logout : "NONE "%s \n",client[*sub].name);
            close(client_fd);
            client[*sub].online = 0;
            return NULL;
        }
        sprintf(recvmemory[*sub],"%s: %s\n",rmsg.msg.from, rmsg.msg.message);
        chat_memory_send(recvmemory[*sub],client_fd);
        printf(BLUE"%s"NONE": %s\n",rmsg.msg.from, rmsg.msg.message);
    }
    return NULL;
}

 char *conf = "./server.conf";

 int main() {
     int port ,sever_listen,fd;
     struct RecvMsg recvmsg; 
     struct Msg msg;
     port = atoi(get_value(conf,"SERVERIP"));
     client = (struct User *)calloc(MAX_CLIENT,sizeof(struct User));

     if((sever_listen = socket_create(port)) < 0) {
         perror(RED"ket_create"NONE);
         return -1;
     }

     while(1) {
         if((fd = accept(sever_listen,NULL,NULL)) < 0) {
             perror(RED"accept"NONE);
             continue;
         }
         recvmsg = chat_recv(fd);
         if (recvmsg.retval < 0) {
             close(fd);
             continue;
         }
         if(check_online(recvmsg.msg.from)) {
             //拒绝连接
             msg.flag = 3;
             strcpy(msg.message,"Already Login!\n");
             chat_send(msg,fd);
             close(fd);
             continue;
         } 
         //同意连接
         msg.flag = 2;
         strcpy(msg.message,"Welcome to this chat room!");
         chat_send(msg,fd);
         int sub;
         sub = find_sub();//
         client[sub].online = 1;
         client[sub].fd = fd;
         strcpy(client[sub].name, recvmsg.msg.from);
         pthread_create(&client[sub].tid,NULL,work,NULL);



     }//子线程

     return 0;
 }
