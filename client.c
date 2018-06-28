#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
int sockfd = 0, sockfd_status = 1;
struct sockaddr_in serv_addr;

void *myThreadFun(void *vargp)
{
    printf("Entering thread function..\n");
    if( connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        sockfd_status = 0;
        sleep(1);
    }
    while(1)
    {
        if(sockfd_status == 0) {
            printf("socket connect request..\n");
            if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            {
                printf("retrying connection....\n");
                sleep(1);
                continue;
            }
        } else {
            sleep(5);
        }
    }
    return NULL;
}



int main(int argc, char *argv[])
{
    int portno=atoi(getenv("SERVER_PORT"));
    char  data[50];
    char *hostname =getenv("SERVER_IP");
    int opt = 1, msgSend = 0;
    pthread_t thread_id;

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

    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    // pthread_join(thread_id, NULL);

    while(1) {
        printf("Enter data to be sent : ");
        gets(data);
        // ssize_t write(int fd, const void *buf, size_t count);
        if( msgSend =  write(sockfd ,data, strlen(data) ) ) {
            printf("message sending successful \n");
        } else {
            printf("message sending failed \n");
            sockfd_status = 0;
        }
    }

    return 0;
}
