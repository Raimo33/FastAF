/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 21:27:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <sys/uio.h>

# define LOG_RING_SIZE 4096U
# define LOG_FILENO    7U

# define STR_LEN_PAIR(s) s, sizeof(s) - 1
# define STR_LEN(s) sizeof(s) - 1

typedef struct
{
  char     data[LOG_RING_SIZE] __attribute__((aligned(16)));
  uint16_t head;
  uint16_t tail;
} t_log_ring;

void  init_logger(void);
void  log(const char *msg, const uint8_t msg_len);
void  flush_logs(void);
void  free_logger(void);

#endif