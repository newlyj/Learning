#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
//1.创建socket套接字        
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1){
        printf("Create listen socket failed!!! errcode: %d\n", errno);
        exit(1);
    }    

    /* Enable address reuse */
    int on = 1;
    int ret = setsockopt( client_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    /* Allow connections to port 8080 from any available interface */
    struct sockaddr_in target = {0};
    memset( &target, 0, sizeof(target) );
    
    target.sin_family = AF_INET;
    target.sin_port = htons(8080);
    //服务端 选项 网卡 127.0.0.1（本地环回） 只接受某个网卡的数据
    target.sin_addr.s_addr = htonl(INADDR_ANY); 
    //printf("%d",local.sin_family);
    //local.sin_addr.s_addr = inet_addr("0.0.0.0");

//2.连接服务器
    if(-1 == connect(client_socket, (struct sockaddr*)&target, sizeof(target))){
        printf("Connect failed!!! errcode: %d\n", errno);
        close(client_socket);
        exit(1);
    }

//3.开始通讯
    while(1){
        char sbuffer[1024] = {0};
        printf("please enter: ");
        scanf("%s", sbuffer);
        send(client_socket, sbuffer, strlen(sbuffer),0);

        char rbuffer[1024] = {0};
        int ret = recv(client_socket, rbuffer, 1024, 0);

        if (ret <= 0) break;
        printf("%s\n", rbuffer);
    }
//4.关闭连接
    close(client_socket);
    return 0;
}