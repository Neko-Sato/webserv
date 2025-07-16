/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservApp.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 03:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/14 08:24:28 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Cycle.hpp"

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <typeinfo>
#include <vector>
#include <map>

class WebservApp : public Connection::App {
public:
  struct Context {
    Connection::Cycle &cycle;
    std::string const &path;
    std::vector<char> const &data;
    Location const &location;

    Context(Connection::Cycle &c, std::string const &p,
            std::vector<char> const &d, Location const &l);
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
  std::string const *_path;
  Location const *_location;
  std::vector<char> _buffer;
  Task *_task;

public:
  WebservApp(Connection::Cycle &cycle);
  ~WebservApp();

  void onData(std::vector<char> const &data);
  void onEof();
};
