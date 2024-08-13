/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Future.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 23:47:05 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 20:59:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __ASYNCIO_FUTURE_HPP__
#define __ASYNCIO_FUTURE_HPP__

#include <exception>

template <typename T> class Future {
private:
  T __result;
  std::exception __exception;
  void __set_result(T &result);
  void __set_exception(std::exception &exception);

public:
  T result();
  bool done();
  bool cancelled();
  void cancel();
  void exception(std::exception &exception);
};

#endif
