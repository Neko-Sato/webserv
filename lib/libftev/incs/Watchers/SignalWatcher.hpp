/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 02:35:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseSignalWatcher.hpp>

namespace ftev {

template <typename T>
class SignalWatcher : public EventLoop::BaseSignalWatcher {
public:
  typedef void (*callback)(SignalWatcher &watcher, T data);

private:
  callback _on_signal;
  T _data;

public:
  SignalWatcher(EventLoop &loop, callback on_signal, T data)
      : BaseSignalWatcher(loop), _on_signal(on_signal), _data(data) {
  }

  void on_signal() {
    _on_signal(*this, _data);
  }
};

} // namespace ftev
