/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:10 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 03:46:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <deque>
#include <iostream>

namespace ftpp {

class BaseSelector {
public:
  static const unsigned int READ = 1;
  static const unsigned int WRITE = 2;
  static const unsigned int ERROR = 4;
  struct events {
    int fd;
    unsigned int events;
  };
  virtual ~BaseSelector() {};
  virtual void add(int fd, int events) = 0;
  virtual void remove(int fd) = 0;
  virtual void modify(int fd, int events) = 0;
  virtual void select(std::deque<events> &events, int timeout) const = 0;
};

} // namespace ftpp
