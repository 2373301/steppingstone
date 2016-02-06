#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAXBUF 1024

int main(int argc, char **argv)
{
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;

    if (argc != 3) {
        printf("parameter error! argv[1] is ip,use xxx.xxx.xxx.xxx format, argv[2] is port.\n");
        exit(0);
    }
    /* create socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(errno);
    }

    /* get the ip and port */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
        perror(argv[1]);
        exit(errno);
    }
    
    /* connect */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connect ");
        exit(errno);
    }

    printf("\n ok, you can send message to the server!\n");
    while (1) {
        
        FD_ZERO(&rfds);
        /* add stdin */
        FD_SET(0, &rfds);
        maxfd = 0;
        /* add socket */
        FD_SET(sockfd, &rfds);
        if (sockfd > maxfd)
            maxfd = sockfd;
       
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) 
        {
            printf("select error! %s", strerror(errno));
            break;
        } else if (retval == 0) 
        {
             
             continue;
        } 
        else 
        {
            if (FD_ISSET(sockfd, &rfds)) 
            {
                
                bzero(buffer, MAXBUF + 1);
                
                len = recv(sockfd, buffer, MAXBUF, 0);
                if (len > 0)
                {
                    printf("recv message:%s,len=%d.\n",buffer, len);
                }
                else 
                {
                    if (len < 0)
                    {
                        printf("recv message error,errno=%d,errstr=%s.\n",errno, strerror(errno));
                    }
                    else
                    {
                        printf("the peer is not online.\n");
                    }
                    break;
                }
            }
            if (FD_ISSET(0, &rfds)) 
            {
               
                bzero(buffer, MAXBUF + 1);
                fgets(buffer, MAXBUF, stdin);
                if (!strncasecmp(buffer, "quit", 4)) 
                {
                    printf("quit!\n");
                    break;
                }
                /*send the message */
                len = send(sockfd, buffer, strlen(buffer) - 1, 0);
                if (len < 0) 
                {
                    printf("send message error,msg=%s,error=%d,errstr=%s.\n", buffer, errno, strerror(errno));
                    break;
                } 
                else
                {
                    printf("ok,send message:%s,len=%d.\n",buffer, len);
                }
            }
        }
    }
   
    close(sockfd);
    return 0;
}
