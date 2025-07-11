/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:58:37 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:38:10 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/logger/Logger.hpp>

namespace ftpp {

Logger::Level const Logger::CRITICAL = 50;
Logger::Level const &Logger::FATAL = CRITICAL;
Logger::Level const Logger::ERROR = 40;
Logger::Level const Logger::WARNING = 30;
Logger::Level const &Logger::WARN = WARNING;
Logger::Level const Logger::INFO = 20;
Logger::Level const Logger::DEBUG = 10;
Logger::Level const Logger::NOTSET = 0;

static Logger::LevelNames _initLevelNames() {
  Logger::LevelNames levelNames;
  levelNames.insert(std::make_pair(Logger::CRITICAL, "CRITICAL"));
  levelNames.insert(std::make_pair(Logger::ERROR, "ERROR"));
  levelNames.insert(std::make_pair(Logger::WARNING, "WARNING"));
  levelNames.insert(std::make_pair(Logger::INFO, "INFO"));
  levelNames.insert(std::make_pair(Logger::DEBUG, "DEBUG"));
  levelNames.insert(std::make_pair(Logger::NOTSET, "NOTSET"));
  return levelNames;
}

Logger::LevelNames Logger::_levelNames = _initLevelNames();

Logger::Logger(Level level, std::ostream &ostream)
    : _level(level), _ostream(&ostream) {
}

Logger::Logger(Logger const &rhs) : _level(rhs._level), _ostream(rhs._ostream) {
}

Logger::~Logger() {
}

Logger &Logger::operator=(Logger const &rhs) {
  if (this != &rhs)
    Logger(rhs).swap(*this);
  return *this;
}

void Logger::swap(Logger &rhs) {
  std::swap(_level, rhs._level);
  std::swap(_ostream, rhs._ostream);
}

void Logger::setLevel(Level level) {
  _level = level;
}

void Logger::setStream(std::ostream &ostream) {
  _ostream = &ostream;
}

void Logger::addLevel(Level level, std::string const &name) {
  std::pair<LevelNames::iterator, bool> res =
      _levelNames.insert(std::make_pair(level, name));
  if (!res.second)
    res.first->second = name;
}

static char const *_colorEscape(Logger::Level level) {
  if (level >= Logger::CRITICAL)
    return "\x1b[91m";
  if (level >= Logger::ERROR)
    return "\x1b[31m";
  if (level >= Logger::WARNING)
    return "\x1b[33m";
  if (level >= Logger::INFO)
    return "\x1b[32m";
  if (level >= Logger::DEBUG)
    return "\x1b[34m";
  return "\x1b[0m";
}

static char const *_resetEscape = "\x1b[0m";

void Logger::operator()(Level level, std::string const &msg) {
  if (_level <= level && _ostream) {
    LevelNames::const_iterator it = _levelNames.upper_bound(level);
    std::string const &name =
        it == _levelNames.begin() ? "UNKNOWN" : (--it)->second;
#if defined(NOCOLOR)
    _ostream << "[" << name << "] " << msg << std::endl;
#else
    *_ostream << _colorEscape(level) << "[" << name << "] " << _resetEscape
              << msg << std::endl;
#endif
  }
}

#if defined(NDEBUG)
Logger logger(Logger::INFO);
#else
Logger logger(Logger::DEBUG);

#endif

} // namespace ftpp

namespace std {

void swap(ftpp::Logger& lhs, ftpp::Logger& rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
