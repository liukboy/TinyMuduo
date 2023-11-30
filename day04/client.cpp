#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "sockfd create failed!");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(connect(sockfd, (sockaddr *)&addr, sizeof(addr)) == -1, "connect failed!");

    while (1)
    {
        char buf[1024];
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        printf("%d \n", write_bytes);
        if (write_bytes < 0)
        {
            printf("socket already disconnect \n");
            break;
        }
        ssize_t len = read(sockfd, buf, sizeof(buf));
        if (len > 0)
        {
            printf("read buf: %s", buf);
        }
        else if (len == 0)
        {
            printf("server socket already disconnect \n");
            close(sockfd);
            break;
        }
        else
        {
            close(sockfd);
            errif(1, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}