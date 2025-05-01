/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 02:40:32 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationDefault.hpp"
#include "tasks/Task.hpp"

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/ProcessWatcher.hpp>
#include <ftev/Pipe/ReadPipe.hpp>
#include <ftev/Pipe/WritePipe.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

class DefaultTask : public Task {
public:
  class CgiProcess : public ftev::EventLoop::ProcessWatcher {
  public:
    class CgiWritePipe : public ftev::WritePipeProtocol,
                         private ftpp::NonCopyable {
    private:
      CgiProcess &_process;
      ftev::WritePipeTransport *_transport;

    public:
      CgiWritePipe(CgiProcess &process, int fd);
      ~CgiWritePipe();

      ftev::WritePipeTransport &getTransport();
      void onDrain();
    };

    class CgiReadPipe : public ftev::ReadPipeProtocol,
                        private ftpp::NonCopyable {
    private:
      CgiProcess &_process;
      ftev::ReadPipeTransport *_transport;

    public:
      CgiReadPipe(CgiProcess &process, int fd);
      ~CgiReadPipe();

      void onData(std::vector<char> const &data);
      void onEof();
    };

  private:
    DefaultTask &_task;
    CgiWritePipe *_writePipe;
    CgiReadPipe *_readPipe;

  public:
    CgiProcess(ftev::EventLoop &loop, DefaultTask &task,
               LocationDefault::Cgi const &cgi);
    ~CgiProcess();

    void onExited(int status);
    void onSignaled(int signum);
    void onData(std::vector<char> const &data);
    void onEof();
  };

private:
  LocationDefault const &_location;
  std::string _path;
  CgiProcess *_cgi;
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
