/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:41:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 00:44:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

class Task : private ftpp::NonCopyable {
private:
  ftev::StreamConnectionTransport &_transport;
  ftev::EventLoop::DeferWatcher &_complete;

  Task();

public:
  Task(ftev::StreamConnectionTransport &transport,
       ftev::EventLoop::DeferWatcher &complete);
  virtual ~Task();

  ftev::StreamConnectionTransport &getTransport() const;

  void complete();

  virtual void onData(std::vector<char> const &data) = 0;
  virtual void onEof() = 0;
};
