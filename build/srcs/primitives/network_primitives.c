/*================================================================================

File: network_primitives.c                                                      
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

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