/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrInfos.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 03:43:17 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/11 00:08:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cstddef>
#include <exception>
#include <netdb.h>

namespace ftpp {

class AddrInfos {
private:
  addrinfo *_addrinfo;

  static addrinfo *_getaddrinfo(char const *name, char const *service,
                                addrinfo const *hints = NULL);

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

  struct Hints : private addrinfo {
  public:
    Hints();
    Hints(int family, int socktype, int protocol, int flags);
    Hints(Hints const &rhs);
    ~Hints();
    Hints &operator=(Hints const &rhs);

    using addrinfo::ai_family;
    using addrinfo::ai_flags;
    using addrinfo::ai_protocol;
    using addrinfo::ai_socktype;

    addrinfo const *get_addrinfo() const;
  };

  class GAIError : public std::exception {
  private:
    int _errno;
    std::string _s;

  public:
    GAIError(int __errno);
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
