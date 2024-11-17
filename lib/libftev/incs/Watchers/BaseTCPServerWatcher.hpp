/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServerWatcher.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 20:56:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 21:12:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseIOWatcher.hpp>
#include <selectors/Selector.hpp>

namespace ftev {

class BaseTCPServerWatcher : public EventLoop::BaseIOWatcher {
private:
  int _fd;
  void start(int fd, int events);

public:
  BaseTCPServerWatcher();
  ~BaseTCPServerWatcher();

  void start();
};

} // namespace ftev
