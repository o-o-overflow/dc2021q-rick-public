#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include "Scene.h"
#include "Camera.h"

#define EXPLOIT 0

int connect();
int recvN(int sockfd, unsigned char *buf, unsigned int size);
