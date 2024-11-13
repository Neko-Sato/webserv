/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/13 21:56:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>

class EpollSelector : public BaseSelector {
private:
  int _epfd;

public:
  static const int max_events = 1024;
  EpollSelector();
  ~EpollSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void wait(std::deque<events> &events, int timeout) const;
};
