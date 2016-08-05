#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "conn.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define IP        "0.0.0.0"
#define PORT      9999
#define MAXEVENTS 1024

int create_listen_socket(const char *ip, int port, int backlog)
{
	int sock;
	sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (-1 == sock)
		return -1;

	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return -1;
 
	struct sockaddr_in serv_addr;
	int addr_len;
	serv_addr.sin_family = AF_INET;
	if (NULL == ip)
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);
	serv_addr.sin_family = AF_INET;
	addr_len = sizeof(struct sockaddr_in);
	if (bind(sock, (struct sockaddr *)&serv_addr, addr_len) < 0)
		return -1;

	if (listen(sock, backlog) < 0)
		return -1;
	return sock;
}

int main()
{
	int listenfd = create_listen_socket(IP, PORT, 5);
	if (listenfd < 0) {
		printf("create listen socket error");
		return -1;
	}

	int fd;
	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd < 0) {
		printf("create epoll fd error");	
		exit(-1);
	}
	struct epoll_event *events = calloc(MAXEVENTS, sizeof(struct epoll_event));
	struct epoll_event ev;
	memset(&ev, 0x00, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
		printf("add listenfd error\n");
		exit(-1);
	}

	int i;
	struct sockaddr_in sa;
	socklen_t sl = sizeof(sa);
	char buf[1024] = {0};
	while (1) {
		int n = epoll_wait(epoll_fd, events, MAXEVENTS, -1); 
		if (n == -1) {
			perror("epoll wait return error: ");
			exit(-1);
		}
		printf("epoll get event[%d]\n", n);
		for (i = 0; i < n; ++i) {
			if (events[i].data.fd == listenfd) {
				while (1) {
					memset(&sa, 0x00, sizeof(sa));
					fd = accept(events[i].data.fd, (struct sockaddr *)&sa, (socklen_t *)&sl);
					if (fd != -1) {
						printf("accept from [%s] -> fd[%d]\n", inet_ntoa(sa.sin_addr), fd);
						ev.events = EPOLLIN | EPOLLRDHUP;
						ev.data.fd = fd;
						if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {
							printf("add fd[%d] to epoll err\n", fd);
						}
					} else {
						printf("accept error");	
						break;
					}
				}
			}

			if (events[i].events & EPOLLIN) {
				memset(buf, 0x00, sizeof(buf));
				int l = read(events[i].data.fd, buf, 1);
				if (l < 0) {
					printf("read error\n");
					continue;
				}
				printf("recv buf[%s] len[%d]\n", buf, l);
				l = write(events[i].data.fd, buf, l);
				printf("write buf[%s] len[%d]\n", buf, l);
			}
			if (events[i].events & EPOLLRDHUP) {
				printf("fd [%d] disconnect\n", events[i].data.fd);
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
			}
		}
	}
}
