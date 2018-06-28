#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
int sockfd;
struct sockaddr_in serv_addr;

void *myThreadFun(void *vargp)
{
    while(1)
    {
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
           printf("Port is closed\n");
           sleep(1);
           continue;       
        } 
        else 
        {
           break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{  
    int portno=7575;
    char  data[50];
    char *hostname ="10.105.206.136";
    int opt = 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        error("ERROR opening socket");
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(hostname);
    printf(" SERVER IP ADDR= %d %s\n",serv_addr.sin_addr.s_addr,hostname);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
          printf("Port is closed\n");
          pthread_t thread_id;
          pthread_create(&thread_id, NULL, myThreadFun, NULL);
          pthread_join(thread_id, NULL);
    } 
    printf("Port is active\n\n Enter data to be sent\n");
    gets(data);
    send(sockfd ,data, strlen(data) , 0 );
    return 0;
}
