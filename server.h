// macros
#define SERV_BUFF_READ 512
#define SERV_BUFF_WRITE 512

#define SERV_MSG_WELCOME "Welcome!\n"
// .macros

struct client_info
{
	struct sockaddr_in cliaddr;
	int cliaddrlen;
	int sockfd;
	char* cliname;
};

void* serv_initconn(void* args);
void serv_broadcast(int src_sockfd, const char* message);