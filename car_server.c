#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <pigpio.h> // 替换 wiringPi

#define BUFSIZE 512

// 电机控制宏
#define MOTOR_GO_FORWARD   gpioWrite(1,1); gpioWrite(4,0); gpioWrite(5,1); gpioWrite(6,0)
#define MOTOR_GO_BACK      gpioWrite(1,0); gpioWrite(4,1); gpioWrite(5,0); gpioWrite(6,1)
#define MOTOR_GO_RIGHT     gpioWrite(1,1); gpioWrite(4,0); gpioWrite(5,0); gpioWrite(6,0)
#define MOTOR_GO_LEFT      gpioWrite(1,0); gpioWrite(4,0); gpioWrite(5,1); gpioWrite(6,0)
#define MOTOR_GO_STOP      gpioWrite(1,0); gpioWrite(4,0); gpioWrite(5,0); gpioWrite(6,0)

typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;
} CLIENT;

void executeCommand(unsigned char command) {  // 提取重复的 switch 代码
    switch(command) {
        case 0x01: MOTOR_GO_FORWARD; printf("forward\n"); break;
        case 0x02: MOTOR_GO_BACK;    printf("back\n"); break;
        case 0x03: MOTOR_GO_LEFT;    printf("left\n"); break;
        case 0x04: MOTOR_GO_RIGHT;   printf("right\n"); break;
        case 0x00: MOTOR_GO_STOP;    printf("stop\n"); break;
        default: MOTOR_GO_STOP;      printf("invalid command\n"); break;
    }
}

int main(int argc, char *argv[]) {
    int sockfd, listenfd, connectfd;
    int ret, maxfd = -1;
    struct timeval tv;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    long portnumber;
    char buf[BUFSIZE];
    int i, maxi = -1, k;
    fd_set rset, allset;
    CLIENT client[FD_SETSIZE];

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *endptr;
    portnumber = strtol(argv[1], &endptr, 10);  // 替换 atoi，增加输入检查
    if (*endptr != '\0' || portnumber <= 0 || portnumber > 65535) {
        fprintf(stderr, "Invalid port number!\n");
        exit(EXIT_FAILURE);
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portnumber);

    if (bind(listenfd, (struct sockaddr *)(&server_addr), sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 128) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if (gpioInitialise() < 0) {  // 替换 wiringPiSetup
        fprintf(stderr, "Failed to initialize GPIO\n");
        exit(EXIT_FAILURE);
    }

    // 初始化 GPIO
    for (i = 1; i <= 6; i++) gpioSetMode(i, PI_OUTPUT);
    gpioSetMode(3, PI_OUTPUT);
    MOTOR_GO_STOP;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    printf("Waiting for client's request...\n");

    while (1) {
        rset = allset;
        tv.tv_sec = 1;  // 调整等待时间，提高 CPU 效率
        tv.tv_usec = 0;

        ret = select(maxfd + 1, &rset, NULL, NULL, &tv);
        if (ret == -1) {
            perror("Select failed");
            break;
        } else if (ret == 0) {
            continue;
        }

        if (FD_ISSET(listenfd, &rset)) {  // 处理新连接
            len = sizeof(client_addr);
            connectfd = accept(listenfd, (struct sockaddr *)(&client_addr), &len);
            if (connectfd == -1) {
                perror("Accept failed");
                continue;
            }
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connectfd;
                    client[i].addr = client_addr;
                    printf("Connection from %s\n", inet_ntoa(client[i].addr.sin_addr));
                    break;
                }
            }
            if (i == FD_SETSIZE) printf("Too many connections\n");
            FD_SET(connectfd, &allset);
            if (connectfd > maxfd) maxfd = connectfd;
            if (i > maxi) maxi = i;
        }

        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i].fd) < 0) continue;
            if (FD_ISSET(sockfd, &rset)) {
                memset(buf, 0, BUFSIZE);
                if ((ret = read(sockfd, buf, BUFSIZE - 1)) > 0) {
                    buf[ret] = '\0';  // 避免缓冲区溢出
                    if (ret == 5 || ret == 6) {
                        if (buf[1] == 0x00) executeCommand(buf[2]);
                        else MOTOR_GO_STOP;
                        gpioWrite(3, (buf[1] == 0x00) ? 1 : 0);
                    } else {
                        MOTOR_GO_STOP;
                    }
                } else {
                    printf("Client disconnected\n");
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    memset(&client[i], 0, sizeof(CLIENT));  // 释放资源
                }
            }
        }
    }

    close(listenfd);
    gpioTerminate(); // 关闭 GPIO
    return 0;
}
