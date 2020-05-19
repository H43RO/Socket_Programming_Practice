#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {
    int sock;
    struct sockaddr_in srvAddr;
    struct sockaddr_in cliAddr;
    unsigned short srvPort; //16bit
    char buffer[BUFFER_SIZE];
    int sentSize, rcvSize, srvAddrLen,cliAddrLen;
    int ret, i;
    
    if(argc != 2){ //실행 인자로 Port 번호가 오지 않으면 Error
        printf("Usage : %s Port\n", argv[0]);
        exit(0);
    }
    
    srvPort = atoi(argv[1]); //Port 번호 입력
    
    //Init socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0){
        printf("socket() failed\n");
        exit(0);
    }
    
    memset(&srvAddr, 0, sizeof(srvAddr)); //값 채우기 전에 구조체 기본값으로 초기화
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY); //서버가 갖고있는 IP 중 어느 IP이든 접속 허용
    srvAddr.sin_port = htons(srvPort); //To Network Byte
    
    //Binding
    ret = bind(sock, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if(ret < 0){
        printf("bind() failed\n");
        exit(0);
    }
    
    printf("Hi, Server!\nEnjoy Your Chatting!\n");
    printf("You can send only one sentence\n\n");
    
    while(1){
        cliAddrLen = sizeof(cliAddr);
        
        //socket을 통해 buffer로 메시지 수신 (메시지 올 때까지 Blocking)
        rcvSize = recvfrom(sock, buffer,BUFFER_SIZE, 0, (struct sockaddr *)&cliAddr, &cliAddrLen);
        
        if(rcvSize < 0){
            printf("Error in recvfrom()\n");
            exit(0);
        }
        
        buffer[rcvSize] = '\0'; //끝단에 null 문자 첨가하여 문자열로 처리
        
        if(!strcmp(buffer, "quit")){
            break;
        }
        
        printf("[Client/%s:%d] %s\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), buffer);
        //IP주소 보기좋게 변환하여 출력 (inet_ntoa)
      
        
        printf("[Server/%s:%d] : ", inet_ntoa(srvAddr.sin_addr), ntohs(srvAddr.sin_port));
        
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';
        
        sentSize = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
        
        //전송 데이터 에러 체크 (데이터 길이가 다른 상황)
        if(sentSize != strlen(buffer)){
            printf("sendto() sent a different number of bytes than expected");
            exit(0);
        }
        
        if(!strcmp(buffer, "quit")){
            break;
        }
        
    }
    
    close(sock);
    printf("Chat closed!\n");
    
    return 0;
}
