/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_primitives.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:02:51 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:25:09 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_PRIMITIVES_H
# define FILE_PRIMITIVES_H

# include <stdint.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int dup_p(int oldfd);
COLD extern inline int fcntl_p(int fd, int cmd, int arg);
COLD extern inline int socket_p(int domain, int type, int protocol);
COLD extern inline int setsockopt_p(int fd, int level, int optname, const void *optval, socklen_t optlen);
COLD extern inline int epoll_create1_p(int flags);
COLD extern inline int epoll_ctl_p(int epfd, int op, int fd, struct epoll_event *event);
HOT extern inline int epoll_wait_p(int epfd, struct epoll_event *events, int maxevents, int timeout);
HOT extern inline int writev_p(int fd, const struct iovec *iov, int iovcnt);
HOT extern inline int read_p(int fd, void *buf, size_t count);
HOT extern inline FILE *fdopen_p(int fd, const char *mode);

#endif