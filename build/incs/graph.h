/*================================================================================

File: graph.h                                                                   
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

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