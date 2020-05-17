
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int cSock;
    struct sockaddr_in srvAddr;
    char *srvIp = NULL;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize, srvAddrLen;
    int ret;
    
    if(argc != 3){
        printf("Usage : %s IP_addr Port\n", argv[0]);
        exit(0);
    }
    
    srvIp = argv[1];
    srvPort = atoi(argv[2]);
    
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
    
    printf("Client is Runing. . .\n");
    printf("Enter the word to translate into capitals\n");
    
    while(1){
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';
        
        sentSize = write(cSock, buffer, strlen(buffer));
        if(sentSize != strlen(buffer)){
            printf("write() sent a different number of bytes than expected\n");
            exit(0);
        }
        
        if(!strcmp(buffer, "quit")) break;
        
        rcvSize = read(cSock, buffer, BUFFER_SIZE);
        if(rcvSize < 0){
            printf("Error in read()\n");
            exit(0);
        }
        
        buffer[rcvSize] = '\0';
        printf("[Server/%s:%d] %s\n", inet_ntoa(srvAddr.sin_addr), ntohs(srvAddr.sin_port), buffer);
        
    }
    
    close(cSock);
    printf("TCP Server is Closed.\n");
    
}

