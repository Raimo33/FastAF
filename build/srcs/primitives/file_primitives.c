/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_primitives.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:00:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 09:54:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/file_primitives.h"

# include <stdio.h> //TODO remove

inline int dup_p(int oldfd)
{
  const int newfd = dup(oldfd);
  if (newfd == -1)
  {
    printf("DEB DUP ERROR\n");
    panic(strerror(errno));
  }
  return newfd;
}

inline int fcntl_p(int fd, int cmd, int arg)
{
  const int ret = fcntl(fd, cmd, arg);
  if (ret == -1)
  {
    printf("DEB FCNTL ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int socket_p(int domain, int type, int protocol)
{
  const int fd = socket(domain, type, protocol);
  if (fd == -1)
  {
    printf("DEB SOCKET ERROR\n");
    panic(strerror(errno));
  }
  return fd;
}

inline int setsockopt_p(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
  if (setsockopt(fd, level, optname, optval, optlen) == -1)
  {
    printf("DEB SETSOCKOPT ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int epoll_create1_p(int flags)
{
  const int fd = epoll_create1(flags);
  if (fd == -1)
  {
    printf("DEB EPOLL_CREATE1 ERROR\n");
    panic(strerror(errno));
  }
  return fd;
}

inline int epoll_ctl_p(int epfd, int op, int fd, struct epoll_event *event)
{
  if (epoll_ctl(epfd, op, fd, event) == -1)
  {
    printf("DEB EPOLL_CTL ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int epoll_wait_p(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
  const int ret = epoll_wait(epfd, events, maxevents, timeout);
  if (ret == -1)
  {
    printf("DEB EPOLL_WAIT ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int writev_p(int fd, const struct iovec *iov, int iovcnt)
{
  const int ret = writev(fd, iov, iovcnt);
  if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
  {
    printf("DEB WRITEV ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int read_p(int fd, void *buf, size_t count)
{
  const int ret = read(fd, buf, count);
  if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
  {
    printf("DEB READ ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline FILE *fdopen_p(const int fd, const char *mode)
{
  FILE *restrict ret = fdopen(fd, mode);
  if (ret == NULL)
  {
    printf("DEB FDOPEN ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}