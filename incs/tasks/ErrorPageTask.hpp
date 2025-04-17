/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageTask.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:45:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:56:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Task.hpp"
#include "configs/ServerConf.hpp"

#include <ftev/Stream/StreamConnection.hpp>

class ErrorPageTask : public Task {
private:
  ServerConf::ErrorPages const &_errorPages;
  int _statusCode;

public:
  ErrorPageTask(ftev::StreamConnectionTransport &transport,
                ftev::EventLoop::DeferWatcher &complete,
                Request const &request,
                ServerConf::ErrorPages const &errorPages, int statusCode);
  ~ErrorPageTask();

  void onData(std::vector<char> const &data);
  void onEof();
  void onCancel();
};
