/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalPipe.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:02:58 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 10:05:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop/SignalPipe.hpp>
#include <cassert>
#include <exceptions/OSError.hpp>

#include <unistd.h>

namespace ftev {

EventLoop::SignalPipe::SignalPipe() {
  if (__glibc_unlikely(pipe(_pipe) == -1))
    throw ftpp::OSError(errno, "pipe");
}

EventLoop::SignalPipe::~SignalPipe() {
  close(_pipe[0]), close(_pipe[1]);
}

int EventLoop::SignalPipe::get_in() const {
  return _pipe[0];
}

int EventLoop::SignalPipe::get_out() const {
  return _pipe[1];
}

} // namespace ftev
