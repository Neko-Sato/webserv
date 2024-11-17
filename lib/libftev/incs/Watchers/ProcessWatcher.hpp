/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 19:16:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseProcessWatcher.hpp>

namespace ftev {

template <typename T>
class ProcessWatcher : public EventLoop::BaseProcessWatcher {
public:
  typedef void (*callback)(BaseProcessWatcher &watcher, int status, T data);

private:
  callback _on_exit;
  T _data;

public:
  ProcessWatcher(EventLoop &loop, callback on_exit, T data)
      : BaseProcessWatcher(loop), _on_exit(on_exit), _data(data) {
  }

  void on_exit(int status) {
    _on_exit(*this, status, _data);
  };
};

} // namespace ftev
