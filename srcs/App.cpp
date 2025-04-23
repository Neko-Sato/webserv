/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:35:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"

App::App(Connection::Cycle &cycle) : cycle(cycle) {
}

App::~App() {
}

void App::onData(std::vector<char> const &) {
}

void App::onEof() {
  Response::Headers headers;
  headers["transfer-encoding"].push_back("chunked");
  cycle.send(200, headers);
  cycle.send("Hello", 5, false);
}

void App::onCancel() {
}
