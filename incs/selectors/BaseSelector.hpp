/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:20:10 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/09 21:11:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SELECTORS_BASESELECTOR_HPP__
#define __SELECTORS_BASESELECTOR_HPP__

#include <ctime>
#include <iostream>
#include <vector>

class BaseSelector {
 private:
 public:
  static const int READ = 0b01;
  static const int WRITE = 0b10;
  struct events {
    int fd;
    int events;
  };
  virtual ~BaseSelector();
  virtual void add(int fd, int events) = 0;
  virtual void remove(int fd) = 0;
  virtual void modify(int fd, int events) = 0;
  virtual std::vector<events> wait(int timeout) = 0;
};

#endif
