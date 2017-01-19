#ifndef SERVER_H
#define SERVER_H

// constants
#define SERV_BUFF_READ 512
#define SERV_BUFF_WRITE 512

#define SERV_MSG_WELCOME "Welcome!\n"
#define SERV_MSC_USERNAME "Please select your username"
// .constants

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common.h"
#include "vector.h"

struct client_info
{
	struct sockaddr_in cliaddr;
	int cliaddrlen;
	int sockfd;
	char cliname[32];
};

void* serv_initconn(void* args);
void cli_broadcast(struct client_info* cli_info, const char* message);

#endif