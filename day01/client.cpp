#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#define ADDR "127.0.0.1"

int main()
{
    char buf[100];
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0)
    {
        printf("create client socket failed!");
        return -1;
    }
    unsigned int iRemoteAddr = 0;
    struct sockaddr_in stRemoteAddr = {0}; // 对端，即目标地址信息
    socklen_t socklen = 0;
    stRemoteAddr.sin_family = AF_INET;
    stRemoteAddr.sin_port = htons(8888);
    inet_pton(AF_INET, ADDR, &iRemoteAddr);
    stRemoteAddr.sin_addr.s_addr = iRemoteAddr;

    if (connect(cfd, (sockaddr *)&stRemoteAddr, sizeof(stRemoteAddr)) < 0)
    {
        printf("connet error!");
    }
    else
    {
        // 接收数据
        read(cfd, buf, 100);
        printf("read buf:%s", buf);
    }
    close(cfd);
    return 0;
}