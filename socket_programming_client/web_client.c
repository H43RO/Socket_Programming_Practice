#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 100000

int main(int argc, char *argv[]){
    int cSock;
    struct sockaddr_in srvAddr;
    char *srvIp = NULL;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize, srvAddrLen;
    int ret;
    
    if(argc != 2){
        printf("Usage : %s IP_addr\n", argv[0]);
        exit(0);
    }
    
    srvIp = argv[1];
    srvPort = 80;
    
    cSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(cSock < 0){
        printf("socket() failed\n");
        exit(0);
    }
    
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(srvIp);
    srvAddr.sin_port = htons(srvPort);
    
    ret = connect(cSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if(ret < 0){
        printf("Client cannot connect to the Server\n");
        exit(0);
    }
    
    printf("\n\n");
    
    strcpy(buffer, "GET / HTTP/1.1\r\n");
    write(cSock, buffer, strlen(buffer));
    strcpy(buffer, "Host: kostat.go.kr\r\n");
    write(cSock, buffer, strlen(buffer));
    strcpy(buffer, "\r\n");
    write(cSock, buffer, strlen(buffer));
    
    rcvSize = read(cSock, buffer, BUFFER_SIZE);
    if(rcvSize < 0){
        printf("Error in read()\n");
        exit(0);
    }
    
    buffer[rcvSize] = '\0';
    printf("%s", buffer);
    
    close(cSock);
    
}

