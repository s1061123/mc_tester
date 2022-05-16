#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char buf[256][256];

int read_buf(char *filename)
{
    FILE *fp;
    int line = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "No such file %s\n", filename);
        exit(0);
    }
    while(!feof(fp)) {
        //fgetws(buf[line], sizeof(buf[line]), fp);
        fgets(buf[line], sizeof(buf[line]), fp);
        line++;
    }
    return line;
}

int main (int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    in_addr_t ipaddr;
    size_t len;
    size_t text_len, str_len;
    int cnt;

    text_len = read_buf("test.txt");

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = inet_addr("239.1.1.2");

    //ipaddr = inet_addr("127.0.0.1");
    ipaddr = inet_addr("192.168.123.254");
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,
                   (char *)&ipaddr, sizeof(ipaddr)) != 0) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    while (1) {
        for (cnt = 0; cnt < text_len - 1; cnt++) {
            str_len = strlen(buf[cnt]);
            buf[cnt][str_len-1] = '\0';
                /*
            str_len = wcsnlen(buf[cnt], 256);
                         */
            len = sendto(sock, buf[cnt], str_len,
                         0, (struct sockaddr *)&addr, sizeof(addr));
            sleep(1);
            printf("%s / idx:%d / %d sent\n", buf[cnt], cnt, len);
        }
    }

    //len = sendto(sock, "HELLO", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
    return EXIT_SUCCESS;
}
