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
#include "server.h"

vec *vec_cli_infos;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        errexit("No port number provided.\n");
    }

    int port = atoi(argv[1]);

    int listenfd, errno;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    errno = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (errno < 0)
    {
        errexit("Binding error.\n");
    }

    vec_cli_infos = vector(sizeof(struct client_info), 64);

    listen(listenfd, 5);

    struct sockaddr_in cli_addr;
    int connfd, clilen = sizeof(cli_addr);

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

        struct client_info* cli_info;
        cli_info = malloc(sizeof(struct client_info));

        (*cli_info).cliaddr = cli_addr;
        (*cli_info).cliaddrlen = clilen;
        (*cli_info).sockfd = connfd;

        pthread_t thread;
        errno = pthread_create(&thread, NULL, serv_initconn, (void*)cli_info);
    }

    close(listenfd);
    vector_free(vec_cli_infos);

    return 0;
}

void* serv_initconn(void* args)
{
    struct client_info* cli_info = args;

    vector_push(vec_cli_infos, cli_info, sizeof(*cli_info));

    char readbuffer[SERV_BUFF_READ], writebuffer[SERV_BUFF_WRITE];
    bzero(readbuffer, sizeof(readbuffer));
    bzero(writebuffer, sizeof(writebuffer));

    printf("%d is connected.\n", cli_info->sockfd);

    strcpy(writebuffer, SERV_MSG_WELCOME);
    write(cli_info->sockfd, writebuffer, strlen(SERV_MSG_WELCOME));

    int readlen;
    while ((readlen = read(cli_info->sockfd, &readbuffer, SERV_BUFF_READ)))
    {
        if (readlen > 1)
        {
            printf("%d>%s", cli_info->sockfd, readbuffer);
            serv_broadcast(cli_info->sockfd, readbuffer);
            bzero(readbuffer, sizeof(readbuffer));
        }
    }

    close(cli_info->sockfd);

    printf("%d is disconnected.\n", cli_info->sockfd);
}

void serv_broadcast(int src_sockfd, const char* message)
{
    char text[SERV_BUFF_WRITE];
    bzero(text, sizeof(text));
    sprintf(text, "%d says: %s", src_sockfd, message);

    struct client_info* ci = malloc(sizeof(struct client_info));
    int i, count = vec_cli_infos->size;

    for (i = 0; i < count; i++)
    {
        vector_getp(vec_cli_infos, ci, i);

        if (ci->sockfd != src_sockfd)
        {
            write(ci->sockfd, text, strlen(text));
        }
    }

    free(ci);
}