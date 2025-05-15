/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/16 05:42:01 by hshimizu         ###   ########.fr       */
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

#include <vector>

class DefaultTask : public Task {
public:
  class CgiManager : private ftpp::NonCopyable {
  public:
    class Process : public ftev::EventLoop::ProcessWatcher {
    private:
      CgiManager &_manager;

    public:
      Process(ftev::EventLoop &loop, CgiManager &manager, int inputFd,
              int outputFd);
      ~Process();

      void onExited(int status);
      void onSignaled(int signum);
    };

    class ReadPipe : public ftev::ReadPipeProtocol, private ftpp::NonCopyable {
    private:
      CgiManager &_manager;
      ftev::ReadPipeTransport *_transport;

    public:
      ReadPipe(CgiManager &manager, int fd);
      ~ReadPipe();

      void onData(std::vector<char> const &data);
      void onEof();
    };

    class WritePipe : public ftev::WritePipeProtocol,
                      private ftpp::NonCopyable {
    private:
      CgiManager &_manager;
      ftev::WritePipeTransport *_transport;

    public:
      WritePipe(CgiManager &manager, int fd);
      ~WritePipe();

      void onDrain();
    };

  private:
    DefaultTask &_task;
    LocationDefault::Cgi const &_cgi;
    Process *_process;
    std::vector<char> _buffer;
    ReadPipe *_readPipe;
    WritePipe *_writePipe;

    CgiManager();

  public:
    CgiManager(DefaultTask &task, LocationDefault::Cgi const &cgi);
    ~CgiManager();

    void onData(std::vector<char> const &data);
    void onEof();
  };

private:
  LocationDefault const &_location;
  std::string _path;
  CgiManager *_cgiManager;
  int _status;

public:
  DefaultTask(Connection::Cycle &cycle, LocationDefault const &location);
  ~DefaultTask();

  void onData(std::vector<char> const &data);
  void onEof();

  void onEofDefault();

  void sendAutoindex();
  void sendFile();
};
