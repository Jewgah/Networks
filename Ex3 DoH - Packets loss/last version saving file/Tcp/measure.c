
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
    //time
    clock_t t;
    double average = 0;
    char buff[MAX];
    
      FILE *fp;
  char *filename = "recv.txt";

  fp = fopen(filename, "wb");
  
    
    
    
    for (int i=0;i<5;i++) {
    
        t = clock(); // start the clock
        	
        bzero(buff, MAX);
        //read the message from client and copy it in buffer
        
        //read(sockfd, buff, sizeof(buff));
          fwrite(buff,sizeof(buff),1,fp);
        // print buffer which contains the client contents
        
        t = clock() - t; //get time spent
        
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
        average+=time_taken;
        
        printf("Received file %d from client in %0.7f seconds\n",i+1,time_taken);
    }
    average/=5;
    printf("\nAverage time: %0.7f seconds\n",average);
    
}

// Driver function
int main()
{
  //for algo
  socklen_t len1;
  char algo[256];

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    //type algo
    len1 = sizeof(algo);
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algo, &len1) != 0)
    {
            perror("getsockopt");
            return -1;
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
        
            printf("\nCurrently using %s:\n", algo);

    // Function for chatting between client and server
    func(connfd);


            strcpy(algo, "reno");
            len1 = strlen(algo);
            //set new
            if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, algo, len1) != 0)
            {
                    perror("setsockopt");
                    return -1;
            }
            printf("\nNow using %s:\n", algo);

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}
