#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>



void communication(void *arg){
    //5.开始通信
    int client_socket = *(int*)arg;
    while ((1))
    {
        char buffer[1024] = {0};
        int ret = recv(client_socket, buffer, 1024, 0);
        if(ret <= 0) break;
        printf("%d: %s\n", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer),0);
    }
    //6.关闭连接
    close(client_socket);
}

int main(int argc, char *argv[])
{
//1.创建socket套接字        
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1){
        printf("Create listen socket failed!!! errcode: %d\n", errno);
        exit(1);
    }    

    /* Enable address reuse */
    int on = 1;
    int ret = setsockopt( listen_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    /* Allow connections to port 8080 from any available interface */
    struct sockaddr_in local = {0};
    memset( &local, 0, sizeof(local) );
    
    local.sin_family = AF_INET;
    local.sin_port = htons(8080);
    //服务端 选项 网卡 127.0.0.1（本地环回） 只接受某个网卡的数据
    local.sin_addr.s_addr = htonl(INADDR_ANY); 
    //printf("%d",local.sin_family);
    //local.sin_addr.s_addr = inet_addr("0.0.0.0");

//2.绑定端口号 
    if(-1 == bind(listen_socket, (struct sockaddr*)&local, sizeof(local))){
        printf("Bind socket failed!!! errcode: %d\n", errno);
        exit(1);
    }

//3.开启监听属性
    if(-1 == listen(listen_socket, 10)){
        printf("listen socket failed!!! errcode: %d\n", errno);
        exit(1);
    }
//4.等待客户端连接
    while(1){
        int client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == -1){
            printf("Create listen socket failed!!! errcode: %d\n", errno);
            continue;        
            }
        
            //创建线程
        //int *sockfd = (int *)malloc(sizeof(int));
        //*sockfd = client_socket;

        pthread_t thread;
        pthread_create(&thread, NULL, (void *)(communication), (void*)&client_socket);

    }
    return 0;

}