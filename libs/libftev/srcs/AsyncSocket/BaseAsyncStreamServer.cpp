/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncStreamServer.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:43:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 10:50:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseAsyncStreamServer.hpp>

namespace ftev {

BaseAsyncStreamServer::BaseAsyncStreamServer() {
  int opt = 1;
  _socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

BaseAsyncStreamServer::~BaseAsyncStreamServer() {
}

void BaseAsyncStreamServer::bind(sockaddr const *addr, socklen_t addrlen) {
  _socket.bind(addr, addrlen);
}

void BaseAsyncStreamServer::listen(int backlog) {
  _socket.listen(backlog);
  start(_socket.get_sockfd(), ftpp::BaseSelector::READ);
}

void BaseAsyncStreamServer::on_read() {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int sockfd;
  try {
    sockfd = _socket.accept(reinterpret_cast<sockaddr *>(&addr), &addrlen);
  } catch (...) {
    return;
  }
  on_accept(sockfd, reinterpret_cast<sockaddr *>(&addr));
}

void BaseAsyncStreamServer::on_write() {
}

} // namespace ftev
