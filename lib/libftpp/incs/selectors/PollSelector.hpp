/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 03:46:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <poll.h>
#include <selectors/BaseSelector.hpp>

namespace ftpp {

class PollSelector : public BaseSelector {
private:
  std::map<int, unsigned int> _fds;

public:
  PollSelector();
  ~PollSelector();
  void add(int fd, int events);
  void remove(int fd);
  void modify(int fd, int events);
  void select(std::deque<events> &events, int timeout) const;
};

} // namespace ftpp