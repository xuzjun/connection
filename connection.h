#ifndef _CONNECTION_H_
#define _CONNECTION_H_

/*
 * create a listen socket
 * @param IP, the IP want to be bound, if NULL, bind 0.0.0.0
 * @param port, the port want to be bound
 * @param backlog, defines the maximum length to which the queue of 
 *                 pending connections  for  sockfd  may grow
 * return:
 * 	-1, something error
 * 	others, socket file descriptor
 */
int create_listen_socket(const char *IP, int port, int backlog);

/*
 * create a non-blocking listen socket
 * @param IP, the IP want to be bound, if NULL, bind 0.0.0.0
 * @param PORT, the port want to be bound
 * return:
 * 	-1, something error
 * 	others, socket file descriptor
 */
int create_non_blocking_listen_socket(const char *IP, int port, int backlog);

/*
 * write the length len of a buff to a file descriptor
 * @param fd, the file descriptor to be writen
 * @param buff, the buffer to be writen
 * @param len, the length to be writen
 * return:
 * 	-1, something error
 * 	>0, the length had writen
 */
int writen(int fd, const char *buff, size_t len);

/*
 * read from a fd to buff
 * @param fd, the file descriptor to read
 * @param buff, the buffer to be padding
 * @param len, the length of buff
 * return:
 * 	-1, something error
 * 	>0, the length had read
 */
int readn(int fd, char *buff, size_t len);

/*
 * send file to a sockfd
 * @param sockfd, the socket file descriptor
 * @param path, the path of file
 * return:
 * 	0, success
 * 	-1, something error
 */
int send_file(int sockfd, const char *path);

#endif
