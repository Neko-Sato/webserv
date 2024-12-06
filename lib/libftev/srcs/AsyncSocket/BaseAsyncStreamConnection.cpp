/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncStreamConnection.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:43:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 07:54:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseAsyncStreamConnection.hpp>

namespace ftev {

BaseAsyncStreamConnection::BaseAsyncStreamConnection() {
  start(_socket.get_sockfd(), ftpp::BaseSelector::READ);
}

BaseAsyncStreamConnection::~BaseAsyncStreamConnection() {
}

void BaseAsyncStreamConnection::close() {
  if (is_active())
    stop();
  delete_later();
}

void BaseAsyncStreamConnection::on_release() {
  delete this;
}

} // namespace ftev
