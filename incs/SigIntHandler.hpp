/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SigIntHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 10:03:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>

class SigIntHandler : public ftev::EventLoop::BaseSignalWatcher {
public:
  SigIntHandler(ftev::EventLoop &loop);
  ~SigIntHandler();

  void on_signal();
};
