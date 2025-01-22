/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 06:18:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _stopper(loop) {
}

Webserv::~Webserv() {
}
