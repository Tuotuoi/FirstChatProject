/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 15时59分24秒
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
char ans[50] = {0};

int main() {
    printf("%s",get_value("./a.conf","SERVERIP"));
}
