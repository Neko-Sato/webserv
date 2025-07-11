/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:58:37 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:37:53 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>

// Not Thread Safe.

namespace ftpp {

class Logger {
public:
  typedef unsigned int Level;
  typedef std::map<Level, std::string> LevelNames;

  static Level const CRITICAL;
  static Level const &FATAL;
  static Level const ERROR;
  static Level const WARNING;
  static Level const &WARN;
  static Level const INFO;
  static Level const DEBUG;
  static Level const NOTSET;

private:
  static LevelNames _levelNames;

  Level _level;
  std::ostream *_ostream;

public:
  Logger(Level level = 0, std::ostream &ostream = std::cerr);
  Logger(Logger const &rhs);
  ~Logger();
  Logger &operator=(Logger const &rhs);

  void swap(Logger &rhs);

  void setLevel(Level level);
  void setStream(std::ostream &ostream);

  static void addLevel(Level level, std::string const &name);

  void operator()(Level Level, std::string const &msg);
};

extern Logger logger;

} // namespace ftpp


namespace std {

void swap(ftpp::Logger& lhs, ftpp::Logger& rhs) throw();

} // namespace std
