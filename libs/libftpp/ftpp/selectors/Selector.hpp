/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:58:44 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:04:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <ctime>
#include <exception>
#include <map>
#include <queue>

namespace ftpp {

class Selector : private NonCopyable {
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

private:
  Mapping _fds;

protected:
  Selector();

public:
  virtual ~Selector();

  virtual void add(int fd, event_t events);
  virtual void remove(int fd);
  virtual void modify(int fd, event_t events);

  Mapping const &getMap() const;

  virtual void select(Events &events, int timeout) const = 0;

  class RegisteredError : public std::exception {
    char const *what() const throw();
  };

  class NotRegisteredError : public std::exception {
    char const *what() const throw();
  };
};

} // namespace ftpp
