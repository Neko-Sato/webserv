/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:48:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftpp {

// This class is intended to make resource movement explicit.
// It cannot be materialized.
// Constructors and assignments must use swap to move resources.

template <class T> class rv : public T, private NonCopyable {
private:
  rv();
  ~rv();

public:
};

template <typename T> rv<T>::~rv() {
}

template <typename T> rv<T> &transfer(T &t) {
  // This is a dangerous cast, though.
  return static_cast<rv<T> &>(t);
}

} // namespace ftpp

/*
// Example...

#include <algorithm>

struct Primitive {
  Primitive() {};
};

struct SwapAble : Primitive {
  SwapAble() {};
  void swap(SwapAble &rhs) {};
};

struct MoveAble : SwapAble {
  MoveAble() {};
  MoveAble(ftpp::rv<MoveAble> &rhs) {};
  MoveAble &operator=(ftpp::rv<MoveAble> &rhs) {
    return *this;
  };
};

class Object {
private:
  Primitive _primitive;
  SwapAble _swapAble;
  MoveAble _moveAble;

public:
  // default constructor
  Object() : _primitive(), _swapAble(), _moveAble() {};
  // constructor with parameters
  Object(Primitive const &primitive, SwapAble const &swapAble,
         MoveAble const &moveAble)
      : _primitive(primitive), _swapAble(swapAble), _moveAble(moveAble) {};
  // copy constructor
  Object(Object const &rhs)
      : _primitive(rhs._primitive), _swapAble(rhs._swapAble),
        _moveAble(rhs._moveAble) {};
  // move constructor
  Object(ftpp::rv<Object> &rhs)
      : _primitive(rhs._primitive), _swapAble(),
        _moveAble(ftpp::transfer(rhs._moveAble)) {
    _swapAble.swap(rhs._swapAble);
  };
  // destructor
  ~Object() {};
  // copy assignment operator
  Object &operator=(Object const &rhs) {
    if (this != &rhs)
      Object(rhs).swap(*this);
    return *this;
  };
  // move assignment operator
  Object &operator=(ftpp::rv<Object> &rhs) {
    if (this != &rhs)
      rhs.swap(*this);
    return *this;
  };
  // swap function
  void swap(Object &rhs) {
    std::swap(_primitive, rhs._primitive);
    _swapAble.swap(rhs._swapAble);
    _moveAble.swap(rhs._moveAble);
  };
};

*/
