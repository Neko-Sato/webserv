/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Writer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:48:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 02:34:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Writer.hpp"
#include "constants.hpp"

#include <sstream>
#include <stdexcept>

Writer::Writer() {
}

Writer::~Writer() {
}

ContentLengthWriter::ContentLengthWriter(std::size_t size) : _size(size) {
}

ContentLengthWriter::~ContentLengthWriter() {
}

void ContentLengthWriter::write(ftev::StreamConnectionTransport &transport,
                                char const *data, std::size_t size, bool more) {
  if (size > _size || (!more && size != _size))
    throw std::runtime_error("invalid content-length");
  transport.write(data, size);
  _size -= size;
}

ChankedWriter::ChankedWriter() : _end(false) {
}

ChankedWriter::~ChankedWriter() {
}

void ChankedWriter::write(ftev::StreamConnectionTransport &transport,
                          char const *data, std::size_t size, bool more) {
  if (_end)
    throw std::runtime_error("invalid chunked transfer encoding");
  if (size != 0) {
    std::ostringstream oss;
    oss << std::hex << size;
    transport.write(oss.str().c_str(), oss.str().size());
    transport.write(CRLF.c_str(), CRLF.size());
    transport.write(data, size);
    transport.write(CRLF.c_str(), CRLF.size());
  }
  if (!more) {
    _end = true;
    transport.write("0", 1);
    transport.write(DOUBLE_CRLF.c_str(), DOUBLE_CRLF.size());
  }
}
