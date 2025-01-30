/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:01:52 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 15:00:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_POINT_H
# define FIXED_POINT_H

# include <stdint.h>
# include <stdbool.h>

# include "extensions.h"

typedef struct
{
  int64_t value;
  uint8_t precision; //number of decimal places
} fixed_point_t;

extern inline fixed_point_t HOT ALWAYS_INLINE fast_add(const fixed_point_t a, const fixed_point_t b);
extern inline fixed_point_t HOT ALWAYS_INLINE fast_sub(const fixed_point_t a, const fixed_point_t b);
extern inline fixed_point_t HOT ALWAYS_INLINE fast_mul(const fixed_point_t a, const fixed_point_t b);
extern inline fixed_point_t HOT ALWAYS_INLINE fast_div(const fixed_point_t a, const fixed_point_t b);

#endif