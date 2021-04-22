#include "utils.h"


int recvN(int sockfd, unsigned char *buf, unsigned int size) {
    for (unsigned int i=0; i<size; i++) {
        unsigned int n = recv(sockfd, buf+i, 1, 0);
        if (n != 1) {
#if DEBUG
            cerr << "err during recv. Got " << i << ", was expecting: " << size << endl;
#endif
            return -1;
        }
    }
    return size;
}


int connect() {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = 4343;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        exit(1);
    }

    int flag = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    server = gethostbyname("rick.challenges.ooo");
    if (server == NULL) {
        cerr << "connection error" << endl;
#if DEBUG
        cerr << "error, no such host" << endl;
#endif
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  {
        cerr << "connection error" << endl;
        exit(1);
    }

    char rick[4];
    memset(rick, 0, 4);
    int n = recvN(sockfd, (unsigned char *) rick, 4);
    if (n == -1) {
        cerr << "connection error" << endl;
        exit(1);
    }

    if (strncmp(rick, "RICK", 4) != 0) {
        cerr << "connection error" << endl;
        exit(1);
    }

    return sockfd;
}
