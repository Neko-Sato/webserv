/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:10 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/25 07:07:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <NonCopyable.hpp>

#include <ctime>
#include <exception>
#include <map>
#include <queue>

namespace ftpp {

class BaseSelector : private NonCopyable {
public:
  typedef unsigned int event_t;
  typedef std::map<int, event_t> Mapping;
  struct event_details {
    int fd;
    event_t events;
  };
  typedef std::queue<event_details> Events;

  static event_t const READ;
  static event_t const WRITE;
  static event_t const EXCEPT;

protected:
  Mapping _fds;
  BaseSelector();

public:
  virtual ~BaseSelector();

  virtual void add(int fd, event_t events);
  virtual void remove(int fd);
  virtual void modify(int fd, event_t events);

  Mapping const &get_map() const;

  virtual void select(Events &events, int timeout) const = 0;

  class RegisteredError : public std::exception {
    char const *what() const throw();
  };

  class NotRegisteredError : public std::exception {
    char const *what() const throw();
  };
};

} // namespace ftpp
