/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskStatic.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 23:26:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/TaskStatic.hpp"
#include "HttpException.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/html/html.hpp>
#include <ftpp/pathlib/pathlib.hpp>
#include <ftpp/string/string.hpp>

#include <dirent.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

TaskStatic::TaskStatic(WebservApp::Context const &ctx) : Task(ctx) {
}

TaskStatic::~TaskStatic() {
}

void TaskStatic::execute() {
  if (ctx.cycle.getRequest().method != "GET")
    ctx.cycle.sendErrorPage(405);
  else
    defaultResponse();
}

void TaskStatic::defaultResponse() {
  try {
    std::string path, pathInfo;
    constructPath().swap(path);
    struct stat st;
    do {
      try {
        if (stat(path.c_str(), &st) == -1)
          throw ftpp::OSError(errno, "stat");
      } catch (ftpp::OSError const &e) {
        switch (e.getErrno()) {
        case ENOENT:
          throw HttpException(404);
        case ENOTDIR: {
          std::size_t pos = path.rfind('/');
          pathInfo += path.substr(pos);
          path.resize(pos);
        }
          continue;
        case EACCES:
          throw HttpException(403);
        default:
          throw HttpException(500);
        }
      }
    } while (false);
    if (S_ISDIR(st.st_mode))
      caseDirectory(path);
    else if (S_ISREG(st.st_mode))
      caseFile(path, pathInfo);
    else
      throw HttpException(403);
  } catch (HttpException &e) {
    ctx.cycle.sendErrorPage(e.getStatus());
  }
}

void TaskStatic::caseDirectory(std::string const &path) {
  LocationStatic const &location =
      static_cast<LocationStatic const &>(ctx.location);
  struct stat st;
  bool found = false;
  LocationStatic::IndexFiles const &index = location.getIndex();
  std::string indexPath;
  for (LocationStatic::IndexFiles::const_iterator it = index.begin();
       !found && it != index.end(); ++it) {
    ftpp::pathjoin(path, *it).swap(indexPath);
    try {
      if (stat(indexPath.c_str(), &st) == -1)
        throw ftpp::OSError(errno, "stat");
    } catch (ftpp::OSError const &e) {
      switch (e.getErrno()) {
      case ENOENT:
        continue;
      case EACCES:
        throw HttpException(403);
      default:
        throw HttpException(500);
      }
    }
    if (S_ISREG(st.st_mode))
      found = true;
  }
  if (found)
    caseFile(indexPath);
  else if (location.getAutoindex())
    caseAutoindex(path);
  else
    throw HttpException(403);
}

void TaskStatic::caseFile(std::string const &path,
                          std::string const &pathInfo) {
  if (!pathInfo.empty())
    ctx.cycle.sendErrorPage(403);
  else
    sendFile(path);
}

void TaskStatic::sendAutoindex(std::string const &path) {
  Headers headers;
  if (!ftpp::ends_with(ctx.cycle.getRequest().uri.getPath(),
                       std::string("/"))) {
    headers["location"].push_back(ctx.cycle.getRequest().uri.getPath() + "/");
    ctx.cycle.send(301, headers);
    ctx.cycle.send(NULL, 0, false);
    return;
  }
  std::ostringstream oss;
  std::string _path;
  ftpp::htmlEscape(ctx.cycle.getRequest().uri.getPath()).swap(_path);
  oss << "<html>";
  oss << "<head><title>Index of " << path << "</title></head>";
  oss << "<body>";
  oss << "<h1>Index of " << path << "</h1>";
  oss << "<hr><pre>";
  oss << "<a href=\"../ \">../</a>\n";
  DIR *dir = opendir(path.c_str());
  if (dir == NULL) {
    if (errno == ENOENT)
      throw HttpException(404);
    else if (errno == EACCES)
      throw HttpException(403);
    else
      throw HttpException(500);
  }
  try {
    struct dirent *entry;
    while ((entry = readdir(dir))) {
      if (entry->d_name[0] == '.')
        continue;
      struct stat st;
      if (stat(ftpp::pathjoin(path, entry->d_name).c_str(), &st) == -1)
        throw HttpException(500);
      std::string name = entry->d_name;
      if (S_ISDIR(st.st_mode))
        name += "/";
      oss << "<a href=\"" << ftpp::htmlEscape(name) << "\">";
      if (name.size() < 50)
        oss << ftpp::htmlEscape(name) << "</a>"
            << std::string(50 - name.size(), ' ');
      else
        oss << ftpp::htmlEscape(name.substr(0, 47)) << "..></a>";
      char timestamp[18];
      strftime(timestamp, sizeof(timestamp), "%d-%b-%Y %H:%M",
               localtime(&st.st_mtime));
      oss << " " << timestamp;
      if (S_ISDIR(st.st_mode))
        oss << std::setw(20) << "-";
      else
        oss << std::setw(20) << st.st_size;
      oss << "\n";
    }
  } catch (...) {
    closedir(dir);
    throw;
  }
  closedir(dir);
  oss << "</pre><hr>";
  oss << "</body>";
  oss << "</html>";
  std::string const &body = oss.str();
  headers["content-length"].push_back(ftpp::to_string(body.size()));
  ctx.cycle.send(200, headers);
  ctx.cycle.send(body.c_str(), body.size(), false);
}

void TaskStatic::caseAutoindex(std::string const &path) {
  sendAutoindex(path);
}

void TaskStatic::sendFile(std::string const &path) {
  int fd = open(path.c_str(), O_RDONLY);
  if (fd != -1) {
    try {
      Headers headers;
      ctx.cycle.send(200, headers);
      for (;;) {
        char buf[4096];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n == -1)
          throw std::runtime_error("read error");
        if (n == 0)
          break;
        ctx.cycle.send(buf, n, true);
      }
      ctx.cycle.send(NULL, 0, false);
    } catch (...) {
      close(fd);
      throw;
    }
    close(fd);
  } else if (errno == ENOENT)
    throw HttpException(404);
  else if (errno == EACCES)
    throw HttpException(403);
  else
    throw HttpException(500);
}

std::string TaskStatic::constructPath() const {
  Request const &request = ctx.cycle.getRequest();
  LocationStatic const &location =
      static_cast<LocationStatic const &>(ctx.location);
  std::string path;
  ftpp::normpath(ftpp::pathjoin(
      location.getRoot(),
      request.uri.getPath().substr(ctx.path.size(), std::string::npos))).swap(path);
  if (!ftpp::starts_with(path, location.getRoot()))
    throw HttpException(403);
  return path;
}
