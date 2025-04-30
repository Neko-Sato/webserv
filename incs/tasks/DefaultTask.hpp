/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/30 14:33:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationDefault.hpp"
#include "tasks/Task.hpp"

#include <ftev/EventLoop/ProcessWatcher.hpp>
#include <ftev/Pipe/ReadPipe.hpp>
#include <ftev/Pipe/WritePipe.hpp>

class DefaultTask : public Task {
// public:
//   class CgiProcess : public ftev::EventLoop::ProcessWatcher {
//   public:
//     class CgiReadPipe : public ftev::ReadPipeProtocol {
//     private:
//       CgiProcess &_process;
//       ftev::ReadPipeTransport *_readPipe;

//     public:
//       CgiReadPipe(CgiProcess &process, int fd);
//       ~CgiReadPipe();

//       void onData(std::vector<char> const &data);
//       void onEof();
//       void onExcept();
//     };

//     class CgiWritePipe : public ftev::WritePipeProtocol {
//     private:
//       CgiProcess &_process;
//       ftev::WritePipeTransport *_writePipe;

//     public:
//       CgiWritePipe(CgiProcess &process, int fd);
//       ~CgiWritePipe();

//       ftev::WritePipeTransport &getTransport();
//       void onDrain();
//       void onExcept();
//     };

//   private:
//     CgiReadPipe *_readPipe;
//     CgiWritePipe *_writePipe;

//   public:
//     CgiProcess(ftev::EventLoop &loop, std::string const &bin,
//                DefaultTask &task);
//     ~CgiProcess();

//     void onExited(int status);
//     void onSignaled(int signum);
//     void onData(std::vector<char> const &data);
//     void onEof();
//   };

private:
  LocationDefault const &_location;
  std::string _path;
  void *_cgi;
  int _status;

public:
  DefaultTask(Connection::Cycle &cycle, LocationDefault const &location);
  ~DefaultTask();

  void onData(std::vector<char> const &data);
  void onEof();
  void onCancel();

  void onEofDefault();

  void sendAutoindex();
  void sendFile();
};
