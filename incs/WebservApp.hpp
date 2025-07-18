/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservApp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 03:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 11:47:22 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Cycle.hpp"

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <map>
#include <typeinfo>
#include <vector>

class WebservApp : public Connection::App {
public:
  struct Context {
    Connection::Cycle &cycle;
    std::vector<char> const &data;
    std::string const &path;
    Location const &location;

    Context(Connection::Cycle &c, std::vector<char> const &d,
            std::string const &p, Location const &l);
  };

  class Task : private ftpp::NonCopyable {
  public:
    typedef Task *(*Factory)(Context const &ctx);
    typedef std::map<std::type_info const *, Factory> Factories;
    static Factories factories;
    static Task *create(Context const &ctx);

    Context ctx;

  private:
    Task();

  protected:
    Task(Context const &ctx);

  public:
    virtual ~Task();

    virtual void execute() = 0;
  };

private:
  int _status;
  std::size_t _maxBodySize;
  std::string _path;
  ServerConf::Locations::const_iterator _location;
  std::vector<char> _buffer;
  Task *_task;

public:
  WebservApp(Connection::Cycle &cycle);
  ~WebservApp();

  void onData(std::vector<char> const &data);
  void onEof();
};
