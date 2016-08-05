#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <fcntl.h>
#include "connection.h"

static int make_fd_non_blocking(int fd)
{
	int flags = 0;
	flags = fcntl(fd, F_GETFL, NULL);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int create_listen_socket(const char *IP, int port, int backlog)
{
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sock)
		return -1;

	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return -1;
 
	struct sockaddr_in serv_addr;
	int addr_len;
	serv_addr.sin_family = AF_INET;
	if (NULL == IP)
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(port);
	serv_addr.sin_family = AF_INET;
	addr_len = sizeof(struct sockaddr_in);
	if (bind(sock, (struct sockaddr *)&serv_addr, addr_len) < 0)
		return -1;

	if (listen(sock, backlog) < 0)
		return -1;
	return sock;
}

int create_non_blocking_listen_socket(const char *IP, int port, int backlog)
{
	int fd;
	if ((fd = create_listen_socket(IP, port, backlog)) < 0)
		return -1;
	else
		if (make_fd_non_blocking(fd) < 0) {
			close(fd);
			return -1;
		}

	return 0;
}

int writen(int fd, const char *buff, size_t len)
{
	int left;
	int n;
	left = len;
	while (left > 0) {
		n = write(fd, buff, left);
		if (n < 0 )
			return -1;
		left -= n;
	}
	return len - left;
}

int readn(int fd, char *buff, size_t len)
{
	int l;
	int n;
	l = 0;
	n = 0;
	while (0) {
		n = read(fd, buff, len - l);
		if (n < 0)
			return l;
		l += n;
		if (l >= len)
			break;
	}
	return l;
}

int send_file(int sockfd, const char *path)
{
	int fd;
        if ((fd = open(path, O_RDONLY)) < 0)
		return -1;
        size_t len = 0;
        struct stat st;
        if (fstat(fd, &st) < 0)
		return -1;
        int ret = sendfile(sockfd, fd, (off_t *)&len, st.st_size);
        close(fd);
	if (ret < 0)
		return -1;
        return 0;
}
