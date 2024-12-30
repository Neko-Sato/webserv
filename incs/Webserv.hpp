/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:17:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configs.hpp"

#include <EventLoop.hpp>

#ifndef DEFAULT_CONFIGURE
#define DEFAULT_CONFIGURE "./default.json"
#endif

/*
	configのserversを読み込む
	addressの情報を重複なしでかき集めて、全てのサーバを建てる
	serverのcallbackどもはconfigの情報を参照する
	headerの情報まで読んでから
	locationの情報を参照するが
	ConfigsにHostを渡したら適切なServerを返すようにする
	Serverにpathを渡したら適切なLocationを返すようにする
　　loctionが生成したtaskを処理することによってresponseが完了する
*/

class Webserv {
private:
  ftev::EventLoop &_loop;
  Configs _configs;

  Webserv();
  Webserv(Webserv const &);
  Webserv &operator=(Webserv const &);

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};
