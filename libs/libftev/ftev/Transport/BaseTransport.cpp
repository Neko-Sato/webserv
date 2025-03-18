/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTransport.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 19:01:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Transport/BaseTransport.hpp>

namespace ftev {

BaseTransport::Handler::Handler(EventLoop &loop, BaseTransport &transport)
    : BaseIOWatcher(loop), _transport(transport) {
}

BaseTransport::Handler::~Handler() {
}

void BaseTransport::Handler::on_read() {
  _transport.on_read();
}

void BaseTransport::Handler::on_write() {
  _transport.on_write();
}

void BaseTransport::Handler::on_except() {
  _transport.on_except();
}

BaseTransport::BaseTransport(EventLoop &loop) : _handler(loop, *this) {
}

BaseTransport::~BaseTransport() {
}

void BaseTransport::on_read() {
}

void BaseTransport::on_write() {
}

void BaseTransport::on_except() {
}

} // namespace ftev
