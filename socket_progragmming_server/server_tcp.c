
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int sSock, listener;
    struct sockaddr_in srvAddr, cliAddr;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize, srvAddrLen, cliAddrLen;
    int ret, i;
    
    if(argc != 2){
        printf("Usage : %s Port\n", argv[0]);
        exit(0);
    }
    
    srvPort = atoi(argv[1]);
    
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener < 0){
        printf("Server socket() failed\n");
        exit(0);
    }
    
    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(srvPort);
    
    ret = bind(listener, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if(ret < 0){
        printf("Server cannot bind local address\n");
        exit(0);
    }
    
    printf("Server is Runing. . .\n");
    
    ret = listen(listener, 5);
    if(ret < 0){
        printf("Server failed to listen\n");
        exit(0);
    }
    
    cliAddrLen = sizeof(cliAddr);
    
    sSock = accept(listener, (struct sockaddr *)&cliAddr, &cliAddrLen);
    if(sSock < 0){
        printf("Server failed to accept\n");
        exit(0);
    }
    
    printf("Client is connected\n");
    
    while(1){
        rcvSize = read(sSock, buffer, BUFFER_SIZE);
        if(rcvSize <0){
            printf("Error in read()\n");
            exit(0);
        }
        buffer[rcvSize] = '\0';
        printf("[Client/%s:%d] %s\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), buffer);
        
        if(!strcmp(buffer, "quit")) break;
        
        for(i = 0; buffer[i] != '\0'; i++){
            buffer[i] = toupper(buffer[i]);
        }
        
        sentSize = write(sSock, buffer, strlen(buffer));
        if(sentSize != strlen(buffer)){
            printf("write() sent a different number of bytes than expected\n");
            exit(0);
        }
        
    }
    
    close(sSock);
    close(listener);
    printf("TCP Server is Closed.\n");
    
}

