/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTransport.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/19 01:56:22 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Transport/BaseTransport.hpp>

namespace ftev {

BaseTransport::BaseTransport(EventLoop &loop) : EventLoop::BaseIOWatcher(loop) {
}

BaseTransport::~BaseTransport() {
}

EventLoop::BaseIOWatcher &BaseTransport::_getWatcher() {
  return *this;
}

void BaseTransport::on_except() {
  getProtocol().on_except();
}

} // namespace ftev
