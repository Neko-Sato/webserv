/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/30 01:46:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#if defined(__linux__)

#include <ftpp/selectors/Selector.hpp>

namespace ftpp {

class EpollSelector : public Selector {
public:
  static int const max_events;

private:
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

#endif
