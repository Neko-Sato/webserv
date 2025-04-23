/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Future.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:48:37 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/21 09:38:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop/DeferWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <exception>

namespace ftev {

template <typename T> class Future : public NonCopyable {
private:
  class Handler : public EventLoop::DeferWatcher {
  private:
    Future<T> &_future;

  public:
    Handler(EventLoop &loop, Future<T> &future);
    ~Handler();

    void onEvent();
  };

  Handler *_handler;
  T *_result;
  std::exception *_exec;

  Future();

public:
  Future(EventLoop &loop);
  virtual ~Future();

  void setResult(T const &result);
  template <typename E> void setError(E const &exec);

  virtual void onDone(T const &result) = 0;
  virtual void onError(std::exception const &exec) = 0;
};

template <typename T>
Future<T>::Future(EventLoop &loop)
    : _handler(NULL), _result(NULL), _exec(NULL) {
  _handler = new Handler(loop, *this);
}

template <typename T> Future<T>::~Future() {
  delete _result;
  delete _exec;
  delete _handler;
}

template <typename T> void Future<T>::setResult(T const &data) {
  if (_result || _exec)
    throw std::runtime_error("already set");
  _result = new T(data);
  try {
    _handler->start();
  } catch (std::exception &e) {
    delete _result;
    _result = NULL;
    throw;
  }
}

template <typename T>
template <typename E>
void Future<T>::setError(E const &exec) {
  if (_result || _exec)
    throw std::runtime_error("already set");
  _exec = new E(exec);
  try {
    _handler->start();
  } catch (std::exception &e) {
    delete _exec;
    _exec = NULL;
    throw;
  }
}

template <typename T>
Future<T>::Handler::Handler(EventLoop &loop, Future<T> &future)
    : DeferWatcher(loop), _future(future) {
}

template <typename T> Future<T>::Handler::~Handler() {
  if (getIsActive())
    cancel();
}

template <typename T> void Future<T>::Handler::onEvent() {
  if (_future->result)
    _future->onDone(*_future->result);
  else if (_future->exec)
    _future->onError(*_future->exec);
}

} // namespace ftev
