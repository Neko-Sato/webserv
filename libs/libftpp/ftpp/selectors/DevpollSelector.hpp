/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DevpollSelector.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/30 01:46:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#if defined(__sun__)

#include <ftpp/selectors/Selector.hpp>

namespace ftpp {

class DevpollSelector : public Selector {
private:
  int _fd;

  static int _create_devpollfd();

public:
  DevpollSelector();
  ~DevpollSelector();

  void add(int fd, event_t events);
  void remove(int fd);
  void modify(int fd, event_t events);
  void select(Events &events, int timeout) const;
};

} // namespace ftpp

#endif
