/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_primitives.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:31:19 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 09:57:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/network_primitives.h"

# include <stdio.h> //TODO remove

inline int getaddrinfo_a_p(int mode, struct gaicb *list[], int ent, struct sigevent *sig)
{
  int ret = getaddrinfo_a(mode, list, ent, sig);
  if (ret != 0)
  {
    printf("DEB GETADDRINFO_A ERROR\n");
    panic(gai_strerror(ret));
  }
  return ret;
}

inline int connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret = connect(sockfd, addr, addrlen);
  if (ret == -1 && errno != EAGAIN && errno != EINPROGRESS)
  {
    printf("DEB CONNECT ERROR\n");
    panic(strerror(errno));
  }

  return ret;
}