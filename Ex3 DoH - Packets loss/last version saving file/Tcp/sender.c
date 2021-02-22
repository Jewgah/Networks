
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>



#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SIZE 1024
#define PORT 8080
#define SA struct sockaddr

void func(FILE *fp,int sockfd)
{
        char buff[SIZE];
        int n;
        bzero(buff, sizeof(buff));
        fgets(buff, SIZE, fp);
        for (int i=0; i<5; i++) {
                write(sockfd, buff, sizeof(buff));
        }
}

int main()
{
        FILE *fp;
        char *filename = "1mb.txt";
        //for algo
        socklen_t len;
        char algo[256];

        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;

        // socket create and varification
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
        }
        else
                printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        //type algo
        len = sizeof(algo);
        if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algo, &len) != 0)
        {
                perror("getsockopt");
                return -1;
        }
        printf("Current: %s\n", algo);

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);

        // connect the client socket to server socket
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
                printf("connection with the server failed...\n");
                exit(0);
        }
        else
                printf("connected to the server..\n");

        fp = fopen(filename, "rb");
        if (fp == NULL) {
                perror("[-]Error in reading file.");
                exit(1);
        }

        // function for chat
        func(fp,sockfd);

        strcpy(algo, "reno");
        len = strlen(algo);
        //set new
        if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algo, len) != 0)
        {
                perror("setsockopt");
                return -1;
        }
        printf("New: %s\n", algo);
        fp = fopen(filename, "r");
        if (fp == NULL) {
                perror("[-]Error in reading file.");
                exit(1);
        }
        func(fp,sockfd);
        // close the socket
        close(sockfd);
}
