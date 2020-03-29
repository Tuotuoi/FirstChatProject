/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 16时13分48秒
 ************************************************************************/
 #include "../common/common.h"
 #include "../common/tcp_server.h"
 #include "../common/chatroom.h"

 #define MAX_CLIENT 512
 struct User {
    char name[20];
     int online;
     pthread_t tid;
     int fd;
 };
struct User *client;
bool check_online(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if(client[i].online && !strcmp(name,client[i].name)) {
            return true;
        }
    }
    return false;
}

 char *conf = "./server.conf"
 int main() {
     int port ,sever_listen;
     struct RecvMsg recvmsg; 
     port = atoi(get_value(conf,"SERVERIP"));
     client = (struct User *)calloc(MAX_CLIENT,sizeof(struct User));

     if((sever_listen = socket_create(port)) < 0) {
         perror("socket_create");
         return -1;
     }

     while(1) {
         if((fd = accept(sever_listen,NULL,NULL)) < 0) {
             perror("accept");
             continue;
         }
         recvmsg = chat_recv(fd);
         if (recvmsg.retval < 0) {
             close(fd);
             continue;
         }
         if(check_online(recvmsg.msg.from)) {
             //拒绝连接
         } else { //同意连接
             int sub;
             sub = find_sub();//
                 client[sub].online = 1;
                 client[sub].fd = fd;
                 strcpy(client[sub].name, recvmsg.msg.from);
                 pthread_create(&client[sub].tid,NULL,work,NULL);
         }

     }//子线程

     return 0;
 }
