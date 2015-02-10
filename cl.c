#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/un.h>  
#include <sys/time.h>  
#include <sys/ioctl.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <pthread.h>  
#define PORT_NUM 21314
#define IP_ADDR "108.75.26.73"	//server ip address
char recv_buf[1500],send_buf[1024];  

char recv_buf_name[3];
char name_compare[3];
char name[20];
  
void display_function(int sockfd){   //receive message from server and display
    int recvbytes;  
    while(1){  
        if((recvbytes = recv(sockfd, recv_buf, 1500, 0))==-1){  
            perror("recv error");  
            exit(1);  
        }else{  
            recv_buf[recvbytes]='\0';
		strncpy(recv_buf_name,recv_buf,3);
		strncpy(name_compare,name,3);
		if(strcmp(name_compare,recv_buf_name)!=0){  
            	printf("%s\n",recv_buf);
		}  
        }  
    }  
}  
  
int main(void){  
    pthread_t id;  
    int sockfd;  
    struct sockaddr_in server_addr;  
    server_addr.sin_family = AF_INET;  //config
    server_addr.sin_port = htons(PORT_NUM);  
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);  
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){  
        perror("socket error");  
        exit(1);  
    }  
    if(connect(sockfd, (struct sockaddr*)&server_addr,sizeof(server_addr))==-1){  
        perror("connect error");  
        exit(1);  
    }
//--------------send username to server--------------
//    char name[20];
    printf("input your name:");  
    scanf("%s",name);  
    send(sockfd,name,strlen(name),0);  

    pthread_create(&id,NULL,(void *)display_function,(int *)sockfd);  
    while(1){  //send message to server
        printf("You:");
	gets(send_buf);  
        if(send(sockfd,send_buf,strlen(send_buf),0)==-1){  
            perror("send error");  
            exit(1);  
        }  
        sleep(1);  
    }  

    close(sockfd);  //close socket and cancle thread
    pthread_cancel(id);  
    return 0;  
}  
