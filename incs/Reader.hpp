/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/10 04:03:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <deque>
#include <vector>

class Reader : private ftpp::NonCopyable {
public:
  Reader();
  virtual ~Reader();

  virtual bool completed() const = 0;
  virtual void read(std::deque<char> &buffer, std::vector<char> &tmp) = 0;
};

class ContentLengthReader : public Reader {
private:
  std::size_t _size;

public:
  ContentLengthReader(std::size_t size = 0);
  ~ContentLengthReader();

  void read(std::deque<char> &buffer, std::vector<char> &res);
  bool completed() const;
};

class ChankedReader : public Reader {
private:
  enum State { READ_SIZE, READ_DATA, READ_CRLF, END };
  State _state;
  std::size_t _pos;
  std::size_t _size;

public:
  ChankedReader();
  ~ChankedReader();

  void read(std::deque<char> &buffer, std::vector<char> &res);
  bool completed() const;
};
