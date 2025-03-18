/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueSelector.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:23:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/selectors/BaseSelector.hpp>

#if defined(__APPLE__) || defined(__FreeBSD__)

namespace ftpp {

class KqueueSelector : public BaseSelector {
public:
  static int const max_events;

private:
  using BaseSelector::_fds;
  int _kq;

  static int _create_kqueue();

public:
  KqueueSelector();
  ~KqueueSelector();

  void add(int fd, event_t events);
  void remove(int fd);
  void modify(int fd, event_t events);
  void select(Events &events, int timeout) const;
};

} // namespace ftpp

#endif
