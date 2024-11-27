/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/28 01:58:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>

namespace ftpp {

class EpollSelector : public BaseSelector {
public:
  static int const max_events;

private:
  using BaseSelector::_fds;
  int _epfd;

  static int _create_epollfd();

public:
  EpollSelector();
  ~EpollSelector();

  void add(int fd, event_t events);
  void remove(int fd);
  void modify(int fd, event_t events);
  void select(Events &events, int timeout) const;
};

} // namespace ftpp
