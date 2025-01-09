/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:47:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 18:19:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdint.h>

# define REST_ENDPOINT "api3.binance.com:443"
# define WS_ENDPOINT "data-stream.binance.vision:9443"
# define FIX_ENDPOINT "fix-oe.binance.com:9000"
# define PAIRS = { "usdcust", "fdusdusdt", "usdtdai", "usdpusdt", "fdusdusdc", "eureuri"}
# define LOGGING_INTERVAL 60000U
# define MAX_MARKET_ORDER_DELAY 50U
# define MAX_LIMIT_ORDER_DELAY 1000U

//TODO capire come fare per rendere config const e anche attributi

typedef struct
{
  char    *api_key;
  uint8_t log_level : 2;
} t_config;

extern t_config g_config;

void init_config(void);

#endif