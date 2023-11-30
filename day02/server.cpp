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

    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(bind(sockfd, (sockaddr *)&addr, sizeof(addr)) == -1, "bind error");
    errif(listen(sockfd, 5) == -1, "listen error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_len = sizeof(clnt_addr);
    int clntfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_len);
    errif(clntfd == -1, "clnt_addr failed");
    while (1)
    {
        char buf[1024];
        // bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(clntfd, buf, sizeof(buf));

        if (read_bytes > 0)
        {
            printf("%s \n", buf);
            write(clntfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("client disconnect!");
            close(clntfd);
            break;
        }
        else if (read_bytes < 0)
        {
            close(clntfd);
            errif(1, "write error!");
        }
    }
    close(sockfd);
    return 0;
}