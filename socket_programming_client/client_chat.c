#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock, ret;
    struct sockaddr_in cliAddr;
    struct sockaddr_in srvAddr;
    char *srvIp = NULL;
    unsigned short srvPort;
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize, srvAddrLen;
    
    if(argc != 3){
        printf("Usage : %s IP_addr Port\n", argv[0]);
        exit(0);
    }
    
    srvIp = argv[1];
    srvPort = atoi(argv[2]);
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0){
        printf("socket() failed\n");
        exit(0);
    }
    
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(srvIp);
    srvAddr.sin_port = htons(srvPort);
    
    //Binding
    ret = bind(sock, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
    if(ret < 0){
        printf("bind() failed\n");
        exit(0);
    }
     
    
    printf("Hi, Client!\nEnjoy Your Chatting!\n");
    printf("You can send only one sentence\n\n");
    
    while(1){
        
        printf("[Client/%s:%d] : ", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
        
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';
        
        sentSize = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&srvAddr, sizeof(srvAddr));

        if(!strcmp(buffer, "quit")){
            break;
        }
        
        if(sentSize != strlen(buffer)){
            printf("sendto() sent a different number of bytes than expected\n");
            exit(0);
        }
        
        srvAddrLen = sizeof(srvAddr);
        rcvSize = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&srvAddr, &srvAddrLen);
        
        
        if(rcvSize < 0){
            printf("Error in recvfrom()\n");
            exit(0);
        }
        
        buffer[rcvSize] = '\0'; //끝단에 null 문자 첨가하여 문자열로 처리
        
        if(!strcmp(buffer, "quit")){
            break;
        }
        
        printf("[Server/%s:%d] %s\n", inet_ntoa(srvAddr.sin_addr), ntohs(srvAddr.sin_port), buffer);
    }
    
    close(sock);
    printf("Chat closed!\n");

    return 0;
}
