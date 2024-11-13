/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/13 21:56:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>
#include <poll.h>
#include <map>

class PollSelector : public BaseSelector {
private:
  std::map<int, unsigned int> _fds;

public:
  PollSelector();
  ~PollSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void wait(std::deque<events> &events, int timeout) const;
};
