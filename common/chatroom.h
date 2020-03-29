/*************************************************************************
	> File Name: chatroom.h
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 16时27分04秒
 ************************************************************************/

#ifndef _CHATROOM_H
#define _CHATROOM_H

#include "head.h"
struct Msg {
    char from[20];
    int flag;
    char message[512];
};

struct RecvMsg {
    struct Msg msg;
    int retval;
};
struct RecvMsg chat_recv(int fd) {
    struct RecvMsg tmp;
    memset(&tmp,0,sizeof(tmp));
    if(recv(fd,&tmp.msg,sizeof(struct Msg),0) <= 0) {
        tmp.retval = -1;
    } 
    return tmp;
#endif
