/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 13:41:39 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/TaskUpload.hpp"
#include "HttpException.hpp"

#include <ftpp/pathlib/pathlib.hpp>
#include <ftpp/exceptions/OSError.hpp>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

TaskUpload::TaskUpload(WebservApp::Context const &ctx) : TaskStatic(ctx) {
}

TaskUpload::~TaskUpload() {
}

void TaskUpload::execute() {
  if (ctx.cycle.getRequest().method == "GET")
    defaultResponse();
  else if (ctx.cycle.getRequest().method == "POST")
    doPost();
  else if (ctx.cycle.getRequest().method == "DELETE")
    doDelete();
  else
    ctx.cycle.sendErrorPage(405);
}

#include <ftpp/string/string.hpp>
#include <ftpp/time/time.hpp>

void TaskUpload::doPost() {
  LocationUpload const &location =
      static_cast<LocationUpload const &>(ctx.location);
  std::string path;
  try {
    constructPath(location.getRoot(), ctx.path).swap(path);
    if (path == location.getRoot()) {
      tm time;
      time_t now = std::time(NULL);
      localtime_r(&now, &time);
      path = ftpp::strftime("%Y%m%d%H%M%S", time);
      path += "_";
      for (int i = 0; i < 6; ++i)
        path += ftpp::ASCII_ALNUM[random() % ftpp::ASCII_ALNUM.size()];
      path = ftpp::pathjoin(location.getRoot(), path);
    }
    try {
      int fd = -1;
      try {
        fd = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (fd == -1)
          throw ftpp::OSError(errno, "open");
        if (write(fd, ctx.data.data(), ctx.data.size()) == -1)
#if defined(FT_SUBJECT_NOT_COMPLIANT)
          throw ftpp::OSError(errno, "write");
#else
          throw std::runtime_error("write: No access to error details");
#endif
      } catch (...) {
        if (fd != -1)
          close(fd);
        throw;
      }
      close(fd);
    } catch (ftpp::OSError const &e) {
      switch (e.getErrno()) {
      case ENOENT:
        throw HttpException(404);
      case EACCES:
        throw HttpException(403);
      default:
        throw HttpException(500);
      }
    }
  } catch (HttpException const &e) {
    ctx.cycle.sendErrorPage(e.getStatus());
    return;
  }
  ctx.cycle.send(200, Headers());
  ctx.cycle.send(NULL, 0, false);
}

void TaskUpload::doDelete() {
  LocationUpload const &location =
      static_cast<LocationUpload const &>(ctx.location);
  std::string path;
  try {
    constructPath(location.getRoot(), ctx.path).swap(path);
    if (path == location.getRoot())
      throw HttpException(400);
    struct stat st;
    try {
      if (stat(path.c_str(), &st) == -1)
        throw ftpp::OSError(errno, "stat");
    } catch (ftpp::OSError const &e) {
      switch (e.getErrno()) {
      case ENOENT:
        throw HttpException(404);
      case EACCES:
        throw HttpException(403);
      default:
        throw HttpException(500);
      }
    }
    if (S_ISREG(st.st_mode)) {
      if (unlink(path.c_str()) == -1)
        throw HttpException(500);
    } else
      throw HttpException(403);
  } catch (HttpException const &e) {
    ctx.cycle.sendErrorPage(e.getStatus());
    return;
  }
  ctx.cycle.send(200, Headers());
  ctx.cycle.send(NULL, 0, false);
}
