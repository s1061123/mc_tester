#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    struct ip_mreq mreq;

    char buf[256];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    //printf("binding...");
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    memset(&mreq, 0, sizeof(mreq));
    //mreq.imr_interface.s_addr = inet_addr("127.0.0.1");
    mreq.imr_interface.s_addr = inet_addr("192.168.123.10");
    mreq.imr_multiaddr.s_addr = inet_addr("239.1.1.2");

    //printf("done\nsetsockopt...");
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (char *)&mreq, sizeof(mreq)) != 0) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    //printf("done\nnow receiving...");
    memset(buf, 0, sizeof(buf));
    recv(sock, buf, sizeof(buf), 0);

    printf("%s\n", buf);

    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                   (char *)&mreq, sizeof(mreq)) != 0) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }
    close(sock);

    return EXIT_SUCCESS;
}
