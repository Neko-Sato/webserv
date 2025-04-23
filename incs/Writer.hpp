/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Writer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/23 23:58:52 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

#include <deque>
#include <vector>

class Writer : private ftpp::NonCopyable {
public:
  Writer();
  virtual ~Writer();

  virtual void write(ftev::StreamConnectionTransport &transport,
                     char const *data, std::size_t size, bool more) = 0;
};

class ContentLengthWriter : public Writer {
private:
  std::size_t _size;

public:
  ContentLengthWriter(std::size_t size = 0);
  ~ContentLengthWriter();

  void write(ftev::StreamConnectionTransport &transport, char const *data,
             std::size_t size, bool more);
};

class ChankedWriter : public Writer {
private:
  bool _end;

public:
  ChankedWriter();
  ~ChankedWriter();

  void write(ftev::StreamConnectionTransport &transport, char const *data,
             std::size_t size, bool more);
};
