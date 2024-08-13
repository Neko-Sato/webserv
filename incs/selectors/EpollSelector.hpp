/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 21:50:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SELECTORS_EPOLLSELECTOR_HPP__
#define __SELECTORS_EPOLLSELECTOR_HPP__

#include <sys/epoll.h>
#include <unistd.h>

#include <map>

#include "./BaseSelector.hpp"

class EpollSelector : public BaseSelector {
private:
  int __fd;
  int __count;

public:
  EpollSelector();
  ~EpollSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void wait(std::vector<events> &events, int timeout);
};

#endif
