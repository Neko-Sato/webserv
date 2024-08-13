/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:43:46 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 21:30:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __ASYNCIO_HANDLER_HPP__
#define __ASYNCIO_HANDLER_HPP__

class BaseHandler {
private:
public:
  virtual ~BaseHandler() {
  }
  virtual void operator()() = 0;
  virtual BaseHandler *copy() = 0;
};

template <typename A> class FunctionHandler : public BaseHandler {
private:
  typedef void (*F)(A);
  F __f;
  A __a;

public:
  FunctionHandler(F f, A a) : __f(f), __a(a) {
  }
  FunctionHandler(const FunctionHandler &src) : __f(src.__f), __a(src.__a) {
  }
  virtual void operator()() {
    return __f(__a);
  };
  FunctionHandler *copy() {
    return new FunctionHandler(*this);
  }
};

template <typename T, typename A> class MethodHandler : public BaseHandler {
private:
  typedef void (T::*F)(A);
  F __f;
  T &__t;
  A __a;

public:
  MethodHandler(F f, T &t, A a) : __f(f), __t(t), __a(a) {
  }
  MethodHandler(const MethodHandler &src)
      : __f(src.__f), __t(src.__t), __a(src.__a) {
  }

  void operator()() {
    __t.*__f(__t);
  }

  MethodHandler *copy() {
    return new MethodHandler(*this);
  }
};

#endif
