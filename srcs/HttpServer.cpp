/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:33:32 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 09:05:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(ftev::EventLoop &loop, Configs const &configs)
    : _loop(loop), _configs(configs) {
/*
	gconfigsをもとにgetaddrinfoを使って
	複数の非同期ソケットを生成する
*/
}

HttpServer::~HttpServer() {
/*
	生成したソケットを破棄する
*/
}

void HttpServer::start() {
  /*
  複数の非同期ソケットをlistenする
  */
}
