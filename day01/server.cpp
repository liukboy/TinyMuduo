#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>

#define ADDR "127.0.0.1"

int main()
{

    char buf[100] = "hello client";
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        printf("create server socket failed!");
        return -1;
    }
    unsigned int iRemoteAddr = 0;
    struct sockaddr_in stRemoteAddr = {0}; // 对端，即目标地址信息
    socklen_t socklen = 0;
    stRemoteAddr.sin_family = AF_INET;
    stRemoteAddr.sin_port = htons(8888);
    stRemoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sfd, (sockaddr *)&stRemoteAddr, sizeof(stRemoteAddr)) < 0)
    {
        printf("server bind failed,errno:  ");
        return 0;
    }

    listen(sfd, 5);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int cfd = accept(sfd, (sockaddr *)&clnt_addr, &clnt_addr_len);

    if (cfd < 0)
    {
        printf("accept failed!");
        return -1;
    }
    else
    {
        // 发数据
        write(cfd, buf, sizeof(buf));
    }

    close(cfd);
    close(sfd);
    return 0;
}