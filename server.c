#include "server.h"

vec *vec_cli_infos;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        errexit("No port number provided\n");
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
        errexit("Binding error\n");
    }

    vec_cli_infos = vector(64);

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

    printf("%d is connected\n", cli_info->sockfd);

    char readbuffer[SERV_BUFF_READ], writebuffer[SERV_BUFF_WRITE];
    bzero(readbuffer, sizeof(readbuffer));
    bzero(writebuffer, sizeof(writebuffer));

    sprintf(writebuffer, "%s\n%s: ", SERV_MSG_WELCOME, SERV_MSC_USERNAME);
    write(cli_info->sockfd, writebuffer, strlen(writebuffer));

    int readlen, firstread = 1;
    while ((readlen = read(cli_info->sockfd, &readbuffer, SERV_BUFF_READ)))
    {
        if (readlen > 1)
        {
            if (firstread)
            {
                strncpy(cli_info->cliname, readbuffer, readlen - 1);
                printf("%d -> %s\n", cli_info->sockfd, cli_info->cliname);

                sprintf(writebuffer, "%s is now online\n", cli_info->cliname);
                cli_broadcast(NULL, writebuffer);
                vector_push(vec_cli_infos, cli_info);

                firstread = !firstread;
            } 
            else 
            {
                printf("%s > %s", cli_info->cliname, readbuffer);
                cli_broadcast(cli_info, readbuffer);
            }
            
            bzero(readbuffer, sizeof(readbuffer));
        }
    }

    printf("%d is disconnected\n", cli_info->sockfd);

    if (vector_remove(vec_cli_infos, cli_info))
    {
        bzero(writebuffer, sizeof(writebuffer));
        sprintf(writebuffer, "%s is disconnected.\n", cli_info->cliname);
        cli_broadcast(NULL, writebuffer);
    }

    close(cli_info->sockfd);
}

void cli_broadcast(struct client_info* cli_info, const char* message)
{
    char text[SERV_BUFF_WRITE];
    bzero(text, sizeof(text));

#ifdef __unix__
    if (cli_info)
        sprintf(text, KYEL "%s says:" KNRM " %s", cli_info->cliname, message);
    else
        sprintf(text, KBLU "ADMIN: %s" KNRM, message);
#else
    if (cli_info)
        sprintf(text, "%s says: %s", cli_info->cliname, message);
    else
        sprintf(text, "ADMIN: %s", message);
#endif

    struct client_info* ci;
    int i, count = vec_cli_infos->size;

    for (i = 0; i < count; i++)
    {
        ci = (struct client_info*)vector_get(vec_cli_infos, i);

        if (!cli_info || ci->sockfd != cli_info->sockfd)
        {
            write(ci->sockfd, text, strlen(text));
        }
    }
}