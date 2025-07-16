/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskCgi.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 22:36:39 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/ProcessWatcher.hpp>
#include <ftev/Pipe/ReadPipe.hpp>
#include <ftev/Pipe/WritePipe.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

#include "locations/LocationCgi.hpp"
#include "tasks/TaskStatic.hpp"

class TaskCgi : public TaskStatic {
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
    public:
      enum State { HEADER, BODY };

    private:
      CgiManager &_manager;
      ftev::ReadPipeTransport *_transport;
      State _state;
      std::deque<char> _buffer;
      bool _bufferClosed;
      std::size_t _pos;
      typedef std::vector<std::string> HeaderValues;
      typedef std::map<std::string, HeaderValues> Headers;
      Headers _headers;

      void _process();
      bool _parseHeader();

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
    WebservApp::Context const &_ctx;
    LocationCgi::Cgi const &_cgi;
    std::string _path;
    std::string _pathInfo;
    Process *_process;
    ReadPipe *_readPipe;
    WritePipe *_writePipe;

    CgiManager();

  public:
    CgiManager(WebservApp::Context const &ctx, LocationCgi::Cgi const &cgi,
               std::string const &path, std::string const &pathInfo);
    ~CgiManager();
  };

private:
  CgiManager *_cgiManager;

public:
  TaskCgi(WebservApp::Context const &ctx);
  virtual ~TaskCgi();

  void execute();
  void caseFile(std::string const &path, std::string const &pathInfo = "");
  void caseAutoindex(std::string const &path);
};
