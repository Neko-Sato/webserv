/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/25 04:55:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/UploadTask.hpp"
#include "Cycle.hpp"
#include "HttpException.hpp"
#include "constants.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/string/string.hpp>

#include <iterator>
#include <set>

UploadTask::UploadTask(Connection::Cycle &cycle, LocationUpload const &location)
    : Task(cycle), _location(location), _status(-1) {
  try {
    Request const &request = cycle.getRequest();
    if (request.method != "POST")
      throw HttpException(405);
    Headers::const_iterator it;
    if ((it = request.headers.find("content-type")) == request.headers.end())
      throw HttpException(400);
    std::multiset<std::string> params;
    ftpp::strsplit(it->second[0], ";", std::inserter(params, params.end()));
    {
      std::multiset<std::string> tmp;
      std::transform(params.begin(), params.end(),
                     std::inserter(tmp, tmp.end()), ftpp::strtrim);
      params.swap(tmp);
    }
    if (params.find("multipart/form-data") == params.end())
      throw HttpException(400);
    std::multiset<std::string>::iterator jt;
    if ((jt = params.lower_bound("boundary=")) == params.end())
      throw HttpException(400);
    if (jt->compare(0, 9, "boundary="))
      throw HttpException(400);
    _boundary = jt->substr(9);
  } catch (HttpException &e) {
    _status = e.getStatus();
  } catch (...) {
    _status = 500;
  }
}

UploadTask::~UploadTask() {
}

void UploadTask::onData(std::vector<char> const &data) {
  if (_status != -1)
    return;
  std::cout << _boundary << std::endl;
  _buffer.insert(_buffer.end(), data.begin(), data.end());
}

void UploadTask::onEof() {
  if (_status != -1) {
    cycle.sendErrorPage(_status);
    return;
  }
  Headers headers;
  headers["Location"].push_back(_location.getRedirect());
  cycle.send(303, headers);
  cycle.send(NULL, 0, false);
}
