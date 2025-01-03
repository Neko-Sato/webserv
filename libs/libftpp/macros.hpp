/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 21:22:42 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 21:33:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define UNUSED(x) (void)(x)

#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define add_overflow(a, b, res) __builtin_add_overflow(a, b, res)
#define sub_overflow(a, b, res) __builtin_sub_overflow(a, b, res)
#define mul_overflow(a, b, res) __builtin_mul_overflow(a, b, res)
#else

#define likely(x) (x)
#define unlikely(x) (x)

template <typename T> bool __add_overflow(T a, T b, T *result) {
  *result = a + b;
  return *result < a;
}

template <typename T> bool __sub_overflow(T a, T b, T *result) {
  *result = a - b;
  return *result > a;
}

template <typename T> bool __mul_overflow(T a, T b, T *result) {
  *result = a * b;
  return a != 0 && *result / a != b;
}

#define add_overflow(a, b, result) __add_overflow(a, b, result)
#define sub_overflow(a, b, result) __sub_overflow(a, b, result)
#define mul_overflow(a, b, result) __mul_overflow(a, b, result)

#endif
