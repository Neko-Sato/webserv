/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrInfos.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 03:43:17 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <cstddef>
#include <exception>
#include <netdb.h>
#include <string>

namespace ftpp {

class AddrInfos : private NonCopyable {
private:
  addrinfo *_addrinfo;

  static addrinfo *_getaddrinfo(char const *name, char const *service,
                                addrinfo const *hints = NULL);
  AddrInfos();

public:
  class iterator {
  private:
    addrinfo const *_current;

  public:
    iterator(addrinfo const *info = NULL);
    iterator(iterator const &rhs);
    ~iterator();

    iterator &operator=(iterator const &rhs);
    iterator &operator++();
    iterator operator++(int);
    bool operator==(iterator const &rhs) const;
    bool operator!=(iterator const &rhs) const;
    addrinfo const &operator*() const;
    addrinfo const *operator->() const;
  };

  class Hints : private addrinfo {
  public:
    Hints();
    Hints(int family, int socktype, int protocol, int flags);
    Hints(Hints const &rhs);
    ~Hints();
    Hints &operator=(Hints const &rhs);

    addrinfo const *get_addrinfo() const;
  };

  class GAIError : public std::exception {
  private:
    int _errno;
    std::string _s;

  public:
    GAIError(int __errno = 0);
    GAIError(GAIError const &rhs);
    ~GAIError() throw();
    GAIError &operator=(GAIError const &rhs);

    int get_errno() const;
    char const *what() const throw();
  };

  AddrInfos(char const *name, char const *service);
  AddrInfos(char const *name, char const *service, Hints const &hints);
  ~AddrInfos();

  iterator begin() const;
  iterator end() const;
};

} // namespace ftpp
