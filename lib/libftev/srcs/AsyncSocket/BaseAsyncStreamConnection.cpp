/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncStreamConnection.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:43:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 10:25:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseAsyncStreamConnection.hpp>

namespace ftev {

BaseAsyncStreamConnection::BaseAsyncStreamConnection() {
  start(_socket.get_sockfd(), ftpp::BaseSelector::READ);
}

BaseAsyncStreamConnection::~BaseAsyncStreamConnection() {
}

} // namespace ftev
