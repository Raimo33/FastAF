/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:19:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 16:07:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

# include <stdint.h>
# include <stdlib.h>

# include "primitives/alloc_primitives.h"
# include "fixed_point.h"
# include "extensions.h"
# include "errors.h"

//https://developers.binance.com/docs/binance-spot-api-docs/filters

//TODO compressed adjacency list

COLD void init_graph(graph_t *restrict graph);
COLD void free_graph(graph_t *restrict graph);


#endif