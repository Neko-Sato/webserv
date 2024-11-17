/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:10 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 16:36:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <deque>
#include <iostream>

namespace ftpp {

class BaseSelector {
private:
  BaseSelector(BaseSelector const &rhs);
  BaseSelector &operator=(BaseSelector const &rhs);

public:
  static const unsigned int READ = 1;
  static const unsigned int WRITE = 2;
  static const unsigned int ERROR = 4;
  struct event_detals {
    int fd;
    unsigned int events;
  };

  BaseSelector();
  virtual ~BaseSelector();
  virtual void add(int fd, int events) = 0;
  virtual void remove(int fd) = 0;
  virtual void modify(int fd, int events) = 0;
  typedef std::deque<event_detals> Events;
  virtual void select(Events &events, int timeout) const = 0;

  class RegisteredError : public std::exception {
    const char *what() const throw();
  };

  class NotRegisteredError : public std::exception {
    const char *what() const throw();
  };
};

} // namespace ftpp
